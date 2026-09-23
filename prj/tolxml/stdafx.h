
#ifndef _INC_STDAFX_H
#define _INC_STDAFX_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>        // MFC core and standard components
#include <afxext.h>        // MFC extensions

#include <shlwapi.h> // path operations

#define _TOLXML_INDEV

#ifdef _WINDLL
#define TOLCDLL
#define TOLCPPDLL
#define TOLXMLDLL
#endif

#include <tolc_util.h>
#include <tolcpp_string.h>

#endif // _INC_STDAFX_H
