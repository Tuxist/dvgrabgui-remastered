/*!
 * @file settingswindow.h
 * @brief DVGrabGUI settings window (a tab in the main window).
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

#ifndef SETTINGSWINDOW_H_
#define SETTINGSWINDOW_H_

#include "includes.h"    //!< General includes.
#include "threadinout.h" //!< Communication classes between threads.

/*!
 * @class SettingsWindow
 * @brief Our settings window class, derived from wxListbook.
 */
class SettingsWindow : public wxListbook
{
public:
	//! @name Constructor/destructor
	//@{
	//! Window constructor.
	SettingsWindow(wxWindow *p, ThreadInOut *cth, wxMutex *mutex, wxFont *menuF,
	               wxFont *statBarF, wxFont *smallF, wxFont *ctrlF,
	               wxString *mSC, wxString *mText, int numSC);
	//! Destructor, nothing to do.
	~SettingsWindow() {};
	//@}

	// ------------------------------------------------
	// First "tab" (capture one).
	// ------------------------------------------------
	//! @name Events handlers for controling the display of the dv frames.
	//@{
	void OnSpinFreqImg     (wxSpinEvent&    WXUNUSED(event));
	void OnCheckShowImg    (wxCommandEvent& WXUNUSED(event));
	void OnCheckPlayBeep   (wxCommandEvent& WXUNUSED(event));
	//@}

	//! @name Events handlers for controling the files recorded.
	//@{
	void TextPath          ();
	void OnTextPath        (wxCommandEvent& WXUNUSED(event));
	void OnTextPath2       (wxFocusEvent&   WXUNUSED(event));
	void OnBttPath         (wxCommandEvent& WXUNUSED(event));
	void TextFile          ();
	void OnTextFile        (wxCommandEvent& WXUNUSED(event));
	void OnTextFile2       (wxFocusEvent&   WXUNUSED(event));
	void OnChoiceFileType  (wxCommandEvent& WXUNUSED(event));
	void OnSpinCollSize    (wxSpinEvent&    WXUNUSED(event));
	void OnSpinCollMCFSize (wxSpinEvent&    WXUNUSED(event));
	//@}

	//! @name Events handlers for controling the time text added to file name.
	//@{
	void OnTimestamp       (wxCommandEvent& WXUNUSED(event));
	void OnTimecode        (wxCommandEvent& WXUNUSED(event));
	void OnTimesys         (wxCommandEvent& WXUNUSED(event));
	//@}

	//! @name Events handlers for controling how/how much to record.
	//@{
	void OnSpinFrameEvery  (wxSpinEvent&    WXUNUSED(event));
	void OnCheckAutosplit  (wxCommandEvent& WXUNUSED(event));
	void OnSpinFrameCount  (wxSpinEvent&    WXUNUSED(event));
	void OnSpinMaxFSize    (wxSpinEvent&    WXUNUSED(event));
	void OnLockstep        (wxCommandEvent& WXUNUSED(event));
	void OnLockstepMD      (wxSpinEvent&    WXUNUSED(event));
	void OnLockstepTD      (wxSpinEvent&    WXUNUSED(event));
	//@}

	// ------------------------------------------------
	// Second "tab" (hardware one).
	// ------------------------------------------------
	//! @name Events handler for controling "hardware" features.
	//@{
	void OnRawPipe         (wxCommandEvent& WXUNUSED(event));
	void OnPort            (wxSpinEvent&    WXUNUSED(event));
	void OnNode            (wxSpinEvent&    WXUNUSED(event));
	void OnChannel         (wxSpinEvent&    WXUNUSED(event));
	void OnGuid            (wxCommandEvent& WXUNUSED(event));
	//@}

	//! @name Events handler for controling misc things.
	//@{
	void OnNoAVC           (wxCommandEvent& WXUNUSED(event));
	void OnNoStop          (wxCommandEvent& WXUNUSED(event));
	void OnBuffers         (wxSpinEvent&    WXUNUSED(event));
	void OnRecVAUX         (wxCommandEvent& WXUNUSED(event));
	//@}

	//! @name Events handler for controling Jpeg settings (not used for now!).
	//@{
	void OnJpegQuality     (wxSpinEvent&    WXUNUSED(event));
	void OnJpegWidth       (wxSpinEvent&    WXUNUSED(event));
	void OnJpegHeight      (wxSpinEvent&    WXUNUSED(event));
	void OnJpegDeinterlace (wxCommandEvent& WXUNUSED(event));
	void OnJpegOverwrite   (wxCommandEvent& WXUNUSED(event));
	//@}

	// ------------------------------------------------
	// Third "tab" (gui one).
	// ------------------------------------------------
	//! @name Event handler for controling shortcuts.
	//@{
	void OnChangeGrid      (wxGridEvent&    event);
	//@}

	//! @name Event handlers for controling GUI fonts.
	//@{
	void OnMenuFont        (wxCommandEvent& WXUNUSED(event));
	void OnStatBarFont     (wxCommandEvent& WXUNUSED(event));
	void OnSmallFont       (wxCommandEvent& WXUNUSED(event));
	void OnCtrlFont        (wxCommandEvent& WXUNUSED(event));
	//@}

	//! @name Event handler for controling GUI mode.
	//@{
	void OnGuiMode         (wxCommandEvent& WXUNUSED(event));
	//@}

	//! @name Various helpers.
	//@{
	void UpdateUI();
	void UpdateAll(bool from_cth = false);
	void UpdateCTH();
	void DiscoverIeee1394Devices(wxArrayString* dev);
	void CalcNbrFramesFromSize(int &i, int &j);
	//@}

private:
	wxPanel      *m_capture,        //!< First panel (settings allways available).
	             *m_hardware,       //!< Second panel (settings only available when capture thread's not running).
	             *m_shortcuts;      //!< Third panel (shortcuts and GUI fonts).

	// ------------------------------------------------
	// First "tab" controls (capture).
	// ------------------------------------------------
	//! @name Controls for controling the display of the dv frames and the "beep" sound playing.
	//@{
	wxSpinCtrl   *m_ctrlFreqImg;    //!< The spin control for setting the frequency of the display of frames.
	wxCheckBox   *m_ctrlShowImg;    //!< The check box for allowing the display of dv frames.
	wxCheckBox   *m_ctrlPlayBeep;   //!< The check box for allowing the "beep" sound when recording.
	//@}

	//! @name Events handlers for controling the files recorded.
	//@{
	wxTextCtrl   *m_ctrlSelPath;    //!< Text control for typing the path of video file.
	wxButton     *m_ctrlDiagSelPath;//!< Button for loading the directory dialog box.
	wxTextCtrl   *m_ctrlSelFile;    //!< Text control for typing the name of video file.
	wxChoice     *m_ctrlFileType;   //!< Choice control for selecting the type of video file.
	wxSpinCtrl   *m_ctrlMaxFSize,   //!< The spin control for setting the max file size.
	             *m_ctrlCollSize,   //!< The spin control for setting the size of the collection.
	             *m_ctrlCollMCFSize;//!< The spin control for setting the minimum file size in a collection.
	wxStaticText *m_ctrlMFSInfo;    //!< Static text giving raw dv time from m_maxFSize.
	//@}

	//! @name Controls for setting the time text added to file name.
	//@{
	wxCheckBox   *m_ctrlTimestamp,  //!< Setting ThreadInOut::m_timestamp.
	             *m_ctrlTimecode,   //!< Setting ThreadInOut::m_timecode.
	             *m_ctrlTimesys;    //!< Setting ThreadInOut::m_timesys.
	//@}

	//! @name Controls for setting how/how much to record.
	//@{
	wxSpinCtrl   *m_ctrlFrameEvery; //!< The spin control for setting the frequency of frames to store.
	wxCheckBox   *m_ctrlAutosplit;  //!< The check box for allowing auto split of files by detecting new dv clips.
	wxSpinCtrl   *m_ctrlFrameCount; //!< The spin control for setting the number of frames to store.
	wxStaticText *m_ctrlFCInfo;     //!< Static text giving pal and ntsc dv time from m_frameCount.
	wxCheckBox   *m_ctrlLockstep;   //!< Setting ThreadInOut::m_lockstep.
	wxSpinCtrl   *m_ctrlLockstepMD, //!< Setting ThreadInOut::m_lockstepMaxDrops.
	             *m_ctrlLockstepTD; //!< Setting ThreadInOut::m_lockstepTotalDrops.
	//@}

	// ------------------------------------------------
	// Second "tab" controls (hardware).
	// ------------------------------------------------
	//! @name Controls for settings "hardware" options.
	//@{
	wxCheckBox   *m_ctrlRawPipe;    //!< Setting ThreadInOut::m_rawPipe.
	wxSpinCtrl   *m_ctrlPort,       //!< Setting ThreadInOut::m_port.
	             *m_ctrlNode,       //!< Setting ThreadInOut::m_node.
	             *m_ctrlChannel;    //!< Setting ThreadInOut::m_channel.
	wxComboBox   *m_ctrlGuid;       //!< Setting ThreadInOut::m_guid.
	//@}

	//! @name Controls for settings misc things.
	//@{
	wxCheckBox   *m_ctrlNoAVC,      //!< Setting ThreadInOut::m_noAVC.
	             *m_ctrlNoStop,     //!< Setting ThreadInOut::m_noStop.
	             *m_ctrlRecVAUX;    //!< Setting ThreadInOut::m_recVAUX.
	wxSpinCtrl   *m_ctrlBuffers;    //!< Setting ThreadInOut::m_buffers.
	//@}

	//! @name Controls for settings Jpeg options (not used for now!).
	//@{
	wxCheckBox   *m_ctrlJpegD,      //!< Setting ThreadInOut::m_jpegDeinterlace.
	             *m_ctrlJpegO;      //!< Setting ThreadInOut::m_jpegOverwrite.
	wxSpinCtrl   *m_ctrlJpegQ,      //!< Setting ThreadInOut::m_jpegQuality.
	             *m_ctrlJpegW,      //!< Setting ThreadInOut::m_jpegWidth.
	             *m_ctrlJpegH;      //!< Setting ThreadInOut::m_jpegHeight.
	//@}

	// ------------------------------------------------
	// Third "tab" controls (GUI).
	// ------------------------------------------------
	//! @name Control for setting shortcuts.
	//@{
	wxGrid       *m_grid;           //!< The table managing shortcuts.
	//@}

	//! @name Controls for setting GUI fonts.
	//@{
	wxButton     *m_ctrlMenuFont,   //!< Setting the menu font (not working!).
	             *m_ctrlStatBarFont,//!< Setting the status bar font.
	             *m_ctrlSmallFont,  //!< Setting the "small font".
	             *m_ctrlCtrlFont;   //!< Setting the controls font.
	//@}
	
	//! @name Control for setting GUI mode.
	//@{
	wxCheckBox   *m_ctrlGUImode;        //!< Setting GUI mode ("normal" or "small screen").
	//@}

public:
	ThreadInOut  *m_cth;            //!< The ThreadInOut object.
	wxMutex      *m_mutex;          //!< The mutex protecting m_cth.

	wxFont       *m_menuFont,       //!< The menu font (currently not working).
	             *m_statBarFont,    //!< The status bar font.
	             *m_smallFont,      //!< The small font (used for the top text info).
	             *m_ctrlFont;       //!< The font for all controls in the MainFrame window.

	wxString     *m_menuSC,         //!< The wxString arrays storing the menu shortcuts for each menu entries available.
	             *m_menuText;       //!< The wxString arrays storing the menu text for each menu entries available.
	int           m_numSC;          //!< The number of menu entries managed by m_grid.

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
		
		wxControl *c;    //!< The wxControl pointer.
		ControlCL *n;    //!< The pointer to the next ControlCL object.
	} *m_firstCtrl;          //!< The first element of the chained list.

DECLARE_EVENT_TABLE()
};

#endif /*SETTINGSWINDOW_H_*/
