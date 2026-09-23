
#include "stdafx.h"
#include <tolc_livetext.h>
#include <tolc_dispatch.h>
#include <tolc_memory.h>

#include <shellapi.h> /* tooltip */
#include <commctrl.h> /* imagelist, toolbar, hotkey */
#pragma comment(lib, "comctl32.lib") /* imagelist, toolbar, hotkey */


TOLEXTC_BEGIN


/* variables */

COLORREF _clrLiveTextNormal=0xff0000;
COLORREF _clrLiveTextLive=0x0000ff;
HWND     _hwndLiveTextTooltip=NULL;

typedef struct tagLiveTextData
{
	int cchText;
	LPTSTR pcText;
	BYTE byOwnNormalColor;
	BYTE byOwnLiveColor;
	COLORREF clrNormal;
	COLORREF clrLive;
} T_LIVETEXTDATA, *PT_LIVETEXTDATA;

/* functions */

/* message map */
LRESULT _LiveText_Create(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_Paint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_MouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_MouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_Click(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_Destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_GetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_SetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_ResetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_GetTextLen(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_GetText(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LiveText_SetText(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

T_MSGMAP _mmLiveText[]=
{
	{WM_PAINT,            _LiveText_Paint},
	{WM_MOUSEMOVE,        _LiveText_MouseMove},
	{WM_MOUSELEAVE,       _LiveText_MouseLeave},
	{WM_LBUTTONUP,        _LiveText_Click},
	{WM_CREATE,           _LiveText_Create},
	{WM_DESTROY,          _LiveText_Destroy},
	{TLTM_GETCOLOR,       _LiveText_GetColor},
	{TLTM_SETCOLOR,       _LiveText_SetColor},
	{TLTM_RESETCOLOR,     _LiveText_ResetColor},
	{TLTM_GETTEXTLEN,     _LiveText_GetTextLen},
	{TLTM_GETTEXT,        _LiveText_GetText},
	{TLTM_SETTEXT,        _LiveText_SetText}
};

T_MSGMAPINF _mmiLiveText={ sizeof(_mmLiveText)/sizeof(T_MSGMAP), _mmLiveText, dwpWindow };

LRESULT CALLBACK _LiveText_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Disp_Message(&_mmiLiveText, hWnd, uMsg, wParam, lParam);
}

/* implementation */

BOOL _LiveText_TrackMouseEvent(HWND hWnd, DWORD dwFlags)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize=sizeof(TRACKMOUSEEVENT);
	tme.dwFlags=dwFlags;
	tme.hwndTrack=hWnd;
	tme.dwHoverTime=HOVER_DEFAULT;
	return _TrackMouseEvent(&tme);
}

void _LiveText_Draw(HWND hWnd, HDC hdc, BYTE byMode)
{
	PT_LIVETEXTDATA pltd=(PT_LIVETEXTDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pltd && pltd->pcText)
	{
		RECT rc;
		HGDIOBJ hOldFont=SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
		int nOldBkMode=SetBkMode(hdc, TRANSPARENT);
		COLORREF clrOldText=GetTextColor(hdc);
		COLORREF clrText=0;

		GetClientRect(hWnd, &rc);

		/* Focus Rec */
		if(GetFocus()==hWnd)
			DrawFocusRect(hdc, &rc);

		switch(byMode)
		{
		case TLT_MODE_NORMAL: clrText=pltd->byOwnNormalColor?pltd->clrNormal:_clrLiveTextNormal; break;
		case TLT_MODE_LIVE: clrText=pltd->byOwnLiveColor?pltd->clrLive:_clrLiveTextLive; break;
		}

		SetTextColor(hdc, clrText);

		TextOut(hdc, 0, 0, pltd->pcText, pltd->cchText);

		SetTextColor(hdc, clrOldText);
		SetBkMode(hdc, nOldBkMode);
		SelectObject(hdc, hOldFont);

		/* underline */
		//if(bUnderline)
		//{
		//	HPEN hPen=CreatePen(PS_SOLID, 2, clrText);
		//	HPEN hOldPen=SelectObject(hdc, hPen);
		//	MoveToEx(hdc, rc.left, rc.bottom, NULL);
		//	LineTo(hdc, rc.right, rc.bottom);
		//	SelectObject(hdc, hOldPen);
		//	DeleteObject(hPen);
		//}
	}
}

LRESULT _LiveText_Paint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc=BeginPaint(hWnd, &ps);
	_LiveText_Draw(hWnd, hdc, TLT_MODE_NORMAL);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT _LiveText_MouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc=GetDC(hWnd);
	_LiveText_Draw(hWnd, hdc, TLT_MODE_LIVE);
	ReleaseDC(hWnd, hdc);

	_LiveText_TrackMouseEvent(hWnd, TME_LEAVE);
	return 0;
}

LRESULT _LiveText_MouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc=GetDC(hWnd);
	_LiveText_Draw(hWnd, hdc, TLT_MODE_NORMAL);
	ReleaseDC(hWnd, hdc);

	return 0;
}

LRESULT _LiveText_Click(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), 0), (LPARAM)hWnd);
	return 0;
}

LRESULT _LiveText_Create(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int cchOrig;
	TCHAR pcOrig[MAX_PATH];

	cchOrig=GetWindowText(hWnd, pcOrig, MAX_PATH);
	if(cchOrig)
	{
		PT_LIVETEXTDATA pltd=Mem_Alloc(MEM_ZERO_MEMORY, sizeof(T_LIVETEXTDATA));
		if(pltd)
		{
			int cch;
			LPTSTR pc;

			pc=pcOrig;
			while(*pc && *pc!=_T('\n'))
				pc++;

			*pc=_T('\0');
			cch=(int)(pc-pcOrig);
			if(cch)
			{
				pltd->pcText=Mem_Alloc(0, (cch+1)*sizeof(TCHAR));
				if(pltd->pcText)
				{
					lstrcpy(pltd->pcText, pcOrig);
					pltd->cchText=cch;
				}
			}

			//cch=cchOrig-cch;
			//if(cch>1)
			//{
			//	pltd->pcHref=Mem_Alloc(0, cch*sizeof(TCHAR));
			//	if(pltd->pcHref)
			//		lstrcpy(pltd->pcHref, pc+1);
			//}

			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)pltd);
		}
	}

	return 0;
}

LRESULT _LiveText_Destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LIVETEXTDATA pltd=(PT_LIVETEXTDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pltd)
	{
		if(pltd->pcText)
			Mem_Free(pltd->pcText);

		Mem_Free(pltd);
	}

	return 0;
}

TOLCDECL BOOL TOLAPI LiveText_RegisterClass(HINSTANCE hInst)
{
	WNDCLASSEX wcex;

	if(GetClassInfoEx(hInst, LIVETEXT_CLASSNAME, &wcex))
		return TRUE;

	wcex.cbSize=sizeof(WNDCLASSEX);
	wcex.style=CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc=(WNDPROC)_LiveText_WndProc;
	wcex.hInstance=hInst;
	wcex.hIcon=NULL;
	wcex.hCursor=(HCURSOR)LoadImage(NULL, IDC_HAND, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE|LR_SHARED);
	wcex.hbrBackground=NULL;
	wcex.lpszMenuName=NULL;
	wcex.lpszClassName=LIVETEXT_CLASSNAME;
	wcex.hIconSm=NULL;
	wcex.cbClsExtra=0;
	wcex.cbWndExtra=0;

	return RegisterClassEx(&wcex);
}

TOLCDECL COLORREF TOLAPI LiveText_GetDefaultColor(BYTE byMode)
{
	switch(byMode)
	{
	case TLT_MODE_NORMAL: return (LRESULT)_clrLiveTextNormal;
	case TLT_MODE_LIVE: return (LRESULT)_clrLiveTextLive;
	}

	return 0;
}

TOLCDECL void TOLAPI LiveText_SetDefaultColor(BYTE byMode, COLORREF clrNew)
{
	switch(byMode)
	{
	case TLT_MODE_NORMAL: _clrLiveTextNormal=clrNew; break;
	case TLT_MODE_LIVE: _clrLiveTextLive=clrNew; break;
	}
}

LRESULT _LiveText_GetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LIVETEXTDATA pltd=(PT_LIVETEXTDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pltd)
	{
		switch(wParam)
		{
		case TLT_MODE_NORMAL: return (LRESULT)pltd->clrNormal;
		case TLT_MODE_LIVE: return (LRESULT)pltd->clrLive;
		}
	}

	return 0;
}

LRESULT _LiveText_SetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LIVETEXTDATA pltd=(PT_LIVETEXTDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pltd)
	{
		switch(wParam)
		{
		case TLT_MODE_NORMAL:
			pltd->byOwnNormalColor=1;
			pltd->clrNormal=(COLORREF)lParam;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case TLT_MODE_LIVE:
			pltd->byOwnLiveColor=1;
			pltd->clrLive=(COLORREF)lParam;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	}

	return 0;
}

LRESULT _LiveText_ResetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LIVETEXTDATA pltd=(PT_LIVETEXTDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pltd)
	{
		switch(wParam)
		{
		case TLT_MODE_NORMAL:
			pltd->byOwnNormalColor=0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case TLT_MODE_LIVE:
			pltd->byOwnLiveColor=0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	}

	return 0;
}

LRESULT _LiveText_GetTextLen(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LIVETEXTDATA pltd=(PT_LIVETEXTDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pltd)
		return pltd->cchText;

	return 0;
}

LRESULT _LiveText_GetText(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LIVETEXTDATA pltd=(PT_LIVETEXTDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pltd && pltd->cchText && ((int)wParam)>pltd->cchText)
	{
		lstrcpy((LPTSTR)lParam, pltd->pcText);
		return pltd->cchText;
	}

	return -1;
}

LRESULT _LiveText_SetText(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LIVETEXTDATA pltd=(PT_LIVETEXTDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pltd)
	{
		if(pltd->pcText)
		{
			Mem_Free(pltd->pcText);
			pltd->pcText=NULL;
			pltd->cchText=0;
		}

		if(lParam)
		{
			LPCTSTR pc=(LPCTSTR)lParam;
			int cch=lstrlen(pc);
			if(cch)
			{
				pltd->pcText=Mem_Alloc(0, (cch+1)*sizeof(TCHAR));
				if(pltd->pcText)
				{
					lstrcpy(pltd->pcText, pc);
					pltd->cchText=cch;
				}
			}
		}
	}

	return 0;
}

TOLEXTC_END
