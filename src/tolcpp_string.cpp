#include "stdafx.h"
#include <tolcpp_string.h>
#include <tolc_string.h>


TOLNS_BEGIN


TOLCPPDECL String TOLAPI PrefixSize(Qword qwSize)
{
	Char pch[TOL_MAXSTR];
	Str_PrefixSize(pch, TOL_MAXSTR, qwSize);

	return String(pch);
}


TOLNS_END
