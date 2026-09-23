
#include "stdafx.h"
#include <tolmfc_view.h>


TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TView


BEGIN_MESSAGE_MAP(TView, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
#ifdef _VIEW_TRACK_MOUSE
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
#endif
END_MESSAGE_MAP()


TView::TView()
	: m_rcClient(0, 0, 0, 0)
{
}

TView::~TView()
{
}

BOOL TView::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle|=WS_EX_CLIENTEDGE;
	//cs.style&=~WS_BORDER;
	cs.lpszClass=AfxRegisterWndClass(CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>(COLOR_WINDOW+1)*/);

	return TRUE;
}

BOOL TView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void TView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	m_rcClient.right=cx;
	m_rcClient.bottom=cy;
}

int TView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if(GetStyle()&WS_TABSTOP)
		SetFocus();

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

#ifdef _VIEW_TRACK_MOUSE
void TView::OnMouseHover(UINT nFlags, CPoint point)
{
	CWnd::OnMouseHover(nFlags, point);

	if(m_trackMouse.OnHover())
		SetFocus();
}

void TView::OnMouseLeave()
{
	CWnd::OnMouseLeave();

	m_trackMouse.OnLeave();
}
#endif


TOLNS_END
