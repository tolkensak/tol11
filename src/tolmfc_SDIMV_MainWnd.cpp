
#include "stdafx.h"
#include <tolmfc_SDIMV_MainWnd.h>
#include <afxrich.h>

TOLNS_BEGIN


///////////////////////////////////////////////////////////////////////////////
//
// TSDIMVSplitInfo


TSDIMVSplitInfo::TSDIMVSplitInfo()
	: m_MaxRow(0)
	, m_MaxCol(0)
{
}

TSDIMVSplitInfo& TSDIMVSplitInfo::operator=(const TSDIMVSplitInfo& si)
{
	return *this;
}


///////////////////////////////////////////////////////////////////////////////
//
// TSDIMVMainWnd


IMPLEMENT_DYNCREATE(TSDIMVMainWnd, TMainWnd)


BEGIN_MESSAGE_MAP(TSDIMVMainWnd, TMainWnd)
END_MESSAGE_MAP()


TSDIMVMainWnd::TSDIMVMainWnd()
{
}

TSDIMVMainWnd::~TSDIMVMainWnd()
{
}

BOOL TSDIMVMainWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	CView* pView;
	TSDIMVWinApp* pApp = (TSDIMVWinApp*)AfxGetApp();
	ASSERT_KINDOF(TSDIMVWinApp, pApp);

	// hold informations from DocTemplates to create splitter
	POSITION TmpPos = pApp->GetFirstDocTemplatePosition();
	while (TmpPos)
	{
		TSDIMVDocTemplate* pTemplate = (TSDIMVDocTemplate*)pApp->GetNextDocTemplate(TmpPos);
		ASSERT_KINDOF(TSDIMVDocTemplate, pTemplate);
		if (pTemplate->m_nSplitterID != -1)
		{
			// find or create the splitter ID in the splitter map
			int nID = pTemplate->m_nSplitterID;
			m_mapSplitInfo[nID].m_MaxRow = max(m_mapSplitInfo[nID].m_MaxRow, pTemplate->m_nRow + 1);
			m_mapSplitInfo[nID].m_MaxCol = max(m_mapSplitInfo[nID].m_MaxCol, pTemplate->m_nCol + 1);
		}
	}
  
	// create the splitter and the views in the splitter
	POSITION MapPos = m_mapSplitInfo.GetStartPosition();
	while (MapPos)
	{
		int nID; TSDIMVSplitInfo SplitInfo;
		m_mapSplitInfo.GetNextAssoc(MapPos, nID, SplitInfo);
		if (m_mapSplitInfo[nID].m_MaxRow && m_mapSplitInfo[nID].m_MaxCol)
		{
			CRect rect; GetClientRect(&rect);
			int cx = rect.Width(); int cy = rect.Height();
			
			// normaly always ToolBar & StatusBar
			cy = cy - 56; if (cy < 56) cy = 0;

			if (m_mapSplitInfo[nID].m_MaxCol) cx /= m_mapSplitInfo[nID].m_MaxCol;
			if (m_mapSplitInfo[nID].m_MaxRow) cy /= m_mapSplitInfo[nID].m_MaxRow;
			CSize sizeDefault(cx, cy);

			m_mapSplitInfo[nID].m_wndSplitter.CreateStatic(this, m_mapSplitInfo[nID].m_MaxRow, m_mapSplitInfo[nID].m_MaxCol);
			// disable view ID to use for next splitter
			m_mapSplitInfo[nID].m_wndSplitter.SetDlgCtrlID(0);
	
			TmpPos = pApp->GetFirstDocTemplatePosition();
			while (TmpPos)
			{
				TSDIMVDocTemplate* pTemplate = (TSDIMVDocTemplate*)pApp->GetNextDocTemplate(TmpPos);
				if (pTemplate->m_nSplitterID == nID)
				{
					// create view
					int nRow = pTemplate->m_nRow, nCol = pTemplate->m_nCol;
					pContext->m_pNewDocTemplate = pTemplate;
					pContext->m_pNewViewClass = pTemplate->GetViewClass();
					if (!m_mapSplitInfo[nID].m_wndSplitter.CreateView(nRow, nCol,
						pContext->m_pNewViewClass, sizeDefault, pContext)) return FALSE;
					pTemplate->m_pView = (CView*)m_mapSplitInfo[nID].m_wndSplitter.GetPane(nRow, nCol);
				}
			}
			// disable view ID to use for next splitter view
			TmpPos = pApp->GetFirstDocTemplatePosition();
			while (TmpPos)
			{
				TSDIMVDocTemplate* pTemplate = (TSDIMVDocTemplate*)pApp->GetNextDocTemplate(TmpPos);
				if (pTemplate->m_nSplitterID == nID)
					pTemplate->m_pView->SetDlgCtrlID(0);
			}
		}
	}

	// create the other views (no splitter)
	TmpPos = pApp->GetFirstDocTemplatePosition();
	while (TmpPos)
	{
		TSDIMVDocTemplate* pTemplate = (TSDIMVDocTemplate*)pApp->GetNextDocTemplate(TmpPos);
		if (pTemplate->m_nSplitterID == -1)
		{
			pContext->m_pNewViewClass = pTemplate->GetViewClass();
			pView = (CView*)CreateView(pContext, AFX_IDW_PANE_FIRST);
			ASSERT(pView != NULL);
			pTemplate->m_pView = pView;
			// disable view ID to use for next view
			pView->SetDlgCtrlID(0);
		}
	}
	SetActiveView(NULL);

	TSDIMVDocTemplate *pTemplate;

	// litle problem ! if a CRichEditView is in the view list
	// we must activate it first to update the layout correctly
	pTemplate = pApp->GetTemplate(RUNTIME_CLASS(CRichEditView));
	if (pTemplate) ActivateView(pTemplate->GetViewClass());

	// activate the first doctemplate view
	pTemplate = pApp->GetFirstTemplate();
	ActivateView(pTemplate->GetViewClass());

	return TRUE;
}

BOOL TSDIMVMainWnd::IsViewActive(UINT nIDResource)
{
	CView* pActiveView=GetActiveView();
	if(pActiveView)
	{
		TSDIMVDocTemplate* pTemplate=((TSDIMVWinApp*)AfxGetApp())->GetTemplate(nIDResource);
		if(pTemplate)
			return pActiveView==pTemplate->m_pView;
	}

	return FALSE;
}

BOOL TSDIMVMainWnd::IsViewActive(CView* pView)
{
	return pView==GetActiveView();
}

BOOL TSDIMVMainWnd::IsViewActive(CRuntimeClass* pViewClass)
{
	CView* pActiveView=GetActiveView();
	if(pActiveView)
		return pViewClass==pActiveView->GetRuntimeClass();

	return FALSE;
}

BOOL TSDIMVMainWnd::ActivateView(UINT nIDResource)
{
	return ActivateView(((TSDIMVWinApp*)AfxGetApp())->GetTemplate(nIDResource));
}

BOOL TSDIMVMainWnd::ActivateView(CView* pView)
{
	return ActivateView(((TSDIMVWinApp*)AfxGetApp())->GetTemplate(pView));
}

BOOL TSDIMVMainWnd::ActivateView(CRuntimeClass* pViewClass)
{
	return ActivateView(((TSDIMVWinApp*)AfxGetApp())->GetTemplate(pViewClass));
}

BOOL TSDIMVMainWnd::ActivateView(TSDIMVDocTemplate* pNewTemplate)
{
	if(!pNewTemplate) // the view class doesn't exist
		return FALSE;

	TSDIMVWinApp* pApp=(TSDIMVWinApp*)AfxGetApp();
	TSDIMVDocTemplate* pActiveTemplate=pApp->GetTemplate(GetActiveView());
	
	if(pNewTemplate==pActiveTemplate) // the new template is the active template, do nothing
		return FALSE;

	if(pActiveTemplate)
	{
		if(pActiveTemplate->m_nSplitterID!=-1 && pActiveTemplate->m_nSplitterID==pNewTemplate->m_nSplitterID)
		{ // the active and new template are in the same splitter, change the active view
			UpdateResource(pNewTemplate);
			SetActiveView(pNewTemplate->m_pView);
			return TRUE;
		}

		if(pActiveTemplate->m_nSplitterID!=-1)
		{ // the active view is in a splitter, hide all views in the splitter and the splitter
			TSDIMVDocTemplate* pDocTempl;
			int nID=pActiveTemplate->m_nSplitterID;
			POSITION posTempl=pApp->GetFirstDocTemplatePosition();

			while(posTempl)
			{
				pDocTempl=(TSDIMVDocTemplate*)pApp->GetNextDocTemplate(posTempl);
				if(pDocTempl->m_nSplitterID==nID)
				{
					pDocTempl->m_pView->ShowWindow(SW_HIDE);
					pDocTempl->m_pView->SetDlgCtrlID(0);
				}
			}

			// hide the splitter
			m_mapSplitInfo[nID].m_wndSplitter.SetDlgCtrlID(0);
			m_mapSplitInfo[nID].m_wndSplitter.ShowWindow(SW_HIDE);
		}
		else
		{ // the active view is not in a splitter, hide this view
			pActiveTemplate->m_pView->SetDlgCtrlID(0);
			pActiveTemplate->m_pView->ShowWindow(SW_HIDE);
		}
	}
  
	if(pNewTemplate->m_nSplitterID!=-1)
	{ // the new view is in a splitter, show all splitter views and the splitter
		TSDIMVDocTemplate* pDocTempl;
		int nID=pNewTemplate->m_nSplitterID;
		POSITION posTempl=pApp->GetFirstDocTemplatePosition();

		while(posTempl)
		{
			pDocTempl=(TSDIMVDocTemplate*)pApp->GetNextDocTemplate(posTempl);
			if(pDocTempl->m_nSplitterID==nID)
			{
				int nRow=pDocTempl->m_nRow;
				int nCol=pDocTempl->m_nCol;
				int nDlgID=m_mapSplitInfo[nID].m_wndSplitter.IdFromRowCol(nRow, nCol);
				pDocTempl->m_pView->SetDlgCtrlID(nDlgID);
				pDocTempl->m_pView->ShowWindow(SW_SHOW);
			}
		}

		// show the splitter
		m_mapSplitInfo[nID].m_wndSplitter.SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		m_mapSplitInfo[nID].m_wndSplitter.ShowWindow(SW_SHOW);
	}
	else
	{ // the new view is not in a splitter, active the new view
		pNewTemplate->m_pView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		pNewTemplate->m_pView->ShowWindow(SW_SHOW);
	}

	UpdateResource(pNewTemplate);

	// display and update the new view
	SetActiveView(pNewTemplate->m_pView);
	RecalcLayout();
	return TRUE;
}

void TSDIMVMainWnd::UpdateMenu(HMENU /*hMenu*/, UINT /*uResID*/)
{
}

void TSDIMVMainWnd::UpdateResource(TSDIMVDocTemplate *pNewTemplate)
{
	TSDIMVWinApp* pApp=(TSDIMVWinApp*)AfxGetApp();

	// support for view context menu, accelerator and help ID like MDI
	//HINSTANCE hInst=AfxFindResourceHandle(MAKEINTRESOURCE(pNewTemplate->GetResourceID()), RT_MENU);

	// if menu is not the default menu, destroy it
	HMENU hMenu=::GetMenu(m_hWnd);
	if(hMenu!=m_hMenuDefault)
		::DestroyMenu(hMenu);

	// load menu from doctemplate
	hMenu=::LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(pNewTemplate->GetResourceID()));
	if(!hMenu)
		hMenu=m_hMenuDefault; // if no menu, get default

	AfterLoadMenu(hMenu);
	UpdateMenu(hMenu, pNewTemplate->GetResourceID());

	if(IsMenuStyle())
		MenuOD_SetOwnerDraw(hMenu, TRUE, TRUE);

	::SetMenu(m_hWnd, hMenu); // load menu

	// load new accelerator table
	HACCEL hAccel=::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(pNewTemplate->GetResourceID()));
	if(!hAccel)
		// if no table for this template, load default
		::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(pApp->GetFirstTemplate()->GetResourceID()));

	// chage help ID for this view
	m_nIDHelp=pNewTemplate->GetResourceID();

	// change the title of the document
	pNewTemplate->GetDocString(m_strTitle, CDocTemplate::windowTitle);
	OnUpdateFrameTitle(TRUE);
}


TOLNS_END
