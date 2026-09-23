
#include "stdafx.h"
#include <tolmysql_connect.h>


TOLNS_MYSQL_BEGIN


//////////////////////////////////////////////////////////////////////////
//
// class MySQLConn
//

MySQLConn::MySQLConn()
	: m_pMySQL(NULL)
{
}

//MySQLConn::MySQLConn(const MySQLConn&)
//{
//}

MySQLConn::~MySQLConn()
{
	Close();
}

//void MySQLConn::operator=(const MySQLConn&)
//{
//}


TOLNS_MYSQL_END
