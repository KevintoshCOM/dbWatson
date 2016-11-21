/*
dbWatson
Database Structur Exporter
https://github.com/Zer0Knowledge/dbWatson

BSD 2-Clause License

Copyright (c) 2016  | Kevin Klein, Tobias Donix, Leonard Franke
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of
   conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list
   of conditions and the following disclaimer in the documentation and/or other
   materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include <sstream>
#include <codecvt>
#include <libpq-fe.h>

#include "DbConnector.h"
#include "common.h"

/*
 * Function: ~PgConnector
 * ----------------------------
 * Frees the connection handle
 */
PgConnector::~PgConnector()
{
	PQfinish( this->m_cnt );
};

/*
 * Function: initDbConnection
 * ----------------------------
 * Inits the database connection
 *
 * Returns the connection status
 */
bool
PgConnector::initDbConnection()
{
	std::wstring cntStr = buildCntStr();

	//libpq won't take wstring
	std::string cntStrAnsi = wstring_tostring( cntStr );

	this->m_cnt = PQconnectdb( cntStrAnsi.c_str() );

	return isConnected();
};

/*
 * Function: isConnected
 * ----------------------------
 * Says whether the db is connected
 *
 * Return a boolean
 */
bool
PgConnector::isConnected() const
{
	return PQstatus( this->m_cnt ) == CONNECTION_OK;
}; 

/*
 * Function: queryTableDesc
 * ----------------------------
 * Reads out the tables
 *
 * Return a list of table structs
 */
std::list<DbTableDesc>
PgConnector::queryTableDesc() const
{
	constexpr char sql[] = "SELECT table_name, table_type "
						 "FROM information_schema.tables "
						 "WHERE table_schema NOT IN ("
						 " 'pg_catalog',"
						 " 'information_schema'"
						 ");\0";
  
	std::list<DbTableDesc> tbls;

	if ( isConnected() )
	{
		PGresult* qRes = PQexec( this->m_cnt, sql );

		if ( PQresultStatus( qRes ) == PGRES_TUPLES_OK )
		{
			int rows = PQntuples( qRes );
			int cols = PQnfields( qRes );

			for ( int i = 0; i < rows; ++i )
			{
				if ( cols == 2 )
				{
					std::wstring tblName = char_towstring( PQgetvalue( qRes, i, 0 ) );
					std::wstring tblType = char_towstring( PQgetvalue( qRes, i, 1 ) );

					DbTableDesc tblDesc = { tblName, tblType, queryColDesc( tblName  ) };

					tbls.push_back( tblDesc );
				}
			}  
		}
	}

	return tbls;
};

/*
 * Function: queryColDesc
 * ----------------------------
 * Reads out the columns of a specific table
 *
 * tblName ... the name of the table
 *
 * Return a list of column structs
 */
std::list<DbColDesc>
PgConnector::queryColDesc(
    std::wstring tblName ) const
{
	std::stringstream sstream;
	sstream << "SELECT column_name, udt_name, character_maximum_length, column_default, is_nullable "
			<< "FROM information_schema.columns "
			<< "WHERE table_name   = '"
			<< wstring_tostring(tblName)            
			<< "' "
			<< "ORDER BY ordinal_position DESC;";

	std::list<DbColDesc> cls;

	if ( isConnected() )
	{
		PGresult* qRes = PQexec( this->m_cnt, sstream.str().c_str() );

		if ( PQresultStatus( qRes ) == PGRES_TUPLES_OK )
		{
			int rows = PQntuples( qRes );
			int cols = PQnfields( qRes );

			for ( int i = 0; i < rows; ++i )
			{
				if ( cols == 5 )
				{
					char yes[4] = "YES";

					std::wstring colName = char_towstring( PQgetvalue( qRes, i, 0 ) );
					std::wstring colType = char_towstring( PQgetvalue( qRes, i, 1 ) );
					std::wstring colLength = char_towstring( PQgetvalue( qRes, i, 2 ) );
					std::wstring colDefaultVal = char_towstring( PQgetvalue( qRes, i, 3 ) );
					bool colNullable = strcmp( PQgetvalue( qRes, i, 4 ), yes ) == 0 ? true : false;

					DbColDesc clDesc = { colName, colType, colLength, colDefaultVal, colNullable };

					cls.push_back( clDesc );
				}
			}
		}
	}

	return cls;
};

/*
 * Function: buildCntStr
 * ----------------------------
 * Creates the connection string
 *
 * Return the connection string as wstring
 */
std::wstring
PgConnector::buildCntStr() const
{
	constexpr int timeout = 10;
  
	std::wstringstream sstream;

	sstream << L"host="
			<< this->dbData.dbServer
			<< L" port="
			<< this->dbData.dbPort
			<< L" dbname="
			<< this->dbData.dbName
			<< L" connect_timeout="
			<< timeout
			<< L" user="
			<< this->dbData.dbUsr
			<< " password="
			<< this->dbData.dbPwd;

	return sstream.str();
}; 
