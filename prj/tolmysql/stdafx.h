
#ifndef _INC_STDAFX_H
#define _INC_STDAFX_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "targetver.h"

#include <windows.h>
#include <tchar.h>

#define _TOLMYSQL_INDEV

#ifdef _WINDLL
#define TOLCDLL
#define TOLCPPDLL
#define TOLMYSQLDLL
#endif

#endif // _INC_STDAFX_H
