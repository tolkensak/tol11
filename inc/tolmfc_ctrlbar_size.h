
#ifndef _INC_TOLMFC_CTRLBAR_SIZE_H
#define _INC_TOLMFC_CTRLBAR_SIZE_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

#include <afxpriv.h>   // for CDockContext
#include <afxtempl.h>  // for CTypedPtrArray

//#define _SCB_REPLACE_MINIFRAME
//#define _SCB_MINIFRAME_CAPTION

//?
//#if defined(_SCB_MINIFRAME_CAPTION) && !defined(_SCB_REPLACE_MINIFRAME)
//	#error "_SCB_MINIFRAME_CAPTION requires _SCB_REPLACE_MINIFRAME"
//#endif
//?

TOLNS_BEGIN

// TCtrlBarSize control bar styles

#define SCBS_EDGELEFT       0x00000001
#define SCBS_EDGERIGHT      0x00000002
#define SCBS_EDGETOP        0x00000004
#define SCBS_EDGEBOTTOM     0x00000008
#define SCBS_EDGEALL        0x0000000F
#define SCBS_SHOWEDGES      0x00000010
#define SCBS_SIZECHILD      0x00000020

// TCtrlBarSize control bar

class TCtrlBarSize;

typedef CTypedPtrArray <CPtrArray, TCtrlBarSize*> TSizeBarArray;

class TOLMFCDECL TCtrlBarSize : public CControlBar
{
	DECLARE_DYNAMIC(TCtrlBarSize)

protected:
	// TCtrlBarDockBar dummy class for access to protected members
	class TOLMFCDECL TCtrlBarDockBar : public CDockBar
	{
		friend class TCtrlBarSize;
	};

public:
	class TOLMFCDECL TCtrlBarInfo
	{
	public:
		CSize sizeFloat;
		CSize sizeHorz;
		CSize sizeVert;
		CSize sizeMinFloat;
		CSize sizeMinHorz;
		CSize sizeMinVert;
	};

	class TOLMFCDECL TCtrlBarKeepSize
	{
	public:
		TCtrlBarKeepSize(TCtrlBarSize* pBar);
		~TCtrlBarKeepSize();

	protected:
		BOOL m_bOldKeepSize;
		TCtrlBarSize* m_pBar;
	};

public:
	TCtrlBarSize();
	virtual ~TCtrlBarSize();

	virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd, CSize sizeDefault, BOOL bHasGripper, UINT nID, DWORD dwStyle=WS_CHILD | WS_VISIBLE | CBRS_LEFT);
	virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd, UINT nID, DWORD dwStyle=WS_CHILD | WS_VISIBLE | CBRS_LEFT);

	const BOOL IsFloating() const;
	const BOOL IsHorzDocked() const;
	const BOOL IsVertDocked() const;
	const BOOL IsSideTracking() const;
	const BOOL GetSCBStyle() const {return m_dwSCBStyle;}

//?
//#if defined(_SCB_REPLACE_MINIFRAME) && !defined(_SCB_MINIFRAME_CAPTION)
//	void EnableDocking(DWORD dwDockStyle);
//#endif
//?

	void GetInfo(TCtrlBarInfo* pInfo);
	void SetInfo(TCtrlBarInfo* pInfo, CFrameWnd* pFrameWnd);

	virtual void LoadState(LPCTSTR lpszProfileName);
	virtual void SaveState(LPCTSTR lpszProfileName);
	static void GlobalLoadState(CFrameWnd* pFrame, LPCTSTR lpszProfileName);
	static void GlobalSaveState(CFrameWnd* pFrame, LPCTSTR lpszProfileName);
	void SetSCBStyle(DWORD dwSCBStyle){m_dwSCBStyle=(dwSCBStyle & ~SCBS_EDGEALL);}

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

protected:
	DWORD   m_dwSCBStyle;
	UINT    m_htEdge;
	CSize   m_szHorz;
	CSize   m_szVert;
	CSize   m_szFloat;
	CSize   m_szMinHorz;
	CSize   m_szMinVert;
	CSize   m_szMinFloat;
	int     m_nTrackPosMin;
	int     m_nTrackPosMax;
	int     m_nTrackPosOld;
	int     m_nTrackEdgeOfs;
	BOOL    m_bTracking;
	BOOL    m_bKeepSize;
	BOOL    m_bParentSizing;
	BOOL    m_bDragShowContent;
	UINT    m_nDockBarID;
	int     m_cxEdge;

protected:
	UINT GetEdgeHTCode(int nEdge);
	BOOL GetEdgeRect(CRect rcWnd, UINT nHitTest, CRect& rcEdge);
	virtual void StartTracking(UINT nHitTest, CPoint point);
	virtual void StopTracking();
	virtual void OnTrackUpdateSize(CPoint& point);
	virtual void OnTrackInvertTracker();
	virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
	virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);

	virtual void AlignControlBars();
	void GetRowInfo(int& nFirst, int& nLast, int& nThis);
	void GetRowSizingBars(TSizeBarArray& arrSCBars);
	void GetRowSizingBars(TSizeBarArray& arrSCBars, int& nThis);
	BOOL NegotiateSpace(int nLengthTotal, BOOL bHorz);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg
#if _MSC_VER>1400
	LRESULT
#else
	UINT
#endif
	OnNcHitTest(CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

//?
//#ifdef _SCB_REPLACE_MINIFRAME
//	friend class CSizingDockFrameWnd;
//#endif //_SCB_REPLACE_MINIFRAME
//?
};

//?
//#ifdef _SCB_REPLACE_MINIFRAME
//#ifndef _SCB_MINIFRAME_CAPTION
//
///////////////////////////////////////////////////////////////////////////
//// CSizingDockContext dockcontext
//
//class CSizingDockContext : public CDockContext
//{
//public:
//	CSizingDockContext(CControlBar* pBar) : CDockContext(pBar) {}
//
//	// Drag Operations
//	virtual void StartDrag(CPoint pt);
//};
//
//#endif //_SCB_MINIFRAME_CAPTION
//
///////////////////////////////////////////////////////////////////////////
//// CSizingDockFrameWnd miniframe
//
//class CSizingDockFrameWnd : public CMiniDockFrameWnd
//{
//	DECLARE_DYNCREATE(CSizingDockFrameWnd)
//
//public:
//	virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
//	TCtrlBarSize* GetSizingControlBar();
//
//	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
//	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
//	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//
//	DECLARE_MESSAGE_MAP()
//};
//
//#endif //_SCB_REPLACE_MINIFRAME
//?

TOLNS_END

#endif /* _INC_TOLMFC_CTRLBAR_SIZE_H */
