
#ifndef _INC_TOLMFC_GRIDCTRL_FIT_H
#define _INC_TOLMFC_GRIDCTRL_FIT_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_gridctrl.h>

TOLNS_BEGIN


class TOLMFCDECL TGridCtrlFit : public TGridCtrl
{
protected:
	class TOLMFCDECL THeaderCtrlFit : public CHeaderCtrl
	{
	public:
		THeaderCtrlFit();

		virtual ~THeaderCtrlFit();

		afx_msg void OnMouseMove(UINT nFlags, CPoint point);

		BOOL m_bTracking;

	protected:
		DECLARE_MESSAGE_MAP()
	};

public:
	TGridCtrlFit();
	virtual ~TGridCtrlFit();

	virtual void ContextMenuHeader(CPoint pt);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	THeaderCtrlFit m_hc;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnResizeLastCol(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_GRIDCTRL_FIT_H */
