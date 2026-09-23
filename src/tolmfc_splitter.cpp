
#include "stdafx.h"
#include <tolmfc_splitter.h>


TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TSplitter::TPane

TSplitter::TPane::TPane()
	: m_dwStyle(0)
	, m_pWnd(NULL)
	, m_nBreadth(0)
	, m_nMinBreadth(0)
	, m_nMaxBreadth(0)
{
}

TSplitter::TPane::~TPane()
{
}

BOOL TSplitter::TPane::AddBreadth(int nBreadth)
{
	if(!IsAutoSize())
	{
		int n=m_nBreadth+nBreadth;

		if(m_nMinBreadth>0 && n<m_nMinBreadth)
			return FALSE;

		if(m_nMaxBreadth>0 && n>m_nMaxBreadth)
			return FALSE;

		m_nBreadth=n;
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TSplitter

BEGIN_MESSAGE_MAP(TSplitter, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


TSplitter::TSplitter(BOOL bHorz, int nWeight)
	: m_bHorz(bHorz)
	, m_nWeight(nWeight)
#ifdef _DRAG_DRAW_LINE
	, m_hbmDrag(NULL)
#endif // _DRAG_DRAW_LINE
{
	if(m_nWeight<1)
		m_nWeight=1;
}

TSplitter::~TSplitter()
{
}

BOOL TSplitter::Create(CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(NULL, NULL, 0, CRect(0, 0, 0, 0), pParentWnd, nID, NULL);
}

BOOL TSplitter::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle=0;
	cs.style=WS_CHILD|WS_VISIBLE;
	cs.lpszClass=AfxRegisterWndClass(CS_DBLCLKS, m_bHorz?(::LoadCursor(NULL, IDC_SIZENS)):(::LoadCursor(NULL, IDC_SIZEWE)), reinterpret_cast<HBRUSH>(COLOR_3DFACE+1));

	return TRUE;
}

void TSplitter::SetPane(BOOL bTopLeft, const TSplitter::TPane& pane)
{
	m_pane[bTopLeft?0:1]=pane;
}

void TSplitter::Size(int cx, int cy)
{
	SetRect(&m_rcOwn, 0, 0, cx, cy);
	Size();
}

void TSplitter::Size(int l, int t, int r, int b)
{
	SetRect(&m_rcOwn, l, t, r, b);
	Size();
}

void TSplitter::Size(LPRECT prc)
{
	if(prc)
		m_rcOwn=*prc;

	CWnd* pWnd;
	TPane* pPane;
	RECT rc1, rc2;

	// get the pane rect placed top or left
	pPane=m_pane;
	rc1=m_rcOwn;

	if(pPane->IsAutoSize())
	{
		if(IsHorz())
			rc1.bottom-=(pPane+1)->GetBreadth()-m_nWeight;
		else
			rc1.right-=(pPane+1)->GetBreadth()-m_nWeight;
	}
	else
	{
		if(IsHorz())
			rc1.bottom=pPane->GetBreadth();
		else
			rc1.right=pPane->GetBreadth();
	}

	// move the splitter
#ifdef _DRAG_DRAW_LINE
	if(IsHorz())
		m_rcWindow.SetRect(rc1.left, rc1.bottom, rc1.right, rc1.bottom+m_nWeight);
	else
		m_rcWindow.SetRect(rc1.right, rc1.top, rc1.right+m_nWeight, rc1.bottom);

	MoveWindow(&m_rcWindow);
#else // _DRAG_DRAW_LINE
	if(IsHorz())
		MoveWindow(rc1.left, rc1.bottom, rc1.right-rc1.left, m_nWeight);
	else
		MoveWindow(rc1.right, rc1.top, m_nWeight, rc1.bottom-rc1.top);
#endif // _DRAG_DRAW_LINE

	// size the pane placed top or left
	pWnd=pPane->GetWnd();
	if(pWnd)
	{
		if(pPane->IsSplitter()) // continue size if it is a splitter
			((TSplitter*)pWnd)->Size(&rc1);
		else
			pWnd->MoveWindow(&rc1);
	}

	// get the pane rect placed bottom or right
	pPane++;
	pWnd=pPane->GetWnd();
	if(!pWnd)
		return;

	rc2=m_rcOwn;

	if(IsHorz())
		rc2.top=rc1.bottom+m_nWeight;
	else
		rc2.left=rc1.right+m_nWeight;

	// size the pane placed bottom or right
	if(pPane->IsSplitter()) // continue size if it is a splitter
		((TSplitter*)pWnd)->Size(&rc2);
	else
		pWnd->MoveWindow(&rc2);
}

void TSplitter::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	SetCapture();

#ifdef _DRAG_DRAW_LINE
	if(m_hbmDrag)
	{
		DeleteObject(m_hbmDrag);
		m_hbmDrag=NULL;
	}

	CRect rc;
	CWnd* pParent=GetParent();
	pParent->GetClientRect(&rc);
	CClientDC dc(pParent);
	m_hbmDrag=CreateCompatibleBitmap(dc.m_hDC, rc.Width(), rc.Height());

	HDC hMemDC=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hbmOld=(HBITMAP)SelectObject(hMemDC, m_hbmDrag);
	BitBlt(hMemDC, 0, 0, rc.Width(), rc.Height(), dc.m_hDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hbmOld);

	m_ptPrev.SetPoint(-1, -1);
#else // _DRAG_DRAW_LINE
	CRect rc;
	CWnd* pWnd;
	TPane* pPane;
	CRect rcClip(m_rcOwn);

	GetParent()->ClientToScreen(&rcClip);

	pPane=m_pane;
	if(pPane->IsSplitter())
	{
		pWnd=pPane->GetWnd();
		if(pWnd && ((TSplitter*)pWnd)->IsHorz()==IsHorz())
		{
			pWnd->GetWindowRect(&rc);

			if(IsHorz())
				rcClip.top=rc.bottom;
			else
				rcClip.left=rc.right;
		}
	}

	pPane++;
	if(pPane->IsSplitter())
	{
		pWnd=pPane->GetWnd();
		if(pWnd && ((TSplitter*)pWnd)->IsHorz()==IsHorz())
		{
			pWnd->GetWindowRect(&rc);

			if(IsHorz())
				rcClip.bottom=rc.top;
			else
				rcClip.right=rc.left;
		}
	}

	ClipCursor(&rcClip);
	m_ptPrev=point;
#endif // _DRAG_DRAW_LINE
}

void TSplitter::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

#ifdef _DRAG_DRAW_LINE
	if(nFlags&MK_LBUTTON)
	{
		CWnd* pParent=GetParent();

		CRect rc(m_rcWindow);
		pParent->ScreenToClient(&rc);

		MapWindowPoints(pParent, &point, 1);

		CClientDC dc(pParent);

		if(m_hbmDrag && m_ptPrev.x!=-1 && m_ptPrev.y!=-1)
		{
			HDC hMemDC=CreateCompatibleDC(dc.m_hDC);
			HBITMAP hbmOld=(HBITMAP)SelectObject(hMemDC, m_hbmDrag);

			if(IsHorz())
				BitBlt(dc.m_hDC, rc.left, m_ptPrev.y, m_rcWindow.Width(), m_rcWindow.Height(), hMemDC, rc.left, m_ptPrev.y, SRCCOPY);
			else
				BitBlt(dc.m_hDC, m_ptPrev.x, rc.top, m_rcWindow.Width(), m_rcWindow.Height(), hMemDC, m_ptPrev.x, rc.top, SRCCOPY);

			SelectObject(hMemDC, hbmOld);
		}

		if(IsHorz())
		{
			dc.MoveTo(rc.left, point.y);
			dc.LineTo(rc.right, point.y);
		}
		else
		{
			dc.MoveTo(point.x, rc.top);
			dc.LineTo(point.x, rc.bottom);
		}

		m_ptPrev=point;
	}
#else // _DRAG_DRAW_LINE
	if(nFlags&MK_LBUTTON)
	{
		int n;

		if(IsHorz())
			n=point.y-m_ptPrev.y;
		else
			n=point.x-m_ptPrev.x;

		if(m_pane[0].AddBreadth(n) && m_pane[1].AddBreadth(-n))
			Size();
	}
#endif // _DRAG_DRAW_LINE
}

void TSplitter::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	if(GetCapture()==this)
	{
		ReleaseCapture();
		ClipCursor(NULL);
	}

#ifdef _DRAG_DRAW_LINE
	if(m_hbmDrag && m_ptPrev.x!=-1 && m_ptPrev.y!=-1)
	{
		CWnd* pParent=GetParent();

		CRect rc(m_rcWindow);
		pParent->ScreenToClient(&rc);

		CClientDC dc(pParent);
		HDC hMemDC=CreateCompatibleDC(dc.m_hDC);
		HBITMAP hbmOld=(HBITMAP)SelectObject(hMemDC, m_hbmDrag);

		if(IsHorz())
			BitBlt(dc.m_hDC, rc.left, m_ptPrev.y, m_rcWindow.Width(), m_rcWindow.Height(), hMemDC, rc.left, m_ptPrev.y, SRCCOPY);
		else
			BitBlt(dc.m_hDC, m_ptPrev.x, rc.top, m_rcWindow.Width(), m_rcWindow.Height(), hMemDC, m_ptPrev.x, rc.top, SRCCOPY);

		SelectObject(hMemDC, hbmOld);

		DeleteObject(m_hbmDrag);
		m_hbmDrag=NULL;

		m_ptPrev.SetPoint(-1, -1);
	}
#endif // _DRAG_DRAW_LINE
}


TOLNS_END
