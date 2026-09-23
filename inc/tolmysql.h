
#ifndef _INC_TOLMYSQL_H
#define _INC_TOLMYSQL_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tol.h>


#ifdef TOLMYSQLDLL
	#ifdef _TOLMYSQL_INDEV
		#define TOLMYSQLDECL        TOLEXP
	#else
		#define TOLMYSQLDECL        TOLIMP
	#endif
#else
	#define TOLMYSQLDECL
#endif


#ifdef __cplusplus
	#define TOLNS_MYSQL          ::tol::mysql
	#define TOLNS_MYSQL_BEGIN    namespace tol { namespace mysql {
	#define TOLNS_MYSQL_END      }}
	#define TOLNS_MYSQL_USING    using namespace tol::mysql;
#else
	#define TOLNS_MYSQL
	#define TOLNS_MYSQL_BEGIN
	#define TOLNS_MYSQL_END
	#define TOLNS_MYSQL_USING
#endif


#include <crtdbg.h> // _ASSERTE
#include <tolcpp_exception.h>
#include <tolcpp_pointer.h>


#include <winsock.h>
#include <mysql.h>

#ifndef _LIB
#pragma comment(lib, "libmysql.lib")
#endif

TOLNS_BEGIN
typedef MYSQL           *PMYSQL;
typedef MYSQL_RES       *PMYSQL_RES;
typedef MYSQL_FIELD     *PMYSQL_FIELD;
typedef MYSQL_ROW       *PMYSQL_ROW;
typedef MY_CHARSET_INFO *PMY_CHARSET_INFO;
TOLNS_END


#ifndef _TOLMYSQL_INDEV
	#ifdef TOLMYSQLDLL
		#ifdef _DEBUG
			#pragma comment(lib, "tolmysql11d.lib")
		#else
			#pragma comment(lib, "tolmysql11.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "libtolmysql11d.lib")
		#else
			#pragma comment(lib, "libtolmysql11.lib")
		#endif
	#endif
#endif


#endif // _INC_TOLMYSQL_H
