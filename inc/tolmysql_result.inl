
#ifndef _INC_TOLMYSQL_RESULT_INL
#define _INC_TOLMYSQL_RESULT_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


TOLNS_MYSQL_BEGIN


#define _TOLMYSQL_RESULT_THROW_EXCEPTION //if(!m_pRes) throw ArgException();



//////////////////////////////////////////////////////////////////////////
//
// class MySQLRes
//

TOLINL
MySQLRes::operator bool() const
{
	return m_pRes!=NULL;
}

TOLINL
MySQLRes::operator PMYSQL_RES()
{
	return m_pRes;
}

TOLINL
void MySQLRes::DataSeek(Uhuge uhOffset)
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	mysql_data_seek(m_pRes, uhOffset);
}

TOLINL
PMYSQL_FIELD MySQLRes::FetchField()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_field(m_pRes);
}

TOLINL
PMYSQL_FIELD MySQLRes::FetchFieldDirect(Uint uField)
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_field_direct(m_pRes, uField);
}

TOLINL
PMYSQL_FIELD MySQLRes::FetchFields()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_fields(m_pRes);
}

TOLINL
PUlong MySQLRes::FetchLengths()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_lengths(m_pRes);
}

TOLINL
MYSQL_ROW MySQLRes::FetchRow()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_fetch_row(m_pRes);
}

TOLINL
Uint MySQLRes::FieldSeek(Uint uOffset)
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_field_seek(m_pRes, uOffset);
}

TOLINL
Uint MySQLRes::FieldTell()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_field_tell(m_pRes);
}


TOLINL
void MySQLRes::FreeResult()
{
	if(m_pRes)
	{
		mysql_free_result(m_pRes);
		m_pRes=NULL;
	}
}

TOLINL
Uint MySQLRes::NumFields()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_num_fields(m_pRes);
}

TOLINL
Uhuge MySQLRes::NumRows()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_num_rows(m_pRes);
}

TOLINL
MYSQL_ROW_OFFSET MySQLRes::RowSeek(MYSQL_ROW_OFFSET offset)
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_row_seek(m_pRes, offset);
}

TOLINL
MYSQL_ROW_OFFSET MySQLRes::RowTell()
{
	_ASSERTE(m_pRes);

	_TOLMYSQL_RESULT_THROW_EXCEPTION

	return mysql_row_tell(m_pRes);
}


TOLNS_MYSQL_END

#endif // _INC_TOLMYSQL_RESULT_INL
