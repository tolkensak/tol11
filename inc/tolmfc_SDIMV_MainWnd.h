
#ifndef _INC_TOLMFC_SDIMV_MAINWND_H
#define _INC_TOLMFC_SDIMV_MAINWND_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_mainwnd.h>
#include <tolmfc_SDIMV_WinApp.h>
#include <afxtempl.h>

TOLNS_BEGIN


///////////////////////////////////////////////////////////////////////////////
//
// TSDIMVSplitInfo


class TOLMFCDECL TSDIMVSplitInfo
{
public:
	TSDIMVSplitInfo::TSDIMVSplitInfo();

	TSDIMVSplitInfo& operator=(const TSDIMVSplitInfo& si);  // function only for CMap

public:
	int m_MaxRow;
	int m_MaxCol;

	CSplitterWnd m_wndSplitter;
};

typedef CMap<int, int, TSDIMVSplitInfo, TSDIMVSplitInfo&> SplitInfoMap;


///////////////////////////////////////////////////////////////////////////////
//
// TSDIMVMainWnd


class TOLMFCDECL TSDIMVMainWnd : public TMainWnd
{
	DECLARE_DYNCREATE(TSDIMVMainWnd)

protected:
	TSDIMVMainWnd();

public:
	virtual ~TSDIMVMainWnd();

	BOOL IsViewActive(UINT nIDResource);
	BOOL IsViewActive(CView* pView);
	BOOL IsViewActive(CRuntimeClass* pViewClass);

	BOOL ActivateView(UINT nIDResource);
	BOOL ActivateView(CView* pView);
	BOOL ActivateView(CRuntimeClass* pViewClass);

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void UpdateResource(TSDIMVDocTemplate* pNewTemplate);
	virtual void UpdateMenu(HMENU /*hMenu*/, UINT /*uResID*/);

protected:
	SplitInfoMap m_mapSplitInfo;

	BOOL ActivateView(TSDIMVDocTemplate* pTemplate);

	DECLARE_MESSAGE_MAP()
};


TOLNS_END

#endif /* _INC_TOLMFC_SDIMV_MAINWND_H */
