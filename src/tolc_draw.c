
#include "stdafx.h"
#include <tolc_draw.h>
#include <tolc_string.h>


TOLEXTC_BEGIN


TOLCDECL BOOL TOLAPI Draw_IsHighContrast()
{
	HIGHCONTRAST hc={0};
	hc.cbSize=sizeof(HIGHCONTRAST);

	if(SystemParametersInfo(SPI_GETHIGHCONTRAST, hc.cbSize ,&hc, 0))
		return hc.dwFlags&HCF_HIGHCONTRASTON;

	return TRUE;
}

TOLCDECL int TOLAPI Draw_GetScreenColorNum()
{
	HDC hDC=GetDC(NULL); /* DC of the desktop */
	int nColor=GetDeviceCaps(hDC, NUMCOLORS);

	if(nColor==-1)
		nColor=64000;

	ReleaseDC(NULL, hDC);
	return nColor;
} 

TOLCDECL COLORREF TOLAPI Draw_GetMixedColor(COLORREF clr1, COLORREF clr2, int nPercent)
{
	if(Draw_GetScreenColorNum()>256) 
	{
		int r=MulDiv(100-nPercent, GetRValue(clr1), 100)+MulDiv(nPercent, GetRValue(clr2), 100);
		int g=MulDiv(100-nPercent, GetGValue(clr1), 100)+MulDiv(nPercent, GetGValue(clr2), 100);
		int b=MulDiv(100-nPercent, GetBValue(clr1), 100)+MulDiv(nPercent, GetBValue(clr2), 100);
		return RGB(r, g, b);
	}

	return clr1;
}

TOLCDECL void TOLAPI Draw_MenuText(HDC hDC, LPRECT pRect, LPCTSTR pcText, UINT uFormat)
{
	int nOldBkMode=SetBkMode(hDC, TRANSPARENT);

	PCChar pcTabPos=Str_Char(pcText, _T('\t'));

	if(pcTabPos)
	{
		DrawText(hDC, pcText, (int)(pcTabPos-pcText), pRect, uFormat);

		pcTabPos++;
		DrawText(hDC, pcTabPos, -1, pRect, uFormat|DT_RIGHT);
	}
	else
		DrawText(hDC, pcText, -1, pRect, uFormat);

	SetBkMode(hDC, nOldBkMode);
}

TOLCDECL void TOLAPI Draw_CheckMark(HDC hDC, LPRECT pRect, COLORREF crColor, int nWeight/*=3*/)
{
	int CX=pRect->right-pRect->left;
	int CY=pRect->bottom-pRect->top;
	//int n=(int)((CX>CY?CY:CX)/10.0);
	//int cx=(int)(7*n);
	//int cy=(int)(5*n);
	//nWidth*=(int)(.5*n);

	int n=2;
	int cx=7;
	int cy=5;

	POINT ptVertex[6]=
	{
		{0, n},
		{0, n+nWeight},
		{n, cy+nWeight},
		{cx, nWeight},
		{cx, 0},
		{n, cy},
	};

	POINT ptVx;
	HBRUSH hbr=CreateSolidBrush(crColor);
	HRGN hrgn=CreatePolygonRgn(ptVertex, 6, WINDING);
	SetViewportOrgEx(hDC, pRect->left+(CX-cx)/2, pRect->top+(CY-cy-nWeight)/2, &ptVx);
	FillRgn(hDC, hrgn, hbr);
	SetViewportOrgEx(hDC, ptVx.x, ptVx.y, NULL);
	DeleteObject(hbr);
	DeleteObject(hrgn);
}

TOLCDECL void TOLAPI Draw_GrayBitmap(HDC hDC, LPRECT pRect, HBITMAP hBitmap, COLORREF crBk)
{
	//BITMAP bm;
	//GetObject(hBitmap, sizeof(BITMAP), &bm);
	//CBitmap* pbm=CBitmap::FromHandle(hBitmap);
	//AfxDrawGrayBitmap(CDC::FromHandle(hDC), (pRect->right+pRect->left-bm.bmWidth)/2, (pRect->bottom+pRect->top-bm.bmHeight)/2, *pbm, RGB(255, 255, 255));

	int i, j;
	BITMAP bm;
	COLORREF cr;
	COLORREF crGray;
	HDC hdcSrc, hdcMask;
	HBITMAP hbmMask, hbmGray, hbmOldMask, hbmOldSrc;

	GetObject(hBitmap, sizeof(BITMAP), &bm);

	hdcSrc=CreateCompatibleDC(hDC);
	hbmOldSrc=(HBITMAP)SelectObject(hdcSrc, hBitmap);
	cr=GetPixel(hdcSrc, 0, 0);

	hdcMask=CreateCompatibleDC(hDC);
	hbmMask=CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
	hbmOldMask=(HBITMAP)SelectObject(hdcMask, hbmMask);

	BitBlt(hdcMask, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);

	for(i=0; i<bm.bmHeight; i++)
		for(j=0; j<bm.bmWidth; j++)
			if(GetPixel(hdcMask, i, j)==cr)
				SetPixelV(hdcMask, i, j, 0xFFFFFF);

	hbmGray=CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
	SelectObject(hdcSrc, hbmGray);
	BitBlt(hdcSrc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMask, 0, 0, SRCCOPY);
	BitBlt(hdcMask, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);

	crGray=GetSysColor(COLOR_GRAYTEXT);
	for(i=0; i<bm.bmHeight; i++)
		for(j=0; j<bm.bmWidth; j++)
			if(GetPixel(hdcMask, i, j)==0xFFFFFF)
				SetPixelV(hdcMask, i, j, crBk);
			else
				SetPixelV(hdcMask, i, j, crGray);

	BitBlt(hDC, (pRect->right+pRect->left-bm.bmWidth)/2, (pRect->bottom+pRect->top-bm.bmHeight)/2,
		bm.bmWidth, bm.bmHeight, hdcMask, 0, 0, SRCCOPY);

	SelectObject(hdcMask, hbmOldMask);
	SelectObject(hdcSrc, hbmOldSrc);
	DeleteObject(hbmMask);
	DeleteObject(hbmGray);
	DeleteDC(hdcMask);
	DeleteDC(hdcSrc);
}

TOLCDECL void TOLAPI Draw_Bitmap(HDC hDC, LPRECT pRect, HBITMAP hBitmap, COLORREF crBk)
{
	int i, j;
	BITMAP bm;
	COLORREF cr;
	HDC hdcSrc, hdcMask;
	HBITMAP hbmMask, hbmOldMask, hbmOldSrc;

	GetObject(hBitmap, sizeof(BITMAP), &bm);

	hdcSrc=CreateCompatibleDC(hDC);
	hbmOldSrc=(HBITMAP)SelectObject(hdcSrc, hBitmap);
	cr=GetPixel(hdcSrc, 0, 0);

	hdcMask=CreateCompatibleDC(hDC);
	hbmMask=CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
	hbmOldMask=(HBITMAP)SelectObject(hdcMask, hbmMask);

	BitBlt(hdcMask, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);

	for(i=0; i<bm.bmHeight; i++)
		for(j=0; j<bm.bmWidth; j++)
			if(GetPixel(hdcMask, i, j)==cr)
				SetPixelV(hdcMask, i, j, crBk);

	BitBlt(hDC, (pRect->right+pRect->left-bm.bmWidth)/2, (pRect->bottom+pRect->top-bm.bmHeight)/2,
		bm.bmWidth, bm.bmHeight, hdcMask, 0, 0, SRCCOPY);

	SelectObject(hdcMask, hbmOldMask);
	SelectObject(hdcSrc, hbmOldSrc);
	DeleteObject(hbmMask);
	DeleteDC(hdcMask);
	DeleteDC(hdcSrc);
}


TOLEXTC_END
