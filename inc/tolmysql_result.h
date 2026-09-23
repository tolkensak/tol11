
#ifndef _INC_TOLMYSQL_RESULT_H
#define _INC_TOLMYSQL_RESULT_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmysql.h>


TOLNS_MYSQL_BEGIN


class MySQLConn;

//////////////////////////////////////////////////////////////////////////
//
// class MySQLRes
//

class TOLMYSQLDECL MySQLRes : public SmartObject
{
//private:
//	MySQLRes(const MySQLRes&);
//	void operator=(const MySQLRes&);

public:
	MySQLRes();
	MySQLRes(const MySQLConn* pConn, PMYSQL_RES pRes);
	virtual ~MySQLRes();

	operator bool() const;
	operator PMYSQL_RES();

	void DataSeek(Uhuge uhOffset);
	PMYSQL_FIELD FetchField();
	PMYSQL_FIELD FetchFieldDirect(Uint uField);
	PMYSQL_FIELD FetchFields();
	PUlong FetchLengths();
	MYSQL_ROW FetchRow();
	Uint FieldSeek(Uint uOffset);
	Uint FieldTell();
	void FreeResult();
	Uint NumFields();
	Uhuge NumRows();
	MYSQL_ROW_OFFSET RowSeek(MYSQL_ROW_OFFSET offset);
	MYSQL_ROW_OFFSET RowTell();

protected:
	const MySQLConn* m_pConn;
	PMYSQL_RES m_pRes;
};

typedef SmartPointer<MySQLRes> MySQLResPtr;


TOLNS_MYSQL_END

#include <tolmysql_result.inl>

#endif /* _INC_TOLMYSQL_RESULT_H */
