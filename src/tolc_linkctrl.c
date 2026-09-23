
#include "stdafx.h"
#include <tolc_linkctrl.h>
#include <tolc_dispatch.h>
#include <tolc_memory.h>

#include <shellapi.h> /* tooltip */
#include <commctrl.h> /* imagelist, toolbar, hotkey, mouse evenet */
#pragma comment(lib, "comctl32.lib") /* imagelist, toolbar, hotkey, mouse evenet */


TOLEXTC_BEGIN


/* variables */

#ifdef TOLCDLL
#pragma data_seg("Shared")
#endif

HWND     _hwndLinkCtrlTooltip=NULL;
COLORREF _clrLinkCtrlNormal=0xff0000;
COLORREF _clrLinkCtrlFlash=0x0000ff;

#ifdef TOLCDLL
#pragma data_seg()
#pragma comment(linker, "/section:Shared,RWS")
#endif

typedef struct tagLinkCtrlData
{
	int cchLabel;
	LPTSTR pcLabel;
	LPTSTR pcHref;
	BYTE byOwnNormalColor;
	BYTE byOwnFlashColor;
	COLORREF clrNormal;
	COLORREF clrFlash;
	HWND hWndMsg;
	UINT uMsgID;
} T_LINKCTRLDATA, *PT_LINKCTRLDATA;

/* functions */

/* message map */
LRESULT _LinkCtrl_Create(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_Paint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_MouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_MouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_Click(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_Destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_GetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_SetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_ResetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_GetLabelLen(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_GetLabel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_SetLabel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_GetHrefLen(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_GetHref(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_SetHref(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_SetMsgID(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _LinkCtrl_SetMsgWnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

T_MSGMAP _mmLinkCtrl[]=
{
	{WM_PAINT,            _LinkCtrl_Paint},
	{WM_MOUSEMOVE,        _LinkCtrl_MouseMove},
	{WM_MOUSELEAVE,       _LinkCtrl_MouseLeave},
	{WM_LBUTTONUP,        _LinkCtrl_Click},
	{WM_CREATE,           _LinkCtrl_Create},
	{WM_DESTROY,          _LinkCtrl_Destroy},
	{TLCM_GETCOLOR,       _LinkCtrl_GetColor},
	{TLCM_SETCOLOR,       _LinkCtrl_SetColor},
	{TLCM_RESETCOLOR,     _LinkCtrl_ResetColor},
	{TLCM_GETLABELLEN,    _LinkCtrl_GetLabelLen},
	{TLCM_GETLABEL,       _LinkCtrl_GetLabel},
	{TLCM_SETLABEL,       _LinkCtrl_SetLabel},
	{TLCM_GETHREFLEN,     _LinkCtrl_GetHrefLen},
	{TLCM_GETHREF,        _LinkCtrl_GetHref},
	{TLCM_SETHREF,        _LinkCtrl_SetHref},
	{TLCM_SETMSGID,       _LinkCtrl_SetMsgID},
	{TLCM_SETMSGWND,      _LinkCtrl_SetMsgWnd},
};

T_MSGMAPINF _mmiLinkCtrl={ sizeof(_mmLinkCtrl)/sizeof(T_MSGMAP), _mmLinkCtrl, dwpWindow };

/* implementation */

BOOL _LinkCtrl_TrackMouseEvent(HWND hWnd, DWORD dwFlags)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize=sizeof(TRACKMOUSEEVENT);
	tme.dwFlags=dwFlags;
	tme.hwndTrack=hWnd;
	tme.dwHoverTime=HOVER_DEFAULT;
	return _TrackMouseEvent(&tme);
}

void _LinkCtrl_Draw(HWND hWnd, HDC hdc, BYTE byMode)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd && plcd->pcLabel)
	{
		HGDIOBJ hOldFont=SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
		int nOldBkMode=SetBkMode(hdc, TRANSPARENT);
		COLORREF clrOldText=GetTextColor(hdc);
		COLORREF clrText=0;

		switch(byMode)
		{
		case TLC_MODE_NORMAL: clrText=plcd->byOwnNormalColor?plcd->clrNormal:_clrLinkCtrlNormal; break;
		case TLC_MODE_FLASH: clrText=plcd->byOwnFlashColor?plcd->clrFlash:_clrLinkCtrlFlash; break;
		}

		SetTextColor(hdc, clrText);
		TextOut(hdc, 0, 0, plcd->pcLabel, plcd->cchLabel);
		SetTextColor(hdc, clrOldText);
		SetBkMode(hdc, nOldBkMode);
		SelectObject(hdc, hOldFont);

		/* underline */
		//if(underline)
		//{
		//	RECT rc;
		//	HPEN hPen=CreatePen(PS_SOLID, 2, clrNormal);
		//	HPEN hOldPen=SelectObject(hdc, hPen);
		//	GetClientRect(hWnd, &rc);
		//	MoveToEx(hdc, rc.left, rc.bottom, NULL);
		//	LineTo(hdc, rc.right, rc.bottom);
		//	SelectObject(hdc, hOldPen);
		//	DeleteObject(hPen);
		//}
	}
}

LRESULT _LinkCtrl_Paint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc=BeginPaint(hWnd, &ps);
	_LinkCtrl_Draw(hWnd, hdc, TLC_MODE_NORMAL);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT _LinkCtrl_MouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc=GetDC(hWnd);
	_LinkCtrl_Draw(hWnd, hdc, TLC_MODE_FLASH);
	ReleaseDC(hWnd, hdc);

	_LinkCtrl_TrackMouseEvent(hWnd, TME_LEAVE);
	return 0;
}

LRESULT _LinkCtrl_MouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc=GetDC(hWnd);
	_LinkCtrl_Draw(hWnd, hdc, TLC_MODE_NORMAL);
	ReleaseDC(hWnd, hdc);

	return 0;
}

LRESULT _LinkCtrl_Click(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
	{
		if(plcd->pcHref)
			ShellExecute(NULL, _T("open"), plcd->pcHref, NULL, NULL, SW_SHOWNORMAL);
		else if(plcd->uMsgID)
			SendMessage(plcd->hWndMsg?plcd->hWndMsg:GetParent(hWnd), plcd->uMsgID, 0, (LPARAM)hWnd);
	}

	return 0;
}

LRESULT _LinkCtrl_Create(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int cchOrig;
	TCHAR pcOrig[MAX_PATH];

	cchOrig=GetWindowText(hWnd, pcOrig, MAX_PATH);
	if(cchOrig)
	{
		PT_LINKCTRLDATA plcd=Mem_Alloc(MEM_ZERO_MEMORY, sizeof(T_LINKCTRLDATA));
		if(plcd)
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
				plcd->pcLabel=Mem_Alloc(0, (cch+1)*sizeof(TCHAR));
				if(plcd->pcLabel)
				{
					lstrcpy(plcd->pcLabel, pcOrig);
					plcd->cchLabel=cch;
				}
			}

			cch=cchOrig-cch;
			if(cch>1)
			{
				plcd->pcHref=Mem_Alloc(0, cch*sizeof(TCHAR));
				if(plcd->pcHref)
					lstrcpy(plcd->pcHref, pc+1);
			}

			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)plcd);
		}
	}

	return 0;
}

LRESULT _LinkCtrl_Destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
	{
		if(plcd->pcLabel)
			Mem_Free(plcd->pcLabel);

		if(plcd->pcHref)
			Mem_Free(plcd->pcHref);

		Mem_Free(plcd);
	}

	return 0;
}

LRESULT CALLBACK _LinkCtrl_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Disp_Message(&_mmiLinkCtrl, hWnd, uMsg, wParam, lParam);
}

TOLCDECL BOOL TOLAPI LinkCtrl_RegisterClass(HINSTANCE hInst)
{
	WNDCLASSEX wcex;

	if(GetClassInfoEx(hInst, LINKCTRL_CLASSNAME, &wcex))
		return TRUE;

	wcex.cbSize=sizeof(WNDCLASSEX);
	wcex.style=CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc=(WNDPROC)_LinkCtrl_WndProc;
	wcex.hInstance=hInst;
	wcex.hIcon=NULL;
	wcex.hCursor=(HCURSOR)LoadImage(NULL, IDC_HAND, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE|LR_SHARED);
	wcex.hbrBackground=NULL;
	wcex.lpszMenuName=NULL;
	wcex.lpszClassName=LINKCTRL_CLASSNAME;
	wcex.hIconSm=NULL;
	wcex.cbClsExtra=0;
	wcex.cbWndExtra=0;

	return RegisterClassEx(&wcex);
}

TOLCDECL COLORREF TOLAPI LinkCtrl_GetDefaultColor(BYTE byMode)
{
	switch(byMode)
	{
	case TLC_MODE_NORMAL: return (LRESULT)_clrLinkCtrlNormal;
	case TLC_MODE_FLASH: return (LRESULT)_clrLinkCtrlFlash;
	}

	return 0;
}

TOLCDECL void TOLAPI LinkCtrl_SetDefaultColor(BYTE byMode, COLORREF clrNew)
{
	switch(byMode)
	{
	case TLC_MODE_NORMAL: _clrLinkCtrlNormal=clrNew; break;
	case TLC_MODE_FLASH: _clrLinkCtrlFlash=clrNew; break;
	}
}

LRESULT _LinkCtrl_GetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
	{
		switch(wParam)
		{
		case TLC_MODE_NORMAL: return (LRESULT)plcd->clrNormal;
		case TLC_MODE_FLASH: return (LRESULT)plcd->clrFlash;
		}
	}

	return 0;
}

LRESULT _LinkCtrl_SetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
	{
		switch(wParam)
		{
		case TLC_MODE_NORMAL:
			plcd->byOwnNormalColor=1;
			plcd->clrNormal=(COLORREF)lParam;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case TLC_MODE_FLASH:
			plcd->byOwnFlashColor=1;
			plcd->clrFlash=(COLORREF)lParam;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	}

	return 0;
}

LRESULT _LinkCtrl_ResetColor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
	{
		switch(wParam)
		{
		case TLC_MODE_NORMAL:
			plcd->byOwnNormalColor=0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case TLC_MODE_FLASH:
			plcd->byOwnFlashColor=0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	}

	return 0;
}

LRESULT _LinkCtrl_GetLabelLen(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
		return plcd->cchLabel;

	return 0;
}

LRESULT _LinkCtrl_GetLabel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd && plcd->cchLabel && ((int)wParam)>plcd->cchLabel)
	{
		lstrcpy((LPTSTR)lParam, plcd->pcLabel);
		return plcd->cchLabel;
	}

	return -1;
}

LRESULT _LinkCtrl_SetLabel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
	{
		if(plcd->pcLabel)
		{
			Mem_Free(plcd->pcLabel);
			plcd->pcLabel=NULL;
			plcd->cchLabel=0;
		}

		if(lParam)
		{
			LPCTSTR pc=(LPCTSTR)lParam;
			int cch=lstrlen(pc);
			if(cch)
			{
				plcd->pcLabel=Mem_Alloc(0, (cch+1)*sizeof(TCHAR));
				if(plcd->pcLabel)
				{
					lstrcpy(plcd->pcLabel, pc);
					plcd->cchLabel=cch;
				}
			}
		}
	}

	return 0;
}

LRESULT _LinkCtrl_GetHrefLen(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd && plcd->pcHref)
		return lstrlen(plcd->pcHref);

	return 0;
}

LRESULT _LinkCtrl_GetHref(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
	{
		int cch=plcd->pcHref?lstrlen(plcd->pcHref):0;
		if(cch && ((int)wParam)>cch)
		{
			lstrcpy((LPTSTR)lParam, plcd->pcHref);
			return cch;
		}
	}

	return -1;
}

LRESULT _LinkCtrl_SetHref(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
	{
		if(plcd->pcHref)
		{
			Mem_Free(plcd->pcHref);
			plcd->pcHref=NULL;
		}

		if(lParam)
		{
			LPCTSTR pc=(LPCTSTR)lParam;
			int cch=lstrlen(pc);
			if(cch)
			{
				plcd->pcHref=Mem_Alloc(0, (cch+1)*sizeof(TCHAR));
				if(plcd->pcHref)
					lstrcpy(plcd->pcHref, pc);
			}
		}
	}

	return 0;
}

LRESULT _LinkCtrl_SetMsgID(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
		plcd->uMsgID=(UINT)wParam;

	return 0;
}

LRESULT _LinkCtrl_SetMsgWnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PT_LINKCTRLDATA plcd=(PT_LINKCTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(plcd)
		plcd->hWndMsg=(HWND)lParam;

	return 0;
}

TOLEXTC_END
