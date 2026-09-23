
#ifndef _INC_TOLC_TEXT_H
#define _INC_TOLC_TEXT_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


//#define _UTF32_SUPPORT

/* Text file code pages */
#define TFE_UTF8      0    /*65001*/
#define TFE_UTF8SIG   1    /*65001*/
#define TFE_UTF16LE   2    /*1200*/
#define TFE_UTF16BE   3    /*1201*/

#ifdef _UTF32_SUPPORT
#define TFE_UTF32LE   4    /*12000*/
#define TFE_UTF32BE   5    /*12001*/
#endif

/* Text file end styles */
#define TFES_WINDOWS   0    /*\r\n*/
#define TFES_UNIX      1    /*\n*/
#define TFES_MAC       2    /*\r*/


TOLCDECL UINT TOLAPI Text_GetEncoding(FILE* pFile);
TOLCDECL void TOLAPI Text_SetEncoding(FILE* pFile, UINT uEncoding);

 /* returned pointer must be delete by Str_Kill, if not null */
TOLCDECL LPSTR TOLAPI Text_ReadLineA(FILE* pFile, UINT uEncoding);
TOLCDECL LPWSTR TOLAPI Text_ReadLineW(FILE* pFile, UINT uEncoding);

TOLCDECL DWORD TOLAPI Text_WriteA(FILE* pFile, LPCSTR pcWrite, int cchWrite, UINT uEncoding);
TOLCDECL DWORD TOLAPI Text_WriteW(FILE* pFile, LPCWSTR pcWrite, int cchWrite, UINT uEncoding);

TOLCDECL DWORD TOLAPI Text_WriteLineA(FILE* pFile, LPCSTR pcWrite, int cchWrite, UINT uEncoding, UINT uEndStyle);
TOLCDECL DWORD TOLAPI Text_WriteLineW(FILE* pFile, LPCWSTR pcWrite, int cchWrite, UINT uEncoding, UINT uEndStyle);

#if defined(_UNICODE) || defined(UNICODE)
#define Text_ReadLine  Text_ReadLineW
#define Text_WriteLine Text_WriteLineW
#else
#define Text_ReadLine  Text_ReadLineA
#define Text_WriteLine Text_WriteLineA
#endif


TOLEXTC_END

#endif /* _INC_TOLC_TEXT_H */
