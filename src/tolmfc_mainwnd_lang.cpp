
#include "stdafx.h"
#include <tolmfc_mainwnd_lang.h>
#include <tolmfc_winapp_lang.h>

TOLNS_BEGIN


IMPLEMENT_DYNCREATE(TMainWndLang, TMainWnd)

BEGIN_MESSAGE_MAP(TMainWndLang, TMainWnd)
END_MESSAGE_MAP()


TMainWndLang::TMainWndLang()
{
}

TMainWndLang::~TMainWndLang()
{
}

void TMainWndLang::AfterLoadMenu(HMENU hMenu)
{
	((TWinAppLang*)AfxGetApp())->AddLangMenu(hMenu);
}

LPCTSTR TMainWndLang::GetIconWndClass(DWORD dwDefaultStyle, UINT nIDResource)
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

BOOL TMainWndLang::LoadAccelTable(LPCTSTR lpszResourceName)
{
	m_hAccelTable=::LoadAccelerators(AfxGetInstanceHandle(), lpszResourceName);
	return (m_hAccelTable!=NULL);
}

BOOL TMainWndLang::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
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

TOLNS_END
