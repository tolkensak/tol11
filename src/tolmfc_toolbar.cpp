
#include "stdafx.h"
#include <tolmfc_toolbar.h>

TOLNS_BEGIN

// TToolBar

BEGIN_MESSAGE_MAP(TToolBar, CToolBar)
END_MESSAGE_MAP()

TToolBar::TToolBar() : m_dwStyle(0)
{
}

TToolBar::~TToolBar()
{
}

BOOL TToolBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle, CRect rcBorder, UINT nID)
{
	if(!CToolBar::CreateEx(pParentWnd, dwCtrlStyle, dwStyle, rcBorder, nID))
		return FALSE;

	m_dwStyle=dwStyle;
	return TRUE;
}

void TToolBar::DrawBorders(CDC* pDC, CRect& rect)
{
}

void TToolBar::DrawGripper(CDC* pDC, const CRect& rect)
{
	CBrush br;
	br.CreateSysColorBrush(COLOR_3DFACE);
	pDC->FillRect(&rect, &br);
	br.DeleteObject();

	int X=3;
	CDC dc;
	CBitmap bm, *pbmOld;
	dc.CreateCompatibleDC(pDC);
	bm.CreateCompatibleBitmap(pDC, X, 2);
	pbmOld=dc.SelectObject(&bm);

	CRect rc(0, 0, X, 1);
	br.CreateSysColorBrush(COLOR_GRAYTEXT);
	dc.FillRect(&rc, &br);
	br.DeleteObject();

	rc.OffsetRect(0, 1);
	br.CreateSysColorBrush(COLOR_3DFACE);
	dc.FillRect(&rc, &br);
	br.DeleteObject();

	rc.SetRect(rect.left+4, rect.top+4, rect.left+4+X, rect.bottom-5);
	br.CreatePatternBrush(&bm);
	pDC->FillRect(&rc, &br);
}

TOLNS_END
