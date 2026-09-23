
#ifndef _INC_TOLMFC_WINAPP_H
#define _INC_TOLMFC_WINAPP_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

TOLNS_BEGIN


class TOLMFCDECL TWinApp : public CWinApp
{
	DECLARE_DYNAMIC(TWinApp)

protected:
	TWinApp();

public:
	virtual ~TWinApp();

	afx_msg void OnAppAbout();
	afx_msg void OnUpdateRecentFileMenu(CCmdUI *pCmdUI);

protected:
	UINT m_uStartedTimes;

	void SetRegistryKey(LPCTSTR lpszRegistryKey);
	void SetRegistryKey(UINT nIDRegistryKey=0);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_WINAPP_H */
