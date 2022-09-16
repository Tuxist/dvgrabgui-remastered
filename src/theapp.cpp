/*!
 * @file theapp.cpp
 * @brief DVGrabGUI application class implementation.
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

#include "theapp.h"

/*!Create a new application object: this macro will allow wxWidgets to create
 * the application object during program execution (it's better than using a
 * static object for many reasons) and also declares the accessor function
 * wxGetApp() which will return the reference of the right type (i.e. TheApp and
 * not wxApp)
 */
IMPLEMENT_APP(TheApp);

//! Application constructor.
TheApp::TheApp() : wxApp(),
                   m_mutex(), m_mutexEnd(), m_threadEnd(m_mutexEnd), m_cth(NULL)
{
	m_frame = NULL;
}

/*!
 * `Main program' equivalent: the program execution "starts" here.
 * @return TRUE if sucess, FLASE if error(s) occurred.
 */
bool TheApp::OnInit()
{
	wxImage::AddHandler(new wxJPEGHandler);
	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxXPMHandler);
//	wxFileSystem::AddHandler(new wxZipFSHandler());

	m_locale = NULL;

	m_firstlog = new wxLogStderr((new wxFFile(wxGetenv(_T("HOME"))
	                                          + wxString(_T("/.dvgrabgui/dvgrabgui.log")),
	                                          _T("w")))->fp());
	wxLog::SetActiveTarget(m_firstlog);
	m_seclog = new wxLogChain(NULL);

	// Create the main application window
	m_frame = new MainFrame(_T("DVGrabGUI 0.9.4"), wxDefaultPosition,
	                        wxSize(800, 600), &m_mutex, &m_cth, m_seclog);
	m_cth.m_frame = m_frame;

	// Show it and tell the application that it's our main window
	m_frame->Show(true);
	SetTopWindow(m_frame);

	// Success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned false here, the
	// application would exit immediately.
	return true;
}

/*!
 * Exit function called just before exiting: used to halt capture and stop/delete the
 * capture thread, if not yet done!
 * Also delete the @a m_locale pointer, if needed.
 * @return The value of the base class OnExit function.
 */
int TheApp::OnExit()
{
	// Halt dv capture, then capture thread.
	m_mutex.Lock();
	m_cth.m_capture = false;
	m_cth.m_running = false;
	m_mutex.Unlock();
	if(m_thread)
	{
		m_thread->Delete();
		m_threadEnd.Wait();
		m_mutexEnd.Unlock();
	}
	m_thread = NULL;

	if(m_locale) delete m_locale;

	delete wxLog::SetActiveTarget(NULL);

	return wxApp::OnExit();
}

/*
 * Helper functions.
 */

/*!
 * Here we have to destroy the MainFrame window, and to recreate
 * a new one in the desired mode.
 */
void TheApp::ChangeGuiMode()
{
	m_mutex.Lock();
	bool smallscr = m_cth.m_smallScreen;
	m_mutex.Unlock();

	wxLogMessage(wxString(_T("Begining change in GUI mode to: "))
	             + (smallscr ? wxString(_T("\"Small Screen\"")) : wxString(_T("\"Normal Mode\""))));
	MainFrame *tf = m_frame;

	// Create the main application window (do not reload config!).
	m_frame = new MainFrame(_T("DVGrabGUI 0.9.4"), wxDefaultPosition,
	                        wxSize(800, 600), &m_mutex, &m_cth, m_seclog, tf, smallscr);
	m_mutex.Lock();
	m_cth.m_frame = m_frame;
	m_mutex.Unlock();
	m_frame->SetPosition(tf->GetPosition());
	// Show it and tell the application that it's our main window
	m_frame->Show(true);
	SetTopWindow(m_frame);

	tf->Close(true);
	wxLogMessage(_T("Change in GUI mode completed"));
}


//! Helper function for starting/halting the capture thread.
/*!
 * @param on @a TRUE to start, @a FALSE to halt the thread.
 */
void TheApp::StartThread(bool on)
{
	// Start thread.
	if(on)
	{
		if(!m_thread)
		{
			// Reset of "hardware" settings
			m_cth.m_port = m_cth.m_backPort;
			m_cth.m_node = m_cth.m_backNode;
			m_cth.m_guid = m_cth.m_backGuid;
			m_cth.m_channel = m_cth.m_backChannel;

			m_cth.m_capture = false;
			m_cth.m_running = false;
			m_cth.m_totalDroppedFrames = m_cth.m_droppedFrames = 0;
			m_mutexEnd.Unlock(); // To be sure to have this mutex unlocked!
			m_mutexEnd.Lock();
			m_thread = new DvGrabThread(&m_mutex, &m_mutexEnd, &m_threadEnd, &m_cth);
			if(m_thread)
			{
				m_cth.m_running = true;
				m_thread->Run();
			}
			else
				m_cth.SetMessage(ERROR, _("DVG-APP: Couldn't start capture thread"));
		}
	}
	// Stop thread.
	// Note: m_cth's values updated (from SettingsWindow's controls)
	// in MainWindow, when receiving "THREAD_TERM" event.
	else
	{
		m_mutex.Lock();
		m_cth.m_capture = false;
		m_mutex.Unlock();
		if(m_thread)
		{
			m_thread->Delete();
			m_threadEnd.Wait();
		}
		m_thread = NULL;
		m_cth.m_running = false;
	}
	// Update UI.
	if(m_cth.m_frame)
		m_frame->UpdateUI();
}

//! Helper function for starting/halting the dv capture.
/*!
 * @param on @a TRUE to start, @a FALSE to halt dv capture.
 */
void TheApp::StartCapt(bool on)
{
	m_mutex.Lock();
	if(m_thread)
	{
		if(on)
			m_cth.m_capture = true;
		else
			m_cth.m_capture = false;
	}
	m_mutex.Unlock();
	m_frame->UpdateUI();
}
