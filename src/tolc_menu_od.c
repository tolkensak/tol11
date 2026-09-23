
#include "stdafx.h"
#include <tolc_string.h>
#include <tolc_memory.h>
#include <tolc_draw.h>

#include <commctrl.h> /* ImageList */


TOLEXTC_BEGIN

typedef struct tagMenu
{
	HFONT hFont;
	HIMAGELIST hImlNormal;
	HIMAGELIST hImlDisabled;
	HIMAGELIST hImlHot;
	SIZE szImage;
	PUint puCmds;
	int nCmdsCount;
	int nStyle;
	UINT uDrawTextFormat;
} T_MENU, *TMENU;

#define _TOLC_TMENU_DEFINED

TOLEXTC_END


#include <tolc_menu_od.h>


TOLEXTC_BEGIN

const PCChar TMENU_STYLENAME[TMENU_STYLECOUNT]=
{
	_T("Windows XP"),
	_T("Office XP")
};

TOLEXTC_END


/* windows xp style */

#define TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_LEFT    2
#define TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_RIGHT   2
#define TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_TOP     2
#define TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_BOTTOM  2

#define TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_LEFT     8
#define TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_RIGHT   18
#define TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_TOP      2
#define TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_BOTTOM   2

#define TMENU_STYLE_WINDOWSXP_SEPARATOR_HEIGHT     9
#define TMENU_STYLE_WINDOWSXP_CHECK_MARK_WEIGHT    3


TOLEXTC_BEGIN

void TOLAPI MenuOD_MeasureWindowsXP(TMENU tMenu, LPMEASUREITEMSTRUCT pmis);
void TOLAPI MenuOD_DrawWindowsXP(TMENU tMenu, LPDRAWITEMSTRUCT pdis);

TOLEXTC_END


/* office xp style */

#define TMENU_STYLE_OFFICEXP_PADDING_LEFT             1
#define TMENU_STYLE_OFFICEXP_PADDING_RIGHT            1
#define TMENU_STYLE_OFFICEXP_PADDING_TOP              2
#define TMENU_STYLE_OFFICEXP_PADDING_BOTTOM           2

#define TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_LEFT        2
#define TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_RIGHT       4
#define TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_TOP         2
#define TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_BOTTOM      2

#define TMENU_STYLE_OFFICEXP_TEXT_MARGIN_LEFT         8
#define TMENU_STYLE_OFFICEXP_TEXT_MARGIN_RIGHT       12
#define TMENU_STYLE_OFFICEXP_TEXT_MARGIN_TOP          2
#define TMENU_STYLE_OFFICEXP_TEXT_MARGIN_BOTTOM       2

#define TMENU_STYLE_OFFICEXP_SEPARATOR_HEIGHT         5
#define TMENU_STYLE_OFFICEXP_CHECK_MARK_WEIGHT        2

#define TMENU_STYLE_OFFICEXP_SELECT_COLOR_SHIFT     0.7f
#define TMENU_STYLE_OFFICEXP_CHECK_MARK_COLOR_SHIFT 0.8f
#define TMENU_STYLE_OFFICEXP_SEPARATOR_COLOR_SHIFT  0.4f


TOLEXTC_BEGIN

void TOLAPI MenuOD_MeasureOfficeXP(TMENU tMenu, LPMEASUREITEMSTRUCT pmis);
void TOLAPI MenuOD_DrawOfficeXP(TMENU tMenu, LPDRAWITEMSTRUCT pdis);

TOLEXTC_END


TOLEXTC_BEGIN

/* private functions */

HIMAGELIST TOLAPI MenuOD_GetDisabledImageList(TMENU tMenu)
{
	if(tMenu)
		return tMenu->hImlDisabled?tMenu->hImlDisabled:tMenu->hImlNormal;

	return NULL;
}

HIMAGELIST TOLAPI MenuOD_GetHotImageList(TMENU tMenu)
{
	if(tMenu)
		return tMenu->hImlHot?tMenu->hImlHot:tMenu->hImlNormal;

	return NULL;
}

int TOLAPI MenuOD_HasItemImage(TMENU tMenu, UINT uID)
{
	if(tMenu && tMenu->puCmds && tMenu->nCmdsCount)
	{
		int i;
		for(i=0; i<tMenu->nCmdsCount; i++)
			if(tMenu->puCmds[i]==uID)
				return i;
	}

	return -1;
}

void TOLAPI MenuOD_CreateFont(TMENU tMenu)
{
	NONCLIENTMETRICS ncm;
	ncm.cbSize=sizeof(NONCLIENTMETRICS);

	if(tMenu->hFont)
	{
		DeleteObject(tMenu->hFont);
		tMenu->hFont=NULL;
	}

	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
	tMenu->hFont=CreateFontIndirect(&ncm.lfMenuFont);
}


/* public functions */

TOLCDECL TMENU TOLAPI MenuOD_Make()
{
	TMENU tMenu=(TMENU)Mem_Alloc(MEM_ZERO_MEMORY, sizeof(T_MENU));
	if(tMenu)
	{
		/*
		tMenu->hFont=NULL;
		tMenu->hIml=NULL;
		tMenu->puCmds=NULL;
		tMenu->nCmdsCount=0;
		*/

		tMenu->szImage.cx=16;
		tMenu->szImage.cy=16;

		MenuOD_CreateFont(tMenu);
		MenuOD_SetStyle(tMenu, TMENU_STYLE_WINDOWSXP);
		tMenu->uDrawTextFormat=/*(MenuOD_ShowUnderlineLetter()?0:DT_HIDEPREFIX)|*/DT_SINGLELINE|DT_VCENTER;
	}

	return tMenu;
}

TOLCDECL void TOLAPI MenuOD_Kill(TMENU tMenu)
{
	if(tMenu)
	{
		if(tMenu->hFont)
			DeleteObject(tMenu->hFont);

		Mem_Free(tMenu);
	}
}

TOLCDECL void TOLAPI MenuOD_SetOwnerDraw(HMENU hMenu, Bool bOwnerDraw, Bool bBarMenu)
{
	if(hMenu)
	{
		int i, n;
		MENUITEMINFO mii;

		mii.cbSize=sizeof(MENUITEMINFO);
		n=GetMenuItemCount(hMenu);

		for(i=0; i<n; i++)
		{
			mii.fMask=MIIM_SUBMENU|MIIM_FTYPE;
			GetMenuItemInfo(hMenu, i, TRUE, &mii);

			if(bOwnerDraw)
			{
				mii.fType|=MFT_OWNERDRAW;
				mii.dwItemData=TMENU_MAKEDATA(bBarMenu, hMenu);
			}
			else
			{
				mii.fType&=~MFT_OWNERDRAW;
				mii.dwItemData=0;
			}

			mii.fMask=MIIM_FTYPE|MIIM_DATA;
			SetMenuItemInfo(hMenu, i, TRUE, &mii);

			if(mii.hSubMenu)
				MenuOD_SetOwnerDraw(mii.hSubMenu, bOwnerDraw, FALSE);
		}
	}
}

TOLCDECL void TOLAPI MenuOD_SettingChange(TMENU tMenu, UINT uFlags, LPCTSTR lpszSection)
{
	if(tMenu)
	{
		if(uFlags==SPI_SETNONCLIENTMETRICS)
			MenuOD_CreateFont(tMenu);

		//if(uFlags==SPI_SETMENUUNDERLINES)
		//{
		//	if(MenuOD_ShowUnderlineLetter())
		//		tMenu->uDrawTextFormat&=~DT_HIDEPREFIX;
		//	else
		//		tMenu->uDrawTextFormat|=DT_HIDEPREFIX;
		//}
	}
}

TOLCDECL int TOLAPI MenuOD_GetImageCmds(TMENU tMenu, PUint puCmds)
{
	if(tMenu)
	{
		puCmds=tMenu->puCmds;
		return tMenu->nCmdsCount;
	}

	return 0;
}

TOLCDECL Bool TOLAPI MenuOD_SetImageCmds(TMENU tMenu, PUint puCmds, int nCount)
{
	if(tMenu && puCmds && nCount>=0)
	{
		tMenu->puCmds=puCmds;
		tMenu->nCmdsCount=nCount;
		return TRUE;
	}

	return FALSE;
}

TOLCDECL HIMAGELIST TOLAPI MenuOD_GetImageList(TMENU tMenu, Uint uType)
{
	if(tMenu)
	{
		switch(uType)
		{
		case TMENU_IMAGELIST_NORMAL:
			return tMenu->hImlNormal;

		case TMENU_IMAGELIST_DISABLED:
			return tMenu->hImlDisabled;

		case TMENU_IMAGELIST_HOT:
			return tMenu->hImlHot;
		}
	}

	return NULL;
}

TOLCDECL Bool TOLAPI MenuOD_SetImageList(TMENU tMenu, HIMAGELIST hIml, Uint uType)
{
	if(!tMenu)
		return FALSE;

	switch(uType)
	{
	case TMENU_IMAGELIST_NORMAL:
		tMenu->hImlNormal=hIml;
		break;

	case TMENU_IMAGELIST_DISABLED:
		tMenu->hImlDisabled=hIml;
		break;

	case TMENU_IMAGELIST_HOT:
		tMenu->hImlHot=hIml;
		break;

	default:
		return FALSE;
	}

	if(hIml)
	{
		int cx, cy;
		if(ImageList_GetIconSize(hIml, &cx, &cy))
		{
			if(cx>tMenu->szImage.cx)
				tMenu->szImage.cx=cx;

			if(cy>tMenu->szImage.cy)
				tMenu->szImage.cy=cy;
		}
	}

	return TRUE;
}

TOLCDECL int TOLAPI MenuOD_GetStyle(TMENU tMenu)
{
	return tMenu?tMenu->nStyle:-1;
}

TOLCDECL Bool TOLAPI MenuOD_SetStyle(TMENU tMenu, int nStyle)
{
	if(tMenu && tMenu->nStyle!=nStyle)
	{
		switch(nStyle)
		{
		case TMENU_STYLE_WINDOWSXP:
		case TMENU_STYLE_OFFICEXP:
			tMenu->nStyle=nStyle;
			return TRUE;
		}
	}

	return FALSE;
}

TOLCDECL void TOLAPI MenuOD_Menu(TMENU tMenu, HMENU hParentMenu, int nPos, UINT uFirstID, HINSTANCE hTextInst, UINT uTextID)
{
	if(tMenu && hParentMenu)
	{
		HMENU hMenu=CreatePopupMenu();
		if(hMenu)
		{
			int i;
			Char pchText[TOL_MAXSTR];

			for(i=0; i<TMENU_STYLECOUNT; i++)
				AppendMenu(hMenu, MF_STRING|MF_BYPOSITION, uFirstID+i, TMENU_STYLENAME[i]);

			if(!hTextInst || !LoadString(hTextInst, uTextID, pchText, TOL_MAXSTR))
				Str_Copy(pchText, _T("&Menu Styles"));

			InsertMenu(hParentMenu, nPos, MF_BYPOSITION|MF_POPUP|MF_STRING, (UINT_PTR)hMenu, pchText);
		}
	}
}

TOLCDECL void TOLAPI MenuOD_Measure(TMENU tMenu, LPMEASUREITEMSTRUCT pmis)
{
	if(tMenu)
	{
		switch(tMenu->nStyle)
		{
		case TMENU_STYLE_WINDOWSXP:
			MenuOD_MeasureWindowsXP(tMenu, pmis);
			break;

		case TMENU_STYLE_OFFICEXP:
			MenuOD_MeasureOfficeXP(tMenu, pmis);
			break;
		}
	}
}

TOLCDECL void TOLAPI MenuOD_Draw(TMENU tMenu, LPDRAWITEMSTRUCT pdis)
{
	if(tMenu)
	{
		switch(tMenu->nStyle)
		{
		case TMENU_STYLE_WINDOWSXP:
			MenuOD_DrawWindowsXP(tMenu, pdis);
			break;

		case TMENU_STYLE_OFFICEXP:
			MenuOD_DrawOfficeXP(tMenu, pdis);
			break;
		}
	}
}


void TOLAPI MenuOD_MeasureWindowsXP(TMENU tMenu, LPMEASUREITEMSTRUCT pmis)
{
	static TCHAR pchText[TOL_MAXSTR];

	HDC hDC;
	SIZE sz;
	HMENU hMenu;
	PChar pcText;
	HGDIOBJ hOldFont;
	MENUITEMINFO mii;

	if(!pmis->itemID)
	{
		pmis->itemHeight=TMENU_STYLE_WINDOWSXP_SEPARATOR_HEIGHT;
		return;
	}

	hMenu=TMENU_GETHMENU(pmis->itemData);
	mii.cbSize=sizeof(MENUITEMINFO);
	mii.fMask=MIIM_STRING;
	mii.dwTypeData=pchText;
	mii.cch=TOL_MAXSTR-1;
	GetMenuItemInfo(hMenu, pmis->itemID, FALSE, &mii);

	pcText=Str_Dup(pchText);
	Str_RemoveCh(pcText, _T('&'));

	hDC=GetDC(NULL);
	hOldFont=SelectObject(hDC, tMenu->hFont);
	GetTextExtentPoint32(hDC, pcText, Str_Len(pcText), &sz);
	SelectObject(hDC, hOldFont);
	ReleaseDC(NULL, hDC);

	Str_Kill(pcText);

	pmis->itemWidth=sz.cx;
	//pmis->itemHeight=sz.cy;

	if(!TMENU_GETBARMENU(pmis->itemData))
	{ /* menu item */
		UINT uHeightImage;

		pmis->itemWidth+=tMenu->szImage.cx+
			TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_LEFT+
			TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_RIGHT+
			TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_LEFT+
			TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_RIGHT;

		pmis->itemHeight=sz.cy+
			TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_TOP+
			TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_BOTTOM;

		uHeightImage=tMenu->szImage.cy+
			TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_TOP+
			TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_BOTTOM;

		if(uHeightImage>pmis->itemHeight)
			pmis->itemHeight=uHeightImage;
	}
}

void TOLAPI MenuOD_DrawWindowsXP(TMENU tMenu, LPDRAWITEMSTRUCT pdis)
{
	HDC hDC=pdis->hDC;
	LPRECT pRect=&pdis->rcItem;
	HMENU hMenu=TMENU_GETHMENU(pdis->itemData);

	if(pdis->itemID)
	{
		static TCHAR pchText[TOL_MAXSTR];

		int cchText;
		int nSaveDC;
		MENUITEMINFO mii;

		mii.cbSize=sizeof(MENUITEMINFO);
		mii.fMask=MIIM_STRING;
		mii.dwTypeData=pchText;
		mii.cch=TOL_MAXSTR-1;
		GetMenuItemInfo(hMenu, pdis->itemID, FALSE, &mii);
		cchText=mii.cch;

		nSaveDC=SaveDC(hDC);
		SetBkMode(hDC, TRANSPARENT);

		if(TMENU_GETBARMENU(pdis->itemData))
		{ /* bar menu */
			if(pdis->itemState & ODS_GRAYED)
			{
				if(pdis->itemState & ODS_SELECTED)
				{
					InflateRect(pRect, -1, -1);
					FillRect(hDC, pRect, GetSysColorBrush(COLOR_HIGHLIGHT));
					InflateRect(pRect, 1, 1);
				}
				else
					FillRect(hDC, pRect, GetSysColorBrush(COLOR_BTNFACE));

				//SetTextColor(hDC, GetSysColor(COLOR_BTNHIGHLIGHT));
				//OffsetRect(pRect, 1, 0);
				//DrawText(hDC, pchText, cchText, pRect, tMenu->uDrawTextFormat|DT_CENTER);
				//OffsetRect(pRect, -1, 0);
				SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT));
			}
			else if(pdis->itemState & ODS_SELECTED)
			{
				InflateRect(pRect, -1, -1);
				FillRect(hDC, pRect, GetSysColorBrush(COLOR_HIGHLIGHT));
				InflateRect(pRect, 1, 1);
				SetTextColor(hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			}
			else
			{
				FillRect(hDC, pRect, GetSysColorBrush(COLOR_BTNFACE));
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}

			DrawText(hDC, pchText, cchText, pRect, tMenu->uDrawTextFormat|DT_CENTER);
		}
		else
		{ /* menu item */
			RECT rc;
			int nIndex;

			/* text */

			rc=*pRect;
			rc.left+=TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_LEFT+
				tMenu->szImage.cx+
				TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_RIGHT+
				TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_LEFT;
			rc.right-=TMENU_STYLE_WINDOWSXP_TEXT_MARGIN_RIGHT;

			if(pdis->itemState & ODS_GRAYED)
			{
				if(pdis->itemState & ODS_SELECTED)
					FillRect(hDC, pRect, GetSysColorBrush(COLOR_HIGHLIGHT));
				else
					FillRect(hDC, pRect, GetSysColorBrush(COLOR_MENU));

				//SetTextColor(hDC, GetSysColor(COLOR_BTNHIGHLIGHT));
				//OffsetRect(&rc, 1, 0);
				//Draw_MenuText(hDC, &rc, pchText, tMenu->uDrawTextFormat);
				//OffsetRect(&rc, -1, 0);
				SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT));
			}
			else if(pdis->itemState & ODS_SELECTED)
			{
				FillRect(hDC, pRect, GetSysColorBrush(COLOR_HIGHLIGHT));
				SetTextColor(hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			}
			else
			{
				FillRect(hDC, pRect, GetSysColorBrush(COLOR_MENU));
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}

			Draw_MenuText(hDC, &rc, pchText, tMenu->uDrawTextFormat);

			/* image */

			rc=*pRect;

			rc.left+=TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_LEFT;
			rc.right=rc.left+tMenu->szImage.cx;

			rc.top+=TMENU_STYLE_WINDOWSXP_IMAGE_MARGIN_TOP;
			rc.bottom=rc.top+tMenu->szImage.cy;

			nIndex=MenuOD_HasItemImage(tMenu, pdis->itemID);
			if(nIndex!=-1)
			{
				if(pdis->itemState&ODS_CHECKED)
					DrawEdge(hDC, &rc, EDGE_SUNKEN, BF_RECT);

				if(pdis->itemState&ODS_GRAYED)
					ImageList_Draw(MenuOD_GetDisabledImageList(tMenu), nIndex, hDC, rc.left, rc.top, 0);
					//Draw_GrayBitmap(hDC, &rc, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_MENU));
				else if(pdis->itemState&ODS_SELECTED)
					ImageList_Draw(MenuOD_GetHotImageList(tMenu), nIndex, hDC, rc.left, rc.top, 0);
					//Draw_Bitmap(hDC, &rc, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_HIGHLIGHT));
				else
					ImageList_Draw(tMenu->hImlNormal, nIndex, hDC, rc.left, rc.top, 0);
					//Draw_Bitmap(hDC, &rc, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_MENU));
			}
			else if(pdis->itemState&ODS_CHECKED)
				Draw_CheckMark(hDC, &rc, (pdis->itemState&ODS_SELECTED)?GetSysColor(COLOR_HIGHLIGHTTEXT):GetSysColor(COLOR_MENUTEXT), TMENU_STYLE_WINDOWSXP_CHECK_MARK_WEIGHT);
		}

		RestoreDC(hDC, nSaveDC);
	}
	else
	{ /* separator */
		int nVCenter;
		HPEN hPen, hPenOld;

		nVCenter=(pRect->top+pRect->bottom)/2;
		FillRect(hDC, pRect, GetSysColorBrush(COLOR_MENU));
		hPen=CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
		hPenOld=(HPEN)SelectObject(hDC, hPen);
		MoveToEx(hDC, pRect->left, nVCenter, NULL);
		LineTo(hDC, pRect->right, nVCenter);
		SelectObject(hDC, hPenOld);
		DeleteObject(hPen);
	}
}

void TOLAPI MenuOD_MeasureOfficeXP(TMENU tMenu, LPMEASUREITEMSTRUCT pmis)
{
	static TCHAR pchText[TOL_MAXSTR];

	HDC hDC;
	SIZE sz;
	HMENU hMenu;
	PChar pcText;
	HGDIOBJ hOldFont;
	MENUITEMINFO mii;

	if(!pmis->itemID)
	{
		pmis->itemHeight=TMENU_STYLE_OFFICEXP_SEPARATOR_HEIGHT;
		return;
	}

	hMenu=TMENU_GETHMENU(pmis->itemData);
	mii.cbSize=sizeof(MENUITEMINFO);
	mii.fMask=MIIM_STRING;
	mii.dwTypeData=pchText;
	mii.cch=TOL_MAXSTR-1;
	GetMenuItemInfo(hMenu, pmis->itemID, FALSE, &mii);

	pcText=Str_Dup(pchText);
	Str_RemoveCh(pcText, _T('&'));

	hDC=GetDC(NULL);
	hOldFont=SelectObject(hDC, tMenu->hFont);
	GetTextExtentPoint32(hDC, pcText, Str_Len(pcText), &sz);
	SelectObject(hDC, hOldFont);
	ReleaseDC(NULL, hDC);

	Str_Kill(pcText);

	pmis->itemWidth=sz.cx;
	//pmis->itemHeight=sz.cy;

	if(!TMENU_GETBARMENU(pmis->itemData))
	{
		UINT uHeightImage;

		pmis->itemWidth+=tMenu->szImage.cx+
			TMENU_STYLE_OFFICEXP_PADDING_LEFT+
			TMENU_STYLE_OFFICEXP_PADDING_RIGHT+
			TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_LEFT+
			TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_RIGHT+
			TMENU_STYLE_OFFICEXP_TEXT_MARGIN_LEFT+
			TMENU_STYLE_OFFICEXP_TEXT_MARGIN_RIGHT;

		//if(pmis->itemData)
		//{
		//	BITMAP bm;
		//	GetObject((HBITMAP)pmis->itemData, sizeof(BITMAP), &bm);
		//	sz.cy=max(bm.bmWidth, bm.bmHeight)+2;
		//}

		pmis->itemHeight=sz.cy+
			TMENU_STYLE_OFFICEXP_TEXT_MARGIN_TOP+
			TMENU_STYLE_OFFICEXP_TEXT_MARGIN_BOTTOM;

		uHeightImage=tMenu->szImage.cy+
			TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_TOP+
			TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_BOTTOM;

		if(uHeightImage>pmis->itemHeight)
			pmis->itemHeight=uHeightImage;

		pmis->itemHeight+=TMENU_STYLE_OFFICEXP_PADDING_TOP+
			TMENU_STYLE_OFFICEXP_PADDING_BOTTOM;
	}
}

void TOLAPI MenuOD_DrawOfficeXPFillSelectRect(HDC hDC, LPRECT pRect, float fColorShift)
{
	HBRUSH hbr;
	COLORREF cr;

	cr=GetSysColor(COLOR_HIGHLIGHT);
	cr=RGB(COLOR_SHIFT(GetRValue(cr), fColorShift), COLOR_SHIFT(GetGValue(cr), fColorShift), COLOR_SHIFT(GetBValue(cr), fColorShift));
	hbr=CreateSolidBrush(cr);
	FillRect(hDC, pRect, hbr);
	DeleteObject(hbr);
}

void TOLAPI MenuOD_DrawOfficeXP(TMENU tMenu, LPDRAWITEMSTRUCT pdis)
{
	HDC hDC=pdis->hDC;
	LPRECT pRect=&pdis->rcItem;
	HMENU hMenu=TMENU_GETHMENU(pdis->itemData);

	if(pdis->itemID)
	{
		static TCHAR pchText[TOL_MAXSTR];

		int cchText;
		int nSaveDC;
		MENUITEMINFO mii;

		mii.cbSize=sizeof(MENUITEMINFO);
		mii.fMask=MIIM_STRING;
		mii.dwTypeData=pchText;
		mii.cch=TOL_MAXSTR-1;
		GetMenuItemInfo(hMenu, pdis->itemID, FALSE, &mii);
		cchText=mii.cch;

		nSaveDC=SaveDC(hDC);
		SetBkMode(hDC, TRANSPARENT);

		if(TMENU_GETBARMENU(pdis->itemData))
		{ /* bar menu */
			if(pdis->itemState & ODS_GRAYED)
			{
				FillRect(hDC, pRect, GetSysColorBrush(COLOR_BTNFACE));
				SetTextColor(hDC, GetSysColor(COLOR_BTNHIGHLIGHT));
				OffsetRect(pRect, 1, 0);
				DrawText(hDC, pchText, cchText, pRect, tMenu->uDrawTextFormat|DT_CENTER);
				OffsetRect(pRect, -1, 0);
				SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT));
			}
			else if(pdis->itemState & ODS_SELECTED)
			{
				InflateRect(pRect, -1, -1);
				MenuOD_DrawOfficeXPFillSelectRect(hDC, pRect, TMENU_STYLE_OFFICEXP_SELECT_COLOR_SHIFT);
				FrameRect(hDC, pRect, GetSysColorBrush(COLOR_HIGHLIGHT));
				InflateRect(pRect, 1, 1);
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}
			else
			{
				FillRect(hDC, pRect, GetSysColorBrush(COLOR_BTNFACE));
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}

			DrawText(hDC, pchText, cchText, pRect, tMenu->uDrawTextFormat|DT_CENTER);
		}
		else
		{ /* menu item */
			int nIndex;
			RECT rc, rcLeft, rcRight, rcImage;

			rcLeft=*pRect;
			rcLeft.right=rcLeft.left+tMenu->szImage.cx+
				TMENU_STYLE_OFFICEXP_PADDING_LEFT+
				TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_LEFT+
				TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_RIGHT;

			rcRight=*pRect;
			rcRight.left=rcLeft.right;

			/* text */

			rc=rcRight;
			rc.left+=TMENU_STYLE_OFFICEXP_TEXT_MARGIN_LEFT;
			rc.top+=TMENU_STYLE_OFFICEXP_PADDING_TOP+TMENU_STYLE_OFFICEXP_TEXT_MARGIN_TOP;
			rc.right-=TMENU_STYLE_OFFICEXP_PADDING_RIGHT+TMENU_STYLE_OFFICEXP_TEXT_MARGIN_RIGHT;
			rc.bottom-=TMENU_STYLE_OFFICEXP_PADDING_BOTTOM+TMENU_STYLE_OFFICEXP_TEXT_MARGIN_BOTTOM;

			if(pdis->itemState & ODS_GRAYED)
			{
				FillRect(hDC, &rcLeft, GetSysColorBrush(COLOR_BTNFACE));
				FillRect(hDC, &rcRight, GetSysColorBrush(COLOR_MENU));
				SetTextColor(hDC, GetSysColor(COLOR_BTNHIGHLIGHT));
				OffsetRect(&rc, 1, 0);
				Draw_MenuText(hDC, &rc, pchText, tMenu->uDrawTextFormat);
				OffsetRect(&rc, -1, 0);
				SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT));
			}
			else if(pdis->itemState & ODS_SELECTED)
			{
				RECT rcSel;

				rcSel=*pRect;
				rcSel.top+=1;//TMENU_STYLE_OFFICEXP_PADDING_TOP;
				rcSel.bottom-=1;//TMENU_STYLE_OFFICEXP_PADDING_BOTTOM;

				MenuOD_DrawOfficeXPFillSelectRect(hDC, &rcSel, TMENU_STYLE_OFFICEXP_SELECT_COLOR_SHIFT);
				FrameRect(hDC, &rcSel, GetSysColorBrush(COLOR_HIGHLIGHT));
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}
			else
			{
				FillRect(hDC, &rcLeft, GetSysColorBrush(COLOR_BTNFACE));
				FillRect(hDC, &rcRight, GetSysColorBrush(COLOR_MENU));
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}

			Draw_MenuText(hDC, &rc, pchText, tMenu->uDrawTextFormat);


			/* image */

			rcImage=rcLeft;
			rcImage.left+=TMENU_STYLE_OFFICEXP_PADDING_LEFT;
			rcImage.top+=TMENU_STYLE_OFFICEXP_PADDING_TOP;
			rcImage.right-=TMENU_STYLE_OFFICEXP_PADDING_RIGHT;
			rcImage.bottom-=TMENU_STYLE_OFFICEXP_PADDING_BOTTOM;

			rc=rcImage;
			rc.left+=TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_LEFT;
			rc.top+=TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_TOP;
			rc.right-=TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_RIGHT;
			rc.bottom-=TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_BOTTOM;

			nIndex=MenuOD_HasItemImage(tMenu, pdis->itemID);
			if(nIndex!=-1)
			{
				if(pdis->itemState&ODS_CHECKED)
				{
					InflateRect(&rcImage, -1, -1);
					MenuOD_DrawOfficeXPFillSelectRect(hDC, &rcImage, TMENU_STYLE_OFFICEXP_CHECK_MARK_COLOR_SHIFT);
					FrameRect(hDC, &rcImage, GetSysColorBrush(COLOR_HIGHLIGHT));
					InflateRect(&rcImage, 1, 1);
				}

				if(pdis->itemState&ODS_GRAYED)
					ImageList_Draw(MenuOD_GetDisabledImageList(tMenu), nIndex, hDC, rc.left, rc.top, 0);
					//Draw_GrayBitmap(hDC, &rcImage, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_BTNFACE));
				else if(pdis->itemState&ODS_SELECTED)
					ImageList_Draw(MenuOD_GetHotImageList(tMenu), nIndex, hDC, rc.left, rc.top, 0);
					//Draw_Bitmap(hDC, &rcImage, (HBITMAP)mii.dwItemData, crLight);
				else
					ImageList_Draw(tMenu->hImlNormal, nIndex, hDC, rc.left, rc.top, 0);
					//Draw_Bitmap(hDC, &rcImage, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_BTNFACE));
			}
			else if(pdis->itemState&ODS_CHECKED)
			{
				InflateRect(&rcImage, -1, -1);
				MenuOD_DrawOfficeXPFillSelectRect(hDC, &rcImage, TMENU_STYLE_OFFICEXP_CHECK_MARK_COLOR_SHIFT);
				FrameRect(hDC, &rcImage, GetSysColorBrush(COLOR_HIGHLIGHT));
				InflateRect(&rcImage, 1, 1);
				Draw_CheckMark(hDC, &rcImage, GetSysColor(COLOR_MENUTEXT), TMENU_STYLE_OFFICEXP_CHECK_MARK_WEIGHT);
			}
		}

		RestoreDC(hDC, nSaveDC);
	}
	else
	{ /* separator */
		HPEN hPen;
		COLORREF cr;
		int nVCenter;
		HGDIOBJ hPenOld;
		RECT rc, rcLeft, rcRight;

		nVCenter=(pRect->top+pRect->bottom)/2;

		rcLeft=*pRect;
		rcLeft.right=rcLeft.left+tMenu->szImage.cx+
			TMENU_STYLE_OFFICEXP_PADDING_LEFT+
			TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_LEFT+
			TMENU_STYLE_OFFICEXP_IMAGE_MARGIN_RIGHT;

		rcRight=*pRect;
		rcRight.left=rcLeft.right;

		FillRect(hDC, &rcLeft, GetSysColorBrush(COLOR_BTNFACE));
		FillRect(hDC, &rcRight, GetSysColorBrush(COLOR_MENU));

		rc=rcRight;
		rc.left+=TMENU_STYLE_OFFICEXP_TEXT_MARGIN_LEFT;

		cr=GetSysColor(COLOR_BTNSHADOW);
		cr=RGB(COLOR_SHIFT(GetRValue(cr), TMENU_STYLE_OFFICEXP_SEPARATOR_COLOR_SHIFT), COLOR_SHIFT(GetGValue(cr), TMENU_STYLE_OFFICEXP_SEPARATOR_COLOR_SHIFT), COLOR_SHIFT(GetBValue(cr), TMENU_STYLE_OFFICEXP_SEPARATOR_COLOR_SHIFT));
		hPen=CreatePen(PS_SOLID, 1, cr);
		hPenOld=SelectObject(hDC, hPen);
		MoveToEx(hDC, rc.left, nVCenter, NULL);
		LineTo(hDC, rc.right, nVCenter);
		SelectObject(hDC, hPenOld);
		DeleteObject(hPen);
	}
}


TOLEXTC_END
