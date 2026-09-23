
#ifndef _INC_TOLMFC_MAINWND_H
#define _INC_TOLMFC_MAINWND_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>
//{menu
#include <tolc_menu_od.h>
//menu}

#ifndef WM_INITIALUPDATE
#define WM_INITIALUPDATE    0x0364  // (params unused) - sent to children
#endif


TOLNS_BEGIN


class TOLMFCDECL TMainWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(TMainWnd)

protected:
	TMainWnd();

public:
	virtual ~TMainWnd();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle=WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext=NULL);

//{menu
	BOOL IsMenuStyle();
	void SetMenuStyle(BOOL bMenuStyle=TRUE);
//menu}

	afx_msg void OnDestroy();
//{menu
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
//menu}

protected:
//{menu
	TMENU m_tMenu;

	virtual void AfterLoadMenu(HMENU hMenu);
	virtual void AddMenuStylesToMenu(HMENU hMenu);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnViewMenuStyle(UINT uID);
	afx_msg void OnUpdateViewMenuStyle(CCmdUI* pCmdUI);
	afx_msg LRESULT OnUmUpdateUI(WPARAM wp, LPARAM lp);
//menu}
	void DockControlBarInline(CControlBar* pCb, CControlBar* pCbTag);
	//void LoadBarState();
	//void SaveBarState();

	DECLARE_MESSAGE_MAP()
};

//{menu
TOLINL
BOOL TMainWnd::IsMenuStyle()
{
	return m_tMenu!=NULL;
}
//menu}


TOLNS_END

#endif /* _INC_TOLMFC_MAINWND_H */
