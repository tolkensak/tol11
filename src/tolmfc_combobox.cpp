
#include "stdafx.h"
#include <tolmfc_combobox.h>


TOLNS_BEGIN


// TComboBoxMRU

TComboBoxMRU::TComboBoxMRU(LPCTSTR pcName)
	: m_strName(_T(""))
{
	if(pcName)
		m_strName=pcName;
}

TComboBoxMRU::~TComboBoxMRU()
{
}


BEGIN_MESSAGE_MAP(TComboBoxMRU, CComboBox)
END_MESSAGE_MAP()


void TComboBoxMRU::PreSubclassWindow()
{
	DWORD dw=GetWindowLong(m_hWnd, GWL_STYLE);
	dw&=~(CBS_OWNERDRAWFIXED|CBS_OWNERDRAWVARIABLE|CBS_SORT|CBS_UPPERCASE|CBS_LOWERCASE);
	dw|=CBS_DROPDOWN|CBS_AUTOHSCROLL;
	SetWindowLongPtr(m_hWnd, GWL_STYLE, dw);
}


void TComboBoxMRU::Add(LPCTSTR pc)
{
	if(!pc)
		return;

	int i=0;
	while((i=FindStringExact(i-1, pc))!=CB_ERR)
		DeleteString(i);

	InsertString(0, pc);
	SetCurSel(0);
}

void LoadMRU(LPCTSTR pcKey, LPCTSTR pcVal, LPARAM lParam)
{
	((CComboBox*)lParam)->AddString(pcVal);
}

//void TComboBoxMRU::Load(IniSetting& ini)
//{
//	if(!m_strName.IsEmpty())
//		ini.Enum(m_strName, LoadMRU, (LPARAM)this);
//}
//
//void TComboBoxMRU::Store(IniSetting& ini)
//{
//	if(m_strName.IsEmpty())
//		return;
//
//	int n=GetCount();
//	if(n)
//	{
//		CString str;
//		IniSetting::Section* pSec=new IniSetting::Section;
//
//		for(int i=0; i<n; i++)
//		{
//			GetLBText(i, str);
//			pSec->arrValue+=(LPCTSTR)str;
//		}
//
//		ini.Write(m_strName, pSec);
//	}
//}


TOLNS_END
