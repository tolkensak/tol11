
#ifndef _INC_TOLMFC_TRACKMOUSE_H
#define _INC_TOLMFC_TRACKMOUSE_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <tolmfc.h>
#include <tolcpp_object.h>

TOLNS_BEGIN


///////////////////////////////////////////////////////////////////////////////
//
// TTrackMouse

class TOLMFCDECL TTrackMouse : public Object
{
public:
	TTrackMouse();
	virtual ~TTrackMouse();

	BOOL IsBound() const;
	BOOL IsOver() const;

	void Bind(HWND hWnd);

	BOOL OnHover();
	BOOL OnLeave();

protected:
	HWND m_hWnd;
	BOOL m_bOver;
};


TOLNS_END

#endif /* _INC_TOLMFC_TRACKMOUSE_H */


#include <tolmfc_trackmouse.inl>
