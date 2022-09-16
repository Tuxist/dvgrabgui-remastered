/*!
 * @file dvgrab.cpp
 * @brief DVGrabGUI capture thread class implementation.
 * @author Bastien Montagne \htmlonly(<a href="mailto:montagne29@wanadoo.fr">montagne29@wanadoo.fr</a>)\endhtmlonly
 * @author Based on dvgrab, by Dan Dennedy and the Kino team.
 * @version 0.9.4
 * @date 2006, 2007, 2008
 */

/* Copyright (C) 2006, 2007, 2008 Bastien Montagne <montagne29@wanadoo.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "dvgrab.h"

// Various controls on VCR fonctionality of ieee1394 davices.
#include <libavc1394/avc1394.h>
#include <libavc1394/avc1394_vcr.h>
#include <libavc1394/rom1394.h>

//#include "frame.h"			//!< Dv frame class.
#include "raw1394util.h"	//!< Miscellaneous functions.
//#include "smiltime.h"

Frame *DvGrabThread::m_frame;
FileHandler *DvGrabThread::m_writer;

//! Constructor
/*!
 * Just do basic initialization, real work is done in Create().
 * @param *m Pointer on general purpose mutex.
 * @param *mE Pointer on mutex protecting DvGrabThread::m_threadEnd.
 * @param *cd Pointer on condition for waiting the real end of this thread.
 * @param *cth Pointer on ThreadInOut communication object.
 */
DvGrabThread::DvGrabThread(wxMutex *m, wxMutex *mE, wxCondition *cd,
                           ThreadInOut *cth) : wxThread(),
                           m_prevCapture(false), m_normTerm(false), m_readerActive(false),
                           m_sizeSplitMode(0), m_openDML(false), m_interactive(false),
                           m_captureActive(false)
{
	m_mutex = m;
	m_mutexEnd = mE;
	m_threadEnd = cd;
	m_cth = cth;
	m_c = NULL;

	m_connection = NULL;
	m_avc = NULL;
	m_frame = NULL;
	m_reader = NULL;
	m_writer = NULL;
	m_timeDuration = NULL;

	Create();
}

//! Real initialization.
/*!
 * Search dv device and prepare reader.
 */
void DvGrabThread::Create()
{
	wxThread::Create();

	m_mutex->Lock();
	if(!m_cth->m_noAVC || m_cth->m_port == -1)
		m_cth->m_node = discoverAVC(&m_cth->m_port, &m_cth->m_guid);

	if(m_interactive && (!m_cth->m_noAVC && m_cth->m_node == -1))
		m_cth->SetMessage(ERROR, _("DVG-CPT: no camera exists"));

	if(m_cth->m_port != -1)
	{
		iec61883Connection::CheckConsistency(m_cth->m_port, m_cth->m_node);

		if(!m_cth->m_noAVC)
		{
			m_avc = new AVC(m_cth->m_port);
			if(!m_avc)
				m_cth->SetMessage(ERROR, _("DVG-CPT: failed to initialize AV/C"));
			if(m_interactive)
				m_avc->Pause(m_cth->m_node);
		}

		if(m_cth->m_guid)
		{
			m_connection = new iec61883Connection(m_cth->m_port, m_cth->m_node);
			if(!m_connection)
				m_cth->SetMessage(ERROR, _("DVG-CPT: failed to establish isochronous connection"));
			m_cth->m_channel = m_connection->GetChannel();
			m_cth->SetMessage(INFO,
			                  wxString::Format(_("DVG-CPT: Established connection over channel %d"),
			                                   m_cth->m_channel));
		}
		m_reader = new iec61883Reader(m_cth->m_port, m_cth->m_channel,
		                              m_cth->m_buffers, this->testCaptureProxy, this );
	}
	else
		m_cth->SetMessage(ERROR, _("DVG-CPT: invalid DV source specified"));
	m_mutex->Unlock();
}

//! Function managing the end of the thread.
/*!
 * Send an event to main frame, indicating if this was a normal or error termination.
 * Then signal (via DvGrabThread::m_threadEnd) the effective end of this thread.
 */
void DvGrabThread::OnExit()
{
	wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, THREAD_TERM);
	if(m_normTerm)
		event.SetInt(1);
	else
		event.SetInt(0);
	m_mutex->Lock();
	wxPostEvent(m_cth->m_frame, event);
	m_mutex->Unlock();

	m_threadEnd->Signal();
}

//! The function called when running the thread (i.e. a sort of 'main').
/*!
 * The main function of this thread.
 * Finish initialization, and start the main loop, run each time a new frame is
 * received by the reader object.
 * @return Not used here, exists only for @a attached threads.
 */
void *DvGrabThread::Entry()
{
	m_mutex->Lock();
	// Control if reader is OK.
	if(!m_reader)
	{
		m_cth->SetMessage(ERROR, _("DVG-CPT: DV reader couldn't be initialized"));
		m_mutex->Unlock();
		cleanup();
		return NULL;
	}
	// Start and control if reader thread is OK.
	if(!m_reader->StartThread())
	{
		m_cth->SetMessage(ERROR, _("DVG-CPT: Couldn't start reader thread"));
		m_mutex->Unlock();
		cleanup();
		return NULL;
	}

	m_lockPending = true;
	m_prevTimeCode.sec = -1;
	m_readerActive = true;

	m_cth->m_totalDroppedFrames = 0;
	m_cth->m_totalFrames = 0;

	// m_c stores the wxImage data.
	m_c = m_cth->m_dvFrame.GetData();
	for(int i = 0; i < 1244160; i++)
		m_c[i] = 0;



	rom1394_directory rom_dir;
	raw1394handle_t handle;
	int i, j = 0;
	int m = raw1394_get_num_ports();
	m_cth->m_devicesCap.Empty();
	for( ; j < m; j++)
	{
		handle = raw1394_open(j);
		for(i = 0; i < raw1394_get_nodecount(handle); ++i)
		{
			wxString t(wxString::Format(_T("Port %d, node %d :\n"), j, i));
			if(rom1394_get_directory(handle, i, &rom_dir) < 0)
			{
				t += wxString(_T("error reading config rom directory"));
				m_cth->m_devicesCap.Add(t);
				continue;
			}
			t += wxString(avc1394_vcr_decode_status(avc1394_vcr_status(handle, i)), wxConvLocal, wxString::npos);
			t += wxString(_T("\n\t"));
			t += wxString(_T("node type: "));
			switch(rom1394_get_node_type(&rom_dir))
			{
				case ROM1394_NODE_TYPE_AVC:
					t += wxString(_T("AVC"));
					break;
				case ROM1394_NODE_TYPE_DC:
					t += wxString(_T("DC"));
					break;
				case ROM1394_NODE_TYPE_SBP2:
					t += wxString(_T("SBP2"));
					break;
				case ROM1394_NODE_TYPE_CPU:
					t += wxString(_T("CPU"));
					break;
				case ROM1394_NODE_TYPE_UNKNOWN:
					t += wxString(_T("UNKNOWN"));
			}
			t += wxString(_T("\n\t"));
			t += wxString(rom_dir.label, wxConvLocal, wxString::npos);
			m_cth->m_devicesCap.Add(t);

			rom1394_free_directory(&rom_dir);
		}
		raw1394_close( handle );
	}


	// This is a html file storing all Video AUXiliary data of dv stream.
	wxTextFile vaux(_T("./VAUX.html"));
	if(m_cth->m_recVAUX)
	{
		if(vaux.Exists())
			vaux.Open();
		else
			vaux.Create();
	}
	m_mutex->Unlock();

	m_normTerm = true;

	// Loop until we're informed otherwise (i.e. main thread stops this one).
	while(!TestDestroy())
	{
		TimeCode timeCode;
		struct tm recDate;

		// Wait (not more than 5s.) for the reader to indicate that something has happened.
		if(!m_reader->WaitForAction(5))
		{
			m_mutex->Lock();
			m_cth->SetMessage(ERROR, _("DVG-CPT: Tired waiting reader!! (> 5s)"));
			m_mutex->Unlock();
			m_normTerm = false;
			break;
		}

		int dropped = m_reader->GetDroppedFrames();
		int incomplete = m_reader->GetIncompleteFrames();

		// Get the next frame
		m_frame = m_reader->GetFrame();

		// Start or halt the dv capture, if there is a difference between
		// m_cth->m_capture and m_cth->m_prevCapture.
		m_mutex->Lock();
		if(m_cth->m_capture && !m_prevCapture)
			startCapture();
		else if(!m_cth->m_capture && m_prevCapture)
			stopCapture();
		m_prevCapture = m_cth->m_capture;
		m_mutex->Unlock();

		// Check if the out queue is falling behind
//		bool critical_mass = m_reader->GetOutQueueSize( ) > m_reader->GetInQueueSize( );

		if(m_frame != NULL)
		{
			// Get timecode and recording date/time
			m_frame->GetTimeCode(timeCode);
			if(!m_frame->GetRecordingDate(recDate))
			{
				// If the month is invalid, then report system date/time
				time_t timesys;
				time(&timesys);
				localtime_r(&timesys, &recDate);
			}

			// Handle exceptional situations
			if(dropped > 0)
			{
				m_mutex->Lock();
				m_cth->m_droppedFrames += dropped;
				m_cth->m_totalDroppedFrames += dropped;
				m_cth->SetMessage(WARNING, wxString::Format(_("DVG-CPT: %d dropped frames: "
				                                              "buffer underrun near:"), dropped));
				m_cth->SetMessage(WARNING, wxString::Format(_("timecode: %2.2d:%2.2d:%2.2d.%2.2d "
				                                              "date %4.4d.%2.2d.%2.2d %2.2d:%2.2d:%2.2d "
				                                              "\nThis error means that the frame could "
				                                              "not be written fast enough!"),
				                                            timeCode.hour, timeCode.min, timeCode.sec,
				                                            timeCode.frame, recDate.tm_year + 1900,
				                                            recDate.tm_mon + 1, recDate.tm_mday,
				                                            recDate.tm_hour, recDate.tm_min, recDate.tm_sec));
				m_cth->SetMessage(WARNING, _("The frame(s) couldn't be written fast enough."));

				// If we are using lockstep feature, and there are too much
				// dropped frames, stop capture (by setting m_lockPending flag).
				if(m_cth->m_lockstep && m_prevCapture)
				{
					if(m_writer->FileIsOpen())
					{
						if((m_cth->m_lockstepMaxDrops > -1
						    && dropped > m_cth->m_lockstepMaxDrops)
						   ||(m_cth->m_lockstepTotalDrops > -1
						    && m_cth->m_droppedFrames > m_cth->m_lockstepTotalDrops))
						{
							m_cth->SetMessage(WARNING, _("DVG-CPT: Closing file early "
							                             "due to too many dropped frames"));
							m_lockPending = true;
						}
						for(int n = 0; n < dropped; n++)
							writeFrame( timeCode, recDate );
					}
				}
				m_mutex->Unlock();
			}

			if(!m_frame->IsComplete() || incomplete > 0)
			{
				m_mutex->Lock();
				if(!m_frame->IsComplete())
					incomplete ++;
				m_cth->m_droppedFrames += incomplete;
				m_cth->m_totalDroppedFrames += incomplete;
				m_cth->SetMessage(WARNING, wxString::Format(_("DVG-CPT: %d frames dropped near:"),
				                                            incomplete));
				m_cth->SetMessage(WARNING, wxString::Format(_("timecode %2.2d:%2.2d:%2.2d.%2.2d "
				                                              "date %4.4d.%2.2d.%2.2d %2.2d:%2.2d:%2.2d"),
				                                            timeCode.hour, timeCode.min,
				                                            timeCode.sec, timeCode.frame,
				                                            recDate.tm_year + 1900, recDate.tm_mon + 1,
				                                            recDate.tm_mday, recDate.tm_hour,
				                                            recDate.tm_min, recDate.tm_sec));
				m_cth->SetMessage(WARNING, _("The ieee1394 driver received an"
				                             "(some) incomplete frame(s)"));

				if(m_cth->m_lockstep && m_prevCapture)
				{
					if(m_writer->FileIsOpen())
					{
						if((m_cth->m_lockstepMaxDrops > -1
						    && incomplete > m_cth->m_lockstepMaxDrops)
						   ||(m_cth->m_lockstepTotalDrops > -1
						    && m_cth->m_droppedFrames > m_cth->m_lockstepTotalDrops))
						{
							m_cth->SetMessage(WARNING, _("DVG-CPT: closing file early due "
							                             "to too many incomplete frames"));
							m_lockPending = true;
						}
						if(!m_frame->IsComplete())
							writeFrame(timeCode, recDate);
						for(int n = 0; n < incomplete; n++)
							writeFrame(timeCode, recDate);
					}
				}
				m_mutex->Unlock();
			}

			else
			{
				// Actually write frame unless not normal speed
				if(m_frame->IsNormalSpeed())
				{
					m_mutex->Lock();
					if(m_cth->m_show)
					{
/*						if(!(m_cth->m_totalFrames%25))
						{
							char* c = m_avc->ReadableTransportStatus(m_cth->m_node);
							m_cth->m_cameraStatus = wxString(c, wxConvLocal, wxSTRING_MAXLEN);
						}*/
						if(!(m_cth->m_totalFrames%m_cth->m_showEvery))
						{
							m_frame->ExtractRGB((void*)m_c);
/*							if(m_avc->IsRecording(m_cth->m_node))
								m_cth->m_isRecording = _T("YES");
							else
								m_cth->m_isRecording = _T("NO");*/
						}
					}
//------------------PAL only!
					// Store VAUX data of the frame.
					if(m_cth->m_recVAUX)
					{
						Pack p;
						vaux.AddLine(_T("<H2>Frame</H2>"));
						for(int i = 0; i < 12; i++)
						{
							vaux.AddLine(wxString::Format(_T("<table><caption><H3>Sequence %d:</H3></caption>\n<tr>"), i+1));
							for(int j = 0; j < 3; j++)
							{
								vaux.AddLine(wxString::Format(_T("\t<td><table border=\"1\"><caption><H4>VAUX DIF Block %d</H4></caption>"), j+1));
								vaux.AddLine(_T("\t\t<tr><td>VAUX</td><td>1</td><td>2</td><td>3</td><td>4</td></tr>"));
								for(int k = 0; k < 15; k++)
								{
									int id = i * 150 * 80 + 3 * 80 + j * 80 + 3 + k * 5;
									m_frame->GetVAUXPack(id, p);
									wxString s(wxString::Format(_T("\t\t<tr><td>%d</td>"), k+1));
//									s += (wxString::Format(_T("\t\t<td>Ox%5.5lx</td>"), id));
									s += (wxString::Format(_T("\t\t<td>Ox%2.2lx; %3.3d</td>"), p.data[1], p.data[1]));
									s += (wxString::Format(_T("\t\t<td>Ox%2.2lx; %3.3d</td>"), p.data[2], p.data[2]));
									s += (wxString::Format(_T("\t\t<td>Ox%2.2lx; %3.3d</td>"), p.data[3], p.data[3]));
									s += (wxString::Format(_T("\t\t<td>Ox%2.2lx; %3.3d</td></tr>"), p.data[4], p.data[4]));
									vaux.AddLine(s);
								}
								vaux.AddLine(_T("\n\t</table></td>"));
							}
							vaux.AddLine(_T("\n</tr></table>"));
						}
					}
					if(m_prevCapture)
						writeFrame(timeCode, recDate);

					m_cth->m_totalFrames++;
					m_mutex->Unlock();
				}

				// drop frame on stdout if getting low on buffers
/*				if ( !critical_mass && m_raw_pipe )
				{
					fd_set wfds;
					struct timeval tv =
					    {
						    0, 20000
					    };
					FD_ZERO( &wfds );
					FD_SET( fileno( stdout ), &wfds );
					if ( select( fileno( stdout ) + 1, NULL, &wfds, NULL, &tv ) )
					{
						write( fileno( stdout ), m_frame->data, m_frame->GetFrameSize() );
					}
				}
*/
			}
			m_reader->DoneWithFrame(m_frame);
			m_prevTimeCode.sec = timeCode.sec;

			// Send event to main frame.
			wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, THREAD_UPDATE);
			m_mutex->Lock();
			if(m_cth->m_frame)
				wxPostEvent(m_cth->m_frame, event);
			m_mutex->Unlock();
		}
		else
		{
			m_mutex->Lock();
			// Reader has erred or signaling a stop condition (end of pipe).
			m_cth->SetMessage(ERROR, _("DVG-CPT: reader has erred or signaling "
			                           "a stop condition (end of pipe): stoping capture thread"));
			m_mutex->Unlock();
			m_normTerm = false;
			break;
		}
	}
	vaux.Write();
	m_readerActive = false;
	cleanup();
	return NULL;
}

//! Function starting the dv capture.
/*!
 * Create and initialize the appropriate file handler (avi, raw dv, jpg, etc.),
 * and play "beep" sound.
 * @return TRUE everything is OK, FALSE otherwise.
 */
bool DvGrabThread::startCapture()
{
	int tryCounter = 400;

	wxString t = m_cth->m_path + (m_cth->m_path.Last() == _T('/') ? _T("") : _T("/")) + m_cth->m_file;
	if((char*)(const char*)t.mb_str(wxConvLibc))
	{
		switch(m_cth->m_fileFormat)
		{
		case RAW_FORMAT:
			m_writer = new RawHandler();
			break;

		case DIF_FORMAT:
			m_writer = new RawHandler(".dif");
			break;

		case AVI_DV1_FORMAT:
			{
				AVIHandler *aviWriter = new AVIHandler(AVI_DV1_FORMAT);
				m_writer = aviWriter;
				break;
			}

		case AVI_DV2_FORMAT:
			{
				AVIHandler *aviWriter = new AVIHandler(AVI_DV2_FORMAT);
				m_writer = aviWriter;
				if (m_cth->m_maxFileSize == 0 || m_cth->m_maxFileSize > 1000)
				{
					m_cth->SetMessage(INFO, _("DVG-CPT: turning on OpenDML to "
					                          "support large file size."));
					m_openDML = true;
				}
				aviWriter->SetOpenDML(m_openDML);
				break;
			}

// Not used!
#ifdef HAVE_LIBQUICKTIME
		case QT_FORMAT:
			m_writer = new QtHandler();
			break;
#endif	// HAVE_LIBQUICKTIME

#if defined(HAVE_LIBJPEG) && defined(HAVE_LIBDV)
		case JPEG_FORMAT:
			m_writer = new JPEGHandler(m_cth->m_jpegQuality, m_cth->m_jpegDeinterlace,
			                           m_cth->m_jpegWidth, m_cth->m_jpegHeight,
			                           m_cth->m_jpegOverwrite );
			break;
#endif	//defined(HAVE_LIBJPEG) && defined(HAVE_LIBDV)
		}
		m_writer->SetTimeStamp(m_cth->m_timestamp);
		m_writer->SetTimeSys(m_cth->m_timesys);
		m_writer->SetTimeCode(m_cth->m_timecode);
		m_writer->SetBaseName((char*)(const char*)t.mb_str(wxConvLibc));
		m_writer->SetMaxFrameCount(m_cth->m_frameCount);
		m_writer->SetAutoSplit(m_cth->m_autosplit);
		m_writer->SetEveryNthFrame(m_cth->m_frameEvery);
		m_writer->SetMaxFileSize((off_t) m_cth->m_maxFileSize * (off_t) (1024 * 1024));
		if (m_cth->m_collectionSize)
			m_sizeSplitMode = 1;
		m_writer->SetSizeSplitMode(m_sizeSplitMode);
		m_writer->SetMaxColSize((off_t) (m_cth->m_collectionSize) * (off_t) (1024 * 1024));
		m_writer->SetMinColSize((off_t) (m_cth->m_collectionSize - m_cth->m_collectionMinCutFileSize)
		                        * (off_t) (1024 * 1024));
	}
	if(m_avc)
		m_avc->Play(m_cth->m_node);

	while(m_frame == NULL && tryCounter-- > 0 )
	{
		Sleep(25);
	}

	if(m_frame)
	{
		m_cth->SetMessage(INFO, _("DVG-CPT: capture started."));
		m_captureActive = true;
		m_cth->m_captFrames = 0;
		m_cth->m_droppedFrames = 0;

		// parse the SMIL time value duration
		if (m_timeDuration == NULL && ! m_cth->m_duration.empty())
			m_timeDuration = new SMIL::MediaClippingTime(m_cth->m_duration, m_frame->GetFrameRate());

		return true;
	}
	else
	{
		m_cth->SetMessage(ERROR, _("DVG-CPT: no DV."));
		return false;
	}
}

//! Stop dv capture
/*!
 * Close and delete file handler (DvGrabThread::m_writer), and set INFO messages.
 * Also stop playing "beep" sound.
 */
void DvGrabThread::stopCapture()
{
	if (m_writer != NULL)
	{
		std::string filename = m_writer->GetFileName();
//		int frames = m_writer->GetFramesWritten();
		m_cth->m_sizeFiles = (float) m_writer->GetFileSize() / 1024 / 1024;

		m_writer->Close();
		delete m_writer;
		m_writer = NULL;
		if (m_avc && m_interactive)
			m_avc->Pause(m_cth->m_node);
		m_cth->SetMessage(INFO, _("DVG-CPT: capture stopped."));
		m_cth->SetMessage(INFO, wxString::Format(_("%8.2f Mo, %d frames written"),
		                  m_cth->m_sizeFiles, m_cth->m_captFrames));
		if (m_frame != NULL)
		{
			TimeCode timeCode;
			struct tm recDate;
			m_frame->GetTimeCode(timeCode);
			if (!m_frame->GetRecordingDate(recDate))
			{
				// If the month is invalid, then report system date/time
				time_t timesys;
				time(&timesys);
				localtime_r(&timesys, &recDate);
			}
			m_cth->SetMessage(INFO, wxString::Format(_("timecode: %2.2d:%2.2d:%2.2d.%2.2d\n"
			                                           "date: %4.4d.%2.2d.%2.2d %2.2d:%2.2d:%2.2d"),
			                                         timeCode.hour, timeCode.min, timeCode.sec, timeCode.frame,
			                                         recDate.tm_year+1900, recDate.tm_mon+1, recDate.tm_mday,
			                                         recDate.tm_hour, recDate.tm_min, recDate.tm_sec));
		}
		if (m_cth->m_droppedFrames > 0)
			m_cth->SetMessage(WARNING, wxString::Format(_("DVG-CPT: %d dropped frames in this capture"),
			                                            m_cth->m_droppedFrames));
		m_cth->m_droppedFrames = 0;
		m_cth->m_captFrames = 0;
		m_cth->m_capture = false;
		m_captureActive = false;
	}
}

//! Write a frame in the video file.
/*!
 * First do controls about lenght of the clip, and new clip detection.
 * Then manage DvGrabThread::m_lockPending flag.
 * And finaly write the frame.
 * @param timeCode The time-code of the current frame.
 * @param recDate The date of the recording of current dv clip.
 */
void DvGrabThread::writeFrame(TimeCode& timeCode, struct tm& recDate)
{
	// see if we have exceeded requested duration
	m_cth->m_captFrames++;
/*	if(m_timeDuration && m_timeDuration->isResolved() &&
	   ((float) m_cth->m_captFrames / m_frame->GetFrameRate() * 1000.0 + 0.5 ) >=
	   m_timeDuration->getResolvedOffset())
	{
		stopCapture();
		m_readerActive = false;
	}
*/
	if(m_cth->m_frameCount)
	{
		if(m_cth->m_captFrames >= m_cth->m_frameCount)
		{
			stopCapture();
			m_readerActive = false;
			m_cth->SetMessage(CAPT_STOP, _T(""));
		}
	}
	else if(m_writer != NULL)
	{
		// This check to split off a new file must occur outside the FileHandler
		// so the new frame is a part of the new file and not the old one
		// It checks for the new recording flag set in the DV stream as well as for
		// timecode discontinuity in seconds.
		int time_diff = timeCode.sec - m_prevTimeCode.sec;
		bool discontinuity = m_prevTimeCode.sec != -1 && (time_diff > 1 || (time_diff < 0 && time_diff > -59));
		m_cth->m_sizeFiles = (float) m_writer->GetFileSize() / 1024 / 1024;

		if(m_writer->FileIsOpen() && m_cth->m_autosplit && (m_frame->IsNewRecording() || discontinuity))
		{
			m_writer->CollectionCounterUpdate();
			m_writer->Close();
			sendCaptureStatus(timeCode, recDate);
			m_lockPending = true;
		}

		if(m_cth->m_lockstep && m_lockPending)
		{
			// If a lock is pending due to dropped frames, close the file
			// TODO: still needed?
			if (m_writer->FileIsOpen())
			{
				m_writer->CollectionCounterUpdate();
				m_writer->Close();
				sendCaptureStatus(timeCode, recDate);
			}

			// Convert DV timecode to #frames
/*			SMIL::MediaClippingTime mcTime( m_frame->GetFrameRate() );
			std::ostringstream sb;
			sb << setfill( '0' ) << std::setw( 2 )
			<< timeCode.hour << ':' << timeCode.min << ':'
			<< timeCode.sec << ':' << timeCode.frame;
			if ( m_frame->IsPAL() )
				mcTime.parseSmpteValue( sb.str() );
			else
				mcTime.parseSmpteNtscDropValue( sb.str() );

			// If lock step point (multiple of frame count) is reached, skip writing
			if ( mcTime.getFrames() % m_frame_count != 0 )
			{
//				pthread_mutex_unlock( &capture_mutex );
				return;
			}
*/			m_lockPending = false;
		}

		// Set the sample frame here to ensure we have a good (normal speed)
		// frame for setting audio stream format. Also, each recording could
		// have different params.
		if(!m_writer->FileIsOpen())
		{
			m_writer->SetSampleFrame(*m_frame);
		}

		if(!m_writer->WriteFrame(*m_frame))
		{
			stopCapture();
			m_cth->SetMessage(ERROR, _("DVG-CPT: writing failed."));
		}

		// The file will be closed by FileHandler if max size or frames are exceeded
		if(!m_writer->FileIsOpen())
		{
			sendCaptureStatus(timeCode, recDate);
			m_lockPending = true;
		}
	}
}

//! Posix thread stuff?
/*!
 * I have to say that I don't fully understand this (not enought knowledge of
 * posix threads, I think!).
 * Anyway, I should on file handlers to use wxThread too, this would be much more consistent, I think.
 * @param arg A DvGrabThread void pointer.
 */
void DvGrabThread::testCaptureProxy(void* arg)
{
	DvGrabThread *self = static_cast<DvGrabThread*>(arg);
	if(self)
		self++;
//	self->testCapture();
}

//! Just set a message to main frame about time-code and date of current frame.
/*!
 * @param timeCode The time-code of current frame.
 * @param recDate The date of recording of the current dv clip.
 */
void DvGrabThread::sendCaptureStatus(TimeCode& timeCode, struct tm& recDate)
{
	m_cth->SetMessage(INFO, wxString::Format(_("DVG-CPT: %8.2f Mo, %d frames written\n"
	                                           "timecode %2.2d:%2.2d:%2.2d.%2.2d \n"
	                                           "date %4.4d.%2.2d.%2.2d %2.2d:%2.2d:%2.2d"),
	                                         m_cth->m_sizeFiles, m_cth->m_captFrames,
	                                         timeCode.hour, timeCode.min, timeCode.sec, timeCode.frame,
	                                         recDate.tm_year + 1900, recDate.tm_mon + 1, recDate.tm_mday,
	                                         recDate.tm_hour, recDate.tm_min, recDate.tm_sec ));
}

//! To know if dv device (camera) is plying.
/*!
 * @return TRUE if dv device is playing.
 */
bool DvGrabThread::isPlaying()
{
	if(!m_avc)
		return false;
	m_mutex->Lock();
	unsigned int status = m_avc->TransportStatus(m_cth->m_node);
	m_mutex->Unlock();
	quadlet_t resp2 = AVC1394_MASK_RESPONSE_OPERAND(status, 2);
	quadlet_t resp3 = AVC1394_MASK_RESPONSE_OPERAND(status, 3);
	return ((resp2 == AVC1394_VCR_RESPONSE_TRANSPORT_STATE_PLAY && resp3 != AVC1394_VCR_OPERAND_PLAY_FORWARD_PAUSE) ||
	        (resp2 == AVC1394_VCR_RESPONSE_TRANSPORT_STATE_RECORD && resp3 != AVC1394_VCR_OPERAND_RECORD_PAUSE));
}

//! Just a timer? Doesn't seems to be used!
bool DvGrabThread::done()
{
	if(m_readerActive)
	{
		timespec t = {0, 25000000};
		return (nanosleep(&t, NULL) == -1);
	}
	return true;
}

//! Cleaning up things before ending the thread
/*!
 * Stop capture, stop camera reading if available, stop and delete reader, etc.
 */
void DvGrabThread::cleanup()
{
	m_mutex->Lock();
	stopCapture();
	if(m_avc && !m_cth->m_noStop)
		m_avc->Stop(m_cth->m_node);
	m_mutex->Unlock();
	m_readerActive = false;
	if(m_reader)
	{
		m_reader->StopThread();
		delete m_reader;
	}
	if(m_avc)
		delete m_avc;
	if(m_connection)
		delete m_connection;
	if(m_timeDuration)
		delete m_timeDuration;
}
