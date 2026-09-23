
#ifndef _INC_TOLMYSQL_CONNECT_INL
#define _INC_TOLMYSQL_CONNECT_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


TOLNS_MYSQL_BEGIN


#define _TOLMYSQL_CONNECT_THROW_EXCEPTION //if(!m_pMySQL) throw ArgException();


//////////////////////////////////////////////////////////////////////////
//
// class MySQLConn
//

TOLINL
MySQLConn::operator bool() _CONST_
{
	return m_pMySQL!=NULL;
}

TOLINL
MySQLConn::operator PMYSQL()
{
	return m_pMySQL;
}

TOLINL
Uhuge MySQLConn::AffectedRows() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_affected_rows(m_pMySQL);
}

TOLINL
int MySQLConn::AutoCommit(Bool bMode) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_autocommit(m_pMySQL, bMode);
}

TOLINL
int MySQLConn::ChangeUser(PCCharA pcUser, PCCharA pcPassword, PCCharA pcDB)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_change_user(m_pMySQL, pcUser, pcPassword, pcDB);
}

TOLINL
PCCharA MySQLConn::CharacterSetName()
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_character_set_name(m_pMySQL);
}

TOLINL
void MySQLConn::Close()
{
	if(m_pMySQL)
	{
		mysql_close(m_pMySQL);
		m_pMySQL=NULL;
	}
}

TOLINL
int MySQLConn::Commit() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_commit(m_pMySQL);
}

//TOLINL
//int MySQLConn::CreateDB(PCCharA pcDB)
//{
//	_ASSERTE(m_pMySQL);
//
//	_TOLMYSQL_CONNECT_THROW_EXCEPTION
//
//	return mysql_create_db(m_pMySQL, pcDB);
//}
//
//TOLINL
//int MySQLConn::DropDB(PCCharA pcDB)
//{
//	_ASSERTE(m_pMySQL);
//
//	_TOLMYSQL_CONNECT_THROW_EXCEPTION
//
//	return mysql_drop_db(m_pMySQL, pcDB);
//}

TOLINL
Uint MySQLConn::Errno() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_errno(m_pMySQL);
}

TOLINL
PCCharA MySQLConn::Error() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_error(m_pMySQL);
}

TOLINL
Uint MySQLConn::FieldCount() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_field_count(m_pMySQL);
}

TOLINL
void MySQLConn::GetCharacterSetInfo(PMY_CHARSET_INFO pInfo) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	mysql_get_character_set_info(m_pMySQL, pInfo);
}

TOLINL
PCCharA MySQLConn::GetHostInfo() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_get_host_info(m_pMySQL);
}

TOLINL
Uint MySQLConn::GetProtoInfo() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_get_proto_info(m_pMySQL);
}

TOLINL
PCCharA MySQLConn::GetServerInfo() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_get_server_info(m_pMySQL);
}

TOLINL
Ulong MySQLConn::GetServerVersion() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_get_server_version(m_pMySQL);
}

TOLINL
PCCharA MySQLConn::GetSSLCipher() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_get_ssl_cipher(m_pMySQL);
}

TOLINL
PCCharA MySQLConn::Info() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_info(m_pMySQL);
}

TOLINL
Bool MySQLConn::Init()
{
	m_pMySQL=mysql_init(m_pMySQL);
	return m_pMySQL!=NULL;
}

TOLINL
Uhuge MySQLConn::InsertId() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_insert_id(m_pMySQL);
}

TOLINL
MySQLResPtr MySQLConn::ListDB(PCCharA pcWild) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return new MySQLRes(this, mysql_list_dbs(m_pMySQL, pcWild));
}

TOLINL
MySQLResPtr MySQLConn::ListFields(PCCharA pcTable, PCCharA pcWild) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return new MySQLRes(this, mysql_list_fields(m_pMySQL, pcTable, pcWild));
}

TOLINL
MySQLResPtr MySQLConn::ListProcesses() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return new MySQLRes(this, mysql_list_processes(m_pMySQL));
}

TOLINL
MySQLResPtr MySQLConn::ListTables(PCCharA pcWild) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return new MySQLRes(this, mysql_list_tables(m_pMySQL, pcWild));
}

TOLINL
Bool MySQLConn::MoreResults() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_more_results(m_pMySQL);
}

TOLINL
int MySQLConn::NextResult() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_next_result(m_pMySQL);
}

TOLINL
int MySQLConn::Options(enum mysql_option option, PCVoid pArg)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_options(m_pMySQL, option, pArg);
}

TOLINL
int MySQLConn::Ping() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_ping(m_pMySQL);
}

TOLINL
int MySQLConn::Query(PCCharA pcStmt) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_query(m_pMySQL, pcStmt);
}

TOLINL
Bool MySQLConn::RealConnect(PCCharA pcHost, PCCharA pcUser, PCCharA pcPassword, PCCharA pcDB, Uint uPort, PCCharA pcUnixSocket, Ulong ulClientFlags)
{
	m_pMySQL=mysql_real_connect(m_pMySQL, pcHost, pcUser, pcPassword, pcDB, uPort, pcUnixSocket, ulClientFlags);
	return m_pMySQL!=NULL;
}

TOLINL
Ulong MySQLConn::RealEscapeString(PCharA pcDest, PCCharA pcSrc, Ulong ulLength) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_real_escape_string(m_pMySQL, pcDest, pcSrc, ulLength);
}

TOLINL
int MySQLConn::RealQuery(PCCharA pcStmt, Ulong ulLength) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_real_query(m_pMySQL, pcStmt, ulLength);
}

TOLINL
int MySQLConn::Refresh(Uint uOptions)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_refresh(m_pMySQL, uOptions);
}

TOLINL
Bool MySQLConn::Rollback() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_rollback(m_pMySQL);
}

TOLINL
int MySQLConn::SelectDB(PCCharA pcDB)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_select_db(m_pMySQL, pcDB);
}

TOLINL
int MySQLConn::SetCharacterSet(PCCharA pcCS)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_set_character_set(m_pMySQL, pcCS);
}

TOLINL
int MySQLConn::SetServerOption(enum enum_mysql_set_option option)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_set_server_option(m_pMySQL, option);
}

TOLINL
int MySQLConn::SetShutdown(enum mysql_enum_shutdown_level level)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_shutdown(m_pMySQL, level);
}

TOLINL
PCCharA MySQLConn::SQLState() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_sqlstate(m_pMySQL);
}

TOLINL
void MySQLConn::SSLSet(PCCharA pcKey, PCCharA pcCert, PCCharA pcCa, PCCharA pcCapath, PCCharA pcCipher)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	mysql_ssl_set(m_pMySQL, pcKey, pcCert, pcCa, pcCapath, pcCipher);
}

TOLINL
PCCharA MySQLConn::Stat() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_stat(m_pMySQL);
}

TOLINL
MySQLResPtr MySQLConn::StoreResult() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return new MySQLRes(this, mysql_store_result(m_pMySQL));
}

TOLINL
Ulong MySQLConn::ThreadId() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_thread_id(m_pMySQL);
}

TOLINL
MySQLResPtr MySQLConn::UseResult() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return new MySQLRes(this, mysql_use_result(m_pMySQL));
}

TOLINL
Uint MySQLConn::WarningCount() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECT_THROW_EXCEPTION

	return mysql_warning_count(m_pMySQL);
}


TOLNS_MYSQL_END

#endif // _INC_TOLMYSQL_CONNECT_INL
