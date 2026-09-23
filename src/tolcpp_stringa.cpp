
#include "stdafx.h"
#include <tolcpp_stringa.h>
#include <tolc_memory.h>
#include <tolc_stringa.h>

#include <stdio.h>
#include <stdarg.h>


TOLNS_BEGIN


StringA::StringA(int nBufferLen)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(nBufferLen<0?0:nBufferLen);
}

StringA::StringA(const StringA& str, int nCount)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(str, nCount);
}

StringA::StringA(PCCharA pc, int nCount)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(pc, nCount);
}

StringA::StringA(PCCharW pc, int nCount, Uint uCodePage)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(pc, nCount, uCodePage);
}

StringA::StringA(CharA ch, int nCount)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(ch, nCount);
}

StringA::StringA(CharW ch, int nCount, Uint uCodePage)
	: m_pc(NULL)
	, m_nLen(0)
{
	SetBufferLen(0);
	Copy(ch, nCount, uCodePage);
}

StringA::~StringA()
{
	Mem_Free(m_pc);
}

PCharA StringA::Buffer(int nBufferLen)
{
	if(nBufferLen>=0)
		SetBufferLen(nBufferLen);

	return m_pc;
}

void StringA::ReleaseBuffer(int nLen)
{
	m_nLen=nLen<0?StrA_Len(m_pc):nLen;
	m_pc[m_nLen]='\0';
}

void StringA::Shrink()
{
	Ulong size=(m_nLen+1)*sizeof(CharA);
	if(size<Mem_Size(m_pc))
		m_pc=(PCharA)Mem_Realloc(0, m_pc, size);
}

void StringA::Empty()
{
	m_pc[0]='\0';
	m_nLen=0;
}

void StringA::Copy(const StringA& str, int nCount)
{
	if(nCount<0)
	{
		SetBufferLen(str.m_nLen);
		StrA_Copy(m_pc, str.m_pc);
		m_nLen=str.m_nLen;
	}
	else if(nCount>0)
	{
		SetBufferLen(nCount);
		StrA_CopyN(m_pc, str.m_pc, nCount);
		m_nLen=nCount;
	}
	else
		Empty();
}

void StringA::Copy(PCCharA pc, int nCount)
{
	if(!pc)
		Empty();
	else if(nCount<0)
	{
		int n=StrA_Len(pc);
		SetBufferLen(n);
		StrA_Copy(m_pc, pc);
		m_nLen=n;
	}
	else if(nCount>0)
	{
		SetBufferLen(nCount);
		StrA_CopyN(m_pc, pc, nCount);
		m_nLen=nCount;
	}
	else
		Empty();
}

void StringA::Copy(PCCharW pc, int nCount, Uint uCodePage)
{
	FromW(pc, nCount, uCodePage);
}

void StringA::Copy(CharA ch, int nCount)
{
	if(nCount<1)
		nCount=1;

	SetBufferLen(nCount);

	for(int i=0; i<nCount; i++)
		m_pc[i]=ch;

	m_pc[nCount]='\0';
	m_nLen=nCount;
}

void StringA::Copy(CharW ch, int nCount, Uint uCodePage)
{
	FromW(&ch, 1, uCodePage);
}

void StringA::Cat(const StringA& str, int nCount)
{
	if(nCount<0)
	{
		SetBufferLen(str.m_nLen+m_nLen);
		StrA_Copy(m_pc+m_nLen, str.m_pc);
		m_nLen+=str.m_nLen;
	}
	else if(nCount>0)
	{
		SetBufferLen(nCount+m_nLen);
		StrA_CopyN(m_pc+m_nLen, str.m_pc, nCount);
		m_nLen+=nCount;
	}
	else
		Empty();
}

void StringA::Cat(PCCharA pc, int nCount)
{
	if(!pc)
		return;

	if(nCount<0)
	{
		int n=StrA_Len(pc);
		SetBufferLen(n+m_nLen);
		StrA_Copy(m_pc+m_nLen, pc);
		m_nLen+=n;
	}
	else if(nCount>0)
	{
		SetBufferLen(nCount+m_nLen);
		StrA_CopyN(m_pc+m_nLen, pc, nCount);
		m_nLen+=nCount;
	}
}

void StringA::Cat(PCCharW pc, int nCount, Uint uCodePage)
{
	Cat(StringA(pc, nCount, uCodePage));
}

void StringA::Cat(CharA ch, int nCount)
{
	if(nCount<1)
		nCount=1;

	nCount+=m_nLen;
	SetBufferLen(nCount);

	for(int i=m_nLen; i<nCount; i++)
		m_pc[i]=ch;

	m_pc[nCount]='\0';
	m_nLen=nCount;
}

void StringA::Cat(CharW ch, int nCount, Uint uCodePage)
{
	Cat(StringA(ch, nCount, uCodePage));
}

void StringA::Format(PCCharA pcFmt, ...)
{
	if(!pcFmt)
		return;

	va_list ap;
	va_start(ap, pcFmt);

	int n=_vscprintf(pcFmt, ap);
	SetBufferLen(n);

	vsprintf(m_pc, pcFmt, ap);
	m_nLen=n;
	va_end(ap);
}

void StringA::FormatCat(PCCharA pcFmt, ...)
{
	if(!pcFmt)
		return;

	va_list ap;
	va_start(ap, pcFmt);

	int n=_vscprintf(pcFmt, ap);
	SetBufferLen(m_nLen+n);

	vsprintf(m_pc+m_nLen, pcFmt, ap);
	m_nLen+=n;
	va_end(ap);
}


void StringA::TrimLeft(PCCharA pcTrim)
{
	if(!pcTrim)
		return;

	PCCharA pcTemp;
	PCharA pc=m_pc;

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

	int n=(int)(pc-m_pc);
	if(n)
	{
		Mem_Move(m_pc, pc, (m_nLen-n+1)*sizeof(CharA));
		m_nLen-=n;
	}
}

void StringA::TrimRight(PCCharA pcTrim)
{
	if(!pcTrim)
		return;

	PCCharA pcTemp;
	PCharA pc=m_pc+m_nLen-1;

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

	*++pc='\0';
	m_nLen=(int)(pc-m_pc);
}

int StringA::Find(PCCharA pcFind, int nCount, int nStart)
{
	if(!pcFind || !*pcFind)
		return -1;

	if(nStart<0 || nStart>=m_nLen)
		return -1;

	int n;
	PCCharA pc1, pc2;
	PCCharA pc=m_pc+nStart;

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

int StringA::Find(CharA chFind, int nStart)
{
	if(!chFind)
		return -1;

	if(nStart<0 || nStart>=m_nLen)
		return -1;

	PCCharA pc=m_pc+nStart;

	while(*pc)
	{
		if(*pc==chFind)
			return (int)(pc-m_pc);

		pc++;
	}

	return -1;
}

int StringA::Comapre(const StringA& str, int nCount) const
{
	if(nCount<0)
		return StrA_Cmp(m_pc, str.m_pc);
	else
		return StrA_CmpN(m_pc, str.m_pc, nCount);
}

int StringA::Comapre(PCCharA pc, int nCount) const
{
	if(nCount<0)
		return StrA_Cmp(m_pc, pc);
	else
		return StrA_CmpN(m_pc, pc, nCount);
}

StringA StringA::Token(PCCharA pcToken, int& nStart)
{
	if(!pcToken || !*pcToken || nStart<0 || nStart>=m_nLen)
	{
		nStart=-1;
		return StringA();
	}

	PCharA pc0=m_pc+nStart;
	PCharA pc1=pc0;

	PCCharA pcTemp;
	while(*pc1)
	{
		pcTemp=pcToken;

		while(*pcTemp)
		{
			if(*pc1==*pcTemp)
			{
				nStart= (int)(pc1-m_pc);
				return StringA(pc0, (int)(pc1-pc0));
			}

			pcTemp++;
		}

		pc1++;
	}

	nStart= (int)(pc1-m_pc);
	return StringA(pc0, (int)(pc1-pc0));
}

void StringA::SetBufferLen(int nLen)
{
	if(nLen<0)
		return;

	Ulong sizeNew=(nLen+1)*sizeof(CharA);

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

			m_pc=(PCharA)Mem_Realloc(0, m_pc, sizeGrow);
		}
	}
	else
	{
		m_pc=(PCharA)Mem_Alloc(0, sizeNew);
		m_pc[0]='\0';
	}
}

Bool StringA::FromW(PCCharW pcSrc, int nCount, Uint uCodePage)
{
	if(!pcSrc)
		return FALSE;

	if(nCount<=0)
		nCount=-1;

	int n=WideCharToMultiByte(uCodePage, 0, pcSrc, nCount, NULL, 0, NULL, NULL);
	if(n)
	{
		SetBufferLen(n);
		WideCharToMultiByte(uCodePage, 0, pcSrc, nCount, m_pc, n, NULL, NULL);
		m_pc[n]='\0';
		m_nLen=n;
		return TRUE;
	}

	Empty();
	return FALSE;
}

Bool StringA::EnvVar(PCCharA pcVarName)
{
    Ulong ulCharCount = GetEnvironmentVariableA(pcVarName, NULL, 0);
    if (ulCharCount == 0) {
        return FALSE;
    }

	SetBufferLen(ulCharCount);

    ulCharCount = GetEnvironmentVariableA(pcVarName, m_pc, ulCharCount + 1);
	if (ulCharCount == 0) {
        return FALSE;
    }

    return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////
//// friend functions
//

TOLCPPDECL Bool TOLAPI operator==(const StringA& str1, const StringA& str2)
{
	return StrA_Cmp(str1.m_pc, str2.m_pc)==0;
}

TOLCPPDECL Bool TOLAPI operator==(const StringA& str, PCCharA pc)
{
	return StrA_Cmp(str.m_pc, pc)==0;
}

TOLCPPDECL Bool TOLAPI operator==(PCCharA pc, const StringA& str)
{
	return StrA_Cmp(pc, str.m_pc)==0;
}

TOLCPPDECL Bool TOLAPI operator==(const StringA& str, PCCharW pc)
{
	return StrA_Cmp(str.m_pc, StringA(pc))==0;
}

TOLCPPDECL Bool TOLAPI operator==(PCCharW pc, const StringA& str)
{
	return StrA_Cmp(StringA(pc), str.m_pc)==0;
}

TOLCPPDECL Bool TOLAPI operator!=(const StringA& str1, const StringA& str2)
{
	return StrA_Cmp(str1.m_pc, str2.m_pc)!=0;
}

TOLCPPDECL Bool TOLAPI operator!=(const StringA& str, PCCharA pc)
{
	return StrA_Cmp(str.m_pc, pc)!=0;
}

TOLCPPDECL Bool TOLAPI operator!=(PCCharA pc, const StringA& str)
{
	return StrA_Cmp(pc, str.m_pc)!=0;
}

TOLCPPDECL Bool TOLAPI operator!=(const StringA& str, PCCharW pc)
{
	return StrA_Cmp(str.m_pc, StringA(pc))!=0;
}

TOLCPPDECL Bool TOLAPI operator!=(PCCharW pc, const StringA& str)
{
	return StrA_Cmp(StringA(pc), str.m_pc)!=0;
}

//TOLCPPDECL StringA TOLAPI operator+(const StringA& str1, const StringA& str2)
//{
//	StringA ret(str1);
//	ret+=str2;
//	return ret;
//}
//
//TOLCPPDECL StringA TOLAPI operator+(const StringA& str, PCCharA pc)
//{
//	StringA ret(pc);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringA TOLAPI operator+(PCCharA pc, const StringA& str)
//{
//	StringA ret(pc);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringA TOLAPI operator+(const StringA& str, PCCharW pc)
//{
//	StringA ret(pc);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringA TOLAPI operator+(PCCharW pc, const StringA& str)
//{
//	StringA ret(pc);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringA TOLAPI operator+(const StringA& str, CharA ch)
//{
//	StringA ret(ch);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringA TOLAPI operator+(CharA ch, const StringA& str)
//{
//	StringA ret(ch);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringA TOLAPI operator+(const StringA& str, CharW ch)
//{
//	StringA ret(ch);
//	ret+=str;
//	return ret;
//}
//
//TOLCPPDECL StringA TOLAPI operator+(CharW ch, const StringA& str)
//{
//	StringA ret(ch);
//	ret+=str;
//	return ret;
//}



TOLNS_END
