
#include "stdafx.h"
#include <tolc_string.h>
#include <math.h>


TOLEXTC_BEGIN


TOLCDECL PCharW TOLAPI Str_A2W(PCCharA pcSrc, int nCount, Uint uCodePage)
{
	if(pcSrc)
	{
		int n;

		if(nCount<=0)
			nCount=-1;

		n=MultiByteToWideChar(uCodePage, 0, pcSrc, nCount, NULL, 0);
		if(n)
		{
			PCharW pc=StrW_Make(n+1);
			if(pc)
			{
				MultiByteToWideChar(uCodePage, 0, pcSrc, nCount, pc, n);
				pc[n]=L'\0';

				return pc;
			}
		}
	}

	return NULL;
}

TOLCDECL PCharA TOLAPI Str_W2A(PCCharW pcSrc, int nCount, Uint uCodePage)
{
	if(pcSrc)
	{
		int n;

		if(nCount<=0)
			nCount=-1;

		n=WideCharToMultiByte(uCodePage, 0, pcSrc, nCount, NULL, 0, NULL, NULL);
		if(n)
		{
			PCharA pc=StrA_Make(n+1);
			if(pc)
			{
				WideCharToMultiByte(uCodePage, 0, pcSrc, nCount, pc, n, NULL, NULL);
				pc[n]='\0';

				return pc;
			}
		}
	}

	return NULL;
}

TOLCDECL int TOLAPI Str_PrefixSize(PChar pcDest, int cchDest, Qword qwSize)
{
	int cch=0;
	Qword qw=1024;
	PCChar pc=_T("KMGTPEZY"); /* Kilo Mega Gega Tera Peta Exa Zetta Yota */

	while(1)
	{
		if(qwSize<qw)
		{
			if(qw==1024)
			{
				cch=Str_Format(NULL, _T("%d B"), qwSize);
				if(pcDest)
				{
					if(cch>cchDest)
						cch*=-1;
					else
						Str_Format(pcDest, _T("%d B"), qwSize);
				}
			}
			else
			{
				float f;

				qw>>=10;
				pc--;
				f=((int)(qwSize/qw))+((float)(qwSize%qw)/qw);

				cch=Str_Format(NULL, _T("%.2f %cB"), f, *pc);
				if(pcDest)
				{
					if(cch>cchDest)
						cch*=-1;
					else
						Str_Format(pcDest, _T("%.2f %cB"), f, *pc);
				}
			}

			break;
		}

		qw<<=10;
		pc++;
	}

	return cch;
}


TOLEXTC_END
