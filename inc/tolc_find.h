
#ifndef _INC_TOLC_FIND_H
#define _INC_TOLC_FIND_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


typedef Bool (TOLCALL *PFNFINDSEARCH)(PCChar pcDir, LPWIN32_FIND_DATA pFindData, Long lParam); /* return TREU to stop find */

TOLCDECL Bool TOLAPI Find_Search(PCChar pcDir, PCChar pcFind /*a.exe;b.txt;c.doc*/, PFNFINDSEARCH pfn, Long lParam, Bool bIncludeSubDir);

/*
typedef struct tagFindThread
{
	LPCTSTR pcDir;
	LPCTSTR pcFind;
	BOOL bIncludeSubDir;
	PFNFOUND pfnFound;
	int pnPriority;
	HANDLE hEventCloneParam; 
} FINDTHREAD, *PFINDTHREAD;

typedef struct tagFindDrive
{
	FINDTHREAD ft;
	int nDriveTypes;
	LPUINT puDriveTypes;
} FINDDRIVE, *PFINDINDRIVE;

TOLCDECL DWORD TOLAPI FindThread(LPVOID param);
TOLCDECL DWORD TOLAPI FindDrive(LPVOID param);
*/


TOLEXTC_END

#endif /* _INC_TOLC_FIND_H */
