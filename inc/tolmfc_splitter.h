
#ifndef _INC_TOLMFC_SPLITTER_H
#define _INC_TOLMFC_SPLITTER_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolmfc.h>
#include <tolcpp_object.h>

TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TSplitter

class TOLMFCDECL TSplitter : public CWnd
{
public:
	class TOLMFCDECL TPane : public Object
	{
	public:
		TPane();
		virtual ~TPane();

		enum { styleSplitter=1, styleAutoSize=2 };

		BOOL IsSplitter() const;
		BOOL IsAutoSize() const;

		int GetBreadth() const;
		CWnd* GetWnd() const;

		BOOL AddBreadth(int nBreadth);

		void Set(DWORD dwStyle, CWnd* pWnd, int nBreadth=0, int nMinBreadth=0, int nMaxBreadth=0);
		TPane& operator=(const TPane& pane);

	protected:
		DWORD m_dwStyle;
		CWnd* m_pWnd;
		int m_nBreadth;
		int m_nMinBreadth;
		int m_nMaxBreadth;
	};

public:
	TSplitter(BOOL bHorz, int nWeight=2);
	virtual ~TSplitter();

	BOOL IsHorz() const;
	BOOL IsVert() const;

	virtual BOOL Create(CWnd* pParentWnd, UINT nID);

	void Size(LPRECT prc=NULL);
	void Size(int cx, int cy);
	void Size(int l, int top, int r, int b);
	void SetPane(BOOL bTopLeft, const TSplitter::TPane& pane);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

protected:
	BOOL m_bHorz;
	int m_nWeight;
	TPane m_pane[2];

	CPoint m_ptPrev;

#ifdef _DRAG_DRAW_LINE
	HBITMAP m_hbmDrag;
	CRect m_rcWindow;
#else // _DRAG_DRAW_LINE
	CRect m_rcOwn;
#endif // _DRAG_DRAW_LINE

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_SPLITTER_H */


#include <tolmfc_splitter.inl>
