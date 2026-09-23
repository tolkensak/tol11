
#ifndef _INC_TOLXML_H
#define _INC_TOLXML_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tol.h>


#ifdef TOLXMLDLL
	#ifdef _TOLXML_INDEV
		#define TOLXMLDECL        TOLEXP
	#else
		#define TOLXMLDECL        TOLIMP
	#endif
#else
	#define TOLXMLDECL
#endif


#ifdef __cplusplus
	#define TOLNS_XML          ::tol::xml
	#define TOLNS_XML_BEGIN    namespace tol { namespace xml {
	#define TOLNS_XML_END      }}
	#define TOLNS_XML_USING    using namespace tol::xml;
#else
	#define TOLNS_XML
	#define TOLNS_XML_BEGIN
	#define TOLNS_XML_END
	#define TOLNS_XML_USING
#endif


//#include <msxml2.h>
#import "msxml4.dll"


#ifndef _TOLXML_INDEV
	#ifdef TOLXMLDLL
		#ifdef _DEBUG
			#pragma comment(lib, "tolxml11d.lib")
		#else
			#pragma comment(lib, "tolxml11.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "libtolxml11d.lib")
		#else
			#pragma comment(lib, "libtolxml11.lib")
		#endif
	#endif
#endif


#endif // _INC_TOLXML_H
