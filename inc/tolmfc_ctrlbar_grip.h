
#ifndef _INC_TOLMFC_CTRLBAR_GRIP_H
#define _INC_TOLMFC_CTRLBAR_GRIP_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_ctrlbar_size.h>

TOLNS_BEGIN

// TCtrlBarGrip control bar

class TOLMFCDECL TCtrlBarGrip : public TCtrlBarSize
{
protected:
	// TCtrlBarButton (button info) helper class
	class TOLMFCDECL TCtrlBarButton
	{
	public:
		TCtrlBarButton();
		void Paint(CDC* pDC, COLORREF crFg);

		inline void Move(CPoint ptTo) {ptOrg=ptTo; }
		inline CRect GetRect() { return CRect(ptOrg, CSize(11, 11)); }

	public:
		BOOL bPushed;
		BOOL bRaised;

	protected:
		CPoint ptOrg;
	};

public:
	TCtrlBarGrip();
	virtual ~TCtrlBarGrip();

	virtual BOOL HasGripper() const;
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	int m_cyGripper;
	TCtrlBarButton m_btnHide;

protected:
	virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
	virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);

	afx_msg
#if _MSC_VER>1400
	LRESULT
#else
	UINT
#endif
	OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	DECLARE_MESSAGE_MAP()
};

TOLNS_END

#endif /* _INC_TOLMFC_CTRLBAR_GRIP_H */
