
#include "stdafx.h"
#include <tolcpp_menu.h>
#include <tolc_draw.h>


TOLNS_BEGIN


HMENU Menu::GetMenuFromCmdID(HMENU hMenu, UINT uCmdID)
{
	if(!hMenu)
		return NULL;

	HMENU hmSub, hmRes;

	int n=GetMenuItemCount(hMenu);
	for(int i=0; i<n; i++)
	{
		hmSub=GetSubMenu(hMenu, i);
		if(hmSub)
		{
			hmRes=GetMenuFromCmdID(hmSub, uCmdID);
			if(hmRes)
				return hmRes;
		}
		else if(uCmdID==GetMenuItemID(hMenu, i))
			return hMenu;
	}

	return NULL;
}


Menu::Menu()
	: m_hFont(NULL)
{
	m_szImage.cx=16;
	m_szImage.cy=16;

	m_nTextMarginLeft=8;
	m_nTextMarginRight=16;

	ChangeSettings();
}

Menu::~Menu()
{
	if(m_hFont)
		DeleteObject(m_hFont);

	DeleteCmdBms(0, TRUE);
}

BOOL Menu::Attach(HMENU hMenu)
{
	if(m_hMenu || !hMenu)
		return FALSE;

	m_hMenu=hMenu;
	SetOwnerDraw(hMenu, TRUE);
	return TRUE;
}

HMENU Menu::Detach()
{
	HMENU hmRet=m_hMenu;
	m_hMenu=NULL;
	return hmRet;
}

void Menu::SetOwnerDraw(HMENU hMenu, BOOL bTopMenu)
{
	if(hMenu)
	{
		MENUITEMINFO mii;
		mii.cbSize=sizeof(MENUITEMINFO);
		mii.fMask=MIIM_FTYPE|MIIM_DATA;
		mii.fType=MFT_OWNERDRAW;

		LPVOID lpv;
		int n=GetMenuItemCount(hMenu);
		for(int i=0; i<n; i++)
		{
			if(bTopMenu)
				mii.dwItemData=1;
			else if(m_CmdBms.Lookup(GetMenuItemID(hMenu, i), lpv))
				mii.dwItemData=(ULONG_PTR)lpv;
			else
				mii.dwItemData=0;

			SetMenuItemInfo(hMenu, i, TRUE, &mii);
			SetOwnerDraw(GetSubMenu(hMenu, i), FALSE);
		}
	}
}

void Menu::ChangeSettings()
{
	if(m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont=NULL;
	}

	NONCLIENTMETRICS ncm;
	ncm.cbSize=sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
	m_hFont=CreateFontIndirect(&ncm.lfMenuFont);
}

void Menu::CreateCmdBms(PUINT puID, int nCount)
{
	if(puID && nCount>0)
	{
		int i;
		HINSTANCE hInst=AfxGetApp()->m_hInstance;
		HIMAGELIST* phiml=new HIMAGELIST[nCount];
		for(i=0; i<nCount; i++)
			phiml[i]=ImageList_LoadBitmap(hInst, MAKEINTRESOURCE(puID[i]), m_szImage.cx, 0, CLR_NONE);

		WORD w;
		DWORD dw;
		LPVOID lpv;
		HBITMAP hbm;
		HDC hDC=::GetDC(NULL);
		HDC hdcMem=CreateCompatibleDC(hDC);
		HBITMAP hbmOld=(HBITMAP)GetCurrentObject(hdcMem, OBJ_BITMAP);
		POSITION pos=m_CmdBms.GetStartPosition();
		while(pos)
		{
			m_CmdBms.GetNextAssoc(pos, dw, lpv);
			w=(WORD)lpv;
			hbm=CreateCompatibleBitmap(hDC, m_szImage.cx, m_szImage.cy);
			SelectObject(hdcMem, hbm);
			ImageList_Draw(phiml[HIBYTE(w)], LOBYTE(w), hdcMem, 0, 0, ILD_NORMAL);
			m_CmdBms[dw]=hbm;
		}
		SelectObject(hdcMem, hbmOld);
		DeleteDC(hdcMem);
		::ReleaseDC(NULL, hDC);

		for(i=0; i<nCount; i++)
			ImageList_Destroy(phiml[i]);
		delete[] phiml;
	}
	else
		DeleteCmdBms(0);
}

void Menu::DeleteCmdBms(DWORD dwKey, BOOL bDelObj)
{
	LPVOID lpv;
	if(dwKey)
	{
		if(m_CmdBms.Lookup(dwKey, lpv) && lpv)
		{
			if(bDelObj)
				DeleteObject((HBITMAP)lpv);
			m_CmdBms[dwKey]=NULL;
		}
	}
	else
	{
		POSITION pos=m_CmdBms.GetStartPosition();
		while(pos)
		{
			m_CmdBms.GetNextAssoc(pos, dwKey, lpv);
			if(lpv)
			{
				if(bDelObj)
					DeleteObject((HBITMAP)lpv);
				m_CmdBms[dwKey]=NULL;
			}
		}
	}
}

void Menu::Measure(LPMEASUREITEMSTRUCT lpmis, int nSkin)
{
	switch(nSkin)
	{
		case 0: MeasureWindows(lpmis); break;
		case 1: MeasureOfficeXP(lpmis); break;
	}
}

void Menu::Draw(LPDRAWITEMSTRUCT lpdis, int nSkin)
{
	switch(nSkin)
	{
		case 0: DrawWindows(lpdis); break;
		case 1: DrawOfficeXP(lpdis); break;
	}
}

void Menu::MeasureWindows(LPMEASUREITEMSTRUCT lpmis)
{
	if(lpmis->itemID)
	{
		MENUITEMINFO mii;
		mii.cbSize=sizeof(MENUITEMINFO);
		mii.fMask=MIIM_STRING;
		mii.dwTypeData=NULL;
		GetMenuItemInfo(m_hMenu, lpmis->itemID, FALSE, &mii);
		mii.dwTypeData=new TCHAR[++mii.cch];
		GetMenuItemInfo(m_hMenu, lpmis->itemID, FALSE, &mii);

		int j=0;
		LPTSTR lps=new TCHAR[++mii.cch];
		for(UINT i=0; i<mii.cch; i++)
			if(mii.dwTypeData[i]!=_T('&'))
				lps[j++]=mii.dwTypeData[i];
		delete[] mii.dwTypeData;

		SIZE sz;
		HDC hDC=GetDC(NULL);
		HFONT hfnt=(HFONT)SelectObject(hDC, m_hFont);
		GetTextExtentPoint32(hDC, lps, j, &sz);
		delete[] lps;
		SelectObject(hDC, hfnt);
		ReleaseDC(NULL, hDC);

		lpmis->itemWidth=sz.cx;
		if(lpmis->itemData!=1)
		{
			lpmis->itemWidth+=m_szImage.cx+4+m_nTextMarginLeft+m_nTextMarginRight;
			//if(lpmis->itemData)
			//{
			//	BITMAP bm;
			//	GetObject((HBITMAP)lpmis->itemData, sizeof(BITMAP), &bm);
			//	sz.cy=max(bm.bmWidth, bm.bmHeight)+2;
			//}
			lpmis->itemHeight=max(sz.cy, m_szImage.cy+4);
		}
	}
	else
		lpmis->itemHeight=7;
}

void Menu::DrawWindows(LPDRAWITEMSTRUCT lpdis)
{
	HDC hDC=lpdis->hDC;
	LPRECT lprc=&lpdis->rcItem;

	if(lpdis->itemID)
	{
		int nSaveDC=SaveDC(hDC);
		SetBkMode(hDC, TRANSPARENT);

		MENUITEMINFO mii;
		mii.cbSize=sizeof(MENUITEMINFO);
		mii.dwTypeData=NULL;
		mii.fMask=MIIM_STRING;
		GetMenuItemInfo(m_hMenu, lpdis->itemID, FALSE, &mii);
		mii.dwTypeData=new TCHAR[++mii.cch];
		mii.fMask|=MIIM_DATA;
		GetMenuItemInfo(m_hMenu, lpdis->itemID, FALSE, &mii);

		if(mii.dwItemData==1)
		{
			if(lpdis->itemState & ODS_GRAYED)
			{
				FillRect(hDC, lprc, GetSysColorBrush(COLOR_BTNFACE));
				SetTextColor(hDC, GetSysColor(COLOR_BTNHIGHLIGHT));
				OffsetRect(lprc, 1, 0);
				DrawText(hDC, mii.dwTypeData, mii.cch, lprc, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
				OffsetRect(lprc, -1, 0);
				SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT));
			}
			else if(lpdis->itemState & ODS_SELECTED)
			{
				InflateRect(lprc, -1, -1);
				FillRect(hDC, lprc, GetSysColorBrush(COLOR_HIGHLIGHT));
				InflateRect(lprc, 1, 1);
				SetTextColor(hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			}
			else
			{
				FillRect(hDC, lprc, GetSysColorBrush(COLOR_BTNFACE));
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}
			DrawText(hDC, mii.dwTypeData, mii.cch, lprc, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
		else
		{
			RECT rc=*lprc;
			rc.left+=m_szImage.cx+4+m_nTextMarginLeft;
			rc.right-=m_nTextMarginRight;
			if(lpdis->itemState & ODS_GRAYED)
			{
				FillRect(hDC, lprc, GetSysColorBrush(COLOR_MENU));
				SetTextColor(hDC, GetSysColor(COLOR_BTNHIGHLIGHT));
				OffsetRect(&rc, 1, 0);
				DrawMenuText(hDC, &rc, mii.dwTypeData);
				OffsetRect(&rc, -1, 0);
				SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT));
			}
			else if(lpdis->itemState & ODS_SELECTED)
			{
				FillRect(hDC, lprc, GetSysColorBrush(COLOR_HIGHLIGHT));
				SetTextColor(hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			}
			else
			{
				FillRect(hDC, lprc, GetSysColorBrush(COLOR_MENU));
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}
			DrawMenuText(hDC, &rc, mii.dwTypeData);

			rc=*lprc;
			rc.right=rc.left+m_szImage.cx+4;
			if(mii.dwItemData)
			{
				if(lpdis->itemState&ODS_CHECKED)
					DrawEdge(hDC, &rc, EDGE_SUNKEN, BF_RECT);

				if(lpdis->itemState&ODS_GRAYED)
					DrawGrayBitmap(hDC, &rc, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_MENU));
				else if(lpdis->itemState&ODS_SELECTED)
					DrawBitmap(hDC, &rc, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_HIGHLIGHT));
				else
					DrawBitmap(hDC, &rc, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_MENU));
			}
			else if(lpdis->itemState&ODS_CHECKED)
				DrawCheckMark(hDC, &rc, (lpdis->itemState&ODS_SELECTED)?GetSysColor(COLOR_HIGHLIGHTTEXT):GetSysColor(COLOR_MENUTEXT));
		}

		delete[] mii.dwTypeData;
		RestoreDC(hDC, nSaveDC);
	}
	else
	{
		FillRect(hDC, lprc, GetSysColorBrush(COLOR_MENU));
		int nVCenter=(lprc->top+lprc->bottom)/2;
		HPEN hPen=CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
		HPEN hPenOld=(HPEN)SelectObject(hDC, hPen);
		MoveToEx(hDC, lprc->left, nVCenter, NULL);
		LineTo(hDC, lprc->right, nVCenter);
		SelectObject(hDC, hPenOld);
		DeleteObject(hPen);
	}
}

void Menu::MeasureOfficeXP(LPMEASUREITEMSTRUCT lpmis)
{

	if(lpmis->itemID)
	{
		MENUITEMINFO mii;
		mii.cbSize=sizeof(MENUITEMINFO);
		mii.fMask=MIIM_STRING;
		mii.dwTypeData=NULL;
		GetMenuItemInfo(m_hMenu, lpmis->itemID, FALSE, &mii);
		mii.dwTypeData=new TCHAR[++mii.cch];
		GetMenuItemInfo(m_hMenu, lpmis->itemID, FALSE, &mii);

		int j=0;
		LPTSTR lps=new TCHAR[++mii.cch];
		for(UINT i=0; i<mii.cch; i++)
			if(mii.dwTypeData[i]!=_T('&'))
				lps[j++]=mii.dwTypeData[i];
		delete[] mii.dwTypeData;

		SIZE sz;
		HDC hDC=GetDC(NULL);
		HFONT hfnt=(HFONT)SelectObject(hDC, m_hFont);
		GetTextExtentPoint32(hDC, lps, j, &sz);
		delete[] lps;
		SelectObject(hDC, hfnt);
		ReleaseDC(NULL, hDC);

		lpmis->itemWidth=sz.cx;
		if(lpmis->itemData!=1)
		{
			lpmis->itemWidth+=m_szImage.cx+5+m_nTextMarginLeft+m_nTextMarginRight;
			//if(lpmis->itemData)
			//{
			//	BITMAP bm;
			//	GetObject((HBITMAP)lpmis->itemData, sizeof(BITMAP), &bm);
			//	sz.cy=max(bm.bmWidth, bm.bmHeight)+2;
			//}
			lpmis->itemHeight=max(sz.cy, m_szImage.cy+4)+4;
		}
	}
	else
		lpmis->itemHeight=1;
}

void Menu::DrawOfficeXP(LPDRAWITEMSTRUCT lpdis)
{
	HDC hDC=lpdis->hDC;
	LPRECT lprc=&lpdis->rcItem;
	if(lpdis->itemID)
	{
		int nSaveDC=SaveDC(hDC);
		SetBkMode(hDC, TRANSPARENT);

		MENUITEMINFO mii;
		mii.cbSize=sizeof(MENUITEMINFO);
		mii.dwTypeData=NULL;
		mii.fMask=MIIM_STRING;
		GetMenuItemInfo(m_hMenu, lpdis->itemID, FALSE, &mii);
		mii.dwTypeData=new TCHAR[++mii.cch];
		mii.fMask|=MIIM_DATA;
		GetMenuItemInfo(m_hMenu, lpdis->itemID, FALSE, &mii);

		if(mii.dwItemData==1)
		{
			if(lpdis->itemState & ODS_GRAYED)
			{
				FillRect(hDC, lprc, GetSysColorBrush(COLOR_BTNFACE));
				SetTextColor(hDC, GetSysColor(COLOR_BTNHIGHLIGHT));
				OffsetRect(lprc, 1, 0);
				DrawText(hDC, mii.dwTypeData, mii.cch, lprc, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
				OffsetRect(lprc, -1, 0);
				SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT));
			}
			else if(lpdis->itemState & ODS_SELECTED)
			{
				InflateRect(lprc, -1, -1);
				COLORREF cr=GetSysColor(COLOR_HIGHLIGHT);
				cr=RGB(COLOR_SHIFT(GetRValue(cr), .6), COLOR_SHIFT(GetGValue(cr), .6), COLOR_SHIFT(GetBValue(cr), .6));
				HBRUSH hbr=CreateSolidBrush(cr);
				FillRect(hDC, lprc, hbr);
				DeleteObject(hbr);
				FrameRect(hDC, lprc, GetSysColorBrush(COLOR_HIGHLIGHT));
				InflateRect(lprc, 1, 1);
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}
			else
			{
				FillRect(hDC, lprc, GetSysColorBrush(COLOR_BTNFACE));
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}
			DrawText(hDC, mii.dwTypeData, mii.cch, lprc, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
		else
		{
			COLORREF crLight;
			RECT rc, rcImage, rcText;
			rcImage=rc=*lprc;
			rcImage.right=rcImage.left+m_szImage.cx+5;
			rcText=rcImage;
			rcText.left=rcImage.right;
			rcText.right=rc.right;
			rc=rcText;
			rc.left+=m_nTextMarginLeft;
			rc.top+=2;
			rc.right-=m_nTextMarginRight;
			rc.bottom-=2;

			if(lpdis->itemState & ODS_GRAYED)
			{
				FillRect(hDC, &rcImage, GetSysColorBrush(COLOR_BTNFACE));
				FillRect(hDC, &rcText, GetSysColorBrush(COLOR_MENU));
				SetTextColor(hDC, GetSysColor(COLOR_BTNHIGHLIGHT));
				OffsetRect(&rc, 1, 0);
				DrawMenuText(hDC, &rc, mii.dwTypeData);
				OffsetRect(&rc, -1, 0);
				SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT));
			}
			else if(lpdis->itemState & ODS_SELECTED)
			{
				crLight=GetSysColor(COLOR_HIGHLIGHT);
				crLight=RGB(COLOR_SHIFT(GetRValue(crLight), .6), COLOR_SHIFT(GetGValue(crLight), .6), COLOR_SHIFT(GetBValue(crLight), .6));
				HBRUSH hbr=CreateSolidBrush(crLight);
				InflateRect(lprc, 0, -2);
				FillRect(hDC, lprc, hbr);
				DeleteObject(hbr);
				FrameRect(hDC, lprc, GetSysColorBrush(COLOR_HIGHLIGHT));
				InflateRect(lprc, 0, 2);
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}
			else
			{
				FillRect(hDC, &rcImage, GetSysColorBrush(COLOR_BTNFACE));
				FillRect(hDC, &rcText, GetSysColorBrush(COLOR_MENU));
				SetTextColor(hDC, GetSysColor(COLOR_MENUTEXT));
			}
			DrawMenuText(hDC, &rc, mii.dwTypeData);

			InflateRect(&rcImage, 0, -2);
			rcImage.right-=1;
			if(mii.dwItemData)
			{
				if(lpdis->itemState&ODS_CHECKED)
				{
					InflateRect(&rcImage, -1, -1);
					FrameRect(hDC, &rcImage, GetSysColorBrush(COLOR_HIGHLIGHT));
					InflateRect(&rcImage, 1, 1);
				}

				if(lpdis->itemState&ODS_GRAYED)
					DrawGrayBitmap(hDC, &rcImage, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_BTNFACE));
				else if(lpdis->itemState&ODS_SELECTED)
					DrawBitmap(hDC, &rcImage, (HBITMAP)mii.dwItemData, crLight);
				else
					DrawBitmap(hDC, &rcImage, (HBITMAP)mii.dwItemData, GetSysColor(COLOR_BTNFACE));
			}
			else if(lpdis->itemState&ODS_CHECKED)
			{
				InflateRect(&rcImage, -1, -1);
				FrameRect(hDC, &rcImage, GetSysColorBrush(COLOR_HIGHLIGHT));
				InflateRect(&rcImage, 1, 1);
				DrawCheckMark(hDC, &rcImage, GetSysColor(COLOR_MENUTEXT), 2);
			}
		}

		delete[] mii.dwTypeData;
		RestoreDC(hDC, nSaveDC);
	}
	else
	{
		RECT rc=*lprc;
		rc.right=rc.left+m_szImage.cx+4;
		FillRect(hDC, &rc, GetSysColorBrush(COLOR_BTNFACE));
		rc.left=rc.right;
		rc.right=lprc->right;
		FillRect(hDC, &rc, GetSysColorBrush(COLOR_MENU));
		rc.left+=m_nTextMarginLeft;
		int nVCenter=(lprc->top+lprc->bottom)/2;
		COLORREF cr=GetSysColor(COLOR_BTNSHADOW);
		cr=RGB(COLOR_SHIFT(GetRValue(cr), 0.4), COLOR_SHIFT(GetGValue(cr), 0.4), COLOR_SHIFT(GetBValue(cr), 0.4));
		HPEN hPen=CreatePen(PS_SOLID, 1, cr);
		HPEN hPenOld=(HPEN)SelectObject(hDC, hPen);
		MoveToEx(hDC, rc.left, nVCenter, NULL);
		LineTo(hDC, rc.right, nVCenter);
		SelectObject(hDC, hPenOld);
		DeleteObject(hPen);
	}
}


TOLNS_END
