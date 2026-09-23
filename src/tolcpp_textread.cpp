
#include "stdafx.h"
#include <tolcpp_text.h>

#include <tolc_memory.h>
#include <tolcpp_array.h>

TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////
// TextReader::View


TextReader::View::View(PCChar pcName)
	: m_pMap(NULL)
	, m_strName(pcName)
	, m_pBuffer(NULL)
	, m_ulOffset(0)
{
}

TextReader::View::~View()
{
	Close();
}

void TextReader::View::Close()
{
	if(m_pBuffer)
		UnmapViewOfFile(m_pBuffer);

	if(m_pMap)
		CloseHandle(m_pMap);

	m_pBuffer=NULL;
	m_pMap=NULL;
	m_ulOffset=0;
}

Bool TextReader::View::Open(PVoid pFile)
{
	Close();

	if(!pFile)
		return FALSE;

	//m_hMap=OpenFileMapping(
	//	FILE_MAP_READ,
	//	FALSE,
	//	m_pcName);

	//if(!m_hMap)
	//{
		m_pMap=CreateFileMapping(
				pFile,         // use paging file
				NULL,          // default security 
				PAGE_READONLY, // read &| write access
				0,             // max. object size 
				0,             // buffer size  
				m_strName);    // name of mapping object

		if(!m_pMap)
		{
			//printf("Could not create file mapping object (%d).\n", GetLastError());
			return FALSE;
		}
	//}

	m_pBuffer=MapViewOfFile(
		m_pMap,        // handle to map object
		FILE_MAP_READ, // read/write permission
		0,
		0,
		0);

	if(!m_pBuffer)
	{
		//printf("Could not map view of file (%d).\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// TextReader


TextReader::TextReader(Encoding enc)
	: Text(enc)
{
}

//TextReader::TextReader(const TextReader&)
//{
//}

TextReader::~TextReader()
{
}

//void TextReader::operator=(const TextReader&)
//{
//}

Ulong TextReader::Open(PCChar pcFile)
{
	Close();

	if(!pcFile)
		return ERROR_BAD_PATHNAME;

	m_pFile=CreateFile(pcFile, GENERIC_READ, OPEN_EXISTING, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_pFile==INVALID_HANDLE_VALUE)
	{
		m_pFile=NULL;
		return GetLastError();
	}

	m_view.Open(m_pFile);
	ReadBOM();

	return ERROR_SUCCESS;
}

void TextReader::Close()
{
	Text::Close();
}

void TextReader::ReadBOM()
{
	if(!m_pFile)
		return;

	int i;
	int cb=0;
	PCodePage pcp;

	for(i=0; i<EncodingCount; i++)
	{
		pcp=(PCodePage)(m_cp+i);
		if(pcp->cbBOM>cb)
			cb=pcp->cbBOM;
	}

	if(!cb)
		return;

	PByte pby=(PByte)Mem_Alloc(0, cb);
	if(!pby)
		return;

	Ulong ulRead=0;
	if(ReadFile(m_pFile, pby, cb, &ulRead, NULL) && ulRead)
	{
		Bool bOk=FALSE;
		int cbRead=(int)ulRead;
		int j;

		for(i=0; i<EncodingCount; i++)
		{
			pcp=(PCodePage)(m_cp+i);
			cb=pcp->cbBOM;
			if(cb>cbRead)
				continue;

			for(j=0; j<cb; j++)
			{
				if(pcp->BOM[j]!=pby[j])
				{
					bOk=FALSE;
					break;
				}

				bOk=TRUE;
			}

			if(bOk)
				break;
		}

		if(bOk)
			m_enc=(Encoding)i;
		else
			cb=0;

		SetFilePointer(m_pFile, cb, NULL, FILE_BEGIN);
		m_view.SetOffset(cb);
	}

	Mem_Free(pby);
}

void TextReader::ReadEnd(PCharA pc, PByte pby)
{
	if(!*pc)
		m_view.SetOffset(~0);
	else
	{
		if(*pc=='\r' && *(pc+1)=='\n')
			pc++;

		pc++;
		m_view.AddOffset((ULONG)pc- (ULONG)pby);
	}
}

void TextReader::ReadEnd(PCharW pc, PByte pby)
{
	if(!*pc)
		m_view.SetOffset(~0);
	else
	{
		if(*pc==L'\r' && *(pc+1)==L'\n')
			pc++;

		pc++;
		m_view.AddOffset((ULONG)pc-(ULONG)pby);
	}
}

Bool TextReader::ReadLine(StringA& str)
{
	str.Empty();

	if(!m_pFile)
		return FALSE;

	PByte pbyView=m_view.GetBuffer();
	if(!pbyView)
		return FALSE;

	switch(m_enc)
	{
	case ENC_UTF16LE:
		{
			PCharW pc=(PCharW)pbyView;
			while(*pc && *pc!=L'\r' && *pc!=L'\n')
				pc++;

			int n=(int)(pc-(PCharW)pbyView);
			if(n)
			{
				str.Copy((PCCharW)pbyView, n);
				ReadEnd(pc, pbyView);
			}
		}
		break;

	case ENC_UTF16BE:
		{
			CharW ch;
			Array<CharW> chs;
			PCharW pc=(PCharW)pbyView;

			while(*pc)
			{
				ch=MAKEWORD(HIBYTE(*pc), LOBYTE(*pc));
				if(ch==L'\r' || ch==L'\n')
					break;

				chs+=ch;
				pc++;
			}

			if(!chs.IsEmpty())
			{
				str.Copy((PCCharW)chs.Data(), chs.Count());

				if(*pc==L'\0')
					m_view.SetOffset(~0);
				else
				{
					if(ch==L'\r' && MAKEWORD(HIBYTE(*(pc+1)), LOBYTE(*(pc+1)))==L'\n')
						pc++;

					pc++;
					m_view.AddOffset((ULONG)pc-(ULONG)pbyView);
				}
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
			PCharA pc=(PCharA)pbyView;
			while(*pc && *pc!='\r' && *pc!='\n')
				pc++;

			int n=(int)pc-(int)pbyView;
			if(n)
			{
				StringW strw((PCCharA)pbyView, n, m_cp[m_enc].uCP);
				str.Copy(strw, strw.Len());
				ReadEnd(pc, pbyView);
			}
		}
		break;
	}

	return TRUE;
}

Bool TextReader::ReadLine(StringW& str)
{
	str.Empty();

	if(!m_pFile)
		return FALSE;

	PByte pbyView=m_view.GetBuffer();
	if(!pbyView)
		return FALSE;

	switch(m_enc)
	{
	case ENC_UTF16LE:
		{
			PCharW pc=(PCharW)pbyView;
			while(*pc && *pc!=L'\r' && *pc!=L'\n')
				pc++;

			str.Copy(pc, (int)pc-(int)pbyView);
			ReadEnd(pc, pbyView);
		}
		break;

	case ENC_UTF16BE:
		{
			CharW ch;
			PCharW pc=(PCharW)pbyView;

			while(*pc)
			{
				ch=MAKEWORD(HIBYTE(*pc), LOBYTE(*pc));
				if(ch==L'\r' || ch==L'\n')
					break;

				str+=ch;
				pc++;
			}

			if(*pc==L'\0')
				m_view.SetOffset(~0);
			else
			{
				if(ch==L'\r' && MAKEWORD(HIBYTE(*(pc+1)), LOBYTE(*(pc+1)))==L'\n')
					pc++;

				pc++;
				m_view.AddOffset((ULONG)pc-(ULONG)pbyView);
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
			PCharA pc=(PCharA)pbyView;
			while(*pc && *pc!='\r' && *pc!='\n')
				pc++;

			int n=(int)pc-(int)pbyView;
			if(n)
				str.Copy((PCCharA)pbyView, n, m_cp[m_enc].uCP);

			ReadEnd(pc, pbyView);
		}
		break;
	}

	return TRUE;
}


TOLNS_END
