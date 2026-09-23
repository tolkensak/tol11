
#ifndef _INC_TOLMFC_SDIMV_MAINWND_LANG_H
#define _INC_TOLMFC_SDIMV_MAINWND_LANG_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_SDIMV_MainWnd.h>

TOLNS_BEGIN


class TOLMFCDECL TSDIMVMainWndLang : public TSDIMVMainWnd
{
	DECLARE_DYNCREATE(TSDIMVMainWndLang)

protected:
	TSDIMVMainWndLang();

public:
	virtual ~TSDIMVMainWndLang();

	BOOL LoadAccelTable(LPCTSTR lpszResourceName);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	afx_msg LRESULT OnUmUpdateUI(WPARAM wp, LPARAM lp);

protected:
	virtual void AfterLoadMenu(HMENU hMenu);
	LPCTSTR GetIconWndClass(DWORD dwDefaultStyle, UINT nIDResource);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_SDIMV_MAINWND_LANG_H */
