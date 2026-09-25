
#ifndef _INC_TOLMYSQL_RESULT_H
#define _INC_TOLMYSQL_RESULT_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolmysql.h>


TOLNS_MYSQL_BEGIN


class Connection;

//////////////////////////////////////////////////////////////////////////
//
// class Result
//

class TOLMYSQLDECL Result : public SmartObject
{
//private:
//	Result(const Result&);
//	void operator=(const Result&);

public:
	Result();
	Result(const Connection* pConn, PMYSQL_RES pRes);
	virtual ~Result();

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
	const Connection* m_pConn;
	PMYSQL_RES m_pRes;
};

typedef SmartPointer<Result> ResultPtr;


TOLNS_MYSQL_END

#include <tolmysql_result.inl>

#endif /* _INC_TOLMYSQL_RESULT_H */
