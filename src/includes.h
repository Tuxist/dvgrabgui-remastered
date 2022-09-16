/*!
 * @file includes.h
 * @brief DVGrabGUI #includes (mainly wxWidgets stuff) and constants used by the other classes.
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

#ifndef INCLUDES_H_
#define INCLUDES_H_

//! ???
#if defined(__GNUG__) && !defined(__APPLE__)
	#pragma implementation
	#pragma interface
#endif

// General wxWidgets local settings.
#include "wx/setup.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers
#ifndef WX_PRECOMP
	#include "wx/wx.h"
	#include "wx/app.h"
	#include "wx/log.h"
	#include "wx/frame.h"
	#include "wx/panel.h"

	#include "wx/timer.h"

	#include "wx/utils.h"
	#include "wx/menu.h"

	#include "wx/msgdlg.h"
	#include "wx/textdlg.h"
	#include "wx/filedlg.h"
	#include "wx/choicdlg.h"

	#include "wx/button.h"
	#include "wx/textctrl.h"
	#include "wx/listbox.h"

	#include "wx/sizer.h"
#endif

// "tab" controls
#include "wx/notebook.h"
#include "wx/listbook.h"

// For the help window.
#include "wx/html/helpctrl.h"
#include "wx/filesys.h"
#include "wx/fs_zip.h"

// for handling images
#include "wx/image.h"

// for the 'beep' sound when capturing
#include "wx/sound.h"

#include "wx/spinctrl.h"
#include "wx/combobox.h"

#include "wx/dir.h"
#include "wx/textfile.h"
#include "wx/regex.h"
#include "wx/tokenzr.h"
#include "wx/filename.h"
#include "wx/ffile.h"

// for the shortcuts editing control.
#include "wx/grid.h"

// for the sys fonts selection.
#include "wx/fontdlg.h"

// for gridbagsizer...
#include "wx/gbsizer.h"

// Max number of languages (!)
#define OFFSET_LANGS 200

//! IDs and other constants for the controls the menus, ...
enum
{
	//! @name Menu items.
	//@{
	EXEC_LOAD_SETTINGS = 100,//!< When the user wants to load a setting file.
	EXEC_RELOAD_SETTINGS,    //!< When the user wants to reload the same setting file.
	EXEC_LOAD_DEFAULT,       //!< When the user wants to load default settings.
	EXEC_SET_DEFAULT,        //!< When the user wants to set current settings as default.
	EXEC_RESET_DEFAULT,      //!< When the user wants to reset default settings.
	EXEC_SAVE_SETTINGS,      //!< When the user wants to save settings.
	EXEC_SAVEAS_SETTINGS,    //!< When the user wants to save settings in a new file.
	EXEC_OPTION,             //!< When the user wants to show the "settings" tab.
	EXEC_SUBLANGS,           //!< When the user displays the lang sub-menu.
	EXEC_QUIT,               //!< When the user wants to close the main window.
	WORK_ERROR_CLEARLOG,     //!< When the user wants to clear the error text window.
	WORK_START_THREAD,       //!< When the user wants to start the capture thread.
	WORK_STOP_THREAD,        //!< When the user wants to stop the capture thread.
	WORK_START_CAPTURE,      //!< When the user wants to start the dv capture.
	WORK_STOP_CAPTURE,       //!< When the user wants to stop the dv capture.
	WORK_CAPT_MOUSE,         //!< When the user wants to capture the mouse.
	WORK_FREE_MOUSE,         //!< When the user wants to free the mouse.
	WORK_FULLSCREEN,         //!< When the user wants to go in/out full screen mode.
	HELP_SHOW,               //!< When the user wants to show the "help" tab.
	HELP_ABOUT,              //!< When the user wants to know more about this app!
	//@}

	//! @name Communication thread.
	//@{
	THREAD_UPDATE = 300,     //!< Sent by the capture thread after each frame received.
	THREAD_TERM,             //!< Sent by the capture thread when ended.
	ERROR,                   //!< Means that this is an error message.
	WARNING,                 //!< Means that this is a warning message.
	INFO,                    //!< Means that this is an info message.
	CAPT_STOP,               //!< Means that the thread stoped the dv capture.
	//@}

	//! @name Inter-window communication.
	//@{
	UPDATE_ALL_WINDOWS = 400,//!< ID used by child windows to refresh ALL the GUI.
	CHANGE_SCREEN_MODE,      //!< ID used by main frame when changing GUI mode.
	//@}

	//! @name Control IDs.
	//@{
	// MainWindow.
	BTT_START_THREAD = 1000, //!< ID of MainFrame::m_buttThread.
	BTT_START_CAPTURE,       //!< ID of MainFrame::m_buttCapt.

	ETXT_SEL_FILE,           //!< ID of MainFrame::m_ctrlSelFile (and SettingsWindow::m_ctrlSelFile!).
	
	TMR_REFRESH,             //!< ID of MainFrame::m_updateTimer.
	TMR_BEEPSOUND,           //!< ID of MainFrame::m_soundTimer.
	
	// SettingsWindow.
	// "capture" panel.
	SPC_FREQ_IMG,            //!< ID of SettingsWindow::m_ctrlFreqImg.
	CKB_SHOW_IMG,            //!< ID of SettingsWindow::m_ctrlShowImg.
	CKB_PLAY_BEEP,           //!< ID of SettingsWindow::m_ctrlPlayBeep.

	ETXT_SEL_PATH,           //!< ID of SettingsWindow::m_ctrlSelPath.
	BTT_SEL_PATH,            //!< ID of SettingsWindow::m_ctrlDiagSelPath.
	CHC_FILE_TYPE,           //!< ID of SettingsWindow::m_ctrlFileType.
	SPC_MAX_FSIZE,           //!< ID of SettingsWindow::m_ctrlMaxFSize.
	SPC_COLL_SIZE,           //!< ID of SettingsWindow::m_ctrlCollSize.
	SPC_COLL_MCFSIZE,        //!< ID of SettingsWindow::m_ctrlCollMCFSize.

	CKB_TIMESTAMP,           //!< ID of SettingsWindow::m_ctrlTimestamp.
	CKB_TIMECODE,            //!< ID of SettingsWindow::m_ctrlTimecode.
	CKB_TIMESYS,             //!< ID of SettingsWindow::m_ctrlTimesys.

	SPC_FRAME_EVERY,         //!< ID of SettingsWindow::m_ctrlFrameEvery.
	CKB_AUTOSPLIT,           //!< ID of SettingsWindow::m_ctrlAutosplit.
	SPC_FRAME_COUNT,         //!< ID of SettingsWindow::m_ctrlFrameCount.
	CKB_LOCKSTEP,            //!< ID of SettingsWindow::m_ctrlLockstep.
	SPC_LOCKSTEP_MD,         //!< ID of SettingsWindow::m_ctrlLockstepMD.
	SPC_LOCKSTEP_TD,         //!< ID of SettingsWindow::m_ctrlLockstepTD.

	// "hardware" panel.
	CKB_RAW_PIPE,            //!< ID of SettingsWindow::m_ctrlRawPipe.
	SPC_PORT,                //!< ID of SettingsWindow::m_ctrlPort.
	SPC_NODE,                //!< ID of SettingsWindow::m_ctrlNode.
	SPC_CHANNEL,             //!< ID of SettingsWindow::m_ctrlChannel.
	CHC_GUID,                //!< ID of SettingsWindow::m_ctrlGuid.

	CKB_NOAVC,               //!< ID of SettingsWindow::m_ctrlNoAVC.
	CKB_NOSTOP,              //!< ID of SettingsWindow::m_ctrlNoStop.
	SPC_BUFFER,              //!< ID of SettingsWindow::m_ctrlBuffers.
	CKB_REC_VAUX,            //!< ID of SettingsWindow::m_ctrlRecVAUX.

	CKB_JPG_DEINTERLACE,     //!< ID of SettingsWindow::m_ctrlJpegD.
	CKB_JPG_OVERWRITE,       //!< ID of SettingsWindow::m_ctrlJpegO.
	SPC_JPG_QUALITY,         //!< ID of SettingsWindow::m_ctrlJpegQ.
	SPC_JPG_WIDTH,           //!< ID of SettingsWindow::m_ctrlJpegW.
	SPC_JPG_HEIGHT,          //!< ID of SettingsWindow::m_ctrlJpegH.

	// "GUI" panel.
	BTT_MENUFONT,            //!< ID of SettingsWindow::m_menuFont.
	BTT_STATBARFONT,         //!< ID of SettingsWindow::m_statBarFont.
	BTT_SMALLFONT,           //!< ID of SettingsWindow::m_smallFont.
	BTT_CTRLFONT,            //!< ID of SettingsWindow::m_ctrlFont.

	CKB_GUIMODE,             //!< ID of SettingsWindow::m_ctrlGUImode.
	//@}

	//! @name Dvgrab's IDs.
	//@{
	//! Type of dv stream.
	PAL_FORMAT = 2000,
	NTSC_FORMAT,
	AVI_DV1_FORMAT,            //!< Type of video files, not all are used for now.
	AVI_DV2_FORMAT,            //!< Type of video files, not all are used for now.
	QT_FORMAT,                 //!< Type of video files, not all are used for now.
	RAW_FORMAT,                //!< Type of video files, not all are used for now.
	DIF_FORMAT,                //!< Type of video files, not all are used for now.
	JPEG_FORMAT,               //!< Type of video files, not all are used for now.
	UNDEFINED,                 //!< Type of video files, not all are used for now.
	//@}

	//! All the IDs of the languages available.
	//@{
	EXEC_LANGS = 3000,
	END_EXEC_LANGS = 3000 + OFFSET_LANGS
	//@}
};

#endif /*INCLUDES_H_*/
