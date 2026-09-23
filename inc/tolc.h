
#ifndef _INC_TOLC_H
#define _INC_TOLC_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tol.h>


#ifdef TOLCDLL
	#ifdef _TOLC_INDEV
		#define TOLCDECL TOLEXP
	#else
		#define TOLCDECL TOLIMP
	#endif
#else
	#define TOLCDECL
#endif


#ifndef _TOLC_INDEV
	#ifdef TOLCDLL
		#ifdef _DEBUG
			#pragma comment(lib, "tolc11d.lib")
		#else
			#pragma comment(lib, "tolc11.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "libtolc11d.lib")
		#else
			#pragma comment(lib, "libtolc11.lib")
		#endif
	#endif
#endif


#endif /* _INC_TOLC_H */
