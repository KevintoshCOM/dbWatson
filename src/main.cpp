/*
dbWatson
Database Structur Exporter
https://github.com/Zer0Knowdlege/dbWatson

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

#include <iostream>
#include <memory>
#include <locale>
#include <codecvt>

#include "INIReader.h"
#include "DbConnector.h"
#include "common.h"

std::map<std::wstring, DbType> dbTypeMapping {
  { L"postgres", DbType::postgres }
};

int main(
    int argc,
    char* argv[] )
{
    INIReader reader( "dbWatson.ini" );
    
    if ( reader.ParseError() < 0 )
    {
        std::cout << "Can't load dbWatson.ini from project dir! \n";
        return 1;
    }

    std::wstring dbType = string_towstring( reader.Get( "default", "dbType", "UNKNOWN" ) );
    DbType dbt;

    try
    {
      dbt = dbTypeMapping.at( dbType );
    }
    catch( const std::out_of_range& )
    {
        std::cout << "Database type not supported! \n";
        return 1;
    }

    DbData dbd = {
      string_towstring( reader.Get( "default", "dbServer", "UNKNOWN" ) ),
      reader.GetInteger( "default", "dbPort", -1 ),
      string_towstring( reader.Get( "default", "dbName", "UNKNOWN" ) ),
      string_towstring( reader.Get( "default", "dbUsr", "UNKNOWN" ) ),
      string_towstring( reader.Get( "default", "dbPasswd", "UNKNOWN" ) )
    };
    
    std::unique_ptr<DbConnector> dbC;
    
    switch( dbt ) 
    {
    case DbType::postgres:
      dbC = std::make_unique<PgConnector>( dbd );
      break;
    default:
      std::cout << "Implementation Error!";
      return 1;
    };
    
    bool cntScs = dbC.get()->initDbConnection();

    std::list<DbTableDesc> tbls;
    if ( cntScs ) 
    {
      tbls = dbC.get()->queryTableDesc();
    }
    
    std::wcout << std::endl << L"Test-Ausgabe:" << std::endl;
    
    for ( DbTableDesc tbl : tbls  )
    {
      std::wcout << tbl.tblName << L'(' <<  tbl.tblType << L')' << std::endl;

      for ( DbColDesc cl : tbl.tblCols  )
      {
	 std::wcout << cl.colName
		    << L" - " << cl.colType
	            << L'(' <<  cl.colLength << L')'
	            << L" Default: " << cl.colDefaultVal
	            << L" Nullable: " << cl.colNullable
		    << std::endl;
      }	
    }

    return 0;
}
