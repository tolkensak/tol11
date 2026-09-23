
#ifndef _INC_TOLMFC_GRIDCTRL_INL
#define _INC_TOLMFC_GRIDCTRL_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

TOLNS_BEGIN


TOLINL
int TGridCtrl::GetColumnCount()
{
	return GetHeaderCtrl()->GetItemCount();
}

TOLINL
CString TGridCtrl::GetEmptyText() const
{
	return m_strEmptyText;
}

TOLINL
void TGridCtrl::GetEmptyText(CString& str) const
{
	str=m_strEmptyText;
}


TOLNS_END

#endif // _INC_TOLCPP_EXCEPTION_INL
