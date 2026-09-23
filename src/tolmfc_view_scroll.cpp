
#include "stdafx.h"
#include <tolmfc_view_scroll.h>
#include <tolc_util.h> // Mouse_WheelScrollLines


TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TScrollView


BEGIN_MESSAGE_MAP(TScrollView, TView)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


TScrollView::TScrollView()
{
	m_szLine.cx=1;
	m_szLine.cy=1;

	m_uMouseWheelScrollLines=Mouse_WheelScrollLines();
}

TScrollView::~TScrollView()
{
}

void TScrollView::OnSize(UINT nType, int cx, int cy)
{
	TView::OnSize(nType, cx, cy);

	SetPageHeight(SB_BOTH);
}

void TScrollView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si;
	si.cbSize=sizeof(si);

	si.fMask=SIF_ALL;
	GetScrollInfo(SB_HORZ, &si);
	si.nPos=max(0, si.nPos);

	int nOldPos=si.nPos; // Save the position for comparison later on

	switch(nSBCode)
	{
	case SB_TOP: // user clicked the HOME keyboard key
		si.nPos=si.nMin;
		break;

	case SB_BOTTOM: // user clicked the END keyboard key
		si.nPos=si.nMax;
		break;

	case SB_LINEUP: // user clicked the top arrow
		si.nPos--;
		break;

	case SB_LINEDOWN: // user clicked the bottom arrow
		si.nPos++;
		break;

	case SB_PAGEUP: // user clicked the scroll bar shaft above the scroll box
		si.nPos-=si.nPage;
		break;

	case SB_PAGEDOWN: // user clicked the scroll bar shaft below the scroll box
		si.nPos+=si.nPage;
		break;

	case SB_THUMBTRACK: // user dragged the scroll box
		si.nPos=si.nTrackPos;
		break;
	}

	// Set the position and then retrieve it.  Due to adjustments by Windows it may not be the same as the value set.
	si.fMask=SIF_POS;
	SetScrollInfo(SB_HORZ, &si);

	GetScrollInfo(SB_HORZ, &si);
	if(si.nPos!=nOldPos) // If the position has changed, scroll window and update it
		ScrollWindow((nOldPos-si.nPos)*m_szLine.cx, 0);

	TView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void TScrollView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si;
	si.cbSize=sizeof(si);

	si.fMask=SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	si.nPos=max(0, si.nPos);

	int nOldPos=si.nPos; // Save the position for comparison later on

	switch(nSBCode)
	{
	case SB_TOP: // user clicked the HOME keyboard key
		si.nPos=si.nMin;
		break;

	case SB_BOTTOM: // user clicked the END keyboard key
		si.nPos=si.nMax;
		break;

	case SB_LINEUP: // user clicked the top arrow
		si.nPos--;
		break;

	case SB_LINEDOWN: // user clicked the bottom arrow
		si.nPos++;
		break;

	case SB_PAGEUP: // user clicked the scroll bar shaft above the scroll box
		si.nPos-=si.nPage;
		break;

	case SB_PAGEDOWN: // user clicked the scroll bar shaft below the scroll box
		si.nPos+=si.nPage;
		break;

	case SB_THUMBTRACK: // user dragged the scroll box
		si.nPos=si.nTrackPos;
		break;
	}

	// Set the position and then retrieve it.  Due to adjustments by Windows it may not be the same as the value set.
	si.fMask=SIF_POS;
	SetScrollInfo(SB_VERT, &si);

	GetScrollInfo(SB_VERT, &si);
	if(si.nPos!=nOldPos) // If the position has changed, scroll window and update it
		ScrollWindow(0, (nOldPos-si.nPos)*m_szLine.cy);

	TView::OnVScroll(nSBCode, nPos, pScrollBar);
}

//void TScrollView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	SCROLLINFO si;
//	si.cbSize=sizeof(si);
//
//	si.fMask=SIF_POS|SIF_RANGE|SIF_PAGE;
//	GetScrollInfo(SB_VERT, &si);
//	si.nPos=max(0, si.nPos);
//
//	int nOldPos=si.nPos; // Save the position for comparison later on
//
//	switch(nChar)
//	{
//	case VK_UP:
//		if(si.nPos>si.nMin)
//			si.nPos--;
//		break;
//
//	case VK_PRIOR:
//		if(si.nPos>si.nMin)
//			si.nPos-=si.nPage;
//
//		if(si.nPos<si.nMin)
//			si.nPos=si.nMin;
//		break;
//
//	case VK_NEXT:
//		if(si.nPos<si.nMax)
//			si.nPos+=si.nPage;
//
//		if(si.nPos>si.nMax)
//			si.nPos=si.nMax;
//		break; 
//
//	case VK_DOWN:
//		if(si.nPos<si.nMax)
//			si.nPos++;
//		break;
//
//	case VK_HOME:
//		si.nPos=si.nMin;
//		break;
//
//	case VK_END:
//		si.nPos=si.nMax;
//		break;
//
//	default:
//		TView::OnKeyDown(nChar, nRepCnt, nFlags);
//		return;
//	}
//
//	si.fMask=SIF_POS;
//	SetScrollInfo(SB_VERT, &si);
//
//	GetScrollInfo(SB_VERT, &si);
//	if(si.nPos!=nOldPos) // If the position has changed, scroll window and update it
//		ScrollWindow(0, (nOldPos-si.nPos)*m_uLineHeight);
//
//	TView::OnKeyDown(nChar, nRepCnt, nFlags);
//}

BOOL TScrollView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	WORD wScrollNotify=zDelta<0?SB_LINEDOWN:SB_LINEUP;

	for(int i=m_uMouseWheelScrollLines; i>0; i--)
		SendMessage(WM_VSCROLL, MAKELONG(wScrollNotify, 0));

	return TView::OnMouseWheel(nFlags, zDelta, pt);
}

void TScrollView::SetLineHeight(int nBar, UINT uLine)
{
	if(nBar==SB_HORZ)
		m_szLine.cx=uLine;
	else if(nBar==SB_VERT)
		m_szLine.cy=uLine;
	else if(nBar==SB_BOTH)
	{
		m_szLine.cx=uLine;
		m_szLine.cy=uLine;
	}
	else
		return;

	SetPageHeight(nBar);
}

void TScrollView::SetPageHeight(int nBar)
{
	SCROLLINFO si;
	si.cbSize=sizeof(si);
	si.fMask=SIF_PAGE;

	if(nBar==SB_HORZ)
	{
		si.nPage=(m_rcClient.right-m_rcClient.left)/m_szLine.cx;
		SetScrollInfo(SB_HORZ, &si);
	}
	else if(nBar==SB_VERT)
	{
		si.nPage=(m_rcClient.bottom-m_rcClient.top)/m_szLine.cy;
		SetScrollInfo(SB_VERT, &si);
	}
	else if(nBar==SB_BOTH)
	{
		si.nPage=(m_rcClient.right-m_rcClient.left)/m_szLine.cx;
		SetScrollInfo(SB_HORZ, &si);

		si.nPage=(m_rcClient.bottom-m_rcClient.top)/m_szLine.cy;
		SetScrollInfo(SB_VERT, &si);
	}
}

void TScrollView::SetMinMax(int nBar, int nMin, int nMax)
{
	SCROLLINFO si;
	si.cbSize=sizeof(si);

	si.fMask=SIF_RANGE;
	si.nMin=nMin;
	si.nMax=nMax;
	SetScrollInfo(nBar, &si);
}


TOLNS_END
