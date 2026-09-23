
#ifndef _INC_TOLCPP_ARRAY_H
#define _INC_TOLCPP_ARRAY_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_object.h>

TOLNS_BEGIN


/////////////////////////////////////////////////////////////////////////////
// Array<TYPE, ARG_TYPE>

template<typename T, typename A=const T&>
class TOLTEMPLDECL Array : public Object
{
public:
	explicit Array(int nCount=0);
	Array(const Array& arr);
	virtual ~Array();

	T* Data();
	const T* Data() const;
	void Shrink();

	int Count() const;
	void Count(int nCountNew, int nGrowBy=-1);
	Bool IsEmpty() const;
	void Empty();

	T& operator[](int nIndex);
	const T& operator[](int nIndex) const;
	T& At(int nIndex);
	const T& At(int nIndex) const;

	Array& operator=(const Array& arr);
	void Copy(const Array& arr);

	Array& operator+=(A a);
	Array& operator+=(const Array& arr);
	void Add(A a, int nCount=1);
	void Add(const Array& arr);

	void Insert(int nIndex, A a, int nCount=1); // add if nIndex is -1
	void Insert(int nIndex, const Array& arr); // add if nIndex is -1
	void Remove(int nIndex, int nCount=1);
	void RemoveAll();

	virtual int Find(A a, int nStart=0) const; // defaults to starting at the top, return -1 if not found
	//virtual void Sort(Bool bAscending=TRUE);

protected:
	T* m_pData;  // the actual array of data
	int m_nCount;     // # of elements
	int m_nCountMax;  // max allocated
	int m_nGrowBy;    // grow amount
};


TOLNS_END

#include <tolcpp_array.inl>

#endif // _INC_TOLCPP_ARRAY_H
