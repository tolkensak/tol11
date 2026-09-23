
#ifndef _INC_TOLMFC_VIEW_H
#define _INC_TOLMFC_VIEW_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

TOLNS_BEGIN

class TOLMFCDECL TView : public CWnd
{
protected:
	TView();

public:
	virtual ~TView();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
#ifdef _VIEW_TRACK_MOUSE
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
#endif

protected:
	CRect m_rcClient;
#ifdef _VIEW_TRACK_MOUSE
	TTrackMouse m_trackMouse;
#endif

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_VIEW_H */
