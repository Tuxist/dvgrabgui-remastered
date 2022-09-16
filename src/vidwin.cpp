/*!
 * @file vidwin.cpp
 * @brief DVGrabGUI video window class implementation.
 * @author Bastien Montagne @htmlonly(<a href="mailto:montagne29@wanadoo.fr">montagne29@wanadoo.fr</a>)@endhtmlonly
 * @version 0.9.4
 * @date 2006, 2007, 2008
 *
 * Nothing to do here, except wxWidgets macro about event handling stuff.
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

#include "vidwin.h"

BEGIN_EVENT_TABLE(VidWin, wxWindow)
	EVT_PAINT(VidWin::OnPaint)
	EVT_MOUSE_EVENTS(VidWin::OnMouse)
END_EVENT_TABLE()

/*!
 * Used for drawing dv frames, and "gui".
 * @param event The wxPaintEvent event.
 */
void VidWin::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	wxString camStat, isRec;
	wxArrayString devCap;
	m_mutex->Lock();
//	camStat = m_cth->m_cameraStatus;
//	isRec = m_cth->m_isRecording;
//	devCap = m_cth->m_devicesCap;
	if(m_cth->m_dvFrame.Ok())
	{
		wxBitmap bmp(m_cth->m_dvFrame);
		dc.DrawBitmap(bmp, 0, 0, true);
	}
	/*else
	{
		dc.SetBrush(*wxBLACK_BRUSH);
		dc.SetPen(*wxBLACK_PEN);
		dc.DrawRectangle(0, 0, 720, 576);
	}*/

	dc.SetLogicalFunction(wxINVERT);
	dc.DrawLine(20, 20, 700, 20);
	dc.DrawLine(700, 20, 700, 556);
	dc.DrawLine(700, 556, 20, 556);
	dc.DrawLine(20, 556, 20, 20);
	dc.DrawLine(360, 15, 360, 41);
	dc.DrawLine(360, 278, 360, 299);
	dc.DrawLine(360, 536, 360, 562);
	dc.DrawLine(15, 288, 41, 288);
	dc.DrawLine(350, 288, 371, 288);
	dc.DrawLine(680, 288, 706, 288);

	wxPoint pts[14];
	if(m_cth->m_running)
	{
		pts[0] = wxPoint(0, 0);
		pts[1] = wxPoint(20, 0);
		pts[2] = wxPoint(20, 20);
		pts[3] = wxPoint(0, 20);
		dc.DrawPolygon(4, pts, 180, 30);
		if(m_cth->m_capture)
		{
			pts[0] = wxPoint(2, 0);
			pts[1] = wxPoint(7, 0);
			pts[2] = wxPoint(7, 20);
			pts[3] = wxPoint(2, 20);
			pts[4] = wxPoint(2, 0);
			pts[5] = wxPoint(12, 0);
			pts[6] = wxPoint(17, 0);
			pts[7] = wxPoint(17, 20);
			pts[8] = wxPoint(12, 20);
			pts[9] = wxPoint(12, 0);
			int i[2] = {5, 5};
			dc.DrawPolyPolygon(2, i, pts, 540, 30);
		}
		else
			dc.DrawCircle(550, 40, 10);
	}
	else
	{
		pts[0] = wxPoint(10, 10);
		pts[1] = wxPoint(0, 0);
		pts[2] = wxPoint(0, 20);
		pts[3] = wxPoint(10, 10);
		pts[4] = wxPoint(10, 0);
		pts[5] = wxPoint(15, 0);
		pts[6] = wxPoint(15, 20);
		pts[7] = wxPoint(10, 20);
		pts[8] = wxPoint(10, 0);
		pts[9] = wxPoint(20, 0);
		pts[10] = wxPoint(25, 0);
		pts[11] = wxPoint(25, 20);
		pts[12] = wxPoint(20, 20);
		pts[13] = wxPoint(20, 0);
		int i[3] = {4, 5, 5};
		dc.DrawPolyPolygon(3, i, pts, 180, 30);
	}
	dc.SetLogicalFunction(wxCOPY);
	m_mutex->Unlock();

	//dc.SetTextBackground(*wxLIGHT_GREY);
//	dc.SetTextForeground(*wxRED);
//	dc.DrawText(wxString(_T("Camera Status:       ")) + camStat, 10, 10);
//	dc.DrawText(wxString(_T("Camera is recording: ")) + isRec, 10, 50);
//	for(uint i = 0; i < devCap.GetCount(); i++)
//		dc.DrawText(devCap[i], 10, 80 * i + 90);
}

/*!
 * Used for managing mouse events (i.e. if left up, start/stop capture(thread)).
 * @param event The wxMouseEvent event.
 */
void VidWin::OnMouse(wxMouseEvent& event)
{
	if(event.RightIsDown())
		wxPostEvent(GetParent(), event);
	else if(event.LeftUp())
	{
		if(event.GetX() < 360)
		{
			wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, BTT_START_THREAD);
			wxPostEvent(GetParent(), evt);
		}
		else
		{
			wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, BTT_START_CAPTURE);
			wxPostEvent(GetParent(), evt);
		}
	}
}
