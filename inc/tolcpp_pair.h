
#ifndef _INC_TOLCPP_PAIR_H
#define _INC_TOLCPP_PAIR_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_object.h>

TOLNS_BEGIN


/////////////////////////////////////////////////////////////////////////////
// Pair<TYPE1, TYPE2>

template<typename T1, typename T2>
class TOLTEMPLDECL Pair : public Object
{
public:
	Pair();
	Pair(const Pair& pair);
	Pair(const T1& one, const T2& two);
	virtual ~Pair();

	void Both(T1& one, T2& two) const;
	void Both(const T1& one, const T2& two);

	T1& One();
	const T1& One() const;

	T2& Two();
	const T2& Two() const;

	Pair& operator=(const Pair& pair);

	template<typename T1, typename T2> friend /*TOLTEMPLDECL*/ Bool TOLAPI operator==(const Pair& pair1, const Pair& pair2);
	template<typename T1, typename T2> friend /*TOLTEMPLDECL*/ Bool TOLAPI operator!=(const Pair& pair1, const Pair& pair2);

	template<typename T1, typename T2> friend /*TOLTEMPLDECL*/ Bool TOLAPI operator>(const Pair& pair1, const Pair& pair2);
	template<typename T1, typename T2> friend /*TOLTEMPLDECL*/ Bool TOLAPI operator<(const Pair& pair1, const Pair& pair2);

	template<typename T1, typename T2> friend /*TOLTEMPLDECL*/ Bool TOLAPI operator>=(const Pair& pair1, const Pair& pair2);
	template<typename T1, typename T2> friend /*TOLTEMPLDECL*/ Bool TOLAPI operator<=(const Pair& pair1, const Pair& pair2);

protected:
	T1 m_one;
	T2 m_two;
};


TOLNS_END

#include <tolcpp_pair.inl>

#endif // _INC_TOLCPP_PAIR_H
