
#ifndef _INC_TOLC_DEBUG_H
#define _INC_TOLC_DEBUG_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


#define DBG_MSG_ERROR 0
#define DBG_MSG_WARN  1
#define DBG_MSG_INFO  2


#ifdef _DEBUG


TOLCDECL void TOLAPI Dbg_Out(PCChar pcFormat, ...);
TOLCDECL void TOLAPI Dbg_Msg(Uint uiType, PCChar pcExpr, PCChar pcFile, Ulong ulLine);

#define DBG_ASSERT(expr)   ((void)((expr) || (Dbg_Msg(DBG_MSG_ERROR, _T(#expr), _T(__FILE__), __LINE__), 0)))
#define DBG_VERIFY(expr)   ((void)((expr) || (Dbg_Msg(DBG_MSG_ERROR, _T(#expr), _T(__FILE__), __LINE__), 0)))
#define DBG_IFTRUE(expr)   ((expr) || (Dbg_Msg(DBG_MSG_ERROR, _T(#expr), _T(__FILE__), __LINE__), 0))
#define DBG_IFFAKE(expr)   ((expr) && (Dbg_Msg(DBG_MSG_ERROR, _T(#expr), _T(__FILE__), __LINE__), 1))


#else /* _DEBUG */


#define Dbg_Out(x)
#define Dbg_Msg(x1, x2, x3, x4)

#define DBG_ASSERT(x)
#define DBG_VERIFY(x)      (x)
#define DBG_IFTRUE(x)      (x)
#define DBG_IFFAKE(x)      (x)


#endif /* _DEBUG */



TOLEXTC_END

#endif /* _INC_TOLC_DEBUG_H */
