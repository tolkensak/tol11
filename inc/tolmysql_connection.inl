
#ifndef _INC_TOLMYSQL_CONNECTION_INL
#define _INC_TOLMYSQL_CONNECTION_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


TOLNS_MYSQL_BEGIN


#define _TOLMYSQL_CONNECTION_THROW_EXCEPTION if(!m_pMySQL) throw ArgException();


//////////////////////////////////////////////////////////////////////////
//
// class Connection
//

TOLINL
Connection::operator bool() _CONST_
{
	return m_pMySQL!=NULL;
}

TOLINL
Connection::operator PMYSQL()
{
	return m_pMySQL;
}

TOLINL
Uhuge Connection::AffectedRows() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_affected_rows(m_pMySQL);
}

TOLINL
int Connection::AutoCommit(Bool bMode) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_autocommit(m_pMySQL, bMode);
}

TOLINL
int Connection::ChangeUser(PCCharA pcUser, PCCharA pcPassword, PCCharA pcDB)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_change_user(m_pMySQL, pcUser, pcPassword, pcDB);
}

TOLINL
PCCharA Connection::CharacterSetName()
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_character_set_name(m_pMySQL);
}

TOLINL
void Connection::Close()
{
	if(m_pMySQL)
	{
		mysql_close(m_pMySQL);
		m_pMySQL=NULL;
	}
}

TOLINL
int Connection::Commit() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_commit(m_pMySQL);
}

//TOLINL
//int Connection::CreateDB(PCCharA pcDB)
//{
//	_ASSERTE(m_pMySQL);
//
//	_TOLMYSQL_CONNECTION_THROW_EXCEPTION
//
//	return mysql_create_db(m_pMySQL, pcDB);
//}
//
//TOLINL
//int Connection::DropDB(PCCharA pcDB)
//{
//	_ASSERTE(m_pMySQL);
//
//	_TOLMYSQL_CONNECTION_THROW_EXCEPTION
//
//	return mysql_drop_db(m_pMySQL, pcDB);
//}

TOLINL
Uint Connection::Errno() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_errno(m_pMySQL);
}

TOLINL
PCCharA Connection::Error() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_error(m_pMySQL);
}

TOLINL
Uint Connection::FieldCount() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_field_count(m_pMySQL);
}

TOLINL
void Connection::GetCharacterSetInfo(PMY_CHARSET_INFO pInfo) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	mysql_get_character_set_info(m_pMySQL, pInfo);
}

TOLINL
PCCharA Connection::GetHostInfo() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_get_host_info(m_pMySQL);
}

TOLINL
Uint Connection::GetProtoInfo() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_get_proto_info(m_pMySQL);
}

TOLINL
PCCharA Connection::GetServerInfo() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_get_server_info(m_pMySQL);
}

TOLINL
Ulong Connection::GetServerVersion() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_get_server_version(m_pMySQL);
}

TOLINL
PCCharA Connection::GetSSLCipher() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_get_ssl_cipher(m_pMySQL);
}

TOLINL
PCCharA Connection::Info() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_info(m_pMySQL);
}

TOLINL
Bool Connection::Init()
{
	m_pMySQL=mysql_init(m_pMySQL);
	return m_pMySQL!=NULL;
}

TOLINL
Uhuge Connection::InsertId() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_insert_id(m_pMySQL);
}

TOLINL
ResultPtr Connection::ListDB(PCCharA pcWild) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return new Result(this, mysql_list_dbs(m_pMySQL, pcWild));
}

TOLINL
ResultPtr Connection::ListFields(PCCharA pcTable, PCCharA pcWild) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return new Result(this, mysql_list_fields(m_pMySQL, pcTable, pcWild));
}

TOLINL
ResultPtr Connection::ListProcesses() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return new Result(this, mysql_list_processes(m_pMySQL));
}

TOLINL
ResultPtr Connection::ListTables(PCCharA pcWild) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return new Result(this, mysql_list_tables(m_pMySQL, pcWild));
}

TOLINL
Bool Connection::MoreResults() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_more_results(m_pMySQL);
}

TOLINL
int Connection::NextResult() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_next_result(m_pMySQL);
}

TOLINL
int Connection::Options(enum mysql_option option, PCVoid pArg)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_options(m_pMySQL, option, pArg);
}

TOLINL
int Connection::Ping() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_ping(m_pMySQL);
}

TOLINL
int Connection::Query(PCCharA pcStmt) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_query(m_pMySQL, pcStmt);
}

TOLINL
Bool Connection::RealConnect(PCCharA pcHost, PCCharA pcUser, PCCharA pcPassword, PCCharA pcDB, Uint uPort, PCCharA pcUnixSocket, Ulong ulClientFlags)
{
	m_pMySQL=mysql_real_connect(m_pMySQL, pcHost, pcUser, pcPassword, pcDB, uPort, pcUnixSocket, ulClientFlags);
	return m_pMySQL!=NULL;
}

TOLINL
Ulong Connection::RealEscapeString(PCharA pcDest, PCCharA pcSrc, Ulong ulLength) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_real_escape_string(m_pMySQL, pcDest, pcSrc, ulLength);
}

TOLINL
int Connection::RealQuery(StringA strStmt) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_real_query(m_pMySQL, strStmt, strStmt.Len());
}

TOLINL
int Connection::RealQuery(PCCharA pcStmt, Ulong ulLength) _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_real_query(m_pMySQL, pcStmt, ulLength);
}

TOLINL
int Connection::Refresh(Uint uOptions)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_refresh(m_pMySQL, uOptions);
}

TOLINL
Bool Connection::Rollback() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_rollback(m_pMySQL);
}

TOLINL
int Connection::SelectDB(PCCharA pcDB)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_select_db(m_pMySQL, pcDB);
}

TOLINL
int Connection::SetCharacterSet(PCCharA pcCS)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_set_character_set(m_pMySQL, pcCS);
}

TOLINL
int Connection::SetServerOption(enum enum_mysql_set_option option)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_set_server_option(m_pMySQL, option);
}

TOLINL
int Connection::SetShutdown(enum mysql_enum_shutdown_level level)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_shutdown(m_pMySQL, level);
}

TOLINL
PCCharA Connection::SQLState() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_sqlstate(m_pMySQL);
}

TOLINL
void Connection::SSLSet(PCCharA pcKey, PCCharA pcCert, PCCharA pcCa, PCCharA pcCapath, PCCharA pcCipher)
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	mysql_ssl_set(m_pMySQL, pcKey, pcCert, pcCa, pcCapath, pcCipher);
}

TOLINL
PCCharA Connection::Stat() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_stat(m_pMySQL);
}

TOLINL
ResultPtr Connection::StoreResult() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return new Result(this, mysql_store_result(m_pMySQL));
}

TOLINL
Ulong Connection::ThreadId() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_thread_id(m_pMySQL);
}

TOLINL
ResultPtr Connection::UseResult() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return new Result(this, mysql_use_result(m_pMySQL));
}

TOLINL
Uint Connection::WarningCount() _CONST_
{
	_ASSERTE(m_pMySQL);

	_TOLMYSQL_CONNECTION_THROW_EXCEPTION

	return mysql_warning_count(m_pMySQL);
}


TOLNS_MYSQL_END

#endif // _INC_TOLMYSQL_CONNECTION_INL
