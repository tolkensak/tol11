
#ifndef _INC_TOLC_STRINGW_H
#define _INC_TOLC_STRINGW_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


TOLCDECL PCharW TOLAPI StrW_Make(int nLen); /* need call StrW_Kill() to free memory after use */
TOLCDECL PCharW TOLAPI StrW_Remake(PCharW pcSrc, int nLen); /* need call StrW_Kill() to free memory after use */

TOLCDECL PCharW TOLAPI StrW_Dup(PCCharW pcSrc); /* need call StrW_Kill() to free memory after use */
TOLCDECL PCharW TOLAPI StrW_DupN(PCCharW pcSrc, int nCount); /* need call StrW_Kill() to free memory after use */

TOLCDECL void TOLAPI StrW_Kill(PCharW pcSrc);

TOLCDECL int TOLAPI StrW_Len(PCCharW pcSrc);

TOLCDECL void TOLAPI StrW_Copy(PCharW pcDest, PCCharW pcSrc);
TOLCDECL void TOLAPI StrW_CopyN(PCharW pcDest, PCCharW pcSrc, int nCount);

TOLCDECL void TOLAPI StrW_Cat(PCharW pcDest, PCCharW pcSrc);
TOLCDECL void TOLAPI StrW_CatN(PCharW pcDest, PCCharW pcSrc, int nCount);

TOLCDECL PCharW TOLAPI StrW_Trim(PCharW pcSrc, PCCharW pcTrim);
TOLCDECL PCharW TOLAPI StrW_TrimL(PCharW pcSrc, PCCharW pcTrim);
TOLCDECL PCharW TOLAPI StrW_TrimR(PCharW pcSrc, PCCharW pcTrim);

TOLCDECL PCCharW TOLAPI StrW_Char(PCCharW pcSrc, CharW ch);
TOLCDECL PCCharW TOLAPI StrW_Str(PCCharW pcSrc, PCCharW pcFind);
TOLCDECL PCCharW TOLAPI StrW_StrN(PCCharW pcSrc, PCCharW pcFind, int nCount);

TOLCDECL int TOLAPI StrW_Cmp(PCCharW pc1, PCCharW pc2);
TOLCDECL int TOLAPI StrW_CmpN(PCCharW pc1, PCCharW pc2, int nCount);

TOLCDECL void TOLAPI StrW_RemoveCh(PCharW pcSrc, CharW chRemove);
TOLCDECL void TOLAPI StrW_Remove(PCharW pcSrc, PCCharW pcRemove);

TOLCDECL PCharW TOLAPI StrW_Token(PCharW pcSrc, PCCharW pcToken);
TOLCDECL int TOLAPI StrW_Format(PCharW pcDest, PCCharW pcFmt, ...);
TOLCDECL PCharW TOLAPI StrW_EnvVar(PCCharW pcVarName); /* after success call, need call StrW_Kill() to free memory after use */

TOLCDECL void TOLAPI StrW_ChangeEndian(PCharW pcSrc);


TOLEXTC_END

#endif /* _INC_TOLC_STRINGW_H */
