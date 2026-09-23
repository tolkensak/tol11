
#ifndef _INC_TOLC_STRING_H
#define _INC_TOLC_STRING_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tolc_stringa.h>
#include <tolc_stringw.h>


TOLEXTC_BEGIN


#if defined(_UNICODE) || defined(UNICODE)

#define Str_Make            StrW_Make
#define Str_Remake          StrW_Remake
#define Str_Dup             StrW_Dup
#define Str_DupN            StrW_DupN
#define Str_Kill            StrW_Kill
#define Str_Len             StrW_Len
#define Str_Cmp             StrW_Cmp
#define Str_CmpN            StrW_CmpN
#define Str_Copy            StrW_Copy
#define Str_CopyN           StrW_CopyN
#define Str_Cat             StrW_Cat
#define Str_CatN            StrW_CatN
#define Str_Trim            StrW_Trim
#define Str_TrimL           StrW_TrimL
#define Str_TrimR           StrW_TrimR
#define Str_Char            StrW_Char
#define Str_Str             StrW_Str
#define Str_StrN            StrW_StrN
#define Str_Token           StrW_Token
#define Str_RemoveCh        StrW_RemoveCh
#define Str_Remove          StrW_Remove
#define Str_Format          StrW_Format
#define Str_EnvVar          StrW_EnvVar

#else /* _UNICODE || UNICODE */

#define Str_Make            StrA_Make
#define Str_Remake          StrA_Remake
#define Str_Dup             StrA_Dup
#define Str_DupN            StrA_DupN
#define Str_Kill            StrA_Kill
#define Str_Len             StrA_Len
#define Str_Cmp             StrA_Cmp
#define Str_CmpN            StrA_CmpN
#define Str_Copy            StrA_Copy
#define Str_CopyN           StrA_CopyN
#define Str_Cat             StrA_Cat
#define Str_CatN            StrA_CatN
#define Str_Trim            StrA_Trim
#define Str_TrimL           StrA_TrimL
#define Str_TrimR           StrA_TrimR
#define Str_Char            StrA_Char
#define Str_Str             StrA_Str
#define Str_StrN            StrA_StrN
#define Str_Token           StrA_Token
#define Str_RemoveCh        StrA_RemoveCh
#define Str_Remove          StrA_Remove
#define Str_Format          StrA_Format
#define Str_EnvVar          StrA_EnvVar

#endif /* _UNICODE || UNICODE */


TOLCDECL PCharW TOLAPI Str_A2W(PCCharA pcSrc, int nCount, Uint uCodePage);
TOLCDECL PCharA TOLAPI Str_W2A(PCCharW pcSrc, int nCount, Uint uCodePage);

TOLCDECL int TOLAPI Str_PrefixSize(PChar pcDest, int cchDest, Qword qwSize);


TOLEXTC_END

#endif /* _INC_TOLC_STRING_H */
