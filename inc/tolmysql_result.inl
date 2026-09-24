
#ifndef _INC_TOLMYSQL_RESULT_INL
#define _INC_TOLMYSQL_RESULT_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


TOLNS_MYSQL_BEGIN


#define _TOLMYSQL_RESULT_THROW_EXCEPTION //if(!m_pRes) throw ArgException();



//////////////////////////////////////////////////////////////////////////
//
// class MySQLResult
//

TOLINL
MySQLResult::operator bool() const
{
	return m_pRes!=NULL;
}

TOLINL
MySQLResult::operator PMYSQL_RES()
{
	return m_pRes;
}

TOLINL
void MySQLResult::DataSeek(Uhuge uhOffset)
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	mysql_data_seek(m_pRes, uhOffset);
}

TOLINL
PMYSQL_FIELD MySQLResult::FetchField()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_field(m_pRes);
}

TOLINL
PMYSQL_FIELD MySQLResult::FetchFieldDirect(Uint uField)
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_field_direct(m_pRes, uField);
}

TOLINL
PMYSQL_FIELD MySQLResult::FetchFields()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_fields(m_pRes);
}

TOLINL
PUlong MySQLResult::FetchLengths()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_lengths(m_pRes);
}

TOLINL
MYSQL_ROW MySQLResult::FetchRow()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_row(m_pRes);
}

TOLINL
Uint MySQLResult::FieldSeek(Uint uOffset)
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_field_seek(m_pRes, uOffset);
}

TOLINL
Uint MySQLResult::FieldTell()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_field_tell(m_pRes);
}


TOLINL
void MySQLResult::FreeResult()
{
	if(m_pRes)
	{
		mysql_free_result(m_pRes);
		m_pRes=NULL;
	}
}

TOLINL
Uint MySQLResult::NumFields()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_num_fields(m_pRes);
}

TOLINL
Uhuge MySQLResult::NumRows()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_num_rows(m_pRes);
}

TOLINL
MYSQL_ROW_OFFSET MySQLResult::RowSeek(MYSQL_ROW_OFFSET offset)
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_row_seek(m_pRes, offset);
}

TOLINL
MYSQL_ROW_OFFSET MySQLResult::RowTell()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_row_tell(m_pRes);
}


TOLNS_MYSQL_END

#endif // _INC_TOLMYSQL_RESULT_INL
