
#include "stdafx.h"
#include <tolmfc_SDIMV_WinApp.h>

TOLNS_BEGIN


///////////////////////////////////////////////////////////////////////////////
//
// TSDIMVDocTemplate


IMPLEMENT_DYNAMIC(TSDIMVDocTemplate, CSingleDocTemplate)


TSDIMVDocTemplate::TSDIMVDocTemplate(UINT nIDResource
								, CRuntimeClass* pDocClass
								, CRuntimeClass* pFrameClass
								, CRuntimeClass* pViewClass
								, int nSplitterID
								, int nRow
								, int nCol)
	: CSingleDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
	, m_nSplitterID(nSplitterID)
	, m_nRow(nRow)
	, m_nCol(nCol)
	, m_pView(NULL)
{
}

void TSDIMVDocTemplate::LoadDocString()
{
	m_strDocStrings.LoadString(m_nIDResource);
}


///////////////////////////////////////////////////////////////////////////////
//
// TSDIMVWinApp


IMPLEMENT_DYNAMIC(TSDIMVWinApp, TWinApp)


TSDIMVWinApp::TSDIMVWinApp()
{
}

TSDIMVDocTemplate* TSDIMVWinApp::GetFirstTemplate()
{
	POSITION pos=GetFirstDocTemplatePosition();
	TSDIMVDocTemplate* pTempl=(TSDIMVDocTemplate*)GetNextDocTemplate(pos);

	ASSERT(pTempl);

	return pTempl;
}

TSDIMVDocTemplate* TSDIMVWinApp::GetTemplate(UINT nIDResource)
{
	TSDIMVDocTemplate* pTempl;
	POSITION pos=GetFirstDocTemplatePosition();
	while(pos)
	{
		pTempl=(TSDIMVDocTemplate*)GetNextDocTemplate(pos);
		if(pTempl->GetResourceID()==nIDResource)
			return pTempl;
	}

	return NULL;
}

TSDIMVDocTemplate* TSDIMVWinApp::GetTemplate(CView* pView)
{
	TSDIMVDocTemplate* pTempl;
	POSITION pos=GetFirstDocTemplatePosition();
	while(pos)
	{
		pTempl=(TSDIMVDocTemplate*)GetNextDocTemplate(pos);
		if(pTempl->m_pView==pView)
			return pTempl;
	}

	return NULL;
}

TSDIMVDocTemplate* TSDIMVWinApp::GetTemplate(CRuntimeClass *pViewClass)
{
	TSDIMVDocTemplate* pTempl;
	POSITION pos=GetFirstDocTemplatePosition();
	while(pos)
	{
		pTempl=(TSDIMVDocTemplate*)GetNextDocTemplate(pos);
		if(pTempl->m_pView->IsKindOf(pViewClass))
			return pTempl;
	}

	return NULL;
}


TOLNS_END
