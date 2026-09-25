
#ifndef _INC_TOLCPP_EXCEPTION_H
#define _INC_TOLCPP_EXCEPTION_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_object.h>

TOLNS_BEGIN


class TOLCPPDECL Exception : public Object
{
protected:
	Exception();

private:
	Exception(const Exception&);
	void operator=(const Exception&);

public:
	virtual ~Exception()=0;

	int GetCode() const;
	void SetCode(int nCode);

protected:
	int m_nCode;
};


class TOLCPPDECL ArgException : public Exception
{
public:
	ArgException();
	virtual ~ArgException();

	//#if _MSC_VER < 1400
public:
//#else
//private:
//#endif
	ArgException(const ArgException&);
	void operator=(const ArgException&);
};

class TOLCPPDECL RuntimeException : public Exception
{
public:
	RuntimeException();
	virtual ~RuntimeException();
	
//#if _MSC_VER < 1400
public:
//#else
//private:
//#endif
	RuntimeException(const RuntimeException&);
	void operator=(const RuntimeException&);
};


TOLNS_END

#include <tolcpp_exception.inl>

#endif // _INC_TOLCPP_EXCEPTION_H
