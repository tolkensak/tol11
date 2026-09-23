
#include "stdafx.h"
#include <tolc_debug.h>
#include <tolc_memory.h>

#include <stdio.h>
#include <stdarg.h>


TOLEXTC_BEGIN


#ifdef _DEBUG


TOLCDECL void TOLAPI Dbg_Out(PCChar pcFormat, ...)
{
	int n;
	PChar pc;
	va_list ap;

	if(!pcFormat)
		return;

	va_start(ap, pcFormat);

#if defined(_UNICODE) || defined(UNICODE)
	n=_vscwprintf(pcFormat, ap);
#else
	n=_vscprintf(pcFormat, ap);
#endif

	pc=Mem_Alloc(0, (n+1)*sizeof(TCHAR));
	if(!pc)
	{
		va_end(ap);
		return;
	}

/*
#if defined(_UNICODE) || defined(UNICODE)
	#if _MSC_VER>1310
		vswprintf(pc, n+1, pcFormat, ap);
	#else
		vswprintf(pc, pcFormat, ap);
	#endif
#else
	vsprintf(pc, pcFormat, ap);
#endif
*/

	_vstprintf(pc, pcFormat, ap);

#ifdef _WINBASE_
	OutputDebugString(pc);
#else
#error No alternative for OutputDebugString
#endif

	va_end(ap);

	Mem_Free(pc);
}

TOLCDECL void TOLAPI Dbg_Msg(Uint uiType, PCChar pcExpr, PCChar pcFile, Ulong ulLine)
{
	PCChar pc;

	switch(uiType)
	{
	case DBG_MSG_ERROR: pc=_T("Error"); break;
	case DBG_MSG_WARN: pc=_T("Warning"); break;
	case DBG_MSG_INFO: pc=_T("Information"); break;
	default: pc=_T("Message"); break;
	}

	Dbg_Out(_T("%s:\n%s (%d)\n%s\n"), pc, pcFile, ulLine, pcExpr);
}

#endif /* _DEBUG */


TOLEXTC_END
