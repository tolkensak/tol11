
#ifndef _INC_TOLMFC_TOOLBAR_H
#define _INC_TOLMFC_TOOLBAR_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

TOLNS_BEGIN

// TToolBar

class TOLMFCDECL TToolBar : public CToolBar
{
public:
	TToolBar();
	virtual ~TToolBar();

	virtual BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle=TBSTYLE_FLAT, DWORD dwStyle=WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC, CRect rcBorder=CRect(0, 0, 0, 0), UINT nID=AFX_IDW_TOOLBAR);

	virtual void DrawBorders(CDC* pDC, CRect& rect);
	virtual void DrawGripper(CDC* pDC, const CRect& rect);

	DWORD m_dwStyle;
	DECLARE_MESSAGE_MAP()
};

TOLNS_END

#endif /* _INC_TOLMFC_TOOLBAR_H */
