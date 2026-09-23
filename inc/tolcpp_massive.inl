
#ifndef _INC_TOLCPP_MASSIVE_INL
#define _INC_TOLCPP_MASSIVE_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <crtdbg.h> // _ASSERTE
#include <tolcpp_exception.h>

TOLNS_BEGIN


template<typename T, int N>
Massive<T, N>::Massive()
{
}

template<typename T, int N>
Massive<T, N>::Massive(const Massive<T, N>& m, int nCount)
{
	_ASSERTE(nCount>0 && nCount<=N);

	if(nCount<=0 || nCount>N)
		throw ArgException();

	for(int i=0; i<nCount; i++)
		m_data[i]=m.m_data[i];
}

template<typename T, int N>
Massive<T, N>::Massive(const T* p, int nCount)
{
	_ASSERTE(p);
	_ASSERTE(nCount>0 && nCount<=N);

	if(!p || nCount<=0 || nCount>N)
		throw ArgException();

	for(int i=0; i<nCount; i++)
		m_data[i]=p[i];
}

template<typename T, int N>
Massive<T, N>::Massive(const T& t, int nCount)
{
	_ASSERTE(nCount>0 && nCount<=N);

	if(nCount<=0 || nCount>N)
		throw ArgException();

	for(int i=0; i<nCount; i++)
		m_data[i]=t;
}

template<typename T, int N>
Massive<T, N>::~Massive()
{
}

template<typename T, int N>
TOLINL
T*
Massive<T, N>::Data()
{
	return m_data;
}

template<typename T, int N>
TOLINL
const T*
Massive<T, N>::Data() const
{
	return m_data;
}

template<typename T, int N>
TOLINL
int
Massive<T, N>::Count() const
{
	return N;
}

template<typename T, int N>
TOLINL
T&
Massive<T, N>::operator[](int nIndex)
{
	_ASSERTE(nIndex>=0 && nIndex<N);

	if(nIndex<0 || nIndex>=N)
		throw ArgException();

	return m_data[nIndex];
}

template<typename T, int N>
TOLINL
const T&
Massive<T, N>::operator[](int nIndex) const
{
	_ASSERTE(nIndex>=0 && nIndex<N);

	if(nIndex<0 || nIndex>=N)
		throw ArgException();

	return m_data[nIndex];
}

template<typename T, int N>
Massive<T, N>&
Massive<T, N>::operator=(const Massive<T, N>& m)
{
	_ASSERTE(this!=&m); // cannot assign to itself

	if(this!=&m)
	{
		for(int i=0; i<N; i++)
			m_data[i]=m.m_data[i];
	}

	return *this;
}

template<typename T, int N>
Massive<T, N>&
Massive<T, N>::operator+=(const Massive<T, N>& m)
{
	_ASSERTE(this!=&m); // cannot assign to itself

	if(this!=&m)
	{
		for(int i=0; i<N; i++)
			m_data[i]+=m.m_data[i];
	}

	return *this;
}

template<typename T, int N>
Massive<T, N>&
Massive<T, N>::operator-=(const Massive<T, N>& m)
{
	_ASSERTE(this!=&m); // cannot assign to itself

	if(this!=&m)
	{
		for(int i=0; i<N; i++)
			m_data[i]-=m.m_data[i];
	}

	return *this;
}

template<typename T, int N>
Massive<T, N>&
Massive<T, N>::operator*=(const Massive<T, N>& m)
{
	_ASSERTE(this!=&m); // cannot assign to itself

	if(this!=&m)
	{
		for(int i=0; i<N; i++)
			m_data[i]*=m.m_data[i];
	}

	return *this;
}

template<typename T, int N>
Massive<T, N>&
Massive<T, N>::operator/=(const Massive<T, N>& m)
{
	_ASSERTE(this!=&m); // cannot assign to itself

	if(this!=&m)
	{
		for(int i=0; i<N; i++)
			m_data[i]/=m.m_data[i];
	}

	return *this;
}



template<typename T, int N>
TOLTEMPLDECL
Massive<T, N>
TOLAPI
operator+(const Massive<T, N>& m1, const Massive<T, N>& m2)
{
	Massive<T, N> r(m1);

	for(int i=0; i<N; i++)
		r.m_data[i]+=m2.m_data[i];

	return r;
}

template<typename T, int N>
TOLTEMPLDECL
Massive<T, N>
TOLAPI
operator-(const Massive<T, N>& m1, const Massive<T, N>& m2)
{
	Massive<T, N> r(m1);

	for(int i=0; i<N; i++)
		r.m_data[i]-=m2.m_data[i];

	return r;
}

template<typename T, int N>
TOLTEMPLDECL
Massive<T, N>
TOLAPI
operator*(const Massive<T, N>& m1, const Massive<T, N>& m2)
{
	Massive<T, N> r(m1);

	for(int i=0; i<N; i++)
		r.m_data[i]*=m2.m_data[i];

	return r;
}

template<typename T, int N>
TOLTEMPLDECL
Massive<T, N>
TOLAPI
operator/(const Massive<T, N>& m1, const Massive<T, N>& m2)
{
	Massive<T, N> r(m1);

	for(int i=0; i<N; i++)
		r.m_data[i]/=m2.m_data[i];

	return r;
}

template<typename T, int N>
TOLTEMPLDECL
Bool
TOLAPI
operator==(const Massive<T, N>& m1, const Massive<T, N>& m2)
{
	for(int i=0; i<N; i++)
		if(m_data[i]!=m.m_data[i])
			return FALSE;

	return TRUE;
}

template<typename T, int N>
TOLTEMPLDECL
Bool
TOLAPI
operator!=(const Massive<T, N>& m1, const Massive<T, N>& m2)
{
	for(int i=0; i<N; i++)
		if(m_data[i]==m.m_data[i])
			return FALSE;

	return TRUE;
}


TOLNS_END

#endif // _INC_TOLCPP_MASSIVE_INL
