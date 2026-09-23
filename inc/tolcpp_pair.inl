
#ifndef _INC_TOLCPP_PAIR_INL
#define _INC_TOLCPP_PAIR_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

TOLNS_BEGIN


/////////////////////////////////////////////////////////////////////////////
// Pair<T1, T2> inline functions

template<typename T1, typename T2>
TOLINL
void
Pair<T1, T2>::Both(T1& one, T2& two) const
{
	one=m_one;
	two=m_two;
}

template<typename T1, typename T2>
TOLINL
void
Pair<T1, T2>::Both(const T1& one, const T2& two)
{
	m_one=one;
	m_two=two;
}

template<typename T1, typename T2>
TOLINL
T1&
Pair<T1, T2>::One()
{
	return m_one;
}

template<typename T1, typename T2>
TOLINL
const T1&
Pair<T1, T2>::One() const
{
	return m_one;
}

template<typename T1, typename T2>
TOLINL
T2&
Pair<T1, T2>::Two()
{
	return m_two;
}

template<typename T1, typename T2>
TOLINL
const T2&
Pair<T1, T2>::Two() const
{
	return m_two;
}

template<typename T1, typename T2>
TOLINL
Pair<T1, T2>&
Pair<T1, T2>::operator=(const Pair<T1, T2>& pair)
{
	m_one=pair.m_one;
	m_two=pair.m_two;

	return *this;
}


/////////////////////////////////////////////////////////////////////////////
// Pair<T1, T2> out-of-line functions

template<typename T1, typename T2>
Pair<T1, T2>::Pair()
{
}

template<typename T1, typename T2>
Pair<T1, T2>::Pair(const Pair<T1, T2>& pair)
	: m_one(pair.m_one)
	, m_two(pair.m_two)
{
}

template<typename T1, typename T2>
Pair<T1, T2>::Pair(const T1& one, const T2& two)
	: m_one(one)
	, m_two(two)
{
}

template<typename T1, typename T2>
Pair<T1, T2>::~Pair()
{
}

/////////////////////////////////////////////////////////////////////////////
// Pair<T1, T2> firend functions

template<typename T1, typename T2>
TOLINL
Bool
TOLAPI
operator==(const Pair<T1, T2>& pair1, const Pair<T1, T2>& pair2)
{
	return	pair1.m_one==pair2.m_one && pair1.m_two==pair2.m_two;
}

template<typename T1, typename T2>
TOLINL
Bool
TOLAPI
operator!=(const Pair<T1, T2>& pair1, const Pair<T1, T2>& pair2)
{
	return	pair1.m_one!=pair2.m_one || pair1.m_two!=pair2.m_two;
}

template<typename T1, typename T2>
TOLINL
Bool
TOLAPI
operator>(const Pair<T1, T2>& pair1, const Pair<T1, T2>& pair2)
{
	return	pair1.m_one>pair2.m_one/* && pair1.m_two>pair2.m_two*/;
}

template<typename T1, typename T2>
TOLINL
Bool
TOLAPI
operator<(const Pair<T1, T2>& pair1, const Pair<T1, T2>& pair2)
{
	return	pair1.m_one<pair2.m_one/* && pair1.m_two<pair2.m_two*/;
}

template<typename T1, typename T2>
TOLINL
Bool
TOLAPI
operator>=(const Pair<T1, T2>& pair1, const Pair<T1, T2>& pair2)
{
	return	pair1.m_one>=pair2.m_one/* && pair1.m_two>=pair2.m_two*/;
}

template<typename T1, typename T2>
TOLINL
Bool
TOLAPI
operator<=(const Pair<T1, T2>& pair1, const Pair<T1, T2>& pair2)
{
	return	pair1.m_one<=pair2.m_one/* && pair1.m_two<=pair2.m_two*/;
}


TOLNS_END

#endif // _INC_TOLCPP_PAIR_INL
