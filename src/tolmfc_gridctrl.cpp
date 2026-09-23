
#include "stdafx.h"
#include <tolmfc_gridctrl.h>

TOLNS_BEGIN


BEGIN_MESSAGE_MAP(TGridCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(HDN_ENDDRAG, 0, OnHdnEndDrag)
END_MESSAGE_MAP()


TGridCtrl::TGridCtrl()
{
}

TGridCtrl::~TGridCtrl()
{
}

void TGridCtrl::PutSpecStyle(LONG& lStyle)
{
	lStyle&=~LVS_TYPEMASK;
	lStyle|=LVS_REPORT|LVS_SHOWSELALWAYS;
}

void TGridCtrl::SetSpecExtendStyle()
{
	SetExtendedStyle(LVS_EX_FULLROWSELECT
#if _WIN32_WINNT>=0x501
		|LVS_EX_DOUBLEBUFFER
#endif
		|LVS_EX_GRIDLINES);
}

BOOL TGridCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	PutSpecStyle(cs.style);
	return CListCtrl::PreCreateWindow(cs);
}

void TGridCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	LONG lStyle=(LONG)GetWindowLongPtr(m_hWnd, GWL_STYLE);
	PutSpecStyle(lStyle);
	SetWindowLongPtr(m_hWnd, GWL_STYLE, lStyle);

	SetSpecExtendStyle();
}

int TGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CListCtrl::OnCreate(lpCreateStruct)==-1)
		return -1;

	SetSpecExtendStyle();
	return 0;
}

void TGridCtrl::OnDestroy()
{
	StoreSettings();
	CListCtrl::OnDestroy();
}

void TGridCtrl::OnHdnEndDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr=reinterpret_cast<LPNMHEADER>(pNMHDR);

	if(phdr->pitem->mask&HDI_ORDER)
	{
		// Correct iOrder so it is just after the last hidden column
		int nColCount=GetColumnCount();
		int* pnCols=new int[nColCount];

		GetColumnOrderArray(pnCols, nColCount);

		for(int i=0; i<nColCount; i++)
		{
			phdr->pitem->iOrder=max(phdr->pitem->iOrder, i);
			Invalidate();
			break;
		}

		delete[] pnCols;
	}

	*pResult=0;
}

void TGridCtrl::ContextMenuHeader(CPoint pt)
{
	int nColCount=GetColumnCount();
	int *pnCols=new int[nColCount];

	GetColumnOrderArray(pnCols, nColCount);

	TCHAR pc[80];
	LVCOLUMN lvc;
	lvc.mask=LVCF_TEXT|LVCF_WIDTH;
	lvc.cchTextMax=80;
	lvc.pszText=pc;

	int col;
	HMENU hMenu=CreatePopupMenu();

	for(int i=0; i<nColCount; i++)
	{
		col=pnCols[i];
		GetColumn(col, &lvc);
		AppendMenu(hMenu, (lvc.cx?MF_CHECKED:0)|MF_STRING, col+1, pc);
	}

	CWnd* pMainWnd=AfxGetMainWnd();
	pMainWnd->SetForegroundWindow();

	ClientToScreen(&pt);
	col=TrackPopupMenuEx(hMenu, TPM_RETURNCMD|TPM_LEFTALIGN|TPM_TOPALIGN, pt.x, pt.y, pMainWnd->m_hWnd, NULL)-1;

	DestroyMenu(hMenu);
	pMainWnd->PostMessage(WM_NULL, 0, 0);

	if(col>=0)
	{
		int cx=GetColumnWidth(col);
		cx=cx?0:LVSCW_AUTOSIZE;
		SetColumnWidth(col, cx);
	}

	delete[] pnCols;
}

BOOL TGridCtrl::ContextMenuClient(HMENU hMenu, UINT& uFlags, int nItem)
{
	return FALSE;
}

void TGridCtrl::OnContextMenu(CWnd* pWnd, CPoint pt)
{
	CRect rc;
	int nItem=-1;

	if(pt.x==-1 && pt.y==-1) // triggred by menu key or F10
	{
		nItem=GetNextItem(-1, LVNI_SELECTED);
		if(nItem<0) // hasn't any selected item
		{
			GetHeaderCtrl()->GetWindowRect(&rc);
			pt.x=0;
			pt.y=rc.Height();
		}
		else // has one or more selected item
		{
			GetItemRect(nItem, &rc, LVIR_ICON);
			pt.x=(rc.right+rc.left)/2;
			pt.y=(rc.bottom+rc.top)/2;
		}
	}
	else // triggered by right mouse button
	{
		ScreenToClient(&pt);
		GetHeaderCtrl()->GetWindowRect(&rc);

		if(pt.y<rc.Height()) // clicked on header area
		{
			ContextMenuHeader(pt);
			return;
		}
		else // clicked on client area
		{
			LVHITTESTINFO lvhti;
			lvhti.pt=pt;
			lvhti.flags=LVHT_ONITEM;
			nItem=HitTest(&lvhti);
		}
	}

	HMENU hMenu=CreatePopupMenu();
	UINT uFlags=TPM_LEFTALIGN|TPM_TOPALIGN;

	if(!ContextMenuClient(hMenu, uFlags, nItem))
	{
		DestroyMenu(hMenu);
		return;
	}

	CWnd* pMainWnd=AfxGetMainWnd();
	pMainWnd->SetForegroundWindow();

	ClientToScreen(&pt);
	TrackPopupMenuEx(hMenu, uFlags, pt.x, pt.y, pMainWnd->m_hWnd, NULL);

	DestroyMenu(hMenu);
	pMainWnd->PostMessage(WM_NULL, 0, 0);
}

void TGridCtrl::SetEmptyText(UINT uStrID)
{
	if(uStrID)
		m_strEmptyText.LoadString(uStrID);
	else
		m_strEmptyText.Empty();
}

void TGridCtrl::SetEmptyText(LPCTSTR pcText)
{
	if(pcText)
		m_strEmptyText=pcText;
	else
		m_strEmptyText.Empty();
}

void TGridCtrl::OnPaint()
{
	if(GetItemCount()==0 || m_strEmptyText.IsEmpty())
	{
		Default();
		return;
	}

#ifdef SPECCLASSNAME
	if(strcmp(GetParent()->GetRuntimeClass()->m_lpszClassName, SPECCLASSNAME))
		return;
#endif

	CRect rc;
	GetClientRect(&rc);

	CHeaderCtrl* pHeader=GetHeaderCtrl();
	if(pHeader)
	{
		CRect rcHeader;
		pHeader->GetWindowRect(&rcHeader);
		rc.top+=rcHeader.Height();
	}

	CPaintDC dc(this);
	dc.FillSolidRect(&rc, GetBkColor());

	CGdiObject* pOldFont=dc.SelectStockObject(DEFAULT_GUI_FONT);
	int nOldBkMode=dc.SetBkMode(TRANSPARENT);
	COLORREF clrOldText=dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	dc.DrawText(_T("\n")+m_strEmptyText, -1, rc, DT_CENTER|DT_WORDBREAK|DT_NOPREFIX|DT_NOCLIP);

	dc.SelectObject(pOldFont);
	dc.SetBkMode(nOldBkMode);
	dc.SetTextColor(clrOldText);
}

void TGridCtrl::LoadSettings()
{
	int nColCount=GetColumnCount();
	if(!nColCount)
		return;

	CString strKey;
	strKey.Format(_T("Ctrl\\Grid\\%d"), GetDlgCtrlID());
	CWinApp* pApp=AfxGetApp();

	if(nColCount!=pApp->GetProfileInt(strKey, _T("ColCount"), 0))
		return;

	UINT uBytes;
	int* pnCols;

	if(pApp->GetProfileBinary(strKey, _T("ColWidth"), (LPBYTE*)&pnCols, &uBytes))
	{
		LVCOLUMN lvc;
		lvc.mask=LVCF_WIDTH;

		for(int i=0; i<nColCount; i++)
		{
			lvc.cx=pnCols[i];
			SetColumn(i, &lvc);
		}
	}

	if(pApp->GetProfileBinary(strKey, _T("ColOrder"), (LPBYTE*)&pnCols, &uBytes))
		SetColumnOrderArray(nColCount, pnCols);

	delete[] pnCols;
}

void TGridCtrl::StoreSettings()
{
	int nColCount=GetColumnCount();

	CString strKey;
	strKey.Format(_T("Ctrl\\Grid\\%d"), GetDlgCtrlID());
	CWinApp* pApp=AfxGetApp();

	pApp->WriteProfileInt(strKey, _T("ColCount"), nColCount);

	if(!nColCount)
		return;

	int* pnCols=new int[nColCount];
	UINT uBytes=nColCount*sizeof(int);

	LVCOLUMN lvc;
	lvc.mask=LVCF_WIDTH;

	for(int i=0; i<nColCount; i++)
	{
		GetColumn(i, &lvc);
		pnCols[i]=lvc.cx;
	}

	pApp->WriteProfileBinary(strKey, _T("ColWidth"), (LPBYTE)pnCols, uBytes);

	GetColumnOrderArray(pnCols, nColCount);
	pApp->WriteProfileBinary(strKey, _T("ColOrder"), (LPBYTE)pnCols, uBytes);

	delete[] pnCols;
}


TOLNS_END
