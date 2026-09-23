
#include "stdafx.h"
#include <tolmfc_ctrlbar_grip.h>

TOLNS_BEGIN

// TCtrlBarButton

TCtrlBarGrip::TCtrlBarButton::TCtrlBarButton()
{
	bRaised=FALSE;
	bPushed=FALSE;
}

void TCtrlBarGrip::TCtrlBarButton::Paint(CDC* pDC, COLORREF crFg)
{
	CRect rc=GetRect();

	if(bPushed)
		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));
	else if(bRaised)
		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));

	COLORREF clrOldTextColor=pDC->SetTextColor(crFg);
	int nPrevBkMode=pDC->SetBkMode(TRANSPARENT);
	CFont font;
	int ppi=pDC->GetDeviceCaps(LOGPIXELSX);
	int pointsize=MulDiv(60, 96, ppi); // 6 points at 96 ppi
	font.CreatePointFont(pointsize, _T("Marlett"));
	CFont* oldfont=pDC->SelectObject(&font);

	//TCHAR ch=0x72;
	//pDC->TextOut(ptOrg.x+2, ptOrg.y+2, &ch, 1); // x-like
	pDC->TextOut(ptOrg.x+2, ptOrg.y+2, CString(_T("r"))); // x-like

	pDC->SelectObject(oldfont);
	pDC->SetBkMode(nPrevBkMode);
	pDC->SetTextColor(clrOldTextColor);
}

// TCtrlBarGrip

TCtrlBarGrip::TCtrlBarGrip()
{
	m_cyGripper=12;
}

TCtrlBarGrip::~TCtrlBarGrip()
{
}

BEGIN_MESSAGE_MAP(TCtrlBarGrip, TCtrlBarSize)
	ON_WM_NCLBUTTONUP()
	ON_WM_NCHITTEST()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

void TCtrlBarGrip::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	if(nHitTest==HTCLOSE)
		m_pDockSite->ShowControlBar(this, FALSE, FALSE); // hide

	TCtrlBarSize::OnNcLButtonUp(nHitTest, point);
}

void TCtrlBarGrip::NcCalcClient(LPRECT pRc, UINT nDockBarID)
{
	CRect rcBar(pRc); // save the bar rect

	// subtract edges
	TCtrlBarSize::NcCalcClient(pRc, nDockBarID);

	if(!HasGripper())
		return;

	CRect rc(pRc); // the client rect as calculated by the base class

	BOOL bHorz=(nDockBarID==AFX_IDW_DOCKBAR_TOP) || (nDockBarID==AFX_IDW_DOCKBAR_BOTTOM);

	if(bHorz)
		rc.DeflateRect(m_cyGripper, 0, 0, 0);
	else
		rc.DeflateRect(0, m_cyGripper, 0, 0);

	// set position for the "x" (hide bar) button
	CPoint ptOrgBtn;
	if(bHorz)
		ptOrgBtn=CPoint(rc.left-13, rc.top);
	else
		ptOrgBtn=CPoint(rc.right-12, rc.top-13);

	m_btnHide.Move(ptOrgBtn-rcBar.TopLeft());

	*pRc=rc;
}

void TCtrlBarGrip::NcPaintGripper(CDC* pDC, CRect rcClient)
{
	if(!HasGripper())
		return;

	// paints a simple "two raised lines" gripper override this if you want a more sophisticated gripper
	CRect gripper=rcClient;
	CRect rcbtn=m_btnHide.GetRect();
	BOOL bHorz=IsHorzDocked();

	gripper.DeflateRect(1, 1);
	if(bHorz)
	{   // gripper at left
		gripper.left -=m_cyGripper;
		gripper.right=gripper.left+3;
		gripper.top=rcbtn.bottom+3;
	}
	else
	{   // gripper at top
		gripper.top -=m_cyGripper;
		gripper.bottom=gripper.top+3;
		gripper.right=rcbtn.left-3;
	}

	gripper.OffsetRect(bHorz ? -1 : 0, bHorz ? 0 : 0);
	pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));

	gripper.OffsetRect(bHorz ? 4 : 0, bHorz ? 0 : 4);
	pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));

	m_btnHide.Paint(pDC, ::GetSysColor(COLOR_BTNTEXT));
}

#if _MSC_VER>1400
LRESULT
#else
UINT
#endif
TCtrlBarGrip::OnNcHitTest(CPoint point)
{
	CRect rcBar;
	GetWindowRect(rcBar);

	UINT nRet = (UINT)TCtrlBarSize::OnNcHitTest(point);
	if(nRet !=HTCLIENT)
		return nRet;

	CRect rc=m_btnHide.GetRect();
	rc.OffsetRect(rcBar.TopLeft());
	if(rc.PtInRect(point))
		return HTCLOSE;

	return HTCLIENT;
}

// TCtrlBarGrip implementation helpers

void TCtrlBarGrip::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	UNUSED_ALWAYS(bDisableIfNoHndler);
	UNUSED_ALWAYS(pTarget);

	if(!HasGripper())
		return;

	BOOL bNeedPaint=FALSE;

	CPoint pt;
	::GetCursorPos(&pt);
	BOOL bHit=(OnNcHitTest(pt)==HTCLOSE);
	BOOL bLButtonDown=(::GetKeyState(VK_LBUTTON)<0);

	BOOL bWasPushed=m_btnHide.bPushed;
	m_btnHide.bPushed=bHit && bLButtonDown;

	BOOL bWasRaised=m_btnHide.bRaised;
	m_btnHide.bRaised=bHit && !bLButtonDown;

	bNeedPaint |=(m_btnHide.bPushed ^ bWasPushed) || (m_btnHide.bRaised ^ bWasRaised);

	if(bNeedPaint)
		SendMessage(WM_NCPAINT);
}

BOOL TCtrlBarGrip::HasGripper() const
{
//?
//#if defined(_SCB_MINIFRAME_CAPTION) || !defined(_SCB_REPLACE_MINIFRAME)
//	// if the miniframe has a caption, don't display the gripper
//	if(IsFloating())
//		return FALSE;
//#endif //_SCB_MINIFRAME_CAPTION
//	return TRUE;
//?
	return !IsFloating();
}

TOLNS_END
