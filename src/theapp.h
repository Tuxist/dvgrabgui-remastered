/*!
 * @file theapp.h
 * @brief DVGrabGUI application class declaration.
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

#ifndef THEAPP_H_
#define THEAPP_H_

#include "includes.h"    //!< General includes.
#include "mainframe.h"   //!< Main frame class.
#include "dvgrab.h"      //!< Capture thread class.
#include "threadinout.h" //!< Communication classes between threads.

//! Foward declaration.
class MainFrame;

//! The usual application class
/*!
 * @class TheApp
 * @brief Define a new application type, derived from wxApp.
 *
 * Manage the locales (gui languages) and capture thread.
 */
class TheApp : public wxApp
{
public:
	//! @name Constructor/Destructor.
	//@{
	TheApp();
	//! Default destructor, nothing to do...
	~TheApp() {};
	//@}
	
	//! @name Init/Exit functions.
	//@{
	//! This function is called on application startup (used for the app initialization).
	virtual bool OnInit();
	//! This function is called just before exiting.
	virtual int OnExit();
	//@}

	//! Used to change the GUI mode: "normal" or "small screen".
	void ChangeGuiMode();

	//! @name Control of capture thread.
	//@{
	//! Start/Stop capture thread.
	void StartThread(bool on);
	//! Start/Stop capture.
	void StartCapt(bool on);
	//@}

public:
	MainFrame *m_frame;      //!< DVGrabGUI's main frame.
	wxLocale *m_locale;      //!< wxWidgets stuff for managing locales...

	DvGrabThread *m_thread;  //!< The capture thread.
	wxMutex m_mutex,         //!< The mutex for accessing shared data between main and capture thread.
	        m_mutexEnd;      //!< The mutex locked during all the life of capture thread, protecting @a m_threadEnd.
	wxCondition m_threadEnd; //!< The condition signaling that the capture thread has actually halted.

	ThreadInOut m_cth;       //!< The object storing all data shared between main and capture threads.

	wxLogStderr *m_firstlog; //!< First log target.
	wxLogChain *m_seclog;    //!< Second log target (chaining the messages to the first one either).
};

DECLARE_APP(TheApp)

#endif /*THEAPP_H_*/
