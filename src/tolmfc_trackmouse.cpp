
#include "stdafx.h"
#include <tolmfc_trackmouse.h>

TOLNS_BEGIN


//extern "C" WINUSERAPI BOOL WINAPI TrackMouseEvent(LPTRACKMOUSEEVENT lpEventTrack);


///////////////////////////////////////////////////////////////////////////////
//
// TTrackMouse

TTrackMouse::TTrackMouse()
	: m_hWnd(NULL)
	, m_bOver(FALSE)
{
}

TTrackMouse::~TTrackMouse()
{
}

void TTrackMouse::Bind(HWND hWnd)
{
	m_hWnd=hWnd;
}

BOOL TTrackMouse::OnHover()
{
	if(m_hWnd && !m_bOver)
	{
		m_bOver=TRUE;

		TRACKMOUSEEVENT tme={ sizeof TRACKMOUSEEVENT, TME_LEAVE, m_hWnd, 0 };
		::TrackMouseEvent(&tme);

		return TRUE;
	}

	return FALSE;
}

BOOL TTrackMouse::OnLeave()
{
	if(m_hWnd && m_bOver)
	{
		m_bOver=FALSE;

		TRACKMOUSEEVENT tme={ sizeof TRACKMOUSEEVENT, TME_HOVER, m_hWnd, 100 };
		::TrackMouseEvent(&tme);

		return TRUE;
	}

	return FALSE;
}


TOLNS_END
