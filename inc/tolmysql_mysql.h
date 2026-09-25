
#ifndef _INC_TOLMYSQL_MYSQL_H
#define _INC_TOLMYSQL_MYSQL_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmysql.h>
#include <tolmysql_connection.h>


TOLNS_MYSQL_BEGIN


//////////////////////////////////////////////////////////////////////////
//
// class MySQL
//

class TOLMYSQLDECL MySQL : public Object
{
//private:
//	MySQL(const MySQL&);
//	void operator=(const MySQL&);

protected:
	MySQL();

public:
	virtual ~MySQL();

	static PCCharA GetClientInfo();
	static Ulong GetClientVersion();
	static Ulong HexString(PCharA pcDest, PCCharA pcSrc, Ulong ulLength);
	static void LibraryEnd();
	static int LibraryInit(int nArgc=0, PCharA *ppcArgv=NULL, PCharA *ppcGroups=NULL);
};


TOLNS_MYSQL_END

#include <tolmysql_mysql.inl>

#endif /* _INC_TOLMYSQL_MYSQL_H */
