
#include "stdafx.h"
#include <tolc_stringa.h>
#include <tolc_memory.h>

#include <stdio.h>
#include <stdarg.h>


TOLEXTC_BEGIN


TOLCDECL PCharA TOLAPI StrA_Make(int nLen)
{
	if(nLen>=0)
		return Mem_Alloc(MEM_ZERO_MEMORY, (nLen+1)*sizeof(CharA));

	return NULL;
}

TOLCDECL PCharA TOLAPI StrA_Remake(PCharA pcSrc, int nLen)
{
	if(pcSrc && nLen>0)
		return Mem_Realloc(0, pcSrc, (nLen+1)*sizeof(CharA));

	return NULL;
}

TOLCDECL PCharA TOLAPI StrA_Dup(PCCharA pcSrc)
{
	if(pcSrc)
	{
		PCharA pc=Mem_Alloc(0, (StrA_Len(pcSrc)+1)*sizeof(CharA));
		if(pc)
		{
			StrA_Copy(pc, pcSrc);
			return pc;
		}
	}

	return NULL;
}

TOLCDECL PCharA TOLAPI StrA_DupN(PCCharA pcSrc, int nCount)
{
	if(pcSrc && nCount>=0)
	{
		PCharA pc=Mem_Alloc(0, (nCount+1)*sizeof(CharA));
		if(pc)
		{
			StrA_CopyN(pc, pcSrc, nCount);
			return pc;
		}
	}

	return NULL;
}

TOLCDECL void TOLAPI StrA_Kill(PCharA pcSrc)
{
	if(pcSrc)
		Mem_Free(pcSrc);
}

TOLCDECL int TOLAPI StrA_Len(PCCharA pcSrc)
{
	if(pcSrc)
	{
		PCCharA pc=pcSrc;

		while(*pc!='\0')
			pc++;

		return (int)(pc-pcSrc);
	}

	return 0;
}

TOLCDECL void TOLAPI StrA_Copy(PCharA pcDest, PCCharA pcSrc)
{
	if(pcDest && pcSrc)
	{
		while(*pcSrc!='\0')
			*pcDest++=*pcSrc++;

		*pcDest='\0';
	}
}

TOLCDECL void TOLAPI StrA_CopyN(PCharA pcDest, PCCharA pcSrc, int nCount)
{
	if(pcDest && pcSrc)
	{
		while(*pcSrc!='\0' && nCount-->0)
			*pcDest++=*pcSrc++;

		*pcDest='\0';
	}
}

TOLCDECL void TOLAPI StrA_Cat(PCharA pcDest, PCCharA pcSrc)
{
	if(pcDest && pcSrc)
	{
		pcDest+=StrA_Len(pcDest);

		while(*pcSrc!='\0')
			*pcDest++=*pcSrc++;

		*pcDest='\0';
	}
}

TOLCDECL void TOLAPI StrA_CatN(PCharA pcDest, PCCharA pcSrc, int nCount)
{
	if(pcDest && pcSrc)
	{
		pcDest+=StrA_Len(pcDest);

		while(*pcSrc!='\0' && nCount-->0)
			*pcDest++=*pcSrc++;

		*pcDest='\0';
	}
}

TOLCDECL PCharA TOLAPI StrA_TrimL(PCharA pcSrc, PCCharA pcTrim)
{
	if(pcSrc)
	{
		if(pcTrim)
		{
			PCCharA pc;

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

TOLCDECL PCharA TOLAPI StrA_TrimR(PCharA pcSrc, PCCharA pcTrim)
{
	if(pcSrc)
	{
		if(pcTrim)
		{
			PCCharA pc;
			PCharA pcIter=pcSrc;

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

			*++pcIter='\0';
		}

		return pcSrc;
	}

	return NULL;
}

TOLCDECL PCharA TOLAPI StrA_Trim(PCharA pcSrc, PCCharA pcTrim)
{
	return StrA_TrimR(StrA_TrimL(pcSrc, pcTrim), pcTrim);
}

TOLCDECL PCCharA TOLAPI StrA_Char(PCCharA pcSrc, CharA chFind)
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

TOLCDECL PCCharA TOLAPI StrA_Str(PCCharA pcSrc, PCCharA pcFind)
{
	if(pcSrc && *pcSrc && pcFind && *pcFind)
	{
		PCCharA pcS, pcF;

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

TOLCDECL PCCharA TOLAPI StrA_StrN(PCCharA pcSrc, PCCharA pcFind, int nCount)
{
	if(pcSrc && *pcSrc && pcFind && *pcFind && nCount>0)
	{
		int n;
		PCCharA pcS, pcF;

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

TOLCDECL int TOLAPI StrA_Cmp(PCCharA pc1, PCCharA pc2)
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

TOLCDECL int TOLAPI StrA_CmpN(PCCharA pc1, PCCharA pc2, int nCount)
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

TOLCDECL void TOLAPI StrA_RemoveCh(PCharA pcSrc, CharA chRemove)
{
	if(pcSrc && *pcSrc && chRemove)
	{
		while(*pcSrc)
		{
			if(*pcSrc==chRemove)
				StrA_Copy(pcSrc, pcSrc+1);
			else
				pcSrc++;
		}
	}
}

TOLCDECL void TOLAPI StrA_Remove(PCharA pcSrc, PCCharA pcRemove)
{
	if(pcSrc && *pcSrc && pcRemove && *pcRemove)
	{
		PCCharA pc;

		while(*pcSrc)
		{
			pc=pcRemove;

			while(*pc)
			{
				if(*pcSrc==*pc)
				{
					StrA_Copy(pcSrc, pcSrc+1);
					pcSrc--;
					break;
				}

				pc++;
			}

			pcSrc++;
		}
	}
}

TOLCDECL PCharA TOLAPI StrA_Token(PCharA pcSrc, PCCharA pcToken)
{
	if(pcSrc && *pcSrc && pcToken && *pcToken)
	{
		PCCharA pc;

		while(*pcSrc)
		{
			pc=pcToken;

			while(*pc)
			{
				if(*pcSrc==*pc)
				{
					*pcSrc='\0';
					return ++pcSrc;
				}

				pc++;
			}

			pcSrc++;
		}
	}

	return NULL;
}

TOLCDECL int TOLAPI StrA_Format(PCharA pcDest, PCCharA pcFmt, ...)
{
	int n=0;

	if(pcFmt)
	{
		va_list ap;
		va_start(ap, pcFmt);

		n=_vscprintf(pcFmt, ap);

		if(pcDest)
			vsprintf(pcDest, pcFmt, ap);

		va_end(ap);
	}

	return n;
}

TOLCDECL PCharA TOLAPI StrA_EnvVar(PCCharA pcVarName)
{
    Ulong ulCharCount = GetEnvironmentVariableA(pcVarName, NULL, 0);
    if (ulCharCount == 0) {
        return NULL;
    }

	PCharA pcValue = StrA_Make(ulCharCount);

    ulCharCount = GetEnvironmentVariableA(pcVarName, pcValue, ulCharCount + 1);
	if (ulCharCount == 0) {
		StrA_Kill(pcValue);
		return NULL;
    }

    return pcValue;
}


TOLEXTC_END
