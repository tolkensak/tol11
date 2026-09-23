
#ifndef _INC_TOLC_MEMDC_H
#define _INC_TOLC_MEMDC_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


typedef struct tagMemDC
{
	HDC hDC;
	HDC hdcComp;
	HBITMAP hbm;
	HGDIOBJ hbmOld;
	RECT rc;
} T_MEMDC, *PT_MEMDC;

TOLCDECL HDC TOLAPI MemDC_BeginPaint(PT_MEMDC pMemDC);
TOLCDECL void TOLAPI MemDC_EndPaint(PT_MEMDC pMemDC);



typedef void (TOLCALL *PFNMEMDCPAINT)(HDC hDC, LPCRECT pRect, LPARAM lParam);

TOLCDECL void TOLAPI MemDC_Paint(HDC hDC, PFNMEMDCPAINT pFn, LPARAM lParam);


TOLEXTC_END

#endif /* _INC_TOLC_MEMDC_H */
