
#ifndef _INC_TOLC_REGISTER_H
#define _INC_TOLC_REGISTER_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


typedef struct tagRegKey
{
	LPTSTR pcName;
	DWORD dwNameLen;
	LPTSTR pcClass;
	DWORD dwClassLen;
	FILETIME ftLastWriteTime;
} T_REGKEY, *PT_REGKEY;
typedef const T_REGKEY *PCT_REGKEY;

typedef struct tagRegVal
{
	LPTSTR pcName;
	DWORD dwNameLen;
	DWORD dwType;
	LPBYTE pbyData;
	DWORD dwDataLen;
} T_REGVAL, *PT_REGVAL;
typedef const T_REGVAL *PCT_REGVAL;

typedef struct tagRegVals
{
	T_REGVAL val;
	LONG lResult;
} T_REGVALS, *PT_REGVALS;


typedef BOOL (TOLCALL *PFNREGENUMKEY)(PCT_REGKEY pRegKey, LPARAM lParam); /* return TRUE to stop enum */
typedef BOOL (TOLCALL *PFNREGENUMVALUE)(PCT_REGVAL pRegVal, LPARAM lParam); /* return TRUE to stop enum */


TOLCDECL LONG TOLAPI Reg_KeyExist(HKEY hRoot, LPCTSTR pcKey);
TOLCDECL LONG TOLAPI Reg_DeleteKey(HKEY hRoot, LPCTSTR pcKey, LPCTSTR pcSubKey);
TOLCDECL LONG TOLAPI Reg_DeleteSubKey(HKEY hRoot, LPCTSTR pcKey);
TOLCDECL LONG TOLAPI Reg_EnumKey(HKEY hRoot, LPCTSTR pcKey, PFNREGENUMKEY pFn, LPARAM lParam);


TOLCDECL LONG TOLAPI Reg_GetValue(HKEY hRoot, LPCTSTR pcKey, LPCTSTR pcValue, LPDWORD pdwType, LPBYTE pbyData, LPDWORD pdwDataLen);
TOLCDECL LONG TOLAPI Reg_GetValues(HKEY hRoot, LPCTSTR pcKey, PT_REGVALS pVals, int nValsCount);
TOLCDECL LONG TOLAPI Reg_SetValue(HKEY hRoot, LPCTSTR pcKey, LPDWORD pdwDisposition, LPCTSTR pcValue, DWORD dwType, LPBYTE pbyData, DWORD dwDataLen);
TOLCDECL LONG TOLAPI Reg_SetValues(HKEY hRoot, LPCTSTR pcKey, LPDWORD pdwDisposition, PT_REGVALS pVals, int nValsCount);
TOLCDECL LONG TOLAPI Reg_DeleteValue(HKEY hRoot, LPCTSTR pcKey, LPCTSTR pcValue);
TOLCDECL LONG TOLAPI Reg_DeleteValues(HKEY hRoot, LPCTSTR pcKey);
TOLCDECL LONG TOLAPI Reg_EnumValue(HKEY hRoot, LPCTSTR pcKey, PFNREGENUMVALUE pFn, LPARAM lParam);


TOLEXTC_END

#endif /* _INC_TOLC_REGISTER_H */
