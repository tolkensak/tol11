
#ifndef _INC_TOLMFC_ABOUTDLG_H
#define _INC_TOLMFC_ABOUTDLG_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

TOLNS_BEGIN


class TOLMFCDECL TAboutDlg : public CDialog
{
public:
	TAboutDlg(CSize szIcon=CSize(48, 48), CWnd* pParent=NULL);
	~TAboutDlg();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

protected:
	HFONT m_hFont;
	HICON m_hIcon;

	int m_nDistance;
	CSize m_szIcon;
	CSize m_szText;

	static const CRect m_rcDlgPadding;
	static const CSize m_szButton;

	CString m_strInformation;
	CString m_strCopyright;

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_ABOUTDLG_H */
