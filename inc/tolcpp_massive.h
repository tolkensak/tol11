
#ifndef _INC_TOLCPP_MASSIVE_H
#define _INC_TOLCPP_MASSIVE_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_object.h>

TOLNS_BEGIN


template<typename T, int N>
class TOLTEMPLDECL Massive : public Object
{
public:
	Massive();
	Massive(const Massive& m, int nCount=N);
	Massive(const T* p, int nCount=N);
	Massive(const T& t, int nCount=N);
	virtual ~Massive();

	T* Data();
	const T* Data() const;

	int Count() const;

	T& operator[](int nIndex);
	const T& operator[](int nIndex) const;

	Massive& operator=(const Massive& m);
	Massive& operator+=(const Massive& m);
	Massive& operator-=(const Massive& m);
	Massive& operator*=(const Massive& m);
	Massive& operator/=(const Massive& m);

	template<typename T, int N> friend TOLTEMPLDECL Massive TOLAPI operator+(const Massive& m1, const Massive& m2);
	template<typename T, int N> friend TOLTEMPLDECL Massive TOLAPI operator-(const Massive& m1, const Massive& m2);
	template<typename T, int N> friend TOLTEMPLDECL Massive TOLAPI operator*(const Massive& m1, const Massive& m2);
	template<typename T, int N> friend TOLTEMPLDECL Massive TOLAPI operator/(const Massive& m1, const Massive& m2);

	template<typename T, int N> friend TOLTEMPLDECL Bool TOLAPI operator==(const Massive& m1, const Massive& m2);
	template<typename T, int N> friend TOLTEMPLDECL Bool TOLAPI operator!=(const Massive& m1, const Massive& m2);

protected:
	T m_data[N];
};


TOLNS_END

#include <tolcpp_massive.inl>

#endif // _INC_TOLCPP_MASSIVE_H
