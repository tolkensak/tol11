
#include "stdafx.h"
#include <tolmfc_SDIMV_MainWnd_Lang.h>
#include <tolmfc_SDIMV_WinApp_Lang.h>

TOLNS_BEGIN


IMPLEMENT_DYNCREATE(TSDIMVMainWndLang, TSDIMVMainWnd)


BEGIN_MESSAGE_MAP(TSDIMVMainWndLang, TSDIMVMainWnd)
	ON_MESSAGE(UM_UPDATE_UI, &TSDIMVMainWndLang::OnUmUpdateUI)
END_MESSAGE_MAP()


TSDIMVMainWndLang::TSDIMVMainWndLang()
{
}

TSDIMVMainWndLang::~TSDIMVMainWndLang()
{
}

void TSDIMVMainWndLang::AfterLoadMenu(HMENU hMenu)
{
	((TSDIMVWinAppLang*)AfxGetApp())->AddLangMenu(hMenu);
}

LPCTSTR TSDIMVMainWndLang::GetIconWndClass(DWORD dwDefaultStyle, UINT nIDResource)
{
	HICON hIcon=::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDResource));
	if(hIcon!=NULL)
	{
		CREATESTRUCT cs;
		memset(&cs, 0, sizeof(CREATESTRUCT));
		cs.style=dwDefaultStyle;
		PreCreateWindow(cs);

		// will fill lpszClassName with default WNDCLASS name ignore instance handle from PreCreateWindow.
		WNDCLASS wndcls;
		if(cs.lpszClass!=NULL
			&& GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls)
			&& wndcls.hIcon!=hIcon)
		{
			// register a very similar WNDCLASS
			return AfxRegisterWndClass(wndcls.style, wndcls.hCursor, wndcls.hbrBackground, hIcon);
		}
	}

	return NULL; // just use the default
}

BOOL TSDIMVMainWndLang::LoadAccelTable(LPCTSTR lpszResourceName)
{
	m_hAccelTable=::LoadAccelerators(AfxGetInstanceHandle(), lpszResourceName);
	return (m_hAccelTable!=NULL);
}

BOOL TSDIMVMainWndLang::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	m_nIDHelp=nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	CString strFullString;
	if(strFullString.LoadString(nIDResource))
		AfxExtractSubString(m_strTitle, strFullString, 0);    // first sub-string

	//VERIFY(AfxDeferRegisterClass(AFX_WNDFRAMEORVIEW_REG));

	// attempt to create the window
	LPCTSTR lpszClass=GetIconWndClass(dwDefaultStyle, nIDResource);
	CString strTitle=m_strTitle;
	if(!Create(lpszClass, strTitle, dwDefaultStyle, rectDefault, pParentWnd, MAKEINTRESOURCE(nIDResource), 0L, pContext))
	{
		return FALSE;   // will self destruct on failure normally
	}

	// save the default menu handle
	ASSERT(m_hWnd!=NULL);

#if _MSC_VER < 1400
	m_hMenuDefault=::GetMenu(m_hWnd);
#else
	m_hMenuDefault=m_dwMenuBarState==AFX_MBS_VISIBLE?::GetMenu(m_hWnd):m_hMenu;
#endif


	// load accelerator resource
	LoadAccelTable(MAKEINTRESOURCE(nIDResource));

	if(pContext==NULL)   // send initial update
		SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

	return TRUE;
}

LRESULT TSDIMVMainWndLang::OnUmUpdateUI(WPARAM wp, LPARAM lp)
{
	if(wp!=UPDATE_UI_LANG && wp!=UPDATE_UI_MENU)
		return 0;

	TSDIMVWinAppLang* pApp=(TSDIMVWinAppLang*)AfxGetApp();
	TSDIMVDocTemplate* pDocTemplate;

	POSITION pos=pApp->GetFirstDocTemplatePosition();
	while(pos)
	{
		pDocTemplate=(TSDIMVDocTemplate*)pApp->GetNextDocTemplate(pos);
		pDocTemplate->LoadDocString();
	}


	pDocTemplate=pApp->GetTemplate(GetActiveView());

	HMENU hMenu=LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(pDocTemplate->GetResourceID()));
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

	// change the title of the document
	pDocTemplate->GetDocString(m_strTitle, CDocTemplate::windowTitle);
	OnUpdateFrameTitle(TRUE);

	CWnd* pWnd=GetWindow(GW_CHILD);
	while(pWnd)
	{
		pWnd->SendMessage(UM_UPDATE_UI, wp, lp);
		pWnd=pWnd->GetNextWindow();
	}

	return 0;
}


TOLNS_END
