
#ifndef _INC_TOLCPP_POINTER_INL
#define _INC_TOLCPP_POINTER_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

TOLNS_BEGIN


// class SmartPointer

template<typename T>
SmartPointer<T>::SmartPointer()
	: m_p(NULL)
{
}

template<typename T>
SmartPointer<T>::SmartPointer(const SmartPointer<T>& sp)
	: m_p(sp.m_p)
{
	if(m_p)
		m_p->IncRef();
}

template<typename T>
SmartPointer<T>::SmartPointer(T* p)
	: m_p(p)
{
	if(m_p)
		m_p->IncRef();
}

template<typename T>
SmartPointer<T>::~SmartPointer()
{
	if(m_p)
		m_p->DecRef();
}

template<typename T>
TOLINL
SmartPointer<T>::operator T*()
{
	return m_p;
}

template<typename T>
TOLINL
T&
SmartPointer<T>::operator*()
{
	return *m_p;
}

template<typename T>
TOLINL
T*
SmartPointer<T>::operator->()
{
	return m_p;
}

template<typename T>
TOLINL
SmartPointer<T>::operator T*() const
{
	return m_p;
}

template<typename T>
TOLINL
const T&
SmartPointer<T>::operator*() const
{
	return *m_p;
}

template<typename T>
TOLINL
const T*
SmartPointer<T>::operator->() const
{
	return m_p;
}

template<typename T>
TOLINL
SmartPointer<T>&
SmartPointer<T>::operator=(const SmartPointer<T>& sp)
{
	if(m_p)
		m_p->DecRef();

	m_p=sp.m_p;

	if(m_p)
		m_p->IncRef();

	return *this;
}

template<typename T>
TOLINL
SmartPointer<T>&
SmartPointer<T>::operator=(T* p)
{
	if(m_p)
		m_p->DecRef();

	m_p=p;

	if(m_p)
		m_p->IncRef();

	return *this;
}


// class SmartObject

TOLINL
void
SmartObject::IncRef()
{
	++m_nRefCount;
}

TOLINL
void
SmartObject::DecRef()
{
	if(--m_nRefCount==0)
		delete this;
}


// class SmartPointerCount

template<typename T>
SmartPointerCount<T>::SmartPointerCount()
	: m_p(NULL)
{
}

template<typename T>
SmartPointerCount<T>::SmartPointerCount(const SmartPointerCount<T>& sp)
	: m_p(sp.m_p)
{
	if(m_p)
		m_p->m_SmartCounter.IncRef();
}

template<typename T>
SmartPointerCount<T>::SmartPointerCount(T* p)
	: m_p(p)
{
	if(m_p)
		m_p->m_SmartCounter.IncRef();
}

template<typename T>
SmartPointerCount<T>::~SmartPointerCount()
{
	if(m_p && m_p->m_SmartCounter.DecRef())
		delete m_p;
}

template<typename T>
TOLINL
SmartPointerCount<T>::operator T*()
{
	return m_p;
}

template<typename T>
TOLINL
T&
SmartPointerCount<T>::operator*()
{
	return *m_p;
}

template<typename T>
TOLINL
T*
SmartPointerCount<T>::operator->()
{
	return m_p;
}

template<typename T>
TOLINL
SmartPointerCount<T>::operator T*() const
{
	return m_p;
}

template<typename T>
TOLINL
const T&
SmartPointerCount<T>::operator*() const
{
	return *m_p;
}

template<typename T>
TOLINL
const T*
SmartPointerCount<T>::operator->() const
{
	return m_p;
}

template<typename T>
TOLINL
SmartPointerCount<T>&
SmartPointerCount<T>::operator=(const SmartPointerCount<T>& sp)
{
	if(m_p && m_p->m_SmartCounter.DecRef())
		delete m_p;

	m_p=sp.m_p;

	if(m_p)
		m_p->m_SmartCounter.IncRef();

	return *this;
}

template<typename T>
TOLINL
SmartPointerCount<T>&
SmartPointerCount<T>::operator=(T* p)
{
	if(m_p && m_p->m_SmartCounter.DecRef())
		delete m_p;

	m_p=p;

	if(m_p)
		m_p->m_SmartCounter.IncRef();

	return *this;
}


// class SmartCounter

TOLINL
void
SmartCounter::IncRef()
{
	++m_nRefCount;
}

TOLINL
Bool
SmartCounter::DecRef()
{
	return --m_nRefCount==0;
}


TOLNS_END

#endif // _INC_TOLCPP_POINTER_INL
