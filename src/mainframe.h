/*!
 * @file mainframe.h
 * @brief DVGrabGUI main frame class declaration.
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

#ifndef MAINFRAME_H_
#define MAINFRAME_H_

#include "includes.h"        // General includes.
#include "mainwindow.h"      // Main window (first tab).
#include "settingswindow.h"  // Settings window (second tab).
#include "threadinout.h"     // Communication classes between threads.

#include "wx/xml/xml.h"      // XML features.

//! forward declaration.
class MainWindow;

/*!
 * @class MainFrame
 * @brief Our main frame class, derived from wxFrame.
 */
class MainFrame : public wxFrame
{
public:
	//! @name Constructor/Destructor.
	//@{
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size,
	          wxMutex* m, ThreadInOut* cth, wxLogChain *clog, MainFrame *oldF = NULL, bool smallscr = false);
	~MainFrame();
	//@}

	//! @name Event handlers, Menu "File".
	//@{
	void OnLoadSettings(wxCommandEvent& WXUNUSED(event));
	void OnReloadSettings(wxCommandEvent& WXUNUSED(event));
	void OnLoadDefault(wxCommandEvent& WXUNUSED(event));
	void OnSetDefault(wxCommandEvent& WXUNUSED(event));
	void OnResetDefault(wxCommandEvent& WXUNUSED(event));
	void OnSaveSettings(wxCommandEvent& WXUNUSED(event));
	void OnSaveAsSettings(wxCommandEvent& WXUNUSED(event));
	void OnOption(wxCommandEvent& WXUNUSED(event));
	void OnSetLanguage(wxCommandEvent& WXUNUSED(event));
	void OnQuit(wxCommandEvent& WXUNUSED(event));
	//@}

	//! @name Event handlers, Menu "Work".
	//@{
	void OnStartThread(wxCommandEvent& WXUNUSED(event));
	void OnStopThread(wxCommandEvent& WXUNUSED(event));
	void OnStartCapt(wxCommandEvent& WXUNUSED(event));
	void OnStopCapt(wxCommandEvent& WXUNUSED(event));
	void OnClearErr(wxCommandEvent& WXUNUSED(event));
	void OnFullScreen(wxCommandEvent& WXUNUSED(event));
	//@}

	//! @name Event handler, Menu "Help".
	//@{
	void OnHelp(wxCommandEvent& WXUNUSED(event));
	void OnAbout(wxCommandEvent& WXUNUSED(event));
	//@}

	//! @name Event handlers, managing thread messages.
	//@{
	void OnThreadUpdate(wxCommandEvent& WXUNUSED(event));
	void OnThreadTerm(wxCommandEvent& event);
	//@}

	//! @name Miscellaneous event handlers
	//@{
	void OnUpdateTimer(wxTimerEvent& WXUNUSED(event));
	void OnBeepTimer(wxTimerEvent& WXUNUSED(event));
	void OnMouse(wxMouseEvent& event);
	void OnClose(wxCloseEvent& WXUNUSED(event));
	void OnUpdate(wxCommandEvent& event);
	//@}

	//! @name Miscellaneous functions
	//@{
	void MouseCapt(bool on);
	void UpdateAll(bool from_cth = false);
	void UpdateUI(bool all = false);
	void SetLang();
	void DiscoverAvailableLanguages();
	void LoadSettings(wxString path);
	void SaveSettings(wxString path);
	void ResetDefault();
	void UpgradeSettingsFile(wxXmlDocument *conf, wxString path, bool askBackup);
	long ToLong(const wxString& value, long def);
	uint64_t ToLLong(const wxString& value, uint64_t def);
	bool ToBool(const wxString& value, bool def);
	//@}

private:
	wxNotebook *m_notebook;         //!< The notebook control containing everything!
	MainWindow *m_mainwind;         //!< The main window (video capture).
	SettingsWindow *m_settingswind; //!< The setting window (subdivided with a "listbook").
	wxTextCtrl *m_textError;        //!< The text control showing error and info messages.
// If wxWidgets 2.8 or sup.
#if (wxMAJOR_VERSION == 2 && wxMINOR_VERSION >= 8) || wxMAJOR_VERSION > 2
	wxHtmlHelpWindow *m_helpwind;   //!< The help window. 
#endif // wxWidgets 2.8 or sup.

	wxMenu *m_menuFile,             //!< The menu "File".
	       *m_menuWork,             //!< The menu "Work".
	       *m_menuHelp,             //!< The menu "Help".
	       *m_subMenuLangs;         //!< The sub-menu for selecting the GUI language.

	wxMutex *m_mutex;               //!< The mutex for accessing shared data between main and capture thread.

	// Miscellaneous variables
//	int m_showedFrames;             //!< The number of frames actually displayed in @a m_vidWin window.
	wxTimer m_updateTimer;          //!< The timer used for periodically refreshing UI.

	ThreadInOut *m_cth;             //!< The object storing all data shared between main and capture threads.

	//! Creating a Hash map object type with @a key: wxString and @a value: int
	WX_DECLARE_STRING_HASH_MAP(int, IDHashMap);
	IDHashMap m_idHM;               //!< Translating from @a (wxString)ID_name to @a (int)ID.

// Public to access these datas from another(new) MainFrame.
public:
	//! Creating a Hash map object type with @a key: int and @a value: wxString
	WX_DECLARE_HASH_MAP(int, wxString, wxIntegerHash, wxIntegerEqual, SCHashMap);
	SCHashMap m_scHM;               //!< Translating from @a (int)ID to @a (wxString)shortcut.

	wxFileName m_confPath;          //!< The path & name of the current settings file.
	wxString m_lang;                //!< Curent language in use.
	wxArrayString m_isoLangs,       //!< All available languages, in iso code.
	         m_readableLangs;       //!< All available languages, in readable english name.

	wxFont m_menuFont,              //!< The menu font (currently not working).
	       m_statBarFont,           //!< The status bar font.
	       m_smallFont,             //!< The small font (used for the top text info).
	       m_ctrlFont;              //!< The font for all controls in the MainFrame window.

	wxHtmlHelpController *m_helpctrl;//!< The help controller.

	wxSound *m_sound;               //!< The "beep" sound when recording.
	wxTimer  m_soundTimer;          //!< The timer used for periodically play "beep" while recording.

private:
	wxString *m_menuSC,             //!< The wxString array storing the menu shortcuts for each menu entries available.
	         *m_menuText;           //!< The wxString array storing the menu text for each menu entries available.
	int      *m_menuID;             //!< The int array storing the menu IDs for each menu entries available.

	/*! @brief True during construction of this object.
	 *  Used to prevent use of UpdateUI() function (called by LoadSettings()),
	 *  witch access to objects not yet created.
	 */
	bool m_constructing;
	bool m_smallScreen;             //!< Wether we are in "small screen" mode or not.

	DECLARE_EVENT_TABLE()           //!< Classical wxWidgets Macro for declaring that this class handle events.
};

#endif /*MAINFRAME_H_*/

