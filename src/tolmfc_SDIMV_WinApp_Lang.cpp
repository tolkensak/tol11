
#include "stdafx.h"
#include <tolmfc_SDIMV_WinApp_Lang.h>

TOLNS_BEGIN


IMPLEMENT_DYNAMIC(TSDIMVWinAppLang, TSDIMVWinApp)

BEGIN_MESSAGE_MAP(TSDIMVWinAppLang, TSDIMVWinApp)
	ON_COMMAND_RANGE(ID_LANG_1, ID_LANG_N, &TSDIMVWinAppLang::OnLang)
	ON_UPDATE_COMMAND_UI_RANGE(ID_LANG_1, ID_LANG_N, &TSDIMVWinAppLang::OnUpdateLang)
END_MESSAGE_MAP()

TSDIMVWinAppLang::TSDIMVWinAppLang()
	: m_tLang(NULL)
	, m_lid(0)
{
}

TSDIMVWinAppLang::~TSDIMVWinAppLang()
{
	Lang_Kill(m_tLang);
}

BOOL TSDIMVWinAppLang::InitInstance()
{
	if(!TSDIMVWinApp::InitInstance())
		return FALSE;

	m_tLang=Lang_Make(TLANG_NAME_FORMAT_FULL);
	if(!m_tLang)
		return FALSE;

	Char pch[TOL_MAXPATH];
	GetModuleFileName(NULL, pch, TOL_MAXPATH);
	Lang_Init(m_tLang, pch);

	SetRegistryKey();

	if(!SetLangInstance(GetProfileInt(_T("Settings"), _T("Lang"), 0))
		&& !SetLangInstance(Lang_GetUserDefaultLangID()))
			m_lid=Lang_Id(m_tLang, 0);

	return TRUE;
}

void TSDIMVWinAppLang::AddLangMenu(HMENU hMenu)
{
}

Bool TSDIMVWinAppLang::SetLangInstance(LANGID lidNew)
{
	if(lidNew==m_lid)
		return FALSE;

	T_LANGFIND lf;
	lf.uMask=TLANG_MASK_LANGID;
	lf.lid=lidNew;

	int i=Lang_Find(m_tLang, &lf);
	if(i==-1)
		return FALSE;

	Char pchPath[TOL_MAXPATH];
	Lang_Path(m_tLang, i, pchPath, TOL_MAXPATH);

	HINSTANCE hInstanceNew;
	if(pchPath[0]==_T('\0'))
		hInstanceNew=m_hInstance;
	else
	{
		hInstanceNew=LoadLibrary(pchPath);
		if(!hInstanceNew)
			return FALSE;
	}

	HINSTANCE hInstanceOld=AfxGetResourceHandle();
	AfxSetResourceHandle(hInstanceNew);

	if(hInstanceOld!=m_hInstance)
		FreeLibrary(hInstanceOld);

	m_lid=lidNew;
	return TRUE;
}

void TSDIMVWinAppLang::OnLang(UINT uID)
{
	if(!SetLangInstance(Lang_Id(m_tLang, uID-ID_LANG_1)))
		return;

	WriteProfileInt(_T("Settings"), _T("Lang"), m_lid);

	if(m_pMainWnd)
		m_pMainWnd->SendMessage(UM_UPDATE_UI, UPDATE_UI_LANG);
}

void TSDIMVWinAppLang::OnUpdateLang(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_lid==Lang_Id(m_tLang, pCmdUI->m_nID-ID_LANG_1));
}


TOLNS_END
