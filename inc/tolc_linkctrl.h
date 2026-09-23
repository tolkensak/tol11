
#ifndef _INC_TOLC_LINKCTRL_H
#define _INC_TOLC_LINKCTRL_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


#define LINKCTRL_CLASSNAME _T("TolLinkCtrl")

#define TLC_MODE_NORMAL     0
#define TLC_MODE_FLASH      1

#define TLCM_GETCOLOR       (WM_USER+1)
#define TLCM_SETCOLOR       (WM_USER+2)
#define TLCM_RESETCOLOR     (WM_USER+3)
#define TLCM_GETLABELLEN    (WM_USER+4)
#define TLCM_GETLABEL       (WM_USER+5)
#define TLCM_SETLABEL       (WM_USER+6)
#define TLCM_GETHREFLEN     (WM_USER+7)
#define TLCM_GETHREF        (WM_USER+8)
#define TLCM_SETHREF        (WM_USER+9)
#define TLCM_SETMSGID       (WM_USER+10)
#define TLCM_SETMSGWND      (WM_USER+11)

TOLCDECL BOOL TOLAPI LinkCtrl_RegisterClass(HINSTANCE hInst);
TOLCDECL COLORREF TOLAPI LinkCtrl_GetDefaultColor(BYTE byMode);
TOLCDECL void TOLAPI LinkCtrl_SetDefaultColor(BYTE byMode, COLORREF clrNew);

#define LinkCtrl_GetColor(hWnd, byMode)                  ((COLORREF)SendMessage((HWND)(hWnd), TLCM_GETCOLOR, (WPARAM)(byMode), 0))
#define LinkCtrl_SetColor(hWnd, byMode, clrNew)          ((void)SendMessage((HWND)(hWnd), TLCM_SETCOLOR, (WPARAM)(byMode), (LPARAM)(clrNew)))
#define LinkCtrl_ResetColor(hWnd, byMode)                ((void)SendMessage((HWND)(hWnd), TLCM_RESETCOLOR, (WPARAM)(byMode), 0))
 // return count char not include terminating NULL
#define LinkCtrl_GetLabelLen(hWnd)                       ((int)SendMessage((HWND)(hWnd), TLCM_GETLABELLEN, 0, 0))
// cchBufferSize include terminating NULL
#define LinkCtrl_GetLabel(hWnd, pcBuffer, cchBufferSize) ((int)SendMessage((HWND)(hWnd), TLCM_GETLABEL, (WPARAM)(cchBufferSize), (LPARAM)(pcBuffer)))
#define LinkCtrl_SetLabel(hWnd, pcLabel)                 ((void)SendMessage((HWND)(hWnd), TLCM_SETLABEL, 0, (LPARAM)(pcLabel)))
// return count char not include terminating NULL
#define LinkCtrl_GetHrefLen(hWnd)                        ((int)SendMessage((HWND)(hWnd), TLCM_GETHREFLEN, 0, 0))
// cchBufferSize include terminating NULL
#define LinkCtrl_GetHref(hWnd, pcBuffer, cchBufferSize)  ((int)SendMessage((HWND)(hWnd), TLCM_GETHREF, (WPARAM)(cchBufferSize), (LPARAM)(pcBuffer)))
#define LinkCtrl_SetHref(hWnd, pcHref)                   ((void)SendMessage((HWND)(hWnd), TLCM_SETHREF, 0, (LPARAM)(pcHref)))

#define LinkCtrl_SetMsgID(hWnd, uMsgID)                  ((void)SendMessage((HWND)(hWnd), TLCM_SETMSGID, (WPARAM)(uMsgID), 0))
#define LinkCtrl_SetMsgWnd(hWnd, hWndMsg)                ((void)SendMessage((HWND)(hWnd), TLCM_SETMSGWND, 0, (LPARAM)(hWndMsg)))


TOLEXTC_END

#endif /* _INC_TOLC_LINKCTRL_H */
