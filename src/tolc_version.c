
#include "stdafx.h"
#include <tolc_version.h>
#include <tolc_memory.h>
#include <tolc_string.h>

#include <winver.h> /* version */
#pragma comment(lib, "version.lib")


TOLEXTC_BEGIN


TOLCDECL int TOLAPI Version_GetSectionData(PCChar pcPath, PCChar pcSection, PChar pcData)
{
	PByte pbyData;
	Dword dwInfoSize;
	Dword dwHandle=0; /* Set to Zero (Unused) */

	dwInfoSize=GetFileVersionInfoSize(pcPath, &dwHandle); /* Size of the Info Structure. */
	if(!dwInfoSize)
		return VERSION_NO_VERSION_INFO;

	pbyData=Mem_Alloc(0, dwInfoSize*sizeof(Byte));
	if(!pbyData)
		return VERSION_INSUFFICENT_MEMORY;

	if(GetFileVersionInfo(pcPath, dwHandle, dwInfoSize, pbyData))
	{
		Uint uLanguageSize; /* Size of language buffer */
		PDword pdwLanguages;

		/* Get the translation information. (Language) */
		if(VerQueryValue(pbyData, _T("\\VarFileInfo\\Translation"), (LPVOID*)&pdwLanguages, &uLanguageSize) && uLanguageSize)
		{
			Uint uSize; /* Size of Data recieved with version information */
			PCChar pc=NULL;
			Char szSubBlock[1024]; /* Version language and section definition */
			int nRet=VERSION_SUCCESS;

			wsprintf(szSubBlock, _T("\\StringFileInfo\\%04hX%04hX\\%s"), LOWORD(*pdwLanguages), HIWORD(*pdwLanguages), pcSection);

			if(VerQueryValue(pbyData, szSubBlock, (LPVOID*)&pc, &uSize))
				Str_Copy(pcData, pc);
			else
				nRet=VERSION_NO_SECTION;

			Mem_Free(pbyData);
			return nRet;
		}
	}

	Mem_Free(pbyData);
	return VERSION_UNABLE_EXTRACT_VERSION_INFO;
}

TOLCDECL int TOLAPI Version_GetFixedFileInfo(PCChar pcPath, VS_FIXEDFILEINFO* const pInfo)
{
	PByte pbyData;
	Dword dwInfoSize;
	Dword dwHandle=0; /* Set to Zero (Unused) */

	dwInfoSize=GetFileVersionInfoSize(pcPath, &dwHandle); /* Size of the Info Structure. */
	if(!dwInfoSize)
		return VERSION_NO_VERSION_INFO;

	pbyData=Mem_Alloc(0, dwInfoSize*sizeof(Byte));
	if(!pbyData)
		return VERSION_INSUFFICENT_MEMORY;
	else
	{
		Uint uSize; /* Size of Data recieved with version information */
		VS_FIXEDFILEINFO* pData=0; /* Pointer to where data will be located */

		if(GetFileVersionInfo(pcPath, dwHandle, dwInfoSize, pbyData) && VerQueryValue(pbyData, _T("\\"), (void**)&pData, &uSize))
		{
			Mem_Copy(pInfo, pData, sizeof(VS_FIXEDFILEINFO));
			Mem_Free(pbyData);
			return VERSION_SUCCESS;
		}
	}

	Mem_Free(pbyData);
	return VERSION_UNABLE_EXTRACT_VERSION_INFO;
}

TOLCDECL int TOLAPI Version_GetLangCodepage(PCChar pcPath, PDword pdwLangCodepage)
{
	PByte pbyData;
	Dword dwInfoSize;
	Dword dwHandle=0; /* Set to Zero (Unused) */

	dwInfoSize=GetFileVersionInfoSize(pcPath, &dwHandle); /* Size of the Info Structure. */
	if(!dwInfoSize)
		return VERSION_NO_VERSION_INFO;

	pbyData=Mem_Alloc(0, dwInfoSize*sizeof(Byte));
	if(!pbyData)
		return VERSION_INSUFFICENT_MEMORY;

	if(GetFileVersionInfo(pcPath, dwHandle, dwInfoSize, pbyData))
	{
		Uint uLanguageSize; /* Size of language buffer */
		PDword pdwLanguages;

		/* Get the translation information. (Language) */
		if(VerQueryValue(pbyData, _T("\\VarFileInfo\\Translation"), (void**)&pdwLanguages, &uLanguageSize) && uLanguageSize)
		{
			*pdwLangCodepage=*pdwLanguages;
			Mem_Free(pbyData);
			return VERSION_SUCCESS;
		}
	}

	Mem_Free(pbyData);
	return VERSION_UNABLE_EXTRACT_VERSION_INFO;
}


TOLEXTC_END
