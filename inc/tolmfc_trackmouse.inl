
#ifndef _INC_TOLMFC_TRACKMOUSE_INL
#define _INC_TOLMFC_TRACKMOUSE_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

TOLNS_BEGIN


///////////////////////////////////////////////////////////////////////////////
//
// TTrackMouse

TOLINL
BOOL TTrackMouse::IsBound() const
{
	return m_hWnd!=NULL;
}

TOLINL
BOOL TTrackMouse::IsOver() const
{
	return m_bOver;
}


TOLNS_END

#endif // _INC_TOLMFC_TRACKMOUSE_INL
