
#ifndef _INC_TOLCPP_POINTER_H
#define _INC_TOLCPP_POINTER_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_object.h>

TOLNS_BEGIN


// class SmartPointer

template<typename T>
class TOLTEMPLDECL SmartPointer
{
public:
	SmartPointer();
	SmartPointer(const SmartPointer& sp);
	SmartPointer(T* p);
	~SmartPointer();

	operator T*();
	T& operator*();
	T* operator->();

	operator T*() const;
	const T& operator*() const;
	const T* operator->() const;

	SmartPointer& operator=(const SmartPointer& sp);
	SmartPointer& operator=(T* p);

private:
	T* m_p;
};


// class SmartObject

class TOLCPPDECL SmartObject : public Object
{
protected:
	SmartObject(); // no instance

public:
	virtual ~SmartObject()=0;

private:
	int m_nRefCount;

	void IncRef();
	void DecRef();

	template<typename T> friend class SmartPointer;
};



// class SmartPointerCount

template<typename T>
class TOLTEMPLDECL SmartPointerCount
{
public:
	SmartPointerCount();
	SmartPointerCount(const SmartPointerCount& sp);
	SmartPointerCount(T* p);
	~SmartPointerCount();

	operator T*();
	T& operator*();
	T* operator->();

	operator T*() const;
	const T& operator*() const;
	const T* operator->() const;

	SmartPointerCount& operator=(const SmartPointerCount& sp);
	SmartPointerCount& operator=(T* p);

private:
	T* m_p;
};

// class SmartCounter

class TOLCPPDECL SmartCounter
{
public:
	SmartCounter();
	~SmartCounter();

private:
	int m_nRefCount;

	void IncRef();
	Bool DecRef();

	template<typename T> friend class SmartPointerCount;
};


#define DECLARE_SMARTCOUNTER \
private: \
	SmartCounter m_SmartCounter; \
	template<typename T> friend class SmartPointerCount;



TOLNS_END

#include <tolcpp_pointer.inl>

#endif // _INC_TOLCPP_POINTER_H
