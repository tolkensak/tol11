
#ifndef _INC_TOLCPP_MENU_H
#define _INC_TOLCPP_MENU_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp.h>

TOLNS_BEGIN


typedef CMap<DWORD, DWORD, LPVOID, LPVOID> CMapDWord2Ptr;


class TOLCPPDECL Menu : public Object
{
public:
	Menu();
	virtual ~Menu();

	BOOL Attach(HMENU hMenu);
	HMENU Detach();

	void CreateCmdBms(PUINT puCmdID, int nCount);
	void DeleteCmdBms(DWORD dwKey, BOOL bDeleteObject=FALSE);

	void Measure(LPMEASUREITEMSTRUCT lpmis, int nSkin);
	void Draw(LPDRAWITEMSTRUCT lpdis, int nSkin);

	void ChangeSettings();

	static HMENU GetMenuFromCmdID(HMENU hMenu, UINT uCmdID);


	CMapDWord2Ptr m_CmdBms;

protected:
	HMENU m_hMenu;
	HFONT m_hFont;
	SIZE m_szImage;
	int m_nTextMarginLeft;
	int m_nTextMarginRight;

	void SetOwnerDraw(HMENU hMenu, BOOL bTopMenu=TRUE);
	void MeasureWindows(LPMEASUREITEMSTRUCT lpmis);
	void DrawWindows(LPDRAWITEMSTRUCT lpdis);
	void MeasureOfficeXP(LPMEASUREITEMSTRUCT lpmis);
	void DrawOfficeXP(LPDRAWITEMSTRUCT lpdis);
};


TOLNS_END

#endif // _INC_TOLCPP_MENU_H
