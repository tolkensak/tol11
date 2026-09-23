
#include "stdafx.h"
#include <tolcpp_exception.h>

TOLNS_BEGIN

Exception::Exception()
{
	m_nCode=0;
}

Exception::Exception(const Exception&)
{
}

Exception::~Exception()
{
}

void Exception::operator=(const Exception&)
{
}



ArgException::ArgException()
{
}

ArgException::ArgException(const ArgException&)
{
}

ArgException::~ArgException()
{
}

void ArgException::operator=(const ArgException&)
{
}


TOLNS_END
