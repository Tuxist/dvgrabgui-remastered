/*!
 * @file threadinout.h
 * @brief Objects for communication between main thread (i.e. class TheApp) and capture thread.
 * @author Bastien Montagne @htmlonly(<a href="mailto:montagne29@wanadoo.fr">montagne29@wanadoo.fr</a>)@endhtmlonly
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

#ifndef THREAD_COMM_H_
#define THREAD_COMM_H_

#include "includes.h"		//!< General includes.

/*!
 * @class ThreadMessage
 * @brief Class managing the messages from capture thread to main thread
 */
class ThreadMessage
{
public:
	//! Constructor, allocating memory for 20 messages.
	ThreadMessage()
	{
		m_message.Alloc(20*sizeof(wxString));
		m_type.Alloc(20*sizeof(int));
	}
	//! More easy to have one function for erasing all messages!
	void Empty()
	{
		m_message.Empty();
		m_type.Empty();
	}
public:
	wxArrayString m_message; //!< The texts of the messages, drawn in the m_textError control.
	wxArrayInt m_type;       //!< The types of the messages (i.e. error, warning or info).
};

/*!
 * @class ThreadInOut
 * @brief Class managing all datas shared between main thread and capture thread.
 */
class ThreadInOut
{
public:
	//! Constructor, seting all parameters to default values.
	/*! @param *f The main window (used by capture thread when sending events).*/
	ThreadInOut(wxFrame *f) :
		m_dvFrame(720, 576), m_capture(false), m_showEvery(25), m_show(true),
		m_beep(true), m_recVAUX(false), m_port(-1), m_backPort(-1), m_node(-1),
		m_backNode(-1), m_rawPipe(false), m_guid(0), m_backGuid(0),
		m_channel(-1), m_backChannel(-1), m_buffers(250), m_noAVC(false),
		m_noStop(false), m_path(_T("")), m_file(_T("")), m_frameEvery(1),
		m_frameCount(0), m_duration(""), m_maxFileSize(1024),
		m_collectionSize(0), m_collectionMinCutFileSize(1),
		m_autosplit(false), m_lockstep(false), m_lockstepMaxDrops(-1),
		m_lockstepTotalDrops(-1), m_timestamp(false), m_timecode(false),
		m_timesys(false), m_fileFormat(AVI_DV2_FORMAT), m_jpegQuality(90),
		m_jpegDeinterlace(false), m_jpegWidth(-1), m_jpegHeight(-1),
		m_jpegOverwrite(false), m_totalDroppedFrames(0), m_droppedFrames(0),
		m_totalFrames(0), m_captFrames(0), m_sizeFiles(0.0),
		m_cameraStatus(_T("No status now!")), m_isRecording(_T("Recording ???"))
	{
		m_frame = f;
		m_running = false;
		m_change_settings_pending = false;
	}
	//! A convinient function for putting quickly a message in the array.
	/*! @param type The type of the message (either ERROR, WARNING, INFO, CAPT_STOP).
	 * @param s The text of the message.*/
	void SetMessage(int type, wxString s)
	{
		m_threadMessage.m_message.Add(s);
		m_threadMessage.m_type.Add(type);
	}
public:
	bool m_smallScreen;             //!< Special GUI mode (for 800x600 res!).

	wxFrame *m_frame;               //!< Main frame.
	bool m_running;                 //!< Whereas the capture thread is running or not.
	bool m_change_settings_pending; //!< Whereas there was modifs in settings while thread running (ie synchro needed).

	wxImage m_dvFrame;              //!< WxImage where to decode dv frame data to display it.

	ThreadMessage m_threadMessage;  //!< The array of messages for main thread.

	bool m_capture;                 //!< True if we are capturing, false otherwise.
	int m_showEvery;                //!< Display every n dv frame.
	bool m_show;                    //!< True to display dv frame.
	bool m_beep;                    //!< True to play "beep" sound while recording.
	bool m_recVAUX;                 //!< True to store VAUX data in html file.

	//! @name The interface card to use (typically == 0).
	//@{
	int m_port,                     //!< Port to use.
	    m_backPort;                 //!< Backup of previous data.
	int m_node,                     //!< Node to use.
	    m_backNode;                 //!< Backup of previous data.
	//@}

	//! @name Other technical things.
	//@{
	bool m_rawPipe;                 //!< True to use stdin wether raw1394 as input.
	uint64_t m_guid;                //!< If multiple dv devices, select one with its hex guid.
	uint64_t m_backGuid;            //!< Backup of previous data.
	int m_channel;                  //!< Channel to try to find dv stream (default 63).
	int m_backChannel;              //!< Backup of previous data.
	int m_buffers;                  //!< Number of frames to buffer (default: 250).
	bool m_noAVC;                   //!< No use of AV/C capabilities.
	bool m_noStop;                  //!< True not to send stop command when halting capture.
	//@}

	//! @name File parameters.
	//@{
	wxString m_path;                //!< Directory of the video file.
	wxString m_file;                //!< Radical of the video file.
	int m_frameEvery;               //!< To store only each n frames (default: all frames).
	int m_frameCount;               //!< Max number of frames to store in each file.
	std::string m_duration;         //!< Maximum time to record, SMIL or SMPTE string.
	int m_maxFileSize;              //!< The maximum size of the video files (default: 1024Mb, 0: infinity).
	int m_collectionSize;           //!< Force to create a new file when the current file reachs this size.
	int m_collectionMinCutFileSize; //!< To avoid litle files when using previous option.
	int m_autosplit;                //!< Bool in fact: true to enable auto split of the dv stream.
	//@}

	//@{
	bool m_lockstep;                //!< Enable lockstep behavior (using m_frameCount).
	int m_lockstepMaxDrops;         //!< If not -1, max number of consecutive frames dropped before closing file.
	int m_lockstepTotalDrops;       //!< If not -1, max number of frames dropped before closing file.
	//@}

	//@{
	int m_timestamp;                //!< Bool in fact: true to put time info from dv stream in file name.
	bool m_timecode;                //!< True to put first time code from dv frame in file name.
	bool m_timesys;                 //!< True to put system time in file name.
	//@}

	//! @name File format.
	//@{
	int m_fileFormat;               //!< Type of file to create (*.avi, *.dv, ...).
//	bool m_openDml;                 //!< True to enable openDML files, set internaly.
	int m_jpegQuality;              //!< For jpeg files: quality of the compression (0 to 100).
	bool m_jpegDeinterlace;         //!< For jpeg files: true to deinterlace frames.
	int m_jpegWidth;                //!< For jpeg files: width and.
	int m_jpegHeight;               //!< Height of the image (<= 2048).
	bool m_jpegOverwrite;           //!< For jpeg files: true to overwrite existing files.
	//@}

	//! @name Informations about capture.
	//@{
	int m_totalDroppedFrames;       //!< Number of frames dropped, since the start of the thread.
	int m_droppedFrames;            //!< Number of frames dropped, since the start of the capture.
	int m_totalFrames;              //!< Number of frames received since the start of the thread.
	int m_captFrames;               //!< Number of frames written in file(s).
	float m_sizeFiles;              //!< Size of file(s) written (not working?).
	//@}

	//! @name DV camera status.
	//@{
	wxString m_cameraStatus;        //!< Camera status.
	wxString m_isRecording;         //!< If camear is recording.
	wxArrayString m_devicesCap;     //!< Capacities of ieee1394 devices connected.
	//@}

//	unsigned int m_transportStatus; //!< not used here
};

#endif /*THREAD_COMM_H_*/
