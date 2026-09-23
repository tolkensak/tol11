
#include "stdafx.h"
#include <tolc_memory.h>

/*
#ifdef _WINDOWS_
#include <tolc_debug.h>
#endif
*/

#include <stdlib.h>
#include <malloc.h>


TOLEXTC_BEGIN


//#ifdef _WINDOWS_
//
//#ifdef _DEBUG2
//
//
//#ifdef TOLCDLL
//#pragma data_seg("Shared")
//#endif
//
//Ulong _ulAllocatedSize=0;
//
//#ifdef TOLCDLL
//#pragma data_seg()
//#pragma comment(linker, "/section:Shared,RWS")
//#endif
//
//
//TOLCDECL void TOLAPI Mem_Leak()
//{
//	Dbg_Out(_T("Memory leak: %d.\n"), _ulAllocatedSize);
//}
//
//
//TOLCDECL PVoid TOLAPI _Mem_AllocDbg(Ulong ulFlags, Ulong ulSize, PCChar pcExpr, PCChar pcFile, Ulong ulLine)
//{
//	PVoid p=HeapAlloc(GetProcessHeap(), ulFlags, ulSize);
//	if(p)
//	{
//		_ulAllocatedSize+=ulSize;
//
//		Dbg_Out(_T("ALLOC Addr: %X, Size: %d, Expr: %s, File: %s, Line %d.\n"), p, ulSize, pcExpr, pcFile, ulLine);
//	}
//
//	return p;
//}
//
//TOLCDECL PVoid TOLAPI _Mem_AllocHeapDbg(Handle heap, Ulong ulFlags, Ulong ulSize, PCChar pcExpr, PCChar pcFile, Ulong ulLine)
//{
//	PVoid p=HeapAlloc(heap, ulFlags, ulSize);
//	if(p)
//	{
//		_ulAllocatedSize+=ulSize;
//
//		Dbg_Out(_T("ALLOC Heap: %X, Addr: %X, Size: %d, Expr: %s, File: %s, Line %d.\n"), heap, p, ulSize, pcExpr, pcFile, ulLine);
//	}
//
//	return p;
//}
//
//TOLCDECL PVoid TOLAPI _Mem_ReallocDbg(Ulong ulFlags, PVoid pMem, Ulong ulSize, PCChar pcMem, PCChar pcExpr, PCChar pcFile, Ulong ulLine)
//{
//	if(pMem)
//	{
//		Ulong sz=Mem_Size(pMem);
//		PVoid p=HeapReAlloc(GetProcessHeap(), ulFlags, pMem, ulSize);
//		if(p)
//		{
//			if(sz!=(Ulong)-1)
//				_ulAllocatedSize-=sz;
//
//			_ulAllocatedSize+=ulSize;
//
//			Dbg_Out(_T("REALLOC New addr: %X, New size: %d, Old addr: %X, Old size: %d, Var: %s, Expr: %s, File: %s, Line %d.\n"), p, ulSize, pMem, sz, pcMem, pcExpr, pcFile, ulLine);
//		}
//
//		return p;
//	}
//
//	return NULL;
//}
//
//TOLCDECL PVoid TOLAPI _Mem_ReallocHeapDbg(Handle heap, Ulong ulFlags, PVoid pMem, Ulong ulSize, PCChar pcMem, PCChar pcExpr, PCChar pcFile, Ulong ulLine)
//{
//	if(pMem)
//	{
//		Ulong sz=Mem_SizeHeap(heap, pMem);
//		PVoid p=HeapReAlloc(heap, ulFlags, pMem, ulSize);
//		if(p)
//		{
//			if(sz!=(Ulong)-1)
//				_ulAllocatedSize-=sz;
//
//			_ulAllocatedSize+=ulSize;
//
//			Dbg_Out(_T("REALLOC Heap: %X, New addr: %X, New size: %d, Old addr: %X, Old size: %d, Var: %s, Expr: %s, File: %s, Line %d.\n"), heap, p, ulSize, pMem, sz, pcMem, pcExpr, pcFile, ulLine);
//		}
//
//		return p;
//	}
//
//	return NULL;
//}
//
//TOLCDECL void TOLAPI _Mem_FreeDbg(PVoid pMem, PCChar pcMem, PCChar pcFile, Ulong ulLine)
//{
//	if(pMem)
//	{
//		Ulong sz=Mem_Size(pMem);
//		if(sz==(Ulong)-1)
//			sz=0;
//
//		if(HeapFree(GetProcessHeap(), 0, pMem))
//		{
//			_ulAllocatedSize-=sz;
//
//			Dbg_Out(_T("FREE Addr: %X, Size: %d, Var: %s,  FILE %s, LINE %d.\n"), pMem, sz, pcMem, pcFile, ulLine);
//		}
//	}
//}
//
//TOLCDECL void TOLAPI _Mem_FreeHeapDbg(Handle heap, PVoid pMem, PCChar pcMem, PCChar pcFile, Ulong ulLine)
//{
//	if(pMem)
//	{
//		Ulong sz=Mem_SizeHeap(heap, pMem);
//		if(sz==(Ulong)-1)
//			sz=0;
//
//		if(HeapFree(heap, 0, pMem))
//		{
//			_ulAllocatedSize-=sz;
//
//			Dbg_Out(_T("FREE Heap: %X, Addr: %X, Size: %d, Var: %s,  FILE %s, LINE %d.\n"), heap, pMem, sz, pcMem, pcFile, ulLine);
//		}
//	}
//}
//
//#endif /* _DEBUG2 */
//
//#endif /* _WINDOWS_ */


TOLCDECL PVoid TOLAPI _Mem_AllocCRT(Ulong ulFlags, Ulong ulSize)
{
	PVoid p=malloc(ulSize);
	if(p && (ulFlags&MEM_ZERO_MEMORY))
		Mem_Set(p, 0, ulSize);

	return p;
}

TOLCDECL PVoid TOLAPI _Mem_ReallocCRT(Ulong ulFlags, Ulong ulSize, PVoid pMem)
{
	PVoid p;
	Ulong ulSizeOld=0;

	if((ulFlags&MEM_ZERO_MEMORY) && pMem)
	{
		ulSizeOld=(Ulong)_msize(pMem);
		if(ulSize<=ulSizeOld)
			ulSizeOld=0;
	}

	p=realloc(pMem, ulSize);
	if(p && ulSizeOld)
		Mem_Set((PVoid)(((PByte)p)+ulSizeOld), 0, ulSize-ulSizeOld);

	return p;
}

TOLCDECL void TOLAPI _Mem_FreeCRT(PVoid pMem)
{
	free(pMem);
}

TOLCDECL Ulong TOLAPI _Mem_SizeCRT(PVoid pMem)
{
	return (Ulong)_msize(pMem);
}

TOLCDECL PVoid TOLAPI _Mem_Alloc(Ulong ulFlags, Ulong ulSize)
{
	return HeapAlloc(GetProcessHeap(), ulFlags, ulSize);
}

TOLCDECL PVoid TOLAPI _Mem_Realloc(Ulong ulFlags, Ulong ulSize, PVoid pMem)
{
	return HeapReAlloc(GetProcessHeap(), ulFlags, pMem, ulSize);
}

TOLCDECL void TOLAPI _Mem_Free(PVoid pMem)
{
	HeapFree(GetProcessHeap(), 0, pMem);
}

TOLCDECL Ulong TOLAPI _Mem_Size(PVoid pMem)
{
	return (Ulong)HeapSize(GetProcessHeap(), 0, pMem);
}

TOLCDECL void TOLAPI Mem_Set(PVoid pDest, Byte byFill, Ulong ulSize)
{
	PByte p=pDest;
	while(ulSize--)
		*p++=byFill;
}

TOLCDECL void TOLAPI Mem_Copy(PVoid pDest, PCVoid pSrc, Ulong ulSize)
{
	PByte d=pDest;
	PCByte s=pSrc;
	while(ulSize--)
		*d++=*s++;
}

TOLCDECL void TOLAPI Mem_Move(PVoid pDest, PCVoid pSrc, Ulong ulSize)
{
	if(pDest>pSrc)
	{
		PByte d=(PByte)pDest+ulSize;
		PCByte s=(PCByte)pSrc+ulSize;
		while(ulSize--)
			*--d=*--s;
	}
	else if(pDest<pSrc)
	{
		PByte d=pDest;
		PCByte s=pSrc;
		while(ulSize--)
			*d++=*s++;
	}
}


TOLEXTC_END
