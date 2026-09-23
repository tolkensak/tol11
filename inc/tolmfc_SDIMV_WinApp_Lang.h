
#ifndef _INC_TOLMFC_SDIMV_WINAPP_LANG_H
#define _INC_TOLMFC_SDIMV_WINAPP_LANG_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_SDIMV_WinApp.h>
#include <tolc_lang.h>

TOLNS_BEGIN


class TOLMFCDECL TSDIMVWinAppLang : public TSDIMVWinApp
{
	DECLARE_DYNAMIC(TSDIMVWinAppLang)

protected:
	TSDIMVWinAppLang();

public:
	virtual ~TSDIMVWinAppLang();

	virtual BOOL InitInstance();
	virtual void AddLangMenu(HMENU hMenu);

	LANGID GetLangID() const;

protected:
	LANGID m_lid;
	TLANG m_tLang;

	Bool SetLangInstance(LANGID lidNew);

	afx_msg void OnLang(UINT uID);
	afx_msg void OnUpdateLang(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_SDIMV_WINAPP_LANG_H */


#include <tolmfc_SDIMV_WinApp_Lang.inl>
