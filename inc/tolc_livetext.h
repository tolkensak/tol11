
#ifndef _INC_TOLC_LIVETEXT_H
#define _INC_TOLC_LIVETEXT_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


#define LIVETEXT_CLASSNAME _T("TolLiveText")

#define TLT_MODE_NORMAL      0
#define TLT_MODE_LIVE        1

#define TLTM_GETCOLOR        (WM_USER+1)
#define TLTM_SETCOLOR        (WM_USER+2)
#define TLTM_RESETCOLOR      (WM_USER+3)
#define TLTM_GETTEXTLEN      (WM_USER+4)
#define TLTM_GETTEXT         (WM_USER+5)
#define TLTM_SETTEXT         (WM_USER+6)

TOLCDECL BOOL TOLAPI LiveText_RegisterClass(HINSTANCE hInst);
TOLCDECL COLORREF TOLAPI LiveText_GetDefaultColor(BYTE byMode);
TOLCDECL void TOLAPI LiveText_SetDefaultColor(BYTE byMode, COLORREF clrNew);

#define LiveText_GetColor(hWnd, byMode)                 (COLORREF)SendMessage((HWND)(hWnd), TLTM_GETCOLOR, (WPARAM)(byMode), 0)
#define LiveText_SetColor(hWnd, byMode, clrNew)         (void)SendMessage((HWND)(hWnd), TLTM_SETCOLOR, (WPARAM)(byMode), (LPARAM)(clrNew))
#define LiveText_ResetColor(hWnd, byMode)               (void)SendMessage((HWND)(hWnd), TLTM_RESETCOLOR, (WPARAM)(byMode), 0)
 // return count char not include terminating NULL
#define LiveText_GetTextLen(hWnd)                       (int)SendMessage((HWND)(hWnd), TLTM_GETTEXTLEN, 0, 0)
// cchBufferSize include terminating NULL
#define LiveText_GetText(hWnd, pcBuffer, cchBufferSize) (int)SendMessage((HWND)(hWnd), TLTM_GETTEXT, (WPARAM)(cchBufferSize), (LPARAM)(pcBuffer))
#define LiveText_SetText(hWnd, pcText)                  (void)SendMessage((HWND)(hWnd), TLTM_SETTEXT, 0, (LPARAM)(pcText))


TOLEXTC_END

#endif /* _INC_TOLC_LIVETEXT_H */
