
#ifndef _INC_TOLC_LIST_H
#define _INC_TOLC_LIST_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolc.h>

TOLEXTC_BEGIN


#ifndef _TOLC_TLIST_DEFINED
#define _TOLC_TLIST_DEFINED
typedef LPVOID POS;
typedef LPVOID TLIST;
typedef LPVOID LDATA, *PLDATA;
typedef const LDATA *PCLDATA;
#endif

typedef int (TOLCALL *PFNTLISTSORT)(LDATA data1, LDATA data2);
typedef BOOL (TOLCALL *PFNTLISTENUM)(LDATA data, LPARAM param); /* return TRUE to break enum */
typedef void (TOLCALL *PFNTLISTKILLITEM)(LDATA data, LPARAM param);

TOLCDECL TLIST TOLAPI List_Make();
TOLCDECL void TOLAPI List_Kill(TLIST tList);
TOLCDECL void TOLAPI List_KillCallback(TLIST tList, PFNTLISTKILLITEM pFn, LPARAM param);

TOLCDECL int TOLAPI List_Num(TLIST tList);

TOLCDECL POS TOLAPI List_Index(TLIST tList, int i);
TOLCDECL LDATA TOLAPI List_At(TLIST tList, POS pos);
TOLCDECL LDATA TOLAPI List_SetAt(TLIST tList, POS pos, LDATA data);

TOLCDECL POS TOLAPI List_Head(TLIST tList);
TOLCDECL POS TOLAPI List_Tail(TLIST tList);
TOLCDECL POS TOLAPI List_Next(TLIST tList, POS pos);
TOLCDECL POS TOLAPI List_Prev(TLIST tList, POS pos);

TOLCDECL BOOL TOLAPI List_AddHead(TLIST tList, LDATA data);
TOLCDECL BOOL TOLAPI List_AddTail(TLIST tList, LDATA data);
TOLCDECL BOOL TOLAPI List_InsertBefore(TLIST tList, POS pos, LDATA data);
TOLCDECL BOOL TOLAPI List_InsertAfter(TLIST tList, POS pos, LDATA data);

TOLCDECL LDATA TOLAPI List_RemoveHead(TLIST tList);
TOLCDECL LDATA TOLAPI List_RemoveTail(TLIST tList);
TOLCDECL LDATA TOLAPI List_Remove(TLIST tList, POS pos);
TOLCDECL void TOLAPI List_RemoveAll(TLIST tList);

TOLCDECL void TOLAPI List_RemoveHeadCallback(TLIST tList, PFNTLISTKILLITEM pFn, LPARAM param);
TOLCDECL void TOLAPI List_RemoveTailCallback(TLIST tList, PFNTLISTKILLITEM pFn, LPARAM param);
TOLCDECL void TOLAPI List_RemoveCallback(TLIST tList, POS pos, PFNTLISTKILLITEM pFn, LPARAM param);
TOLCDECL void TOLAPI List_RemoveAllCallback(TLIST tList, PFNTLISTKILLITEM pFn, LPARAM param);

TOLCDECL POS TOLAPI List_Find(TLIST tList, LDATA data);
TOLCDECL POS TOLAPI List_Enum(TLIST tList, PFNTLISTENUM pFn, LPARAM param);
TOLCDECL POS TOLAPI List_EnumRev(TLIST tList, PFNTLISTENUM pFn, LPARAM param);
TOLCDECL void TOLAPI List_Swap(TLIST tList, POS pos1, POS pos2);
TOLCDECL void TOLAPI List_Sort(TLIST tList, PFNTLISTSORT pFn);


TOLEXTC_END

#endif /* _INC_TOLC_LIST_H */
