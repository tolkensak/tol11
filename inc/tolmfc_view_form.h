
#ifndef _INC_TOLMFC_VIEW_FORM_H
#define _INC_TOLMFC_VIEW_FORM_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_view_scroll.h>


TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TFormView window


class TOLMFCDECL TFormView : public TScrollView
{
protected:
	TFormView();

public:
	virtual ~TFormView();

	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	CWnd* m_pPrevFocusWnd;

	void OnTabPressed();

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_VIEW_FORM_H */
