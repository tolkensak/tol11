
#ifndef _INC_TOLC_LANG_H
#define _INC_TOLC_LANG_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


#ifndef _TOLC_TLANG_DEFINED
#define _TOLC_TLANG_DEFINED
typedef PVoid TLANG;
#endif /*_TOLC_TLANG_DEFINED*/


#define TLANG_NAME_FORMAT_FULL   LOCALE_SLANGUAGE
#define TLANG_NAME_FORMAT_SHORT2 LOCALE_SISO639LANGNAME
#define TLANG_NAME_FORMAT_SHORT3 LOCALE_SABBREVLANGNAME
#define TLANG_NAME_FORMAT_ASCII  LOCALE_SENGLANGUAGE
#define TLANG_NAME_FORMAT_NATIVE LOCALE_SNATIVELANGNAME

#define TLANG_MASK_LANGID 1
#define TLANG_MASK_NAME   2
#define TLANG_MASK_PATH   4


TOLCDECL TLANG TOLAPI Lang_Make(Uint uNameFormat);
TOLCDECL void TOLAPI Lang_Kill(TLANG tLang);

TOLCDECL int TOLAPI Lang_Count(TLANG tLang);
TOLCDECL void TOLAPI Lang_Init(TLANG tLang, PCChar pcExePath);

TOLCDECL LANGID TOLAPI Lang_Id(TLANG tLang, int nIndex);
TOLCDECL int TOLAPI Lang_Name(TLANG tLang, int nIndex, PChar pcName, int nNameBufferLen);
TOLCDECL int TOLAPI Lang_Path(TLANG tLang, int nIndex, PChar pcPath, int nPathBufferLen);
TOLCDECL void TOLAPI Lang_Menu(TLANG tLang, HMENU hParentMenu, int nPos, UINT uFirstID, HINSTANCE hTextInst, UINT uTextID);


#define TLANG_ITEM_RESULT_SUCCESS      0
#define TLANG_ITEM_RESULT_BAD_ARG      1
#define TLANG_ITEM_RESULT_NO_DATA      2
#define TLANG_ITEM_RESULT_NAME_SMALL   4
#define TLANG_ITEM_RESULT_PATH_SMALL   8

typedef struct tagLangItem
{
	int nIndex;
	Uint uMask;
	LANGID lid;
	PChar pcName;
	int nNameBufferLen;
	PChar pcPath;
	int nPathBufferLen;
} T_LANGITEM, *PT_LANGITEM;

TOLCDECL int TOLAPI Lang_Item(TLANG tLang, PT_LANGITEM pItem);


typedef struct tagLangFind
{
	Uint uMask;
	LANGID lid;
	PCChar pcName;
	PCChar pcPath;
} T_LANGFIND, *PT_LANGFIND;

TOLCDECL int TOLAPI Lang_Find(TLANG tLang, PT_LANGFIND pFind);


/***************************************************************************************************
	Free funcions     ******************************************************************************/

TOLCDECL LANGID Lang_GetUserDefaultLangID();
TOLCDECL int Lang_GetName(LANGID lid, PChar pcName, int nNameBufferLen, UINT uFormat);


TOLEXTC_END

#endif /* _INC_TOLC_LANG_H */
