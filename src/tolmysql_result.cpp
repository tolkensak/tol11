
#include "stdafx.h"
#include <tolmysql_result.h>


TOLNS_MYSQL_BEGIN


//////////////////////////////////////////////////////////////////////////
//
// class MySQLRes
//

MySQLRes::MySQLRes()
	: m_pConn(NULL)
	, m_pRes(NULL)
{
}

//MySQLRes::MySQLRes(const MySQLRes&)
//{
//}

MySQLRes::MySQLRes(const MySQLConn* pConn, PMYSQL_RES pRes)
	: m_pConn(pConn)
	, m_pRes(pRes)
{
}

MySQLRes::~MySQLRes()
{
	FreeResult();
}

//void MySQLRes::operator=(const MySQLRes&)
//{
//}


TOLNS_MYSQL_END
