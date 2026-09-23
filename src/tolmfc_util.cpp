
#include "stdafx.h"
#include <tolmfc_util.h>
#include <stdio.h> /*MsgBox*/
#include <stdarg.h> /*MsgBox*/

TOLNS_BEGIN


//TOLMFCDECL int TOLAPI MsgBox(HWND hWnd, LPCTSTR pcText, UINT uType)
//{
//	if(!pcText)
//		return 0;
//
//	TCHAR pchTitle[TOL_MAXSTR];
//
//	if(hWnd)
//		GetWindowText(hWnd, pchTitle, TOL_MAXSTR);
//	else
//		lstrcpy(pchTitle, _T("Message"));
//
//	return MessageBox(hWnd, pcText, pchTitle, uType);
//}

//TOLMFCDECL int TOLAPI MsgBox(HWND hWnd, UINT uTextID, LPCTSTR pcSafe, UINT uType)
//{
//	TCHAR pchText[TOL_MAXSTR];
//
//	if(!LoadString(NULL, uTextID, pchText, TOL_MAXPATH))
//	{
//		if(pcSafe)
//			lstrcpy(pchText, pcSafe);
//		else
//			pchText[0]=_T('\0');
//	}
//
//	return MsgBox(hWnd, pchText, uType);
//}

#ifdef TOLMFCDLL
#pragma data_seg("Shared")
#endif
UINT g_uMsgBoxStyle=0;
HHOOK g_hMsgBoxHook=NULL;
HINSTANCE g_hMsgBoxLangInst=NULL;
WNDPROC g_fnMsgBoxOldWndProc=NULL;
#ifdef TOLMFCDLL
#pragma data_seg()
#pragma comment(linker, "/section:Shared,RWS")
#endif


BOOL CALLBACK MsgBox_EnumChild(HWND hwnd, LPARAM lParam)
{
	UINT uCtrlID=GetDlgCtrlID(hwnd);

	if(uCtrlID==2 && (g_uMsgBoxStyle&MB_TYPEMASK)==MB_OK)
		uCtrlID=1;

	CString str;
	if(str.LoadString(g_hMsgBoxLangInst, uCtrlID))
		SetWindowText(hwnd, str);

	//TCHAR pch[32];
	//if(LoadString(g_hMsgBoxLangInst, GetDlgCtrlID(hwnd), pch, 31))
	//	SetWindowText(hwnd, pch);

	return TRUE;
}

LRESULT CALLBACK MsgBox_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRet=CallWindowProc(g_fnMsgBoxOldWndProc, hWnd, uMsg, wParam, lParam);

	if(uMsg==WM_INITDIALOG)
	{
		EnumChildWindows(hWnd, MsgBox_EnumChild, NULL);
	}
	else if(uMsg==WM_NCDESTROY)
	{
		UnhookWindowsHookEx(g_hMsgBoxHook);
		g_fnMsgBoxOldWndProc=NULL;
		g_hMsgBoxLangInst=NULL;
		g_hMsgBoxHook=NULL;
		g_uMsgBoxStyle=0;
	}

	return lRet;
}

LRESULT CALLBACK MsgBox_SetWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode==HC_ACTION)
	{
		CWPSTRUCT* pwp=(CWPSTRUCT*)lParam;
		if(pwp->message==WM_INITDIALOG)
			g_fnMsgBoxOldWndProc=(WNDPROC)SetWindowLongPtr(pwp->hwnd, -4, (LONG_PTR)MsgBox_WndProc); // -4=GWL_WNDPROC
	}

	return CallNextHookEx(g_hMsgBoxHook, nCode, wParam, lParam);
}

TOLMFCDECL int TOLAPI MsgBox(CWnd* pParent, UINT uStyle, UINT uTextID, ...)
{
	CString str;
	if(!str.LoadString(uTextID))
		return 0;

	CString strText;
	va_list ap;
	va_start(ap, uTextID);
	strText.FormatV(str, ap);
	va_end(ap);

	CString strCaption;
	if(!strCaption.LoadString(AFX_IDS_APP_TITLE))
		strCaption=_T("Message");

	if(!pParent)
		pParent=AfxGetMainWnd();

	g_uMsgBoxStyle=uStyle;
	g_hMsgBoxLangInst=AfxGetResourceHandle();
	g_hMsgBoxHook=SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)MsgBox_SetWndProc, NULL, GetCurrentThreadId());

	return pParent->MessageBox(strText, strCaption, uStyle);
}

TOLMFCDECL int TOLAPI MsgBox(CWnd* pParent, UINT uStyle, LPCTSTR pcText, ...)
{
	if(!pcText)
		return 0;

	CString strText;
	va_list ap;
	va_start(ap, pcText);
	strText.FormatV(pcText, ap);
	va_end(ap);

	CString strCaption;
	if(!strCaption.LoadString(AFX_IDS_APP_TITLE))
		strCaption=_T("Message");

	if(!pParent)
		pParent=AfxGetMainWnd();

	g_uMsgBoxStyle=uStyle;
	g_hMsgBoxLangInst=AfxGetResourceHandle();
	g_hMsgBoxHook=SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)MsgBox_SetWndProc, NULL, GetCurrentThreadId());

	return pParent->MessageBox(strText, strCaption, uStyle);
}


TOLNS_END
