
#ifndef _INC_TOLXML_UTIL_H
#define _INC_TOLXML_UTIL_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolxml.h>


TOLNS_XML_BEGIN


TOLXMLDECL MSXML2::IXMLDOMDocument2Ptr /*TOLAPI*/ XML_Open(LPCTSTR pcFile, BOOL bValidateOnParse=FALSE);
TOLXMLDECL CString /*TOLAPI*/ XML_Transform(LPCTSTR pcXmlFile, LPCTSTR pcXslFile, LPCTSTR pcHtmlFile, BOOL bCssDirRelateiveToXsl=TRUE);


TOLNS_XML_END

#endif /* _INC_TOLXML_UTIL_H */
