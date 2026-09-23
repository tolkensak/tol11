
#ifndef _INC_TOLC_APP_H
#define _INC_TOLC_APP_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


/*********************************************************************************************************
 key
**************/

#define KEY_ISPRESSED(shVirKey) (GetKeyState((SHORT)(shVirKey))>>(sizeof(SHORT)*8-1))
#define KEY_ISTOGGLED(shVirKey) (GetKeyState((SHORT)(shVirKey))&1)


/*********************************************************************************************************
 application
************************/

TOLCDECL int TOLAPI App_RootRegKey(HINSTANCE hInstance, PChar pcBuffer, int nBufferLen);

TOLCDECL BOOL TOLAPI App_Alone(LPCTSTR pcWndClass, LPCTSTR pcWndTitle);
TOLCDECL WORD TOLAPI App_SoleStart(LPCTSTR pcUnique);
TOLCDECL void TOLAPI App_SoleStop(WORD w);


/*********************************************************************************************************
 process
********************/

TOLCDECL DWORD TOLAPI Process_FindAnother(DWORD dwPID);


/*********************************************************************************************************
 hotkey
********************/

TOLCDECL DWORD TOLAPI HotKey_CtlToSys(WORD wHotKey);
TOLCDECL WORD TOLAPI HotKey_SysToCtl(DWORD dwHotKey);
TOLCDECL int TOLAPI HotKey_CtlToText(WORD wHotKey, LPTSTR pcBuff, int nBuffLen);
TOLCDECL int TOLAPI HotKey_SysToText(DWORD dwHotKey, LPTSTR pcBuff, int nBuffLen);


/*********************************************************************************************************
 rectangle
***********************/

TOLCDECL void TOLAPI Rect_GetEdge(LPRECT prcEdge, LPCRECT prcOuter, LPCRECT prcInner);
TOLCDECL void TOLAPI Rect_Sizing(LPRECT prc, int left, int top, int right, int bottom);

TOLCDECL void TOLAPI Rect_ClientToScreen(HWND hWnd, LPRECT lprc);
TOLCDECL void TOLAPI Rect_ScreenToClient(HWND hWnd, LPRECT lprc);


/*********************************************************************************************************
 resource
**********************/

TOLCDECL LPVOID TOLAPI Resource_Load(HMODULE hModule, LPCTSTR pcType, LPCTSTR pcName, LANGID lid);
TOLCDECL int TOLAPI Resource_MsgBox(HINSTANCE hInst, HWND hWnd, LPCTSTR pcTitle, UINT uType, UINT uID, LANGID lid, LPCTSTR pcSafe);

TOLCDECL void TOLAPI Resource_GetTooltip(PCChar pcOrig, PChar pcDest, int cch, BOOL bWithShortKey);
TOLCDECL void TOLAPI Resource_GetShortkey(PCChar pcOrig, PChar pcDest, int cch);
TOLCDECL void TOLAPI Resource_GetInfo(PCChar pcOrig, PChar pcDest, int cch);

TOLCDECL int TOLAPI Resource_LoadStr(HMODULE hModule, UINT uID, PChar pcBuff, int cchBuff, LANGID lid);


/*********************************************************************************************************
 tooltip
************************/

TOLCDECL HWND TOLAPI Tooltip_Create(DWORD dwStyle, HWND hWndParent, HINSTANCE hInstance);


/*********************************************************************************************************
 window
**********************/

TOLCDECL BOOL TOLAPI Wnd_MoveToCenter(HWND hWnd, HWND hWndOwner);
TOLCDECL void TOLAPI Wnd_MoveToCenter0(HWND hWnd);
TOLCDECL void TOLAPI Wnd_SetForeground(HWND hWnd);
TOLCDECL LRESULT TOLAPI Wnd_EnableTheme(HWND hwnd, LPCWSTR classList, LPCWSTR subApp, LPCWSTR idlist);


/*********************************************************************************************************
 Mouse
**********************/


TOLCDECL UINT TOLAPI Mouse_WheelScrollLines();


/*********************************************************************************************************
 Folder
**********************/


TOLCDECL BOOL TOLAPI Folder_EnsureExists(LPCTSTR pcFolder);



TOLEXTC_END

#endif /* _INC_TOLC_APP_H */
