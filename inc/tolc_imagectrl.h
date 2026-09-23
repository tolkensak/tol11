
#ifndef _INC_TOLC_IMAGECTRL_H
#define _INC_TOLC_IMAGECTRL_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


#define IMAGECTRL_CLASSNAME _T("TolImageCtrl")


TOLCDECL BOOL TOLAPI ImageCtrl_RegisterClass(HINSTANCE hInst);


#define TICM_SETPATH (WM_USER+1)
#define ImageCtrl_SetPath(hWnd, pcFilePath) \
	((void)SendMessage((HWND)(hWnd), TICM_SETDATA, 0, (LPARAM)(pcFilePath)))


TOLEXTC_END

#endif /* _INC_TOLC_IMAGECTRL_H */
