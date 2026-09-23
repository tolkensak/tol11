
#include "stdafx.h"
#include <tolmfc_memdc.h>


TOLNS_BEGIN


// TMemDC

TMemDC::TMemDC(HDC hDC, LPCRECT prc)
{
	ASSERT(hDC!=NULL);

	if(prc)
		CopyRect(&m_rc, prc);
	else
		GetClipBox(hDC, &m_rc);

	LPtoDP(hDC, (LPPOINT)&m_rc, 2);

	m_hDCOrg=hDC;
	m_hDC=CreateCompatibleDC(hDC);
	m_hbm=CreateCompatibleBitmap(hDC, m_rc.right-m_rc.left, m_rc.bottom-m_rc.top);
	m_hbmOld=SelectObject(m_hDC, m_hbm);
	SetWindowOrgEx(m_hDC, m_rc.left, m_rc.top, NULL);
}

TMemDC::~TMemDC()
{
	BitBlt(m_hDCOrg, m_rc.left, m_rc.top, m_rc.right-m_rc.left, m_rc.bottom-m_rc.top, m_hDC, m_rc.left, m_rc.top, SRCCOPY);
	SelectObject(m_hDC, m_hbmOld);
	DeleteObject(m_hbm);
	DeleteDC(m_hDC);
}


//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email: keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History:
//		10/3/97
//			Fixed scrolling bug.
//			Added print support.
//		25/2/98
//			fixed minor assertion bug
//
// This class implements a memory Device Context


CMemDC::CMemDC(CDC* pDC) : CDC()
{
	ASSERT(pDC!=NULL);

	m_pdcOrg=pDC;
	m_pbmOld=NULL;
#ifndef WCE_NO_PRINTING
	m_bMemDC=!pDC->IsPrinting();
#else
	m_bMemDC=FALSE;
#endif

	if(m_bMemDC)
	{
		pDC->GetClipBox(&m_rc);
		CreateCompatibleDC(pDC);
		m_bm.CreateCompatibleBitmap(pDC, m_rc.Width(), m_rc.Height());
		m_pbmOld=SelectObject(&m_bm);

#ifndef _WIN32_WCE
		SetWindowOrg(m_rc.left, m_rc.top);
#endif
	}
	else // Make a copy of the relevent parts of the current DC for printing
	{
#ifndef WCE_NO_PRINTING
		m_bPrinting=pDC->m_bPrinting;
#endif
		m_hDC=pDC->m_hDC;
		m_hAttribDC=pDC->m_hAttribDC;
	}
}

CMemDC::~CMemDC()
{
	if(m_bMemDC)
	{
		// Copy the offscreen bitmap onto the screen.
		m_pdcOrg->BitBlt(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), this, m_rc.left, m_rc.top, SRCCOPY);
		//Swap back the original bitmap.
		SelectObject(m_pbmOld);
	}
	else
	{
		// All we need to do is replace the DC with an illegal value,
		// this keeps us from accidently deleting the handles associated with
		// the CDC that was passed to the constructor.
		m_hDC=m_hAttribDC=NULL;
	}
}

//CMemDC* CMemDC::operator->() {return this;}
//operator CMemDC::CMemDC*() {return this;}


TOLNS_END
