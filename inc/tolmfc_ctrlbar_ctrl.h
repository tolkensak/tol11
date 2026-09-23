
#ifndef _INC_TOLMFC_CTRLBAR_CTRL_H
#define _INC_TOLMFC_CTRLBAR_CTRL_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_ctrlbar.h>

TOLNS_BEGIN



class TOLMFCDECL TCtrlBarCtrl : public TCtrlBar
{
public:
	TCtrlBarCtrl();
	virtual ~TCtrlBarCtrl();

	CWnd* GetCtrl();
	void SetCtrl(CWnd* pWnd);

protected:
	CWnd* m_pCtrl;

	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_CTRLBAR_CTRL_H */
