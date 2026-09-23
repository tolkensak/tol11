
#include "stdafx.h"
#include <tolmfc_ctrlbar_view.h>

TOLNS_BEGIN


BEGIN_MESSAGE_MAP(TCtrlBarView, TCtrlBar)
	ON_WM_SIZE()
END_MESSAGE_MAP()


TCtrlBarView::TCtrlBarView()
	: m_pView(NULL)
{
}

TCtrlBarView::~TCtrlBarView()
{
}


void TCtrlBarView::OnSize(UINT nType, int cx, int cy) 
{
	TCtrlBar::OnSize(nType, cx, cy);

	if(m_pView)
	{
		int dx=GetSystemMetrics(SM_CXEDGE);
		int dy=GetSystemMetrics(SM_CYEDGE);
		m_pView->MoveWindow(dx, dy, cx-2*dx, cy-2*dy);
	}
}

CView* TCtrlBarView::GetView()
{
	return m_pView;
}

BOOL TCtrlBarView::CreateView(CRuntimeClass *pViewClass, CCreateContext *pContext)
{
	ASSERT_VALID(this);
	ASSERT(pViewClass !=NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	CCreateContext context;
	if(pContext==NULL)
	{
		// if no context specified, generate one from the currently selected client if possible
		CView* pOldView=NULL;
		if(pOldView !=NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(context.m_pCurrentFrame==NULL);
			context.m_pLastView=pOldView;
			context.m_pCurrentDoc=pOldView->GetDocument();
			if(context.m_pCurrentDoc !=NULL)
				context.m_pNewDocTemplate=
				context.m_pCurrentDoc->GetDocTemplate();
		}
		pContext=&context;
	}
	
	CWnd* pWnd;
	TRY
	{
		pWnd=(CWnd*)pViewClass->CreateObject();
		if(pWnd==NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL
		
	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd==NULL);       // not yet created
	
	DWORD dwStyle=AFX_WS_DEFAULT_VIEW;
	CRect rect;
	// Create with the right size and position
	if(!pWnd->Create(NULL, NULL, dwStyle, rect, this, 0, pContext))
	{
		TRACE0("Warning: couldn't create client pane for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}

	m_pView=(CView*)pWnd;
	((CFrameWnd*)GetParent())->SetActiveView((CView *)m_pView);

	return TRUE;
}


TOLNS_END
