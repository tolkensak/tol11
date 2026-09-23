
#ifndef _INC_TOL_H
#define _INC_TOL_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


/*************************************************************************************************************
 Calling convention
*************************************************************************************************************/

#ifdef _MAC
	#ifdef _68K_
		#define TOLCALL __pascal
	#else
		#define TOLCALL
	#endif

	#ifdef DOSWIN32
		#define TOLAPI  __cdecl
	#else
		#define TOLAPI
	#endif
#elif (_MSC_VER>=800) || defined(_STDCALL_SUPPORTED) /* _MAC */
	#ifdef _WINDLL
		#define TOLCALL  __stdcall
		#define TOLAPI   __stdcall
	#else
		#define TOLCALL
		#define TOLAPI
	#endif
#else /* _MAC */
	#define TOLCALL
	#define TOLAPI
#endif /* _MAC */



/*************************************************************************************************************
 Extern
*************************************************************************************************************/

#ifdef __cplusplus
	#define TOLEXTC           extern "C"
	#define TOLEXTC_BEGIN     extern "C" {
	#define TOLEXTC_END       }
#else
	#define TOLEXTC
	#define TOLEXTC_BEGIN
	#define TOLEXTC_END
#endif



/*************************************************************************************************************
 Function types
*************************************************************************************************************/

#ifdef _MSC_VER
	#define TOLINL     __inline
	#define TOLIMP     __declspec(dllimport)
	#define TOLEXP     __declspec(dllexport)
#else
	#ifdef __cplusplus
		#define TOLINL inline
	#else
		#define TOLINL
	#endif

	#define TOLIMP
	#define TOLEXP
#endif



/*************************************************************************************************************
 Namespace
*************************************************************************************************************/

#ifdef __cplusplus
	#define TOLNS          ::tol
	#define TOLNS_BEGIN    namespace tol {
	#define TOLNS_END      }
	#define TOLNS_USING    using namespace tol;
#else
	#define TOLNS
	#define TOLNS_BEGIN
	#define TOLNS_END
	#define TOLNS_USING
#endif



/*************************************************************************************************************
 Constants
*************************************************************************************************************/

#define TOL_MAXSTR    1024
#define TOL_MAXPATH   1024

#ifndef NULL
	#ifdef __cplusplus
		#define NULL 0
	#else
		#define NULL ((void*)0)
	#endif
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif



/*************************************************************************************************************
 Data Types
*************************************************************************************************************/

/* void */
typedef void Void, *PVoid,                               *Handle;
typedef const Void *PCVoid;

/* bool */
//#ifdef __BOOL_DEFINED
//	typedef bool Bool, *PBool;
//	typedef const Bool *PCBool;
//#else
	typedef int Bool, *PBool;
	typedef const Bool *PCBool;
//#endif

/* char */
typedef char Tiny, *PTiny;
typedef const Tiny *PCTiny;
typedef unsigned char Utiny, *PUtiny,                    Byte, *PByte;
typedef const Utiny *PCUtiny,                            *PCByte;

/* short */
typedef short Short, *PShort;
typedef const Short *PCShort;
typedef unsigned short Ushort, *PUshort,                 Word, *PWord;
typedef const Ushort *PCUshort,                          *PCWord;

/* int */
typedef int Int, *PInt;
typedef const Int *PCInt;
typedef unsigned int Uint, *PUint;
typedef const Uint *PCUint;

/* long */
typedef long Long, *PLong;
typedef const Long *PCLong;
typedef unsigned long Ulong, *PUlong,                    Dword, *PDword;
typedef const Ulong *PCUlong,                            *PCDword;

/* huge */
#if (!defined (_MAC) && (!defined(MIDL_PASS) || defined(__midl)) && (!defined(_M_IX86) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64)))
	typedef __int64 huge;
	typedef unsigned __int64 uhuge;
	#define TOL_MAXHUGE                         (0x7fffffffffffffff)
#elif defined(_MAC) && defined(_MAC_INT_64)
	typedef __int64 huge;
	typedef unsigned __int64 uhuge;
	#define TOL_MAXHUGE                         (0x7fffffffffffffff)
#else
	typedef double huge;
	typedef double uhuge;
#endif //_MAC and int64

typedef huge Huge, *PHuge;
typedef const Huge *PCHuge;
typedef uhuge Uhuge, *PUhuge,                              Qword, *PQword;
typedef const Uhuge *PCUhuge,                              *PCQword;

/* float */
typedef float Float, *PFloat;
typedef const Float *PCFloat;

/* double */
typedef double Double, *PDouble;
typedef const double CDouble, *PCDouble;

/* char */
typedef char CharA, *PCharA;
typedef const CharA *PCCharA;

/* wchar */
typedef wchar_t CharW, *PCharW;
typedef const CharW *PCCharW;

/* tchar */
#if defined(UNICODE) || defined(_UNICODE)
	typedef CharW    Char;
	typedef PCharW   PChar;
	typedef PCCharW  PCChar;
	/*#define __T(x) L ## x*/
#else /* _UNICODE */
	typedef CharA    Char;
	typedef PCharA   PChar;
	typedef PCCharA  PCChar;
	/*#define __T(x) x*/
#endif /* _UNICODE */

/*#define _T(x) __T(x)*/


#endif /* _INC_TOL_H */
