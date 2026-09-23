
#ifndef _INC_TOLCPP_STRING_H
#define _INC_TOLCPP_STRING_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_stringa.h>
#include <tolcpp_stringw.h>

TOLNS_BEGIN


#if defined(_UNICODE) || defined(UNICODE)
typedef StringW      String;
#else
typedef StringA      String;
#endif


TOLCPPDECL String TOLAPI PrefixSize(Qword qwSize);


TOLNS_END

#endif // _INC_TOLCPP_STRING_H
