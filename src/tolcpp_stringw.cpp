
#include "stdafx.h"
#include <tolcpp_stringw.h>
#include <tolc_memory.h>
#include <tolc_stringw.h>

#include <stdio.h>
#include <stdarg.h>


TOLNS_BEGIN


StringW::StringW(int nBufferLen)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(nBufferLen<0?0:nBufferLen);
}

StringW::StringW(const StringW& str, int nCount)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(str, nCount);
}

StringW::StringW(PCCharW pc, int nCount)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(pc, nCount);
}

StringW::StringW(PCCharA pc, int nCount, Uint uCodePage)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(pc, nCount, uCodePage);
}

StringW::StringW(CharW ch, int nCount)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(ch, nCount);
}

StringW::StringW(CharA ch, int nCount, Uint uCodePage)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(ch, nCount, uCodePage);
}

StringW::~StringW()
{
	Mem_Free(m_pc);
}

PCharW StringW::Buffer(int nBufferLen)
{
	if(nBufferLen>=0)
		SetBufferLen(nBufferLen);

	return m_pc;
}

void StringW::ReleaseBuffer(int nLen)
{
	m_nLen=nLen<0?StrW_Len(m_pc):nLen;
	m_pc[m_nLen]=L'\0';
}

void StringW::Shrink()
{
	Ulong size=(m_nLen+1)*sizeof(CharW);
	if(size<Mem_Size(m_pc))
		m_pc=(PCharW)Mem_Realloc(0, m_pc, size);
}

void StringW::Empty()
{
	m_pc[0]=L'\0';
	m_nLen=0;
}

void StringW::Copy(const StringW& str, int nCount)
{
	if(nCount<0)
	{
		SetBufferLen(str.m_nLen);
		StrW_Copy(m_pc, str.m_pc);
		m_nLen=str.m_nLen;
	}
	else if(nCount>0)
	{
		SetBufferLen(nCount);
		StrW_CopyN(m_pc, str.m_pc, nCount);
		m_nLen=nCount;
	}
	else
		Empty();
}

void StringW::Copy(PCCharW pc, int nCount)
{
	if(!pc)
		Empty();
	else if(nCount<0)
	{
		int n=StrW_Len(pc);
		SetBufferLen(n);
		StrW_Copy(m_pc, pc);
		m_nLen=n;
	}
	else if(nCount>0)
	{
		SetBufferLen(nCount);
		StrW_CopyN(m_pc, pc, nCount);
		m_nLen=nCount;
	}
	else
		Empty();
}

void StringW::Copy(PCCharA pc, int nCount, Uint uCodePage)
{
	FromA(pc, nCount, uCodePage);
}

void StringW::Copy(CharW ch, int nCount)
{
	if(nCount<1)
		nCount=1;

	SetBufferLen(nCount);

	for(int i=0; i<nCount; i++)
		m_pc[i]=ch;

	m_pc[nCount]=L'\0';
	m_nLen=nCount;
}

void StringW::Copy(CharA ch, int nCount, Uint uCodePage)
{
	FromA(&ch, 1, uCodePage);
}

void StringW::Cat(const StringW& str, int nCount)
{
	if(nCount<0)
	{
		SetBufferLen(str.m_nLen+m_nLen);
		StrW_Copy(m_pc+m_nLen, str.m_pc);
		m_nLen+=str.m_nLen;
	}
	else if(nCount>0)
	{
		SetBufferLen(nCount+m_nLen);
		StrW_CopyN(m_pc+m_nLen, str.m_pc, nCount);
		m_nLen+=nCount;
	}
	else
		Empty();
}

void StringW::Cat(PCCharW pc, int nCount)
{
	if(!pc)
		return;

	if(nCount<0)
	{
		int n=StrW_Len(pc);
		SetBufferLen(n+m_nLen);
		StrW_Copy(m_pc+m_nLen, pc);
		m_nLen+=n;
	}
	else if(nCount>0)
	{
		SetBufferLen(nCount+m_nLen);
		StrW_CopyN(m_pc+m_nLen, pc, nCount);
		m_nLen+=nCount;
	}
}

void StringW::Cat(PCCharA pc, int nCount, Uint uCodePage)
{
	Cat(StringW(pc, nCount, uCodePage));
}

void StringW::Cat(CharW ch, int nCount)
{
	if(nCount<1)
		nCount=1;

	nCount+=m_nLen;
	SetBufferLen(nCount);

	for(int i=m_nLen; i<nCount; i++)
		m_pc[i]=ch;

	m_pc[nCount]=L'\0';
	m_nLen=nCount;
}

void StringW::Cat(CharA ch, int nCount, Uint uCodePage)
{
	Cat(StringW(ch, nCount, uCodePage));
}

void StringW::Format(PCCharW pcFmt, ...)
{
	if(!pcFmt)
		return;

	va_list ap;
	va_start(ap, pcFmt);

	int n=_vscwprintf(pcFmt, ap);
	SetBufferLen(n);

	vswprintf(m_pc, n+1, pcFmt, ap);
	m_nLen=n;
	va_end(ap);
}

void StringW::FormatCat(PCCharW pcFmt, ...)
{
	if(!pcFmt)
		return;

	va_list ap;
	va_start(ap, pcFmt);

	int n=_vscwprintf(pcFmt, ap);
	SetBufferLen(m_nLen+n);

	vswprintf(m_pc+m_nLen, n+1, pcFmt, ap);
	m_nLen+=n;
	va_end(ap);
}


void StringW::TrimLeft(PCCharW pcTrim)
{
	if(!pcTrim)
		return;

	PCCharW pcTemp;
	PCharW pc=m_pc;

	while(*pc)
	{
		pcTemp=pcTrim;

		while(*pcTemp)
		{
			if(*pcTemp==*pc)
				break;

			pcTemp++;
		}

		if(!*pcTemp)
			break;

		pc++;
	}

	int n= (int)(pc-m_pc);
	if(n)
	{
		Mem_Move(m_pc, pc, (m_nLen-n+1)*sizeof(CharW));
		m_nLen-=n;
	}
}

void StringW::TrimRight(PCCharW pcTrim)
{
	if(!pcTrim)
		return;

	PCCharW pcTemp;
	PCharW pc=m_pc+m_nLen-1;

	while(pc>=m_pc)
	{
		pcTemp=pcTrim;

		while(*pcTemp)
		{
			if(*pc==*pcTemp)
				break;

			pcTemp++;
		}

		if(!*pcTemp)
			break;

		pc--;
	}

	*++pc=L'\0';
	m_nLen= (int)(pc-m_pc);
}

int StringW::Find(PCCharW pcFind, int nCount, int nStart)
{
	if(!pcFind || !*pcFind)
		return -1;

	if(nStart<0 || nStart>=m_nLen)
		return -1;

	int n;
	PCCharW pc1, pc2;
	PCCharW pc=m_pc+nStart;

	while(*pc)
	{
		n=0;
		pc1=pc;
		pc2=pcFind;

		while(*pc2 && n<nCount)
		{
			if(*pc1!=*pc2)
				break;

			pc1++;
			pc2++;
			n++;
		}

		if(!*pc2 || n==nCount)
			return (int)(pc-m_pc);

		pc++;
	}

	return -1;
}

int StringW::Find(CharW chFind, int nStart)
{
	if(!chFind)
		return -1;

	if(nStart<0 || nStart>=m_nLen)
		return -1;

	PCCharW pc=m_pc+nStart;

	while(*pc)
	{
		if(*pc==chFind)
			return (int)(pc-m_pc);

		pc++;
	}

	return -1;
}

int StringW::Comapre(const StringW& str, int nCount) const
{
	if(nCount<0)
		return StrW_Cmp(m_pc, str.m_pc);
	else
		return StrW_CmpN(m_pc, str.m_pc, nCount);
}

int StringW::Comapre(PCCharW pc, int nCount) const
{
	if(nCount<0)
		return StrW_Cmp(m_pc, pc);
	else
		return StrW_CmpN(m_pc, pc, nCount);
}

StringW StringW::Token(PCCharW pcToken, int& nStart)
{
	if(!pcToken || !*pcToken || nStart<0 || nStart>=m_nLen)
	{
		nStart=-1;
		return StringW();
	}

	PCharW pc0=m_pc+nStart;
	PCharW pc1=pc0;

	PCCharW pcTemp;
	while(*pc1)
	{
		pcTemp=pcToken;

		while(*pcTemp)
		{
			if(*pc1==*pcTemp)
			{
				nStart= (int)(pc1-m_pc);
				return StringW(pc0, (int)(pc1-pc0));
			}

			pcTemp++;
		}

		pc1++;
	}

	nStart= (int)(pc1-m_pc);
	return StringW(pc0, (int)(pc1-pc0));
}

Bool StringW::EnvVar(PCCharW pcVarName)
{
    Ulong ulCharCount = GetEnvironmentVariableW(pcVarName, NULL, 0);
    if (ulCharCount == 0) {
        return FALSE;
    }

	SetBufferLen(ulCharCount);

    ulCharCount = GetEnvironmentVariableW(pcVarName, m_pc, ulCharCount + 1);
	if (ulCharCount == 0) {
        return FALSE;
    }

    return TRUE;
}

void StringW::ChangeEndian()
{
	Byte by;
	PCharW pc=m_pc;
	PByte pby=(PByte)pc;

	while(*pc)
	{
		by=*pby;
		*pby=*(pby+1);
		*(pby+1)=by;

		pby+=2;
		pc++;
	}
}

void StringW::SetBufferLen(int nLen)
{
	if(nLen<0)
		return;

	Ulong sizeNew=(nLen+1)*sizeof(CharW);

	if(m_pc)
	{
		Ulong sizeCur=Mem_Size(m_pc);

		if(sizeNew>sizeCur)
		{
			Ulong sizeGrow=sizeCur/8;

			if(sizeGrow<4)
				sizeGrow=4;
			else if(sizeGrow>1024)
				sizeGrow=1024;

			sizeGrow+=sizeCur;

			if(sizeGrow<sizeNew)
				sizeGrow=sizeNew;

			m_pc=(PCharW)Mem_Realloc(0, m_pc, sizeGrow);
		}
	}
	else
	{
		m_pc=(PCharW)Mem_Alloc(0, sizeNew);
		m_pc[0]=L'\0';
	}
}

Bool StringW::FromA(PCCharA pcSrc, int nCount, Uint uCodePage)
{
	if(!pcSrc)
		return FALSE;

	if(nCount<=0)
		nCount=-1;

	int n=MultiByteToWideChar(uCodePage, 0, pcSrc, nCount, NULL, 0);
	if(n)
	{
		SetBufferLen(n);
		MultiByteToWideChar(uCodePage, 0, pcSrc, nCount, m_pc, n);
		m_pc[n]=L'\0';
		m_nLen=n;
		return TRUE;
	}

	Empty();
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////
//// friend functions

TOLCPPDECL Bool TOLAPI operator==(const StringW& str1, const StringW& str2)
{
	return StrW_Cmp(str1.m_pc, str2.m_pc)==0;
}

TOLCPPDECL Bool TOLAPI operator==(const StringW& str, PCCharW pc)
{
	return StrW_Cmp(str.m_pc, pc)==0;
}

TOLCPPDECL Bool TOLAPI operator==(PCCharW pc, const StringW& str)
{
	return StrW_Cmp(pc, str.m_pc)==0;
}

TOLCPPDECL Bool TOLAPI operator==(const StringW& str, PCCharA pc)
{
	return StrW_Cmp(str.m_pc, StringW(pc))==0;
}

TOLCPPDECL Bool TOLAPI operator==(PCCharA pc, const StringW& str)
{
	return StrW_Cmp(StringW(pc), str.m_pc)==0;
}

TOLCPPDECL Bool TOLAPI operator!=(const StringW& str1, const StringW& str2)
{
	return StrW_Cmp(str1.m_pc, str2.m_pc)!=0;
}

TOLCPPDECL Bool TOLAPI operator!=(const StringW& str, PCCharW pc)
{
	return StrW_Cmp(str.m_pc, pc)!=0;
}

TOLCPPDECL Bool TOLAPI operator!=(PCCharW pc, const StringW& str)
{
	return StrW_Cmp(pc, str.m_pc)!=0;
}

TOLCPPDECL Bool TOLAPI operator!=(const StringW& str, PCCharA pc)
{
	return StrW_Cmp(str.m_pc, StringW(pc))!=0;
}

TOLCPPDECL Bool TOLAPI operator!=(PCCharA pc, const StringW& str)
{
	return StrW_Cmp(StringW(pc), str.m_pc)!=0;
}

//
//TOLCPPDECL StringW TOLAPI operator+(const StringW& str1, const StringW& str2)
//{
//	StringW ret(str1);
//	ret+=str2;
//	return ret;
//}
//
//TOLCPPDECL StringW TOLAPI operator+(const StringW& str, PCCharW pc)
//{
//	StringW ret(pc);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringW TOLAPI operator+(PCCharW pc, const StringW& str)
//{
//	StringW ret(pc);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringW TOLAPI operator+(const StringW& str, PCCharA pc)
//{
//	StringW ret(pc);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringW TOLAPI operator+(PCCharA pc, const StringW& str)
//{
//	StringW ret(pc);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringW TOLAPI operator+(const StringW& str, CharW ch)
//{
//	StringW ret(ch);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringW TOLAPI operator+(CharW ch, const StringW& str)
//{
//	StringW ret(ch);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringW TOLAPI operator+(const StringW& str, CharA ch)
//{
//	StringW ret(ch);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringW TOLAPI operator+(CharA ch, const StringW& str)
//{
//	StringW ret(ch);
//	ret+=str;
//	return ret;
//}


TOLNS_END
