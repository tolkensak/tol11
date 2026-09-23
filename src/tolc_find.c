
#include "stdafx.h"
#include <tolc_find.h>
#include <tolc_string.h>

#include <shlwapi.h> /* Path... */
#pragma comment(lib, "shlwapi.lib")


TOLEXTC_BEGIN


TOLCDECL Bool TOLAPI Find_Search(PCChar pcDir, PCChar pcFind, PFNFINDSEARCH pfn, Long lParam, Bool bIncludeSubDir)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	Char pch[TOL_MAXPATH];
	PChar pc0, pc1, pc2;

	if(!pcDir || !pcFind || !pfn)
		return FALSE;

	pc0=pc1=Str_Dup(pcFind);

	while(pc1)
	{
		pc2=Str_Token(pc1, _T(";"));

		PathCombine(pch, pcDir, pc1);
		hFind=FindFirstFile(pch, &fd);
		if(hFind!=INVALID_HANDLE_VALUE)
		{
			do{
				if(pfn(pcDir, &fd, lParam))
				{
					FindClose(hFind);
					Str_Kill(pc0);
					return TRUE;
				}
			}while(FindNextFile(hFind, &fd));
		}

		FindClose(hFind);
		pc1=pc2;
	}

	Str_Kill(pc0);

	if(!bIncludeSubDir)
		return FALSE;

	PathCombine(pch, pcDir, _T("*.*"));
	hFind=FindFirstFile(pch, &fd);
	if(hFind!=INVALID_HANDLE_VALUE)
	{
		do{
			if(PathIsDirectory(fd.cFileName) && Str_Cmp(fd.cFileName, _T(".")) && Str_Cmp(fd.cFileName, _T("..")))
			{
				PathCombine(pch, pcDir, fd.cFileName);
				if(Find_Search(pch, pcFind, pfn, lParam, TRUE))
				{
					FindClose(hFind);
					return TRUE;
				}
			}
		}while(FindNextFile(hFind, &fd));
	}

	FindClose(hFind);
	return FALSE;
}

/*
TOLCDECL DWORD TOLAPI FindThread(LPVOID param)
{
	PFINDTHREAD pft=(PFINDTHREAD)param;

	LPTSTR pcRootDir=StrDup(pft->pcRootDir);
	LPTSTR pcWhat=StrDup(pft->pcWhat);

	if(pft->hEventCloneParam)
		SetEvent(pft->hEventCloneParam);

	Find(pcRootDir, pcWhat, pft->pFn, pft->bIncludeSubDir);

	LocalFree(pcRootDir);
	LocalFree(pcWhat);

	return 0;
}

TOLCDECL DWORD TOLAPI FindDrive(LPVOID param)
{
	int i, n;
	FINDTHREAD ft;
	PFINDINDRIVE pFd=(PFINDINDRIVE)param;

	HANDLE hThrd;
	HARR arrThrd=ArrCreate(0, sizeof(LPVOID));

	DWORD dw=GetLogicalDriveStrings(0, NULL);
	LPTSTR pc, pcDrive=Mem_Alloc(0, (dw+1)*sizeof(TCHAR));
	GetLogicalDriveStrings(dw, pcDrive);

	ft.pcFor=pFd->pcFor;
	ft.bSubDir=pFd->bSubDir;
	ft.pFn=pFd->pFn;

	g_hEventFindDriveCopyThreadParam=CreateEvent(NULL, TRUE, TRUE, _T("EventToCopyThreadParam"));

	if(pFd->nDriveTypes)
	{
		UINT u;

		for(pc=pcDrive; *pc; pc+=4)
		{
			u=GetDriveType(pc);

			for(i=0; i<pFd->nDriveTypes; i++)
			{
				if(u==*(pFd->puDriveTypes+i))
				{
					ft.pcRootDir=pc;
					hThrd=CreateThread(NULL, 0, FindThreadProc, (LPVOID)&ft, 0, NULL);

					if(hThrd)
					{
						SetThreadPriority(hThrd, pFd->nThreadPriority);
						ArrAdd(arrThrd, 1, hThrd);

						ResetEvent(g_hEventFindDriveCopyThreadParam);
						WaitForSingleObject(g_hEventFindDriveCopyThreadParam, INFINITE);
					}

					break;
				}
			}
		}
	}
	else
	{
		for(pc=pcDrive; *pc; pc+=4)
		{
			ft.pcRootDir=pc;
			hThrd=CreateThread(NULL, 0, FindThreadProc, (LPVOID)&ft, 0, NULL);

			if(hThrd)
			{
				SetThreadPriority(hThrd, pFd->nThreadPriority);
				ArrAdd(arrThrd, 1, hThrd);

				ResetEvent(g_hEventFindDriveCopyThreadParam);
				WaitForSingleObject(g_hEventFindDriveCopyThreadParam, INFINITE);
			}
		}
	}

	CloseHandle(g_hEventFindDriveCopyThreadParam);
	g_hEventFindDriveCopyThreadParam=NULL;

	n=ArrGetNum(arrThrd);
	if(n)
	{
		WaitForMultipleObjects(n, ArrGetBuff(arrThrd), TRUE, INFINITE);

		for(i=0; i<n; i++)
			CloseHandle(ArrGetAt(arrThrd, i));
	}

	Mem_Free(pcDrive);
	ArrDestroy(arrThrd);

	return 0;
}
*/


TOLEXTC_END
