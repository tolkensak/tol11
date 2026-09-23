
#ifndef _INC_TOLCPP_EXCEPTION_H
#define _INC_TOLCPP_EXCEPTION_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_object.h>

TOLNS_BEGIN


class TOLCPPDECL Exception : public Object
{
private:
	Exception(const Exception&);
	void operator=(const Exception&);

protected:
	Exception();

public:
	virtual ~Exception()=0;

	int Code() const;
	void Code(int nCode);

protected:
	int m_nCode;
};


class TOLCPPDECL ArgException : public Exception
{
//#if _MSC_VER < 1400
public:
//#else
//private:
//#endif
	ArgException(const ArgException&);
	void operator=(const ArgException&);

public:
	ArgException();
	virtual ~ArgException();
};


TOLNS_END

#include <tolcpp_exception.inl>

#endif // _INC_TOLCPP_EXCEPTION_H
