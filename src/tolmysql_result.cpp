
#include "stdafx.h"
#include <tolmysql_result.h>


TOLNS_MYSQL_BEGIN


//////////////////////////////////////////////////////////////////////////
//
// class MySQLResult
//

MySQLResult::MySQLResult()
	: m_pConn(NULL)
	, m_pRes(NULL)
{
}

//MySQLResult::MySQLResult(const MySQLResult&)
//{
//}

MySQLResult::MySQLResult(const MySQLConnect* pConn, PMYSQL_RES pRes)
	: m_pConn(pConn)
	, m_pRes(pRes)
{
}

MySQLResult::~MySQLResult()
{
	FreeResult();
}

//void MySQLResult::operator=(const MySQLResult&)
//{
//}


TOLNS_MYSQL_END
