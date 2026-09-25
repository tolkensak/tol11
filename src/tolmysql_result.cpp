
#include "stdafx.h"
#include <tolmysql_result.h>


TOLNS_MYSQL_BEGIN


//////////////////////////////////////////////////////////////////////////
//
// class Result
//

Result::Result()
	: m_pConn(NULL)
	, m_pRes(NULL)
{
}

//Result::Result(const Result&)
//{
//}

Result::Result(const Connection* pConn, PMYSQL_RES pRes)
	: m_pConn(pConn)
	, m_pRes(pRes)
{
}

Result::~Result()
{
	FreeResult();
}

//void Result::operator=(const Result&)
//{
//}


TOLNS_MYSQL_END
