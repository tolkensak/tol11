
#ifndef _INC_TOLMFC_VIEW_LIST_H
#define _INC_TOLMFC_VIEW_LIST_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_view_scroll.h>


TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TListView window


class TOLMFCDECL TListView : public TScrollView
{
protected:
	TListView();

public:
	virtual ~TListView();

	int HitTest(POINT pt);
	void GetItemRect(int nItem, LPRECT prc);

	BOOL IsVisible(int nItem);
	void EnsureVisible(int nItem);

	void Select(int nItem);

	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

protected:
	int m_nSelect;

	virtual void BeforeDrawItem(HDC hDC);
	virtual void AfterDrawItem(HDC hDC);
	virtual void DrawItem(HDC hDC, int nItem);

	virtual BOOL SelChanging(int nNew, int nOld);
	virtual void SelChanged(int nNew, int nOld);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_VIEW_LIST_H */
