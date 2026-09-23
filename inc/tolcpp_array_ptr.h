
#ifndef _INC_TOLCPP_ARRAY_PTR_H
#define _INC_TOLCPP_ARRAY_PTR_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_array.h>

TOLNS_BEGIN


/////////////////////////////////////////////////////////////////////////////
// PtrArray<TYPE, ARG_TYPE>

template<typename T, typename A=const T&>
class TOLTEMPLDECL PtrArray : public Array<T, A>
{
public:
	explicit PtrArray(int nCount=0);
	PtrArray(const PtrArray& arr);
	virtual ~PtrArray();

	virtual int Find(A a, int nStart=0) const; // defaults to starting at the top, return -1 if not found
	virtual void Sort(Bool bAscending=TRUE);
};


TOLNS_END

#include <tolcpp_array_ptr.inl>

#endif // _INC_TOLCPP_ARRAY_PTR_H
