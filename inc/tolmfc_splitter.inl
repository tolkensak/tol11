
#ifndef _INC_TOLCPP_SPLITTER_INL
#define _INC_TOLCPP_SPLITTER_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TSplitter::TPane

TOLINL
void TSplitter::TPane::Set(DWORD dwStyle, CWnd* pWnd, int nBreadth, int nMinBreadth, int nMaxBreadth)
{
	m_dwStyle=dwStyle;
	m_pWnd=pWnd;
	m_nBreadth=nBreadth;
	m_nMinBreadth=nMinBreadth;
	m_nMaxBreadth=nMaxBreadth;
}

TOLINL
TSplitter::TPane& TSplitter::TPane::operator=(const TSplitter::TPane& pane)
{
	m_dwStyle=pane.m_dwStyle;
	m_pWnd=pane.m_pWnd;
	m_nBreadth=pane.m_nBreadth;
	m_nMinBreadth=pane.m_nMinBreadth;
	m_nMaxBreadth=pane.m_nMaxBreadth;

	return *this;
}

TOLINL
BOOL TSplitter::TPane::IsSplitter() const
{
	return m_dwStyle&styleSplitter;
}

TOLINL
BOOL TSplitter::TPane::IsAutoSize() const
{
	return m_dwStyle&styleAutoSize;
}

TOLINL
int TSplitter::TPane::GetBreadth() const
{
	return m_nBreadth;
}

TOLINL
CWnd* TSplitter::TPane::GetWnd() const
{
	return m_pWnd;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TSplitter

TOLINL
BOOL TSplitter::IsHorz() const
{
	return m_bHorz;
}

TOLINL
BOOL TSplitter::IsVert() const
{
	return !m_bHorz;
}


TOLNS_END

#endif // _INC_TOLCPP_SPLITTER_INL
