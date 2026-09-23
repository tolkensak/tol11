
#include "stdafx.h"
#include <tolcpp_object.h>
#include <tolc_memory.h>

TOLNS_BEGIN


// class Object

Object::Object()
{
}

Object::Object(const Object&)
{
}

Object::~Object()
{
}

void Object::operator=(const Object&)
{
}

//void* Object::operator new(size_t sz)
//{
//	return Mem_Alloc(MEM_ZERO_MEMORY, sz);
//}
//
//void Object::operator delete(void* p)
//{
//	Mem_Free(p);
//}
//
//void* Object::operator new[](size_t sz)
//{
//	return Mem_Alloc(MEM_ZERO_MEMORY, sz);
//}
//
//void Object::operator delete[](void* p)
//{
//	Mem_Free(p);
//}
//
//void* Object::operator new(size_t sz, void* loc)
//{
//	return loc;
//}
//
//void Object::operator delete(void* p, void* loc)
//{
//}


TOLNS_END
