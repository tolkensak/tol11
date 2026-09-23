
#ifndef _INC_TOLCPP_POINT3D_H
#define _INC_TOLCPP_POINT3D_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_massive.h>

TOLNS_BEGIN


template<typename T>
class TOLTEMPLDECL Point3D : public Massive<T, 3>
{
public:
	Point3D();
	Point3D(const Point3D& pt, int nCount=3);
	Point3D(const T* p, int nCount=3);
	Point3D(const T& t, int nCount=3);
	Point3D(const T& x, const T& y, const T& z);
	virtual ~Point3D();

	void Value(T& x, T& y, T& z) const;
	void Value(const T& x, const T& y, const T& z);

	Point3D& operator=(const Point3D& pt);
	Point3D Vector(const Point3D& pt);

	T& x;
	T& y;
	T& z;
};


TOLNS_END

#include <tolcpp_point3d.inl>

#endif // _INC_TOLCPP_POINT3D_H
