
#include "stdafx.h"
#include <tolc_memdc.h>


TOLEXTC_BEGIN


TOLCDECL HDC TOLAPI MemDC_BeginPaint(PT_MEMDC pMemDC)
{
	/*ASSERT(pMemDC!=NULL);*/

	GetClipBox(pMemDC->hDC, &pMemDC->rc);

	pMemDC->hdcComp=CreateCompatibleDC(pMemDC->hDC);
	pMemDC->hbm=CreateCompatibleBitmap(pMemDC->hDC, pMemDC->rc.right-pMemDC->rc.left, pMemDC->rc.bottom-pMemDC->rc.top);
	pMemDC->hbmOld=SelectObject(pMemDC->hdcComp, pMemDC->hbm);
	SetWindowOrgEx(pMemDC->hdcComp, pMemDC->rc.left, pMemDC->rc.top, NULL);

	return pMemDC->hdcComp;
}

TOLCDECL void TOLAPI MemDC_EndPaint(PT_MEMDC pMemDC)
{
	/*ASSERT(pMemDC!=NULL);*/

	BitBlt(pMemDC->hDC, pMemDC->rc.left, pMemDC->rc.top, pMemDC->rc.right-pMemDC->rc.left, pMemDC->rc.bottom-pMemDC->rc.top, pMemDC->hdcComp, pMemDC->rc.left, pMemDC->rc.top, SRCCOPY);
	SelectObject(pMemDC->hdcComp, pMemDC->hbmOld);
	DeleteObject(pMemDC->hbm);
	DeleteDC(pMemDC->hdcComp);
}

TOLCDECL void TOLAPI MemDC_Paint(HDC hDC, PFNMEMDCPAINT pFn, LPARAM lParam)
{
	RECT rc;
	HDC hMemDC;
	HBITMAP hbm;
	HGDIOBJ hbmOld;

	/*ASSERT(hDC!=NULL);
	ASSERT(pFn!=NULL);*/


	GetClipBox(hDC, &rc);

	hMemDC=CreateCompatibleDC(hDC);
	hbm=CreateCompatibleBitmap(hDC, rc.right-rc.left, rc.bottom-rc.top);
	hbmOld=SelectObject(hMemDC, hbm);
	SetWindowOrgEx(hMemDC, rc.left, rc.top, NULL);

	pFn(hMemDC, &rc, lParam);

	BitBlt(hDC, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, hMemDC, rc.left, rc.top, SRCCOPY);
	SelectObject(hMemDC, hbmOld);
	DeleteObject(hbm);
	DeleteDC(hMemDC);
}


TOLEXTC_END
