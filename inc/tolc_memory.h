
#ifndef _INC_TOLC_MEMORY_H
#define _INC_TOLC_MEMORY_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


#define MEM_GENERATE_EXCEPTIONS                      0x00000004 // HEAP_GENERATE_EXCEPTIONS
#define MEM_ZERO_MEMORY                              0x00000008 // HEAP_ZERO_MEMORY
#define MEM_REALLOC_IN_PLACE_ONLY                    0x00000010 // HEAP_REALLOC_IN_PLACE_ONLY


TOLCDECL PVoid TOLAPI _Mem_AllocCRT(Ulong ulFlags, Ulong ulSize);
TOLCDECL PVoid TOLAPI _Mem_ReallocCRT(Ulong ulFlags, Ulong ulSize, PVoid pMem);
TOLCDECL void TOLAPI _Mem_FreeCRT(PVoid pMem);
TOLCDECL Ulong TOLAPI _Mem_SizeCRT(PVoid pMem);

TOLCDECL PVoid TOLAPI _Mem_Alloc(Ulong ulFlags, Ulong ulSize);
TOLCDECL PVoid TOLAPI _Mem_Realloc(Ulong ulFlags, Ulong ulSize, PVoid pMem);
TOLCDECL void TOLAPI _Mem_Free(PVoid pMem);
TOLCDECL Ulong TOLAPI _Mem_Size(PVoid pMem);


#ifdef _TOLMEM_CRT

#define Mem_Alloc(ulFlags, ulSize)                   _Mem_AllocCRT((Ulong)(ulFlags), (size_t)(ulSize))
#define Mem_Realloc(ulFlags, pMem, ulSize)           _Mem_ReallocCRT((Ulong)(ulFlags), (size_t)(ulSize), (PVoid)(pMem))
#define Mem_Free(pMem)                               _Mem_FreeCRT((PVoid)(pMem))
#define Mem_Size(pMem)                               _Mem_SizeCRT((PVoid)(pMem))

#else /* _TOLMEM_CRT */

#define Mem_Alloc(ulFlags, ulSize)                   _Mem_Alloc((Ulong)(ulFlags), (size_t)(ulSize))
#define Mem_Realloc(ulFlags, pMem, ulSize)           _Mem_Realloc((Ulong)(ulFlags), (size_t)(ulSize), (PVoid)(pMem))
#define Mem_Free(pMem)                               _Mem_Free((PVoid)(pMem))
#define Mem_Size(pMem)                               _Mem_Size((PVoid)(pMem))

#endif /* _TOLMEM_CRT */



//#ifdef _DEBUG2
//
//TOLCDECL PVoid TOLAPI _Mem_AllocDbg(Ulong ulFlags, Ulong ulSize, PCChar pcExpr, PCChar pcFile, Ulong ulLine);
//TOLCDECL PVoid TOLAPI _Mem_AllocHeapDbg(Handle heap, Ulong ulFlags, Ulong ulSize, PCChar pcExpr, PCChar pcFile, Ulong ulLine);
//TOLCDECL PVoid TOLAPI _Mem_ReallocDbg(Ulong ulFlags, PVoid pMem, Ulong ulSize, PCChar pcMem, PCChar pcExpr, PCChar pcFile, Ulong ulLine);
//TOLCDECL PVoid TOLAPI _Mem_ReallocHeapDbg(Handle heap, Ulong ulFlags, PVoid pMem, Ulong ulSize, PCChar pcMem, PCChar pcExpr, PCChar pcFile, Ulong ulLine);
//TOLCDECL void TOLAPI _Mem_FreeDbg(PVoid pMem, PCChar pcMem, PCChar pcFile, Ulong ulLine);
//TOLCDECL void TOLAPI _Mem_FreeHeapDbg(Handle heap, PVoid pMem, PCChar pcMem, PCChar pcFile, Ulong ulLine);
//
//#define Mem_Alloc(ulFlags, ulSize)                   _Mem_AllocDbg((Ulong)(ulFlags), (Ulong)(ulSize), _T(#ulSize), _T(__FILE__), __LINE__)
//#define Mem_AllocHeap(heap, ulFlags, ulSize)         _Mem_AllocHeapDbg((Handle)(heap), (Ulong)(ulFlags), (Ulong)(ulSize), _T(#ulSize), _T(__FILE__), __LINE__)
//#define Mem_Realloc(ulFlags, pMem, ulSize)           _Mem_ReallocDbg((Ulong)(ulFlags), (PVoid)(pMem), (Ulong)(ulSize), _T(#pMem), _T(#ulSize), _T(__FILE__), __LINE__)
//#define Mem_ReallocHeap(heap, ulFlags, pMem, ulSize) _Mem_ReallocHeapDbg((Handle)(heap), (Ulong)(ulFlags), (PVoid)(pMem), (Ulong)(ulSize), _T(#pMem), _T(#ulSize), _T(__FILE__), __LINE__)
//#define Mem_Free(pMem)                               _Mem_FreeDbg((PVoid)(pMem), _T(#pMem), _T(__FILE__), __LINE__)
//#define Mem_FreeHeap(heap, pMem)                     _Mem_FreeHeapDbg((Handle)(heap), (PVoid)(pMem), _T(#pMem), _T(__FILE__), __LINE__)
//
//TOLCDECL void TOLAPI Mem_Leak();
//
//#else /* _DEBUG2 */
//
//#define Mem_Alloc(ulFlags, ulSize)                   HeapAlloc(GetProcessHeap(), (Ulong)(ulFlags), (Ulong)(ulSize))
//#define Mem_AllocHeap(heap, ulFlags, ulSize)         HeapAlloc((Handle)(heap), (Ulong)(ulFlags), (Ulong)(ulSize))
//#define Mem_Realloc(ulFlags, pMem, ulSize)           HeapReAlloc(GetProcessHeap(), (Ulong)(ulFlags), (PVoid)(pMem), (Ulong)(ulSize))
//#define Mem_ReallocHeap(heap, ulFlags, pMem, ulSize) HeapReAlloc((Handle)(heap), (Ulong)(ulFlags), (PVoid)(pMem), (Ulong)(ulSize))
//#define Mem_Free(pMem)                               HeapFree(GetProcessHeap(), 0, (PVoid)(pMem))
//#define Mem_FreeHeap(heap, pMem)                     HeapFree((Handle)(heap), 0, (PVoid)(pMem))
//
//#define Mem_Leak()
//
//#endif /* _DEBUG2 */
//
//#define Mem_Size(pMem)                               ((Ulong)HeapSize(GetProcessHeap(), 0, (PVoid)(pMem)))
//#define Mem_SizeHeap(heap, pMem)                     ((Ulong)HeapSize((Handle)(heap), 0, (PVoid)(pMem)))



TOLCDECL void TOLAPI Mem_Set(PVoid pDest, Byte byFill, Ulong ulSize);
TOLCDECL void TOLAPI Mem_Copy(PVoid pDest, PCVoid pSrc, Ulong ulSize);
TOLCDECL void TOLAPI Mem_Move(PVoid pDest, PCVoid pSrc, Ulong ulSize);


TOLEXTC_END

#endif /* _INC_TOLC_MEMORY_H */
