/*!
 * @file settingswindow.cpp
 * @brief DVGrabGUI unique dialog box class implementation.
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

#include "settingswindow.h"

//! @name Defining all control texts (as I use them several times, easyer to do it once here!)
//@{
// Tab's text.
#define TXT_TAB_CAPTURE _("Capture")
#define TXT_TAB_HARDWARE _("Hardware")
#define TXT_TAB_SHTC_GUIFONTS _("Shortcuts\nGUI fonts\nGUI mode")

// ------------------------------------------------
// First "tab" texts (capture).
// ------------------------------------------------
#define TXT_SHOW_IMG _("Show video")
#define TXT_FREQ_IMG _("1 img./ ")
#define TXT_PLAY_BEEP _("Play beep")
#define TXT_STBOX_DVDISPLAY _("Dv display controls")

#define TXT_STBOX_VIDEOPARAMS _("Video file parameters")
#define TXT_SEL_PATH _("Directory and prefix for video files:")
#define TXT_FILE_TYPE _("Video file type:")
#define TXT_FILETYPE_AVI1 _("AVI 1 (*.avi)")
#define TXT_FILETYPE_AVI2 _("AVI 2 (*.avi)")
#define TXT_FILETYPE_RAWDV _("Raw (*.dv)")
#define TXT_FILETYPE_RAWDIFF _("Raw (*.dif)")
#define TXT_FILETYPE_JPEG _("Jpeg (*.jpg)")
#define TXT_MAXFS _("Max file size(Mo):")
#define TXT_MAXFS_INFO1_7ARGS _("dv time: %2.2d:%2.2d:%2.2d:[%2.2d|%2.2d]\nPAL frames: %6.6d\nNTSC frames: %6.6d")
#define TXT_MAXFS_INFO2 _("\nNo limit!\n")
#define TXT_COLL_MAXCFS _("Max file size in a collection(Mo)\n(ex: 2048 for a FAT32 partition):")
#define TXT_COLL_MINFS _("Min file size in a collection(Mo):")

#define TXT_STBOX_FILENAMEPARAMS _("File name options: Append to file name:")
#define TXT_TIME_STAMP _("The dv Date/Time")
#define TXT_TIMECODE _("The 1rst dv frame timecode")
#define TXT_TIME_SYS _("The system Date/Time")

#define TXT_STBOX_RECORDINGPARAMS _("Recording features")
#define TXT_FRAME_EVERY _("Save one frame each:")
#define TXT_AUTO_SPLIT _("Auto detect\nnew clips")
#define TXT_FRAME_COUNT _("Max frames to save:")
#define TXT_FC_INFO1_8ARGS _("PAL:\t\t%2.2d:%2.2d:%2.2d:%2.2d\nNTSC:\t%2.2d:%2.2d:%2.2d:%2.2d")
#define TXT_FC_INFO2 _("No limit!\n")
#define TXT_LOCKSTEP _("Enable Lockstep")
#define TXT_LOCKSTEP_MAXD _("Number of consecutive drops allowed:")
#define TXT_LOCKSTEP_TOTD _("Total number of drops allowed:")

// ------------------------------------------------
// Second "tab" texts (hardware).
// ------------------------------------------------
#define TXT_STBOX_DEVICEPARAMS _("Device selection options")
#define TXT_RAW_PIPE _("Use stdin\nas dv input")
#define TXT_DV_PORT _("Port\n(auto = -1):")
#define TXT_DV_NODE _("Node\n(auto = -1):")
#define TXT_DV_CHANNEL _("Channel\n(auto = -1, default = 63):")
#define TXT_DV_GUID _("GUID (auto = Ox0000000000000000):")

#define TXT_STBOX_CAPTUREPARAMS _("Capture options")
#define TXT_NO_AVC _("Disable use of AV/C")
#define TXT_NO_STOP _("Disable stop command\nwhen halting capture")
#define TXT_REC_VAUX _("Store VAUX data")
#define TXT_BUFFER _("Number of frames to keep in RAM:")

#define TXT_STBOX_JPEGPARAMS _("Jpeg Options")
#define TXT_JPEG_DEINTERLACE _("Deinterlace dv frames")
#define TXT_JPEG_OVERWRITE _("Allow overwriting jpeg files")
#define TXT_JPEG_QUALITY _("Quality\n(default = 90):")
#define TXT_JPEG_WIDTH _("Width\n(no change = -1):")
#define TXT_JPEG_HEIGHT _("Height\n(no change = -1):")

// ------------------------------------------------
// Third "tab" texts (Shortcuts/GUI fonts).
// ------------------------------------------------
#define TXT_STBOX_SHTCGRID _("Shortcuts for menu entries")
#define TXT_SHTCGRID_MENU _("Menu entries")
#define TXT_SHTCGRID_SHORTCUTS _("shortcuts")

#define TXT_STBOX_GUIMODE _("Select GUI mode")
#define TXT_GUI_MODE _("Activate \"small screen\" mode")

#define TXT_STBOX_GUIFONTS _("Select the fonts of the GUI")
#define TXT_GUIFONTS_MENU _("Menu Font")
#define TXT_GUIFONTS_STATBAR _("Status bar Font")
#define TXT_GUIFONTS_SMALL _("Small Font")
#define TXT_GUIFONTS_CTRL _("Controls Font")
//@}

//! @name wxWidgets' events table for OptionDiag.
//@{
BEGIN_EVENT_TABLE(SettingsWindow, wxListbook)
	// ------------------------------------------------
	// First "tab" controls (capture).
	// ------------------------------------------------
	EVT_SPINCTRL   (SPC_FREQ_IMG,        SettingsWindow::OnSpinFreqImg)
	EVT_CHECKBOX   (CKB_SHOW_IMG,        SettingsWindow::OnCheckShowImg)
	EVT_CHECKBOX   (CKB_PLAY_BEEP,       SettingsWindow::OnCheckPlayBeep)

	EVT_TEXT_ENTER (ETXT_SEL_PATH,       SettingsWindow::OnTextPath)
	EVT_BUTTON     (BTT_SEL_PATH,        SettingsWindow::OnBttPath)
	EVT_TEXT_ENTER (ETXT_SEL_FILE,       SettingsWindow::OnTextFile)
	EVT_CHOICE     (CHC_FILE_TYPE,       SettingsWindow::OnChoiceFileType)
	EVT_SPINCTRL   (SPC_MAX_FSIZE,       SettingsWindow::OnSpinMaxFSize)
	EVT_SPINCTRL   (SPC_COLL_SIZE,       SettingsWindow::OnSpinCollSize)
	EVT_SPINCTRL   (SPC_COLL_MCFSIZE,    SettingsWindow::OnSpinCollMCFSize)

	EVT_CHECKBOX   (CKB_TIMESTAMP,       SettingsWindow::OnTimestamp)
	EVT_CHECKBOX   (CKB_TIMECODE,        SettingsWindow::OnTimecode)
	EVT_CHECKBOX   (CKB_TIMESYS,         SettingsWindow::OnTimesys)

	EVT_SPINCTRL   (SPC_FRAME_EVERY,     SettingsWindow::OnSpinFrameEvery)
	EVT_CHECKBOX   (CKB_AUTOSPLIT,       SettingsWindow::OnCheckAutosplit)
	EVT_SPINCTRL   (SPC_FRAME_COUNT,     SettingsWindow::OnSpinFrameCount)
	EVT_CHECKBOX   (CKB_LOCKSTEP,        SettingsWindow::OnLockstep)
	EVT_SPINCTRL   (SPC_LOCKSTEP_MD,     SettingsWindow::OnLockstepMD)
	EVT_SPINCTRL   (SPC_LOCKSTEP_TD,     SettingsWindow::OnLockstepTD)

	// ------------------------------------------------
	// Second "tab" controls (hardware).
	// ------------------------------------------------
	EVT_CHECKBOX   (CKB_RAW_PIPE,        SettingsWindow::OnRawPipe)
	EVT_SPINCTRL   (SPC_PORT,            SettingsWindow::OnPort)
	EVT_SPINCTRL   (SPC_NODE,            SettingsWindow::OnNode)
	EVT_SPINCTRL   (SPC_CHANNEL,         SettingsWindow::OnChannel)
	EVT_COMBOBOX   (CHC_GUID,            SettingsWindow::OnGuid)
	EVT_TEXT_ENTER (CHC_GUID,            SettingsWindow::OnGuid)

	EVT_CHECKBOX   (CKB_NOAVC,           SettingsWindow::OnNoAVC)
	EVT_CHECKBOX   (CKB_NOSTOP,          SettingsWindow::OnNoStop)
	EVT_SPINCTRL   (SPC_BUFFER,          SettingsWindow::OnBuffers)
	EVT_CHECKBOX   (CKB_REC_VAUX,        SettingsWindow::OnRecVAUX)

	EVT_CHECKBOX   (CKB_JPG_DEINTERLACE, SettingsWindow::OnJpegDeinterlace)
	EVT_CHECKBOX   (CKB_JPG_OVERWRITE,   SettingsWindow::OnJpegOverwrite)
	EVT_SPINCTRL   (SPC_JPG_QUALITY,     SettingsWindow::OnJpegQuality)
	EVT_SPINCTRL   (SPC_JPG_WIDTH,       SettingsWindow::OnJpegWidth)
	EVT_SPINCTRL   (SPC_JPG_HEIGHT,      SettingsWindow::OnJpegHeight)

	// ------------------------------------------------
	// Third "tab" controls (Shortcuts/GUI fonts).
	// ------------------------------------------------
	EVT_GRID_CELL_CHANGE(                SettingsWindow::OnChangeGrid)

	EVT_CHECKBOX   (CKB_GUIMODE,         SettingsWindow::OnGuiMode)

	EVT_BUTTON     (BTT_MENUFONT,        SettingsWindow::OnMenuFont)
	EVT_BUTTON     (BTT_STATBARFONT,     SettingsWindow::OnStatBarFont)
	EVT_BUTTON     (BTT_SMALLFONT,       SettingsWindow::OnSmallFont)
	EVT_BUTTON     (BTT_CTRLFONT,        SettingsWindow::OnCtrlFont)

END_EVENT_TABLE()
//@}

/*!
 * Initialize controls and create the layout (three tabs for now, heavy use of sizers!).
 *
 * @param *p The parent window.
 * @param *cth The ThreadInOut object, for reading and storing controls' values.
 * @param *mutex The mutex protecting access to the ThreadInOut object.
 * @param *menuF The pointer to the "menu" font.
 * @param *statBarF The pointer to the "status bar" font.
 * @param *smallF The pointer to the "small" font.
 * @param *ctrlF The pointer to the "control" font.
 * @param *mSC The wxString array containing the menu shortcuts.
 * @param *mText The wxString array containing the menu entries texts.
 * @param numSC The number of menu entries with shortcut available.
 */
SettingsWindow::SettingsWindow(wxWindow *p, ThreadInOut *cth, wxMutex *mutex, wxFont *menuF, 
                               wxFont *statBarF, wxFont *smallF, wxFont *ctrlF,
                               wxString *mSC, wxString *mText, int numSC)
                              : wxListbook(p, wxID_ANY)
{
	m_cth = cth;
	m_mutex = mutex;

	m_menuFont = menuF;
	m_statBarFont = statBarF;
	m_smallFont = smallF;
	m_ctrlFont = ctrlF;

	m_menuSC = mSC;
	m_menuText = mText;
	m_numSC = numSC;

	m_capture = new wxPanel(this, wxID_ANY);
	m_hardware = new wxPanel(this, wxID_ANY);
	m_shortcuts = new wxPanel(this, wxID_ANY);

	// Generics sizers used for layout.
	wxBoxSizer *boxSz, *tsz;
	wxStaticBoxSizer *stBoxSz;
	wxFlexGridSizer *flxGridSz;

	// Generic static text controls.
	wxStaticText *text;
	ControlCL *current = NULL;

	// ------------------------------------------------
	// Creating content of the first tab (Capture one).
	// ------------------------------------------------
	// Sizer of m_capture panel.
	wxBoxSizer *captureBSizer = new wxBoxSizer(wxVERTICAL);

	// *** First box --- Controls managing how the frames are displayed.
	stBoxSz = new wxStaticBoxSizer(wxHORIZONTAL, m_capture, TXT_STBOX_DVDISPLAY);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = m_firstCtrl = new ControlCL(stBoxSz->GetStaticBox());

	m_ctrlFreqImg = new wxSpinCtrl(m_capture, SPC_FREQ_IMG, wxString::Format(_T("%d"), m_cth->m_showEvery),
	                               wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                               1, 100, m_cth->m_showEvery);
	m_ctrlFreqImg->SetFont(*m_ctrlFont);
	m_ctrlShowImg = new wxCheckBox(m_capture, CKB_SHOW_IMG, TXT_SHOW_IMG);
	m_ctrlShowImg->SetFont(*m_ctrlFont);
	m_ctrlShowImg->SetValue(m_cth->m_show);
	m_ctrlPlayBeep = new wxCheckBox(m_capture, CKB_PLAY_BEEP, TXT_PLAY_BEEP);
	m_ctrlPlayBeep->SetFont(*m_ctrlFont);
	m_ctrlPlayBeep->SetValue(m_cth->m_beep);
	
	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_FREQ_IMG));
	current->c->SetFont(*m_ctrlFont);
	boxSz = new wxBoxSizer(wxVERTICAL);
	boxSz->Add(m_ctrlShowImg, 0, wxALL, 0);
	boxSz->Add(m_ctrlPlayBeep, 0, wxALL, 0);
	stBoxSz->Add(current->c, 0, wxALIGN_CENTER | wxALL, 0);
	stBoxSz->Add(m_ctrlFreqImg, 1, wxALIGN_CENTER | wxALL, 0);
	stBoxSz->Add(boxSz, 0, wxALIGN_CENTER | wxLEFT, 50);
	stBoxSz->AddStretchSpacer(2);
	captureBSizer->Add(stBoxSz, 0, wxEXPAND | wxBOTTOM | wxTOP, 5);

	
	// *** Second box --- Controls managing the files settings.
	wxString ts;
	int i, j;

	stBoxSz = new wxStaticBoxSizer(wxVERTICAL, m_capture, TXT_STBOX_VIDEOPARAMS);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = current->Add(stBoxSz->GetStaticBox());
	flxGridSz = new wxFlexGridSizer(3, 4, 20, 0);
	flxGridSz->AddGrowableCol(0, 6);
	flxGridSz->AddGrowableCol(1, 1);
	flxGridSz->AddGrowableCol(2, 4);
	flxGridSz->AddGrowableCol(3, 1);
	boxSz = new wxBoxSizer(wxVERTICAL);

	m_ctrlSelPath = new wxTextCtrl(m_capture, ETXT_SEL_PATH, m_cth->m_path, wxDefaultPosition,
	                               wxDefaultSize, wxTE_PROCESS_ENTER);
	m_ctrlSelPath->SetFont(*m_ctrlFont);
	m_ctrlSelPath->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(SettingsWindow::OnTextPath2), NULL, this);
	m_ctrlDiagSelPath = new wxButton(m_capture, BTT_SEL_PATH, _T("..."),
	                                 wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	m_ctrlDiagSelPath->SetFont(*m_ctrlFont);
	m_ctrlSelFile = new wxTextCtrl(m_capture, ETXT_SEL_FILE, m_cth->m_file, wxDefaultPosition,
	                               wxDefaultSize, wxTE_PROCESS_ENTER);
	m_ctrlSelFile->SetFont(*m_ctrlFont);
	m_ctrlSelFile->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(SettingsWindow::OnTextFile2), NULL, this);

	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_SEL_PATH));
	current->c->SetFont(*m_ctrlFont);
	boxSz->Add(current->c, 0, wxALL, 0);
	tsz = new wxBoxSizer(wxHORIZONTAL);
	tsz->Add(m_ctrlSelPath, 3, wxALIGN_CENTER | wxALL, 0);
	tsz->Add(m_ctrlDiagSelPath, 0, wxALIGN_CENTER | wxALL, 0);
	tsz->Add(m_ctrlSelFile, 1, wxALIGN_CENTER | wxALL, 0);
	boxSz->Add(tsz, 0, wxEXPAND | wxALL, 0);
	flxGridSz->Add(boxSz, 1, wxEXPAND | wxALL, 0);
	flxGridSz->AddStretchSpacer(1);

#if defined(HAVE_LIBJPEG)
	wxString s[5] = {TXT_FILETYPE_AVI1, TXT_FILETYPE_AVI2, TXT_FILETYPE_RAWDV, TXT_FILETYPE_RAWDIFF, TXT_FILETYPE_JPEG};
	m_ctrlFileType = new wxChoice(m_capture, CHC_FILE_TYPE, wxDefaultPosition,
	                              wxDefaultSize, 5, s);
#else
	wxString s[4] = {TXT_FILETYPE_AVI1, TXT_FILETYPE_AVI2, TXT_FILETYPE_RAWDV, TXT_FILETYPE_RAWDIFF};
	m_ctrlFileType = new wxChoice(m_capture, CHC_FILE_TYPE, wxDefaultPosition,
	                              wxDefaultSize, 4, s);
#endif // defined(HAVE_LIBJPEG)
	m_ctrlFileType->SetFont(*m_ctrlFont);
	switch(m_cth->m_fileFormat)
	{
		case AVI_DV1_FORMAT:
			m_ctrlFileType->SetSelection(0);
			break;
		case AVI_DV2_FORMAT:
			m_ctrlFileType->SetSelection(1);
			break;
		case RAW_FORMAT:
			m_ctrlFileType->SetSelection(2);
			break;
		case DIF_FORMAT:
			m_ctrlFileType->SetSelection(3);
			break;
#if defined(HAVE_LIBJPEG)
		case JPEG_FORMAT:
			m_ctrlFileType->SetSelection(4);
			break;
#endif // defined(HAVE_LIBJPEG)
		default:
			m_ctrlFileType->SetSelection(1);
	}

	boxSz = new wxBoxSizer(wxHORIZONTAL);
	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_FILE_TYPE));
	current->c->SetFont(*m_ctrlFont);
	boxSz->Add(current->c, 0, wxALIGN_CENTER | wxRIGHT, 5);
	boxSz->Add(m_ctrlFileType, 1, wxALIGN_CENTER | wxALL, 0);
	flxGridSz->Add(boxSz, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 0);
	flxGridSz->AddStretchSpacer(1);

	m_ctrlMaxFSize = new wxSpinCtrl(m_capture, SPC_MAX_FSIZE, wxString::Format(_T("%d"), m_cth->m_maxFileSize),
	                                wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                                0, 1048576/*1 To*/, m_cth->m_maxFileSize);
	m_ctrlMaxFSize->SetFont(*m_ctrlFont);
	if(m_ctrlMaxFSize->GetValue())
	{
		// Fills i with number of PAL frames, and j with number of NTSC frames, that
		// can be stored in max file size (Mo)...
		CalcNbrFramesFromSize(i, j);
		// Formating file size (in Mo) in (raw, avi2) dv time and (pal, ntsc) number of frames.
		ts.Printf(TXT_MAXFS_INFO1_7ARGS, i/90000, (i%90000)/1500, ((i%90000)%1500)/25,
		          ((i%90000)%1500)%25, ((j%108000)%1500)%30, i, j);
	}
	else
		ts = wxString(TXT_MAXFS_INFO2);
	m_ctrlMFSInfo = new wxStaticText(m_capture, wxID_ANY, ts, wxDefaultPosition,
	                                 wxDefaultSize, wxST_NO_AUTORESIZE);
	m_ctrlMFSInfo->SetFont(*m_ctrlFont);

	boxSz = new wxBoxSizer(wxVERTICAL);
	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_MAXFS));
	current->c->SetFont(*m_ctrlFont);
	boxSz->Add(current->c, 0, wxALL, 0);
	boxSz->Add(m_ctrlMaxFSize, 0, wxEXPAND | wxALL, 0);
	flxGridSz->Add(boxSz, 1, wxEXPAND | wxALL, 0);
	flxGridSz->AddStretchSpacer(1);
	flxGridSz->Add(m_ctrlMFSInfo, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	flxGridSz->AddStretchSpacer(1);

	m_ctrlCollSize = new wxSpinCtrl(m_capture, SPC_COLL_SIZE,
	                                wxString::Format(_T("%d"), m_cth->m_collectionSize),
	                                wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                                0, 1048576/*1 To*/, m_cth->m_collectionSize);
	m_ctrlCollSize->SetFont(*m_ctrlFont);

	boxSz = new wxBoxSizer(wxVERTICAL);
	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_COLL_MAXCFS));
	current->c->SetFont(*m_ctrlFont);
	boxSz->Add(current->c, 0, wxEXPAND | wxALL, 0);
	boxSz->Add(m_ctrlCollSize, 0, wxEXPAND | wxALL, 0);
	flxGridSz->Add(boxSz, 1, wxEXPAND | wxALL, 0);
	flxGridSz->AddStretchSpacer(1);

	m_ctrlCollMCFSize = new wxSpinCtrl(m_capture, SPC_COLL_MCFSIZE,
	                                   wxString::Format(_T("%d"), m_cth->m_collectionMinCutFileSize),
	                                   wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                                   0, 1048576/*1 To*/, m_cth->m_collectionMinCutFileSize);
	m_ctrlCollMCFSize->SetFont(*m_ctrlFont);
	
	boxSz = new wxBoxSizer(wxVERTICAL);
	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_COLL_MINFS));
	current->c->SetFont(*m_ctrlFont);
	boxSz->Add(current->c, 1, wxALL, 0);
	boxSz->Add(m_ctrlCollMCFSize, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	flxGridSz->Add(boxSz, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	flxGridSz->AddStretchSpacer(1);
	stBoxSz->Add(flxGridSz, 1, wxEXPAND | wxTOP | wxBOTTOM, 10);
	captureBSizer->Add(stBoxSz, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);


	// *** Third box --- Controls managing the file names options.
	stBoxSz = new wxStaticBoxSizer(wxHORIZONTAL, m_capture, TXT_STBOX_FILENAMEPARAMS);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = current->Add(stBoxSz->GetStaticBox());

	m_ctrlTimestamp = new wxCheckBox(m_capture, CKB_TIMESTAMP, TXT_TIME_STAMP);
	m_ctrlTimestamp->SetValue(m_cth->m_timestamp);
	m_ctrlTimestamp->SetFont(*m_ctrlFont);
	m_ctrlTimecode = new wxCheckBox(m_capture, CKB_TIMECODE, TXT_TIMECODE);
	m_ctrlTimecode->SetValue(m_cth->m_timecode);
	m_ctrlTimecode->SetFont(*m_ctrlFont);
	m_ctrlTimesys = new wxCheckBox(m_capture, CKB_TIMESYS, TXT_TIME_SYS);
	m_ctrlTimesys->SetValue(m_cth->m_timesys);
	m_ctrlTimesys->SetFont(*m_ctrlFont);
	
	stBoxSz->Add(m_ctrlTimestamp, 0, wxALIGN_CENTER | wxRIGHT, 20);
	stBoxSz->Add(m_ctrlTimecode, 0, wxALIGN_CENTER | wxRIGHT, 20);
	stBoxSz->Add(m_ctrlTimesys, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
	captureBSizer->Add(stBoxSz, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);


	// *** Forth box --- Controls managing the recording options.
	stBoxSz = new wxStaticBoxSizer(wxVERTICAL, m_capture, TXT_STBOX_RECORDINGPARAMS);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = current->Add(stBoxSz->GetStaticBox());
	flxGridSz = new wxFlexGridSizer(2, 4, 20, 20);
	flxGridSz->AddGrowableCol(0, 4);
	flxGridSz->AddGrowableCol(1, 9);
	flxGridSz->AddGrowableCol(2, 7);
	flxGridSz->AddGrowableCol(3, 1);
	boxSz = new wxBoxSizer(wxVERTICAL);
	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_FRAME_EVERY));
	current->c->SetFont(*m_ctrlFont);

	m_ctrlAutosplit = new wxCheckBox(m_capture, CKB_AUTOSPLIT, TXT_AUTO_SPLIT);
	m_ctrlAutosplit->SetFont(*m_ctrlFont);
	m_ctrlAutosplit->SetValue(m_cth->m_autosplit);

	flxGridSz->Add(m_ctrlAutosplit, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	m_ctrlFrameEvery = new wxSpinCtrl(m_capture, SPC_FRAME_EVERY, wxString::Format(_T("%d"), m_cth->m_frameEvery),
	                              wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                              1, 2592000/*24h in NTSC*/, m_cth->m_frameEvery);
	m_ctrlFrameEvery->SetFont(*m_ctrlFont);

	boxSz->Add(current->c, 0, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 0);
	boxSz->Add(m_ctrlFrameEvery, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	flxGridSz->Add(boxSz, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 0);

	m_ctrlFrameCount = new wxSpinCtrl(m_capture, SPC_FRAME_COUNT, wxString::Format(_T("%d"), m_cth->m_frameCount),
	                                  wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                                  0, 7635500/*~1 To & ~85h en PAL*/, m_cth->m_frameCount);
	m_ctrlFrameCount->SetFont(*m_ctrlFont);
	i = m_cth->m_frameCount * m_cth->m_frameEvery;
	// Formating number of frames in pal and ntsc time.
	i ? ts.Printf(TXT_FC_INFO1_8ARGS,
	              i/90000, (i%90000)/1500, ((i%90000)%1500)/25,
	              ((i%90000)%1500)%25, i/108000, (i%108000)/1800,
	              ((i%108000)%1800)/30, ((i%108000)%1800)%30)
	  : ts.Printf(TXT_FC_INFO2);
	m_ctrlFCInfo = new wxStaticText(m_capture, wxID_ANY, ts);
	m_ctrlFCInfo->SetFont(*m_ctrlFont);

	boxSz = new wxBoxSizer(wxVERTICAL);
	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_FRAME_COUNT));
	current->c->SetFont(*m_ctrlFont);
	boxSz->Add(current->c, 0, wxALL, 0);
	boxSz->Add(m_ctrlFrameCount, 0, wxEXPAND | wxALL, 0);
	boxSz->Add(m_ctrlFCInfo, 0, wxEXPAND | wxALL, 0);
	flxGridSz->Add(boxSz, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	flxGridSz->AddStretchSpacer(1);

	m_ctrlLockstep = new wxCheckBox(m_capture, CKB_LOCKSTEP, TXT_LOCKSTEP);
	m_ctrlLockstep->SetValue(m_cth->m_lockstep);
	m_ctrlLockstep->SetFont(*m_ctrlFont);

	flxGridSz->Add(m_ctrlLockstep, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 0);

	m_ctrlLockstepMD = new wxSpinCtrl(m_capture, SPC_LOCKSTEP_MD,
	                                  wxString::Format(_T("%d"), m_cth->m_lockstepMaxDrops),
	                                  wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                                  -1, 1000, m_cth->m_lockstepMaxDrops);
	m_ctrlLockstepMD->SetFont(*m_ctrlFont);

	boxSz = new wxBoxSizer(wxVERTICAL);
	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_LOCKSTEP_MAXD));
	current->c->SetFont(*m_ctrlFont);
	boxSz->Add(current->c, 0, wxALL, 0);
	boxSz->Add(m_ctrlLockstepMD, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	flxGridSz->Add(boxSz, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 0);

	m_ctrlLockstepTD = new wxSpinCtrl(m_capture, SPC_LOCKSTEP_TD,
	                                  wxString::Format(_T("%d"), m_cth->m_lockstepTotalDrops),
	                                  wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                                  -1, 1000, m_cth->m_lockstepTotalDrops);
	m_ctrlLockstepTD->SetFont(*m_ctrlFont);
	
	boxSz = new wxBoxSizer(wxVERTICAL);
	current = current->Add(new wxStaticText(m_capture, wxID_ANY, TXT_LOCKSTEP_TOTD));
	current->c->SetFont(*m_ctrlFont);
	boxSz->Add(current->c, 0, wxALL, 0);
	boxSz->Add(m_ctrlLockstepTD, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	flxGridSz->Add(boxSz, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	flxGridSz->AddStretchSpacer(1);
	stBoxSz->Add(flxGridSz, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
	captureBSizer->Add(stBoxSz, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);


	// Add captureSizer to m_capture, and m_cature to m_notebook.
	m_capture->SetSizer(captureBSizer);
	captureBSizer->Fit(m_capture);
	AddPage(m_capture, TXT_TAB_CAPTURE, true);


	// ------------------------------------------------
	// Creating content of the second tab ("hardware" settings one).
	// ------------------------------------------------
	// Sizer of m_hardware panel.
	wxBoxSizer *hardwareBSizer = new wxBoxSizer(wxVERTICAL);


	// *** First box --- Controls managing device selection.
	stBoxSz = new wxStaticBoxSizer(wxHORIZONTAL, m_hardware, TXT_STBOX_DEVICEPARAMS);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = current->Add(stBoxSz->GetStaticBox());
	wxGridBagSizer *gbSz = new wxGridBagSizer(2, 20);

	m_ctrlRawPipe = new wxCheckBox(m_hardware, CKB_RAW_PIPE, TXT_RAW_PIPE);
	m_ctrlRawPipe->SetValue(m_cth->m_rawPipe);
	m_ctrlRawPipe->SetFont(*m_ctrlFont);

	stBoxSz->Add(m_ctrlRawPipe, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 20);

	m_ctrlPort = new wxSpinCtrl(m_hardware, SPC_PORT, wxString::Format(_T("%d"), m_cth->m_port),
	                            wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                            -1, 65535, m_cth->m_port);
	m_ctrlPort->SetFont(*m_ctrlFont);
	m_ctrlNode = new wxSpinCtrl(m_hardware, SPC_NODE, wxString::Format(_T("%d"), m_cth->m_node),
	                            wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                            -1, 65535, m_cth->m_node);
	m_ctrlNode->SetFont(*m_ctrlFont);
	m_ctrlChannel = new wxSpinCtrl(m_hardware, SPC_CHANNEL, wxString::Format(_T("%d"), m_cth->m_channel),
	                               wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                               -1, 65535, m_cth->m_channel);
	m_ctrlChannel->SetFont(*m_ctrlFont);
	wxArrayString ieee1394devices;
	DiscoverIeee1394Devices(&ieee1394devices);
	m_ctrlGuid = new wxComboBox(m_hardware, CHC_GUID, ieee1394devices[0],
	                            wxDefaultPosition, wxDefaultSize, ieee1394devices);
	m_ctrlGuid->SetFont(wxFont(m_ctrlFont->GetPointSize(),
	                           wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));
	
	text = new wxStaticText(m_hardware, wxID_ANY, TXT_DV_PORT);
	text->SetFont(*m_ctrlFont);
	current = current->Add(text);
	gbSz->Add(text, wxGBPosition(0, 0), wxDefaultSpan, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 0);
	text = new wxStaticText(m_hardware, wxID_ANY, TXT_DV_NODE);
	text->SetFont(*m_ctrlFont);
	current = current->Add(text);
	gbSz->Add(text, wxGBPosition(0, 1), wxDefaultSpan, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 0);
	text = new wxStaticText(m_hardware, wxID_ANY, TXT_DV_CHANNEL);
	text->SetFont(*m_ctrlFont);
	current = current->Add(text);
	gbSz->Add(text, wxGBPosition(0, 2), wxDefaultSpan, wxALL | wxALIGN_LEFT | wxALIGN_TOP, 0);
	text = new wxStaticText(m_hardware, wxID_ANY, TXT_DV_GUID);
	text->SetFont(*m_ctrlFont);
	current = current->Add(text);
	gbSz->Add(text, wxGBPosition(2, 0), wxGBSpan(1, 3), wxALL | wxALIGN_LEFT | wxALIGN_BOTTOM, 0);
	gbSz->Add(m_ctrlPort, wxGBPosition(1, 0), wxDefaultSpan, wxALL | wxALIGN_CENTER | wxEXPAND, 0);
	gbSz->Add(m_ctrlNode, wxGBPosition(1, 1), wxDefaultSpan, wxALL | wxALIGN_CENTER | wxEXPAND, 0);
	gbSz->Add(m_ctrlChannel, wxGBPosition(1, 2), wxDefaultSpan, wxALL | wxALIGN_CENTER | wxEXPAND, 0);
	gbSz->Add(m_ctrlGuid, wxGBPosition(3, 0), wxGBSpan(1, 3), wxALL | wxALIGN_CENTER | wxEXPAND, 0);
	stBoxSz->Add(gbSz, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
	hardwareBSizer->Add(stBoxSz, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	
	// *** Second box --- Controls managing capture options.
	stBoxSz = new wxStaticBoxSizer(wxHORIZONTAL, m_hardware, TXT_STBOX_CAPTUREPARAMS);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = current->Add(stBoxSz->GetStaticBox());
	flxGridSz = new wxFlexGridSizer(2, 2, 0, 20);

	m_ctrlNoAVC = new wxCheckBox(m_hardware, CKB_NOAVC, TXT_NO_AVC);
	m_ctrlNoAVC->SetValue(m_cth->m_noAVC);
	m_ctrlNoAVC->SetFont(*m_ctrlFont);
	m_ctrlNoStop = new wxCheckBox(m_hardware, CKB_NOSTOP, TXT_NO_STOP);
	m_ctrlNoStop->SetValue(m_cth->m_noStop);
	m_ctrlNoStop->SetFont(*m_ctrlFont);
	m_ctrlBuffers = new wxSpinCtrl(m_hardware, SPC_BUFFER, wxString::Format(_T("%d"), m_cth->m_buffers),
	                               wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                               25, 1000, m_cth->m_buffers);
	m_ctrlBuffers->SetFont(*m_ctrlFont);

	flxGridSz->Add(m_ctrlNoAVC, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
	text = new wxStaticText(m_hardware, wxID_ANY, TXT_BUFFER);
	text->SetFont(*m_ctrlFont);
	current = current->Add(text);
	flxGridSz->Add(text, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
	flxGridSz->Add(m_ctrlNoStop, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
	flxGridSz->Add(m_ctrlBuffers, 1, wxALL | wxALIGN_CENTER | wxEXPAND, 0);
	stBoxSz->Add(flxGridSz, 0, wxALIGN_CENTER | wxRIGHT | wxTOP | wxBOTTOM, 10);

	m_ctrlRecVAUX = new wxCheckBox(m_hardware, CKB_REC_VAUX, TXT_REC_VAUX);
	m_ctrlRecVAUX->SetValue(m_cth->m_recVAUX);
	m_ctrlRecVAUX->SetFont(*m_ctrlFont);

	stBoxSz->Add(m_ctrlRecVAUX, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 10);
	hardwareBSizer->Add(stBoxSz, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);


	// *** Third box --- Controls managing JPEG options.
	stBoxSz = new wxStaticBoxSizer(wxHORIZONTAL, m_hardware, TXT_STBOX_JPEGPARAMS);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = current->Add(stBoxSz->GetStaticBox());
	flxGridSz = new wxFlexGridSizer(2, 4, 0, 20);

	m_ctrlJpegQ = new wxSpinCtrl(m_hardware, SPC_JPG_QUALITY, wxString::Format(_T("%d"), m_cth->m_jpegQuality),
	                             wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                             1, 100, m_cth->m_jpegQuality);
	m_ctrlJpegQ->SetFont(*m_ctrlFont);
	m_ctrlJpegW = new wxSpinCtrl(m_hardware, SPC_JPG_WIDTH, wxString::Format(_T("%d"), m_cth->m_jpegWidth),
	                             wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                             -1, 2048, m_cth->m_jpegWidth);
	m_ctrlJpegW->SetFont(*m_ctrlFont);
	m_ctrlJpegH = new wxSpinCtrl(m_hardware, SPC_JPG_HEIGHT, wxString::Format(_T("%d"), m_cth->m_jpegHeight),
	                             wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
	                             -1, 2048, m_cth->m_jpegHeight);
	m_ctrlJpegH->SetFont(*m_ctrlFont);
	m_ctrlJpegD = new wxCheckBox(m_hardware, CKB_JPG_DEINTERLACE, TXT_JPEG_DEINTERLACE);
	m_ctrlJpegD->SetValue(m_cth->m_jpegDeinterlace);
	m_ctrlJpegD->SetFont(*m_ctrlFont);
	m_ctrlJpegO = new wxCheckBox(m_hardware, CKB_JPG_OVERWRITE, TXT_JPEG_OVERWRITE);
	m_ctrlJpegO->SetValue(m_cth->m_jpegOverwrite);
	m_ctrlJpegO->SetFont(*m_ctrlFont);

	flxGridSz->Add(m_ctrlJpegD, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
	text = new wxStaticText(m_hardware, wxID_ANY, TXT_JPEG_QUALITY);
	text->SetFont(*m_ctrlFont);
	current = current->Add(text);
	flxGridSz->Add(text, 0, wxALL, 0);
	text = new wxStaticText(m_hardware, wxID_ANY, TXT_JPEG_WIDTH);
	text->SetFont(*m_ctrlFont);
	current = current->Add(text);
	flxGridSz->Add(text, 0, wxALL, 0);
	text = new wxStaticText(m_hardware, wxID_ANY, TXT_JPEG_HEIGHT);
	text->SetFont(*m_ctrlFont);
	current = current->Add(text);
	flxGridSz->Add(text, 0, wxALL, 0);
	flxGridSz->Add(m_ctrlJpegO, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
	flxGridSz->Add(m_ctrlJpegQ, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);
	flxGridSz->Add(m_ctrlJpegW, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);
	flxGridSz->Add(m_ctrlJpegH, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);
	stBoxSz->Add(flxGridSz, 0, wxTOP | wxBOTTOM, 10);
	hardwareBSizer->Add(stBoxSz, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);


	// Add captureSizer to m_capture, and m_cature to m_notebook.
	m_hardware->SetSizer(hardwareBSizer);
	captureBSizer->Fit(m_hardware);
	AddPage(m_hardware, TXT_TAB_HARDWARE, true);


	// ------------------------------------------------
	// Creating content of the third tab (shortcuts one).
	// ------------------------------------------------
	// Sizer of m_shortcuts panel.
	wxBoxSizer *shortcutsBSizer = new wxBoxSizer(wxHORIZONTAL);


	// *** First box --- Shortcuts settings.
	stBoxSz = new wxStaticBoxSizer(wxVERTICAL, m_shortcuts, TXT_STBOX_SHTCGRID);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = current->Add(stBoxSz->GetStaticBox());

	m_grid = new wxGrid(m_shortcuts, wxID_ANY, wxPoint(0,0), wxSize(300, 150),
	                    wxSUNKEN_BORDER | wxWANTS_CHARS);
	m_grid->CreateGrid(m_numSC, 2);
	m_grid->SetColSize(0, 200);
	m_grid->SetColSize(1, 100);
	m_grid->SetColLabelValue(0, TXT_SHTCGRID_MENU);
	m_grid->SetColLabelValue(1, TXT_SHTCGRID_SHORTCUTS);
	m_grid->SetDefaultCellFont(*m_ctrlFont);
	m_grid->SetLabelFont(*m_ctrlFont);
	for(int i = 0; i < m_numSC; i++)
	{
		m_grid->SetCellValue(i, 0, m_menuText[i]);
		m_grid->SetReadOnly(i, 0);
		m_grid->SetCellValue(i, 1, m_menuSC[i]);
	}

	stBoxSz->Add(m_grid, 1, wxEXPAND | wxALL, 10);
	shortcutsBSizer->Add(stBoxSz, 1, wxEXPAND | wxALL, 5);

	
	// *** Second box --- Controls managing GUI mode.
	boxSz = new wxBoxSizer(wxVERTICAL);
	stBoxSz = new wxStaticBoxSizer(wxVERTICAL, m_shortcuts, TXT_STBOX_GUIMODE);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = current->Add(stBoxSz->GetStaticBox());

	m_ctrlGUImode = new wxCheckBox(m_shortcuts, CKB_GUIMODE, TXT_GUI_MODE);
	m_ctrlGUImode->SetValue(m_cth->m_smallScreen);
	m_ctrlGUImode->SetFont(*m_ctrlFont);
	
	stBoxSz->Add(m_ctrlGUImode, 0, wxALL, 0);
	boxSz->Add(stBoxSz, 0, wxEXPAND | wxALL, 5);


	// *** Third box --- Controls managing GUI fonts.
	stBoxSz = new wxStaticBoxSizer(wxVERTICAL, m_shortcuts, TXT_STBOX_GUIFONTS);
	stBoxSz->GetStaticBox()->SetFont(*m_ctrlFont);
	current = current->Add(stBoxSz->GetStaticBox());

	m_ctrlMenuFont = new wxButton(m_shortcuts, BTT_MENUFONT, TXT_GUIFONTS_MENU);
	m_ctrlMenuFont->SetFont(*m_menuFont);
	m_ctrlStatBarFont = new wxButton(m_shortcuts, BTT_STATBARFONT, TXT_GUIFONTS_STATBAR);
	m_ctrlStatBarFont->SetFont(*m_statBarFont);
	m_ctrlSmallFont = new wxButton(m_shortcuts, BTT_SMALLFONT, TXT_GUIFONTS_SMALL);
	m_ctrlSmallFont->SetFont(*m_smallFont);
	m_ctrlCtrlFont = new wxButton(m_shortcuts, BTT_CTRLFONT, TXT_GUIFONTS_CTRL);
	m_ctrlCtrlFont->SetFont(*m_ctrlFont);

	stBoxSz->SetMinSize(250, -1);
	stBoxSz->Add(m_ctrlMenuFont, 0, wxALL | wxEXPAND, 10);
	stBoxSz->Add(m_ctrlStatBarFont, 0, wxALL | wxEXPAND, 10);
	stBoxSz->Add(m_ctrlSmallFont, 0, wxALL | wxEXPAND, 10);
	stBoxSz->Add(m_ctrlCtrlFont, 0, wxALL | wxEXPAND, 10);
	boxSz->Add(stBoxSz, 0, wxEXPAND | wxALL, 5);
	shortcutsBSizer->Add(boxSz, 0, /*wxEXPAND |*/ wxALL, 0);

	m_shortcuts->SetSizer(shortcutsBSizer);
	AddPage(m_shortcuts, TXT_TAB_SHTC_GUIFONTS, true);


	// First tab selected as default.
	SetSelection(0);

	SetFont(*m_ctrlFont);

	UpdateUI();
}


// ----------------------------------------------
// First tab (capture) event handlers.
// ----------------------------------------------

//! Event handler managing @a m_ctrlFreqImg events.
/*!
 * Used for setting the frequency of refreshing the dv frames window.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnSpinFreqImg(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_showEvery = m_ctrlFreqImg->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlShowImg events.
/*!
 * Used for activating the display of dv frames.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnCheckShowImg(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_show = m_ctrlShowImg->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlPlayBeep events.
/*!
 * Used for activating the "beep" sound when recording.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnCheckPlayBeep(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_beep = m_ctrlPlayBeep->GetValue();
	m_mutex->Unlock();
}


//! Helper func managing all @a m_ctrlSelPath events.
/*!
 * Used for selecting the directory where to store video files.
 */	
void SettingsWindow::TextPath()
{
	wxString s(m_ctrlSelPath->GetValue());
	m_mutex->Lock();
	wxString oldpath = m_cth->m_path;
	m_mutex->Unlock();
	// If no first '/' in path, prepend the cwd.
	if(s[0] != _T('/'))
	{
		s.Prepend(::wxGetCwd() + _T("/"));
	}
	// If exists, set the path in m_cth.
	if(!wxDirExists(s))
	{
		if(wxMessageBox(_("This directory doesn't exist:\n\"") + s + _("\"\nDo you want to create it?"),
		                _("Directory doesn't exist"), wxYES_NO | wxICON_EXCLAMATION) == wxYES)
			wxMkdir(s);
		else
			s = oldpath;
	}
	m_mutex->Lock();
	m_cth->m_path = s;
	m_mutex->Unlock();
	m_ctrlSelPath->SetValue(s);
}

//! Event handler managing @a m_ctrlSelPath events.
/*!
 * Used for selecting the directory where to store video files.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */	
void SettingsWindow::OnTextPath(wxCommandEvent& WXUNUSED(event))
{
	TextPath();
}

//! Event handler managing @a m_ctrlSelPath events.
/*!
 * Used for selecting the directory where to store video files.
 * Called when losing the focus (supposed to be like an "enter" action).
 * @param WXUNUSED(event) Unused wxFocusEvent event.
 */	
void SettingsWindow::OnTextPath2(wxFocusEvent& WXUNUSED(event))
{
	TextPath();
}

//! Event handler managing @a m_ctrlDiagSelPath events.
/*!
 * Used to call the system directory selector dialog, with the possibility of creating a folder.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnBttPath(wxCommandEvent& WXUNUSED(event))
{
	wxString s(wxGetenv(_T("HOME")));
	wxDirDialog *diag = new wxDirDialog(this, _("Choose the directory to store the video"),
	                                    s, wxDD_NEW_DIR_BUTTON);
	if(diag->ShowModal() == wxID_OK)
	{
		m_mutex->Lock();
		m_cth->m_path = diag->GetPath();
		m_ctrlSelPath->SetValue(m_cth->m_path);
		m_mutex->Unlock();
	}
	diag->Destroy();
}

//! Helper func managing all @a m_ctrlSelFile events.
/*!
 * Used for selecting the radical of the video file name.
 */
void SettingsWindow::TextFile()
{
	m_mutex->Lock();
	m_cth->m_file = m_ctrlSelFile->GetValue();
	m_mutex->Unlock();
	wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
	wxPostEvent(GetParent(), evt);
}

//! Event handler managing @a m_ctrlSelFile events.
/*!
 * Used for selecting the radical of the video file name.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnTextFile(wxCommandEvent& WXUNUSED(event))
{
	TextFile();
}

//! Event handler managing @a m_ctrlSelFile events.
/*!
 * Used for selecting the radical of the video file name.
 * Called when losing the focus (supposed to be like an "enter" action).
 * @param WXUNUSED(event) Unused wxFocusEvent event.
 */
void SettingsWindow::OnTextFile2(wxFocusEvent& WXUNUSED(event))
{
	TextFile();
}

//! Event handler managing @a m_ctrlFileType events.
/*!
 * Used for selecting the type of the video file (avi1, avi2, raw dv, ...).
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnChoiceFileType(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	switch(m_ctrlFileType->GetSelection())
	{
		case 0:
			m_cth->m_fileFormat = AVI_DV1_FORMAT;
			break;
		case 1:
			m_cth->m_fileFormat = AVI_DV2_FORMAT;
			break;
		case 2:
			m_cth->m_fileFormat = RAW_FORMAT;
			break;
		case 3:
			m_cth->m_fileFormat = DIF_FORMAT;
			break;
#if defined(HAVE_LIBJPEG)
		case 4:
			m_cth->m_fileFormat = JPEG_FORMAT;
			break;
#endif // defined(HAVE_LIBJPEG)
		default:
			m_cth->m_fileFormat = AVI_DV2_FORMAT;
	}

	// Updating the dv time for max file size (depends on the file format!).
	int j, i;
	wxString ts;
	if(m_ctrlMaxFSize->GetValue())
	{
		m_mutex->Unlock();
		// Fills i with numbre of PAL frames, and j with number of NTSC frames, that
		// can be stored in max file size (Mo)...
		CalcNbrFramesFromSize(i, j);
		m_mutex->Lock();
		// Formating file size (in Mo) in (raw, avi2) dv time and (pal, ntsc) number of frames.
		ts.Printf(TXT_MAXFS_INFO1_7ARGS, i/90000, (i%90000)/1500, ((i%90000)%1500)/25,
		          ((i%90000)%1500)%25, ((j%108000)%1500)%30, i, j);
	}
	else
		ts = wxString(TXT_MAXFS_INFO2);
	m_ctrlMFSInfo->SetLabel(ts);
	m_mutex->Unlock();
#if defined(HAVE_LIBJPEG)
	UpdateUI();
#endif // defined(HAVE_LIBJPEG)
}

//! Event handler managing @a m_ctrlMaxFSize events.
/*!
 * Used for setting the maximum video file size.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnSpinMaxFSize(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_maxFileSize = m_ctrlMaxFSize->GetValue();
	int j, i = m_ctrlMaxFSize->GetValue();
	wxString ts;
	if(m_ctrlMaxFSize->GetValue())
	{
		m_mutex->Unlock();
		// Fills i with numbre of PAL frames, and j with number of NTSC frames, that
		// can be stored in size i Mo...
		CalcNbrFramesFromSize(i, j);
		m_mutex->Lock();
		// Formating file size (in Mo) in (raw, avi2) dv time and (pal, ntsc) number of frames.
		ts.Printf(TXT_MAXFS_INFO1_7ARGS, i/90000, (i%90000)/1500, ((i%90000)%1500)/25,
		          ((i%90000)%1500)%25, ((j%108000)%1500)%30, i, j);
	}
	else
		ts = wxString(TXT_MAXFS_INFO2);
	m_ctrlMFSInfo->SetLabel(ts);
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlCollSize events.
/*!
 * Used for setting the collection size.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnSpinCollSize(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_collectionSize = m_ctrlCollSize->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlCollMCFSize events.
/*!
 * Used for setting the minimum size of a file in a collection.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnSpinCollMCFSize(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_collectionMinCutFileSize = m_ctrlCollMCFSize->GetValue();
	m_mutex->Unlock();
}


//! Event handler managing @a m_ctrlTimestamp events.
/*!
 * Used for adding the time stamp of the camera to video file name.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnTimestamp(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_timestamp = m_ctrlTimestamp->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlTimecode events.
/*!
 * Used for adding the timecode of the first frame to video file name.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnTimecode(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_timecode = m_ctrlTimecode->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlTimesys events.
/*!
 * Used for adding the system time to video file name.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnTimesys(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_timesys = m_ctrlTimesys->GetValue();
	m_mutex->Unlock();
}


//! Event handler managing @a m_ctrlAutosplit events.
/*!
 * Used for activating @a autosplit feature.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnCheckAutosplit(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_autosplit = m_ctrlAutosplit->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlFrameEvery events.
/*!
 * Used for setting the frequency of frames really stored.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnSpinFrameEvery(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_frameEvery = m_ctrlFrameEvery->GetValue();
	int i = m_ctrlFrameCount->GetValue() * m_cth->m_frameEvery;
	wxString ts;
	// Format the numberof frames in pal and ntsc time.
	i ? ts.Printf(TXT_FC_INFO1_8ARGS,
	              i/90000, (i%90000)/1500, ((i%90000)%1500)/25,
	              ((i%90000)%1500)%25, i/108000, (i%108000)/1800,
	              ((i%108000)%1800)/30, ((i%108000)%1800)%30)
	  : ts.Printf(TXT_FC_INFO2);
	m_ctrlFCInfo->SetLabel(ts);
	Layout();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlFrameCount events.
/*!
 * Used for setting the max number of frames to store before halting dv capture.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnSpinFrameCount(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_frameCount = m_ctrlFrameCount->GetValue();
	int i = m_cth->m_frameCount * m_ctrlFrameEvery->GetValue();
	wxString ts;
	// Format the numberof frames in pal and ntsc time.
	i ? ts.Printf(TXT_FC_INFO1_8ARGS,
	              i/90000, (i%90000)/1500, ((i%90000)%1500)/25,
	              ((i%90000)%1500)%25, i/108000, (i%108000)/1800,
	              ((i%108000)%1800)/30, ((i%108000)%1800)%30)
	  : ts.Printf(TXT_FC_INFO2);
	m_ctrlFCInfo->SetLabel(ts);
	Layout();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlLockstep events.
/*!
 * Used for (de)activating the "lockstep" feature.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnLockstep(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_lockstep = m_ctrlLockstep->GetValue();
	m_mutex->Unlock();
	UpdateUI();
}

//! Event handler managing @a m_ctrlLockstepMD events.
/*!
 * Used for setting the max number of droped frames allowed during one dv capture.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnLockstepMD(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_lockstepMaxDrops = m_ctrlLockstepMD->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlLockstepTD events.
/*!
 * Used for setting the max number of droped frames allowed over all dv captures.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnLockstepTD(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_lockstepTotalDrops = m_ctrlLockstepTD->GetValue();
	m_mutex->Unlock();
}


// ----------------------------------------------
// Second tab (hardware) event handlers.
// ----------------------------------------------

//! Event handler managing @a m_ctrlRawPipe events.
/*!
 * Used for grabing dv from "stdin" stream.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnRawPipe(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_rawPipe = m_ctrlRawPipe->GetValue();
	m_mutex->Unlock();
	UpdateUI();
}

//! Event handler managing @a m_ctrlPort events.
/*!
 * Used for setting the port to use for dv capture.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnPort(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_port = m_cth->m_backPort = m_ctrlPort->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlNode events.
/*!
 * Used for setting the node to use for dv capture.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnNode(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_node = m_cth->m_backNode = m_ctrlNode->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlChannel events.
/*!
 * Used for setting the channel to use for dv capture.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnChannel(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_channel = m_cth->m_backChannel = m_ctrlChannel->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlGuid events.
/*!
 * Used for setting the dv device to use for dv capture.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnGuid(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
	{
		// Extract the longlong value of guid from text data.
		wxRegEx reg(_T("\\A0x[0-9a-fA-F]{16}.*"), wxRE_ADVANCED);
		uint64_t i = 0;
		wxString j = m_ctrlGuid->GetValue();

		if(reg.Matches(j))
		{
			char *s = new char[16];
			wxString str(j.Mid(2, 16));
			for(int k = 0; k < 16; k++)
				s[k] = str[k];
			sscanf(s, "%llx", &i);
			delete [] s;
			if(i)
			{
				str = wxString::Format(_T("0x%16.16llx"), i);
				wxString st = m_ctrlGuid->GetString(0);
				uint k = 1;
				for(; k < (uint) m_ctrlGuid->GetCount(); k++)
				{
					st = m_ctrlGuid->GetString(k);
					if(st.StartsWith(str))
						break;
				}
				if(k == (uint) m_ctrlGuid->GetCount())
				{
					m_ctrlGuid->Append(str);
					m_ctrlGuid->SetStringSelection(str);
				}
			}
			else
				m_ctrlGuid->SetSelection(0);
		}
		else
			m_ctrlGuid->SetSelection(0);

		m_cth->m_guid = m_cth->m_backGuid = i;
	}
	m_mutex->Unlock();
}


//! Event handler managing @a m_ctrlNoAVC events.
/*!
 * Used for (de)activating the AVC feature (not really used anyway!).
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnNoAVC(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_noAVC = m_ctrlNoAVC->GetValue();
	m_mutex->Unlock();
	UpdateUI();
}

//! Event handler managing @a m_ctrlNoStop events.
/*!
 * Used for (de)activating the "Stop" command sent when halting capture.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnNoStop(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_noStop = m_ctrlNoStop->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlBuffers events.
/*!
 * Used for setting the buffer size (in dv frames).
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnBuffers(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_buffers = m_ctrlBuffers->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlRecVAUX events.
/*!
 * Used for recording VAUX datas in a file while capturing (not really usefull!).
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnRecVAUX(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_recVAUX = m_ctrlRecVAUX->GetValue();
	m_mutex->Unlock();
}


//! Event handler managing @a m_ctrlJpegD events.
/*!
 * Used for setting the JPEG images deinterlacing.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnJpegDeinterlace(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_jpegDeinterlace = m_ctrlJpegD->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlJpegO events.
/*!
 * Used for allowing the overwrite of previous JPEG files.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnJpegOverwrite(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_jpegOverwrite = m_ctrlJpegO->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlJpegQ events.
/*!
 * Used for setting the JPEG quality encoding.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnJpegQuality(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_jpegQuality = m_ctrlJpegQ->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlJpegW events.
/*!
 * Used for setting the JPEG images width.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnJpegWidth(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_jpegWidth = m_ctrlJpegW->GetValue();
	m_mutex->Unlock();
}

//! Event handler managing @a m_ctrlJpegH events.
/*!
 * Used for setting the JPEG images height.
 * @param WXUNUSED(event) Unused wxSpinEvent event.
 */
void SettingsWindow::OnJpegHeight(wxSpinEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	if(m_cth->m_running)
		m_cth->m_change_settings_pending = true;
	else
		m_cth->m_jpegHeight = m_ctrlJpegH->GetValue();
	m_mutex->Unlock();
}


// ----------------------------------------------
// Third tab (shortcuts/GUI fonts) event handlers.
// ----------------------------------------------

//! Managing Cell_Changed events from m_grid.
/*!
 * Making small checks for beeing sure that only shortcuts are modified, and that
 * the selected shortcuts isn't till in use.
 * @param event The wxGridEvent event.
 */
void SettingsWindow::OnChangeGrid(wxGridEvent& event)
{
	// If we are not in col 1 (i.e. shortcuts one), and in a real row, exit.
	if(event.GetCol() != 1)
		return;
	int row = event.GetRow();
	if(row < 0 || row >= m_numSC)
		return;
	wxString tstr = m_grid->GetCellValue(row, 1);
	
	// "Erasing" a shortcut!
	if(tstr == _T(""))
	{
		m_menuSC[row] = _T("");
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
		wxPostEvent(GetParent(), evt);
		return;
	}

	// Verifying if the typed shortcut is valid.
	wxString sc1, sc2, sc3, sc4; // Max. four parts in a shortcut ("ctrl+alt+shift+A").
	wxString regex(_T("(\\s)*F(1[0-2]|0?[1-9])(\\s)*"
	                  "|(\\s)*[a-zA-Z]{1}(\\s)*"
	                  "|(\\s)*enter(\\s)*"));
	wxRegEx re(regex, wxRE_EXTENDED | wxRE_ICASE); // Ignore case.
	wxStringTokenizer stk(tstr, _T("\t\r\n +-"), wxTOKEN_STRTOK);
	while(stk.HasMoreTokens())
	{
		wxString s = stk.GetNextToken();
		if(s.IsSameAs(_T("Shift"), false) || s.IsSameAs(_T("s_"), false))
			sc1 = _T("Shift+");
		else if(s.IsSameAs(_T("Ctrl"), false) || s.IsSameAs(_T("c_"), false))
			sc2 = _T("Ctrl+");
		else if(s.IsSameAs(_T("Alt"), false) || s.IsSameAs(_T("a_"), false))
			sc3 = _T("Alt+");
		else if(re.Matches(s))
			sc4 = re.GetMatch(s);
	}
	// This part of the shortcut is mandatory.
	if(sc4.IsEmpty())
	{
		wxMessageBox(_("Warning: \"") + sc1 + sc2 + sc3 + _("\" is not a valid shortcut!"),
		             _("Invalid Shortcut!"), wxOK | wxICON_ERROR);
		m_grid->SetCellValue(row, 1, m_menuSC[row]);
		return;
	}
	// Make sc4 "Enter" if enter.
	if(sc4.IsSameAs(_T("Enter"), false))
	{
		sc4.MakeLower();
		sc4[0] = _T('E');
	}
	// Make sc4 UpperCase if alpha or Fxx.
	else
		sc4.MakeUpper();
	// Remove first '0' if F0x.
	if(sc4[1] == _T('0'))
	{
		sc4[1] = sc4[2];
		sc4.Truncate(2);
	}
	if((sc1 + sc2 + sc3 + sc4).Length() == 1)
	{
		wxMessageBox(_("Warning: Char only shortcuts are not allowed!"),
		             _("Invalid Shortcut!"), wxOK | wxICON_ERROR);
		m_grid->SetCellValue(row, 1, m_menuSC[row]);
		return;
	}

	m_menuSC[row] = sc1 + sc2 + sc3 + sc4;
	// Verifying if the typed shortcut isn't in use; if case, erase one of them.
	m_grid->SetCellValue(row, 1, m_menuSC[row]);
	for (int i = 0; i < row; i++)
	{
		if(m_menuSC[row] == m_grid->GetCellValue(i, 1))
		{
			wxString t = m_grid->GetCellValue(i, 0);
			wxMessageDialog *diag = new wxMessageDialog(this, _("Overwrite existing shortcut?\n"
			                                                    "already assigned to: ") + m_menuText[i],
			                                            _("Warning"),
			                                            wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION);
			if(diag->ShowModal() == wxID_YES)
				m_grid->SetCellValue(i, 1, (m_menuSC[i] = _T("")));
			else
			{
				m_grid->SetCellValue(row, 1, (m_menuSC[row] = _T("")));
				return;
			}
			
		}
	}
	for (int i = row+1; i < m_numSC; i++)
	{
		if(m_menuSC[row] == m_grid->GetCellValue(i, 1))
		{
			wxString t = m_grid->GetCellValue(i, 0);
			wxMessageDialog *diag = new wxMessageDialog(this, _("Overwrite existing shortcut?\n"
			                                                    "already assigned to: ") + m_menuText[i],
			                                            _("Warning"),
			                                            wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION);
			if(diag->ShowModal() == wxID_YES)
				m_grid->SetCellValue(i, 1, (m_menuSC[i] = _T("")));
			else
			{
				m_grid->SetCellValue(row, 1, (m_menuSC[row] = _T("")));
				return;
			}
		}
	}

	wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
	wxPostEvent(GetParent(), evt);
}


//! Event handler managing @a m_ctrlGUImode events.
/*!
 * Used for setting GUI mode ("normal" or "small screen").
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnGuiMode(wxCommandEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	m_cth->m_smallScreen = m_ctrlGUImode->GetValue();
	m_mutex->Unlock();
	wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
	wxPostEvent(GetGrandParent(), evt);
}


//! Event handler managing @a m_ctrlMenuFont events.
/*!
 * Used for setting the "menu" font (Not working, for now!).
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnMenuFont(wxCommandEvent& WXUNUSED(event))
{
	wxFontData fdata;
	fdata.SetInitialFont(*m_menuFont);
// If wxWidgets 2.8 or sup.
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 8) || wxMAJOR_VERSION > 2
	wxFontDialog fdiag(this, fdata);
#else  // wxWidgets 2.6 (not suporting lower anyway!)
	wxFontDialog fdiag(this, &fdata);
#endif // wxWidgets 2.8 or sup.
	if(fdiag.ShowModal() == wxID_OK)
	{
		*m_menuFont = fdiag.GetFontData().GetChosenFont();
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
		wxPostEvent(GetGrandParent(), evt);
	}
}

//! Event handler managing @a m_ctrlStatBarFont events.
/*!
 * Used for setting the "status bar" font.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnStatBarFont(wxCommandEvent& WXUNUSED(event))
{
	wxFontData fdata;
	fdata.SetInitialFont(*m_statBarFont);
// If wxWidgets 2.8 or sup.
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 8) || wxMAJOR_VERSION > 2
	wxFontDialog fdiag(this, fdata);
#else  // wxWidgets 2.6 (not suporting lower anyway!)
	wxFontDialog fdiag(this, &fdata);
#endif // wxWidgets 2.8 or sup.
	if(fdiag.ShowModal() == wxID_OK)
	{
		*m_statBarFont = fdiag.GetFontData().GetChosenFont();
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
		wxPostEvent(GetGrandParent(), evt);
	}
}

//! Event handler managing @a m_ctrlSmallFont events.
/*!
 * Used for setting the "small" font.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnSmallFont(wxCommandEvent& WXUNUSED(event))
{
	wxFontData fdata;
	fdata.SetInitialFont(*m_smallFont);
// If wxWidgets 2.8 or sup.
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 8) || wxMAJOR_VERSION > 2
	wxFontDialog fdiag(this, fdata);
#else  // wxWidgets 2.6 (not suporting lower anyway!)
	wxFontDialog fdiag(this, &fdata);
#endif // wxWidgets 2.8 or sup.
	if(fdiag.ShowModal() == wxID_OK)
	{
		*m_smallFont = fdiag.GetFontData().GetChosenFont();
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
		wxPostEvent(GetGrandParent(), evt);
	}
}

//! Event handler managing @a m_ctrlCtrlFont events.
/*!
 * Used for setting the "control" font.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void SettingsWindow::OnCtrlFont(wxCommandEvent& WXUNUSED(event))
{
	wxFontData fdata;
	fdata.SetInitialFont(*m_ctrlFont);
// If wxWidgets 2.8 or sup.
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 8) || wxMAJOR_VERSION > 2
	wxFontDialog fdiag(this, fdata);
#else  // wxWidgets 2.6 (not suporting lower anyway!)
	wxFontDialog fdiag(this, &fdata);
#endif // wxWidgets 2.8 or sup.
	if(fdiag.ShowModal() == wxID_OK)
	{
		*m_ctrlFont = fdiag.GetFontData().GetChosenFont();
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, UPDATE_ALL_WINDOWS);
		wxPostEvent(GetGrandParent(), evt);
	}
}

// ----------------------------------------------
// Helper functions.
// ----------------------------------------------

//! Managing the activation/deactivation of controls depending on state of others.
void SettingsWindow::UpdateUI()
{
	if(m_ctrlRawPipe->GetValue())
	{
		m_ctrlPort->Enable(false);
		m_ctrlNode->Enable(false);
		m_ctrlChannel->Enable(false);
		m_ctrlGuid->Enable(false);
	}
	else
	{
		m_ctrlPort->Enable(true);
		m_ctrlNode->Enable(true);
		m_ctrlChannel->Enable(true);
		m_ctrlGuid->Enable(true);
	}

	if(m_ctrlNoAVC->GetValue())
		m_ctrlNoStop->Enable(false);
	else
		m_ctrlNoStop->Enable(true);

	if(m_ctrlLockstep->GetValue())
	{
		m_ctrlLockstepMD->Enable(true);
		m_ctrlLockstepTD->Enable(true);
	}
	else
	{
		m_ctrlLockstepMD->Enable(false);
		m_ctrlLockstepTD->Enable(false);
	}
#if defined(HAVE_LIBJPEG)
	if(m_ctrlFileType->GetSelection()==4)
	{
		m_ctrlJpegQ->Enable(true);
		m_ctrlJpegW->Enable(true);
		m_ctrlJpegH->Enable(true);
		m_ctrlJpegD->Enable(true);
		m_ctrlJpegO->Enable(true);
	}
	else
	{
		m_ctrlJpegQ->Enable(false);
		m_ctrlJpegW->Enable(false);
		m_ctrlJpegH->Enable(false);
		m_ctrlJpegD->Enable(false);
		m_ctrlJpegO->Enable(false);
	}
#else
	m_ctrlJpegQ->Enable(false);
	m_ctrlJpegW->Enable(false);
	m_ctrlJpegH->Enable(false);
	m_ctrlJpegD->Enable(false);
	m_ctrlJpegO->Enable(false);
#endif // defined(HAVE_LIBJPEG)
}

//! Helper function udating all text in UI.
/*!
 * Note that this function isn't only directly called internaly:
 * it is also called from the parent window, when __ALL__ the GUI
 * needs to be refreshed (language change, font change, ...).
 *
 * @param from_cth Used to force the controls to refresh there values from the ThreadInOut object
 * (when a new setting file is loaded, for example).
 * Note that the access to m_cth must be safe (either capture thread stoped, or m_mutex locked).
 */
void SettingsWindow::UpdateAll(bool from_cth)
{
	SetPageText(0, TXT_TAB_CAPTURE);
	SetPageText(1, TXT_TAB_HARDWARE);
	SetPageText(2, TXT_TAB_SHTC_GUIFONTS);
	SetFont(*m_ctrlFont);

	m_mutex->Lock();
	m_ctrlSelFile->SetValue(m_cth->m_file);
	wxString ieee = m_ctrlGuid->GetValue();
	if(from_cth)
	{
		m_ctrlFreqImg->SetValue(m_cth->m_showEvery);
		m_ctrlShowImg->SetValue(m_cth->m_show);
		m_ctrlPlayBeep->SetValue(m_cth->m_beep);

		m_ctrlSelPath->SetValue(m_cth->m_path);
		m_ctrlSelFile->SetValue(m_cth->m_file);
		switch(m_cth->m_fileFormat)
		{
			case AVI_DV1_FORMAT:
				m_ctrlFileType->SetSelection(0);
				break;
			case AVI_DV2_FORMAT:
				m_ctrlFileType->SetSelection(1);
				break;
			case RAW_FORMAT:
				m_ctrlFileType->SetSelection(2);
				break;
			case DIF_FORMAT:
				m_ctrlFileType->SetSelection(3);
				break;
#if defined(HAVE_LIBJPEG)
			case JPEG_FORMAT:
				m_ctrlFileType->SetSelection(4);
				break;
#endif // defined(HAVE_LIBJPEG)
			default:
				m_ctrlFileType->SetSelection(1);
		}
		m_ctrlMaxFSize->SetValue(m_cth->m_maxFileSize);
		m_ctrlCollSize->SetValue(m_cth->m_collectionSize);
		m_ctrlCollMCFSize->SetValue(m_cth->m_collectionMinCutFileSize);

		m_ctrlTimestamp->SetValue(m_cth->m_timestamp);
		m_ctrlTimecode->SetValue(m_cth->m_timecode);
		m_ctrlTimesys->SetValue(m_cth->m_timesys);

		m_ctrlFrameEvery->SetValue(m_cth->m_frameEvery);
		m_ctrlAutosplit->SetValue(m_cth->m_autosplit);
		m_ctrlFrameCount->SetValue(m_cth->m_frameCount);
		m_ctrlLockstep->SetValue(m_cth->m_lockstep);
		m_ctrlLockstepMD->SetValue(m_cth->m_lockstepMaxDrops);
		m_ctrlLockstepTD->SetValue(m_cth->m_lockstepTotalDrops);

		m_ctrlRawPipe->SetValue(m_cth->m_rawPipe);
		m_ctrlPort->SetValue(m_cth->m_port);
		m_ctrlNode->SetValue(m_cth->m_node);
		m_ctrlChannel->SetValue(m_cth->m_channel);
		ieee.Format(_T("0x%llx"), m_cth->m_guid);
	
		m_ctrlNoAVC->SetValue(m_cth->m_noAVC);
		m_ctrlNoStop->SetValue(m_cth->m_noStop);
		m_ctrlBuffers->SetValue(m_cth->m_buffers);
		m_ctrlRecVAUX->SetValue(m_cth->m_recVAUX);

		m_ctrlJpegQ->SetValue(m_cth->m_jpegQuality);
		m_ctrlJpegW->SetValue(m_cth->m_jpegWidth);
		m_ctrlJpegH->SetValue(m_cth->m_jpegHeight);
		m_ctrlJpegD->SetValue(m_cth->m_jpegDeinterlace);
		m_ctrlJpegO->SetValue(m_cth->m_jpegOverwrite);
	}
	m_mutex->Unlock();

	// ------------------------------------------------
	// First "tab" controls.
	// ------------------------------------------------
	wxString ts;
	int i, j;

	m_ctrlFreqImg->SetFont(*m_ctrlFont);
	m_ctrlShowImg->SetLabel(TXT_SHOW_IMG);
	m_ctrlShowImg->SetFont(*m_ctrlFont);
	m_ctrlPlayBeep->SetLabel(TXT_PLAY_BEEP);
	m_ctrlPlayBeep->SetFont(*m_ctrlFont);

	m_ctrlSelPath->SetFont(*m_ctrlFont);
	m_ctrlDiagSelPath->SetFont(*m_ctrlFont);
	m_ctrlDiagSelPath->SetLabel(_T("..."));
	m_ctrlSelFile->SetFont(*m_ctrlFont);
	m_ctrlFileType->SetString(0, TXT_FILETYPE_AVI1);
	m_ctrlFileType->SetString(1, TXT_FILETYPE_AVI2);
	m_ctrlFileType->SetString(2, TXT_FILETYPE_RAWDV);
	m_ctrlFileType->SetString(3, TXT_FILETYPE_RAWDIFF);
	m_ctrlFileType->SetString(4, TXT_FILETYPE_JPEG);
	m_ctrlFileType->SetFont(*m_ctrlFont);
	m_ctrlMaxFSize->SetFont(*m_ctrlFont);
	if(m_ctrlMaxFSize->GetValue())
	{
		// Fills i with numbre of PAL frames, and j with number of NTSC frames, that
		// can be stored in max file size (Mo)...
		CalcNbrFramesFromSize(i, j);
		// Formating file size (in Mo) in (raw, avi2) dv time and (pal, ntsc) number of frames.
		ts.Printf(TXT_MAXFS_INFO1_7ARGS, i/90000, (i%90000)/1500, ((i%90000)%1500)/25,
		          ((i%90000)%1500)%25, ((j%108000)%1500)%30, i, j);
	}
	else
		ts = wxString(TXT_MAXFS_INFO2);
	m_ctrlMFSInfo->SetLabel(ts);
	m_ctrlMFSInfo->SetFont(*m_ctrlFont);
	m_ctrlCollSize->SetFont(*m_ctrlFont);
	m_ctrlCollMCFSize->SetFont(*m_ctrlFont);

	m_ctrlTimestamp->SetLabel(TXT_TIME_STAMP);
	m_ctrlTimestamp->SetFont(*m_ctrlFont);
	m_ctrlTimecode->SetLabel(TXT_TIMECODE);
	m_ctrlTimecode->SetFont(*m_ctrlFont);
	m_ctrlTimesys->SetLabel(TXT_TIME_SYS);
	m_ctrlTimesys->SetFont(*m_ctrlFont);

	m_ctrlFrameEvery->SetFont(*m_ctrlFont);
	m_ctrlAutosplit->SetLabel(TXT_AUTO_SPLIT);
	m_ctrlAutosplit->SetFont(*m_ctrlFont);
	m_ctrlFrameCount->SetFont(*m_ctrlFont);
	i = m_ctrlFrameCount->GetValue() * m_ctrlFrameEvery->GetValue();
	// Formating number of frames in pal and ntsc time.
	i ? ts.Printf(TXT_FC_INFO1_8ARGS,
	              i/90000, (i%90000)/1500, ((i%90000)%1500)/25,
	              ((i%90000)%1500)%25, i/108000, (i%108000)/1800,
	              ((i%108000)%1800)/30, ((i%108000)%1800)%30)
	  : ts.Printf(TXT_FC_INFO2);
	m_ctrlFCInfo->SetLabel(ts);
	m_ctrlFCInfo->SetFont(*m_ctrlFont);
	m_ctrlLockstep->SetLabel(TXT_LOCKSTEP);
	m_ctrlLockstep->SetFont(*m_ctrlFont);
	m_ctrlLockstepMD->SetFont(*m_ctrlFont);
	m_ctrlLockstepTD->SetFont(*m_ctrlFont);


	// ------------------------------------------------
	// Second "tab" controls.
	// ------------------------------------------------
	m_ctrlRawPipe->SetLabel(TXT_RAW_PIPE);
	m_ctrlRawPipe->SetFont(*m_ctrlFont);
	m_ctrlPort->SetFont(*m_ctrlFont);
	m_ctrlNode->SetFont(*m_ctrlFont);
	m_ctrlChannel->SetFont(*m_ctrlFont);

	m_ctrlGuid->Clear();
	wxArrayString ieee1394devices;
	DiscoverIeee1394Devices(&ieee1394devices);
	m_ctrlGuid->Append(ieee1394devices);
	m_ctrlGuid->SetSelection(0);          //First, default selection (0x0000000000000000),
	m_ctrlGuid->SetStringSelection(ieee); //then the last selected device, if still present!
	m_ctrlGuid->SetFont(wxFont(m_ctrlFont->GetPointSize(),
	                           wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));

	m_ctrlNoAVC->SetLabel(TXT_NO_AVC);
	m_ctrlNoAVC->SetFont(*m_ctrlFont);
	m_ctrlNoStop->SetLabel(TXT_NO_STOP);
	m_ctrlNoStop->SetFont(*m_ctrlFont);
	m_ctrlBuffers->SetFont(*m_ctrlFont);
	m_ctrlRecVAUX->SetLabel(TXT_REC_VAUX);
	m_ctrlRecVAUX->SetFont(*m_ctrlFont);

	m_ctrlJpegQ->SetFont(*m_ctrlFont);
	m_ctrlJpegW->SetFont(*m_ctrlFont);
	m_ctrlJpegH->SetFont(*m_ctrlFont);
	m_ctrlJpegD->SetLabel(TXT_JPEG_DEINTERLACE);
	m_ctrlJpegD->SetFont(*m_ctrlFont);
	m_ctrlJpegO->SetLabel(TXT_JPEG_OVERWRITE);
	m_ctrlJpegO->SetFont(*m_ctrlFont);


	// ------------------------------------------------
	// Third "tab" controls.
	// ------------------------------------------------
	m_grid->SetColLabelValue(0, TXT_SHTCGRID_MENU);
	m_grid->SetColLabelValue(1, TXT_SHTCGRID_SHORTCUTS);
	m_grid->SetDefaultCellFont(*m_ctrlFont);
	m_grid->SetLabelFont(*m_ctrlFont);
	int d = m_numSC - m_grid->GetNumberRows();
	if(d > 0)
		m_grid->AppendRows(d);
	else if (d < 0)
		m_grid->DeleteRows(0, -d);
	for(int i = 0; i < m_numSC; i++)
	{
		m_grid->SetCellValue(i, 0, m_menuText[i]);
		m_grid->SetReadOnly(i, 0);
		m_grid->SetCellValue(i, 1, m_menuSC[i]);
	}

	m_ctrlGUImode->SetLabel(TXT_GUI_MODE);
	m_ctrlGUImode->SetFont(*m_ctrlFont);

	m_ctrlMenuFont->SetLabel(TXT_GUIFONTS_MENU);
	m_ctrlMenuFont->SetFont(*m_menuFont);
	m_ctrlStatBarFont->SetLabel(TXT_GUIFONTS_STATBAR);
	m_ctrlStatBarFont->SetFont(*m_statBarFont);
	m_ctrlSmallFont->SetLabel(TXT_GUIFONTS_SMALL);
	m_ctrlSmallFont->SetFont(*m_smallFont);
	m_ctrlCtrlFont->SetLabel(TXT_GUIFONTS_CTRL);
	m_ctrlCtrlFont->SetFont(*m_ctrlFont);

	// Updating all wxStaticTexts and wxStaticBoxs 'stored' in ControlCL.
	// Have to do it twice, because of problems with wxStaticBoxs if font not changed!
	for(int i = -1; i < 2; i += 2)
	{
		m_ctrlFont->SetPointSize(m_ctrlFont->GetPointSize() + i);
		ControlCL *current = m_firstCtrl;
		current = current->Update(TXT_STBOX_DVDISPLAY, *m_ctrlFont);
		current = current->Update(TXT_FREQ_IMG, *m_ctrlFont);
		current = current->Update(TXT_STBOX_VIDEOPARAMS, *m_ctrlFont);
		current = current->Update(TXT_SEL_PATH, *m_ctrlFont);
		current = current->Update(TXT_FILE_TYPE, *m_ctrlFont);
		current = current->Update(TXT_MAXFS, *m_ctrlFont);
		current = current->Update(TXT_COLL_MAXCFS, *m_ctrlFont);
		current = current->Update(TXT_COLL_MINFS, *m_ctrlFont);
		current = current->Update(TXT_STBOX_FILENAMEPARAMS, *m_ctrlFont);
		current = current->Update(TXT_STBOX_RECORDINGPARAMS, *m_ctrlFont);
		current = current->Update(TXT_FRAME_EVERY, *m_ctrlFont);
		current = current->Update(TXT_FRAME_COUNT, *m_ctrlFont);
		current = current->Update(TXT_LOCKSTEP_MAXD, *m_ctrlFont);
		current = current->Update(TXT_LOCKSTEP_TOTD, *m_ctrlFont);
		current = current->Update(TXT_STBOX_DEVICEPARAMS, *m_ctrlFont);
		current = current->Update(TXT_DV_PORT, *m_ctrlFont);
		current = current->Update(TXT_DV_NODE, *m_ctrlFont);
		current = current->Update(TXT_DV_CHANNEL, *m_ctrlFont);
		current = current->Update(TXT_DV_GUID, *m_ctrlFont);
		current = current->Update(TXT_STBOX_CAPTUREPARAMS, *m_ctrlFont);
		current = current->Update(TXT_BUFFER, *m_ctrlFont);
		current = current->Update(TXT_STBOX_JPEGPARAMS, *m_ctrlFont);
		current = current->Update(TXT_JPEG_QUALITY, *m_ctrlFont);
		current = current->Update(TXT_JPEG_WIDTH, *m_ctrlFont);
		current = current->Update(TXT_JPEG_HEIGHT, *m_ctrlFont);
		current = current->Update(TXT_STBOX_SHTCGRID, *m_ctrlFont);
		current = current->Update(TXT_STBOX_GUIMODE, *m_ctrlFont);
		current = current->Update(TXT_STBOX_GUIFONTS, *m_ctrlFont);
	}

// Finally, complete GUI refresh.
	m_capture->GetSizer()->Layout();
	m_hardware->GetSizer()->Layout();
	m_shortcuts->GetSizer()->Layout();
//	Fit();
//	GetSizer()->Fit(this);
//	GetSizer()->SetDimension(0, 0, GetClientSize().x, GetClientSize().y);
//	GetParent()->SetClientSize(GetSize());
	UpdateUI();
}

//! Helper function udating the ThreadInOut object.
/*!
 * Here, we update ThreadInOut values from the controls' values.
 * Used when the user change some settings that cannot be really
 * applied while the capture thread is running.
 */
void SettingsWindow::UpdateCTH()
{
	// Capture thread is supposed to be halted, but...
	m_mutex->Lock();
	if(!m_cth->m_change_settings_pending || m_cth->m_running)
	{
		// Nothing to do, return!
		m_mutex->Unlock();
		return;
	}

	m_cth->m_rawPipe = m_ctrlRawPipe->GetValue();
	m_cth->m_backPort = m_cth->m_port = m_ctrlPort->GetValue();
	m_cth->m_backNode = m_cth->m_node = m_ctrlNode->GetValue();
	m_cth->m_backChannel = m_cth->m_channel = m_ctrlChannel->GetValue();
	// Extract the longlong value of guid from text data.
	uint64_t i = 0;
	wxString j = m_ctrlGuid->GetValue();
	if(j.ToUTF8())
	{
		char *s = new char[16];
		wxString str(j.Mid(2, 16));
		for(int k = 0; k < 16; k++)
			s[k] = str[k];
		sscanf(s, "%llx", &i);
		delete [] s;
	}
	m_cth->m_backGuid = m_cth->m_guid = i;

	m_cth->m_noAVC = m_ctrlNoAVC->GetValue();
	m_cth->m_noStop = m_ctrlNoStop->GetValue();
	m_cth->m_buffers = m_ctrlBuffers->GetValue();
	m_cth->m_recVAUX = m_ctrlRecVAUX->GetValue();

	m_cth->m_lockstep = m_ctrlLockstep->GetValue();
	m_cth->m_lockstepMaxDrops = m_ctrlLockstepMD->GetValue();
	m_cth->m_lockstepTotalDrops = m_ctrlLockstepTD->GetValue();

	m_cth->m_jpegQuality = m_ctrlJpegQ->GetValue();
	m_cth->m_jpegWidth = m_ctrlJpegW->GetValue();
	m_cth->m_jpegHeight = m_ctrlJpegH->GetValue();
	m_cth->m_jpegDeinterlace = m_ctrlJpegD->GetValue();
	m_cth->m_jpegOverwrite = m_ctrlJpegO->GetValue();

	m_cth->m_timecode = m_ctrlTimecode->GetValue();
	m_cth->m_timestamp = m_ctrlTimestamp->GetValue();
	m_cth->m_timesys = m_ctrlTimesys->GetValue();

	// No more changes pending!
	m_cth->m_change_settings_pending = false;
	m_mutex->Unlock();
}


//! Helper function discovering ieee1394 devices.
/*!
 * Here we explore the device directory of bus ieee1394, using a small regex
 * to find all available devices, and then propose them to user via m_ctrlGuid.
 * @param dev A wxArrayString storing all devices name found.
 */
void SettingsWindow::DiscoverIeee1394Devices(wxArrayString* dev)
{
	dev->Add(_T("0x0000000000000000"));
	wxRegEx reg(_T("\\A(0x)?[0-9a-fA-F]{16}\\Z"), wxRE_ADVANCED);
	wxDir dir(_T("/sys/bus/ieee1394/devices"));
	if(dir.IsOpened())
	{
		dev->Add(_T(""));
		if(dir.GetFirst(&dev->Last(), _T(""), wxDIR_DIRS))
		{
			dev->Add(_T(""));
			while(dir.GetNext(&dev->Last()))
			{
				dev->Add(_T(""));
			}
		}
		for(unsigned int i = 1; i < dev->GetCount(); )
		{
			if(reg.Matches((*dev)[i]))
			{
				// Known to work on Fedora 5, 6, 7.
				wxString st1(_T("/sys/bus/ieee1394/devices/"));
				st1 += (*dev)[i] + wxString(_T("/vendor_oui"));
				// Known to work on Debian 4.0.
				wxString st2(_T("/sys/bus/ieee1394/devices/"));
				st2 += (*dev)[i] + wxString(_T("/vendor_name_kv"));
				if(::wxFileExists(st1))
				{
					wxTextFile f(st1);
					if(f.Open())
						(*dev)[i] += wxString(_T("  ***  ")) + f.GetFirstLine();
				}
				else if(::wxFileExists(st2))
				{
					wxTextFile f(st2);
					if(f.Open())
						(*dev)[i] += wxString(_T("  ***  ")) + f.GetFirstLine();
				}

				(*dev)[i].Prepend(_T("0x"));
				i++;
			}
			else
				dev->RemoveAt(i, 1);
		}
		return;
	}
	// New directory name in Fedora 9, for the new firewire stack?
	dir.Open(_T("/sys/bus/firewire/devices"));
	if(dir.IsOpened())
	{
		dev->Add(_T(""));
		if(dir.GetFirst(&dev->Last(), _T(""), wxDIR_DIRS))
		{
			dev->Add(_T(""));
			while(dir.GetNext(&dev->Last()))
			{
				dev->Add(_T(""));
			}
		}
		for(unsigned int i = 1; i < dev->GetCount(); )
		{
			wxString guid_file(_T("/sys/bus/firewire/devices/"));
			guid_file += (*dev)[i] + wxString(_T("/guid"));
			wxString guid;
			if(::wxFileExists(guid_file))
			{
				wxTextFile f(guid_file);
				if(f.Open())
					guid = f.GetFirstLine();
			}
			if(reg.Matches(guid))
			{
				// Known to work on Fedora 9.
				wxString st1(_T("/sys/bus/firewire/devices/"));
				st1 += (*dev)[i] + wxString(_T("/vendor_name"));
				wxString st2(_T("/sys/bus/firewire/devices/"));
				st2 += (*dev)[i] + wxString(_T("/model_name"));
				(*dev)[i].Prepend(guid + wxString(_T("  ***  (")));
				(*dev)[i] += _T(") - ");
				if(::wxFileExists(st1))
				{
					wxTextFile f(st1);
					if(f.Open())
						(*dev)[i] += f.GetFirstLine() + wxString(_T(" "));
				}
				if(::wxFileExists(st2))
				{
					wxTextFile f(st2);
					if(f.Open())
						(*dev)[i] += f.GetFirstLine();
				}

				i++;
			}
			else
				dev->RemoveAt(i, 1);
		}
		return;
	}
}

//! Helper function calculating the (min) number of frames allowed by a max file size.
/*!
 * Here we try to figure out how much frames a certain file size can contain,
 * based on the standart (PAL/NTSC), and the codec (raw dv, AVI2, or JPEG).
 * Note that Jpeg is a worst estimation!
 * @param[out] &i The Pal number of frames.
 * @param[out] &j The NTSC number of frames.
 */
void SettingsWindow::CalcNbrFramesFromSize(int &i, int &j)
{
	m_mutex->Lock();
	bool avi2 = m_cth->m_fileFormat == AVI_DV2_FORMAT;
#if defined(HAVE_LIBJPEG)
	bool jpeg = m_cth->m_fileFormat == JPEG_FORMAT;
#endif // defined(HAVE_LIBJPEG)
	i = m_ctrlMaxFSize->GetValue();
	
	if(avi2)
	{
		j = int(i * 8.295696203); /* = * 1024*1024/126400(NTSC dv frame + audio size)*/
		i = int(i * 6.913080169); /* = * 1024*1024/151680(PAL dv frame + audio size)*/
	}
#if defined(HAVE_LIBJPEG)
	else if(jpeg)
	{
		if(90 < m_cth->m_jpegQuality)
		{
			j = int(i * 1.992569972); /* = * 1024*1024/526243(noise_worst NTSC 100_jpeg pict)*/
			i = int(i * 1.696508052); /* = * 1024*1024/618079(noise_worst PAL 100_jpeg pict)*/
		}
		else if(80 < m_cth->m_jpegQuality)
		{
			j = int(i * 3.266276466); /* = * 1024*1024/321031(noise_worst NTSC 90_jpeg pict)*/
			i = int(i * 2.779341329); /* = * 1024*1024/377275(noise_worst PAL 90_jpeg pict)*/
		}
		else if(70 < m_cth->m_jpegQuality)
		{
			j = int(i * 4.123171053); /* = * 1024*1024/254313(noise_worst NTSC 80_jpeg pict)*/
			i = int(i * 3.50862118); /* = * 1024*1024/298857(noise_worst PAL 80_jpeg pict)*/
		}
		else if(60 < m_cth->m_jpegQuality)
		{
			j = int(i * 4.846508317); /* = * 1024*1024/216357(noise_worst NTSC 70_jpeg pict)*/
			i = int(i * 4.119833883); /* = * 1024*1024/254519(noise_worst PAL 70_jpeg pict)*/
		}
		else if(50 < m_cth->m_jpegQuality)
		{
			j = int(i * 5.568409202); /* = * 1024*1024/188308(noise_worst NTSC 60_jpeg pict)*/
			i = int(i * 4.734426289); /* = * 1024*1024/221479(noise_worst PAL 60_jpeg pict)*/
		}
		else if(40 < m_cth->m_jpegQuality)
		{
			j = int(i * 6.301349118); /* = * 1024*1024/166405(noise_worst NTSC 50_jpeg pict)*/
			i = int(i * 5.356819552); /* = * 1024*1024/195746(noise_worst PAL 50_jpeg pict)*/
		}
		else if(30 < m_cth->m_jpegQuality)
		{
			j = int(i * 7.279401862); /* = * 1024*1024/144047(noise_worst NTSC 40_jpeg pict)*/
			i = int(i * 6.193450834); /* = * 1024*1024/169304(noise_worst PAL 40_jpeg pict)*/
		}
		else if(20 < m_cth->m_jpegQuality)
		{
			j = int(i * 8.887442365); /* = * 1024*1024/117984(noise_worst NTSC 30_jpeg pict)*/
			i = int(i * 7.570125979); /* = * 1024*1024/138515(noise_worst PAL 30_jpeg pict)*/
		}
		else if(10 < m_cth->m_jpegQuality)
		{
			j = int(i * 12.381489922); /* = * 1024*1024/84689(noise_worst NTSC 20_jpeg pict)*/
			i = int(i * 10.50487888); /* = * 1024*1024/99818(noise_worst PAL 20_jpeg pict)*/
		}
		else
		{
			j = int(i * 20.958944633); /* = * 1024*1024/50030(noise_worst NTSC 10_jpeg pict)*/
			i = int(i * 17.818985148); /* = * 1024*1024/58846(noise_worst PAL 10_jpeg pict)*/
		}
	}
#endif // defined(HAVE_LIBJPEG)
	else
	{
		j = int(i * 8.738133333); /* = * 1024*1024/120000(NTSC dv frame size)*/
		i = int(i * 7.281777778); /* = * 1024*1024/144000(PAL dv frame size)*/
	}
	m_mutex->Unlock();
}
