
#ifndef _INC_TOLMFC_UTIL_INL
#define _INC_TOLMFC_UTIL_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

TOLNS_BEGIN


TOLINL
BOOL IsKeyPressed(SHORT shVirKey)
{
	return GetKeyState(shVirKey)>>(sizeof(SHORT)*8-1);
}

TOLINL
BOOL IsKeyToggled(SHORT shVirKey)
{
	return GetKeyState(shVirKey)&1;
}


TOLNS_END

#endif /* _INC_TOLMFC_UTIL_INL */
