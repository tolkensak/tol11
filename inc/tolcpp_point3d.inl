
#ifndef _INC_TOLCPP_POINT3D_INL
#define _INC_TOLCPP_POINT3D_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <math.h>

TOLNS_BEGIN


template<typename T>
Point3D<T>::Point3D()
	: Massive()
	, x(m_data[0])
	, y(m_data[1])
	, z(m_data[2])
{
}

template<typename T>
Point3D<T>::Point3D(const Point3D<T>& pt, int nCount)
	: Massive(pt, nCount)
	, x(m_data[0])
	, y(m_data[1])
	, z(m_data[2])
{
}

template<typename T>
Point3D<T>::Point3D(const T* p, int nCount)
	: Massive(p, nCount)
	, x(m_data[0])
	, y(m_data[1])
	, z(m_data[2])
{
}

template<typename T>
Point3D<T>::Point3D(const T& t, int nCount)
	: Massive(t, nCount)
	, x(m_data[0])
	, y(m_data[1])
	, z(m_data[2])
{
}

template<typename T>
Point3D<T>::Point3D(const T& x, const T& y, const T& z)
	: Massive()
	, x(m_data[0])
	, y(m_data[1])
	, z(m_data[2])
{
	this->x=x;
	this->y=y;
	this->z=z;
}

template<typename T>
Point3D<T>::~Point3D()
{
}

template<typename T>
TOLINL
void
Point3D<T>::Value(T& x, T& y, T& z) const
{
	x=this->x;
	y=this->y;
	z=this->z;
}

template<typename T>
TOLINL
void
Point3D<T>::Value(const T& x, const T& y, const T& z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}

template<typename T>
TOLINL
Point3D<T>&
Point3D<T>::operator=(const Point3D<T>& pt)
{
	Massive::operator=(pt);
	return *this;
}

template<typename T>
Point3D<T>
Point3D<T>::Vector(const Point3D<T>& pt)
{
	Point3D t(
		y*pt.z-z*pt.y,
		z*pt.x-x*pt.z,
		z*pt.y-y*pt.z
	);

	return t/sqrt(pow(t.x, 2)+pow(t.y, 2)+pow(t.z, 2));
}


TOLNS_END

#endif // _INC_TOLCPP_POINT3D_INL
