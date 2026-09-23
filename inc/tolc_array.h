
#ifndef _INC_TOLC_ARRAY_H
#define _INC_TOLC_ARRAY_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


#ifndef _TOLC_TARRAY_DEFINED
#define _TOLC_TARRAY_DEFINED

typedef PVoid TARRAY;
typedef PVoid TARRAYITEM, *PTARRAYITEM;
typedef const TARRAYITEM *PCTARRAYITEM;

#define _WITH_TARRAYKILLITEM
#ifdef _WITH_TARRAYKILLITEM
typedef void (TOLCALL *PFNTARRAYKILLITEM)(TARRAYITEM item);
#endif /*_WITH_TARRAYKILLITEM*/

#endif /*_TOLC_TARRAY_DEFINED*/


typedef int (TOLCALL *PFNTARRAYSORT)(TARRAYITEM item1, TARRAYITEM item2, Long lParam);
typedef Bool (TOLCALL *PFNTARRAYENUM)(int nIndex, TARRAYITEM item, Long lParam); /* return TRUE to break enum */


TOLCDECL TARRAY TOLAPI Array_Make(int nCount);
TOLCDECL void TOLAPI Array_Kill(TARRAY tArr);

TOLCDECL PCTARRAYITEM TOLAPI Array_Data(TARRAY tArr);
TOLCDECL void TOLAPI Array_Shrink(TARRAY tArr);

TOLCDECL int TOLAPI Array_Count(TARRAY tArr);
TOLCDECL void TOLAPI Array_SetCount(TARRAY tArr, int nCountNew);

TOLCDECL TARRAYITEM TOLAPI Array_At(TARRAY tArr, int nIndex);
TOLCDECL void TOLAPI Array_SetAt(TARRAY tArr, int nIndex, TARRAYITEM item);

TOLCDECL void TOLAPI Array_Add(TARRAY tArr, TARRAYITEM item);
TOLCDECL void TOLAPI Array_AddN(TARRAY tArr, TARRAYITEM item, int nCount);
TOLCDECL void TOLAPI Array_AddA(TARRAY tArr, TARRAY tArrSrc);

TOLCDECL void TOLAPI Array_Insert(TARRAY tArr, int nIndex, TARRAYITEM item);
TOLCDECL void TOLAPI Array_InsertN(TARRAY tArr, int nIndex, TARRAYITEM item, int nCount);
TOLCDECL void TOLAPI Array_InsertA(TARRAY tArr, int nIndex, TARRAY tArrSrc);

TOLCDECL void TOLAPI Array_Remove(TARRAY tArr, int nIndex);
TOLCDECL void TOLAPI Array_RemoveN(TARRAY tArr, int nIndex, int nCount);
TOLCDECL void TOLAPI Array_RemoveAll(TARRAY tArr);

TOLCDECL void TOLAPI Array_Copy(TARRAY tArr, TARRAY tArrSrc);
TOLCDECL void TOLAPI Array_Sort(TARRAY tArr, PFNTARRAYSORT pfnSort, Long lParam);

TOLCDECL int TOLAPI Array_Enum(TARRAY tArr, int nIndex, PFNTARRAYENUM pfnEnum, Long lParam);
TOLCDECL int TOLAPI Array_EnumRev(TARRAY tArr, int nIndex, PFNTARRAYENUM pfnEnum, Long lParam);


#ifdef _WITH_TARRAYKILLITEM
TOLCDECL void TOLAPI Array_SetKillItem(TARRAY tArr, PFNTARRAYKILLITEM pfnKillItem);
#endif


TOLEXTC_END

#endif /* _INC_TOLC_ARRAY_H */
