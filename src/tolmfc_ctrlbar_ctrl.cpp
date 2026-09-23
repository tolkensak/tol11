
#include "stdafx.h"
#include <tolmfc_ctrlbar_ctrl.h>

TOLNS_BEGIN


BEGIN_MESSAGE_MAP(TCtrlBarCtrl, TCtrlBar)
	ON_WM_SIZE()
END_MESSAGE_MAP()


TCtrlBarCtrl::TCtrlBarCtrl()
	: m_pCtrl(NULL)
{
}

TCtrlBarCtrl::~TCtrlBarCtrl()
{
}

void TCtrlBarCtrl::OnSize(UINT nType, int cx, int cy) 
{
	TCtrlBar::OnSize(nType, cx, cy);

	if(m_pCtrl)
	{
		int dx=GetSystemMetrics(SM_CXEDGE);
		int dy=GetSystemMetrics(SM_CYEDGE);
		m_pCtrl->MoveWindow(dx, dy, cx-2*dx, cy-2*dy);
	}
}

CWnd* TCtrlBarCtrl::GetCtrl()
{
	return m_pCtrl;
}

void TCtrlBarCtrl::SetCtrl(CWnd* pCtrl)
{
	m_pCtrl=pCtrl;
}


TOLNS_END
