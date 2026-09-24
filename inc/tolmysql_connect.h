
#ifndef _INC_TOLMYSQL_CONNECT_H
#define _INC_TOLMYSQL_CONNECT_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmysql.h>
#include <tolmysql_result.h>


TOLNS_MYSQL_BEGIN

#define _CONST_ const

//////////////////////////////////////////////////////////////////////////
//
// class MySQLConnect
//

class TOLMYSQLDECL MySQLConnect : public SmartObject
{
//private:
//	MySQLConnect(const MySQLConnect&);
//	void operator=(const MySQLConnect&);

public:
	MySQLConnect();
	virtual ~MySQLConnect();

	operator bool() _CONST_;
	operator PMYSQL();

	Uhuge AffectedRows() _CONST_;
	int AutoCommit(Bool bMode=TRUE) _CONST_;
	int ChangeUser(PCCharA pcUser, PCCharA pcPassword, PCCharA pcDB);
	PCCharA CharacterSetName();
	void Close();
	int Commit() _CONST_;
	//int CreateDB(PCCharA pcDB);
	//int DropDB(PCCharA pcDB);
	Uint Errno() _CONST_;
	PCCharA Error() _CONST_;
	Uint FieldCount() _CONST_;
	void GetCharacterSetInfo(PMY_CHARSET_INFO pInfo) _CONST_;
	PCCharA GetHostInfo() _CONST_;
	Uint GetProtoInfo() _CONST_;
	PCCharA GetServerInfo() _CONST_;
	Ulong GetServerVersion() _CONST_;
	PCCharA GetSSLCipher() _CONST_;
	PCCharA Info() _CONST_;
	Bool Init();
	Uhuge InsertId() _CONST_;
	MySQLResultPtr ListDB(PCCharA pcWild) _CONST_;
	MySQLResultPtr ListFields(PCCharA pcTable, PCCharA pcWild) _CONST_;
	MySQLResultPtr ListProcesses() _CONST_;
	MySQLResultPtr ListTables(PCCharA pcWild) _CONST_;
	Bool MoreResults() _CONST_;
	int NextResult() _CONST_;
	int Options(enum mysql_option option, PCVoid pArg);
	int Ping() _CONST_;
	int Query(PCCharA pcStmt) _CONST_;
	Bool RealConnect(PCCharA pcHost, PCCharA pcUser, PCCharA pcPassword, PCCharA pcDB, Uint uPort=0, PCCharA pcUnixSocket=NULL, Ulong ulClientFlags=0);
	Ulong RealEscapeString(PCharA pcDest, PCCharA pcSrc, Ulong ulLength) _CONST_;
	int RealQuery(PCCharA pcStmt, Ulong ulLength) _CONST_;
	int Refresh(Uint uOptions);
	Bool Rollback() _CONST_;
	int SelectDB(PCCharA pcDB);
	int SetCharacterSet(PCCharA pcCS);
	int SetServerOption(enum enum_mysql_set_option option);
	int SetShutdown(enum mysql_enum_shutdown_level level);
	PCCharA SQLState() _CONST_;
	void SSLSet(PCCharA pcKey, PCCharA pcCert, PCCharA pcCa, PCCharA pcCapath, PCCharA pcCipher);
	PCCharA Stat() _CONST_;
	MySQLResultPtr StoreResult() _CONST_;
	Ulong ThreadId() _CONST_;
	MySQLResultPtr UseResult() _CONST_;
	Uint WarningCount() _CONST_;

protected:
	PMYSQL m_pMySQL;
};

#ifndef _TOL_MYSQLCONNECTPTR
typedef SmartPointer<MySQLConnect> MySQLConnectPtr;
#endif


TOLNS_MYSQL_END

#include <tolmysql_connect.inl>

#endif /* _INC_TOLMYSQL_CONNECT_H */
