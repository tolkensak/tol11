
#include "stdafx.h"
#include <tolc_memory.h>


TOLEXTC_BEGIN


#define _WITH_TARRAYKILLITEM


typedef PVoid TARRAYITEM, *PTARRAYITEM;
typedef const TARRAYITEM *PCTARRAYITEM;

#ifdef _WITH_TARRAYKILLITEM
typedef void (TOLCALL *PFNTARRAYKILLITEM)(TARRAYITEM item);
#endif


typedef struct tagArray
{
	PTARRAYITEM pData;
	int nCount;
	int nCountMax;

#ifdef _WITH_TARRAYKILLITEM
	PFNTARRAYKILLITEM pfnKillItem;
#endif
} T_ARRAY, *TARRAY;

#define _TOLC_TARRAY_DEFINED


TOLEXTC_END


#include <tolc_array.h>


TOLEXTC_BEGIN


TOLCDECL void TOLAPI Array_SetCount(TARRAY tArr, int CountNew)
{
	if(!tArr || CountNew<0 || CountNew==tArr->nCount)
		return;

	if(CountNew==0)
	{
		if(tArr->pData)
		{
#ifdef _WITH_TARRAYKILLITEM
			if(tArr->pfnKillItem)
			{
				int i;
				for(i=0; i<tArr->nCount; i++)
					tArr->pfnKillItem(tArr->pData[i]);
			}
#endif

			Mem_Free(tArr->pData);
			tArr->pData=NULL;
		}

		tArr->nCount=tArr->nCountMax=0;
	}
	else if(!tArr->pData)
	{
		tArr->pData=(PTARRAYITEM)Mem_Alloc(MEM_ZERO_MEMORY, CountNew*sizeof(TARRAYITEM));
		tArr->nCount=tArr->nCountMax=CountNew;
	}
	else if(CountNew<=tArr->nCountMax)
	{
		if(CountNew>tArr->nCount) // it fits
		{ // initialize the new elements
			Mem_Set(tArr->pData+tArr->nCount, 0, (CountNew-tArr->nCount)*sizeof(TARRAYITEM));
		}
		else if(tArr->nCount>CountNew)
		{ // destroy the old elements
#ifdef _WITH_TARRAYKILLITEM
			if(tArr->pfnKillItem)
			{
				int i;
				for(i=CountNew; i<tArr->nCount; i++)
					tArr->pfnKillItem(tArr->pData[i]);
			}
#endif
		}

		tArr->nCount=CountNew;
	}
	else
	{ // otherwise, grow array
		// heuristically determine growth when nGrowBy==0 (this avoids heap fragmentation in many situations)
		int CountMax;
		int nGrowBy=tArr->nCount/8;

		nGrowBy=(nGrowBy<4)?4:((nGrowBy>1024)?1024:nGrowBy);

		if(CountNew<tArr->nCountMax+nGrowBy)
			CountMax=tArr->nCountMax+nGrowBy; // granularity
		else
			CountMax=CountNew; // no slush

		tArr->pData=(PTARRAYITEM)Mem_Realloc(MEM_ZERO_MEMORY, tArr->pData, CountMax*sizeof(TARRAYITEM));

		tArr->nCount=CountNew;
		tArr->nCountMax=CountMax;
	}
}

TOLCDECL TARRAY TOLAPI Array_Make(int nCount)
{
	TARRAY tArr=(TARRAY)Mem_Alloc(MEM_ZERO_MEMORY, sizeof(T_ARRAY));
	Array_SetCount(tArr, nCount);
	return tArr;
}

TOLCDECL void TOLAPI Array_Kill(TARRAY tArr)
{
	Array_SetCount(tArr, 0);
	Mem_Free(tArr);
}

TOLCDECL PCTARRAYITEM TOLAPI Array_Data(TARRAY tArr)
{
	return tArr?tArr->pData:NULL;
}

TOLCDECL void TOLAPI Array_Shrink(TARRAY tArr)
{
	if(tArr && tArr->nCount<tArr->nCountMax)
	{
		if(tArr->nCount)
			tArr->pData=(PTARRAYITEM)Mem_Realloc(0, tArr->pData, tArr->nCount*sizeof(TARRAYITEM));

		tArr->nCountMax=tArr->nCount;
	}
}

TOLCDECL int TOLAPI Array_Count(TARRAY tArr)
{
	return tArr?tArr->nCount:0;
}

TOLCDECL TARRAYITEM TOLAPI Array_At(TARRAY tArr, int nIndex)
{
	return (tArr && nIndex>=0 && nIndex<tArr->nCount)?tArr->pData[nIndex]:(TARRAYITEM)0;
}

TOLCDECL void TOLAPI Array_SetAt(TARRAY tArr, int nIndex, TARRAYITEM item)
{
	if(tArr && nIndex>=0)
	{
		if(nIndex>=tArr->nCount)
			Array_SetCount(tArr, nIndex+1);

		tArr->pData[nIndex]=item;
	}
}

TOLCDECL void TOLAPI Array_Add(TARRAY tArr, TARRAYITEM item)
{
	if(tArr)
	{
		int nCountOld=tArr->nCount;
		Array_SetCount(tArr, nCountOld+1);
		tArr->pData[nCountOld]=item;
	}
}

TOLCDECL void TOLAPI Array_AddN(TARRAY tArr, TARRAYITEM item, int nCount)
{
	if(tArr && nCount>0)
	{
		PTARRAYITEM p;
		int nCountOld=tArr->nCount;
		Array_SetCount(tArr, nCountOld+nCount);

		p=tArr->pData+nCountOld;
		while(nCount--)
			*p++=item;
	}
}

TOLCDECL void TOLAPI Array_AddA(TARRAY tArr, TARRAY tArrSrc)
{
	if(tArr && tArrSrc && tArr!=tArrSrc && tArrSrc->nCount>0)
	{
		int nCountOld=tArr->nCount;
		Array_SetCount(tArr, nCountOld+tArrSrc->nCount);
		Mem_Copy(tArr->pData+nCountOld, tArrSrc->pData, (tArrSrc->nCount)*sizeof(TARRAYITEM));
	}
}

TOLCDECL void TOLAPI Array_Insert(TARRAY tArr, int nIndex, TARRAYITEM item)
{
	if(tArr)
	{
		if(nIndex<0) // add if nIndex is -1
			nIndex=tArr->nCount;

		if(nIndex>=tArr->nCount)
		{ // adding after the end of the array
			Array_SetCount(tArr, nIndex+1); // grow so nIndex is valid
		}
		else
		{ // inserting in the middle of the array
			int nCountOld=tArr->nCount;
			Array_SetCount(tArr, tArr->nCount+1); // grow it to new count
			Mem_Move(tArr->pData+nIndex+1, tArr->pData+nIndex, (nCountOld-nIndex)*sizeof(TARRAYITEM)); // shift old data up to fill gap
		}

		tArr->pData[nIndex]=item;
	}
}

TOLCDECL void TOLAPI Array_InsertN(TARRAY tArr, int nIndex, TARRAYITEM item, int nCount)
{
	if(tArr && nCount>0)
	{
		if(nIndex<0) // add if nIndex is -1
			nIndex=tArr->nCount;

		if(nIndex>=tArr->nCount)
		{ // adding after the end of the array
			Array_SetCount(tArr, nIndex+nCount); // grow so nIndex is valid
		}
		else
		{ // inserting in the middle of the array
			int nCountOld=tArr->nCount;
			Array_SetCount(tArr, tArr->nCount+nCount); // grow it to new count
			Mem_Move(tArr->pData+nIndex+nCount, tArr->pData+nIndex, (nCountOld-nIndex)*sizeof(TARRAYITEM)); // shift old data up to fill gap
		}

		while(nCount--)
			tArr->pData[nIndex++]=item;
	}
}

TOLCDECL void TOLAPI Array_InsertA(TARRAY tArr, int nIndex, TARRAY tArrSrc)
{
	if(tArr && tArrSrc && tArr!=tArrSrc && tArrSrc->nCount>0)
	{
		int i;
		if(nIndex<0) // add if nIndex is -1
			nIndex=tArr->nCount;

		Array_InsertN(tArr, nIndex, tArrSrc->pData[0], tArrSrc->nCount);
		for(i=0; i<tArrSrc->nCount; i++)
			tArr->pData[nIndex+i]=tArrSrc->pData[i];
	}
}

TOLCDECL void TOLAPI Array_Remove(TARRAY tArr, int nIndex)
{
	if(tArr && nIndex>=0 && nIndex<tArr->nCount)
	{
		int nUpperBound=nIndex+1;
		if(nUpperBound<=tArr->nCount && nUpperBound>=nIndex && nUpperBound>=1)
		{
			// just remove a range
			int nCountMove=tArr->nCount-nUpperBound;

#ifdef _WITH_TARRAYKILLITEM
			if(tArr->pfnKillItem)
				tArr->pfnKillItem(tArr->pData[nIndex]);
#endif

			if(nCountMove)
				Mem_Move(tArr->pData+nIndex, tArr->pData+nUpperBound, nCountMove*sizeof(TARRAYITEM));

			tArr->nCount--;
		}
	}
}

TOLCDECL void TOLAPI Array_RemoveN(TARRAY tArr, int nIndex, int nCount)
{
	if(tArr && nIndex>=0 && nIndex<tArr->nCount && nCount>0)
	{
		int nUpperBound=nIndex+nCount;
		if(nUpperBound<=tArr->nCount && nUpperBound>=nIndex && nUpperBound>=nCount)
		{
			// just remove a range
			int nCountMove=tArr->nCount-nUpperBound;

#ifdef _WITH_TARRAYKILLITEM
			if(tArr->pfnKillItem)
			{
				int i;
				for(i=0; i<nCount; i++)
					tArr->pfnKillItem(tArr->pData[nIndex+i]);
			}
#endif

			if(nCountMove)
				Mem_Move(tArr->pData+nIndex, tArr->pData+nUpperBound, nCountMove*sizeof(TARRAYITEM));

			tArr->nCount-=nCount;
		}
	}
}

TOLCDECL void TOLAPI Array_RemoveAll(TARRAY tArr)
{
	Array_SetCount(tArr, 0);
}

TOLCDECL void TOLAPI Array_Copy(TARRAY tArr, TARRAY tArrSrc)
{
	if(tArr && tArrSrc && tArr!=tArrSrc)
	{
		Array_SetCount(tArr, tArrSrc->nCount);

		if(tArrSrc->nCount>0)
			Mem_Copy(tArr->pData, tArrSrc->pData, (tArrSrc->nCount)*sizeof(TARRAYITEM));
	}
}

TOLCDECL void TOLAPI Array_Sort(TARRAY tArr, PFNTARRAYSORT pfnSort, Long lParam)
{
	if(tArr && tArr->nCount && pfnSort)
	{
		TARRAYITEM item;
		PTARRAYITEM p1, p2;
		PTARRAYITEM pLast=tArr->pData+tArr->nCount-1;

		for(p1=tArr->pData; p1<pLast; p1++)
			for(p2=p1+1; p2<=pLast; p2++)
				if(pfnSort(*p1, *p2, lParam)>0)
				{
					item=*p1;
					*p1=*p2;
					*p2=item;
				}
	}
}

TOLCDECL int TOLAPI Array_Enum(TARRAY tArr, int nIndex, PFNTARRAYENUM pfnEnum, Long lParam)
{
	if(tArr && nIndex<tArr->nCount && pfnEnum)
	{
		if(nIndex<0)
			nIndex=0;

		for(; nIndex<tArr->nCount; nIndex++)
			if(pfnEnum(nIndex, tArr->pData[nIndex], lParam))
				return nIndex;
	}

	return -1;
}

TOLCDECL int TOLAPI Array_EnumRev(TARRAY tArr, int nIndex, PFNTARRAYENUM pfnEnum, Long lParam)
{
	if(tArr && nIndex<tArr->nCount && pfnEnum)
	{
		if(nIndex<0)
			nIndex=tArr->nCount-1;

		for(; nIndex>=0; nIndex--)
			if(pfnEnum(nIndex, tArr->pData[nIndex], lParam))
				return nIndex;
	}

	return -1;
}

#ifdef _WITH_TARRAYKILLITEM
TOLCDECL void TOLAPI Array_SetKillItem(TARRAY tArr, PFNTARRAYKILLITEM pfnKillItem)
{
	if(tArr)
		tArr->pfnKillItem=pfnKillItem;
}
#endif

//TOLCDECL void TOLAPI Array_AddAuto(TARRAY tArr, int num, FNARRCREATEITEM func, LPARAM lParam)
//{
//	if(DBG_IFTRUE(tArr && num>0))
//	{
//		int i=tArr->nCount;
//		Array_SetCount(tArr, tArr->nCount+num, !func);
//
//		if(func)
//		{
//			PTARRAYITEM p=tArr->pData+i;
//			while(num--)
//				*p++=func(i++, lParam);
//		}
//	}
//}

//TOLCDECL void TOLAPI Array_InsertAuto(TARRAY tArr, int i, int num, FNARRCREATEITEM func, LPARAM lParam)
//{
//	if(DBG_IFTRUE(tArr && i>=0 && i<tArr->nCount && num>0))
//	{
//		PTARRAYITEM p;
//		Array_SetCount(tArr, tArr->nCount+num, FALSE);
//
//		if(i<tArr->nCount)
//			MoveMemory(tArr->pData+i+num, tArr->pData+i, tArr->nCount-i);
//
//		p=tArr->pData+i;
//		while(num--)
//			*p++=func(i++, lParam);
//	}
//}

//TOLCDECL void TOLAPI Array_RemoveAuto(TARRAY tArr, int i, int num, FNARRDESTROYITEM func, LPARAM lParam)
//{
//	if(DBG_IFTRUE(tArr && i>=0 && func) && num>0)
//	{
//		int j=i;
//		int n=(i+num<tArr->nCount)?num:tArr->nCount-i;
//		PTARRAYITEM p=tArr->pData+i;
//
//		while(n--)
//			func(j++, *p++, lParam);
//
//		if(i+num<tArr->nCount)
//			MoveMemory(tArr->pData+i, tArr->pData+i+num, tArr->nCount-i-num);
//
//		tArr->nCount-=num;
//	}
//}
//
//TOLCDECL void TOLAPI Array_RemoveAllAuto(TARRAY tArr, FNARRDESTROYITEM func, LPARAM lParam)
//{
//	Array_RemoveAuto(tArr, 0, tArr->nCount, func, lParam);
//}


TOLEXTC_END
