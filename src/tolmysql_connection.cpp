
#include "stdafx.h"
#include <tolmysql_connection.h>


TOLNS_MYSQL_BEGIN


//////////////////////////////////////////////////////////////////////////
//
// class Connection
//

Connection::Connection()
	: m_pMySQL(NULL)
{
}

Connection::Connection(PCCharA pcHost, PCCharA pcUser, PCCharA pcPassword, PCCharA pcDB, Uint uPort, PCCharA pcUnixSocket, Ulong ulClientFlags)
	: m_pMySQL(NULL)
{
	m_pMySQL = mysql_init(m_pMySQL);
	if (m_pMySQL)
	{
		if (!mysql_real_connect(m_pMySQL, pcHost, pcUser, pcPassword, pcDB, uPort, pcUnixSocket, ulClientFlags))
		{
			Close();
			throw RuntimeException();
		}
	}
	else {
		throw RuntimeException();
	}
}

//Connection::Connection(const Connection&)
//{
//}

Connection::~Connection()
{
	Close();
}

//void Connection::operator=(const Connection&)
//{
//}


TOLNS_MYSQL_END
