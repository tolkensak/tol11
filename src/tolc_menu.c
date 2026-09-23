
#include "stdafx.h"
#include <tolc_string.h>


TOLEXTC_BEGIN


TOLCDECL Bool TOLAPI Menu_CopyItem(HMENU hmDest, int nPos, HMENU hmSrc, Uint uID, Bool bLoseShortKey)
{
	MENUITEMINFO mii;
	TCHAR pch[TOL_MAXSTR];

	if(!hmDest || !hmSrc)
		return FALSE;

	mii.cbSize=sizeof(mii);
	mii.fMask=MIIM_FTYPE|MIIM_BITMAP|MIIM_CHECKMARKS|MIIM_DATA|MIIM_ID|MIIM_STATE|MIIM_STRING|MIIM_SUBMENU;
	mii.dwTypeData=pch;
	mii.cch=TOL_MAXSTR-1;

	if(nPos<0)
		nPos=GetMenuItemCount(hmDest);

	if(!GetMenuItemInfo(hmSrc, uID, FALSE, &mii))
		return FALSE;

	if(bLoseShortKey)
		Str_Token(pch, _T("\t"));

	return InsertMenuItem(hmDest, nPos, TRUE, &mii);
}

TOLCDECL Bool TOLAPI Menu_Copy(HMENU hmDest, int nPos, HMENU hmSrc)
{
	int i, n;
	MENUITEMINFO mii;
	TCHAR pch[TOL_MAXSTR];

	if(!hmDest || !hmSrc)
		return FALSE;

	n=GetMenuItemCount(hmSrc);
	if(!n)
		return FALSE;

	if(nPos<0)
		nPos=GetMenuItemCount(hmDest);

	mii.cbSize=sizeof(mii);
	mii.fMask=MIIM_FTYPE|MIIM_BITMAP|MIIM_CHECKMARKS|MIIM_DATA|MIIM_ID|MIIM_STATE|MIIM_STRING|MIIM_SUBMENU;
	mii.dwTypeData=pch;

	for(i=0; i<n; i++)
	{
		mii.cch=TOL_MAXSTR-1;
		if(GetMenuItemInfo(hmSrc, i, TRUE, &mii) && InsertMenuItem(hmDest, nPos, TRUE, &mii))
			nPos++;
	}

	return TRUE;
}

TOLCDECL Bool TOLAPI Menu_IsItemChecked(HWND hWnd, Uint uID)
{
	MENUITEMINFO mii;
	mii.cbSize=sizeof(MENUITEMINFO);
	mii.fMask=MIIM_STATE;
	GetMenuItemInfo(GetMenu(hWnd), uID, FALSE, &mii);
	return mii.fState&MFS_CHECKED;
}

TOLCDECL HMENU TOLAPI Menu_GetParent(HMENU hMenu, Uint uID, Bool bByID)
{
	if(hMenu && uID)
	{
		int i, n;
		HMENU hm;
		MENUITEMINFO mii;

		mii.cbSize=sizeof(MENUITEMINFO);
		mii.fMask=MIIM_ID|MIIM_SUBMENU;

		n=GetMenuItemCount(hMenu);

		for(i=0; i<n; i++)
		{
			GetMenuItemInfo(hMenu, i, TRUE, &mii);

			if(bByID)
			{
				if(mii.wID==uID)
					return hMenu;
			}
			else
			{
				if(mii.hSubMenu==(HMENU)(INT_PTR)uID)
					return hMenu;
			}

			if(mii.hSubMenu)
			{
				hm=Menu_GetParent(mii.hSubMenu, uID, bByID);
				if(hm)
					return hm;
			}
		}
	}

	return NULL;
}

TOLCDECL Bool TOLAPI Menu_ShowUnderlineLetter()
{
	BOOL bShowUnderlineLetter=FALSE;
	SystemParametersInfo(SPI_GETMENUUNDERLINES, 0, (PVOID)&bShowUnderlineLetter, 0);
	return bShowUnderlineLetter;
}


TOLEXTC_END
