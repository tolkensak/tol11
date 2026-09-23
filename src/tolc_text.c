
#include "stdafx.h"
#include <tolc_text.h>
#include <tolc_memory.h>
#include <tolc_debug.h>
#include <tolc_string.h>

#include <stdio.h>


TOLEXTC_BEGIN


typedef struct tagTextFileEncoding
{
	UINT uCodePage;
	int cbBOM;
	BYTE BOM[4];
} T_TEXTFILEENCODING, *PT_TEXTFILEENCODING;

const T_TEXTFILEENCODING _TextFileEncoding[]=
{
	{65001, 0, 0, 0, 0, 0},          // UTF8
	{65001, 3, 0xEF,0xBB,0xBF, 0},   // UTF8SIG
	{1200,  2, 0xFF,0xFE, 0, 0},     // UTF16LE
	{1201,  2, 0xFE,0xFF, 0, 0},     // UTF16BE
#ifdef _UTF32_SUPPORT
	{12000, 4, 0xFF,0xFE,0x00,0x00}, // UTF32LE
	{12001, 4, 0x00,0x00,0xFE,0xFF}, // UTF32BE
#endif
};

TOLCDECL UINT TOLAPI Text_GetEncoding(FILE* pFile)
{
	UINT uEncoding=0;
	int i, n, cb, cbRead;
	LPBYTE pby;
	PT_TEXTFILEENCODING ptfe;

	if(!pFile)
		return uEncoding;

	cb=0;
	n=sizeof(_TextFileEncoding)/sizeof(T_TEXTFILEENCODING);
	for(i=0; i<n; i++)
	{
		ptfe=(PT_TEXTFILEENCODING)(_TextFileEncoding+i);
		if(ptfe->cbBOM>cb)
			cb=ptfe->cbBOM;
	}

	if(!cb)
		return uEncoding;

	pby=Mem_Alloc(0, cb);
	if(!pby)
		return uEncoding;

	cbRead=fread(pby, sizeof(BYTE), cb, pFile);
	if(cbRead)
	{
		int j;

		for(i=0; i<n; i++)
		{
			ptfe=(PT_TEXTFILEENCODING)(_TextFileEncoding+i);
			cb=ptfe->cbBOM;

			if(cb>cbRead)
				continue;

			for(j=0; j<cb; j++)
				if(ptfe->BOM[j]!=pby[j])
				{
					j=-1;
					break;
				}

			if(j>0)
				break;
		}

		if(j>0)
			uEncoding=i;
		else
			cb=0;

		fseek(pFile, cb, SEEK_SET);
	}

	Mem_Free(pby);

	return uEncoding;
}

LPWSTR TOLAPI _Text_ReadLineUTF16LE(FILE* pFile)
{
	int i;
	WCHAR ch;
	LPWSTR pc, pc2;
	int cch=TOL_MAXSTR;

	if(!pFile || feof(pFile))
		return NULL;

	pc=StrW_Make(cch);
	if(!pc)
		return NULL;

	i=0;
	ch=getwc(pFile);
	while(!feof(pFile))
	{
		if(ch==L'\r')
		{
			ch=getwc(pFile);
			if(ch!=L'\n')
				ungetwc(ch, pFile);

			break;
		}

		if(ch==L'\n')
			break;

		pc[i]=ch;

		i++;
		if(i==cch)
		{
			cch<<=1;
			pc2=StrW_Remake(pc, cch);
			if(!pc2)
			{
				StrW_Kill(pc);
				return NULL;
			}

			pc=pc2;
		}

		ch=getwc(pFile);
	}

	pc[i]=L'\0';
	return pc;
}

LPWSTR TOLAPI _Text_ReadLineUTF16BE(FILE* pFile)
{
	int i;
	WCHAR ch;
	LPWSTR pc, pc2;
	int cch=TOL_MAXSTR;

	if(!pFile || feof(pFile))
		return NULL;

	pc=StrW_Make(cch);
	if(!pc)
		return NULL;

	i=0;
	ch=getwc(pFile);
	ch=MAKEWORD(HIBYTE(ch), LOBYTE(ch));
	while(!feof(pFile))
	{
		if(ch==L'\r')
		{
			ch=getwc(pFile);
			if(ch!=L'\n')
				ungetwc(ch, pFile);

			break;
		}

		if(ch==L'\n')
			break;

		pc[i]=ch;

		i++;
		if(i==cch)
		{
			cch<<=1;
			pc2=StrW_Remake(pc, cch);
			if(!pc2)
			{
				StrW_Kill(pc);
				return NULL;
			}

			pc=pc2;
		}

		ch=getwc(pFile);
		ch=MAKEWORD(HIBYTE(ch), LOBYTE(ch));
	}

	pc[i]=L'\0';
	return pc;
}

LPSTR TOLAPI _Text_ReadLineA(FILE* pFile)
{
	int i;
	CHAR ch;
	LPSTR pc, pc2;
	int cch=TOL_MAXSTR;

	if(!pFile || feof(pFile))
		return NULL;

	pc=StrA_Make(cch);
	if(!pc)
		return NULL;

	i=0;
	ch=getc(pFile);
	while(!feof(pFile))
	{
		if(ch=='\r')
		{
			ch=getc(pFile);
			if(ch!='\n')
				ungetc(ch, pFile);

			break;
		}

		if(ch=='\n')
			break;

		pc[i]=ch;

		i++;
		if(i==cch)
		{
			cch<<=1;
			pc2=StrA_Remake(pc, cch);
			if(!pc2)
			{
				StrA_Kill(pc);
				return NULL;
			}

			pc=pc2;
		}

		ch=getc(pFile);
	}

	pc[i]='\0';
	return pc;
}

TOLCDECL LPWSTR TOLAPI Text_ReadLineW(FILE* pFile, UINT uEncoding)
{
	if(!pFile)
		return NULL;

	switch(uEncoding)
	{
	case TFE_UTF16LE:
		return _Text_ReadLineUTF16LE(pFile);

	case TFE_UTF16BE:
		return _Text_ReadLineUTF16BE(pFile);

#ifdef _UTF32_SUPPORT
	case TFE_UTF32LE:
		#error No implementation
		break;

	case TFE_UTF32BE:
		#error No implementation
		break;
#endif

	default:
		{
			LPSTR pcA=_Text_ReadLineA(pFile);
			if(pcA)
			{
				LPWSTR pcW=Str_A2W(pcA, -1, _TextFileEncoding[uEncoding].uCodePage);
				StrA_Kill(pcA);
				return pcW;
			}
		}
		break;
	}

	return NULL;
}

TOLCDECL LPSTR TOLAPI Text_ReadLineA(FILE* pFile, UINT uEncoding)
{
	LPWSTR pcW=NULL;

	if(!pFile)
		return NULL;

	switch(uEncoding)
	{
	case TFE_UTF16LE:
		pcW=_Text_ReadLineUTF16LE(pFile);
		break;

	case TFE_UTF16BE:
		pcW=_Text_ReadLineUTF16BE(pFile);
		break;

#ifdef _UTF32_SUPPORT
	case TFE_UTF32LE:
		#error No implementation
		break;

	case TFE_UTF32BE:
		#error No implementation
		break;
#endif

	default:
		{
			LPSTR pcA=_Text_ReadLineA(pFile);
			if(pcA)
			{
				pcW=Str_A2W(pcA, -1, _TextFileEncoding[uEncoding].uCodePage);
				StrA_Kill(pcA);
			}
		}
		break;
	}

	if(pcW)
	{
		LPSTR pcA=Str_W2A(pcW, -1, 0);
		StrW_Kill(pcW);
		return pcA;
	}

	return NULL;
}


TOLCDECL void TOLAPI Text_SetEncoding(FILE* pFile, UINT uEncoding)
{
	UINT n=sizeof(_TextFileEncoding)/sizeof(T_TEXTFILEENCODING);

	if(pFile && uEncoding<n)
	{
		PT_TEXTFILEENCODING ptfe=(PT_TEXTFILEENCODING)(_TextFileEncoding+uEncoding);
		if(ptfe->cbBOM)
			fwrite(ptfe->BOM, sizeof(BYTE), ptfe->cbBOM, pFile);
	}
}

TOLCDECL DWORD TOLAPI Text_WriteW(FILE* pFile, LPCWSTR pcWrite, int cchWrite, UINT uEncoding)
{
	DWORD dwRet=0;

	if(!pFile || !pcWrite)
		return dwRet;

	switch(uEncoding)
	{
	case TFE_UTF16LE:
		dwRet=fwrite(pcWrite, sizeof(WCHAR), cchWrite>0?cchWrite:lstrlenW(pcWrite), pFile);
		break;

	case TFE_UTF16BE:
		{
			int cch=cchWrite>0?cchWrite:lstrlenW(pcWrite);
			LPWSTR pc=StrW_DupN(pcWrite, cch);
			if(pc)
			{
				StrW_ChangeEndian(pc);
				dwRet=fwrite(pc, sizeof(WCHAR), cch, pFile);
				StrW_Kill(pc);
			}
		}
		break;

#ifdef _UTF32_SUPPORT
	case TFE_UTF32LE:
		#error No implementation
		break;

	case TFE_UTF32BE:
		#error No implementation
		break;
#endif

	default:
		{
			LPSTR pcA=Str_W2A(pcWrite, cchWrite, _TextFileEncoding[uEncoding].uCodePage);
			if(pcA)
			{
				dwRet=fwrite(pcA, sizeof(CHAR), lstrlenA(pcA), pFile);
				StrA_Kill(pcA);
			}
		}
		break;
	}

	return dwRet;
}

TOLCDECL DWORD TOLAPI Text_WriteLineW(FILE* pFile, LPCWSTR pcWrite, int cchWrite, UINT uEncoding, UINT uEndStyle)
{
	if(pFile)
	{
		DWORD dw=Text_WriteW(pFile, pcWrite, cchWrite, uEncoding);

		switch(uEndStyle)
		{
		case TFES_WINDOWS: dw+=Text_WriteW(pFile, L"\r\n", 2, uEncoding); break;
		case TFES_UNIX:    dw+=Text_WriteW(pFile, L"\n", 1, uEncoding);   break;
		case TFES_MAC:     dw+=Text_WriteW(pFile, L"\r", 1, uEncoding);   break;
		}

		return dw;
	}

	return 0;
}

TOLEXTC_END
