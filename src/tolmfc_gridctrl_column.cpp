
#include "stdafx.h"
#include <tolmfc_gridctrl_column.h>

TOLNS_BEGIN


TGridCtrlColumn::TGridCtrlColumn(Type type, int nWidth, BOOL bHidden)
	: m_type(type)
	, m_nWidth(nWidth)
	, m_bHidden(bHidden)
{
}

TGridCtrlColumn::~TGridCtrlColumn()
{
}

int TGridCtrlColumn::GetAlign() const
{
	switch(m_type)
	{
	case typeFloat:
	case typeMoney:
		return LVCFMT_RIGHT;
	case typeInteger:
	case typeMarkup:
	case typeBoolean:
		return LVCFMT_CENTER;
	}

	return LVCFMT_LEFT;
}

void TGridCtrlColumn::Hide(BOOL bHide)
{
	if(bHide && m_nWidth<20)
		m_nWidth=LVSCW_AUTOSIZE;

	m_bHidden=bHide;
}


TOLNS_END
