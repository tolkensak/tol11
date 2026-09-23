
#include "stdafx.h"
#include <tolcpp_text.h>

TOLNS_BEGIN


const Text::CodePage Text::m_cp[EncodingCount]=
{
	{65001, 0, 0,   0,   0,   0},     // CP_UTF8
	{65001, 3, 0xEF,0xBB,0xBF,0},     // CP_UTF8SIG
	{1200,  2, 0xFF,0xFE,0,   0},     // CP_UTF16LE
	{1201,  2, 0xFE,0xFF,0,   0},     // CP_UTF16BE
#ifdef _UTF32_SUPPORT
	{12000, 4, 0xFF,0xFE,0x00,0x00}, // CP_UTF32LE
	{12001, 4, 0x00,0x00,0xFE,0xFF}, // CP_UTF32BE
#endif
};


Text::Text(Encoding enc)
	: m_pFile(NULL)
	, m_enc(ENC_UTF8)
{
	SetEncoding(enc);
}

//Text::Text(const Text&)
//{
//}

Text::~Text()
{
	Close();
}

//void Text::operator=(const Text&)
//{
//}

void Text::Close()
{
	if(m_pFile)
	{
		CloseHandle(m_pFile);
		m_pFile=NULL;
	}
}


TOLNS_END
