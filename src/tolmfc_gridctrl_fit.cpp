
#include "stdafx.h"
#include <tolmfc_gridctrl_fit.h>

TOLNS_BEGIN


BEGIN_MESSAGE_MAP(TGridCtrlFit::THeaderCtrlFit, CHeaderCtrl)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


TGridCtrlFit::THeaderCtrlFit::THeaderCtrlFit()
	: m_bTracking(FALSE)
{
}

TGridCtrlFit::THeaderCtrlFit::~THeaderCtrlFit()
{
}

void TGridCtrlFit::THeaderCtrlFit::OnMouseMove(UINT nFlags, CPoint point)
{
	CHeaderCtrl::OnMouseMove(nFlags, point);

	if(m_bTracking)
		GetParent()->PostMessage(TCFM_RESIZELASTCOL);
}


BEGIN_MESSAGE_MAP(TGridCtrlFit, TGridCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(TCFM_RESIZELASTCOL, OnResizeLastCol)
	ON_NOTIFY(HDN_BEGINTRACKA, 0, OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, OnHdnBegintrack)
	ON_NOTIFY(HDN_ENDTRACKA, 0, OnHdnEndtrack)
	ON_NOTIFY(HDN_ENDTRACKW, 0, OnHdnEndtrack)
END_MESSAGE_MAP()


TGridCtrlFit::TGridCtrlFit()
{
}

TGridCtrlFit::~TGridCtrlFit()
{
}

int TGridCtrlFit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(TGridCtrl::OnCreate(lpCreateStruct)==-1)
		return -1;

	m_hc.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	return 0;
}

void TGridCtrlFit::OnDestroy()
{
	if(m_hc.GetSafeHwnd())
		m_hc.UnsubclassWindow();

	TGridCtrl::OnDestroy();
}

void TGridCtrlFit::OnSize(UINT nType, int cx, int cy)
{
	TGridCtrl::OnSize(nType, cx, cy);

	PostMessage(TCFM_RESIZELASTCOL);
}

LRESULT TGridCtrlFit::OnResizeLastCol(WPARAM, LPARAM)
{
	if(m_hc.GetSafeHwnd())
	{
		int i=m_hc.GetItemCount()-1;
		if(i>=0)
			SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}

	return 0;
}

void TGridCtrlFit::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_hc.m_bTracking=TRUE;

	*pResult=0;
}

void TGridCtrlFit::OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_hc.m_bTracking=FALSE;

	*pResult=0;
}

void TGridCtrlFit::ContextMenuHeader(CPoint pt)
{
	TGridCtrl::ContextMenuHeader(pt);

	PostMessage(TCFM_RESIZELASTCOL);
}


TOLNS_END
