
#ifndef _INC_TOLMFC_UTIL_H
#define _INC_TOLMFC_UTIL_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmfc.h>

TOLNS_BEGIN


TOLMFCDECL int TOLAPI MsgBox(CWnd* pParent, UINT uStyle, UINT uTextID, ...);
TOLMFCDECL int TOLAPI MsgBox(CWnd* pParent, UINT uStyle, LPCTSTR pcText, ...);


BOOL IsKeyPressed(SHORT shVirKey);
BOOL IsKeyToggled(SHORT shVirKey);


TOLNS_END


#include <tolmfc_util.inl>

#endif /* _INC_TOLMFC_UTIL_H */
