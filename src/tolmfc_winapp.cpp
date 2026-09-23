
#include "stdafx.h"
#include <tolmfc_winapp.h>
#include <tolc_version.h> /*SetRegistryKey*/
#include <tolc_menu.h> /*OnUpdateRecentFileMenu*/
#include <afxadv.h> /*OnUpdateRecentFileMenu*/
#include <tolmfc_aboutdlg.h>

TOLNS_BEGIN


IMPLEMENT_DYNAMIC(TWinApp, CWinApp)

BEGIN_MESSAGE_MAP(TWinApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &TWinApp::OnAppAbout)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, &TWinApp::OnUpdateRecentFileMenu)
END_MESSAGE_MAP()


TWinApp::TWinApp()
	: m_uStartedTimes(0)
{
}

TWinApp::~TWinApp()
{
}

void TWinApp::SetRegistryKey(LPCTSTR lpszRegistryKey)
{
	SetRegistryKey();
}

void TWinApp::SetRegistryKey(UINT nIDRegistryKey)
{
	TCHAR pchPath[MAX_PATH];
	GetModuleFileName(m_hInstance, pchPath, MAX_PATH);

	TCHAR pch[MAX_PATH];
	if(Version_GetSectionData(pchPath, _T("CompanyName"), pch)!=VERSION_SUCCESS)
		lstrcpy(pch, _T("Tolken"));

	CWinApp::SetRegistryKey(pch);


	if(Version_GetSectionData(pchPath, _T("ProductName"), pch)!=VERSION_SUCCESS)
		lstrcpy(pch, _T("Unknown"));

	TCHAR pch2[MAX_PATH];
	if(Version_GetSectionData(pchPath, _T("ProductVersion"), pch2)!=VERSION_SUCCESS)
		lstrcpy(pch2, _T("X.X"));


	lstrcat(pch, _T("\\"));
	lstrcat(pch, pch2);

	free((LPVOID)m_pszProfileName);
	m_pszProfileName=_tcsdup(pch);

	if(m_uStartedTimes)
	{
		m_uStartedTimes=GetProfileInt(_T(""), _T("StartedTimes"), 0)+1;
		WriteProfileInt(_T(""), _T("StartedTimes"), m_uStartedTimes);
	}
}

void TWinApp::OnAppAbout()
{
	TAboutDlg dlg;
	dlg.DoModal();
}

void TWinApp::OnUpdateRecentFileMenu(CCmdUI *pCmdUI)
{
	ASSERT_VALID(this);

	if(!m_pRecentFileList) // no MRU files
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	ASSERT(m_pRecentFileList->m_arrNames);

	CMenu* pMenu=pCmdUI->m_pMenu;
	if(m_pRecentFileList->m_strOriginal.IsEmpty() && pMenu)
		pMenu->GetMenuString(pCmdUI->m_nID, m_pRecentFileList->m_strOriginal, MF_BYCOMMAND);


	if(m_pRecentFileList->m_arrNames[0].IsEmpty()) // no MRU files
	{
		if(!m_pRecentFileList->m_strOriginal.IsEmpty())
			pCmdUI->SetText(m_pRecentFileList->m_strOriginal);

		pCmdUI->Enable(FALSE);
		return;
	}

	if(!pCmdUI->m_pMenu)
		return;


	HMENU hParentMenu=Menu_GetParent(pCmdUI->m_pMenu->m_hMenu, ID_FILE_MRU_FILE1, TRUE);
	if(!hParentMenu)
		hParentMenu=pCmdUI->m_pMenu->m_hMenu;


	int i;
	for(i=0; i<m_pRecentFileList->m_nSize; i++)
		DeleteMenu(hParentMenu, pCmdUI->m_nID+i, MF_BYCOMMAND);

	TCHAR pchCurDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, pchCurDir);
	int nCurDir=lstrlen(pchCurDir);
	ASSERT(nCurDir>=0);
	pchCurDir[nCurDir]=_T('\\');
	pchCurDir[++nCurDir] = _T('\0');

	CString strName;
	CString strTemp;
	for(i=0; i<m_pRecentFileList->m_nSize; i++)
	{
		if(!m_pRecentFileList->GetDisplayName(strName, i, pchCurDir, nCurDir))
			break;

		// double up any '&' characters so they are not underlined
		LPCTSTR lpszSrc=strName;
		LPTSTR lpszDest=strTemp.GetBuffer(strName.GetLength()*2);

		while(*lpszSrc)
		{
			if(*lpszSrc=='&')
				*lpszDest++ ='&';

			if(_istlead(*lpszSrc))
				*lpszDest++ =*lpszSrc++;

			*lpszDest++ =*lpszSrc++;
		}

		*lpszDest=0;
		strTemp.ReleaseBuffer();

		// insert mnemonic + the file name
		TCHAR buf[10];
		int nItem=(i+1+m_pRecentFileList->m_nStart)%_AFX_MRU_MAX_COUNT;

		// number &1 thru &9, then 1&0, then 11 thru ...
		if(nItem>10)
			wsprintf(buf, _T("%d "), nItem);
		else if(nItem==10)
			lstrcpy(buf, _T("1&0 "));
		else
			wsprintf(buf, _T(" &%d "), nItem);

		InsertMenu(hParentMenu, pCmdUI->m_nIndex, MF_STRING|MF_BYPOSITION, pCmdUI->m_nID, CString(buf)+strTemp);

		//if(IsMenuOwnerDraw())
		//	ModifyMenu(hParentMenu, pCmdUI->m_nIndex, MF_OWNERDRAW|MF_BYPOSITION, pCmdUI->m_nID, (LPCWSTR)TMENU_MAKEDATA(FALSE, hParentMenu));

		pCmdUI->m_nIndex++;
		pCmdUI->m_nID++;
	}

	// update end menu count
	pCmdUI->m_nIndex--; // point to last menu added
	pCmdUI->m_nIndexMax=pCmdUI->m_pMenu->GetMenuItemCount();
	pCmdUI->m_bEnableChanged=TRUE;    // all the added items are enabled
}


TOLNS_END
