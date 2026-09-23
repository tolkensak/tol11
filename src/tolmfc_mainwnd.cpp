
#include "stdafx.h"
#include <tolmfc_mainwnd.h>

TOLNS_BEGIN


IMPLEMENT_DYNCREATE(TMainWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(TMainWnd, CFrameWnd)
//{menu
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_COMMAND_RANGE(ID_MENU_STYLE_1, ID_MENU_STYLE_1+TMENU_STYLECOUNT, &TMainWnd::OnViewMenuStyle)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MENU_STYLE_1, ID_MENU_STYLE_1+TMENU_STYLECOUNT, &TMainWnd::OnUpdateViewMenuStyle)
	ON_MESSAGE(UM_UPDATE_UI, &TMainWnd::OnUmUpdateUI)
	ON_WM_SETTINGCHANGE()
//menu}
	ON_WM_DESTROY()
END_MESSAGE_MAP()


TMainWnd::TMainWnd()
	: m_tMenu(NULL)
{
}

TMainWnd::~TMainWnd()
{
//{menu
	MenuOD_Kill(m_tMenu);
//menu}
}

BOOL TMainWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	RECT rc=rect;
	CWinApp* pApp=AfxGetApp();
	CString str=pApp->GetProfileString(_T("Settings"), _T("Placement"));

	if(!str.IsEmpty())
	{
		UINT nCmdShow;

		if(_stscanf(str, _T("%u,%d,%d,%d,%d"), &nCmdShow, &rc.left, &rc.top, &rc.right, &rc.bottom)==5)
		{
			if(nCmdShow!=SW_SHOWMINIMIZED)
				pApp->m_nCmdShow=nCmdShow;
		}
		else
			rc=rect;
	}

	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rc, pParentWnd, lpszMenuName, dwExStyle, pContext);
}

void TMainWnd::OnDestroy()
{
	CString str;
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	str.Format(_T("%u,%d,%d,%d,%d"), wp.showCmd, wp.rcNormalPosition.left, wp.rcNormalPosition.top, wp.rcNormalPosition.right, wp.rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("Placement"), str);

	CFrameWnd::OnDestroy();
}

void TMainWnd::DockControlBarInline(CControlBar* pCb, CControlBar* pCbTag)
{
	RecalcLayout(TRUE);

	RECT rc;
	pCbTag->GetWindowRect(&rc);

	UINT u=0;
	DWORD dw=pCbTag->GetBarStyle();

	if(dw&CBRS_ALIGN_TOP)
	{
		u=AFX_IDW_DOCKBAR_TOP;
		OffsetRect(&rc, 1, 0);
	}
	else if(dw&CBRS_ALIGN_BOTTOM)
	{
		u=AFX_IDW_DOCKBAR_BOTTOM;
		OffsetRect(&rc, 1, 0);
	}
	else if(dw&CBRS_ALIGN_LEFT)
	{
		u=AFX_IDW_DOCKBAR_LEFT;
		OffsetRect(&rc, 0, 1);
	}
	else if(dw&CBRS_ALIGN_RIGHT)
	{
		u=AFX_IDW_DOCKBAR_RIGHT;
		OffsetRect(&rc, 0, 1);
	}

	DockControlBar(pCb, u, &rc);
}

//void TMainWnd::LoadBarState()
//{
//	CString str;
//	CControlBar* pCb;
//	CWinApp* pApp=AfxGetApp();
//	POSITION pos=m_listControlBars.GetHeadPosition();
//	while(pos)
//	{
//		pCb=(CControlBar*)m_listControlBars.GetNext(pos);
//		str.Format(_T("ctrl\\bar%d"), pCb->GetDlgCtrlID());
//
//		if(pApp->GetProfileInt(str, _T("Floating"), 0))
//		{
//			FloatControlBar(pCb, CPoint(pApp->GetProfileInt(str, _T("FloatX"), 0), pApp->GetProfileInt(str, _T("FloatY"), 0)), pApp->GetProfileInt(str, _T("FloatStyle"), pCb->GetBarStyle()));
//		}
//		else
//		{
//			UINT n=0;
//			DWORD dw=pApp->GetProfileInt(str, _T("DockBarID"), 0);
//			n=(dw&CBRS_ALIGN_TOP)?AFX_IDW_DOCKBAR_TOP:n;
//			n=(dw&CBRS_ALIGN_BOTTOM && n==0)?AFX_IDW_DOCKBAR_BOTTOM:n;
//			n=(dw&CBRS_ALIGN_LEFT && n==0)?AFX_IDW_DOCKBAR_LEFT:n;
//			n=(dw&CBRS_ALIGN_RIGHT && n==0)?AFX_IDW_DOCKBAR_RIGHT:n;
//
//			RECT rc;
//			SetRect(&rc
//				, pApp->GetProfileInt(str, _T("DockLeft"), 0)
//				, pApp->GetProfileInt(str, _T("DockTop"), 0)
//				, pApp->GetProfileInt(str, _T("DockRight"), 0)
//				, pApp->GetProfileInt(str, _T("DockBottom"), 0));
//			DockControlBar(pCb, n, &rc);
//		}
//	}
//}

//void TMainWnd::SaveBarState()
//{
//	CString str;
//	CControlBar* pCb;
//	CWinApp* pApp=AfxGetApp();
//	POSITION pos=m_listControlBars.GetHeadPosition();
//	while(pos)
//	{
//		pCb=(CControlBar*)m_listControlBars.GetNext(pos);
//		str.Format(_T("ctrl\\bar%d"), pCb->GetDlgCtrlID());
//
//		pApp->WriteProfileInt(str, _T("Floating"), pCb->IsFloating());
//		pApp->WriteProfileInt(str, _T("FloatX"), 100);
//		pApp->WriteProfileInt(str, _T("FloatY"), 100);
//		pApp->WriteProfileInt(str, _T("FloatStyle"), pCb->GetBarStyle());
//		pApp->WriteProfileInt(str, _T("DockBarID"), pCb->GetBarStyle());
//		pApp->WriteProfileInt(str, _T("DockLeft"), 10);
//		pApp->WriteProfileInt(str, _T("DockTop"), 10);
//		pApp->WriteProfileInt(str, _T("DockRight"), 100);
//		pApp->WriteProfileInt(str, _T("DockBottom"), 20);
//	}
//}

//{menu
void TMainWnd::SetMenuStyle(BOOL bMenuStyle)
{
	if(bMenuStyle)
	{
		if(!IsMenuStyle())
		{
			m_tMenu=MenuOD_Make();
			MenuOD_SetStyle(m_tMenu, AfxGetApp()->GetProfileInt(_T("Settings"), _T("MenuStyle"), TMENU_STYLE_OFFICEXP));
			MenuOD_SetOwnerDraw(::GetMenu(m_hWnd), TRUE, TRUE);
		}
	}
	else
	{
		if(IsMenuStyle())
		{
			MenuOD_Kill(m_tMenu);
			m_tMenu=NULL;
			MenuOD_SetOwnerDraw(::GetMenu(m_hWnd), FALSE, TRUE);
		}
	}
}

void TMainWnd::AfterLoadMenu(HMENU hMenu)
{
}

void TMainWnd::AddMenuStylesToMenu(HMENU hMenu)
{
}

BOOL TMainWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	AfterLoadMenu(cs.hMenu);

	if(IsMenuStyle())
	{
		AddMenuStylesToMenu(cs.hMenu);
		MenuOD_SetOwnerDraw(cs.hMenu, TRUE, TRUE);
	}

	return TRUE;
}

void TMainWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWnd::OnSettingChange(uFlags, lpszSection);

	if(IsMenuStyle())
	{
		MenuOD_SettingChange(m_tMenu, uFlags, lpszSection);
		DrawMenuBar();
	}
}

void TMainWnd::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(lpMeasureItemStruct->CtlType==ODT_MENU)
		MenuOD_Measure(m_tMenu, lpMeasureItemStruct);

	CFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void TMainWnd::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->CtlType==ODT_MENU)
		MenuOD_Draw(m_tMenu, lpDrawItemStruct);

	CFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void TMainWnd::OnViewMenuStyle(UINT uID)
{
	int nMenuStyleNew=uID-ID_MENU_STYLE_1;
	if(!MenuOD_SetStyle(m_tMenu, nMenuStyleNew))
		return;

	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("MenuStyle"), nMenuStyleNew);
	SendMessage(UM_UPDATE_UI, UPDATE_UI_MENU);
}

void TMainWnd::OnUpdateViewMenuStyle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID-ID_MENU_STYLE_1==MenuOD_GetStyle(m_tMenu));
}

LRESULT TMainWnd::OnUmUpdateUI(WPARAM wp, LPARAM lp)
{
	if(wp!=UPDATE_UI_LANG && wp!=UPDATE_UI_MENU)
		return 0;

	HMENU hMenu=LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(128));
	if(!hMenu)
		return 0;

	AfterLoadMenu(hMenu);
	AddMenuStylesToMenu(hMenu);
	MenuOD_SetOwnerDraw(hMenu, TRUE, TRUE);

	CMenu menu;
	menu.Attach(hMenu);

	SetMenu(NULL);
	DestroyMenu(m_hMenuDefault);

	SetMenu(&menu);
	m_hMenuDefault=hMenu;

	menu.Detach();

	CWnd* pWnd=GetWindow(GW_CHILD);
	while(pWnd)
	{
		pWnd->SendMessage(UM_UPDATE_UI, wp, lp);
		pWnd=pWnd->GetNextWindow();
	}

	return 0;
}
//menu}

TOLNS_END
