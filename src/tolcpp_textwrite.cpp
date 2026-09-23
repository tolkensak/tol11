
#include "stdafx.h"
#include <tolcpp_text.h>

#include <tolc_memory.h>
#include <tolcpp_array.h>
#include <tolc_string.h>

TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////
// TextWriter


TextWriter::TextWriter(Encoding enc, EndStyle es)
	: Text(enc)
	, m_es(ES_WINDOWS)
{
	SetEndStyle(es);
}

//TextWriter::TextWriter(const TextWriter&)
//{
//}

TextWriter::~TextWriter()
{
}

//void TextWriter::operator=(const TextWriter&)
//{
//}

void TextWriter::SetEndStyle(EndStyle es)
{
	if(es>=0 && es<EndStyleNum)
		m_es=es;
}

Ulong TextWriter::Open(PCChar pcFile)
{
	Close();

	if(!pcFile)
		return ERROR_BAD_PATHNAME;

	m_pFile=CreateFile(pcFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_pFile==INVALID_HANDLE_VALUE)
	{
		m_pFile=NULL;
		return GetLastError();
	}

	WriteBOM();

	return ERROR_SUCCESS;
}

void TextWriter::Close()
{
	if(m_pFile)
		FlushFileBuffers(m_pFile);

	Text::Close();
}

void TextWriter::WriteBOM()
{
	PCodePage pcp=(PCodePage)(m_cp+m_enc);
	if(pcp->cbBOM)
	{
		Ulong ulWritten=0;
		WriteFile(m_pFile, pcp->BOM, pcp->cbBOM, &ulWritten, NULL);
	}
}

Bool TextWriter::Write(PCCharA pcWrite, int cchWrite)
{
	if(!m_pFile && !pcWrite)
		return FALSE;

	if(cchWrite<1)
		cchWrite=StrA_Len(pcWrite);

	switch(m_enc)
	{
	case ENC_UTF16LE:
		{
			StringW strw(pcWrite, cchWrite, m_cp[m_enc].uCP);
			if(!strw.IsEmpty())
			{
				Ulong ulWritten=0;
				return WriteFile(m_pFile, strw, strw.Len()*sizeof(CharW), &ulWritten, NULL);
			}
		}
		break;

	case ENC_UTF16BE:
		{
			StringW strw(pcWrite, cchWrite, m_cp[m_enc].uCP);
			if(!strw.IsEmpty())
			{
				strw.ChangeEndian();

				Ulong ulWritten=0;
				return WriteFile(m_pFile, strw, strw.Len()*sizeof(CharW), &ulWritten, NULL);
			}
		}
		break;

#ifdef _UTF32_SUPPORT
	case ENC_UTF32LE:
		#error No implementation
		break;

	case ENC_UTF32BE:
		#error No implementation
		break;
#endif

	default:
		{
			Ulong ulWritten=0;
			return WriteFile(m_pFile, pcWrite, cchWrite*sizeof(CharA), &ulWritten, NULL);
		}
		break;
	}

	return FALSE;
}

Bool TextWriter::Write(PCCharW pcWrite, int cchWrite)
{
	if(!m_pFile && !pcWrite)
		return FALSE;

	if(cchWrite<1)
		cchWrite=StrW_Len(pcWrite);

	switch(m_enc)
	{
	case ENC_UTF16LE:
		{
			Ulong ulWritten=0;
			return WriteFile(m_pFile, pcWrite, cchWrite*sizeof(CharW), &ulWritten, NULL);
		}
		break;

	case ENC_UTF16BE:
		{
			StringW strw(pcWrite, cchWrite);
			if(!strw.IsEmpty())
			{
				strw.ChangeEndian();

				Ulong ulWritten=0;
				return WriteFile(m_pFile, strw, strw.Len()*sizeof(CharW), &ulWritten, NULL);
			}
		}
		break;

#ifdef _UTF32_SUPPORT
	case ENC_UTF32LE:
		#error No implementation
		break;

	case ENC_UTF32BE:
		#error No implementation
		break;
#endif

	default:
		{
			StringA stra(pcWrite, cchWrite, m_cp[m_enc].uCP);
			if(!stra.IsEmpty())
			{
				Ulong ulWritten=0;
				return WriteFile(m_pFile, stra, stra.Len()*sizeof(CharA), &ulWritten, NULL);
			}
		}
		break;
	}

	return FALSE;
}

Bool TextWriter::WriteLine(PCCharA pcWrite, int cchWrite)
{
	Bool bRet=Write(pcWrite, cchWrite);

	if(bRet)
	{
		switch(m_es)
		{
		case ES_WINDOWS: Write("\r\n", 2); break;
		case ES_UNIX:    Write("\n", 1);   break;
		case ES_MAC:     Write("\r", 1);   break;
		}
	}

	return bRet;
}

Bool TextWriter::WriteLine(PCCharW pcWrite, int cchWrite)
{
	Bool bRet=Write(pcWrite, cchWrite);

	if(bRet)
	{
		switch(m_es)
		{
		case ES_WINDOWS: Write(L"\r\n", 2); break;
		case ES_UNIX:    Write(L"\n", 1);   break;
		case ES_MAC:     Write(L"\r", 1);   break;
		}
	}

	return bRet;
}


TOLNS_END
