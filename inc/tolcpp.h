
#ifndef _INC_TOLCPP_H
#define _INC_TOLCPP_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tol.h>


#ifdef TOLCPPDLL
	#ifdef _TOLCPP_INDEV
		#define TOLCPPDECL        TOLEXP
		#define TOLTEMPLDECL    TOLEXP
	#else
		#define TOLCPPDECL        TOLIMP
		#define TOLTEMPLDECL
	#endif
#else
	#define TOLCPPDECL
	#define TOLTEMPLDECL
#endif


#ifndef _TOLCPP_INDEV
	#ifdef TOLCPPDLL
		#ifdef _DEBUG
			#pragma comment(lib, "tolcpp11d.lib")
		#else
			#pragma comment(lib, "tolcpp11.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "libtolcpp11d.lib")
		#else
			#pragma comment(lib, "libtolcpp11.lib")
		#endif
	#endif
	#define _TOLCPP_IMPORTED
#endif


#endif // _INC_TOLCPP_H
