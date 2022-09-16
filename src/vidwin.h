/*!
 * @file vidwin.h
 * @brief DVGrabGUI video window class declaration.
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

#ifndef VIDWIN_H_
#define VIDWIN_H_

#include "includes.h"    //!< General includes.
#include "threadinout.h" //!< Communication objects between threads.

/*!
 * @class VidWin
 * @brief Window showing dv frames.
 */
class VidWin : public wxWindow
{
public:
	//! @name Constructor/destructor.
	//@{
	//! Basic constructor.
	/*!
	 * @param *par The parent window (i.e. the main frame).
	 * @param *m The mutex protecting access to the ThreadInOut object.
	 * @param *cth A pointer on the ThreadInOut object owned by MainFrame class.
	 */
	VidWin(wxWindow *par, wxMutex *m, ThreadInOut *cth)
	      : wxWindow(par, wxID_ANY, wxDefaultPosition, wxSize(720, 576), wxNO_BORDER)
	{
		m_mutex = m;
		m_cth = cth;
	}
	//! Void destructor.
	~VidWin() {};
	//@}

	//! Managing wxPaintEvent events
	void OnPaint(wxPaintEvent& WXUNUSED(event));
	
	//! Receive mouse events and send them to parent window.
	void OnMouse(wxMouseEvent& event);

	ThreadInOut *m_cth; //!< Shared data between threads.
	wxMutex *m_mutex;   //!< Mutex controlling access to ThreadInOut object.

	//! Classical wxWidgets Macro for events handling
	DECLARE_EVENT_TABLE()
};

#endif /*VIDWIN_H_*/
