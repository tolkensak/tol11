
#ifndef _INC_TOLC_STRINGA_H
#define _INC_TOLC_STRINGA_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


TOLCDECL PCharA TOLAPI StrA_Make(int nLen); /* need call StrA_Kill() to free memory after use */
TOLCDECL PCharA TOLAPI StrA_Remake(PCharA pcSrc, int nLen); /* need call StrA_Kill() to free memory after use */

TOLCDECL PCharA TOLAPI StrA_Dup(PCCharA pcSrc); /* need call StrA_Kill() to free memory after use */
TOLCDECL PCharA TOLAPI StrA_DupN(PCCharA pcSrc, int nCount); /* need call StrA_Kill() to free memory after use */

TOLCDECL void TOLAPI StrA_Kill(PCharA pcSrc);

TOLCDECL int TOLAPI StrA_Len(PCCharA pcSrc);

TOLCDECL void TOLAPI StrA_Copy(PCharA pcDest, PCCharA pcSrc);
TOLCDECL void TOLAPI StrA_CopyN(PCharA pcDest, PCCharA pcSrc, int nCount);

TOLCDECL void TOLAPI StrA_Cat(PCharA pcDest, PCCharA pcSrc);
TOLCDECL void TOLAPI StrA_CatN(PCharA pcDest, PCCharA pcSrc, int nCount);

TOLCDECL PCharA TOLAPI StrA_Trim(PCharA pcSrc, PCCharA pcTrim);
TOLCDECL PCharA TOLAPI StrA_TrimL(PCharA pcSrc, PCCharA pcTrim);
TOLCDECL PCharA TOLAPI StrA_TrimR(PCharA pcSrc, PCCharA pcTrim);

TOLCDECL PCCharA TOLAPI StrA_Char(PCCharA pcSrc, CharA ch);
TOLCDECL PCCharA TOLAPI StrA_Str(PCCharA pcSrc, PCCharA pcFind);
TOLCDECL PCCharA TOLAPI StrA_StrN(PCCharA pcSrc, PCCharA pcFind, int nCount);

TOLCDECL int TOLAPI StrA_Cmp(PCCharA pc1, PCCharA pc2);
TOLCDECL int TOLAPI StrA_CmpN(PCCharA pc1, PCCharA pc2, int nCount);

TOLCDECL void TOLAPI StrA_RemoveCh(PCharA pcSrc, CharA chRemove);
TOLCDECL void TOLAPI StrA_Remove(PCharA pcSrc, PCCharA pcRemove);

TOLCDECL PCharA TOLAPI StrA_Token(PCharA pcSrc, PCCharA pcToken);
TOLCDECL int TOLAPI StrA_Format(PCharA pcDest, PCCharA pcFmt, ...);
TOLCDECL PCharA TOLAPI StrA_EnvVar(PCCharA pcVarName); /* after success call, need call StrA_Kill() to free memory after use */


TOLEXTC_END

#endif /* _INC_TOLC_STRINGA_H */
