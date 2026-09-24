
#ifndef _INC_TOLCPP_ARRAY_INL
#define _INC_TOLCPP_ARRAY_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <new.h>    // ::new
#include <crtdbg.h> // _ASSERTE

#include <tolc_memory.h>
#include <tolcpp_exception.h>

TOLNS_BEGIN


/////////////////////////////////////////////////////////////////////////////
// Array<T, A> inline functions

template<typename T, typename A>
TOLINL
T*
Array<T, A>::Data()
{
	return m_pData;
}

template<typename T, typename A>
TOLINL
const T*
Array<T, A>::Data() const
{
	return m_pData;
}

template<typename T, typename A>
TOLINL
int
Array<T, A>::Count() const
{
	return m_nCount;
}

template<typename T, typename A>
TOLINL
Bool
Array<T, A>::IsEmpty() const
{
	return m_nCount==0;
}

template<typename T, typename A>
TOLINL
void
Array<T, A>::Empty()
{
	Count(0);
}

template<typename T, typename A>
TOLINL
void
Array<T, A>::RemoveAll()
{
	Count(0);
}

template<typename T, typename A>
TOLINL
T&
Array<T, A>::operator[](int nIndex)
{
	_ASSERTE(nIndex>=0 && nIndex<m_nCount);

	if(nIndex<0 || nIndex>=m_nCount)
		throw ArgException();

	return m_pData[nIndex];
}

template<typename T, typename A>
TOLINL
const T&
Array<T, A>::operator[](int nIndex) const
{
	_ASSERTE(nIndex>=0 && nIndex<m_nCount);

	if(nIndex<0 || nIndex>=m_nCount)
		throw ArgException();

	return m_pData[nIndex];
}

template<typename T, typename A>
TOLINL
T&
Array<T, A>::At(int nIndex)
{
	_ASSERTE(nIndex>=0 && nIndex<m_nCount);

	if(nIndex<0 || nIndex>=m_nCount)
		throw ArgException();

	return m_pData[nIndex];
}

template<typename T, typename A>
TOLINL
const T&
Array<T, A>::At(int nIndex) const
{
	_ASSERTE(nIndex>=0 && nIndex<m_nCount);

	if(nIndex<0 || nIndex>=m_nCount)
		throw ArgException();

	return m_pData[nIndex];
}


/////////////////////////////////////////////////////////////////////////////
// Array<T, A> out-of-line functions

template<typename T, typename A>
Array<T, A>::Array(int nCount)
	: m_pData(NULL)
	, m_nCount(0)
	, m_nCountMax(0)
	, m_nGrowBy(0)
{
	if(nCount>0)
		Count(nCount);
}

template<typename T, typename A>
Array<T, A>::Array(const Array<T, A>& arr)
	: m_pData(NULL)
	, m_nCount(0)
	, m_nCountMax(0)
	, m_nGrowBy(0)
{
	Count(arr.m_nCount);
	Mem_Copy(m_pData, arr.m_pData, arr.m_nCount*sizeof(T));
}

template<typename T, typename A>
Array<T, A>::~Array()
{
	if(m_pData)
	{
		for(int i=0; i<m_nCount; i++)
			(m_pData+i)->~T();

		Mem_Free(m_pData);
	}
}

template<typename T, typename A>
void
Array<T, A>::Count(int nCountNew, int nGrowBy)
{
	_ASSERTE(nCountNew>=0);

	if(nCountNew<0)
		throw ArgException();

	if(nCountNew==m_nCount)
		return;

	if(nGrowBy>=0)
		m_nGrowBy=nGrowBy;

	if(nCountNew==0)
	{ // shrink to nothing
		if(m_pData)
		{
			for(int i=0; i<m_nCount; i++)
				(m_pData+i)->~T();

			Mem_Free(m_pData);
			m_pData=NULL;
		}

		m_nCount=m_nCountMax=0;
	}
	else if(!m_pData)
	{ // create buffer big enough to hold number of requested elements or m_nGrowBy elements, whichever is larger.
#ifdef SIZE_T_MAX
		_ASSERTE(nCountNew<=SIZE_T_MAX/sizeof(T)); // no overflow
#endif

		int nCountMax=(nCountNew>m_nGrowBy)?nCountNew:m_nGrowBy;
		m_pData=(T*)Mem_Alloc(MEM_ZERO_MEMORY, nCountMax*sizeof(T));

		for(int i=0; i<nCountNew; i++)
#pragma push_macro("new")
#undef new
			::new((void*)(m_pData+i)) T;
#pragma pop_macro("new")

		m_nCount=nCountNew;
		m_nCountMax=nCountMax;
	}
	else if(nCountNew<=m_nCountMax)
	{
		if(nCountNew>m_nCount) // it fits
		{ // initialize the new elements
			Mem_Set(m_pData+m_nCount, 0, (nCountNew-m_nCount)*sizeof(T));

			for(int i=m_nCount; i<nCountNew; i++)
#pragma push_macro("new")
#undef new
				::new((void*)(m_pData+i)) T;
#pragma pop_macro("new")
		}
		else if(m_nCount>nCountNew)
		{ // destroy the old elements
			for(int i=nCountNew; i<m_nCount; i++)
				(m_pData+i)->~T();
		}

		m_nCount=nCountNew;
	}
	else
	{ // otherwise, grow array
		nGrowBy=m_nGrowBy;

		if(nGrowBy==0)
		{ // heuristically determine growth when nGrowBy==0 (this avoids heap fragmentation in many situations)
			nGrowBy=m_nCount/8;
			nGrowBy=(nGrowBy<4)?4:((nGrowBy>1024)?1024:nGrowBy);
		}

		int nCountMax;
		if(nCountNew<m_nCountMax+nGrowBy)
			nCountMax=m_nCountMax+nGrowBy; // granularity
		else
			nCountMax=nCountNew; // no slush

		_ASSERTE(nCountMax>=m_nCountMax); // no wrap around

		if(nCountMax<m_nCountMax)
			throw ArgException();

#ifdef SIZE_T_MAX
		_ASSERTE(nCountMax<=SIZE_T_MAX/sizeof(T)); // no overflow
#endif

		m_pData=(T*)Mem_Realloc(MEM_ZERO_MEMORY, m_pData, nCountMax*sizeof(T));

		_ASSERTE(nCountNew>m_nCount);

		for(int i=m_nCount; i<nCountNew; i++)
#pragma push_macro("new")
#undef new
			::new((void*)(m_pData+i)) T;
#pragma pop_macro("new")

		m_nCount=nCountNew;
		m_nCountMax=nCountMax;
	}
}

template<typename T, typename A>
void
Array<T, A>::Shrink()
{ // shrink to desired count
	if(m_nCount==m_nCountMax)
		return;

#ifdef SIZE_T_MAX
	_ASSERTE(m_nCount<=SIZE_T_MAX/sizeof(T)); // no overflow
#endif

	if(m_nCount)
		m_pData=(T*)Mem_Realloc(0, m_pData, m_nCountMax*sizeof(T));

	m_nCountMax=m_nCount;
}

//template<typename T, typename A>
//void
//Array<T, A>::SetAt(int nIndex, A a)
//{
//	_ASSERTE(nIndex>=0);
//	
//	if(nIndex<0)
//		throw ArgException();
//
//	if(nIndex>=m_nCount)
//		Count(nIndex+1);
//
//	m_pData[nIndex]=a;
//}

template<typename T, typename A>
TOLINL
Array<T, A>&
Array<T, A>::operator=(const Array<T, A>& arr)
{
	_ASSERTE(this!=&arr); // cannot assign to itself

	if(this!=&arr)
	{
		Count(arr.m_nCount);
		Mem_Copy(m_pData, arr.m_pData, arr.m_nCount*sizeof(T));
	}

	return *this;
}

template<typename T, typename A>
TOLINL
void
Array<T, A>::Copy(const Array<T, A>& arr)
{
	_ASSERTE(this!=&arr); // cannot assign to itself

	if(this!=&arr)
	{
		Count(arr.m_nCount);
		Mem_Copy(m_pData, arr.m_pData, arr.m_nCount*sizeof(T));
	}
}

template<typename T, typename A>
Array<T, A>&
Array<T, A>::operator+=(A a)
{
	int nIndex=m_nCount;
	Count(nIndex+1);
	m_pData[nIndex]=a;
	return *this;
}

template<typename T, typename A>
TOLINL
Array<T, A>&
Array<T, A>::operator+=(const Array<T, A>& arr)
{
	_ASSERTE(this!=&arr); // cannot append to itself

	if(this==&arr)
		throw ArgException();

	int nCountOld=m_nCount;
	Count(m_nCount+arr.m_nCount);
	Mem_Copy(m_pData+nCountOld, arr.m_pData, arr.m_nCount*sizeof(T));

	return *this;
}

template<typename T, typename A>
void
Array<T, A>::Add(A a, int nCount)
{
	int nIndex=m_nCount;
	Count(nIndex+nCount);

	for(; nIndex<m_nCount; nIndex++)
		m_pData[nIndex]=a;
}

template<typename T, typename A>
void
Array<T, A>::Add(const Array<T, A>& arr)
{
	_ASSERTE(this!=&arr); // cannot append to itself

	if(this==&arr)
		throw ArgException();

	int nCountOld=m_nCount;
	Count(m_nCount+arr.m_nCount);
	Mem_Copy(m_pData+nCountOld, arr.m_pData, arr.m_nCount*sizeof(T));
}

template<typename T, typename A>
void
Array<T, A>::Insert(int nIndex, A a, int nCount)
{
	_ASSERTE(nCount>0); // zero or negative count not allowed

	if(nCount<=0)
		throw ArgException();

	if(nIndex<0) // add if nIndex is -1
		nIndex=m_nCount;

	if(nIndex>=m_nCount)
	{ // adding after the end of the array
		Count(nIndex+nCount); // grow so nIndex is valid
	}
	else
	{ // inserting in the middle of the array
		int nCountOld=m_nCount;
		Count(m_nCount+nCount); // grow it to new count

		//for(int i=nCountOld; i<m_nCount; i++) // destroy intial data before copying over it
		//	(m_pData+i)->~T();

		Mem_Move(m_pData+nIndex+nCount, m_pData+nIndex, (nCountOld-nIndex)*sizeof(T)); // shift old data up to fill gap
		Mem_Set(m_pData+nIndex, 0, nCount*sizeof(T)); // re-init slots we copied from

		for(int i=0; i<nCount; i++)
#pragma push_macro("new")
#undef new
			::new((void*)(m_pData+nIndex+i)) T;
#pragma pop_macro("new")
	}

	// insert new value in the gap
	_ASSERTE(nIndex+nCount<=m_nCount);

	while(nCount--)
		m_pData[nIndex++]=a;
}

template<typename T, typename A>
void
Array<T, A>::Insert(int nIndex, const Array<T, A>& arr)
{
	_ASSERTE(this!=&arr); // cannot insert to itself

	if(this==&arr)
		throw ArgException();

	if(nIndex<0) // add if nIndex is -1
		nIndex=m_nCount;

	if(arr.m_nCount>0)
	{
		Insert(nIndex, arr.m_pData[0], arr.m_nCount);
		for(int i=0; i<arr.m_nCount; i++)
			m_pData[nIndex+i]=arr.m_pData[i];
	}
}

template<typename T, typename A>
void
Array<T, A>::Remove(int nIndex, int nCount)
{
	_ASSERTE(nIndex>=0);
	_ASSERTE(nCount>0);

	int nUpperBound=nIndex+nCount;

	_ASSERTE(nUpperBound<=m_nCount && nUpperBound>=nIndex && nUpperBound>=nCount);

	if(nIndex<0 || nCount<=0 || (nUpperBound>m_nCount) || (nUpperBound<nIndex) || (nUpperBound<nCount))
		throw ArgException();

	// just remove a range
	int nCountMove=m_nCount-nUpperBound;
	for(int i=0; i<nCount; i++)
		(m_pData+nIndex+i)->~T();

	if(nCountMove)
		Mem_Move(m_pData+nIndex, m_pData+nUpperBound, nCountMove*sizeof(T));

	m_nCount-=nCount;
}

template<typename T, typename A>
int
Array<T, A>::Find(A a, int nStart) const
{
	_ASSERTE(nStart>=0);

	if(nStart<0)
		throw ArgException();

	for(int i=nStart; i<m_nCount; i++)
		if(m_pData[i]==a)
			return i;

	return -1;
}

//template<typename T, typename A>
//void
//Array<T, A>::Sort(Bool bAscending)
//{
//	if(m_nCount<2)
//		return;
//
//	T t, *p1, *p2;
//	T *pLast=m_pData+m_nCount-1;
//
//	if(bAscending)
//	{
//		for(p1=m_pData; p1<pLast; p1++)
//			for(p2=p1+1; p2<=pLast; p2++)
//				if(*p1>*p2)
//				{
//					t=*p1;
//					*p1=*p2;
//					*p2=t;
//				}
//	}
//	else
//	{
//		for(p1=m_pData; p1<pLast; p1++)
//			for(p2=p1+1; p2<=pLast; p2++)
//				if(*p1<*p2)
//				{
//					t=*p1;
//					*p1=*p2;
//					*p2=t;
//				}
//	}
//}


TOLNS_END

#endif // _INC_TOLCPP_ARRAY_INL
