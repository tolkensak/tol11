
#ifndef _INC_TOLMFC_WINAPP_INL
#define _INC_TOLMFC_WINAPP_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

TOLNS_BEGIN


TOLINL
LANGID TWinAppLang::GetLangID() const
{
	return m_lid;
}


TOLNS_END

#endif /* _INC_TOLMFC_WINAPP_INL */
