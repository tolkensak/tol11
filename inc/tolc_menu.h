
#ifndef _INC_TOLC_MENU_H
#define _INC_TOLC_MENU_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


TOLCDECL Bool TOLAPI Menu_Copy(HMENU hmDest, int nPos, HMENU hmSrc);
TOLCDECL Bool TOLAPI Menu_CopyItem(HMENU hmDest, int nPos, HMENU hmSrc, Uint uID, Bool bLoseShortKey);
TOLCDECL Bool TOLAPI Menu_IsItemChecked(HWND hWnd, Uint uID);
TOLCDECL HMENU TOLAPI Menu_GetParent(HMENU hMenu, Uint uID, Bool bByID);
TOLCDECL Bool TOLAPI Menu_ShowUnderlineLetter();


TOLEXTC_END

#endif /* _INC_TOLC_MENU_H */
