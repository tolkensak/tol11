
#ifndef _INC_TOLCPP_OBJECT_H
#define _INC_TOLCPP_OBJECT_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp.h>

TOLNS_BEGIN


// class Object

class TOLCPPDECL Object
{
private:
	Object(const Object&);
	void operator=(const Object&);

protected:
	Object();

public:
	virtual ~Object()=0; // virtual destructors are necessary

	//void* operator new(size_t sz);
	//void operator delete(void* p);

	//void* operator new[](size_t sz);
	//void operator delete[](void* p);

	//void* operator new(size_t sz, void* loc);
	//void operator delete(void* p, void* loc);
};


TOLNS_END

#include <tolcpp_object.inl>

#endif // _INC_TOLCPP_OBJECT_H
