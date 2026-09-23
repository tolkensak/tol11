
#ifndef _INC_TOLMFC_GRIDCTRL_COLUMN_INL
#define _INC_TOLMFC_GRIDCTRL_COLUMN_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

TOLNS_BEGIN


TOLINL
TGridCtrlColumn::Type TGridCtrlColumn::GetType() const
{
	return m_type;
}

TOLINL
void TGridCtrlColumn::SetType(TGridCtrlColumn::Type type)
{
	m_type=type;
}

TOLINL
int TGridCtrlColumn::GetWidth() const
{
	return m_nWidth;
}

TOLINL
void TGridCtrlColumn::SetWidth(int nWidth)
{
	m_nWidth=nWidth;
}

TOLINL
BOOL TGridCtrlColumn::IsHidden() const
{
	return m_bHidden;
}


TOLNS_END

#endif /* _INC_TOLMFC_GRIDCTRL_COLUMN_INL */
