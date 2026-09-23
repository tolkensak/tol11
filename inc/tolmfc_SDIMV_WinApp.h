
#ifndef _INC_TOLMFC_SDIMV_WINAPP_H
#define _INC_TOLMFC_SDIMV_WINAPP_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc_winapp.h>

TOLNS_BEGIN


///////////////////////////////////////////////////////////////////////////////
//
// TSDIMVDocTemplate


class TOLMFCDECL TSDIMVDocTemplate : public CSingleDocTemplate
{
	DECLARE_DYNAMIC(TSDIMVDocTemplate)

public:
	TSDIMVDocTemplate(UINT nIDResource
		, CRuntimeClass* pDocClass
		, CRuntimeClass* pFrameClass
		, CRuntimeClass* pViewClass
		, int nSplitterID=-1
		, int nRow=0
		, int nCol=0);

public:
	int m_nSplitterID;	// -1, no splitter else splitter ID (0, 1,...)
	int m_nRow;			// if splitter, row number (0, 1,...)
	int m_nCol;			// if splitter, col number (0, 1,...)
	CView* m_pView;		// view pointer

	// these overrides to retreive the view class and resource ID
	UINT GetResourceID();
	CRuntimeClass* GetViewClass();
	void LoadDocString();
};

TOLINL
UINT TSDIMVDocTemplate::GetResourceID()
{
	return m_nIDResource;
}

TOLINL
CRuntimeClass* TSDIMVDocTemplate::GetViewClass()
{
	return m_pViewClass;
}


///////////////////////////////////////////////////////////////////////////////
//
// TSDIMVWinApp


class TOLMFCDECL TSDIMVWinApp : public TWinApp
{
	DECLARE_DYNAMIC(TSDIMVWinApp)

public:
	TSDIMVWinApp();

	TSDIMVDocTemplate* GetFirstTemplate();
	TSDIMVDocTemplate* GetTemplate(UINT nIDResource);
	TSDIMVDocTemplate* GetTemplate(CView* pView);
	TSDIMVDocTemplate* GetTemplate(CRuntimeClass *pViewClass);
};


TOLNS_END

#endif /* _INC_TOLMFC_SDIMV_WINAPP_H */
