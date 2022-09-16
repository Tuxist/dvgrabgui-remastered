/*!
 * @file dvgrab.h
 * @brief DVGrabGUI capture thread class declaration.
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

#ifndef DVGRAB_H_
#define DVGRAB_H_

#include "includes.h"           //!< General includes.
#include "threadinout.h"        //!< Communication classes between threads.

#include <libraw1394/raw1394.h> //!< Low level API for ieee1394 ports.

#include "ieee1394io.h"         //!< ieee1394 reader, directly using dvgrab version.
#include "filehandler.h"        //!< Handlers for different type of files (avi, dv, jpeg, ...), direcly using dvgrab version.
#include "frame.h"              //!< Class containing a dv frame, directly using dvgrab version.
#include "smiltime.h"           //!< Managing different time encodings, directly using dvgrab version.

#define RAW_BUF_SIZE    (10240) //!< Not used?

/*!
 * @class DvGrabThread
 * @brief Thread managing capture process, inheriting from wxThread.
 */
class DvGrabThread : public wxThread
{
public:
	//! @name Constructor/Destructor
	//@{
	DvGrabThread(wxMutex *m, wxMutex *mE, wxCondition *cd, ThreadInOut *cth);
	//! Default destructor, nothing to do.
	~DvGrabThread() {}
	//@}

	//! @name Standart wxThread functions.
	//@{
	void Create();
	virtual void* Entry();
	virtual void OnExit();
	//@}

	//! @name Internal functions (might also be private?).
	//@{
	bool startCapture();
	void stopCapture();
	//! Void function, not used.
	void status() {};
	bool isPlaying();
	//! Not Used! @return Allways TRUE.
	bool isInteractive() {return m_interactive;}
	bool done();
//	void testCapture( void );
	static void testCaptureProxy( BusResetHandlerData ); //!< Posix thread stuff?
	//@}

private:
	//! @name Internal functions.
	//@{
	void sendCaptureStatus(TimeCode& timeCode, struct tm& recDate);
	void writeFrame(TimeCode& timeCode, struct tm& recDate);
	void cleanup();
	//@}

private:
	//! @name Objects shared between threads.
	//@{
	wxMutex *m_mutex;                        //!< General access control mutex
	wxMutex *m_mutexEnd;                     //!< Mutex protecting m_threadEnd condition.
	wxCondition *m_threadEnd;                //!< Condition for waiting real end of thread.
	ThreadInOut *m_cth;                      //!< Object regrouping all datas shared between threads.
	//@}

	unsigned char *m_c;                      //!< Image buffer.
	bool m_prevCapture;                      //!< Flag remembering if we are capturing or not.
	bool m_normTerm;                         //!< Flag indicating if thread has ended due to internal error.

private:
	static Frame *m_frame;                   //!< Array of dv Frames.
	AVC *m_avc;                              //!< Object for accessing to VCR commands.
	iec61883Connection* m_connection;        //!< Connection object!
	IEEE1394Reader *m_reader;                //!< Object managing reading of ieee1394 data.
	static FileHandler *m_writer;            //!< Object managing writing dv frames in file.

	bool m_readerActive;                     //!< Indicate if m_reader is working.
	int m_sizeSplitMode;                     //!< Internal SplitMode flag, for now, just true if ThreadInOut::m_collectionSize > 0.
	bool m_openDML;                          //!< Internal OpenDML flag, true if ThreadInOut::m_maxFileSize > 1000 or == 0.
	bool m_interactive;                      //!< Interactive mode flag, allways true here.
	SMIL::MediaClippingTime* m_timeDuration; //!< @brief Duration of video recording, not used here because of problems
	                                         //!< with wxString cast to std::string (ThreadInOut::m_duration).
	bool m_lockPending;                      //!< Flag indicating that the video file is to be closed.
	TimeCode m_prevTimeCode;                 //!< Storing timecode of previous frame, used to detect new clips in dv stream.
	bool m_captureActive;                    //!< Indicate that we are recording dv stream.

	unsigned int m_transportStatus;          //!< Not used here for now.
};

#endif /*DVGRAB_H_*/
