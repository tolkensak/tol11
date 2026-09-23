
#include "stdafx.h"
#include <tolcpp_ini.h>
#include <tolcpp_text.h>
#include <tolc_string.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


TOLNS_BEGIN


#define SPACE_CHARS _T(" \t")

////////////////////////////////////////////////////////////////////////
//
// IniSetting

IniSetting::IniSetting(BOOL bForRead, LPCTSTR pcSeparator, LPCTSTR pcCommentMark)
	: m_bForRead(bForRead)
	, m_strSeparator(_T("="))
	, m_strCommentMark(_T("//"))
{
	if(pcSeparator)
		m_strSeparator=pcSeparator;

	if(pcCommentMark)
		m_strCommentMark=pcCommentMark;

	if(!m_bForRead)
		return;

	TextReader f;
	if(f.Open(GetPath())!=ERROR_SUCCESS)
		return;

	LPTSTR pc, pc2;
	String strLine;
	Section* pSec;

	while(f.ReadLine(strLine))
	{
		pc=strLine.Buffer();

		pc=Str_Trim(pc, SPACE_CHARS);
		if(!*pc)
			continue;

		pc2=(LPTSTR)Str_Str(pc, m_strCommentMark);
		if(pc2)
		{
			*pc2=_T('\0');
			pc=Str_Trim(pc, SPACE_CHARS);
			if(!*pc)
				continue;
		}

		if(!Str_Str(pc, m_strSeparator))
		{
			pSec=new Section();
			m_mapSect[pc]=pSec;
			continue;
		}

		if(!pSec)
			continue;

		pc2=Str_Token(pc, m_strSeparator);
		if(!pc2)
			continue;

		pc2=Str_Trim(pc2, SPACE_CHARS);
		if(!*pc2)
			continue;

		pc=Str_Trim(pc, SPACE_CHARS);
		if(*pc)
			pSec->mapValue[pc]=pc2;
		else
			pSec->arrValue+=pc2;
	}
}

IniSetting::~IniSetting()
{
	if(m_bForRead)
		return;

	POS pos1;
	String strSec;
	Section* pSec;

	TextWriter f;
	if(f.Open(GetPath())==ERROR_SUCCESS)
	{
		int i;
		POS pos2;
		String str, strKey, strValue;

		for(pos1=m_mapSect.First(strSec, pSec); pos1; pos1=m_mapSect.Next(pos1, strSec, pSec))
		{
			if(!pSec)
				continue;

			f.WriteLine(strSec);

			for(pos2=pSec->mapValue.First(strKey, strValue); pos2; pos2=pSec->mapValue.Next(pos2, strKey, strValue))
			{
				str.Format(_T("%s%s%s"), (LPCTSTR)strKey, (LPCTSTR)m_strSeparator, (LPCTSTR)strValue);
				f.WriteLine(str);
			}

			for(i=0; i<pSec->arrValue.Count(); i++)
			{
				str.Format(_T("%s%s"), (LPCTSTR)m_strSeparator, (LPCTSTR)pSec->arrValue[i]);
				f.WriteLine(str);
			}
		}
	}

	for(pos1=m_mapSect.First(strSec, pSec); pos1; pos1=m_mapSect.Next(pos1, strSec, pSec))
		delete pSec;
}

String IniSetting::GetPath()
{
	TCHAR pch[TOL_MAXPATH];
	GetModuleFileName(NULL, pch, TOL_MAXPATH);
	PathRenameExtension(pch, _T(".ini"));
	return pch;
}

String IniSetting::Read(LPCTSTR pcSec, LPCTSTR pcKey)
{
	if(!pcSec|| !pcKey)
		return _T("");

	Section* pSec=m_mapSect[pcSec];
	if(!pSec)
		return _T("");

	return pSec->mapValue[pcKey];
}

void IniSetting::Enum(LPCTSTR pcSec, PFNINIENUM pFn, LPARAM lParam)
{
	if(!pcSec || !pFn)
		return;

	Section* pSec=m_mapSect[pcSec];
	if(!pSec)
		return;

	POS pos;
	String key, value;
	for(pos=pSec->mapValue.First(key, value); pos; pos=pSec->mapValue.Next(pos, key, value))
		pFn(key, value, lParam);

	for(int i=0; i<pSec->arrValue.Count(); i++)
		pFn(NULL, pSec->arrValue[i], lParam);
}

void IniSetting::Write(LPCTSTR pcSec, Section* pSec)
{
	if(!pcSec)
		return;

	Section* p=m_mapSect[pcSec];
	if(p)
	{
		m_mapSect.Remove(pcSec);
		delete p;
	}

	if(pSec)
		m_mapSect[pcSec]=pSec;
}


TOLNS_END
