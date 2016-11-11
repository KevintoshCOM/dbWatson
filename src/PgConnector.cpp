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

#include <sstream>
#include <codecvt>
#include <libpq-fe.h>

#include "DbConnector.h"

PgConnector::~PgConnector()
{
  PQfinish( this->m_cnt );
};

bool
PgConnector::initDbConnection()
{
  std::wstring cntStr = buildCntStr();

  //libpq won't take wstring
  using cvt_type = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<cvt_type, wchar_t> cvt;

  std::string cntStrAnsi = cvt.to_bytes( cntStr );
  
  this->m_cnt = PQconnectdb( cntStrAnsi.c_str() );
  
  return PQstatus( this->m_cnt ) == CONNECTION_OK;
};

std::list<DbTableDesc>
PgConnector::queryTableDesc()
{
  std::list<DbTableDesc> tbls;
  
  return tbls;
};

std::wstring
PgConnector::buildCntStr()
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
