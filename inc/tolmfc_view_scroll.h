
#ifndef _INC_TOLMFC_VIEW_SCROLL_H
#define _INC_TOLMFC_VIEW_SCROLL_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_view.h>


TOLNS_BEGIN


class TOLMFCDECL TScrollView : public TView
{
protected:
	TScrollView();

public:
	virtual ~TScrollView();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	UINT GetLineHeight(int nBar) const;
	void SetLineHeight(int nBar, UINT uLine);
	void SetMinMax(int nBar, int nMin, int nMax);

private:
	SIZE m_szLine;
	UINT m_uMouseWheelScrollLines;

protected:
	void SetPageHeight(int nBar);

	DECLARE_MESSAGE_MAP()
};


TOLINL
UINT TScrollView::GetLineHeight(int nBar) const
{
	if(nBar==SB_HORZ)
		return m_szLine.cx;

	if(nBar==SB_VERT)
		return m_szLine.cy;

	return 0;
}


TOLNS_END

#endif /* _INC_TOLMFC_VIEW_SCROLL_H */
