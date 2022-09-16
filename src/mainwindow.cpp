/*!
 * @file mainwindow.cpp
 * @brief DVGrabGUI main window class implementation.
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

#include "mainwindow.h"


//! @name Defining all control texts (as I use them several times, easyer to do it once here!)
//@{
#define TXT_STB_CAPTURE         _("Capture control")
#define TXT_START_THREAD        _("Standby")
#define TXT_STOP_THREAD         _("Stop")
#define TXT_START_CAPTURE       _("Record")
#define TXT_STOP_CAPTURE        _("Pause")

#define TXT_STB_VIDEO_FILE      _("Video file prefix")

#define TXT_STB_MISC_INFOS      _("Misc infos")
#define TXT_CAPTURE_INFO1_5ARGS _("Frames received: %6.6d\nFrames captured: %6.6d\n" \
                                  /*"File(s) size: %d Mo\n\n"*/                      \
                                  "Total frames dropped: %3.3d\n"                    \
                                  "Frames dropped\nduring capture: %3.3d\n---\n")
#define TXT_CAPTURE_INFO2_4ARGS _("Port: %2.2d\nNode: %2.2d\nChannel: %2.2d\nGUID: Ox%16.16llx\n---\n")
#define TXT_CAPTURE_INFO3       _("Camera status:\n\t")
#define TXT_CAPTURE_INFO4       _("\nCamera is recording:\n\t")
#define TXT_CAPTURE_INFO5       _("\nDevice capacities:\n\t")

#define TXT_STB_DISK_SPACE      _("Remaining disk space")
#define TXT_TOTAL_SPACE_7ARGS   _("\nTOTAL:   %5.2fGo\n\"Raw\" dv: %d:%2.2d:%2.2d\n" \
                                  "\"AVI 2\" dv: %d:%2.2d:%2.2d\n\n---\n\n")
#define TXT_FREE_SPACE_7ARGS    _("FREE:   %5.2fGo\n\"Raw\" dv: %d:%2.2d:%2.2d\n"    \
                                  "\"AVI 2\" dv: %d:%2.2d:%2.2d\n")
//@}


// wxWidgets event table...
BEGIN_EVENT_TABLE(MainWindow, wxPanel)
	EVT_BUTTON(BTT_START_THREAD, MainWindow::OnButtThread)
	EVT_BUTTON(BTT_START_CAPTURE, MainWindow::OnButtCapt)

	EVT_TEXT_ENTER(ETXT_SEL_FILE, MainWindow::OnTextFile)

	EVT_MOUSE_EVENTS(MainWindow::OnMouse)
	EVT_SIZE(MainWindow::OnSize)
END_EVENT_TABLE()


/*!
 * @param *par The parent window (should be a MainFrame one!).
 * @param *cth The ThreadInOut object.
 * @param *mutex The mutex protecting ThreadInOut object.
 * @param *smallF The small GUI font used in the MainWindow::m_textInfo control.
 * @param *ctrlF The control GUI font.
 */
MainWindow::MainWindow(wxWindow *par, ThreadInOut *cth, wxMutex *mutex,
                       wxFont *smallF, wxFont *ctrlF)
                      : wxPanel(par, wxID_ANY)
{
	m_cth = cth;
	m_mutex = mutex;

	m_mutex->Lock();
	bool smallscr = m_cth->m_smallScreen;
	m_mutex->Unlock();

	m_smallFont = smallF;
	m_ctrlFont = ctrlF;

// Lets create the layout of the main frame, with sizers (see doxygen generated doc for more infos).
	// Top sizer.
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// Sizer regrouping the video window and right-hand controls.
	wxBoxSizer *vidSizer = new wxBoxSizer(wxHORIZONTAL);

	// Window wich shows the dv frames received.
	m_vidWin = new VidWin(this, m_mutex, m_cth);
	vidSizer->Add(m_vidWin, 0, wxCENTER | wxLEFT, 5);

	// Sizer for the right controls (it depends if we are in "small screen" mode or not!).
	wxBoxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);
	if(smallscr)
	{
		m_diskGauge = new wxGauge(this, wxID_ANY, 1000, wxDefaultPosition,
		                          wxDefaultSize, wxGA_VERTICAL | wxGA_SMOOTH);
		m_mutex->Lock();
		wxString path = m_cth->m_path;
		m_mutex->Unlock();
		wxLongLong tot, free;
		wxGetDiskSpace(path, &tot, &free);
		double dtot = tot.ToDouble();
		double dfree = free.ToDouble();
		m_diskGauge->SetValue(1000 - (int)((dfree/dtot)*1000.0));

		paneSizer->Add(m_diskGauge, 1, wxEXPAND | wxALL, 0);
	}
	else
	{
		// Generics sizers used for layout.
		wxStaticBoxSizer *stBoxSz;
		wxBoxSizer *boxSz;
		ControlCL *current = NULL;

		// Controls for starting/halting capture thread and dv capture.
		stBoxSz = new wxStaticBoxSizer(wxHORIZONTAL, this, TXT_STB_CAPTURE);
		stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
		current = m_firstCtrl = new ControlCL(stBoxSz->GetStaticBox());
		m_buttThread = new wxButton(this, BTT_START_THREAD, TXT_START_THREAD);
		m_buttThread->SetFont(*m_ctrlFont);
		m_buttCapt = new wxButton(this, BTT_START_CAPTURE, TXT_START_CAPTURE);
		m_buttCapt->SetFont(*m_ctrlFont);
		stBoxSz->Add(m_buttThread, 1, wxALL, 0);
		stBoxSz->Add(m_buttCapt, 1, wxALL, 0);
		paneSizer->Add(stBoxSz, 0, wxEXPAND | wxALL, 0);

		// Controls for managing the video file parameters.
		stBoxSz = new wxStaticBoxSizer(wxVERTICAL, this, TXT_STB_VIDEO_FILE);
		stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
		current = current->Add(stBoxSz->GetStaticBox());
		m_ctrlSelFile = new wxTextCtrl(this, ETXT_SEL_FILE, m_cth->m_file, wxDefaultPosition,
		                               wxDefaultSize, wxTE_PROCESS_ENTER);
		m_ctrlSelFile->SetFont(*m_ctrlFont);
		m_ctrlSelFile->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(MainWindow::OnTextFile2), NULL, this);
		boxSz = new wxBoxSizer(wxHORIZONTAL);
		boxSz->Add(m_ctrlSelFile, 1, wxALIGN_CENTER | wxALL, 0);
		stBoxSz->Add(boxSz, 0, wxEXPAND | wxALL, 0);
		paneSizer->Add(stBoxSz, 0, wxEXPAND | wxALL, 0);

		// m_textInfo giving infos about capture thread and camera.
		stBoxSz = new wxStaticBoxSizer(wxVERTICAL, this, TXT_STB_MISC_INFOS);
		stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
		current = current->Add(stBoxSz->GetStaticBox());
		wxString st;
		st =  wxString::Format(TXT_CAPTURE_INFO1_5ARGS, m_cth->m_totalFrames,
		                       m_cth->m_captFrames, /*m_cth->m_sizeFiles,*/
		                       m_cth->m_totalDroppedFrames, m_cth->m_droppedFrames);
		st += wxString::Format(TXT_CAPTURE_INFO2_4ARGS, m_cth->m_port,
		                       m_cth->m_node, m_cth->m_channel, m_cth->m_guid);
		st += TXT_CAPTURE_INFO3 + m_cth->m_cameraStatus;
		st += TXT_CAPTURE_INFO4 + m_cth->m_isRecording;
//		st += TXT_CAPTURE_INFO5;
//		for(uint i = 0; i < m_cth->m_devicesCap.GetCount(); i++)
//			st += m_cth->m_devicesCap[i] + _T("\n");
		m_textInfo = new wxStaticText(this, wxID_ANY, st);
		m_textInfo->SetFont(*m_smallFont);
		stBoxSz->Add(m_textInfo, 0, wxEXPAND | wxALL, 0);
		paneSizer->Add(stBoxSz, 0, wxEXPAND | wxALL, 0);

		stBoxSz = new wxStaticBoxSizer(wxHORIZONTAL, this, TXT_STB_DISK_SPACE);
		stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
		current = current->Add(stBoxSz->GetStaticBox());
		m_diskGauge = new wxGauge(this, wxID_ANY, 1000, wxDefaultPosition,
		                          wxDefaultSize, wxGA_VERTICAL | wxGA_SMOOTH);
		m_mutex->Lock();
		wxString path = m_cth->m_path;
		m_mutex->Unlock();
		wxLongLong tot, free;
		wxGetDiskSpace(path, &tot, &free);
		double dtot = tot.ToDouble();
		double dfree = free.ToDouble();
		int rawsectot = (int)(dtot / 3600000.0); // 3600000o = 1 sec. of DV.
		int rawsecfree = (int)(dfree / 3600000.0);
		int avi2sectot = (int)(dtot / 3792000.0); // 3792000o = 1 sec. of (avi2 = raw + audio) DV.
		int avi2secfree = (int)(dfree / 3792000.0);
		m_diskGauge->SetValue(1000 - (int)((dfree/dtot)*1000.0));
		st =  wxString::Format(TXT_TOTAL_SPACE_7ARGS, (dtot / 1073741824.0),
		                       rawsectot/3600, (rawsectot%3600)/60, (rawsectot%3600)%60,
		                       avi2sectot/3600, (avi2sectot%3600)/60, (avi2sectot%3600)%60);
		st += wxString::Format(TXT_FREE_SPACE_7ARGS, (dfree / 1073741824.0),
		                       rawsecfree/3600, (rawsecfree%3600)/60, (rawsecfree%3600)%60,
		                       avi2secfree/3600, (avi2secfree%3600)/60, (avi2secfree%3600)%60);
		m_diskInfo = new wxStaticText(this, wxID_ANY, st);
		m_diskInfo->SetFont(*m_smallFont);
		stBoxSz->Add(m_diskGauge, 0, wxEXPAND | wxRIGHT, 10);
		stBoxSz->Add(m_diskInfo, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 0);
		paneSizer->Add(stBoxSz, 1, wxEXPAND | wxALL, 0);
	}

	// Add paneSizer to vidSizer.
	vidSizer->Add(paneSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 4);

	// Add vidSizer to top sizer.
	sizer->Add(vidSizer, 1, wxEXPAND | wxTOP, 5);

	// Finaly, set the top sizer in the frame.
	SetSizer(sizer);

//	sizer->Layout();
	sizer->Fit(this);
	sizer->SetDimension(0, 0, GetClientSize().x, GetClientSize().y);
	GetParent()->SetClientSize(GetSize());
}

// ----------------------------------------------------------------------------
// Handlers for the different controls
// ----------------------------------------------------------------------------

//! Event handler managing @a m_buttThread events.
/*!
 * Used for starting/halting capture thread.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainWindow::OnButtThread(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	bool t = m_cth->m_running;
	m_mutex->Unlock();
	wxGetApp().StartThread(!t);
}

//! Event handler managing @a m_buttCapt events.
/*!
 * Used for starting/halting the recording of the dv stream.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainWindow::OnButtCapt(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	bool t = m_cth->m_capture;
	m_mutex->Unlock();
	wxGetApp().StartCapt(!t);
}


//! Event handler managing @a m_ctrlSelFile events.
/*!
 * Used for selecting the radical of the video file name.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainWindow::OnTextFile(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_file = m_ctrlSelFile->GetValue();
	m_mutex->Unlock();
	// To update the other SettingsWindow::m_ctrlSelFile!
	wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
	wxPostEvent(GetParent(), evt);
}

//! Event handler managing @a m_ctrlSelFile events.
/*!
 * Used for selecting the radical of the video file name.
 * Called when losing the focus (supposed to be like an "enter" action).
 * @param WXUNUSED(event) Unused wxFocusEvent event.
 */
void MainWindow::OnTextFile2 (wxFocusEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_file = m_ctrlSelFile->GetValue();
	m_mutex->Unlock();
	// To update the other SettingsWindow::m_ctrlSelFile!
	wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
	wxPostEvent(GetParent(), evt);
}

//! Event handler managing mouse events.
/*!
 * Call ("propagate") the event handler of the mainframe.
 * @param event The wxMouseEvent event.
 */
void MainWindow::OnMouse(wxMouseEvent& event)
{
	wxPostEvent(wxGetApp().GetTopWindow(), event);
}

//! Event handler managing size events.
/*!
 * Call ("propagate") the event handler of the mainframe,
 * then set the dimensions for the internal sizer.
 * @param event The wxSizeEvent event.
 */
void MainWindow::OnSize(wxSizeEvent& event)
{
	event.Skip();
	GetSizer()->SetDimension(0, 0, GetClientSize().x, GetClientSize().y);
}

//! Helper function udating all text in menus and UI.
/*!
 *  Only used when changing UI language or loading new config file.
 */
void MainWindow::UpdateAll()
{
	m_mutex->Lock();
	if(m_cth->m_smallScreen)
	{
		m_mutex->Unlock();
		return;          // Nothing to do if in "small screen" mode.
	}

// Control fonts
	m_textInfo->SetFont(*m_smallFont);
	m_diskInfo->SetFont(*m_smallFont);

	m_buttThread->SetFont(*m_ctrlFont);
	m_buttCapt->SetFont(*m_ctrlFont);
	m_ctrlSelFile->SetValue(m_cth->m_file);
	m_ctrlSelFile->SetFont(*m_ctrlFont);
	
	m_mutex->Unlock();

// Udating all wxStaticTexts and wxStaticBoxs 'stored' in ControlCL.
	// Have to do it twice, because of problems with wxStaticBoxs if font not changed!
	for(int i = -1; i < 2; i += 2)
	{
		m_ctrlFont->SetPointSize(m_ctrlFont->GetPointSize() + i);
		ControlCL *current = m_firstCtrl;
		current = current->Update(TXT_STB_CAPTURE, *m_ctrlFont);
		current = current->Update(TXT_STB_VIDEO_FILE, *m_ctrlFont);
		current = current->Update(TXT_STB_MISC_INFOS, *m_ctrlFont);
		current = current->Update(TXT_STB_DISK_SPACE, *m_ctrlFont);
	}

// Finally, complete GUI refresh.
// Problems here with sizers!
//	UpdateUI(true);
//	GetSizer()->Layout();
//	Fit();
//	GetSizer()->Fit(this);
	GetSizer()->SetDimension(0, 0, GetClientSize().x, GetClientSize().y);
//	GetParent()->SetClientSize(GetSize());
	UpdateUI(true);
}

//! Helper function refreshing User Interface.
/*!
 * @param all @a TRUE to update ALL the controls, @a FALSE otherwise.
 */
void MainWindow::UpdateUI(bool all)
{
	m_mutex->Lock();

	if(!m_cth->m_smallScreen)
	{
		if(m_cth->m_capture)
			m_buttCapt->SetLabel(TXT_STOP_CAPTURE);
		else
			m_buttCapt->SetLabel(TXT_START_CAPTURE);

		if(m_cth->m_running)
		{
			m_buttThread->SetLabel(TXT_STOP_THREAD);
			m_buttCapt->Enable(true);
		}
		else
		{
			m_buttThread->SetLabel(TXT_START_THREAD);
			m_buttCapt->Enable(false);
			m_buttCapt->SetLabel(TXT_START_CAPTURE);
		}
	}

	if(all)
	{
		wxLongLong tot, free;
		wxGetDiskSpace(m_cth->m_path, &tot, &free);
		double dtot = tot.ToDouble();
		double dfree = free.ToDouble();
		m_diskGauge->SetValue(1000 - (int)((dfree/dtot)*1000.0));

		if(!m_cth->m_smallScreen)
		{
			m_ctrlSelFile->SetValue(m_cth->m_file);

			wxString st;
			st =  wxString::Format(TXT_CAPTURE_INFO1_5ARGS, m_cth->m_totalFrames,
			                       m_cth->m_captFrames, /*m_cth->m_sizeFiles,*/
			                       m_cth->m_totalDroppedFrames, m_cth->m_droppedFrames);
			st += wxString::Format(TXT_CAPTURE_INFO2_4ARGS, m_cth->m_port,
			                       m_cth->m_node, m_cth->m_channel, m_cth->m_guid);
			st += TXT_CAPTURE_INFO3 + m_cth->m_cameraStatus;
			st += TXT_CAPTURE_INFO4 + m_cth->m_isRecording;
//			st += TXT_CAPTURE_INFO5;
//			for(uint i = 0; i < m_cth->m_devicesCap.GetCount(); i++)
//				st += m_cth->m_devicesCap[i] + _T("\n");
			m_textInfo->SetLabel(st);

			int rawsectot = (int)(dtot / 3600000.0); // 3600000o = 1 sec. of DV.
			int rawsecfree = (int)(dfree / 3600000.0);
			int avi2sectot = (int)(dtot / 3792000.0); // 3792000o = 1 sec. of (avi2) DV.
			int avi2secfree = (int)(dfree / 3792000.0);
			m_diskGauge->SetValue(1000 - (int)((dfree/dtot)*1000.0));
			st =  wxString::Format(TXT_TOTAL_SPACE_7ARGS, (dtot / 1073741824.0),
			                       rawsectot/3600, (rawsectot%3600)/60, (rawsectot%3600)%60,
			                       avi2sectot/3600, (avi2sectot%3600)/60, (avi2sectot%3600)%60);
			st += wxString::Format(TXT_FREE_SPACE_7ARGS, (dfree / 1073741824.0),
			                       rawsecfree/3600, (rawsecfree%3600)/60, (rawsecfree%3600)%60,
			                       avi2secfree/3600, (avi2secfree%3600)/60, (avi2secfree%3600)%60);
			m_diskInfo->SetLabel(st);
		}
	}
	m_mutex->Unlock();
}	

//! Managing update events from capture thread.
/*!
 * Only show the current frame if the total number of shown frames (@a shownFrames)
 * is a multiple of m_showEvery.
 */
void MainWindow::ThreadUpdate()
{
	static int receivedFrames(0);
	m_mutex->Lock();
	int se = m_cth->m_showEvery;
	bool s = m_cth->m_show;
	m_mutex->Unlock();
	if(s && !(receivedFrames % se))
	{
		m_vidWin->Refresh();
		m_showedFrames++;
	}

	// Updating side text info, at least 1 time per seconds, when capture thread running.
	if(!(receivedFrames % 25))
	{
		wxString st;
		m_mutex->Lock();
		st =  wxString::Format(TXT_CAPTURE_INFO1_5ARGS, m_cth->m_totalFrames,
		                       m_cth->m_captFrames, /*m_cth->m_sizeFiles,*/
		                       m_cth->m_totalDroppedFrames, m_cth->m_droppedFrames);
		st += wxString::Format(TXT_CAPTURE_INFO2_4ARGS, m_cth->m_port,
		                       m_cth->m_node, m_cth->m_channel, m_cth->m_guid);
		st += TXT_CAPTURE_INFO3 + m_cth->m_cameraStatus;
		st += TXT_CAPTURE_INFO4 + m_cth->m_isRecording;
//		st += TXT_CAPTURE_INFO5;
//		for(uint i = 0; i < m_cth->m_devicesCap.GetCount(); i++)
//			st += m_cth->m_devicesCap[i] + _T("\n");
		m_mutex->Unlock();
		m_textInfo->SetLabel(st);
	}

	receivedFrames++;
}
