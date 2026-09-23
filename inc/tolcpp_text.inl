
#ifndef _INC_TOLCPP_TEXT_INL
#define _INC_TOLCPP_TEXT_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

TOLNS_BEGIN


TOLINL
PVoid Text::GetFile()
{
	return m_pFile;
}

TOLINL
Text::Encoding Text::GetEncoding() const
{
	return m_enc;
}

TOLINL
Bool Text::SetEncoding(Text::Encoding enc)
{
	if(enc<0 || enc>=EncodingCount)
		return FALSE;

	m_enc=enc;
	return TRUE;
}


TOLINL
PByte TextReader::View::GetBuffer()
{
	return m_pBuffer?((m_ulOffset==~0)?NULL:((PByte)m_pBuffer+m_ulOffset)):NULL;
}

TOLINL
Ulong TextReader::View::GetOffset() const
{
	return m_ulOffset;
}

TOLINL
void TextReader::View::SetOffset(Ulong ulOffset)
{
	m_ulOffset=ulOffset;
}

TOLINL
void TextReader::View::AddOffset(Ulong ulOffset)
{
	m_ulOffset+=ulOffset;
}

TOLINL
PByte TextReader::GetBuffer()
{
	return m_view.GetBuffer(m_enc);
}

TOLINL
PByte TextReader::View::GetBuffer(Encoding enc)
{
	return m_pBuffer?((PByte)m_pBuffer+m_cp[enc].cbBOM):NULL;
}


TOLNS_END

#endif // _INC_TOLCPP_TEXT_INL
