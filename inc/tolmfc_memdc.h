
#ifndef _INC_TOLMFC_MEMDC_H
#define _INC_TOLMFC_MEMDC_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolmfc.h>

TOLNS_BEGIN


//////////////////////////////////////////////////
// TMemDC

class TOLMFCDECL TMemDC
{
public:
	TMemDC(HDC hDC, LPCRECT prc=NULL);
	virtual ~TMemDC();

	HDC m_hDC;

protected:
	RECT m_rc;
	HDC m_hDCOrg;
	HBITMAP m_hbm;
	HGDIOBJ m_hbmOld;
};


//////////////////////////////////////////////////
// CMemDC


class CMemDC : public CDC
{
public:
	CMemDC(CDC* pDC);
	virtual ~CMemDC();

	//CMemDC* operator->(); // Allow usage as a pointer
	//operator CMemDC*(); // Allow usage as a pointer

protected:
	CRect    m_rc;		// Rectangle of drawing area.
	CDC*     m_pdcOrg;	// Saves CDC passed in constructor
	CBitmap  m_bm;		// Offscreen bitmap
	CBitmap* m_pbmOld;	// bitmap originally found in CMemDC
	BOOL     m_bMemDC;	// TRUE if CDC really is a Memory DC.
};


TOLNS_END

#endif /* _INC_TOLMFC_MEMDC_H */
