
#include "stdafx.h"
#include <tolmfc_view_form.h>
#include <tolmfc_memdc.h> // double buffer paint
#include <tolmfc_util.h> // IsKeyPressed


TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TFormView window


BEGIN_MESSAGE_MAP(TFormView, TScrollView)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
END_MESSAGE_MAP()


TFormView::TFormView()
	: m_pPrevFocusWnd(NULL)
{
}

TFormView::~TFormView()
{
}

void TFormView::OnPaint()
{
	CPaintDC dc(this);
	TMemDC mdc(dc.m_hDC);
	FillRect(mdc.m_hDC, &m_rcClient, (HBRUSH)(COLOR_3DFACE+1));
}

void TFormView::OnSetFocus(CWnd* pOldWnd)
{
	TScrollView::OnSetFocus(pOldWnd);

	if(m_pPrevFocusWnd)
		m_pPrevFocusWnd=SetFocus();
}

void TFormView::OnKillFocus(CWnd* pNewWnd)
{
	TScrollView::OnKillFocus(pNewWnd);

	CWnd* pWnd=GetFocus();
	if(pWnd && IsChild(pWnd))
		m_pPrevFocusWnd=pWnd;
}

void TFormView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar==VK_TAB)
	{
		OnTabPressed();
		return;
	}

	TScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void TFormView::OnTabPressed()
{
	CWnd* pFocus=GetFocus();

	if(!pFocus || !IsChild(pFocus))
		pFocus=GetWindow(GW_CHILD);

	if(pFocus)
		GetNextDlgTabItem(pFocus, IsKeyPressed(VK_SHIFT))->SetFocus();
}


TOLNS_END
