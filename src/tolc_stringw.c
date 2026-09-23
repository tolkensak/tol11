
#include "stdafx.h"
#include <tolc_stringw.h>
#include <tolc_memory.h>

#include <stdio.h>
#include <stdarg.h>


TOLEXTC_BEGIN


TOLCDECL PCharW TOLAPI StrW_Make(int nLen)
{
	if(nLen>=0)
		return Mem_Alloc(MEM_ZERO_MEMORY, (nLen+1)*sizeof(CharW));

	return NULL;
}

TOLCDECL PCharW TOLAPI StrW_Remake(PCharW pcSrc, int nLen)
{
	if(pcSrc && nLen>0)
		return Mem_Realloc(0, pcSrc, (nLen+1)*sizeof(CharW));

	return NULL;
}

TOLCDECL PCharW TOLAPI StrW_Dup(PCCharW pcSrc)
{
	if(pcSrc)
	{
		PCharW pc=Mem_Alloc(0, (StrW_Len(pcSrc)+1)*sizeof(CharW));
		if(pc)
		{
			StrW_Copy(pc, pcSrc);
			return pc;
		}
	}

	return NULL;
}

TOLCDECL PCharW TOLAPI StrW_DupN(PCCharW pcSrc, int nCount)
{
	if(pcSrc && nCount>=0)
	{
		PCharW pc=Mem_Alloc(0, (nCount+1)*sizeof(CharW));
		if(pc)
		{
			StrW_CopyN(pc, pcSrc, nCount);
			return pc;
		}
	}

	return NULL;
}

TOLCDECL void TOLAPI StrW_Kill(PCharW pcSrc)
{
	if(pcSrc)
		Mem_Free(pcSrc);
}

TOLCDECL int TOLAPI StrW_Len(PCCharW pcSrc)
{
	if(pcSrc)
	{
		PCCharW pc=pcSrc;

		while(*pc!=L'\0')
			pc++;

		return (int)(pc-pcSrc);
	}

	return 0;
}

TOLCDECL void TOLAPI StrW_Copy(PCharW pcDest, PCCharW pcSrc)
{
	if(pcDest && pcSrc)
	{
		while(*pcSrc!=L'\0')
			*pcDest++=*pcSrc++;

		*pcDest=L'\0';
	}
}

TOLCDECL void TOLAPI StrW_CopyN(PCharW pcDest, PCCharW pcSrc, int nCount)
{
	if(pcDest && pcSrc)
	{
		while(*pcSrc!=L'\0' && nCount-->0)
			*pcDest++=*pcSrc++;

		*pcDest=L'\0';
	}
}

TOLCDECL void TOLAPI StrW_Cat(PCharW pcDest, PCCharW pcSrc)
{
	if(pcDest && pcSrc)
	{
		pcDest+=StrW_Len(pcDest);

		while(*pcSrc!=L'\0')
			*pcDest++=*pcSrc++;

		*pcDest=L'\0';
	}
}

TOLCDECL void TOLAPI StrW_CatN(PCharW pcDest, PCCharW pcSrc, int nCount)
{
	if(pcDest && pcSrc)
	{
		pcDest+=StrW_Len(pcDest);

		while(*pcSrc!=L'\0' && nCount-->0)
			*pcDest++=*pcSrc++;

		*pcDest=L'\0';
	}
}

TOLCDECL PCharW TOLAPI StrW_TrimL(PCharW pcSrc, PCCharW pcTrim)
{
	if(pcSrc)
	{
		if(pcTrim)
		{
			PCCharW pc;

			while(*pcSrc)
			{
				pc=pcTrim;

				while(*pc)
				{
					if(*pcSrc==*pc)
						break;

					pc++;
				}

				if(!*pc)
					break;

				pcSrc++;
			}
		}

		return pcSrc;
	}

	return NULL;
}

TOLCDECL PCharW TOLAPI StrW_TrimR(PCharW pcSrc, PCCharW pcTrim)
{
	if(pcSrc)
	{
		if(pcTrim)
		{
			PCCharW pc;
			PCharW pcIter=pcSrc;

			while(*pcIter++);
				pcIter-=2;

			while(pcIter>=pcSrc)
			{
				pc=pcTrim;

				while(*pc)
				{
					if(*pcIter==*pc)
						break;

					pc++;
				}

				if(!*pc)
					break;

				pcIter--;
			}

			*++pcIter=L'\0';
		}

		return pcSrc;
	}

	return NULL;
}

TOLCDECL PCharW TOLAPI StrW_Trim(PCharW pcSrc, PCCharW pcTrim)
{
	return StrW_TrimR(StrW_TrimL(pcSrc, pcTrim), pcTrim);
}

TOLCDECL PCCharW TOLAPI StrW_Char(PCCharW pcSrc, CharW chFind)
{
	if(chFind)
	{
		if(pcSrc)
		{
			while(*pcSrc)
			{
				if(*pcSrc==chFind)
					return pcSrc;

				pcSrc++;
			}
		}
	}

	return NULL;
}

TOLCDECL PCCharW TOLAPI StrW_Str(PCCharW pcSrc, PCCharW pcFind)
{
	if(pcSrc && *pcSrc && pcFind && *pcFind)
	{
		PCCharW pcS, pcF;

		while(*pcSrc)
		{
			pcS=pcSrc;
			pcF=pcFind;

			while(*pcF)
			{
				if(*pcS!=*pcF)
					break;

				pcS++;
				pcF++;
			}

			if(!*pcF)
				return pcSrc;

			pcSrc++;
		}
	}

	return NULL;
}

TOLCDECL PCCharW TOLAPI StrW_StrN(PCCharW pcSrc, PCCharW pcFind, int nCount)
{
	if(pcSrc && *pcSrc && pcFind && *pcFind && nCount>0)
	{
		int n;
		PCCharW pcS, pcF;

		while(*pcSrc)
		{
			n=0;
			pcS=pcSrc;
			pcF=pcFind;

			while(*pcF && n<nCount)
			{
				if(*pcS!=*pcF)
					break;

				pcS++;
				pcF++;
				n++;
			}

			if(!*pcF || n==nCount)
				return pcSrc;

			pcSrc++;
		}
	}

	return NULL;
}

TOLCDECL int TOLAPI StrW_Cmp(PCCharW pc1, PCCharW pc2)
{
	if(pc1 && pc2)
	{
		while(1)
		{
			if(!*pc1 && !*pc2)
				return 0;

			if(!*pc1)
				return -1;

			if(!*pc2)
				return 1;

			if(*pc1>*pc2)
				return 1;

			if(*pc1<*pc2)
				return -1;

			pc1++;
			pc2++;
		}
	}
	else if(pc1)
		return 1;
	else if(pc2)
		return -1;

	return 0;
}

TOLCDECL int TOLAPI StrW_CmpN(PCCharW pc1, PCCharW pc2, int nCount)
{
	if(nCount>0)
	{
		if(pc1 && pc2)
		{
			while(nCount--)
			{
				if(!*pc1 && !*pc2)
					return 0;

				if(!*pc1)
					return -1;

				if(!*pc2)
					return 1;

				if(*pc1>*pc2)
					return 1;

				if(*pc1<*pc2)
					return -1;

				pc1++;
				pc2++;
			}
		}
		else if(pc1)
			return 1;
		else if(pc2)
			return -1;
	}

	return 0;
}

TOLCDECL void TOLAPI StrW_RemoveCh(PCharW pcSrc, CharW chRemove)
{
	if(pcSrc && *pcSrc && chRemove)
	{
		while(*pcSrc)
		{
			if(*pcSrc==chRemove)
				StrW_Copy(pcSrc, pcSrc+1);
			else
				pcSrc++;
		}
	}
}

TOLCDECL void TOLAPI StrW_Remove(PCharW pcSrc, PCCharW pcRemove)
{
	if(pcSrc && *pcSrc && pcRemove && *pcRemove)
	{
		PCCharW pc;

		while(*pcSrc)
		{
			pc=pcRemove;

			while(*pc)
			{
				if(*pcSrc==*pc)
				{
					StrW_Copy(pcSrc, pcSrc+1);
					pcSrc--;
					break;
				}

				pc++;
			}

			pcSrc++;
		}
	}
}

TOLCDECL PCharW TOLAPI StrW_Token(PCharW pcSrc, PCCharW pcToken)
{
	if(pcSrc && *pcSrc && pcToken && *pcToken)
	{
		PCCharW pc;

		while(*pcSrc)
		{
			pc=pcToken;

			while(*pc)
			{
				if(*pcSrc==*pc)
				{
					*pcSrc=L'\0';
					return ++pcSrc;
				}

				pc++;
			}

			pcSrc++;
		}
	}

	return NULL;
}

TOLCDECL int TOLAPI StrW_Format(PCharW pcDest, PCCharW pcFmt, ...)
{
	int n=0;

	if(pcFmt)
	{
		va_list ap;
		va_start(ap, pcFmt);

		n=_vscwprintf(pcFmt, ap);

		if(pcDest)
			vswprintf(pcDest, pcFmt, ap);

		va_end(ap);
	}

	return n;
}

TOLCDECL PCharW TOLAPI StrW_EnvVar(PCCharW pcVarName)
{
    Ulong ulCharCount = GetEnvironmentVariableW(pcVarName, NULL, 0);
    if (ulCharCount == 0) {
        return NULL;
    }

	PCharW pcValue = StrW_Make(ulCharCount);

    ulCharCount = GetEnvironmentVariableW(pcVarName, pcValue, ulCharCount + 1);
	if (ulCharCount == 0) {
		StrW_Kill(pcValue);
        return NULL;
    }

    return pcValue;
}

TOLCDECL void TOLAPI StrW_ChangeEndian(PCharW pcSrc)
{
	if(pcSrc)
	{
		Byte by;
		PByte pby=(PByte)pcSrc;

		while(*pcSrc)
		{
			by=*pby;
			*pby=*(pby+1);
			*(pby+1)=by;

			pby+=2;
			pcSrc++;
		}
	}
}


TOLEXTC_END
