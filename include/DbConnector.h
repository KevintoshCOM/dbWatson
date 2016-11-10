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

#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <list>
#include <string>

struct DbData {
  const wchar_t* dbServer;
  const wchar_t* dbName;
  const wchar_t* dbUsr;
  const wchar_t* dbPwd;
};

struct DbColDesc {
  const wchar_t* colName;
  const wchar_t* colType;
  const wchar_t* colLength;
};

struct DbTableDesc {
  const wchar_t* tblName;
  std::list<DbColDesc> tblCols;
};

class DbConnector {
 public:
  explicit DbConnector( DbData dbData ) :dbData{dbData} {};
  virtual bool initDbConnection() = 0;
  virtual std::list<DbTableDesc> queryTableDesc() = 0;
 protected:
  DbData dbData;
  
  virtual std::string buildCntStr() = 0;
};

class PgConnector : public DbConnector {
  using DbConnector::DbConnector;
  
 public:
  bool initDbConnection() override;
  std::list<DbTableDesc> queryTableDesc() override;
 protected:
  std::string buildCntStr() override;
};

#endif //DBCONNECTOR_H
