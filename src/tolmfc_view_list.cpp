
#include "stdafx.h"
#include <tolmfc_view_list.h>
#include <tolmfc_memdc.h> // double buffer paint


TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TListView window


BEGIN_MESSAGE_MAP(TListView, TScrollView)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


TListView::TListView()
	: m_nSelect(-1)
{
}

TListView::~TListView()
{
}

int TListView::HitTest(POINT pt)
{
	if(PtInRect(&m_rcClient, pt))
	{
		SCROLLINFO si;
		si.cbSize=sizeof(si);

		si.fMask=SIF_POS|SIF_RANGE;
		GetScrollInfo(SB_VERT, &si);
		si.nPos=max(0, si.nPos);

		int nItem=si.nPos+pt.y/GetLineHeight(SB_VERT);
		if(nItem<=si.nMax)
			return nItem;
	}

	return -1;
}

void TListView::GetItemRect(int nItem, LPRECT prc)
{
	ASSERT(nItem>=0);
	ASSERT(prc);

	SCROLLINFO si;
	si.cbSize=sizeof(si);

	si.fMask=SIF_POS;
	GetScrollInfo(SB_VERT, &si);
	si.nPos=max(0, si.nPos);

	nItem-=si.nPos;

	prc->left=m_rcClient.left;
	prc->top=m_rcClient.top+nItem*GetLineHeight(SB_VERT);
	prc->right=m_rcClient.right;
	prc->bottom=prc->top+GetLineHeight(SB_VERT);
}

BOOL TListView::IsVisible(int nItem)
{
	SCROLLINFO si;
	si.cbSize=sizeof(si);

	si.fMask=SIF_POS|SIF_PAGE;
	GetScrollInfo(SB_VERT, &si);
	si.nPos=max(0, si.nPos);
	
	if(nItem<si.nPos || nItem>=si.nPos+(int)si.nPage)
		return FALSE;

	return TRUE;
}

void TListView::EnsureVisible(int nItem)
{
	if(nItem==-1)
		return;

	SCROLLINFO si;
	si.cbSize=sizeof(si);

	si.fMask=SIF_POS|SIF_PAGE;
	GetScrollInfo(SB_VERT, &si);
	si.nPos=max(0, si.nPos);
	
	int nOldPos=si.nPos;

	if(nItem<si.nPos)
		si.nPos=nItem;
	else if(nItem>=si.nPos+(int)si.nPage)
		si.nPos=nItem-(int)si.nPage+1;
	else
		return;

	si.fMask=SIF_POS;
	SetScrollInfo(SB_VERT, &si);

	GetScrollInfo(SB_VERT, &si);
	if(si.nPos!=nOldPos)
		ScrollWindow(0, (nOldPos-si.nPos)*GetLineHeight(SB_VERT));
}

void TListView::OnPaint()
{
	CPaintDC dc(this);
	TMemDC mdc(dc.m_hDC);
	HDC hDC=mdc.m_hDC;

	FillRect(hDC, &m_rcClient, (HBRUSH)(COLOR_WINDOW+1));

	SCROLLINFO si;
	si.fMask=SIF_POS|SIF_PAGE|SIF_RANGE;

	GetScrollInfo(SB_VERT, &si);
	int nFirst=max(0, si.nPos);
	int nLast=min(si.nMax, si.nPos+(int)si.nPage);

	BeforeDrawItem(hDC);

	for(int i=nFirst; i<=nLast; i++)
		DrawItem(hDC, i);

	AfterDrawItem(hDC);
}

void TListView::BeforeDrawItem(HDC hDC)
{
}

void TListView::AfterDrawItem(HDC hDC)
{
}

void TListView::DrawItem(HDC hDC, int nItem)
{
}

void TListView::OnLButtonUp(UINT nFlags, CPoint point)
{
	TScrollView::OnLButtonUp(nFlags, point);
	Select(HitTest(point));
}

void TListView::Select(int nItem)
{
	if(nItem==m_nSelect)
		return;

	if(!SelChanging(nItem, m_nSelect))
		return;

	int nOld=m_nSelect;
	m_nSelect=nItem;

	EnsureVisible(m_nSelect);
	SelChanged(m_nSelect, nOld);
}

BOOL TListView::SelChanging(int nNew, int nOld)
{
	return TRUE;
}

void TListView::SelChanged(int nNew, int nOld)
{
	CClientDC dc(this);
	HDC hDC=dc.m_hDC;

	BeforeDrawItem(hDC);
	DrawItem(hDC, nNew);
	DrawItem(hDC, nOld);
	AfterDrawItem(hDC);
}

void TListView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SCROLLINFO si;
	si.cbSize=sizeof(si);

	si.fMask=SIF_RANGE|SIF_PAGE;
	GetScrollInfo(SB_VERT, &si);

	int nSel=m_nSelect;

	switch(nChar)
	{
	case VK_UP:
		if(nSel>si.nMin)
			nSel--;
		break;

	case VK_PRIOR:
		if(nSel>si.nMin)
			nSel-=si.nPage;

		if(nSel<si.nMin)
			nSel=si.nMin;
		break;

	case VK_NEXT:
		if(nSel<si.nMax)
			nSel+=si.nPage;

		if(nSel>si.nMax)
			nSel=si.nMax;
		break; 

	case VK_DOWN:
		if(nSel<si.nMax)
			nSel++;
		break;

	case VK_HOME:
		nSel=si.nMin;
		break;

	case VK_END:
		nSel=si.nMax;
		break;

	default:
		TScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	Select(nSel);

	TScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


TOLNS_END
