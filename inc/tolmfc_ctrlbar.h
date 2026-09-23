
#ifndef _INC_TOLMFC_CTRLBAR_H
#define _INC_TOLMFC_CTRLBAR_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_ctrlbar_grip.h>

TOLNS_BEGIN


class TOLMFCDECL TCtrlBar : public TCtrlBarGrip
{
public:
	TCtrlBar();
	virtual ~TCtrlBar();

protected:
	BOOL    m_bActive; // a child has focus
	CString m_sFontFace;

	virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_CTRLBAR_H */
