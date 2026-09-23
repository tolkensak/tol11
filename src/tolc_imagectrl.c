
#include "stdafx.h"
#include <tolc_imagectrl.h>
#include <tolc_dispatch.h>
#include <tolc_memory.h>
#include <ocidl.h>
#include <olectl.h>


TOLEXTC_BEGIN


#define IMAGECTRL_PROPNAME _T("TolImageCtrlProp")


/* T_IMAGECTRLDATA */

typedef struct tagImageCtrlData {
	LPPICTURE pPicture;
} T_IMAGECTRLDATA, *PT_IMAGECTRLDATA;


/* functions */

void _ImageCtrl_CleanData(HWND hWnd)
{
	PT_IMAGECTRLDATA pd=(PT_IMAGECTRLDATA)GetProp(hWnd, IMAGECTRL_PROPNAME);
	if(pd)
	{
		if(pd->pPicture)
			pd->pPicture->Release();

		Mem_Free(pd);
	}

	RemoveProp(hWnd, IMAGECTRL_PROPNAME);
}


/* message map */

LRESULT _ImageCtrl_Paint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _ImageCtrl_SetPath(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT _ImageCtrl_Destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

T_MSGMAP _mmImageCtrl[]=
{
	{WM_PAINT,     _ImageCtrl_Paint},
	{TICM_SETPATH, _ImageCtrl_SetPath},
	{WM_DESTROY,   _ImageCtrl_Destroy},
};

T_MSGMAPINF _mmiImageCtrl={ sizeof(_mmImageCtrl)/sizeof(T_MSGMAP), _mmImageCtrl, dwpWindow };


/* implementation */

LRESULT _ImageCtrl_SetPath(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPCTSTR pcFilePath=(LPCTSTR)lParam;
	if(pcFilePath)
	{
		PT_IMAGECTRLDATA pd=Mem_Alloc(MEM_ZERO_MEMORY, sizeof(T_IMAGECTRLDATA));
		if(pd)
		{
			OLECHAR polec[MAX_PATH];
#ifdef UNICODE
			lstrcpyn(polec, pcFilePath, MAX_PATH);
#else
			mbstowcs(polec, pcFilePath, MAX_PATH);
#endif

			if(S_OK==OleLoadPicturePath(polec, NULL, 0, 0, IID_IPicture, (LPVOID*)&pd->pPicture))
				SetProp(hWnd, IMAGECTRL_PROPNAME, (HANDLE)pd);
			else
				Mem_Free(pd);
		}
	}
	else
		_ImageCtrl_CleanData(hWnd);

	return 0;
}

LRESULT _ImageCtrl_Destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	_ImageCtrl_CleanData(hWnd);
	return 0;
}

LRESULT _ImageCtrl_Paint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//PAINTSTRUCT ps;
	//HDC hdc=BeginPaint(hWnd, &ps);


	//PT_IMAGECTRLDATA picd=(PT_IMAGECTRLDATA)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	//if(picd && picd->pcLabel)
	//{
	//	HGDIOBJ hOldFont=SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
	//	int nOldBkMode=SetBkMode(hdc, TRANSPARENT);
	//	COLORREF clrOldText=GetTextColor(hdc);
	//	COLORREF clrText=0;

	//	switch(byMode)
	//	{
	//	case TLC_MODE_NORMAL: clrText=picd->byOwnNormalColor?picd->clrNormal:_clrImageCtrlNormal; break;
	//	case TLC_MODE_FLASH: clrText=picd->byOwnFlashColor?picd->clrFlash:_clrImageCtrlFlash; break;
	//	}

	//	SetTextColor(hdc, clrText);
	//	TextOut(hdc, 0, 0, picd->pcLabel, picd->cchLabel);
	//	SetTextColor(hdc, clrOldText);
	//	SetBkMode(hdc, nOldBkMode);
	//	SelectObject(hdc, hOldFont);

	//	/* underline */
	//	//if(underline)
	//	//{
	//	//	RECT rc;
	//	//	HPEN hPen=CreatePen(PS_SOLID, 2, clrNormal);
	//	//	HPEN hOldPen=SelectObject(hdc, hPen);
	//	//	GetClientRect(hWnd, &rc);
	//	//	MoveToEx(hdc, rc.left, rc.bottom, NULL);
	//	//	LineTo(hdc, rc.right, rc.bottom);
	//	//	SelectObject(hdc, hOldPen);
	//	//	DeleteObject(hPen);
	//	//}
	//}

	//EndPaint(hWnd, &ps);
	return 0;
}

LRESULT CALLBACK _ImageCtrl_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Disp_Message(&_mmiImageCtrl, hWnd, uMsg, wParam, lParam);
}


/* functions */

TOLCDECL BOOL TOLAPI ImageCtrl_RegisterClass(HINSTANCE hInst)
{
	WNDCLASSEX wcex;

	if(GetClassInfoEx(hInst, IMAGECTRL_CLASSNAME, &wcex))
		return TRUE;

	wcex.cbSize=sizeof(WNDCLASSEX);
	wcex.style=CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc=(WNDPROC)_ImageCtrl_WndProc;
	wcex.hInstance=hInst;
	wcex.hIcon=NULL;
	wcex.hCursor=(HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE|LR_SHARED);
	wcex.hbrBackground=NULL;
	wcex.lpszMenuName=NULL;
	wcex.lpszClassName=IMAGECTRL_CLASSNAME;
	wcex.hIconSm=NULL;
	wcex.cbClsExtra=0;
	wcex.cbWndExtra=0;

	return RegisterClassEx(&wcex);
}


TOLEXTC_END
