
#include "stdafx.h"
#include <tolmfc_aboutdlg.h>
#include <tolc_version.h>

TOLNS_BEGIN


#define IDD_ABOUT  (100)
#define IDR_MAIN   (128)


BEGIN_MESSAGE_MAP(TAboutDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


const CRect TAboutDlg::m_rcDlgPadding(20, 25, 20, 15);
const CSize TAboutDlg::m_szButton(74, 23);


TAboutDlg::TAboutDlg(CSize szIcon, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ABOUT, pParent)
	, m_szIcon(szIcon)
{
	m_hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
	m_hIcon=(HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAIN), IMAGE_ICON, m_szIcon.cx, m_szIcon.cy, 0);

	Char pchPath[TOL_MAXPATH];
	GetModuleFileName(AfxGetResourceHandle(), pchPath, TOL_MAXPATH);

	Char pchData[TOL_MAXSTR];

	if(Version_GetSectionData(pchPath, _T("ProductName"), pchData)==VERSION_SUCCESS)
		m_strInformation=pchData;

	if(Version_GetSectionData(pchPath, _T("ProductVersion"), pchData)==VERSION_SUCCESS)
	{
		m_strInformation+=_T(" ");
		m_strInformation+=pchData;
	}

	if(m_strInformation.IsEmpty())
		m_strInformation.LoadString(AFX_IDS_APP_TITLE);

	if(Version_GetSectionData(pchPath, _T("LegalCopyright"), pchData)==VERSION_SUCCESS)
		m_strCopyright=pchData;
}

TAboutDlg::~TAboutDlg()
{
	DestroyIcon(m_hIcon);
}

BOOL TAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWndNext;
	CWnd* pWnd=GetWindow(GW_CHILD);
	while(pWnd)
	{
		pWndNext=pWnd->GetWindow(GW_HWNDNEXT);
		pWnd->DestroyWindow();
		pWnd=pWndNext;
	}

	HDC hdc=::GetDC(m_hWnd);
	HGDIOBJ hOldFont=SelectObject(hdc, m_hFont);

	GetTextExtentPoint32(hdc, m_strInformation, m_strInformation.GetLength(), &m_szText);

	SIZE sz;
	GetTextExtentPoint32(hdc, m_strCopyright, m_strCopyright.GetLength(), &sz);
	m_nDistance=sz.cx/m_strCopyright.GetLength()*4;

	if(sz.cx>m_szText.cx)
		m_szText.cx=sz.cx;

	m_szText.cx+=30;

	SelectObject(hdc, hOldFont);
	::ReleaseDC(m_hWnd, hdc);

	sz.cx=m_rcDlgPadding.left
		+m_szIcon.cx
		+m_nDistance
		+m_szText.cx
		+m_rcDlgPadding.right;

	sz.cy=m_rcDlgPadding.top
		+m_szIcon.cy;

	int cy=m_rcDlgPadding.top
		+m_szText.cy
		+m_szText.cy
		+m_szText.cy;

	if(cy>sz.cy)
		sz.cy=cy;

	sz.cy+=m_nDistance;
	cy=sz.cy;
	sz.cy+=m_szButton.cy+m_rcDlgPadding.bottom;

	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);

	sz.cx+=rcWindow.Width()-rcClient.Width();
	sz.cy+=rcWindow.Height()-rcClient.Height();

	//MoveWindow(rcWindow.left, rcWindow.top, sz.cx, sz.cy);
	SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);

	HWND hwnd=CreateWindowEx(0, _T("Button"), _T("OK"), WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_DEFPUSHBUTTON, sz.cx-m_szButton.cx-m_rcDlgPadding.right, cy, m_szButton.cx, m_szButton.cy, m_hWnd, (HMENU)(IDOK), AfxGetInstanceHandle(), NULL);
	if(hwnd)
		::SendMessage(hwnd, WM_SETFONT, (WPARAM)m_hFont, 0);

	return TRUE;
}

void TAboutDlg::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc=::BeginPaint(m_hWnd, &ps);

	int nOldBkMode=SetBkMode(hdc, TRANSPARENT);
	HGDIOBJ hOldFont=SelectObject(hdc, m_hFont);

	//CRect rc;
	//rc.left=0;
	//rc.right=m_rcDlgPadding.left+m_rcImgPadding.left+m_szIcon.cx+m_rcImgPadding.right;
	//rc.top=0;
	//rc.bottom=m_rcDlgPadding.top+m_rcImgPadding.top+m_szIcon.cy+m_rcImgPadding.bottom+m_rcDlgPadding.bottom;
	////rc.left=m_rcDlgPadding.left;
	////rc.right=m_rcDlgPadding.left+m_rcImgPadding.left+m_szIcon.cx+m_rcImgPadding.right;
	////rc.top=m_rcDlgPadding.top;
	////rc.bottom=m_rcDlgPadding.top+m_rcImgPadding.top+m_szIcon.cy+m_rcImgPadding.bottom;

	//FillRect(hdc, &rc, GetSysColorBrush(COLOR_WINDOW));
	//DrawEdge(hdc, &rc, EDGE_ETCHED, BF_RIGHT);

	DrawIconEx(hdc, m_rcDlgPadding.left, m_rcDlgPadding.top, m_hIcon, m_szIcon.cx, m_szIcon.cy, 0, NULL, DI_NORMAL);

	int left=m_rcDlgPadding.left
		+m_szIcon.cx
		+m_nDistance;

	int top=m_rcDlgPadding.top;
	TextOut(hdc, left, top, m_strInformation, m_strInformation.GetLength());

	top+=m_szText.cy+m_szText.cy;
	TextOut(hdc, left, top, m_strCopyright, m_strCopyright.GetLength());

	SelectObject(hdc, hOldFont);
	SetBkMode(hdc, nOldBkMode);
	::ReleaseDC(m_hWnd, hdc);
}


TOLNS_END
