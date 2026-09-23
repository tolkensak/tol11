
#ifndef _INC_TOLCPP_EXCEPTION_INL
#define _INC_TOLCPP_EXCEPTION_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

TOLNS_BEGIN


TOLINL
int Exception::Code() const
{
	return m_nCode;
}

TOLINL
void Exception::Code(int nCode)
{
	m_nCode=nCode;
}


TOLNS_END

#endif // _INC_TOLCPP_EXCEPTION_INL
