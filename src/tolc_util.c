
#include "stdafx.h"
#include <tolc_util.h>
#include <tolc_version.h>
#include <tolc_string.h>

#include <tlhelp32.h> /* process */
#include <commctrl.h> /* imagelist, toolbar, hotkey */

#include <shlwapi.h> /* Path... */
#pragma comment(lib, "shlwapi.lib")


TOLEXTC_BEGIN


/*********************************************************************************************************
 application
************************/

TOLCDECL BOOL TOLAPI App_Alone(LPCTSTR pcWndClass, LPCTSTR pcWndTitle)
{
	HWND hWnd=FindWindow(pcWndClass, pcWndTitle);
	if(hWnd)
	{
		Wnd_SetForeground(hWnd);
		return FALSE;
	}

	return TRUE;
}

TOLCDECL WORD TOLAPI App_SoleStart(LPCTSTR pcUnique)
{
	if(GlobalFindAtom(pcUnique))
		return 0;

	return (WORD)GlobalAddAtom(pcUnique);
}

TOLCDECL void TOLAPI App_SoleStop(WORD w)
{
	if(w)
		GlobalDeleteAtom((ATOM)w);
}

TOLCDECL int TOLAPI App_RootRegKey(HINSTANCE hInstance, PChar pcBuffer, int nBufferLen)
{
	int n;
	Char pchPath[TOL_MAXPATH];
	Char pchCompany[TOL_MAXSTR];
	Char pchProduct[TOL_MAXSTR];
	Char pchVersion[TOL_MAXSTR];
	PCChar pcSoftware=_T("Software");

	GetModuleFileName(hInstance, pchPath, TOL_MAXPATH);

	if(Version_GetSectionData(pchPath, _T("CompanyName"), pchCompany)!=VERSION_SUCCESS)
		Str_Copy(pchCompany, _T("Unknown Company"));

	if(Version_GetSectionData(pchPath, _T("ProductName"), pchProduct)!=VERSION_SUCCESS)
		Str_Copy(pchProduct, _T("Unknown Product"));

	if(Version_GetSectionData(pchPath, _T("ProductVersion"), pchVersion)!=VERSION_SUCCESS)
		Str_Copy(pchVersion, _T("Unknown Version"));

	n=Str_Format(NULL, _T("%s\\%s\\%s\\%s"), pcSoftware, pchCompany, pchProduct, pchVersion);

	if(pcBuffer && (n+1)<=nBufferLen)
		Str_Format(pcBuffer, _T("%s\\%s\\%s\\%s"), pcSoftware, pchCompany, pchProduct, pchVersion);

	return n;
}


/*********************************************************************************************************
 process
********************/

BOOL CALLBACK EnumDesktopWindowsProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwPID;
	GetWindowThreadProcessId(hWnd, &dwPID);

	if(dwPID==(DWORD)lParam)
	{
		Wnd_SetForeground(hWnd);
		return FALSE;
	}

	return TRUE;
}

TOLCDECL DWORD TOLAPI Process_FindAnother(DWORD dwPID)
{
	HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap!=INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32={0};
		pe32.dwSize=sizeof(PROCESSENTRY32);

		if(Process32First(hProcessSnap, &pe32))
		{
			do{
				if(pe32.th32ProcessID==dwPID)
				{
					TCHAR pch[TOL_MAXPATH];
					lstrcpy(pch, pe32.szExeFile);

					if(Process32First(hProcessSnap, &pe32))
					{
						do{
							if(lstrcmpi(pe32.szExeFile, pch)==0 && pe32.th32ProcessID!=dwPID)
							{
								EnumDesktopWindows(NULL, EnumDesktopWindowsProc, pe32.th32ProcessID);
								CloseHandle(hProcessSnap);
								return pe32.th32ProcessID;
							}
						}while(Process32Next(hProcessSnap, &pe32));
					}

					break;
				}
			}while(Process32Next(hProcessSnap, &pe32));
		}

		CloseHandle(hProcessSnap);
	}

	return 0;
}


/*********************************************************************************************************
 hotkey
********************/

#define HOTKEY_NAMELEN 128

TOLCDECL DWORD TOLAPI HotKey_CtlToSys(WORD wHotKey)
{
	WORD wModif=0;
	BYTE byModif=HIBYTE(wHotKey);

	if(byModif&HOTKEYF_ALT)
		wModif|=MOD_ALT;

	if(byModif&HOTKEYF_CONTROL)
		wModif|=MOD_CONTROL;

	if(byModif&HOTKEYF_SHIFT)
		wModif|=MOD_SHIFT;

	if(byModif&HOTKEYF_EXT)
		wModif|=MOD_WIN;

	return MAKELONG(LOBYTE(wHotKey), wModif);
}

TOLCDECL int TOLAPI HotKey_CtlToText(WORD wHotKey, LPTSTR pcBuff, int nBuffLen)
{
	if(wHotKey)
	{
		int len=0;
		BYTE byModif=HIBYTE(wHotKey);

		TCHAR pcCtrl[HOTKEY_NAMELEN];
		TCHAR pcAlt[HOTKEY_NAMELEN];
		TCHAR pcShift[HOTKEY_NAMELEN];
		TCHAR pcWin[HOTKEY_NAMELEN];
		TCHAR pcKey[HOTKEY_NAMELEN];

		if(byModif&HOTKEYF_CONTROL)
		{
			if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(VK_CONTROL, 0)), pcCtrl, HOTKEY_NAMELEN))
				return 0;

			len+=1+lstrlen(pcCtrl);
		}

		if(byModif&HOTKEYF_ALT)
		{
			if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(VK_MENU, 0)), pcAlt, HOTKEY_NAMELEN))
				return 0;

			len+=1+lstrlen(pcAlt);
		}

		if(byModif&HOTKEYF_SHIFT)
		{
			if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(VK_SHIFT, 0)), pcShift, HOTKEY_NAMELEN))
				return 0;

			len+=1+lstrlen(pcShift);
		}

		if(byModif&HOTKEYF_EXT)
		{
			if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(VK_LWIN, 0)), pcWin, HOTKEY_NAMELEN))
				return 0;

			len+=1+lstrlen(pcWin);
		}

		if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(LOBYTE(wHotKey), 0)), pcKey, HOTKEY_NAMELEN))
			return 0;

		len+=lstrlen(pcKey);

		if(!pcBuff || nBuffLen<len)
			return -len;

		*pcBuff=_T('\0');

		if(byModif&HOTKEYF_CONTROL)
			pcBuff+=wsprintf(pcBuff, _T("%s+"), pcCtrl);

		if(byModif&HOTKEYF_ALT)
			pcBuff+=wsprintf(pcBuff, _T("%s+"), pcAlt);

		if(byModif&HOTKEYF_SHIFT)
			pcBuff+=wsprintf(pcBuff, _T("%s+"), pcShift);

		if(byModif&HOTKEYF_EXT)
			pcBuff+=wsprintf(pcBuff, _T("%s+"), pcWin);

		lstrcat(pcBuff, pcKey);

		return len;
	}
	else if(pcBuff)
		*pcBuff=_T('\0');

	return 0;
}

TOLCDECL WORD TOLAPI HotKey_SysToCtl(DWORD dwHotKey)
{
	BYTE byModif=0;
	WORD wModif=HIWORD(dwHotKey);

	if(wModif&MOD_ALT)
		byModif|=HOTKEYF_ALT;

	if(wModif&MOD_CONTROL)
		byModif|=HOTKEYF_CONTROL;

	if(wModif&MOD_SHIFT)
		byModif|=HOTKEYF_SHIFT;

	if(wModif&MOD_WIN)
		byModif|=HOTKEYF_EXT;

	return MAKEWORD(LOWORD(dwHotKey), byModif);
}

TOLCDECL int TOLAPI HotKey_SysToText(DWORD dwHotKey, LPTSTR pcBuff, int nBuffLen)
{
	if(dwHotKey)
	{
		int len=0;
		WORD wModif=HIWORD(dwHotKey);

		TCHAR pcCtrl[HOTKEY_NAMELEN];
		TCHAR pcAlt[HOTKEY_NAMELEN];
		TCHAR pcShift[HOTKEY_NAMELEN];
		TCHAR pcWin[HOTKEY_NAMELEN];
		TCHAR pcKey[HOTKEY_NAMELEN];

		if(wModif&MOD_CONTROL)
		{
			if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(VK_CONTROL, 0)), pcCtrl, HOTKEY_NAMELEN))
				return 0;

			len+=1+lstrlen(pcCtrl);
		}

		if(wModif&MOD_ALT)
		{
			if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(VK_MENU, 0)), pcAlt, HOTKEY_NAMELEN))
				return 0;

			len+=1+lstrlen(pcAlt);
		}

		if(wModif&MOD_SHIFT)
		{
			if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(VK_SHIFT, 0)), pcShift, HOTKEY_NAMELEN))
				return 0;

			len+=1+lstrlen(pcShift);
		}

		if(wModif&MOD_WIN)
		{
			if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(VK_LWIN, 0)), pcWin, HOTKEY_NAMELEN))
				return 0;

			len+=1+lstrlen(pcWin);
		}

		if(!GetKeyNameText(MAKELONG(0, MapVirtualKey(LOWORD(dwHotKey), 0)), pcKey, HOTKEY_NAMELEN))
			return 0;

		len+=lstrlen(pcKey);

		if(!pcBuff || nBuffLen<len)
			return -len;

		*pcBuff=_T('\0');

		if(wModif&MOD_CONTROL)
			pcBuff+=wsprintf(pcBuff, _T("%s+"), pcCtrl);

		if(wModif&MOD_ALT)
			pcBuff+=wsprintf(pcBuff, _T("%s+"), pcAlt);

		if(wModif&MOD_SHIFT)
			pcBuff+=wsprintf(pcBuff, _T("%s+"), pcShift);

		if(wModif&MOD_WIN)
			pcBuff+=wsprintf(pcBuff, _T("%s+"), pcWin);

		lstrcat(pcBuff, pcKey);

		return len;
	}
	else if(pcBuff)
		*pcBuff=_T('\0');

	return 0;
}


/*********************************************************************************************************
 rectangle
***********************/

TOLCDECL void TOLAPI Rect_GetEdge(LPRECT prcEdge, LPCRECT prcOuter, LPCRECT prcInner)
{
	prcEdge->left=prcInner->left-prcOuter->left;
	prcEdge->top=prcInner->top-prcOuter->top;
	prcEdge->right=prcOuter->right-prcInner->right;
	prcEdge->bottom=prcOuter->bottom-prcInner->bottom;
}

TOLCDECL void TOLAPI Rect_Size(LPRECT prc, int left, int top, int right, int bottom)
{
	prc->left-=left;
	prc->top-=top;
	prc->right+=right;
	prc->bottom+=bottom;
}

TOLCDECL void TOLAPI Rect_ClientToScreen(HWND hWnd, LPRECT lprc)
{
	POINT pt;

	pt.y=lprc->top;
	pt.x=lprc->left;
	ClientToScreen(hWnd, &pt);
	lprc->top=pt.y;
	lprc->left=pt.x;

	pt.y=lprc->bottom;
	pt.x=lprc->right;
	ClientToScreen(hWnd, &pt);
	lprc->bottom=pt.y;
	lprc->right=pt.x;
}

TOLCDECL void TOLAPI Rect_ScreenToClient(HWND hWnd, LPRECT lprc)
{
	POINT pt;

	pt.y=lprc->top;
	pt.x=lprc->left;
	ScreenToClient(hWnd, &pt);
	lprc->top=pt.y;
	lprc->left=pt.x;

	pt.y=lprc->bottom;
	pt.x=lprc->right;
	ScreenToClient(hWnd, &pt);
	lprc->bottom=pt.y;
	lprc->right=pt.x;
}


/*********************************************************************************************************
 resource
**********************/

TOLCDECL int TOLAPI Resource_MsgBox(HINSTANCE hInst, HWND hWnd, LPCTSTR pcTitle, UINT uType, UINT uID, LANGID lid, LPCTSTR pcSafe)
{
	TCHAR pc[TOL_MAXSTR];

	if(Resource_LoadStr(hInst, uID, pc, TOL_MAXSTR, lid)>0)
		pcSafe=pc;
	else if(!pcSafe)
		return 0;

	return MessageBox(hWnd, pcSafe, pcTitle, uType);
}

TOLCDECL LPVOID TOLAPI Resource_Load(HMODULE hModule, LPCTSTR pcType, LPCTSTR pcName, LANGID lid)
{
	HRSRC hRes;         // handle/ptr. to res. info. in hExe
	HRSRC hResLoad;     // handle to loaded resource
	LPVOID lpResLock;   // pointer to resource data


	if(!pcType || !pcName)
		return NULL;


	// Locate the dialog box resource in the .EXE file.
	hRes=FindResourceEx(hModule, pcType, pcName, lid);
	if(!hRes)
	{
		hRes=FindResourceEx(hModule, pcType, pcName, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	if(!hRes)
	{
		//ErrorHandler("Could not locate resource.");
		return NULL;
	}

	// Load the dialog box into global memory.
	hResLoad=LoadResource(hModule, hRes);
	if(!hResLoad)
	{
		//ErrorHandler("Could not load resource.");
		return NULL;
	}

	// Lock the dialog box into global memory.
	lpResLock=LockResource(hResLoad);
	if(!lpResLock)
	{
		//ErrorHandler("Could not lock resource.");
	}

	return lpResLock;
}

TOLCDECL void TOLAPI Resource_GetTooltip(PCChar pcOrig, PChar pcDest, int cch, BOOL bWithShortKey)
{
	if(pcOrig && pcDest)
	{
		PCChar pc=pcOrig;

		for(; *pc; pc++)
		{
			if(*pc==_T('\n'))
			{
				BOOL bEnclose=FALSE;

				for(pc++; *pc && cch>0; pc++, pcDest++, cch--)
				{
					if(*pc==_T('\n'))
					{
						if(bWithShortKey)
						{
							*pcDest++=_T(' ');
							*pcDest=_T('(');
							cch-=2;
							bEnclose=TRUE;
							continue;
						}
						else
							break;
					}
					else
						*pcDest=*pc;
				}

				if(bEnclose)
					*pcDest++=_T(')');

				break;
			}
		}

		*pcDest=_T('\0');
	}
}

TOLCDECL void TOLAPI Resource_GetShortkey(PCChar pcOrig, PChar pcDest, int cch)
{
	if(pcOrig && pcDest)
	{
		int i=0;
		PCChar pc=pcOrig;

		for(; *pc; pc++)
		{
			if(*pc==_T('\n'))
				i++;

			if(i==2)
			{
				for(pc++; *pc && cch>0; pc++, cch--)
					*pcDest++=*pc;

				break;
			}
		}

		*pcDest=_T('\0');
	}
}

TOLCDECL void TOLAPI Resource_GetInfo(PCChar pcOrig, PChar pcDest, int cch)
{
	if(pcOrig && pcDest)
	{
		PCChar pc=pcOrig;

		for(; *pc && cch>0; pc++, cch--)
		{
			if(*pc==_T('\n'))
				break;

			*pcDest++=*pc;
		}

		*pcDest=_T('\0');
	}
}

TOLCDECL int TOLAPI Resource_LoadStr(HMODULE hModule, UINT uID, PChar pcBuff, int cchBuff, LANGID lid)
{
	HRSRC hRes;
	PChar pc, pcRes;
	UINT idResBlk=uID/16+1;
	int i, cch=0;
	int nIndex=uID%16;


	// Locate the dialog box resource in the .EXE file.
	hRes=FindResourceEx(hModule, RT_STRING, MAKEINTRESOURCE(idResBlk), lid);
	if(!hRes)
	{
		hRes=FindResourceEx(hModule, RT_STRING, MAKEINTRESOURCE(idResBlk), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	if(!hRes)
	{
		//ErrorHandler("Could not locate resource.");
		return 0;
	}

	// Load the dialog box into global memory.
	pcRes=(PChar)LoadResource(hModule, hRes);
	if(!pcRes)
	{
		//ErrorHandler("Could not load resource.");
		return 0;
	}

	pc=pcRes;
	for(i=0; i<16; i++)
	{
		if(*pc)
		{
			cch=*pc;  // String size in characters.
			pc++;

			if(i==nIndex)
			{
				// The string has been found in the string table.
				if(pcBuff || cchBuff>cch)
				{
					lstrcpyn(pcBuff, pc, cch+1);
					return cch;
				}

				return -cch;
			}

			pc+=cch;
		}
		else
			pc++;
	}

	return 0;
}


/*********************************************************************************************************
 tooltip
************************/

TOLCDECL HWND TOLAPI Tooltip_Create(DWORD dwStyle, HWND hWndParent, HINSTANCE hInstance)
{
	/* CREATE A TOOLTIP WINDOW */
	HWND hWnd=CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
		WS_POPUP|dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hWndParent, NULL, hInstance, NULL);

	if(hWnd)
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

	return hWnd;
}


/*********************************************************************************************************
 window
**********************/

TOLCDECL BOOL TOLAPI Wnd_MoveToCenter(HWND hWnd, HWND hwndOwner)
{
	RECT rcChild, rcOwner;
	int  cxChild, cyChild, cxOwner, cyOwner;
	int  cxScreen, cyScreen, xNew, yNew;
	HDC  hDC;

	if(!hwndOwner || !IsWindowVisible(hwndOwner) || IsIconic(hwndOwner))
		hwndOwner=GetDesktopWindow();

	/* Get the Height and Width of the child window */
	GetWindowRect(hWnd, &rcChild);
	cxChild=rcChild.right-rcChild.left;
	cyChild=rcChild.bottom-rcChild.top;

	/* Get the Height and Width of the parent window */
	GetWindowRect(hwndOwner, &rcOwner);
	cxOwner=rcOwner.right-rcOwner.left;
	cyOwner=rcOwner.bottom-rcOwner.top;

	//* Get the display limits */
	hDC=GetDC(hwndOwner);
	cxScreen=GetDeviceCaps(hDC, HORZRES);
	cyScreen=GetDeviceCaps(hDC, VERTRES);
	ReleaseDC(hwndOwner, hDC);

	/* Calculate new X position, then adjust for screen */
	xNew=rcOwner.left+((cxOwner-cxChild) / 2);
	if(xNew<0)
	{
		xNew=0;
	}
	else if((xNew+cxChild)>cxScreen)
	{
		xNew=cxScreen-cxChild;
	}

	/* Calculate new Y position, then adjust for screen */
	yNew=rcOwner.top+((cyOwner-cyChild) / 2);
	if(yNew<0)
	{
		yNew=0;
	}
	else if((yNew+cyChild)>cyScreen)
	{
		yNew=cyScreen-cyChild;
	}

	return SetWindowPos(hWnd, NULL, xNew, yNew, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
}

TOLCDECL void TOLAPI Wnd_MoveToCenter0(HWND hWnd)
{
	HWND hOwner;
	RECT rc, rcOwned, rcOwner;

	hOwner=GetParent(hWnd);
	if(!hOwner)
		hOwner=GetDesktopWindow();

	GetWindowRect(hOwner, &rcOwner);
	GetWindowRect(hWnd, &rcOwned);
	CopyRect(&rc, &rcOwner);

	/* Offset the owner and dialog box rectangles so that
	// right and bottom values represent the width and
	// height, and then offset the owner again to discard
	// space taken up by the dialog box. */
	OffsetRect(&rcOwned, -rcOwned.left, -rcOwned.top);
	OffsetRect(&rc, -rc.left, -rc.top);
	OffsetRect(&rc, -rcOwned.right, -rcOwned.bottom);

	/* The new position is the sum of half the remaining
	// space and the owner's original position. */
	SetWindowPos(hWnd, HWND_TOP, rcOwner.left+(rc.right/2), rcOwner.top+(rc.bottom/2), 0, 0, SWP_NOSIZE);
}

TOLCDECL void TOLAPI Wnd_SetForeground(HWND hWnd)
{
	if(!IsWindowVisible(hWnd))
		ShowWindow(hWnd, SW_SHOW);

	if(IsIconic(hWnd))
		OpenIcon(hWnd);

	ShowOwnedPopups(hWnd, TRUE);
	SetForegroundWindow(GetLastActivePopup(hWnd));
}

TOLCDECL LRESULT TOLAPI Wnd_EnableTheme(HWND hwnd, LPCWSTR classList, LPCWSTR subApp, LPCWSTR idlist)
{
	HMODULE hinstDll;
	HRESULT (__stdcall *pSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
	HANDLE (__stdcall *pOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
	HRESULT (__stdcall *pCloseThemeData)(HANDLE hTheme);

	hinstDll=LoadLibrary(_T("UxTheme.dll"));
	if(hinstDll)
	{
		(FARPROC)pOpenThemeData=GetProcAddress(hinstDll, "OpenThemeData");
		(FARPROC)pCloseThemeData=GetProcAddress(hinstDll, "CloseThemeData");
		(FARPROC)pSetWindowTheme=GetProcAddress(hinstDll, "SetWindowTheme");
		FreeLibrary(hinstDll);

		if(pSetWindowTheme && pOpenThemeData && pCloseThemeData)
		{
			HANDLE theme=pOpenThemeData(hwnd,classList);
			if(theme)
			{
				pCloseThemeData(theme);
				return pSetWindowTheme(hwnd, subApp, idlist);
			}
		}
	}
	return S_FALSE;
}


/*********************************************************************************************************
 Mouse
**********************/


TOLCDECL UINT TOLAPI Mouse_WheelScrollLines()
{
	HKEY hKey;
	int nScrollLines=3; // reasonable default

	if(RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"), 0, KEY_QUERY_VALUE, &hKey)==ERROR_SUCCESS)
	{
		TCHAR szData[128];
		DWORD dwKeyDataType;
		DWORD dwDataBufSize=sizeof(szData);

		if(RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType, (LPBYTE) &szData, &dwDataBufSize)==ERROR_SUCCESS)
			nScrollLines=_tcstoul(szData, NULL, 10);

		RegCloseKey(hKey);
	}

	return nScrollLines;
}


/*********************************************************************************************************
 Folder
**********************/

TOLCDECL BOOL TOLAPI Folder_EnsureExists(LPCTSTR pcFolder)
{
	//if(pcFolder==NULL || pcFolder[0]==_T('\0'))
	//	return FALSE;

	BOOL bExists=PathFileExists(pcFolder);
	if(!bExists)
	{
		LPTSTR pc=Str_Dup(pcFolder);
		PathRemoveFileSpec(pc);
		bExists=Folder_EnsureExists(pc);
		Str_Kill(pc);

		if(bExists)
			bExists=CreateDirectory(pcFolder, NULL);
	}

	return bExists;
}


TOLEXTC_END
