
#include "stdafx.h"
#include <tolmysql_connect.h>


TOLNS_MYSQL_BEGIN


//////////////////////////////////////////////////////////////////////////
//
// class MySQLConnect
//

MySQLConnect::MySQLConnect()
	: m_pMySQL(NULL)
{
}

//MySQLConnect::MySQLConnect(const MySQLConnect&)
//{
//}

MySQLConnect::~MySQLConnect()
{
	Close();
}

//void MySQLConnect::operator=(const MySQLConnect&)
//{
//}


TOLNS_MYSQL_END
