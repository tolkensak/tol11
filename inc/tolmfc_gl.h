
#ifndef _INC_TOLMFC_GL_H
#define _INC_TOLMFC_GL_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

TOLNS_BEGIN

// TGLView

class TOLMFCDECL TGLView : public CView
{
	DECLARE_DYNAMIC(TGLView)

protected:
	TGLView();
	virtual ~TGLView();

public:
	afx_msg void OnDestroy();

protected:
	HPALETTE m_hPalette;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	BOOL SetupPixelFormat(HDC hDC);
	void SetupPalette();
	void ManagePalette();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();

	DECLARE_MESSAGE_MAP()
};

// TGLWnd

class TOLMFCDECL TGLWnd : public CWnd
{
protected:
	TGLWnd();
	virtual ~TGLWnd();

public:
	afx_msg void OnDestroy();

protected:
	HPALETTE m_hPalette;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	BOOL SetupPixelFormat(HDC hDC);
	void SetupPalette();
	void ManagePalette();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();

	DECLARE_MESSAGE_MAP()
};

TOLNS_END

#endif /* _INC_TOLMFC_GL_H */
