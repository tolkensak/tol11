
#ifndef _INC_TOLCPP_ARRAY_PTR_INL
#define _INC_TOLCPP_ARRAY_PTR_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <new.h>    // ::new
#include <crtdbg.h> // _ASSERTE

#include <tolc_memory.h>
#include <tolcpp_exception.h>

TOLNS_BEGIN



template<typename T, typename A>
PtrArray<T, A>::PtrArray(int nCount)
	: Array(nCount)
{
}

template<typename T, typename A>
PtrArray<T, A>::PtrArray(const PtrArray<T, A>& arr)
	: Array(arr)
{
}

template<typename T, typename A>
PtrArray<T, A>::~PtrArray()
{
}


template<typename T, typename A>
int
PtrArray<T, A>::Find(A a, int nStart) const
{
	_ASSERTE(nStart>=0);

	if(nStart<0)
		throw ArgException();

	for(int i=nStart; i<m_nCount; i++)
		if(m_pData[i]==a)
			return i;

	return -1;
}

template<typename T, typename A>
void
PtrArray<T, A>::Sort(Bool bAscending)
{
	if(m_nCount<2)
		return;

	T t, *p1, *p2;
	T *pLast=m_pData+m_nCount-1;

	if(bAscending)
	{
		for(p1=m_pData; p1<pLast; p1++)
			for(p2=p1+1; p2<=pLast; p2++)
				if(**p1>**p2)
				{
					t=*p1;
					*p1=*p2;
					*p2=t;
				}
	}
	else
	{
		for(p1=m_pData; p1<pLast; p1++)
			for(p2=p1+1; p2<=pLast; p2++)
				if(**p1<**p2)
				{
					t=*p1;
					*p1=*p2;
					*p2=t;
				}
	}
}


TOLNS_END

#endif // _INC_TOLCPP_ARRAY_PTR_INL
