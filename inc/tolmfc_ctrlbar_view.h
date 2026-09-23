
#ifndef _INC_TOLMFC_CTRLBAR_VIEW_H
#define _INC_TOLMFC_CTRLBAR_VIEW_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_ctrlbar.h>

TOLNS_BEGIN


class TOLMFCDECL TCtrlBarView : public TCtrlBar
{
public:
	TCtrlBarView();
	virtual ~TCtrlBarView();

	CView* GetView();
	BOOL CreateView(CRuntimeClass *pViewClass, CCreateContext *pContext=NULL);

protected:
	CView* m_pView;

	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_CTRLBAR_VIEW_H */
