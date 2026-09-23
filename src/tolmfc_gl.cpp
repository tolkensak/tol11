
#include "stdafx.h"
#include <tolmfc_gl.h>

#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

TOLNS_BEGIN

// TGLView

IMPLEMENT_DYNAMIC(TGLView, CView)

BEGIN_MESSAGE_MAP(TGLView, CView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

TGLView::TGLView()
{
	m_hPalette=NULL;
}

TGLView::~TGLView()
{
	if(m_hPalette)
		::DeleteObject(m_hPalette);
}

BOOL TGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CView::PreCreateWindow(cs))
		return FALSE;

    cs.style|=WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
}

int TGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CView::OnCreate(lpCreateStruct)==-1)
		return -1;

	HDC hDC=::GetDC(m_hWnd);
	if(!SetupPixelFormat(hDC))
		return -1;

	// Create the OpenGL rendering context
	HGLRC hRC=wglCreateContext(hDC);
	if(!hRC) // This system cannot run OpenGL
	{
		TRACE("Error creating OpenGL rendering context...\n");

		ASSERT(FALSE);

		AfxMessageBox(_T("Cannot initialize OpenGL...quitting."), MB_ICONERROR);

		return -1; // will fail new document creation...
	}

	wglMakeCurrent(hDC, hRC); // We now make it the current rendering context so we might set our clear color
	return 0;
}

void TGLView::OnDestroy()
{
	CView::OnDestroy();

	HDC hDC=wglGetCurrentDC();
	HGLRC hRC=wglGetCurrentContext();

	wglMakeCurrent(NULL, NULL);

	// Shut down OpenGL by deleting the rendering context
	if(hRC)
		wglDeleteContext(hRC);

	if(hDC)
		::ReleaseDC(m_hWnd, hDC);
}

// TGLView drawing
BOOL TGLView::OnEraseBkgnd(CDC* pDC) 
{
	// Don't erase view... let OpenGL handle that for us...
	return TRUE;
}

BOOL TGLView::SetupPixelFormat(HDC hDC)
{
	// Initialize OpenGL parameters (Win32)
	PIXELFORMATDESCRIPTOR pfd; 
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR); 
	pfd.nVersion=1; 
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER; 
	pfd.dwLayerMask=PFD_MAIN_PLANE; 
	pfd.iPixelType=PFD_TYPE_RGBA; 
	pfd.cColorBits=8; 
	pfd.cDepthBits=16; 
	pfd.cAccumBits=0; 
	pfd.cStencilBits=0; 
	
	//// Initialize OpenGL parameters (default)
	//PIXELFORMATDESCRIPTOR pfd=
	//{
	//	sizeof(PIXELFORMATDESCRIPTOR)	// Structure size
	//	, 1						// Version number
	//	, PFD_DRAW_TO_WINDOW	// Property flags
	//	| PFD_SUPPORT_OPENGL
	//	| PFD_DOUBLEBUFFER		// (remove if no double buf)
	//	, PFD_TYPE_RGBA			// PixelType
	//	, 24					// 24-bit color
	//	, 0, 0, 0, 0, 0, 0		// Color bits and shift
	//	, 0, 0, 0, 0, 0, 0, 0	// Alpha and accum buffer bits
	//	, 32					// 32-bit depth buffer
	//	, 0, 0					// No stencil or aux buffer
	//	, PFD_MAIN_PLANE		// Layer type
	//	, 0						// Reserved
	//	, 0, 0, 0				// Unsupported
	//};

	// Tell GDI to convert device context from Win32 to OpenGL.
    int nPixelFormat=ChoosePixelFormat(hDC, &pfd);
    if(!nPixelFormat)
	{
		// This system cannot run OpenGL
		TRACE("Error retrieving pixel format index...\n");
		ASSERT(FALSE);
		AfxMessageBox(_T("Cannot initialize OpenGL...quitting."), MB_ICONERROR);
		return FALSE; // will fail new document creation...
	}

    if(!SetPixelFormat(hDC, nPixelFormat, &pfd))
	{
		// This system cannot run OpenGL
		TRACE("Error setting new pixel format...\n");
		ASSERT(FALSE);
		AfxMessageBox(_T("Cannot initialize OpenGL...quitting."), MB_ICONERROR);
		return FALSE; // will fail new document creation...
	}

	// Update the PIXELFORMATDESCRIPTOR structure once
	// the device context has been modified.
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// The PIXELFORMATDESCRIPTOR has been updated, so we now
	// determine whether to create and manage a custom palette.
	if(pfd.dwFlags&PFD_NEED_PALETTE)
		SetupPalette(); // We do, so build a new palette...
	return TRUE;
}

// TGLView manage our palette
void TGLView::SetupPalette()
{
	struct LOGICALPALETTE
	{
		WORD wVersion;
		WORD wNumEntries;
		PALETTEENTRY peaEntries[256];
	} pal={0x300, 256};

	BYTE cRed[]={0, 36, 72, 109, 145, 182, 218, 255};
	BYTE cGreen[]={0, 36, 72, 109, 145, 182, 218, 255};
	BYTE cBlue[]={0, 85, 170, 255};

	// Fill our logical palette structure with color data
	for(int i=0; i<256; i++)
	{
		pal.peaEntries[i].peRed=cRed[i&0x07];
		pal.peaEntries[i].peGreen=cGreen[(i>>0x03)&0x07];
		pal.peaEntries[i].peBlue=cBlue[(i>>0x06)&0x03];
		pal.peaEntries[i].peFlags=0;
	}

	// Build our palette
	m_hPalette=CreatePalette((LOGPALETTE*)&pal);
}

void TGLView::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CView::OnPaletteChanged(pFocusWnd);
	ManagePalette();
}

BOOL TGLView::OnQueryNewPalette() 
{
	ManagePalette();
	return CView::OnQueryNewPalette();
}

void TGLView::ManagePalette()
{
	if(m_hPalette)
	{
		CClientDC dc(this);
		SelectPalette(dc.m_hDC, m_hPalette, FALSE);
		dc.RealizePalette();
		Invalidate();
	}
}

// TGLWnd

BEGIN_MESSAGE_MAP(TGLWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

TGLWnd::TGLWnd()
{
	m_hPalette=NULL;
}

TGLWnd::~TGLWnd()
{
	if(m_hPalette)
		::DeleteObject(m_hPalette);
}

BOOL TGLWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style|=WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return TRUE;
}

int TGLWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CWnd::OnCreate(lpCreateStruct)==-1)
		return -1;

	HDC hDC=::GetDC(m_hWnd);
	if(!SetupPixelFormat(hDC))
		return -1;

	// Create the OpenGL rendering context
	HGLRC hRC=wglCreateContext(hDC);
	if(!hRC) // This system cannot run OpenGL
	{
		TRACE("Error creating OpenGL rendering context...\n");

		ASSERT(FALSE);

		AfxMessageBox(_T("Cannot initialize OpenGL...quitting."), MB_ICONERROR);

		return -1; // will fail new document creation...
	}

	wglMakeCurrent(hDC, hRC); // We now make it the current rendering context so we might set our clear color
	return 0;
}

void TGLWnd::OnDestroy()
{
	CWnd::OnDestroy();

	HDC hDC=wglGetCurrentDC();
	HGLRC hRC=wglGetCurrentContext();

	wglMakeCurrent(NULL, NULL);

	// Shut down OpenGL by deleting the rendering context
	if(hRC)
		wglDeleteContext(hRC);

	if(hDC)
		::ReleaseDC(m_hWnd, hDC);
}

// TGLWnd drawing
BOOL TGLWnd::OnEraseBkgnd(CDC* pDC) 
{
	// Don't erase view... let OpenGL handle that for us...
	return TRUE;
}

BOOL TGLWnd::SetupPixelFormat(HDC hDC)
{
	// Initialize OpenGL parameters (Win32)
	PIXELFORMATDESCRIPTOR pfd; 
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR); 
	pfd.nVersion=1; 
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER; 
	pfd.dwLayerMask=PFD_MAIN_PLANE; 
	pfd.iPixelType=PFD_TYPE_RGBA; 
	pfd.cColorBits=8; 
	pfd.cDepthBits=16; 
	pfd.cAccumBits=0; 
	pfd.cStencilBits=0; 
	
	//// Initialize OpenGL parameters (default)
	//PIXELFORMATDESCRIPTOR pfd=
	//{
	//	sizeof(PIXELFORMATDESCRIPTOR)	// Structure size
	//	, 1						// Version number
	//	, PFD_DRAW_TO_WINDOW	// Property flags
	//	| PFD_SUPPORT_OPENGL
	//	| PFD_DOUBLEBUFFER		// (remove if no double buf)
	//	, PFD_TYPE_RGBA			// PixelType
	//	, 24					// 24-bit color
	//	, 0, 0, 0, 0, 0, 0		// Color bits and shift
	//	, 0, 0, 0, 0, 0, 0, 0	// Alpha and accum buffer bits
	//	, 32					// 32-bit depth buffer
	//	, 0, 0					// No stencil or aux buffer
	//	, PFD_MAIN_PLANE		// Layer type
	//	, 0						// Reserved
	//	, 0, 0, 0				// Unsupported
	//};

	// Tell GDI to convert device context from Win32 to OpenGL.
    int nPixelFormat=ChoosePixelFormat(hDC, &pfd);
    if(!nPixelFormat)
	{
		// This system cannot run OpenGL
		TRACE("Error retrieving pixel format index...\n");
		ASSERT(FALSE);
		AfxMessageBox(_T("Cannot initialize OpenGL...quitting."), MB_ICONERROR);
		return FALSE; // will fail new document creation...
	}

    if(!SetPixelFormat(hDC, nPixelFormat, &pfd))
	{
		// This system cannot run OpenGL
		TRACE("Error setting new pixel format...\n");
		ASSERT(FALSE);
		AfxMessageBox(_T("Cannot initialize OpenGL...quitting."), MB_ICONERROR);
		return FALSE; // will fail new document creation...
	}

	// Update the PIXELFORMATDESCRIPTOR structure once
	// the device context has been modified.
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// The PIXELFORMATDESCRIPTOR has been updated, so we now
	// determine whether to create and manage a custom palette.
	if(pfd.dwFlags&PFD_NEED_PALETTE)
		SetupPalette(); // We do, so build a new palette...
	return TRUE;
}

// TGLWnd manage our palette
void TGLWnd::SetupPalette()
{
	struct LOGICALPALETTE
	{
		WORD wVersion;
		WORD wNumEntries;
		PALETTEENTRY peaEntries[256];
	} pal={0x300, 256};

	BYTE cRed[]={0, 36, 72, 109, 145, 182, 218, 255};
	BYTE cGreen[]={0, 36, 72, 109, 145, 182, 218, 255};
	BYTE cBlue[]={0, 85, 170, 255};

	// Fill our logical palette structure with color data
	for(int i=0; i<256; i++)
	{
		pal.peaEntries[i].peRed=cRed[i&0x07];
		pal.peaEntries[i].peGreen=cGreen[(i>>0x03)&0x07];
		pal.peaEntries[i].peBlue=cBlue[(i>>0x06)&0x03];
		pal.peaEntries[i].peFlags=0;
	}

	// Build our palette
	m_hPalette=CreatePalette((LOGPALETTE*)&pal);
}

void TGLWnd::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CWnd::OnPaletteChanged(pFocusWnd);
	ManagePalette();
}

BOOL TGLWnd::OnQueryNewPalette() 
{
	ManagePalette();
	return CWnd::OnQueryNewPalette();
}

void TGLWnd::ManagePalette()
{
	if(m_hPalette)
	{
		CClientDC dc(this);
		SelectPalette(dc.m_hDC, m_hPalette, FALSE);
		dc.RealizePalette();
		Invalidate();
	}
}

TOLNS_END
