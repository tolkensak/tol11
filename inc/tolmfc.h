
#ifndef _INC_TOLMFC_H
#define _INC_TOLMFC_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tol.h>


#ifdef TOLMFCDLL
	#ifdef _TOLMFC_INDEV
		#define TOLMFCDECL TOLEXP
	#else
		#define TOLMFCDECL TOLIMP
	#endif
#else
	#define TOLMFCDECL
#endif



#define ID_LANG_1                 34001
#define ID_LANG_N                 34199
#define ID_MENU_STYLE_1           34201
//#define ID_ICON_1               34301
//#define ID_ICON_N               34399

#define UM_UPDATE_UI       (WM_USER+901)
#define UPDATE_UI_LANG                1
#define UPDATE_UI_MENU                2
#define UPDATE_UI_ICON                3
#define TCFM_RESIZELASTCOL  (WM_APP+901)


#ifndef _TOLMFC_INDEV
	#ifdef TOLMFCDLL
		#ifdef _DEBUG
			#pragma comment(lib, "tolmfc11d.lib")
		#else
			#pragma comment(lib, "tolmfc11.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "libtolmfc11d.lib")
		#else
			#pragma comment(lib, "libtolmfc11.lib")
		#endif
	#endif
#endif


#endif /* _INC_TOLMFC_H */
