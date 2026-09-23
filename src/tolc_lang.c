
#include "stdafx.h"
#include <tolc_array.h>
#include <tolc_memory.h>
#include <tolc_string.h>
#include <tolc_find.h>
#include <tolc_version.h>

#include <shlwapi.h> /* Path... */
#pragma comment(lib, "shlwapi.lib")


TOLEXTC_BEGIN


typedef struct tagLang
{
	TARRAY tArr;
	Uint uNameFormat;
} T_LANG, *TLANG;

#define _TOLC_TLANG_DEFINED


typedef struct tagLangData
{
	LANGID lid;
	PChar pcName;
	PChar pcPath;
} T_LANGDATA, *PT_LANGDATA;


TOLEXTC_END


#define _TOLC_TMENU_DEFINED
#include <tolc_lang.h>


TOLEXTC_BEGIN


void TOLAPI Lang_Add(TLANG tLang, PT_LANGDATA pData)
{
	PT_LANGDATA pDataNew;

	if(!tLang || !pData)
		return;

	pDataNew=Mem_Alloc(0, sizeof(T_LANGDATA));
	if(pDataNew)
	{
		pDataNew->lid=pData->lid;
		pDataNew->pcName=Str_Dup(pData->pcName);
		pDataNew->pcPath=Str_Dup(pData->pcPath);
		Array_Add(tLang->tArr, pDataNew);
	}
}

void TOLAPI Lang_Erase(TLANG tLang)
{
	if(tLang)
	{
		PT_LANGDATA pData;
		int i, n=Array_Count(tLang->tArr);

		for(i=0; i<n; i++)
		{
			pData=(PT_LANGDATA)Array_At(tLang->tArr, i);
			Str_Kill(pData->pcName);
			Str_Kill(pData->pcPath);
			Mem_Free(pData);
		}

		Array_RemoveAll(tLang->tArr);
	}
}


TOLCDECL TLANG TOLAPI Lang_Make(Uint uNameFormat)
{
	TLANG tLang=(TLANG)Mem_Alloc(MEM_ZERO_MEMORY, sizeof(T_LANG));
	if(tLang)
	{
		tLang->tArr=Array_Make(0);
		if(!tLang->tArr)
		{
			Mem_Free(tLang);
			return NULL;
		}

		tLang->uNameFormat=uNameFormat;
	}

	return tLang;
}

TOLCDECL void TOLAPI Lang_Kill(TLANG tLang)
{
	if(tLang)
	{
		Lang_Erase(tLang);
		Array_Kill(tLang->tArr);
		Mem_Free(tLang);
	}
}

TOLCDECL int TOLAPI Lang_Count(TLANG tLang)
{
	if(tLang)
		return Array_Count(tLang->tArr);

	return 0;
}


typedef struct tagLangFoundParam
{
	TLANG tLang;
	PCChar pcProductName;
	PCChar pcProductVersion;
} T_LANGFOUNDPARAM, *PT_LANGFOUNDPARAM;


int TOLCALL Lang_Sort(TARRAYITEM item1, TARRAYITEM item2, Long lParam)
{
	return Str_Cmp(((PT_LANGDATA)item1)->pcName, ((PT_LANGDATA)item2)->pcName);
}

Bool TOLCALL Lang_Found(PCChar pcDir, LPWIN32_FIND_DATA pFindData, Long lParam)
{
	Dword dw;
	T_LANGDATA data;
	PT_LANGFOUNDPARAM pParam=(PT_LANGFOUNDPARAM)(LONG_PTR)lParam;

	Char pch[TOL_MAXSTR];
	Char pchBufferName[TOL_MAXSTR];
	Char pchBufferPath[TOL_MAXPATH];

	data.pcName=pchBufferName;
	data.pcPath=pchBufferPath;

	PathCombine(data.pcPath, pcDir, pFindData->cFileName);

	if(Version_GetSectionData(data.pcPath, _T("ProductName"), pch)==VERSION_SUCCESS
		&& Str_Cmp(pParam->pcProductName, pch)==0
		&& Version_GetSectionData(data.pcPath, _T("ProductVersion"), pch)==VERSION_SUCCESS
		&& Str_Cmp(pParam->pcProductVersion, pch)==0
		&& Version_GetLangCodepage(data.pcPath, &dw)==VERSION_SUCCESS)
	{
		data.lid=LOWORD(dw);
		GetLocaleInfo(data.lid, pParam->tLang->uNameFormat, data.pcName, 30);
		Lang_Add(pParam->tLang, &data);
	}

	return FALSE;
}

TOLCDECL void TOLAPI Lang_Init(TLANG tLang, PCChar pcExePath)
{
	Char pchDir[TOL_MAXPATH];
	Char pchProductName[TOL_MAXSTR];
	Char pchProductVersion[TOL_MAXSTR];

	Dword dw;
	T_LANGDATA data;
	T_LANGFOUNDPARAM param;
	Char pchBufferName[TOL_MAXSTR];
	Char pchBufferPath[TOL_MAXPATH];

	if(!tLang || !pcExePath)
		return;

	data.pcName=pchBufferName;
	data.pcPath=pchBufferPath;

	Lang_Erase(tLang);

	if(Version_GetSectionData(pcExePath, _T("ProductName"), pchProductName)!=VERSION_SUCCESS)
		return;

	if(Version_GetSectionData(pcExePath, _T("ProductVersion"), pchProductVersion)!=VERSION_SUCCESS)
		return;

	if(Version_GetLangCodepage(pcExePath, &dw)==VERSION_SUCCESS)
	{
		data.lid=LOWORD(dw);
		GetLocaleInfo(data.lid, tLang->uNameFormat, data.pcName, TOL_MAXSTR);
		data.pcPath[0]=_T('\0');
		Lang_Add(tLang, &data);
	}

	Str_Copy(pchDir, pcExePath);
	PathRemoveFileSpec(pchDir);
	PathAppend(pchDir, _T("Lang"));

	param.tLang=tLang;
	param.pcProductName=pchProductName;
	param.pcProductVersion=pchProductVersion;
	Find_Search(pchDir, _T("*.dll"), Lang_Found, (LONG)(LONG_PTR)&param, FALSE);

	Array_Sort(tLang->tArr, Lang_Sort, 0);
}

TOLCDECL LANGID TOLAPI Lang_Id(TLANG tLang, int nIndex)
{
	if(tLang && nIndex>=0 && nIndex<Array_Count(tLang->tArr))
	{
		PT_LANGDATA pData=(PT_LANGDATA)Array_At(tLang->tArr, nIndex);
		if(pData)
			return pData->lid;
	}

	return 0;
}

TOLCDECL int TOLAPI Lang_Name(TLANG tLang, int nIndex, PChar pcName, int nNameBufferLen)
{
	if(tLang && nIndex>=0 && nIndex<Array_Count(tLang->tArr))
	{
		PT_LANGDATA pData=(PT_LANGDATA)Array_At(tLang->tArr, nIndex);
		if(pData)
		{
			int n=Str_Len(pData->pcName);
			if(nNameBufferLen>=n+1)
			{
				Str_Copy(pcName, pData->pcName);
				return n;
			}
		}
	}

	return -1;
}

TOLCDECL int TOLAPI Lang_Path(TLANG tLang, int nIndex, PChar pcPath, int nPathBufferLen)
{
	if(tLang && nIndex>=0 && nIndex<Array_Count(tLang->tArr))
	{
		PT_LANGDATA pData=(PT_LANGDATA)Array_At(tLang->tArr, nIndex);
		if(pData)
		{
			int n=Str_Len(pData->pcPath);
			if(nPathBufferLen>=n+1)
			{
				Str_Copy(pcPath, pData->pcPath);
				return n;
			}
		}
	}

	return -1;
}

TOLCDECL int TOLAPI Lang_Item(TLANG tLang, PT_LANGITEM pItem)
{
	if(tLang && pItem && pItem->uMask && pItem->nIndex>=0 && pItem->nIndex<Array_Count(tLang->tArr))
	{
		int nRet=TLANG_ITEM_RESULT_SUCCESS;

		PT_LANGDATA pData=(PT_LANGDATA)Array_At(tLang->tArr, pItem->nIndex);
		if(!pData)
			return TLANG_ITEM_RESULT_NO_DATA;

		if(pItem->uMask&TLANG_MASK_LANGID)
			pItem->lid=pData->lid;

		if(pItem->uMask&TLANG_MASK_NAME)
		{
			if(pItem->nNameBufferLen<Str_Len(pData->pcName)+1)
				nRet|=TLANG_ITEM_RESULT_NAME_SMALL;
			else
				Str_Copy(pItem->pcName, pData->pcName);
		}

		if(pItem->uMask&TLANG_MASK_PATH)
		{
			if(pItem->nPathBufferLen<Str_Len(pData->pcPath)+1)
				nRet|=TLANG_ITEM_RESULT_PATH_SMALL;
			else
				Str_Copy(pItem->pcPath, pData->pcPath);
		}

		return nRet;
	}

	return TLANG_ITEM_RESULT_BAD_ARG;
}

TOLCDECL int TOLAPI Lang_Find(TLANG tLang, PT_LANGFIND pFind)
{
	if(tLang && pFind && pFind->uMask)
	{
		Bool bRes;
		PT_LANGDATA pData;
		int i, n=Array_Count(tLang->tArr);

		for(i=0; i<n; i++)
		{
			bRes=FALSE;
			pData=(PT_LANGDATA)Array_At(tLang->tArr, i);

			if(pData)
			{
				if(pFind->uMask&TLANG_MASK_LANGID)
					bRes=(pFind->lid==pData->lid);

				if(bRes && (pFind->uMask&TLANG_MASK_NAME))
					bRes=(Str_Cmp(pFind->pcName, pData->pcName)==0);

				if(bRes && (pFind->uMask&TLANG_MASK_PATH))
					bRes=(Str_Cmp(pFind->pcPath, pData->pcPath)==0);

				if(bRes)
					return i;
			}
		}
	}

	return -1;
}

TOLCDECL void TOLAPI Lang_Menu(TLANG tLang, HMENU hParentMenu, int nPos, UINT uFirstID, HINSTANCE hTextInst, UINT uTextID)
{
	if(tLang && hParentMenu)
	{
		int n=Lang_Count(tLang);
		if(n)
		{
			HMENU hMenu=CreatePopupMenu();
			if(hMenu)
			{
				int i;
				T_LANGITEM li;
				Char pchText[TOL_MAXSTR];

				li.uMask=TLANG_MASK_NAME;
				li.pcName=pchText;
				li.nNameBufferLen=TOL_MAXSTR;

				for(i=0; i<n; i++)
				{
					li.nIndex=i;
					Lang_Item(tLang, &li);
					AppendMenu(hMenu, MF_STRING, uFirstID+i, li.pcName);
				}

				if(!hTextInst || !LoadString(hTextInst, uTextID, pchText, TOL_MAXSTR))
					Str_Copy(pchText, _T("&Languages"));

				InsertMenu(hParentMenu, nPos, MF_BYPOSITION|MF_POPUP|MF_STRING, (UINT_PTR)hMenu, pchText);
				InsertMenu(hParentMenu, nPos+1, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
			}
		}
	}
}


/***************************************************************************************************
	Free funcions     ******************************************************************************/


typedef struct tagEnumLangParam
{
	int nCount;
	LANGID lid;
} T_ENUMLANGPARAM, *PT_ENUMLANGPARAM;


BOOL CALLBACK Lang_Resource(HANDLE hModule, LPCTSTR lpszType, LPCTSTR lpszName, WORD wIDLanguage, LONG_PTR lParam)
{
	PT_ENUMLANGPARAM pParam=(PT_ENUMLANGPARAM)lParam;
	pParam->nCount++;
	pParam->lid=(LANGID)wIDLanguage;

	return TRUE;        // continue enumeration
}

LANGID Lang_GetNTDLLNativeLangID()
{
	// Get the HModule for ntdll.
	HMODULE hModule=GetModuleHandle(_T("ntdll.dll"));
	if(hModule)
	{
		PCChar pcType=(PCChar)((LPVOID)((WORD)16));
		PCChar pcName=(LPCTSTR)1;

		T_ENUMLANGPARAM param;
		ZeroMemory(&param, sizeof(T_ENUMLANGPARAM));

		if(EnumResourceLanguages(hModule, pcType, pcName, (ENUMRESLANGPROC)Lang_Resource, (LONG_PTR)&param)
			&& param.nCount>0
			&& param.nCount<3)
			return param.lid;
	}

	return 0;
}

// Checks if NT4 system is Hongkong SAR version
Bool Lang_IsHongKongVersion()
{
	Bool bRes=FALSE;

	HMODULE hModule=LoadLibrary(_T("imm32.dll"));
	if(hModule)
	{
		typedef BOOL (WINAPI *IMMRELEASECONTEXT)(HWND,HIMC);
		IMMRELEASECONTEXT pImmReleaseContext=(IMMRELEASECONTEXT)GetProcAddress(hModule, "ImmReleaseContext");
		if(pImmReleaseContext)
			bRes=pImmReleaseContext(NULL, NULL);

		FreeLibrary(hModule);
	}

	return bRes;
}

TOLCDECL LANGID Lang_GetUserDefaultLangID()
{
	return GetUserDefaultUILanguage();

	//LANGID lid=0;
	//OSVERSIONINFO vi;

	//vi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	//if(GetVersionEx(&vi))
	//{
	//	switch(vi.dwPlatformId)
	//	{
	//		case VER_PLATFORM_WIN32_NT: // On Windows NT, Windows 2000 or higher
	//		{
	//			if(vi.dwMajorVersion>=5) // Windows 2000 or higher
	//				lid=GetUserDefaultUILanguage();
	//			else // for NT4 check the language of ntdll.dll
	//			{
	//				lid=Lang_GetNTDLLNativeLangID();
	//				if(lid==1033 && Lang_IsHongKongVersion()) // special processing for Honkong SAR version of NT4
	//					lid=3076;
	//			}
	//		}
	//		break;

	//		case VER_PLATFORM_WIN32_WINDOWS: // On Windows 95, Windows 98 or Windows ME
	//		{
	//			HKEY hKey;

	//			// Open the registry key for the UI language
	//			if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("Default\\Control Panel\\Desktop\\ResourceLocale"), 0, KEY_QUERY_VALUE, &hKey)==ERROR_SUCCESS)
	//			{
	//				DWORD dwType, dwBufferLen;

	//				// Get the type and buffer length of the default key
	//				if(RegQueryValueEx(hKey, NULL, NULL, &dwType, NULL, &dwBufferLen)==ERROR_SUCCESS && dwType==REG_SZ)
	//				{
	//					PChar pc=Str_Make(dwBufferLen);

	//					// Read the default key value
	//					if(RegQueryValueEx(hKey, NULL, NULL, &dwType, (LPBYTE)pc, &dwBufferLen)==ERROR_SUCCESS)
	//						lid=_ttoi(pc);

	//					Str_Kill(pc);
	//				}

	//				RegCloseKey(hKey);
	//			}
	//		}
	//		break;
	//	}
	//}

	//return lid?lid:GetUserDefaultLangID();
}

TOLCDECL int Lang_GetName(LANGID lid, PChar pcName, int nNameBufferLen, UINT uFormat)
{
	return GetLocaleInfo(lid, uFormat, pcName, nNameBufferLen);
}


TOLEXTC_END
