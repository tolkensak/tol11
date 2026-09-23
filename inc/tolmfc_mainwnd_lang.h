
#ifndef _INC_TOLMFC_MAINWND_LANG_H
#define _INC_TOLMFC_MAINWND_LANG_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_mainwnd.h>

TOLNS_BEGIN


class TOLMFCDECL TMainWndLang : public TMainWnd
{
	DECLARE_DYNCREATE(TMainWndLang)

protected:
	TMainWndLang();

public:
	virtual ~TMainWndLang();

	BOOL LoadAccelTable(LPCTSTR lpszResourceName);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

protected:
	virtual void AfterLoadMenu(HMENU hMenu);
	LPCTSTR GetIconWndClass(DWORD dwDefaultStyle, UINT nIDResource);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_MAINWND_LANG_H */
