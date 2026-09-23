
#ifndef _INC_TOLC_MENU_OD_H
#define _INC_TOLC_MENU_OD_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>
#include <commctrl.h> /* ImageList */


TOLEXTC_BEGIN


#define TMENU_STYLECOUNT          2
#define TMENU_STYLE_WINDOWSXP     0
#define TMENU_STYLE_OFFICEXP      1

TOLEXTC const PCChar TMENU_STYLENAME[TMENU_STYLECOUNT];


#define TMENU_IMAGELIST_NORMAL    0
#define TMENU_IMAGELIST_DISABLED  1
#define TMENU_IMAGELIST_HOT       2


#define TMENU_MAKEDATA(bBarMenu, hMenu) ((DWORD)(((DWORD)(((DWORD_PTR)(bBarMenu)) & 1))|((DWORD)(((DWORD_PTR)(hMenu)) & 0x7fffffff)) << 1))
#define TMENU_GETBARMENU(data)          ((Bool)(((DWORD_PTR)(data)) & 1))
#define TMENU_GETHMENU(data)            ((HMENU)((((DWORD_PTR)(data)) >> 1) & 0x7fffffff))


#ifndef _TOLC_TMENU_DEFINED
#define _TOLC_TMENU_DEFINED
typedef PVoid TMENU;
#endif /*_TOLC_TMENU_DEFINED*/


TOLCDECL TMENU TOLAPI MenuOD_Make();
TOLCDECL void TOLAPI MenuOD_Kill(TMENU tMenu);

TOLCDECL void TOLAPI MenuOD_SetOwnerDraw(HMENU hMenu, Bool bOwnerDraw, Bool bBarMenu);
TOLCDECL void TOLAPI MenuOD_SettingChange(TMENU tMenu, UINT uFlags, LPCTSTR lpszSection);

TOLCDECL int TOLAPI MenuOD_GetImageCmds(TMENU tMenu, PUint puCmds);
TOLCDECL Bool TOLAPI MenuOD_SetImageCmds(TMENU tMenu, PUint puCmds, int nCount);

TOLCDECL HIMAGELIST TOLAPI MenuOD_GetImageList(TMENU tMenu, Uint uType);
TOLCDECL Bool TOLAPI MenuOD_SetImageList(TMENU tMenu, HIMAGELIST hIml, Uint uType);

TOLCDECL int TOLAPI MenuOD_GetStyle(TMENU tMenu);
TOLCDECL Bool TOLAPI MenuOD_SetStyle(TMENU tMenu, int nStyle);

TOLCDECL void TOLAPI MenuOD_Menu(TMENU tMenu, HMENU hParentMenu, int nPos, UINT uFirstID, HINSTANCE hTextInst, UINT uTextID);

TOLCDECL void TOLAPI MenuOD_Measure(TMENU tMenu, LPMEASUREITEMSTRUCT pmis);
TOLCDECL void TOLAPI MenuOD_Draw(TMENU tMenu, LPDRAWITEMSTRUCT pdis);


TOLEXTC_END

#endif /* _INC_TOLC_MENU_OD_H */
