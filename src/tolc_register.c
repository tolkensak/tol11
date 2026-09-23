
#include "stdafx.h"
#include <tolc_register.h>
#include <tolc_memory.h>


TOLEXTC_BEGIN


TOLCDECL LONG TOLAPI Reg_KeyExist(HKEY hRoot, LPCTSTR pcKey)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	if(pcKey)
		lRet=RegOpenKeyEx(hRoot, pcKey, 0, KEY_QUERY_VALUE, &hKey);
	else
		hKey=hRoot;

	if(lRet==ERROR_SUCCESS && pcKey)
		RegCloseKey(hKey);

	return lRet;
}

TOLCDECL LONG TOLAPI Reg_GetValue(HKEY hRoot, LPCTSTR pcKey, LPCTSTR pcValue, LPDWORD pdwType, LPBYTE pbyData, LPDWORD pdwDataLen)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	if(pcKey)
		lRet=RegOpenKeyEx(hRoot, pcKey, 0, KEY_QUERY_VALUE, &hKey);
	else
		hKey=hRoot;

	if(lRet==ERROR_SUCCESS)
	{
		lRet=RegQueryValueEx(hKey, pcValue, NULL, pdwType, pbyData, pdwDataLen);

		if(pcKey)
			RegCloseKey(hKey);
	}

	return lRet;
}

TOLCDECL LONG TOLAPI Reg_GetValues(HKEY hRoot, LPCTSTR pcKey, PT_REGVALS pVals, int nValsCount)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	if(pcKey)
		lRet=RegOpenKeyEx(hRoot, pcKey, 0, KEY_QUERY_VALUE, &hKey);
	else
		hKey=hRoot;

	if(lRet==ERROR_SUCCESS)
	{
		int i;
		for(i=0; i<nValsCount; i++)
			pVals[i].lResult=RegQueryValueEx(hKey, pVals[i].val.pcName, NULL, &pVals[i].val.dwType, pVals[i].val.pbyData, &pVals[i].val.dwDataLen);

		if(pcKey)
			RegCloseKey(hKey);
	}

	return lRet;
}

TOLCDECL LONG TOLAPI Reg_SetValue(HKEY hRoot, LPCTSTR pcKey, LPDWORD pdwDisposition, LPCTSTR pcValue, DWORD dwType, LPBYTE pbyData, DWORD dwDataLen)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	if(pcKey)
		lRet=RegCreateKeyEx(hRoot, pcKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, pdwDisposition);
	else
		hKey=hRoot;

	if(lRet==ERROR_SUCCESS)
	{
		lRet=RegSetValueEx(hKey, pcValue, 0, dwType, pbyData, dwDataLen);

		if(pcKey)
			RegCloseKey(hKey);
	}

	return lRet;
}

TOLCDECL LONG TOLAPI Reg_SetValues(HKEY hRoot, LPCTSTR pcKey, LPDWORD pdwDisposition, PT_REGVALS pVals, int nValsCount)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	if(pcKey)
		lRet=RegCreateKeyEx(hRoot, pcKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, pdwDisposition);
	else
		hKey=hRoot;

	if(lRet==ERROR_SUCCESS)
	{
		int i;
		for(i=0; i<nValsCount; i++)
			pVals[i].lResult=RegSetValueEx(hKey, pVals[i].val.pcName, 0, pVals[i].val.dwType, pVals[i].val.pbyData, pVals[i].val.dwDataLen);


		if(pcKey)
			RegCloseKey(hKey);
	}

	return lRet;
}

TOLCDECL LONG TOLAPI Reg_DeleteValue(HKEY hRoot, LPCTSTR pcKey, LPCTSTR pcValue)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	if(pcKey)
		lRet=RegOpenKeyEx(hRoot, pcKey, 0, KEY_SET_VALUE, &hKey);
	else
		hKey=hRoot;

	if(lRet==ERROR_SUCCESS)
	{
		lRet=RegDeleteValue(hKey, pcValue);

		if(pcKey)
			RegCloseKey(hKey);
	}

	return lRet;
}

TOLCDECL LONG TOLAPI Reg_DeleteValues(HKEY hRoot, LPCTSTR pcKey)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	DWORD dwNum;
	DWORD dwMaxNameLen;

	if(pcKey)
		lRet=RegOpenKeyEx(hRoot, pcKey, 0, KEY_QUERY_VALUE|KEY_SET_VALUE, &hKey);
	else
		hKey=hRoot;

	if(lRet!=ERROR_SUCCESS)
		return lRet;

	lRet=RegQueryInfoKey(hKey,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&dwNum,
		&dwMaxNameLen,
		NULL,
		NULL,
		NULL);

	if(lRet==ERROR_SUCCESS && dwNum)
	{
		LPTSTR pcName;
		DWORD dw, dwNameLen;

		dwMaxNameLen++;
		pcName=Mem_Alloc(0, dwMaxNameLen*sizeof(TCHAR));

		for(dw=dwNum-1; lRet==ERROR_SUCCESS; dw--)
		{
			dwNameLen=dwMaxNameLen;
			lRet=RegEnumValue(hKey, dw, pcName, &dwNameLen, NULL, NULL, NULL, NULL);

			if(lRet==ERROR_NO_MORE_ITEMS)
			{
				lRet=ERROR_SUCCESS;
				break;
			}

			if(lRet==ERROR_SUCCESS)
				lRet=RegDeleteValue(hKey, pcName);
		}

		Mem_Free(pcName);
	}

	if(pcKey)
		RegCloseKey(hKey);

	return lRet;
}

TOLCDECL LONG TOLAPI Reg_DeleteSubKey(HKEY hRoot, LPCTSTR pcKey)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	DWORD dwNum;
	DWORD dwMaxNameLen;

	if(!pcKey)
		return ERROR_INVALID_PARAMETER;

	lRet=RegOpenKeyEx(hRoot, pcKey, 0, KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS|KEY_SET_VALUE, &hKey);

	if(lRet!=ERROR_SUCCESS)
		return lRet;

	lRet=RegQueryInfoKey(hKey,
		NULL,
		NULL,
		NULL,
		&dwNum,
		&dwMaxNameLen,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if(lRet==ERROR_SUCCESS && dwNum)
	{
		LPTSTR pc, pcName;
		DWORD dw, dwNameLen;

		dwMaxNameLen++;
		pcName=Mem_Alloc(0, dwMaxNameLen*sizeof(TCHAR));
		pc=Mem_Alloc(0, (dwMaxNameLen+lstrlen(pcKey))*sizeof(TCHAR));

		for(dw=dwNum-1; lRet==ERROR_SUCCESS; dw--)
		{
			dwNameLen=dwMaxNameLen;
			lRet=RegEnumKeyEx(hKey, dw, pcName, &dwNameLen, NULL, NULL, NULL, NULL);

			if(lRet==ERROR_NO_MORE_ITEMS)
			{
				lRet=ERROR_SUCCESS;
				break;
			}

			if(lRet==ERROR_SUCCESS)
			{
				wsprintf(pc, _T("%s\\%s"), pcKey, pcName);
				lRet=Reg_DeleteSubKey(hRoot, pc);

				if(lRet==ERROR_SUCCESS)
					lRet=RegDeleteKey(hKey, pcName);
			}
		}

		Mem_Free(pcName);
		Mem_Free(pc);
	}

	RegCloseKey(hKey);
	return lRet;
}

TOLCDECL LONG TOLAPI Reg_DeleteKey(HKEY hRoot, LPCTSTR pcKey, LPCTSTR pcSubKey)
{
	LONG lRet=ERROR_SUCCESS;

	if(!pcSubKey)
		return ERROR_INVALID_PARAMETER;

	if(pcKey)
	{
		TCHAR pc[MAX_PATH];
		wsprintf(pc, _T("%s\\%s"), pcKey, pcSubKey);
		lRet=Reg_DeleteSubKey(hRoot, pc);
	}
	else
		lRet=Reg_DeleteSubKey(hRoot, pcSubKey);

	if(lRet==ERROR_SUCCESS)
	{
		HKEY hKey;

		if(pcKey)
			lRet=RegOpenKeyEx(hRoot, pcKey, 0, KEY_SET_VALUE, &hKey);
		else
			hKey=hRoot;

		if(lRet==ERROR_SUCCESS)
		{
			lRet=RegDeleteKey(hKey, pcSubKey);

			if(pcKey)
				RegCloseKey(hKey);
		}
	}

	return lRet;
}

TOLCDECL LONG TOLAPI Reg_EnumKey(HKEY hRoot, LPCTSTR pcKey, PFNREGENUMKEY pFn, LPARAM lParam)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	DWORD dwNum;
	DWORD dwMaxNameLen;
	DWORD dwMaxClassLen;

	if(pcKey)
		lRet=RegOpenKeyEx(hRoot, pcKey, 0, KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS, &hKey);
	else
		hKey=hRoot;

	if(lRet!=ERROR_SUCCESS)
		return lRet;

	lRet=RegQueryInfoKey(hKey,
		NULL,
		NULL,
		NULL,
		&dwNum,
		&dwMaxNameLen,
		&dwMaxClassLen,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if(lRet==ERROR_SUCCESS && dwNum)
	{
		DWORD dw;
		T_REGKEY rk;

		dwMaxNameLen++;
		rk.pcName=Mem_Alloc(0, dwMaxNameLen*sizeof(TCHAR));

		dwMaxClassLen++;
		rk.pcClass=Mem_Alloc(0, dwMaxClassLen*sizeof(TCHAR));

		for(dw=0; dw<dwNum; dw++)
		{
			rk.dwNameLen=dwMaxNameLen;
			rk.dwClassLen=dwMaxClassLen;

			lRet=RegEnumKeyEx(hKey, dw, rk.pcName, &rk.dwNameLen, NULL, rk.pcClass, &rk.dwClassLen, &rk.ftLastWriteTime);

			if(lRet==ERROR_NO_MORE_ITEMS)
			{
				lRet=ERROR_SUCCESS;
				break;
			}

			if(lRet!=ERROR_SUCCESS || pFn(&rk, lParam))
				break;
		}

		Mem_Free(rk.pcName);
		Mem_Free(rk.pcClass);
	}

	if(pcKey)
		RegCloseKey(hKey);

	return lRet;
}

TOLCDECL LONG TOLAPI Reg_EnumValue(HKEY hRoot, LPCTSTR pcKey, PFNREGENUMVALUE pFn, LPARAM lParam)
{
	HKEY hKey;
	LONG lRet=ERROR_SUCCESS;

	DWORD dwNum;
	DWORD dwMaxNameLen;
	DWORD dwMaxDataLen;

	if(pcKey)
		lRet=RegOpenKeyEx(hRoot, pcKey, 0, KEY_QUERY_VALUE, &hKey);
	else
		hKey=hRoot;

	if(lRet!=ERROR_SUCCESS)
		return lRet;

	lRet=RegQueryInfoKey(hKey,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&dwNum,
		&dwMaxNameLen,
		&dwMaxDataLen,
		NULL,
		NULL);

	if(lRet==ERROR_SUCCESS && dwNum)
	{
		DWORD dw;
		T_REGVAL rv;

		dwMaxNameLen++;
		rv.pcName=Mem_Alloc(0, dwMaxNameLen*sizeof(TCHAR));
		rv.pbyData=Mem_Alloc(0, dwMaxDataLen*sizeof(BYTE));

		for(dw=0; dw<dwNum; dw++)
		{
			rv.dwNameLen=dwMaxNameLen;
			rv.dwDataLen=dwMaxDataLen;

			lRet=RegEnumValue(hKey, dw, rv.pcName, &rv.dwNameLen, NULL, &rv.dwType, rv.pbyData, &rv.dwDataLen);

			if(lRet!=ERROR_SUCCESS || pFn(&rv, lParam))
				break;
		}

		Mem_Free(rv.pcName);
		Mem_Free(rv.pbyData);
	}

	if(pcKey)
		RegCloseKey(hKey);

	return lRet;
}

TOLEXTC_END
