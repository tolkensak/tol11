
#ifndef _INC_TOLCPP_STRINGA_INL
#define _INC_TOLCPP_STRINGA_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <crtdbg.h> // _ASSERTE
#include <tolcpp_exception.h>

TOLNS_BEGIN


TOLINL int StringA::Len() const
{
	return m_nLen;
}

TOLINL Bool StringA::IsEmpty() const
{
	return m_nLen==0;
}

inline StringA::operator PCCharA() const
{
	return m_pc;
}

TOLINL CharA& StringA::operator[](int nIndex)
{
	_ASSERTE(nIndex>=0 && nIndex<m_nLen);

	if(nIndex<0 || nIndex>=m_nLen)
		throw ArgException();

	return m_pc[nIndex];
}

TOLINL CharA StringA::operator[](int nIndex) const
{
	_ASSERTE(nIndex>=0 && nIndex<m_nLen);

	if(nIndex<0 || nIndex>=m_nLen)
		throw ArgException();

	return m_pc[nIndex];
}

TOLINL StringA& StringA::operator=(const StringA& str)
{
	Copy(str);
	return *this;
}

TOLINL StringA& StringA::operator=(PCCharA pc)
{
	Copy(pc);
	return *this;
}

TOLINL StringA& StringA::operator=(PCCharW pc)
{
	Copy(pc);
	return *this;
}

TOLINL StringA& StringA::operator=(CharA ch)
{
	Copy(ch);
	return *this;
}

TOLINL StringA& StringA::operator=(CharW ch)
{
	Copy(ch);
	return *this;
}

TOLINL StringA& StringA::operator+=(const StringA& str)
{
	Cat(str);
	return *this;
}

TOLINL StringA& StringA::operator+=(PCCharA pc)
{
	Cat(pc);
	return *this;
}

TOLINL StringA& StringA::operator+=(PCCharW pc)
{
	Cat(pc);
	return *this;
}

TOLINL StringA& StringA::operator+=(CharA ch)
{
	Cat(ch);
	return *this;
}

TOLINL StringA& StringA::operator+=(CharW ch)
{
	Cat(ch);
	return *this;
}

TOLINL void StringA::Trim(PCCharA pcTrim)
{
	TrimLeft(pcTrim);
	TrimRight(pcTrim);
}

TOLINL StringA StringA::Left(int nCount)
{
	_ASSERTE(nCount>0);

	if(nCount<=0)
		throw ArgException();

	return StringA(m_pc, nCount);
}

TOLINL StringA StringA::Right(int nCount)
{
	_ASSERTE(nCount>0);

	if(nCount<=0)
		throw ArgException();

	if(nCount>m_nLen)
		nCount=m_nLen;

	return StringA(m_pc+m_nLen-nCount);
}

TOLINL StringA StringA::Middle(int nIndex, int nCount)
{
	_ASSERTE(nIndex>=0 && nIndex<m_nLen);

	if(nIndex<0 || nIndex>=m_nLen)
		throw ArgException();

	return StringA(m_pc+nIndex, nCount);
}


TOLNS_END

#endif // _INC_TOLCPP_STRINGA_INL
