
#ifndef _INC_TOLMFC_GRIDCTRL_COLUMN_H
#define _INC_TOLMFC_GRIDCTRL_COLUMN_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

TOLNS_BEGIN


class TOLMFCDECL TGridCtrlColumn
{
public:
	enum Type
	{
		  typeText
		, typeInteger
		, typeMarkup
		, typeFloat
		, typeMoney
		, typeBoolean
		, typeDateTime
		, typeDate
		, typeTime
	};

public:
	TGridCtrlColumn(Type type=typeText, int nWidth=-1, BOOL bHidden=FALSE);
	~TGridCtrlColumn();

	Type GetType() const;
	void SetType(Type type);
	int GetAlign() const;

	int GetWidth() const;
	void SetWidth(int nWidth);

	BOOL IsHidden() const;
	void Hide(BOOL bHide=TRUE);

protected:
	Type m_type;
	int m_nWidth;
	BOOL m_bHidden;
};

typedef CArray<TGridCtrlColumn, TGridCtrlColumn&> TGridCtrlColumnArray;


TOLNS_END

#endif /* _INC_TOLMFC_GRIDCTRL_COLUMN_H */


#include <tolmfc_gridctrl_column.inl>
