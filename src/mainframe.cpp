/*!
 * @file mainframe.cpp
 * @brief DVGrabGUI main frame class implementation.
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

#include "mainframe.h"

//! @name Defining all texts I use several times, easyer to do it once here!
//@{
// Macro converting the "text" representation of enums in a real wxString!
#define ETS(id) wxString(_T(#id)) // ETS = EnumToString.

// Standard paths ...
#define LARGE_ICON wxString(DVGRABGUI_SETTING_PATH) + wxString(_T("dvgrabgui32x32.xpm"))
#define SMALL_ICON wxString(DVGRABGUI_SETTING_PATH) + wxString(_T("dvgrabgui16x16.xpm"))
#define FILE_LOG wxString(wxGetenv(_T("HOME"))) + wxString(_T("/.dvgrabgui/dvgrabgui.log"))
#define FILE_DEFAULT_DVS wxString(wxGetenv(_T("HOME"))) + wxString(_T("/.dvgrabgui/default.dvs.xml"))
#define FILE_HELP wxString(_("en_GB/help/help_dvgrabgui_en_GB.hhp"))
#define DIR_HELP_TEMP wxString(wxGetenv(_T("HOME"))) + wxString(_T("/.dvgrabgui/help/"))

// GUI texts.
// Menu features.
#define NBR_SHORTCUTS 17     // Number of menu entries allowded to use shortcuts.
// "TXT" for "text", "MTXT" for "menu text", "HTXT" for "help text"...
// "File" menu.
#define MTXT_MENU_FILE       _("&File")
#define TXT_LOAD_SETTINGS    _("Load settings")
#define MTXT_LOAD_SETTINGS   _("&Load settings\t")
#define HTXT_LOAD_SETTINGS   _("Load a new configuration")
#define TXT_RELOAD_SETTINGS  _("Reload settings")
#define MTXT_RELOAD_SETTINGS _("&Reload settings\t")
#define HTXT_RELOAD_SETTINGS _("Reload the current configuration")
#define TXT_LOAD_DEFAULT     _("Reload default settings")
#define MTXT_LOAD_DEFAULT    _("Reload &default settings\t")
#define HTXT_LOAD_DEFAULT    _("Reload the default configuration")
#define TXT_SET_DEFAULT      _("Set as default")
#define MTXT_SET_DEFAULT     _("Se&t as default\t")
#define HTXT_SET_DEFAULT     _("Set the current configuration as default")
#define TXT_RESET_DEFAULT    _("Reset default")
#define MTXT_RESET_DEFAULT   _("R&eset default\t")
#define HTXT_RESET_DEFAULT   _("Reset the default configuration")
#define TXT_SAVE_SETTINGS    _("Save settings")
#define MTXT_SAVE_SETTINGS   _("&Save settings\t")
#define HTXT_SAVE_SETTINGS   _("Save the current configuration")
#define TXT_SAVEAS_SETTINGS  _("Save settings as...")
#define MTXT_SAVEAS_SETTINGS _("S&ave settings as...\t")
#define HTXT_SAVEAS_SETTINGS _("Save the current configuration in another file")
#define TXT_OPTION           _("Options")
#define MTXT_OPTION          _("&Options\t")
#define HTXT_OPTION          _("Show the Settings tab")
#define TXT_LANGUAGES        _("Languages")
#define MTXT_LANGUAGES       _("Lan&guages\t")
#define HTXT_LANGUAGES       _("Select the GUI language")
#define TXT_QUIT             _("Exit")
#define MTXT_QUIT            _("E&xit\t")
#define HTXT_QUIT            _("Quit this program")
// "Work" menu.
#define MTXT_MENU_WORK       _("&Work")
#define TXT_START_THREAD     _("Start Thread")
#define MTXT_START_THREAD    _("&Start Thread\t")
#define HTXT_START_THREAD    _("Start the thread managing the dv stream")
#define TXT_STOP_THREAD      _("Stop Thread")
#define MTXT_STOP_THREAD     _("S&top Thread\t")
#define HTXT_STOP_THREAD     _("Stop the thread managing the dv stream")
#define TXT_START_CAPTURE    _("Start Capture")
#define MTXT_START_CAPTURE   _("St&art Capture\t")
#define HTXT_START_CAPTURE   _("Start capturing dv data (i.e. recording video!)")
#define TXT_STOP_CAPTURE     _("Stop Capture")
#define MTXT_STOP_CAPTURE    _("St&op Capture\t")
#define HTXT_STOP_CAPTURE    _("Stop capturing dv data (i.e. recording video!)")
#define TXT_ERROR_CLEARLOG   _("Clear Error log")
#define MTXT_ERROR_CLEARLOG  _("&Clear Error log\t")
#define HTXT_ERROR_CLEARLOG  _("Clear the error log window")
#define TXT_FULLSCREEN       _("Full Screen")
#define MTXT_FULLSCREEN      _("&Full Screen\t")
#define HTXT_FULLSCREEN      _("Go in/out full screen mode")
// "Help" menu.
#define MTXT_MENU_HELP       _("&Help")
#define TXT_HELP_SHOW        _("Help")
#define MTXT_HELP_SHOW       _("&Help\t")
#define HTXT_HELP_SHOW       _("Show the \"Help\" window")
#define TXT_ABOUT            _("About...")
#define MTXT_ABOUT           _("&About...\t")
#define HTXT_ABOUT           _("Show the \"About\" dialog")

// Tabs features.
#define TTXT_VIDEO           m_smallScreen ? _("V\nI\nD\nE\nO") : _("Video")
#define TTXT_SETTINGS        m_smallScreen ? _("S\nE\nT\nT\nI\nN\nG\nS") : _("Settings")
#define TTXT_LOGS            m_smallScreen ? _("L\nO\nG\nS") : _("Logs")
#define TTXT_HELP            m_smallScreen ? _("H\nE\nL\nP") : _("Help")


// XML texts (ids...)
// Nodes & properties names.
#define XML_NP_ROOT         _T("dvgrabgui") // Main (root) node.
#define XML_NP_ENTRY        _T("entry")     // Dvgrabgui option node.
#define XML_NP_VERSION      _T("version")   // Version of the file (root node).
#define XML_NP_NAME         _T("name")      // Name (ID) of the option.
#define XML_NP_TYPE         _T("type")      // Type of the option (menu shortcut, setting value, font, ...).
#define XML_NP_VALUE        _T("value")     // Value of the option.
#define XML_NP_FAMILY       _T("family")    // For "font" type: family font.
#define XML_NP_SIZE         _T("size")      // For "font" type: size font (in pts).

// Values of nodes' properties.
#define XML_VER_OLD_00      _T("")          // First version, no "version" property!
#define XML_VER_CUR         _T("1.1")       // Current version.
#define XML_TYPE_BOOL       _T("bool")      // bool value type.
#define XML_TYPE_INT        _T("int")       // int value type.
#define XML_TYPE_LONGLONG   _T("longlong")  // longlong value type.
#define XML_TYPE_STRING     _T("string")    // wxString value type.
#define XML_TYPE_MENU       _T("menu")      // Menu shortcut value type.
#define XML_TYPE_FONT       _T("font")      // wxFont value type.
#define XML_TYPE_STRING     _T("string")    // wxString value type.
// Values of "XML_NP_NAME".
// -- GUI.
#define XML_NAME_MENUFONT   _T("menuFont")
#define XML_NAME_STBARFONT  _T("statBarFont")
#define XML_NAME_SMALLFONT  _T("smallFont")
#define XML_NAME_CTRLFONT   _T("ctrlFont")
#define XML_NAME_LANG       _T("lang")
#define XML_NAME_SMALLSCR   _T("smallScreen")
// -- Options.
#define XML_NAME_SHOWEVERY  _T("showEvery")
#define XML_NAME_SHOW       _T("show")
#define XML_NAME_BEEP       _T("beep")
#define XML_NAME_PORT       _T("port")
#define XML_NAME_NODE       _T("node")
#define XML_NAME_RAWPIPE    _T("rawPipe")
#define XML_NAME_GUID       _T("guid")
#define XML_NAME_CHANNEL    _T("channel")
#define XML_NAME_BUFFERS    _T("buffers")
#define XML_NAME_NOAVC      _T("noAVC")
#define XML_NAME_NOSTOP     _T("noStop")
#define XML_NAME_PATH       _T("path")
#define XML_NAME_FILE       _T("file")
#define XML_NAME_FRAMEEVERY _T("frameEvery")
#define XML_NAME_FRAMECOUNT _T("frameCount")
#define XML_NAME_DURATION   _T("duration")
#define XML_NAME_MAXFSIZE   _T("maxFileSize")
#define XML_NAME_COLLSIZE   _T("collectionSize")
#define XML_NAME_COLLMCFS   _T("collectionMinCutFileSize")
#define XML_NAME_AUTOSPLIT  _T("autosplit")
#define XML_NAME_LOCKSTEP   _T("lockstep")
#define XML_NAME_LOCKSTEPMD _T("lockstepMaxDrops")
#define XML_NAME_LOCKSTEPTD _T("lockstepTotalDrops")
#define XML_NAME_TIMESTAMP  _T("timestamp")
#define XML_NAME_TIMECODE   _T("timecode")
#define XML_NAME_TIMESYS    _T("timesys")
#define XML_NAME_FILEFMT    _T("fileFormat")
#define XML_NAME_JPEGQ      _T("jpegQuality")
#define XML_NAME_JPEGD      _T("jpegDeinterlace")
#define XML_NAME_JPEGW      _T("jpegWidth")
#define XML_NAME_JPEGH      _T("jpegHeight")
#define XML_NAME_JPEGO      _T("jpegOverwrite")
//@}

/*!
 * @name wxWidgets' events table for class MainFrame.
 * @brief The event tables connect the wxWidgets events with the functions (event
 * handlers) which process them.
 */
//@{
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(EXEC_LOAD_SETTINGS, MainFrame::OnLoadSettings)
	EVT_MENU(EXEC_RELOAD_SETTINGS, MainFrame::OnReloadSettings)
	EVT_MENU(EXEC_LOAD_DEFAULT, MainFrame::OnLoadDefault)
	EVT_MENU(EXEC_SET_DEFAULT, MainFrame::OnSetDefault)
	EVT_MENU(EXEC_RESET_DEFAULT, MainFrame::OnResetDefault)
	EVT_MENU(EXEC_SAVE_SETTINGS, MainFrame::OnSaveSettings)
	EVT_MENU(EXEC_SAVEAS_SETTINGS, MainFrame::OnSaveAsSettings)
	EVT_MENU(EXEC_OPTION, MainFrame::OnOption)
	EVT_MENU_RANGE(EXEC_LANGS, EXEC_LANGS+OFFSET_LANGS, MainFrame::OnSetLanguage)
	EVT_MENU(EXEC_QUIT,  MainFrame::OnQuit)

	EVT_MENU(WORK_START_THREAD, MainFrame::OnStartThread)
	EVT_MENU(WORK_STOP_THREAD, MainFrame::OnStopThread)
	EVT_MENU(WORK_START_CAPTURE, MainFrame::OnStartCapt)
	EVT_MENU(WORK_STOP_CAPTURE, MainFrame::OnStopCapt)
	EVT_MENU(WORK_ERROR_CLEARLOG,  MainFrame::OnClearErr)
	EVT_MENU(WORK_FULLSCREEN, MainFrame::OnFullScreen)

	EVT_MENU(HELP_SHOW, MainFrame::OnHelp)
	EVT_MENU(HELP_ABOUT, MainFrame::OnAbout)

	EVT_MENU(THREAD_UPDATE, MainFrame::OnThreadUpdate)
	EVT_MENU(THREAD_TERM, MainFrame::OnThreadTerm)

	EVT_TIMER(TMR_REFRESH, MainFrame::OnUpdateTimer)
	EVT_TIMER(TMR_BEEPSOUND, MainFrame::OnBeepTimer)

	EVT_MENU(UPDATE_ALL_WINDOWS, MainFrame::OnUpdate)

	EVT_MOUSE_EVENTS(MainFrame::OnMouse)

	EVT_CLOSE(MainFrame::OnClose)
END_EVENT_TABLE()
//@}

/*!
 * This main frame can be created in two different modes: "normal" and "small screen", the later
 * optimised for 800x600 touch-screens!
 *
 * Do the folowing things:
 * 		-# Initializes all datas:
 * 			- Creates a Hash Map between the string name of the menus IDs and there numeric values;
 * 			- Creates the Status Bar;
 * 			- Loads all other settings from *.dvs.xml files.
 * 		-# Creates the menus and menu bar, using a Hash Map between the menus IDs and the (string) shortcuts.
 * 		-# Creates all its child windows, in a wxNotebook window.
 * 		-# Starts the UI timer and call UpdateUI (for having an up-to-date UI!).
 * 		.
 * @param title The title of this window!
 * @param pos The position of the window.
 * @param size The size of the window.
 * @param *m The mutex protecting ThreadInOut object.
 * @param *cth The ThreadInOut object.
 * @param *clog The log target chaining log messages to the first target (file).
 * @param *oldF The old frame: if NULL, load/use defaults settings from file;
 *              if not-NULL, don't load settings, and read in the old MainFrame
 *              some settings (shortcuts, lang, gui fonts, conf path...).
 * @param smallscr Wether this frame should be in "normal" or "small screen" mode.
 */
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size,
                     wxMutex *m, ThreadInOut *cth, wxLogChain *clog, MainFrame *oldF, bool smallscr)
                    : wxFrame(NULL, wxID_ANY, title, pos, size),
                      m_updateTimer(this), m_smallScreen(smallscr)
{
	wxLogMessage(_T("Creating a new DVGrabGUI 0.9.4 main frame! --- ID: %d"), GetId());

	m_constructing = true;
	m_mutex = m;
	m_cth = cth;
	m_menuFile = NULL;
	m_menuWork = NULL;
	m_menuHelp = NULL;
	m_subMenuLangs = NULL;

	wxIconBundle icon(LARGE_ICON, wxBITMAP_TYPE_XPM);
	icon.AddIcon(SMALL_ICON, wxBITMAP_TYPE_XPM);
	SetIcons(icon);

	m_menuFont = m_statBarFont = m_smallFont = m_ctrlFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	m_smallFont.SetPointSize(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize()-4);

	if(m_smallScreen)
		SetSizeHints(800, 600);				// The (min) size of the main window.
	else
		SetSizeHints(900, 675);				// The (min) size of the main window.

	// Create hash map between string name of the menus IDs and num values of these IDs.
	// Used by LoadSettings()!
	m_idHM[ETS(EXEC_LOAD_SETTINGS)]   = EXEC_LOAD_SETTINGS;
	m_idHM[ETS(EXEC_RELOAD_SETTINGS)] = EXEC_RELOAD_SETTINGS;
	m_idHM[ETS(EXEC_LOAD_DEFAULT)]    = EXEC_LOAD_DEFAULT;
	m_idHM[ETS(EXEC_SET_DEFAULT)]     = EXEC_SET_DEFAULT;
	m_idHM[ETS(EXEC_RESET_DEFAULT)]   = EXEC_RESET_DEFAULT;
	m_idHM[ETS(EXEC_SAVE_SETTINGS)]   = EXEC_SAVE_SETTINGS;
	m_idHM[ETS(EXEC_SAVEAS_SETTINGS)] = EXEC_SAVEAS_SETTINGS;
	m_idHM[ETS(EXEC_OPTION)]          = EXEC_OPTION;
	m_idHM[ETS(EXEC_QUIT)]            = EXEC_QUIT;
	m_idHM[ETS(WORK_START_THREAD)]    = WORK_START_THREAD;
	m_idHM[ETS(WORK_STOP_THREAD)]     = WORK_STOP_THREAD;
	m_idHM[ETS(WORK_START_CAPTURE)]   = WORK_START_CAPTURE;
	m_idHM[ETS(WORK_STOP_CAPTURE)]    = WORK_STOP_CAPTURE;
	m_idHM[ETS(WORK_ERROR_CLEARLOG)]  = WORK_ERROR_CLEARLOG;
	m_idHM[ETS(WORK_FULLSCREEN)]      = WORK_FULLSCREEN;
	m_idHM[ETS(HELP_SHOW)]            = HELP_SHOW;
	m_idHM[ETS(HELP_ABOUT)]           = HELP_ABOUT;

	// Load default settings.
	if(!oldF)
	{
		m_confPath = FILE_DEFAULT_DVS;
		if(!m_confPath.FileExists())
			ResetDefault();
		LoadSettings(m_confPath.GetFullPath());
		m_mutex->Lock();
		m_smallScreen = m_cth->m_smallScreen;
		m_mutex->Unlock();
	}
	else
	{
		m_scHM = oldF->m_scHM;
		m_confPath = oldF->m_confPath;
		m_lang = oldF->m_lang;
		m_isoLangs = oldF->m_isoLangs;
		m_readableLangs = oldF->m_readableLangs;

		m_menuFont = oldF->m_menuFont;
		m_statBarFont = oldF->m_statBarFont;
		m_smallFont = oldF->m_smallFont;
		m_ctrlFont = oldF->m_ctrlFont;

		m_mutex->Lock();
		m_cth->m_smallScreen = m_smallScreen;
		m_mutex->Unlock();
	}

#if wxUSE_STATUSBAR
	if(!m_smallScreen)
	{
		// Create a status bar with 4 panes (the first one reserved for help messages from menu items).
		CreateStatusBar(4);
		GetStatusBar()->SetFont(m_statBarFont);
		int k[4] = {-1, 200, 150, 100}; // The first pane have variable size,
		SetStatusWidths(4, k);          // the three others, fixed size.
	}
#endif // wxUSE_STATUSBAR

// Create the menu bar; create shortcuts using m_scHM hash map.
	m_menuText = new wxString[NBR_SHORTCUTS];
	m_menuSC   = new wxString[NBR_SHORTCUTS];
	m_menuID   = new int[NBR_SHORTCUTS];

	m_menuID[0] =EXEC_LOAD_SETTINGS;   m_menuSC[0] =m_scHM[EXEC_LOAD_SETTINGS];   m_menuText[0] =TXT_LOAD_SETTINGS;
	m_menuID[1] =EXEC_RELOAD_SETTINGS; m_menuSC[1] =m_scHM[EXEC_RELOAD_SETTINGS]; m_menuText[1] =TXT_RELOAD_SETTINGS;
	m_menuID[2] =EXEC_LOAD_DEFAULT;    m_menuSC[2] =m_scHM[EXEC_LOAD_DEFAULT];    m_menuText[2] =TXT_LOAD_DEFAULT;
	m_menuID[3] =EXEC_SET_DEFAULT;     m_menuSC[3] =m_scHM[EXEC_SET_DEFAULT];     m_menuText[3] =TXT_SET_DEFAULT;
	m_menuID[4] =EXEC_RESET_DEFAULT;   m_menuSC[4] =m_scHM[EXEC_RESET_DEFAULT];   m_menuText[4] =TXT_RESET_DEFAULT;
	m_menuID[5] =EXEC_SAVE_SETTINGS;   m_menuSC[5] =m_scHM[EXEC_SAVE_SETTINGS];   m_menuText[5] =TXT_SAVE_SETTINGS;
	m_menuID[6] =EXEC_SAVEAS_SETTINGS; m_menuSC[6] =m_scHM[EXEC_SAVEAS_SETTINGS]; m_menuText[6] =TXT_SAVEAS_SETTINGS;
	m_menuID[7] =EXEC_OPTION;          m_menuSC[7] =m_scHM[EXEC_OPTION];          m_menuText[7] =TXT_OPTION;
	m_menuID[8] =EXEC_QUIT;            m_menuSC[8] =m_scHM[EXEC_QUIT];            m_menuText[8] =TXT_QUIT;
	m_menuID[9] =WORK_START_THREAD;    m_menuSC[9] =m_scHM[WORK_START_THREAD];    m_menuText[9] =TXT_START_THREAD;
	m_menuID[10]=WORK_STOP_THREAD;     m_menuSC[10]=m_scHM[WORK_STOP_THREAD];     m_menuText[10]=TXT_STOP_THREAD;
	m_menuID[11]=WORK_START_CAPTURE;   m_menuSC[11]=m_scHM[WORK_START_CAPTURE];   m_menuText[11]=TXT_START_CAPTURE;
	m_menuID[12]=WORK_STOP_CAPTURE;    m_menuSC[12]=m_scHM[WORK_STOP_CAPTURE];    m_menuText[12]=TXT_STOP_CAPTURE;
	m_menuID[13]=WORK_ERROR_CLEARLOG;  m_menuSC[13]=m_scHM[WORK_ERROR_CLEARLOG];  m_menuText[13]=TXT_ERROR_CLEARLOG;
	m_menuID[14]=WORK_FULLSCREEN;      m_menuSC[14]=m_scHM[WORK_FULLSCREEN];      m_menuText[14]=TXT_FULLSCREEN;
	m_menuID[15]=HELP_SHOW;            m_menuSC[15]=m_scHM[HELP_SHOW];            m_menuText[15]=TXT_HELP_SHOW;
	m_menuID[16]=HELP_ABOUT;           m_menuSC[16]=m_scHM[HELP_ABOUT];           m_menuText[16]=TXT_ABOUT;

	// First, the File menu.
	wxArrayString txt;
	// Language stuff.
	DiscoverAvailableLanguages();
	m_subMenuLangs = new wxMenu();
	for(unsigned int i = 0; i < m_readableLangs.GetCount(); i++)
		m_subMenuLangs->AppendRadioItem(EXEC_LANGS+i, m_readableLangs[i]);
	SetLang();


	m_menuFile = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
	m_menuFile->Append(EXEC_LOAD_SETTINGS, wxString(MTXT_LOAD_SETTINGS) + m_scHM[EXEC_LOAD_SETTINGS],
	                   HTXT_LOAD_SETTINGS);
	m_menuFile->Append(EXEC_RELOAD_SETTINGS, wxString(MTXT_RELOAD_SETTINGS) + m_scHM[EXEC_RELOAD_SETTINGS],
	                   HTXT_RELOAD_SETTINGS);
	m_menuFile->Append(EXEC_LOAD_DEFAULT, wxString(MTXT_LOAD_DEFAULT) + m_scHM[EXEC_LOAD_DEFAULT],
	                   HTXT_LOAD_DEFAULT);
	m_menuFile->Append(EXEC_SET_DEFAULT, wxString(MTXT_SET_DEFAULT) + m_scHM[EXEC_SET_DEFAULT],
	                   HTXT_SET_DEFAULT);
	m_menuFile->Append(EXEC_RESET_DEFAULT, wxString(MTXT_RESET_DEFAULT) + m_scHM[EXEC_RESET_DEFAULT],
	                   HTXT_RESET_DEFAULT);
	m_menuFile->Append(EXEC_SAVE_SETTINGS, wxString(MTXT_SAVE_SETTINGS) + m_scHM[EXEC_SAVE_SETTINGS],
	                   HTXT_SAVE_SETTINGS);
	m_menuFile->Append(EXEC_SAVEAS_SETTINGS, wxString(MTXT_SAVEAS_SETTINGS) + m_scHM[EXEC_SAVEAS_SETTINGS],
	                   HTXT_SAVEAS_SETTINGS);
	m_menuFile->Append(EXEC_OPTION, wxString(MTXT_OPTION) + m_scHM[EXEC_OPTION],
	                   HTXT_OPTION);
	m_menuFile->AppendSeparator();
	m_menuFile->Append(EXEC_SUBLANGS, MTXT_LANGUAGES, m_subMenuLangs,
	                   HTXT_LANGUAGES);
	m_menuFile->AppendSeparator();
	m_menuFile->Append(EXEC_QUIT, wxString(MTXT_QUIT) + m_scHM[EXEC_QUIT],
	                   HTXT_QUIT);

	// Then, the Work menu.
	m_menuWork = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
	m_menuWork->Append(WORK_START_THREAD, wxString(MTXT_START_THREAD) + m_scHM[WORK_START_THREAD],
	                   HTXT_START_THREAD);
	m_menuWork->Append(WORK_STOP_THREAD, wxString(MTXT_STOP_THREAD) + m_scHM[WORK_STOP_THREAD],
	                   HTXT_STOP_THREAD);
	m_menuWork->AppendSeparator();
	m_menuWork->Append(WORK_START_CAPTURE, wxString(MTXT_START_CAPTURE) + m_scHM[WORK_START_CAPTURE],
	                   HTXT_START_CAPTURE);
	m_menuWork->Append(WORK_STOP_CAPTURE, wxString(MTXT_STOP_CAPTURE) + m_scHM[WORK_STOP_CAPTURE],
	                   HTXT_STOP_CAPTURE);
	m_menuWork->AppendSeparator();
	m_menuWork->Append(WORK_ERROR_CLEARLOG, wxString(MTXT_ERROR_CLEARLOG) + m_scHM[WORK_ERROR_CLEARLOG],
	                   HTXT_ERROR_CLEARLOG);
	m_menuWork->AppendSeparator();
	m_menuWork->AppendCheckItem(WORK_FULLSCREEN, wxString(MTXT_FULLSCREEN) + m_scHM[WORK_FULLSCREEN],
	                   HTXT_FULLSCREEN);

	// At the end, the Help menu.
	m_menuHelp = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
	m_menuHelp->Append(HELP_SHOW, wxString(MTXT_HELP_SHOW) + m_scHM[HELP_SHOW],
	                   HTXT_HELP_SHOW);
	m_menuHelp->Append(HELP_ABOUT, wxString(MTXT_ABOUT) + m_scHM[HELP_ABOUT],
	                   HTXT_ABOUT);

	// Now append the freshly created menus to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->SetFont(m_menuFont);
	menuBar->Append(m_menuFile, MTXT_MENU_FILE);
	menuBar->Append(m_menuWork, MTXT_MENU_WORK);
	menuBar->Append(m_menuHelp, MTXT_MENU_HELP);
	// ... and attach this menu bar to the frame.
	SetMenuBar(menuBar);

	// Constructing the child windows, in a wxNotebook window.
	if(m_smallScreen)
		m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_LEFT);
	else
		m_notebook = new wxNotebook(this, wxID_ANY);
	m_mainwind = new MainWindow(m_notebook, m_cth, m_mutex, &m_smallFont, &m_ctrlFont);
	m_settingswind = new SettingsWindow(m_notebook, m_cth, m_mutex, &m_menuFont,
	                                    &m_statBarFont, &m_smallFont, &m_ctrlFont,
	                                    m_menuSC, m_menuText, NBR_SHORTCUTS);
	m_textError = new wxTextCtrl(m_notebook, wxID_ANY, wxEmptyString,
	                             wxDefaultPosition, wxDefaultSize,
	                             wxTE_MULTILINE | wxTE_RICH);
	m_textError->SetFont(m_ctrlFont);
	m_textError->SetEditable(false);
	m_textError->LoadFile(FILE_LOG);
	// Using m_textError as log target for all wxWidgets logs messages (through a wxLogChain).
	wxLog *log = new wxLogTextCtrl(m_textError);
	clog->SetLog(log);

	m_notebook->AddPage(m_mainwind, TTXT_VIDEO, oldF ? false : true);
	m_notebook->AddPage(m_settingswind, TTXT_SETTINGS);
	m_notebook->AddPage(m_textError, TTXT_LOGS);
// If wxWidgets 2.8 or sup.
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 8) || wxMAJOR_VERSION > 2
	m_helpwind = new wxHtmlHelpWindow(m_notebook, wxID_ANY);
	m_helpctrl = new wxHtmlHelpController(wxHF_DEFAULT_STYLE | wxHF_OPEN_FILES | wxHF_EMBEDDED, m_notebook);
	m_helpctrl->SetHelpWindow(m_helpwind);
	m_notebook->AddPage(m_helpwind, TTXT_HELP);
#else  // wxWidgets 2.6 (not suporting lower anyway!)
	m_helpctrl = new wxHtmlHelpController(wxHF_DEFAULT_STYLE | wxHF_OPEN_FILES);
#endif // wxWidgets 2.8 or sup.
	if(!wxDirExists(DIR_HELP_TEMP))
		wxMkdir(DIR_HELP_TEMP);
	m_helpctrl->SetTempDir(DIR_HELP_TEMP);
	m_helpctrl->AddBook(wxFileName(wxString(DVGRABGUI_DOC_PATH) + FILE_HELP));

	m_notebook->SetFont(m_ctrlFont);
	SetClientSize(m_notebook->GetSize());
	if(oldF)
	{
		m_notebook->SetSelection(oldF->m_notebook->GetSelection());
		m_settingswind->SetSelection(oldF->m_settingswind->GetSelection());
	}

	m_sound = new wxSound();
	m_sound->Create(wxString(DVGRABGUI_SETTING_PATH) + wxString(_T("beep.wav")));
	if(!m_sound->IsOk())
		m_cth->SetMessage(ERROR, _("DVG-MFR: Couldn't load the \"beep\" wave file"));
	// Initialise sound timer for "beep" playing.
	m_soundTimer.SetOwner(this, TMR_BEEPSOUND);

	// Start timer for updating UI.
	m_updateTimer.SetOwner(this, TMR_REFRESH);
	m_updateTimer.Start(200);
	// Construction process ended!
	m_constructing = false;
	// Now we can update UI.
	UpdateUI();
}

//! Frame destructor, nothing special to do!
MainFrame::~MainFrame()
{
}

// ----------------------------------------------------------------------------
// event handlers: File, Work and Help menus
// ----------------------------------------------------------------------------

//! Event handler managing the @a "Load settings" menu item.
/*!
 * Used to load a setting file.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnLoadSettings(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog fdiag(this, _("Load settings ..."), m_confPath.GetPath(), _T(""),
	                   _("DVGrabGUI settings file (*.dvs.xml)|*.dvs.xml"),
	                   wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(fdiag.ShowModal() == wxID_OK)
	{
		m_confPath = fdiag.GetPath();
		LoadSettings(m_confPath.GetFullPath());
	}
}

//! Event handler managing the @a "Reload settings" menu item.
/*!
 * Used to reload the same setting file.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnReloadSettings(wxCommandEvent& WXUNUSED(event))
{
	LoadSettings(m_confPath.GetFullPath());
}

//! Event handler managing the @a "Load default" menu item.
/*!
 * Used to reload default settings.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnLoadDefault(wxCommandEvent& WXUNUSED(event))
{
	m_confPath = FILE_DEFAULT_DVS;
	if(!m_confPath.FileExists())
		ResetDefault();
	LoadSettings(m_confPath.GetFullPath());
}

//! Event handler managing the @a "Set as default" menu item.
/*!
 * Used to set current settings as default.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnSetDefault(wxCommandEvent& WXUNUSED(event))
{
	SaveSettings(FILE_DEFAULT_DVS);
}

//! Event handler managing the @a "Reset default" menu item.
/*!
 * Used to reset default settings.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnResetDefault(wxCommandEvent& WXUNUSED(event))
{
	ResetDefault();
}

//! Event handler managing the @a "Save settings" menu item.
/*!
 * Used to save current settings in current file.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnSaveSettings(wxCommandEvent& WXUNUSED(event))
{
	// If current config file is default one, send a SaveAs event (use SetDefault
	// to overwrite default config).
	if(m_confPath.GetFullName() == wxString(_T("default.dvs.xml")))
	{
		wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, EXEC_SAVEAS_SETTINGS);
		wxPostEvent(this, event);
		return;
	}
	// Same thing if config file allready exists and user doesn't want to overwrite it.
	if(m_confPath.FileExists())
	{
		if(wxMessageBox(wxString(_("Overwrite file: ")) + m_confPath.GetFullPath() + wxString(_T(" ?")),
		                _("Warning"), wxYES_NO | wxICON_EXCLAMATION) == wxNO)
		{
			wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, EXEC_SAVEAS_SETTINGS);
			wxPostEvent(this, event);
			return;
		}
	}
	SaveSettings(m_confPath.GetFullPath());
}

//! Event handler managing the @a "Save settings as ..." menu item.
/*!
 * Used to save current settings in another file.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnSaveAsSettings(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog fdiag(this, _("Save settings as ..."), m_confPath.GetPath(), _T(""),
	                   _("DVGrabGUI settings file (*.dvs.xml)|*.dvs.xml"),
	                   wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(fdiag.ShowModal() == wxID_OK)
	{
		m_confPath = fdiag.GetPath();
		SaveSettings(m_confPath.GetFullPath());
	}
}

//! Event handler managing the @a "Options" menu item.
/*!
 * Used to show the "settings" tab.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnOption(wxCommandEvent& WXUNUSED(event))
{
	m_notebook->SetSelection(1); // SettingsWindow tab!
}

//! Event handler managing all the @a "Languages" sub-menu items.
/*!
 * Used when changing UI language.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnSetLanguage(wxCommandEvent& WXUNUSED(event))
{
	// Discover the new language checked and update all UI.
	for(unsigned int i = 0; i < m_readableLangs.GetCount(); i++)
	{
		if(m_subMenuLangs->IsChecked(EXEC_LANGS+i))
		{
			m_lang = m_isoLangs[i];
			SetLang();
			UpdateAll();
			return;
		}
	}
}

//! Event handler managing the @a "Exit" menu item.
/*!
 * Used when leaving the app.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}

//! Event handler managing the @a "Start Thread" menu item.
/*!
 * Used to start the capture thread.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnStartThread(wxCommandEvent& WXUNUSED(event))
{
	wxGetApp().StartThread(true);
}

//! Event handler managing the @a "Stop Thread" menu item.
/*!
 * Used to halt the capture thread.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnStopThread(wxCommandEvent& WXUNUSED(event))
{
	wxGetApp().StartThread(false);
}

//! Event handler managing the @a "Start Capture" menu item.
/*!
 * Used to start the recording of dv stream.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnStartCapt(wxCommandEvent& WXUNUSED(event))
{
	wxGetApp().StartCapt(true);
}

//! Event handler managing the @a "Stop Capture" menu item.
/*!
 * Used to halt the recording of dv stream.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnStopCapt(wxCommandEvent& WXUNUSED(event))
{
	wxGetApp().StartCapt(false);
}

//! Event handler managing the @a "Clear Error log" menu item.
/*!
 * Used to clear the error log window (i.e. m_textError control).
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnClearErr(wxCommandEvent& WXUNUSED(event))
{
	m_textError->Clear();
}

//! Event handler managing the @a "Full Screen" menu item.
/*!
 * Used to go in/out the full screen mode.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnFullScreen(wxCommandEvent& WXUNUSED(event))
{
	if(IsFullScreen())
	{
		ShowFullScreen(false);
		m_menuWork->Check(WORK_FULLSCREEN, false);
	}
	else
	{
		ShowFullScreen(true, wxFULLSCREEN_NOBORDER | wxFULLSCREEN_NOCAPTION);
		m_menuWork->Check(WORK_FULLSCREEN, true);
	}
}

//! Event handler managing the @a "Help" menu item.
/*!
 * Used to display the "help" tab.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnHelp(wxCommandEvent& WXUNUSED(event))
{
// If wxWidgets 2.8 or sup.
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 8) || wxMAJOR_VERSION > 2
	m_notebook->SetSelection(3);
#else  // wxWidgets 2.6 (not suporting lower anyway!)
	m_helpctrl->DisplayContents();
#endif // wxWidgets 2.8 or sup.
}

//! Event handler managing the @a "About" menu item.
/*!
 * Used to show a short info message box.
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(_("DVGrabGUI (c) 2006-2008 Bastien Montagne.\n"
	               "Released under the GNU GPL license, "
	               "either v2 or any later version.\n"
	               "Based on DVGrab, by Dan Dennedy and Kino team.\n"
	               "Coded for ASCIA association."),
	             _("About DVGrabGUI"), wxOK | wxICON_INFORMATION, this);
}

// ----------------------------------------------------------------------------
// Handlers for messages from the capture thread (using generic wxComandEvent)
// ----------------------------------------------------------------------------

//! Event handler managing update events from capture thread.
/*!
 * Just call the MainWindow::ThreadUpdate() func, for now!
 * @param WXUNUSED(event) Unused wxCommandEvent event.
 */
void MainFrame::OnThreadUpdate(wxCommandEvent& WXUNUSED(event))
{
	m_mainwind->ThreadUpdate();
}

//! Event handler managing the term event sent by the capture thread when halted.
/*!
 * @param event Say if the thread stoped due to internal error or not.
 */
void MainFrame::OnThreadTerm(wxCommandEvent& event)
{
	// No need of mutex protection here!
	wxGetApp().m_thread = NULL;
	if(!event.GetInt())
		m_cth->SetMessage(WARNING, _("DVG-MFR: Capture Thread stopped, due to internal error(s)"));

	// Reset of "hardware" settings.
	m_cth->m_port = m_cth->m_backPort;
	m_cth->m_node = m_cth->m_backNode;
	m_cth->m_guid = m_cth->m_backGuid;
	m_cth->m_channel = m_cth->m_backChannel;
	m_cth->m_running = false;
	// If changes were made in config while thread running, apply them!
	if(m_cth->m_change_settings_pending && m_settingswind)
		m_settingswind->UpdateCTH();

	UpdateUI();
}

// ----------------------------------------------------------------------------
// various handlers
// ----------------------------------------------------------------------------

//! Event handler managing @a m_updateTimer events.
/*!
 * Used for updating and saving the log messages periodically.
 * @param WXUNUSED(event) Unused wxTimerEvent event.
 */
void MainFrame::OnUpdateTimer(wxTimerEvent& WXUNUSED(event))
{
	m_mutex->Lock();

	// Update the Error text window, with a color for each type.
	// Then empty the message list.
	unsigned int num = m_cth->m_threadMessage.m_message.GetCount();
	if(num && num == m_cth->m_threadMessage.m_type.GetCount())
	{
		for(unsigned int i = 0; i < num; i++)
		{
			switch(m_cth->m_threadMessage.m_type[i])
			{
				case ERROR:
					wxLogError(m_cth->m_threadMessage.m_message[i]);
					break;
				case WARNING:
					wxLogWarning(m_cth->m_threadMessage.m_message[i]);
					break;
				case INFO:
					wxLogMessage(m_cth->m_threadMessage.m_message[i]);
					break;
				case CAPT_STOP:
					UpdateUI();
					break;
			}
		}
		m_cth->m_threadMessage.Empty();
	}

	m_mutex->Unlock();
}

//! Event handler managing @a m_soundTimer events.
/*!
 * Used for playing "beep" sound periodically when recording.
 * @param WXUNUSED(event) Unused wxTimerEvent event.
 */
void MainFrame::OnBeepTimer(wxTimerEvent& WXUNUSED(event))
{
	m_mutex->Lock();
	bool b = m_cth->m_capture;
	m_mutex->Unlock();
	if(!b)
	{
		m_soundTimer.Stop();
		return;
	}
	m_sound->Play();
}

//! Event handler managing mouse events.
/*!
 * Used for capturing/releasing the mouse and, when captured, to start/halt capturing dv stream.
 * @param event wxMouseEvent event, used to read mouse buttons state.
 */
void MainFrame::OnMouse(wxMouseEvent& event)
{
	// If both buttons are pressed, flip capture state of the mouse
	if((event.LeftDown() && event.RightIsDown()) || (event.RightDown() && event.LeftIsDown()))
	{
		if(!HasCapture())
			MouseCapt(true);
		else
			MouseCapt(false);
	}
	// Else, if mouse's captured and left button's pressed, start capture,
	// right button's pressed, stop capture.
	else if(event.LeftDown() && HasCapture())
		wxGetApp().StartCapt(true);
	else if(event.RightDown() && HasCapture())
		wxGetApp().StartCapt(false);
}

//! Event handler managing close event.
/*!
 * @param WXUNUSED(event) Unused wxCloseEvent event.
 */
void MainFrame::OnClose(wxCloseEvent& WXUNUSED(event))
{
	// Needed, because m_cth->m_frame might be pointing on
	// a new frame created in TheApp::ChangeGuiMode(), and
	// we don't want to erase this new pointer!
	m_mutex->Lock();
	if(this == m_cth->m_frame)
		m_cth->m_frame = NULL;
	m_mutex->Unlock();

	wxLogMessage(_T("Exiting DVGrabGUI 0.9.4 mainframe! --- ID: %d"), GetId());

	// General cleanup.
	delete [] m_menuID;
	delete [] m_menuSC;
	delete [] m_menuText;

	delete m_helpctrl;

	Destroy();
}

//! Event handler managing Update events sent by other windows.
/*!
 * Used by child windows to force full update of the GUI.
 * @param WXUNUSED(event) Unused wxCloseEvent event.
 */
void MainFrame::OnUpdate(wxCommandEvent& event)
{
	UpdateAll();
}

// ----------------------------------------------------------------------------
// Various helpers
// ----------------------------------------------------------------------------

//! Helper function for capturing the mouse (when using it to command dv capture).
/*!
 * Control if we are allowed to capture and/or release the mouse before doing it.
 * @param on @a TRUE to capture, @a FALSE to release.
 */
void MainFrame::MouseCapt(bool on)
{
	m_mutex->Lock();
	bool thread = m_cth->m_running;
	m_mutex->Unlock();
	if(thread)
	{
		if(on && !HasCapture())
		{
			CaptureMouse();
		}
		else if(!on && HasCapture())
		{
			ReleaseMouse();
		}
		UpdateUI();
	}
}

//! Helper function udating all text in menus and UI.
/*!
 *  Only used when changing UI language or loading new config file
 *  (and some changes on settings, like GUI fonts!).
 */
void MainFrame::UpdateAll(bool from_cth)
{
// First of all, if the "screen mode" changed, call TheApp for a new main frame!
	m_mutex->Lock();
	if(m_smallScreen != m_cth->m_smallScreen)
	{
		m_mutex->Unlock();
		wxGetApp().ChangeGuiMode();
		return;
	}
	m_mutex->Unlock();

// Here I try to udate the languages sub-menu, but this seems to be tricky!
//	wxMenuItem *t = m_menuFile->Remove(EXEC_SUBLANGS);
//	delete t;
//	DiscoverAvailableLanguages();
//	unsigned int NbrLangs = m_readableLangs.GetCount();
//	delete m_subMenuLangs;
//	m_subMenuLangs = new wxMenu();
//	for(unsigned int i = 0; i < NbrLangs; i++)
//		m_subMenuLangs->AppendRadioItem(EXEC_LANGS+i, m_readableLangs[i]);
//	t = new wxMenuItem(m_menuFile, EXEC_SUBLANGS, _("&Languages"),
//	                               _("Select the GUI language"), wxITEM_NORMAL,
//	                               m_subMenuLangs);
//	m_menuFile->Insert(9, t);

// Udating all menu strings.
	//Updating from/to "settings" tab (wether from_cth is false or true).
	m_menuText[0]  = TXT_LOAD_SETTINGS;
	m_menuText[1]  = TXT_RELOAD_SETTINGS;
	m_menuText[2]  = TXT_LOAD_DEFAULT;
	m_menuText[3]  = TXT_SET_DEFAULT;
	m_menuText[4]  = TXT_RESET_DEFAULT;
	m_menuText[5]  = TXT_SAVE_SETTINGS;
	m_menuText[6]  = TXT_SAVEAS_SETTINGS;
	m_menuText[7]  = TXT_OPTION;
	m_menuText[8]  = TXT_QUIT;
	m_menuText[9]  = TXT_START_THREAD;
	m_menuText[10] = TXT_STOP_THREAD;
	m_menuText[11] = TXT_START_CAPTURE;
	m_menuText[12] = TXT_STOP_CAPTURE;
	m_menuText[13] = TXT_ERROR_CLEARLOG;
	m_menuText[14] = TXT_FULLSCREEN;
	m_menuText[15] = TXT_HELP_SHOW;
	m_menuText[16] = TXT_ABOUT;
	if(from_cth)
	{
		for(int i = 0; i < NBR_SHORTCUTS; i++)
			m_menuSC[i] = m_scHM[m_menuID[i]];
	}
	else
	{
		for(int i = 0; i < NBR_SHORTCUTS; i++)
			m_scHM[m_menuID[i]] = m_menuSC[i];
	}


	// Does not work ?
	GetMenuBar()->SetFont(m_menuFont);
	GetMenuBar()->SetLabelTop(0, MTXT_MENU_FILE);
	GetMenuBar()->SetLabelTop(1, MTXT_MENU_WORK);
	GetMenuBar()->SetLabelTop(2, MTXT_MENU_HELP);

	// First, the File menu.
	m_menuFile->SetLabel(EXEC_LOAD_SETTINGS, wxString(MTXT_LOAD_SETTINGS) + m_scHM[EXEC_LOAD_SETTINGS]);
	m_menuFile->SetHelpString(EXEC_LOAD_SETTINGS, HTXT_LOAD_SETTINGS);
	m_menuFile->SetLabel(EXEC_RELOAD_SETTINGS, wxString(MTXT_RELOAD_SETTINGS) + m_scHM[EXEC_RELOAD_SETTINGS]);
	m_menuFile->SetHelpString(EXEC_RELOAD_SETTINGS, HTXT_RELOAD_SETTINGS);
	m_menuFile->SetLabel(EXEC_LOAD_DEFAULT, wxString(MTXT_LOAD_DEFAULT) + m_scHM[EXEC_LOAD_DEFAULT]);
	m_menuFile->SetHelpString(EXEC_LOAD_DEFAULT, HTXT_LOAD_DEFAULT);
	m_menuFile->SetLabel(EXEC_SET_DEFAULT, wxString(MTXT_SET_DEFAULT) + m_scHM[EXEC_SET_DEFAULT]);
	m_menuFile->SetHelpString(EXEC_SET_DEFAULT, HTXT_SET_DEFAULT);
	m_menuFile->SetLabel(EXEC_RESET_DEFAULT, wxString(MTXT_RESET_DEFAULT) + m_scHM[EXEC_RESET_DEFAULT]);
	m_menuFile->SetHelpString(EXEC_RESET_DEFAULT, HTXT_RESET_DEFAULT);
	m_menuFile->SetLabel(EXEC_SAVE_SETTINGS, wxString(MTXT_SAVE_SETTINGS) + m_scHM[EXEC_SAVE_SETTINGS]);
	m_menuFile->SetHelpString(EXEC_SAVE_SETTINGS, HTXT_SAVE_SETTINGS);
	m_menuFile->SetLabel(EXEC_SAVEAS_SETTINGS, wxString(MTXT_SAVEAS_SETTINGS) + m_scHM[EXEC_SAVEAS_SETTINGS]);
	m_menuFile->SetHelpString(EXEC_SAVEAS_SETTINGS, HTXT_SAVEAS_SETTINGS);
	m_menuFile->SetLabel(EXEC_OPTION, wxString(MTXT_OPTION) + m_scHM[EXEC_OPTION]);
	m_menuFile->SetHelpString(EXEC_OPTION, HTXT_OPTION);
	m_menuFile->SetLabel(EXEC_SUBLANGS, MTXT_LANGUAGES);
	m_menuFile->SetHelpString(EXEC_SUBLANGS, HTXT_LANGUAGES);
	m_menuFile->SetLabel(EXEC_QUIT, wxString(MTXT_QUIT) + m_scHM[EXEC_QUIT]);
	m_menuFile->SetHelpString(EXEC_QUIT, HTXT_QUIT);

	// Then, the Work menu.
	m_menuWork->SetLabel(WORK_START_THREAD, wxString(MTXT_START_THREAD) + m_scHM[WORK_START_THREAD]);
	m_menuWork->SetHelpString(WORK_START_THREAD, HTXT_START_THREAD);
	m_menuWork->SetLabel(WORK_STOP_THREAD, wxString(MTXT_STOP_THREAD) + m_scHM[WORK_STOP_THREAD]);
	m_menuWork->SetHelpString(WORK_STOP_THREAD, HTXT_STOP_THREAD);
	m_menuWork->SetLabel(WORK_START_CAPTURE, wxString(MTXT_START_CAPTURE) + m_scHM[WORK_START_CAPTURE]);
	m_menuWork->SetHelpString(WORK_START_CAPTURE, HTXT_START_CAPTURE);
	m_menuWork->SetLabel(WORK_STOP_CAPTURE, wxString(MTXT_STOP_CAPTURE) + m_scHM[WORK_STOP_CAPTURE]);
	m_menuWork->SetHelpString(WORK_STOP_CAPTURE, HTXT_STOP_CAPTURE);
	m_menuWork->SetLabel(WORK_ERROR_CLEARLOG, wxString(MTXT_ERROR_CLEARLOG) + m_scHM[WORK_ERROR_CLEARLOG]);
	m_menuWork->SetHelpString(WORK_ERROR_CLEARLOG, HTXT_ERROR_CLEARLOG);
	m_menuWork->SetLabel(WORK_FULLSCREEN, wxString(MTXT_FULLSCREEN) + m_scHM[WORK_FULLSCREEN]);
	m_menuWork->SetHelpString(WORK_FULLSCREEN, HTXT_FULLSCREEN);

	// At the end, the Help menu.
	m_menuHelp->SetLabel(HELP_SHOW, wxString(MTXT_HELP_SHOW) + m_scHM[HELP_SHOW]);
	m_menuHelp->SetHelpString(HELP_SHOW, HTXT_HELP_SHOW);
	m_menuHelp->SetLabel(HELP_ABOUT, wxString(MTXT_ABOUT) + m_scHM[HELP_ABOUT]);
	m_menuHelp->SetHelpString(HELP_ABOUT, HTXT_ABOUT);

	if(!m_smallScreen)
		GetStatusBar()->SetFont(m_statBarFont);

// Updating tabs.
	m_notebook->SetPageText(0, TTXT_VIDEO);
	m_notebook->SetPageText(1, TTXT_SETTINGS);
	m_notebook->SetPageText(2, TTXT_LOGS);
// If wxWidgets 2.8 or sup.
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 8) || wxMAJOR_VERSION > 2
	m_notebook->SetPageText(3, TTXT_HELP);
#endif // wxWidgets 2.8 or sup.
	m_notebook->SetFont(m_ctrlFont);
	
// Refresh now all child windows.
	m_mainwind->UpdateAll();
	m_settingswind->UpdateAll(from_cth);
	m_textError->SetFont(m_ctrlFont);
	m_textError->SetEditable(false);

// Finally, complete GUI refresh.
	UpdateUI(true);
	SetClientSize(m_notebook->GetSize());
}

//! Helper function refreshing User Interface.
/*!
 * @param all @a TRUE to update ALL the controls, @a FALSE otherwise.
 */
void MainFrame::UpdateUI(bool all)
{
	// If main frame in construction, return.
	if(m_constructing)
		return;

	if(!m_smallScreen)
	{
		if(HasCapture())
			SetStatusText(_("Mouse Captured"), 2);
		else
			SetStatusText(_("Mouse Free"), 2);
	}

	m_mutex->Lock();
	if(m_cth->m_capture)
	{
		// Because of problems with my wxWidgets version, can't use the "loop" func!.
		// "Program received signal SIGSEGV, Segmentation fault.
		//  [Switching to Thread -1328546912 (LWP 2535)]
		//  0x0035a855 in wxSoundBackendSDL::FillAudioBuffer () from /usr/lib/libwx_gtk2u_adv-2.6.so.0"
		if(m_cth->m_beep && m_sound->IsOk())
		{
			m_soundTimer.Start(1500);
		}
		if(!m_smallScreen)
			SetStatusText(_("RECORDING"), 3);
		m_menuWork->Enable(WORK_START_CAPTURE, false);
		m_menuWork->Enable(WORK_STOP_CAPTURE, true);
	}
	else
	{
		m_soundTimer.Stop();
		if(!m_smallScreen)
			SetStatusText(_("PAUSED"), 3);
		m_menuWork->Enable(WORK_START_CAPTURE, true);
		m_menuWork->Enable(WORK_STOP_CAPTURE, false);
	}

	if(m_cth->m_running)
	{
		if(!m_smallScreen)
			SetStatusText(_("Capture thread running"), 1);
		m_menuWork->Enable(WORK_START_THREAD, false);
		m_menuWork->Enable(WORK_STOP_THREAD, true);
	}
	else
	{
		if(!m_smallScreen)
			SetStatusText(_("Capture thread stoped"), 1);
		m_menuWork->Enable(WORK_START_THREAD, true);
		m_menuWork->Enable(WORK_STOP_THREAD, false);
		m_menuWork->Enable(WORK_START_CAPTURE, false);
		m_menuWork->Enable(WORK_STOP_CAPTURE, false);
	}
	m_mutex->Unlock();

	// No more usefull.
	if(all)
	{
	}

	// Refreshing child windows' UI.
	m_mainwind->UpdateUI(all);
	m_settingswind->UpdateUI();
}

//! @brief Helper function updating the TheApp::m_locale wxLocale variable,
//!        according to m_lang variable.
void MainFrame::SetLang()
{
	// If TheApp::m_locale exists, delete it; then construct a new one.
	if(wxGetApp().m_locale)
		delete wxGetApp().m_locale;
	wxGetApp().m_locale = new wxLocale();
	// Get the wxWidgets language code from MainFrame::m_lang.
	int lang = wxLANGUAGE_UNKNOWN;
	if(m_lang == _T("def"))
		lang = wxLANGUAGE_DEFAULT;
	else
		lang = wxLocale::FindLanguageInfo(m_lang)->Language;
	// To be sure that the correct entry in language sub-menu is checked.
	wxString st = wxLocale::GetLanguageInfo(lang)->Description;
	for(unsigned int i = 0; i < m_readableLangs.GetCount(); i++)
		if(m_readableLangs[i] == st)
		{
			m_subMenuLangs->FindItem(EXEC_LANGS+i)->Check();
			break;
		}
	// Complete TheApp::m_locale (re)setting.
	if(!wxGetApp().m_locale->Init(lang))
	{
		wxLogMessage(wxString(_T("Problem setting new locale: '")) + st + wxString(_T("'; returning to default one!\n"
		                      "This might be because this locale isn't installed on your system...")));
		lang = wxLANGUAGE_DEFAULT;
		// To be sure that the correct entry in language sub-menu is checked.
		wxString st = wxLocale::GetLanguageInfo(lang)->Description;
		for(unsigned int i = 0; i < m_readableLangs.GetCount(); i++)
			if(m_readableLangs[i] == st)
			{
				m_subMenuLangs->FindItem(EXEC_LANGS+i)->Check();
				break;
			}
		wxGetApp().m_locale->Init(lang);
	}

	wxGetApp().m_locale->AddCatalogLookupPathPrefix(wxString(DVGRABGUI_SETTING_PATH) + wxString(_T("langs")));
	wxGetApp().m_locale->AddCatalog(_T("dvgrabgui"));

	// Now, the help file... (if we aren't in construction!).
	if(m_constructing)
		return;
	if(m_helpctrl)
		delete m_helpctrl;
// If wxWidgets 2.8 or sup.
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 8) || wxMAJOR_VERSION > 2
	m_helpctrl = new wxHtmlHelpController(wxHF_DEFAULT_STYLE | wxHF_OPEN_FILES | wxHF_EMBEDDED, m_notebook);
	m_helpctrl->SetHelpWindow(m_helpwind);
#else  // wxWidgets 2.6 (not suporting lower anyway!)
	m_helpctrl = new wxHtmlHelpController(wxHF_DEFAULT_STYLE | wxHF_OPEN_FILES);
#endif // wxWidgets 2.8 or sup.
	m_helpctrl->SetTempDir(DIR_HELP_TEMP);
	m_helpctrl->AddBook(wxFileName(wxString(DVGRABGUI_DOC_PATH) + FILE_HELP));
}

//! Helper function reading the "langs" directory, for discovering all installed languages.
void MainFrame::DiscoverAvailableLanguages()
{
	// Empty both the text arrays of iso codes and english names of available languages.
	m_isoLangs.Empty();
	m_readableLangs.Empty();
	// Then do the real work.
	wxDir dir(wxString(DVGRABGUI_SETTING_PATH) + wxString(_T("langs")));
	if(dir.IsOpened())
	{
		// First, scan the iso named directory, one per language.
		m_isoLangs.Add(_T(""));
		if(dir.GetFirst(&m_isoLangs.Last(), _T(""), wxDIR_DIRS))
		{
			m_isoLangs.Add(_T(""));
			while(dir.GetNext(&m_isoLangs.Last()))
			{
				m_isoLangs.Add(_T(""));
			}
			m_isoLangs.RemoveAt(m_isoLangs.GetCount()-1);
		}
		// Then, fill MainFrame::m_readableLangs from MainFrame::m_isoLangs values.
		for(unsigned int i = 0; i < m_isoLangs.GetCount() && i < OFFSET_LANGS; i++)
		{
			const wxLanguageInfo *inf = wxLocale::FindLanguageInfo(m_isoLangs[i]);
			if(inf)
				m_readableLangs.Add(inf->Description);
		}
	}
}

//! Helper function for loading settings from a xml file, using the wxWidgets XML parser.
/*!
 * @param path The complete path of the *.dvs.xml file to load.
 */
void MainFrame::LoadSettings(wxString path)
{
	m_mutex->Lock();

	// Load the XML configuration file, and check it is realy a dvgrabgui one.
	wxXmlDocument conf;
	if(!conf.Load(path))
		return;
	if(conf.GetRoot()->GetName() != XML_NP_ROOT)
		return;
	// If not the good version, update it!
	if(conf.GetRoot()->GetPropVal(XML_NP_VERSION, XML_VER_OLD_00) != XML_VER_CUR)
		UpgradeSettingsFile(&conf, path, true);

	wxXmlNode *entry = conf.GetRoot()->GetChildren();
	while(entry)
	{
		wxString name, type, value, family, size;
		long l = 0;

		// These two properties are optional...
		entry->GetPropVal(XML_NP_FAMILY, &family);
		entry->GetPropVal(XML_NP_SIZE, &size);
		// Sort of giant "switch ... case" ...
		// If one of the four next tests is false, just try the next sibling node...
		if(entry->GetName() != XML_NP_ENTRY)
			;
		else if(!entry->GetPropVal(XML_NP_NAME, &name))
			;
		else if(!entry->GetPropVal(XML_NP_TYPE, &type))
			;
		else if(!entry->GetPropVal(XML_NP_VALUE, &value))
			;
		// For menu shortcuts, use m_idHM to translate string ID in numeric ID,
		// and m_scHM to translate numeric ID in string shortcut!
		else if(type == XML_TYPE_MENU && m_idHM.find(name) != m_idHM.end())
		{
			// Verifying if the given shortcut is valid.
			wxString sc1, sc2, sc3, sc4; // Max. four parts in a shortcut ("ctrl+alt+shift+A").
			wxString regex(_T("F(1[0-2]|0?[1-9])|[a-zA-Z]{1}|enter")); // Regex for fourth part.
			wxRegEx re(regex, wxRE_EXTENDED | wxRE_ICASE); // Ignore case.
			wxStringTokenizer stk(value, _T("\t\r\n +-"), wxTOKEN_STRTOK);
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
				m_scHM[m_idHM[name]] = _T("");
			else
			{
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
				m_scHM[m_idHM[name]] = sc1 + sc2 + sc3 + sc4;
			}
		}
		// GUI fonts.
		else if(type == XML_TYPE_FONT) {
			wxFontFamily ff = wxFONTFAMILY_DEFAULT;
			if(family == _T("wxFONTFAMILY_DECORATIVE"))
				ff = wxFONTFAMILY_DECORATIVE;
			else if(family == _T("wxFONTFAMILY_ROMAN"))
				ff = wxFONTFAMILY_ROMAN;
			else if(family == _T("wxFONTFAMILY_SCRIPT"))
				ff = wxFONTFAMILY_SCRIPT;
			else if(family == _T("wxFONTFAMILY_SWISS"))
				ff = wxFONTFAMILY_SWISS;
			else if(family == _T("wxFONTFAMILY_MODERN"))
				ff = wxFONTFAMILY_MODERN;
			else if(family == _T("wxFONTFAMILY_TELETYPE"))
				ff = wxFONTFAMILY_TELETYPE;
			l = 10;
			size.ToLong(&l);

			wxFont tfont((int) l, ff, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, value);
			if(tfont.Ok())
			{
				if(name == XML_NAME_MENUFONT)
					m_menuFont = tfont;
				else if(name == XML_NAME_STBARFONT)
					m_statBarFont = tfont;
				else if(name == XML_NAME_SMALLFONT)
					m_smallFont = tfont;
				else if(name == XML_NAME_CTRLFONT)
					m_ctrlFont = tfont;
			}
		}
		// GUI language.
		else if(name == XML_NAME_LANG && type == XML_TYPE_STRING)
			m_lang = value;
		// "Small screen" GUI mode (800x600).
		else if(name == XML_NAME_SMALLSCR && type == XML_TYPE_BOOL)
			m_cth->m_smallScreen = ToBool(value, false);
		// Video display options.
		else if(name == XML_NAME_SHOWEVERY && type == XML_TYPE_INT)
			m_cth->m_showEvery = (int) ToLong(value, 25);
		else if(name == XML_NAME_SHOW && type == XML_TYPE_BOOL)
			m_cth->m_show = ToBool(value, true);
		else if(name == XML_NAME_BEEP && type == XML_TYPE_BOOL)
			m_cth->m_beep = ToBool(value, true);
		// Capture options.
		else if(name == XML_NAME_PORT && type == XML_TYPE_INT)
			m_cth->m_port = (int) ToLong(value, -1);
		else if(name == XML_NAME_NODE && type == XML_TYPE_INT)
			m_cth->m_node = (int) ToLong(value, -1);
		else if(name == XML_NAME_RAWPIPE && type == XML_TYPE_BOOL)
			m_cth->m_rawPipe = ToBool(value, false);
		else if(name == XML_NAME_GUID && type == XML_TYPE_LONGLONG)
			m_cth->m_guid = ToLLong(value, 0);
		else if(name == XML_NAME_CHANNEL && type == XML_TYPE_INT)
			m_cth->m_channel = (int) ToLong(value, -1);
		else if(name == XML_NAME_BUFFERS && type == XML_TYPE_INT)
			m_cth->m_buffers = (int) ToLong(value, 250);
		else if(name == XML_NAME_NOAVC && type == XML_TYPE_BOOL)
			m_cth->m_noAVC = ToBool(value, false);
		else if(name == XML_NAME_NOSTOP && type == XML_TYPE_BOOL)
			m_cth->m_noStop = ToBool(value, false);
		else if(name == XML_NAME_PATH && type == XML_TYPE_STRING)
			m_cth->m_path = value;
		else if(name == XML_NAME_FILE && type == XML_TYPE_STRING)
			m_cth->m_file = value;
		else if(name == XML_NAME_FRAMEEVERY && type == XML_TYPE_INT)
			m_cth->m_frameEvery = (int) ToLong(value, 1);
		else if(name == XML_NAME_FRAMECOUNT && type == XML_TYPE_INT)
			m_cth->m_frameCount = (int) ToLong(value, 0);
		// Very hairy to cast wxString in std::string!!!!!!
		// So not using m_duration for now.
		else if(name == XML_NAME_DURATION)
			m_cth->m_duration = ""; //std::string(wxStdString(value));
		else if(name == XML_NAME_MAXFSIZE && type == XML_TYPE_INT)
			m_cth->m_maxFileSize = (int) ToLong(value, 1024);
		else if(name == XML_NAME_COLLSIZE && type == XML_TYPE_INT)
			m_cth->m_collectionSize = (int) ToLong(value, 0);
		else if(name == XML_NAME_COLLMCFS && type == XML_TYPE_INT)
			m_cth->m_collectionMinCutFileSize = (int) ToLong(value, 1);
		else if(name == XML_NAME_AUTOSPLIT && type == XML_TYPE_BOOL)
			m_cth->m_autosplit = ToBool(value, false);
		else if(name == XML_NAME_LOCKSTEP && type == XML_TYPE_BOOL)
			m_cth->m_lockstep = ToBool(value, false);
		else if(name == XML_NAME_LOCKSTEPMD && type == XML_TYPE_INT)
			m_cth->m_lockstepMaxDrops = (int) ToLong(value, -1);
		else if(name == XML_NAME_LOCKSTEPTD && type == XML_TYPE_INT)
			m_cth->m_lockstepTotalDrops = (int) ToLong(value, -1);
		// Video file name options.
		else if(name == XML_NAME_TIMESTAMP && type == XML_TYPE_BOOL)
			m_cth->m_timestamp = ToBool(value, false);
		else if(name == XML_NAME_TIMECODE && type == XML_TYPE_BOOL)
			m_cth->m_timecode = ToBool(value, false);
		else if(name == XML_NAME_TIMESYS && type == XML_TYPE_BOOL)
			m_cth->m_timesys = ToBool(value, false);
		// Video file type options.
		else if(name == XML_NAME_FILEFMT && type == XML_TYPE_STRING) {
			if(value == _T("AVI_DV1"))
				m_cth->m_fileFormat = AVI_DV1_FORMAT;
			else if(value == _T("AVI_DV2"))
				m_cth->m_fileFormat = AVI_DV2_FORMAT;
			else if(value == _T("QT"))
				m_cth->m_fileFormat = QT_FORMAT;
			else if(value == _T("RAW"))
				m_cth->m_fileFormat = RAW_FORMAT;
			else if(value == _T("DIF"))
				m_cth->m_fileFormat = DIF_FORMAT;
			else if(value == _T("JPEG"))
				m_cth->m_fileFormat = JPEG_FORMAT;
			else
				m_cth->m_fileFormat = UNDEFINED;
		}
		else if(name == XML_NAME_JPEGQ && type == XML_TYPE_INT)
			m_cth->m_jpegQuality = (int) ToLong(value, 90);
		else if(name == XML_NAME_JPEGD && type == XML_TYPE_BOOL)
			m_cth->m_jpegDeinterlace = ToBool(value, false);
		else if(name == XML_NAME_JPEGW && type == XML_TYPE_INT)
			m_cth->m_jpegWidth = (int) ToLong(value, -1);
		else if(name == XML_NAME_JPEGH && type == XML_TYPE_INT)
			m_cth->m_jpegHeight = (int) ToLong(value, -1);
		else if(name == XML_NAME_JPEGO && type == XML_TYPE_BOOL)
			m_cth->m_jpegOverwrite = ToBool(value, false);

		entry = entry->GetNext();
	}

	m_mutex->Unlock();

	SetLang();
	// If not in construction of the window, update all, using m_cth values!
	if(!m_constructing)
		UpdateAll(true);
}

//! Helper function for saving settings in a xml file, using the wxWidgets XML parser.
/*!
 * @param path The complete path of the *.dvs.xml file to write with current settings.
 */
void MainFrame::SaveSettings(wxString path)
{
	m_mutex->Lock();

	// New & empty Xml tree.
	wxXmlDocument conf;
	wxXmlNode *root, *entry;

	root = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ROOT);
	root->AddProperty(XML_NP_VERSION, XML_VER_CUR);

	// GUI language.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_LANG);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, m_lang);
	root->AddChild(entry);

	// "Small screen" GUI mode (800x600).
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_SMALLSCR);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_smallScreen ? _T("true") : _T("false"));
	root->AddChild(entry);

	// Video display options.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_SHOWEVERY);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_showEvery));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_SHOW);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_show ? _T("true") : _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_BEEP);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_beep ? _T("true") : _T("false"));
	root->AddChild(entry);

	// Capture options.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_PORT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_port));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_NODE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_node));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_RAWPIPE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_rawPipe ? _T("true") : _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_GUID);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_LONGLONG);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%llx"), m_cth->m_guid));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_CHANNEL);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_channel));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_BUFFERS);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_buffers));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_NOAVC);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_noAVC ? _T("true") : _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_NOSTOP);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_noStop ? _T("true") : _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_PATH);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_path);
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_FILE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_file);
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_FRAMEEVERY);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_frameEvery));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_FRAMECOUNT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_frameCount));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_DURATION);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, wxString(m_cth->m_duration.c_str(), m_cth->m_duration.size()));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_MAXFSIZE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_maxFileSize));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_COLLSIZE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_collectionSize));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_COLLMCFS);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_collectionMinCutFileSize));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_AUTOSPLIT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_autosplit ? _T("true") : _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_LOCKSTEP);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_lockstep ? _T("true") : _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_LOCKSTEPMD);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_lockstepMaxDrops));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_LOCKSTEPTD);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_lockstepTotalDrops));
	root->AddChild(entry);

	// Video file name options.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_TIMESTAMP);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_timestamp ? _T("true") : _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_TIMECODE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_timecode ? _T("true") : _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_TIMESYS);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_timesys ? _T("true") : _T("false"));
	root->AddChild(entry);

	// Video file type options.
	wxString ts;
	switch(m_cth->m_fileFormat)
	{
		case AVI_DV1_FORMAT:
			ts = _T("AVI_DV1");
			break;
		case AVI_DV2_FORMAT:
			ts = _T("AVI_DV2");
			break;
		case QT_FORMAT:
			ts = _T("QT");
			break;
		case RAW_FORMAT:
			ts = _T("RAW");
			break;
		case DIF_FORMAT:
			ts = _T("DIF");
			break;
		case JPEG_FORMAT:
			ts = _T("JPEG");
			break;
		default:
			ts = _T("UNDEFINED");
	}
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_FILEFMT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, ts);
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGQ);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_jpegQuality));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGD);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_jpegDeinterlace ? _T("true") : _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGW);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_jpegWidth));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGH);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, wxString::Format(_T("%d"), m_cth->m_jpegHeight));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGO);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, m_cth->m_jpegOverwrite ? _T("true") : _T("false"));
	root->AddChild(entry);

	// For menus, iterate all the ids stored in m_idHM, accessing shortcuts with m_scHM hash map.
	IDHashMap::iterator it;
	for(it = m_idHM.begin(); it != m_idHM.end(); it++)
	{
		entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
		entry->AddProperty(XML_NP_NAME, it->first);		// The id's name.
		entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
		entry->AddProperty(XML_NP_VALUE, m_scHM[it->second]);	// The id's shortcut.
		root->AddChild(entry);
	}

	// GUI fonts.
	wxFont *tfont[4];
	wxString tfamily[4];
	wxString tfontname[4];
	int tsize[4];
	tfont[0] = &m_menuFont;
	tfont[1] = &m_statBarFont;
	tfont[2] = &m_smallFont;
	tfont[3] = &m_ctrlFont;
	for(int i = 0; i < 4; i++)
	{
		switch(tfont[i]->GetFamily())
		{
			case wxFONTFAMILY_DECORATIVE:
				tfamily[i] = _T("wxFONTFAMILY_DECORATIVE");
				break;
			case wxFONTFAMILY_ROMAN:
				tfamily[i] = _T("wxFONTFAMILY_ROMAN");
				break;
			case wxFONTFAMILY_SCRIPT:
				tfamily[i] = _T("wxFONTFAMILY_SCRIPT");
				break;
			case wxFONTFAMILY_SWISS:
				tfamily[i] = _T("wxFONTFAMILY_SWISS");
				break;
			case wxFONTFAMILY_MODERN:
				tfamily[i] = _T("wxFONTFAMILY_MODERN");
				break;
			case wxFONTFAMILY_TELETYPE:
				tfamily[i] = _T("wxFONTFAMILY_TELETYPE");
				break;
			default:
				tfamily[i] = _T("wxFONTFAMILY_DEFAULT");
		}
		tfontname[i] = tfont[i]->GetFaceName();
		tsize[i] = tfont[i]->GetPointSize();
	}
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_MENUFONT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_FONT);
	entry->AddProperty(XML_NP_VALUE, tfontname[0]);
	entry->AddProperty(XML_NP_FAMILY, tfamily[0]);
	entry->AddProperty(XML_NP_SIZE, wxString::Format(_T("%d"), tsize[0]));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_STBARFONT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_FONT);
	entry->AddProperty(XML_NP_VALUE, tfontname[1]);
	entry->AddProperty(XML_NP_FAMILY, tfamily[1]);
	entry->AddProperty(XML_NP_SIZE, wxString::Format(_T("%d"), tsize[1]));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_SMALLFONT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_FONT);
	entry->AddProperty(XML_NP_VALUE, tfontname[2]);
	entry->AddProperty(XML_NP_FAMILY, tfamily[2]);
	entry->AddProperty(XML_NP_SIZE, wxString::Format(_T("%d"), tsize[2]));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_CTRLFONT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_FONT);
	entry->AddProperty(XML_NP_VALUE, tfontname[3]);
	entry->AddProperty(XML_NP_FAMILY, tfamily[3]);
	entry->AddProperty(XML_NP_SIZE, wxString::Format(_T("%d"), tsize[3]));
	root->AddChild(entry);

	conf.SetRoot(root);
	conf.Save(path);

	m_mutex->Unlock();
}

//! Helper function for resetting default.dvs.xml file, using the wxWidgets XML parser.
void MainFrame::ResetDefault()
{
	wxString path = wxGetenv(_T("HOME")) + wxString(_T("/.dvgrabgui"));
	if(!::wxDirExists(path))
	{
		::wxMkdir(path);
	}

	// New & empty Xml tree.
	wxXmlDocument conf;
	wxXmlNode *root, *entry;

	root = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ROOT);
	root->AddProperty(XML_NP_VERSION, XML_VER_CUR);

	// GUI language.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_LANG);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, _T("def"));
	root->AddChild(entry);

	// "Small screen" GUI mode (800x600).
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_SMALLSCR);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	// Video display options.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_SHOWEVERY);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("25"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_SHOW);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("true"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_BEEP);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("true"));
	root->AddChild(entry);

	// Capture options.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_PORT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("-1"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_NODE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("-1"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_RAWPIPE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_GUID);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_LONGLONG);
	entry->AddProperty(XML_NP_VALUE, _T("0"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_CHANNEL);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("-1"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_BUFFERS);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("250"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_NOAVC);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_NOSTOP);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_PATH);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, wxString(wxGetenv(_T("HOME"))) + wxString(_T("/")));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_FILE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, _T("cap-"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_FRAMEEVERY);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("1"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_FRAMECOUNT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("0"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_DURATION);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_MAXFSIZE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("1024"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_COLLSIZE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("0"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_COLLMCFS);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("1"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_AUTOSPLIT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_LOCKSTEP);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_LOCKSTEPMD);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("-1"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_LOCKSTEPTD);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("-1"));
	root->AddChild(entry);

	// Video file name options.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_TIMESTAMP);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_TIMECODE);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_TIMESYS);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	// Video file type options.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_FILEFMT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_STRING);
	entry->AddProperty(XML_NP_VALUE, _T("AVI_DV2"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGQ);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("90"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGD);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGW);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("-1"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGH);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_INT);
	entry->AddProperty(XML_NP_VALUE, _T("-1"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_JPEGO);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
	entry->AddProperty(XML_NP_VALUE, _T("false"));
	root->AddChild(entry);

	// Menu shortcuts.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(EXEC_OPTION));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(EXEC_LOAD_SETTINGS));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("Ctrl+O"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(EXEC_RELOAD_SETTINGS));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(EXEC_LOAD_DEFAULT));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(EXEC_SET_DEFAULT));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(EXEC_RESET_DEFAULT));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(EXEC_SAVE_SETTINGS));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("Ctrl+S"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(EXEC_SAVEAS_SETTINGS));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("Shift+Ctrl+S"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(EXEC_QUIT));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("Alt+X"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(WORK_ERROR_CLEARLOG));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("Ctrl+C"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(WORK_START_THREAD));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("Alt+S"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(WORK_STOP_THREAD));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("Alt+H"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(WORK_START_CAPTURE));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("Shift+Alt+S"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(WORK_STOP_CAPTURE));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("Shift+Alt+H"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(WORK_FULLSCREEN));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("F11"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(HELP_SHOW));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T("F1"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, ETS(HELP_ABOUT));
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	root->AddChild(entry);

	// GUI fonts.
	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_MENUFONT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_FONT);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	entry->AddProperty(XML_NP_FAMILY, _T("wxFONTFAMILY_DEFAULT"));
	entry->AddProperty(XML_NP_SIZE, _T("10"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_STBARFONT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_FONT);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	entry->AddProperty(XML_NP_FAMILY, _T("wxFONTFAMILY_DEFAULT"));
	entry->AddProperty(XML_NP_SIZE, _T("10"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_SMALLFONT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_FONT);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	entry->AddProperty(XML_NP_FAMILY, _T("wxFONTFAMILY_DEFAULT"));
	entry->AddProperty(XML_NP_SIZE, _T("7"));
	root->AddChild(entry);

	entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
	entry->AddProperty(XML_NP_NAME, XML_NAME_CTRLFONT);
	entry->AddProperty(XML_NP_TYPE, XML_TYPE_FONT);
	entry->AddProperty(XML_NP_VALUE, _T(""));
	entry->AddProperty(XML_NP_FAMILY, _T("wxFONTFAMILY_DEFAULT"));
	entry->AddProperty(XML_NP_SIZE, _T("8"));
	root->AddChild(entry);

	conf.SetRoot(root);
	conf.Save(path + wxString(_T("/default.dvs.xml")));
}

//! Helper function for updating a conf file to the current version.
/*!
 * This function is here to update the old configuration files, when I've made changes in
 * the structure of this conf file.
 *
 * @param conf The wxXmlDocument to update.
 * @param path The complete path of the *.dvs.xml file to update.
 * @param askBackup If to ask the user for a backup copy of his conf file, before updating it.
 */
void MainFrame::UpgradeSettingsFile(wxXmlDocument *conf, wxString path, bool askBackup)
{
	wxString backpath = path + _T(".bak");
	if(askBackup)
		if(::wxMessageBox(_("Old version of configuration file!\n"
		                    "DVGrabGUI is about to update it...\n"
		                    "Do you want to make a backup copy of the old version?"),
		                  _("Warning: old version of config file!"),
		                  wxYES_NO | wxICON_QUESTION) == wxYES)
			conf->Save(backpath);

	wxXmlNode *oldroot, *root, *oldentry, *entry;
	oldroot = conf->GetRoot();
	root = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ROOT);
	root->AddProperty(XML_NP_VERSION, XML_VER_CUR);

	// If the original (first version) config file...
	if(oldroot->GetName() == _T("dvgrabgui")
	   && oldroot->GetPropVal(_T("version"), XML_VER_OLD_00) == XML_VER_OLD_00)
	{
		oldentry = oldroot->GetChildren();
		while(oldentry)
		{
			if(oldentry->GetName() != XML_NP_ENTRY)
				;
			else if(oldentry->GetPropVal(_T("name"), _T("")) == _T("menu"))
			{
				entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
				entry->AddProperty(XML_NP_NAME, oldentry->GetPropVal(_T("id"), _T("")));
				entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
				entry->AddProperty(XML_NP_VALUE, oldentry->GetPropVal(_T("value"), _T("")));
			}
			else if(oldentry->GetPropVal(_T("type"), _T("")) == _T("font"))
			{
				entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
				entry->AddProperty(XML_NP_NAME, oldentry->GetPropVal(_T("name"), _T("")));
				entry->AddProperty(XML_NP_TYPE, XML_TYPE_FONT);
				entry->AddProperty(XML_NP_VALUE, oldentry->GetPropVal(_T("font"), _T("")));
				entry->AddProperty(XML_NP_FAMILY, oldentry->GetPropVal(_T("family"), _T("")));
				entry->AddProperty(XML_NP_SIZE, oldentry->GetPropVal(_T("size"), _T("")));
			}
			else
				entry = new wxXmlNode(*oldentry);
			root->AddChild(entry);
			oldentry = oldentry->GetNext();
		}
		// "Small screen" GUI mode (800x600).
		entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
		entry->AddProperty(XML_NP_NAME, XML_NAME_SMALLSCR);
		entry->AddProperty(XML_NP_TYPE, XML_TYPE_BOOL);
		entry->AddProperty(XML_NP_VALUE, _T("false"));
		root->AddChild(entry);
		// New menu entries (blank shortcuts to avoid doubles).
		// "Reload current settings".
		entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
		entry->AddProperty(XML_NP_NAME, ETS(EXEC_RELOAD_SETTINGS));
		entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
		entry->AddProperty(XML_NP_VALUE, _T(""));
		root->AddChild(entry);
		// "Full screen"
		entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
		entry->AddProperty(XML_NP_NAME, ETS(WORK_FULLSCREEN));
		entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
		entry->AddProperty(XML_NP_VALUE, _T(""));
		root->AddChild(entry);
		// "Help".
		entry = new wxXmlNode(wxXML_ELEMENT_NODE, XML_NP_ENTRY);
		entry->AddProperty(XML_NP_NAME, ETS(HELP_SHOW));
		entry->AddProperty(XML_NP_TYPE, XML_TYPE_MENU);
		entry->AddProperty(XML_NP_VALUE, _T(""));
		root->AddChild(entry);

	}

	conf->SetRoot(root);
	conf->Save(path);
}

//! Helper function to cast a wxString to a long.
/*!
 * @param value The wxString to cast.
 * @param def The default value to return, if the cast failed.
 * @return Return the cast value, or the provided "def" one if the cast failed.
 */
long MainFrame::ToLong(const wxString& value, long def)
{
	value.ToLong(&def);
	return def;
}

//! Helper function to cast a wxString to a longlong (i.e. uint64_t).
/*!
 * @param value The wxString to cast.
 * @param def The default value to return, if the cast failed.
 * @return Return the cast value, or the provided "def" one if the cast failed.
 */
uint64_t MainFrame::ToLLong(const wxString& value, uint64_t def)
{
	// Only way I found to cast wxString in char* for Unicode build!
	char *s = new char[value.Length()];
	for(unsigned int k = 0; k < value.Length(); k++)
		s[k] = value[k];
	sscanf(s, "%llx", &def);
	delete [] s;
	return def;
}

//! Helper function to cast a wxString to a bool.
/*!
 * @param value The wxString to cast.
 * @param def The default value to return, if the cast failed.
 * @return Return the cast value, or the provided "def" one if the cast failed.
 */
bool MainFrame::ToBool(const wxString& value, bool def)
{
	if(value == _T("true"))
		return true;
	else if(value == _T("false"))
		return false;
	else return def;
}
