
#ifndef _INC_TOLMYSQL_MYSQL_INL
#define _INC_TOLMYSQL_MYSQL_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


TOLNS_MYSQL_BEGIN


//////////////////////////////////////////////////////////////////////////
//
// class MySQL
//

TOLINL
PCCharA MySQL::GetClientInfo()
{
	return mysql_get_client_info();
}

TOLINL
Ulong MySQL::GetClientVersion()
{
	return mysql_get_client_version();
}

TOLINL
Ulong MySQL::HexString(PCharA pcDest, PCCharA pcSrc, Ulong ulLength)
{
	return  mysql_hex_string(pcDest, pcSrc, ulLength);
}

TOLINL
void MySQL::LibraryEnd()
{
	mysql_library_end();
}

TOLINL
int MySQL::LibraryInit(int nArgc, PCharA *ppcArgv, PCharA *ppcGroups)
{
	return mysql_library_init(nArgc, ppcArgv, ppcGroups);
}


TOLNS_MYSQL_END

#endif // _INC_TOLMYSQL_MYSQL_INL
