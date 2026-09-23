
#ifndef _INC_TOLC_VERSION_H
#define _INC_TOLC_VERSION_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


#define VERSION_SUCCESS                      (0)
#define VERSION_NO_VERSION_INFO             (-1)
#define VERSION_INSUFFICENT_MEMORY          (-2)
#define VERSION_UNABLE_EXTRACT_VERSION_INFO (-3)
#define VERSION_NO_SECTION                  (-4)

TOLCDECL int TOLAPI Version_GetSectionData(PCChar pcPath, PCChar pcSection, PChar pcData);
TOLCDECL int TOLAPI Version_GetFixedFileInfo(PCChar pcPath, VS_FIXEDFILEINFO* const pInfo);
TOLCDECL int TOLAPI Version_GetLangCodepage(PCChar pcPath, PDword pdwLangCodepage);


TOLEXTC_END

#endif /* _INC_TOLC_VERSION_H */
