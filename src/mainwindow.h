/*!
 * @file mainwindow.h
 * @brief DVGrabGUI main window class declaration.
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

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "includes.h"    //!< General includes.
#include "theapp.h"      //!< The app object.
#include "vidwin.h"      //!< Window class showing dv frames.
#include "threadinout.h" //!< Communication classes between threads.

/*!
 * @class MainFrame
 * @brief This window display dv frames and a few controls/infos (only free space on hdd if in "small screen" mode!).
 */
class MainWindow : public wxPanel
{
public:
	//! @name Constructor/Destructor.
	//@{
	MainWindow(wxWindow* par, ThreadInOut *cth, wxMutex *mutex,
	           wxFont *smallF, wxFont *ctrlF);
	//! Default destructor, nothing to do.
	~MainWindow() {};
	//@}

	//! @name Events handlers for starting/halting capture thread and dv capture.
	//@{
	void OnButtThread(wxCommandEvent& WXUNUSED(event));
	void OnButtCapt(wxCommandEvent& WXUNUSED(event));
	//@}

	//! Event handlers for the name of the video file.
	//@{
	void OnTextFile (wxCommandEvent& WXUNUSED(event));
	void OnTextFile2 (wxFocusEvent& WXUNUSED(event));
	//@}

	//! @name Misc events handlers.
	//@{
	void OnMouse(wxMouseEvent& event);
	void OnSize(wxSizeEvent& event);
	//@}

	//! @name Various helpers.
	//@{
	void UpdateAll();
	void UpdateUI(bool all = false);
	void ThreadUpdate();
	//@}

public:
	VidWin       *m_vidWin;      //!< Window witch should show the dv frames(!!!).

	//! @name Controls for starting/halting capture thread and dv capture.
	//@{
	wxButton     *m_buttThread,  //!< The button starting/halting the capture thread.
	             *m_buttCapt;    //!< The button starting/halting the dv capture.
	//@}

	wxTextCtrl   *m_ctrlSelFile; //!< Text control for typing the name of video file.
	wxStaticText *m_textInfo;    //!< The static text giving infos on current state of capture thread.
	wxGauge      *m_diskGauge;   //!< The wxGauge control showing how much space's left on capture disk.
	wxStaticText *m_diskInfo;    //!< The static text giving total/free space on capture disk.

	ThreadInOut  *m_cth;         //!< The object storing all data shared between main and capture threads.
	wxMutex      *m_mutex;       //!< The mutex for accessing shared data between main and capture thread.

	// Miscellaneous variables
	int          m_showedFrames; //!< The number of frames actually displayed in @a m_vidWin window.
	wxTimer      m_updateTimer;  //!< The timer used for periodically refreshing UI.

	wxFont       *m_smallFont,   //!< The small font (used for the top text info).
	             *m_ctrlFont;    //!< The font for all controls in the MainFrame window.

	/*!
	 * @class ControlCL
	 * @brief A chained list 'storing' all wxStaticTexts and wxStaticBox we do
	 *        not need to keep as individual pointers.
	 *
	 * This is used for accessing one after the other, and update text label of
	 * each control, when changing UI language or loading new config file.
	 */
	class ControlCL
	{
	public:
		//! Default constructor.
		ControlCL(){c = NULL; n= NULL;}
		//! @brief Constructor setting the wxControl pointer.
		//! @param ctrl The wxControl pointer to be stored.
		ControlCL(wxControl *ctrl){c = ctrl; n = NULL;}
		//! @brief Add a new element to the chained list.
		//! @param ctrl The wxControl pointer to be stored in the new element.
		//! @return The pointer on the new ControlCL object.
		ControlCL* Add(wxControl *ctrl){n = new ControlCL(ctrl); return n;}
		//! @brief Set a new label to the wxControl pointed by this element.
		//! @param &s The new wxString name/value of this control.
		//! @param &f The new font for this control.
		//! @return The next element, or NULL if none.
		//!
		//! The order "SetLabel, SetFont" seems important for wxStaticText controls!
		ControlCL* Update(const wxString &s, const wxFont &f){c->SetLabel(s); c->SetFont(f); return n;}
		//! @brief Default destructor.
		//!
		//! Note that there is no need of deleting the wxControl object here:
		//! wxWidegets will do it!
		//! Note too that the ControlCL list is deleted recursively.
		~ControlCL(){if(n) delete n;}
		wxControl *c;        //!< The wxControl pointer.
		ControlCL *n;        //!< The pointer to the next ControlCL object.
	} *m_firstCtrl;         //!< The first element of the chained list.

DECLARE_EVENT_TABLE()
};

#endif /*MAINWINDOW_H_*/
