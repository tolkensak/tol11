
#ifndef _INC_TOLMFC_GRIDCTRL_H
#define _INC_TOLMFC_GRIDCTRL_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

TOLNS_BEGIN


class TOLMFCDECL TGridCtrl : public CListCtrl
{
public:
	TGridCtrl();
	virtual ~TGridCtrl();

	CString GetEmptyText() const;
	void GetEmptyText(CString& str) const;
	void SetEmptyText(LPCTSTR pcText);
	void SetEmptyText(UINT uStrID);

	int GetColumnCount();

	void LoadSettings();
	void StoreSettings();

	virtual void ContextMenuHeader(CPoint pt);
	virtual BOOL ContextMenuClient(HMENU hMenu, UINT& uFlags, int nItem);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnHdnEndDrag(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	CString m_strEmptyText;

	virtual void PutSpecStyle(LONG& lStyle);
	virtual void SetSpecExtendStyle();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_GRIDCTRL_H */


#include <tolmfc_gridctrl.inl>
