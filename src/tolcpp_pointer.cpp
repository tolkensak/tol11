
#include "stdafx.h"
#include <tolcpp_pointer.h>

TOLNS_BEGIN


// class SmartObject

SmartObject::SmartObject()
	: m_nRefCount(0)
{
}

SmartObject::~SmartObject()
{
}


// class SmartCounter

SmartCounter::SmartCounter()
	: m_nRefCount(0)
{
}

SmartCounter::~SmartCounter()
{
}


TOLNS_END
