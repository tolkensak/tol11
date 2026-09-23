
#ifndef _INC_TOLMFC_COMBOBOX_H
#define _INC_TOLMFC_COMBOBOX_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>
#include <tolcpp_ini.h>

TOLNS_BEGIN

// TComboBoxMRU

class TOLMFCDECL TComboBoxMRU : public CComboBox
{
public:
	TComboBoxMRU(LPCTSTR pcName=NULL);
	virtual ~TComboBoxMRU();

	void Add(LPCTSTR pc);

	//void Load(IniSetting& ini);
	//void Store(IniSetting& ini);

protected:
	CString m_strName;

	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
};

TOLNS_END

#endif /* _INC_TOLMFC_COMBOBOX_H */
