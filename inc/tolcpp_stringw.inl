
#ifndef _INC_TOLCPP_STRINGW_INL
#define _INC_TOLCPP_STRINGW_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <crtdbg.h> // _ASSERTE
#include <tolcpp_exception.h>
#include "tolcpp_stringw.h"

TOLNS_BEGIN


TOLINL int StringW::Len() const
{
	return m_nLen;
}

TOLINL Bool StringW::IsEmpty() const
{
	return m_nLen==0;
}

inline StringW::operator PCCharW() const
{
	return m_pc;
}

TOLINL CharW& StringW::operator[](int nIndex)
{
	_ASSERTE(nIndex>=0 && nIndex<m_nLen);

	if(nIndex<0 || nIndex>=m_nLen)
		throw ArgException();

	return m_pc[nIndex];
}

TOLINL CharW StringW::operator[](int nIndex) const
{
	_ASSERTE(nIndex>=0 && nIndex<m_nLen);

	if(nIndex<0 || nIndex>=m_nLen)
		throw ArgException();

	return m_pc[nIndex];
}

TOLINL StringW& StringW::operator=(const StringW& str)
{
	Copy(str);
	return *this;
}

TOLINL StringW& StringW::operator=(PCCharW pc)
{
	Copy(pc);
	return *this;
}

TOLINL StringW& StringW::operator=(PCCharA pc)
{
	Copy(pc);
	return *this;
}

TOLINL StringW& StringW::operator=(CharW ch)
{
	Copy(ch);
	return *this;
}

TOLINL StringW& StringW::operator=(CharA ch)
{
	Copy(ch);
	return *this;
}

TOLINL StringW& StringW::operator+=(const StringW& str)
{
	Cat(str);
	return *this;
}

TOLINL StringW& StringW::operator+=(PCCharW pc)
{
	Cat(pc);
	return *this;
}

TOLINL StringW& StringW::operator+=(PCCharA pc)
{
	Cat(pc);
	return *this;
}

TOLINL StringW& StringW::operator+=(CharW ch)
{
	Cat(ch);
	return *this;
}

TOLINL StringW& StringW::operator+=(CharA ch)
{
	Cat(ch);
	return *this;
}

TOLINL void StringW::Trim(PCCharW pcTrim)
{
	TrimLeft(pcTrim);
	TrimRight(pcTrim);
}

TOLINL StringW StringW::Left(int nCount)
{
	_ASSERTE(nCount>0);

	if(nCount<=0)
		throw ArgException();

	return StringW(m_pc, nCount);
}

TOLINL StringW StringW::Right(int nCount)
{
	_ASSERTE(nCount>0);

	if(nCount<=0)
		throw ArgException();

	if(nCount>m_nLen)
		nCount=m_nLen;

	return StringW(m_pc+m_nLen-nCount);
}

TOLINL StringW StringW::Middle(int nIndex, int nCount)
{
	_ASSERTE(nIndex>=0 && nIndex<m_nLen);

	if(nIndex<0 || nIndex>=m_nLen)
		throw ArgException();

	return StringW(m_pc+nIndex, nCount);
}


TOLNS_END

#endif // _INC_TOLCPP_STRINGW_INL
