
#ifndef _INC_TOLC_DRAW_H
#define _INC_TOLC_DRAW_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


#define COLOR_SHIFT(x, f) ((BYTE)(x+(255-x)*f))


TOLCDECL BOOL TOLAPI Draw_IsHighContrast();

TOLCDECL int TOLAPI Draw_GetScreenColorNum();
TOLCDECL COLORREF TOLAPI Draw_GetMixedColor(COLORREF clr1, COLORREF clr2, int nPercent);

TOLCDECL void TOLAPI Draw_MenuText(HDC hDC, LPRECT pRect, LPCTSTR pcText, UINT uFormat);
TOLCDECL void TOLAPI Draw_CheckMark(HDC hDC, LPRECT pRect, COLORREF crColor, int nWeight/*=3*/);
TOLCDECL void TOLAPI Draw_Bitmap(HDC hDC, LPRECT pRect, HBITMAP hBitmap, COLORREF crBk);
TOLCDECL void TOLAPI Draw_GrayBitmap(HDC hDC, LPRECT pRect, HBITMAP hBitmap, COLORREF crBk);


TOLEXTC_END

#endif /* _INC_TOLC_DRAW_H */
