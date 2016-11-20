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

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <vector>

#include "XMLExporter.h"
#include "common.h"
#include "DbConnector.h"

void
XMLExporter::exportToFS()
{  
  int rc;
  xmlTextWriterPtr writer;
  xmlChar *tmp;
  std::vector<xmlChar> encBuffer;
  
	for ( DbTableDesc tbl : this->m_tbls  )
    {
		
		writer = xmlNewTextWriterFilename( wstring_tostring( tbl.tblName ).c_str(), 0 );
		if ( writer == NULL ) {
			std::cerr << "testXmlwriterFilename: Error creating the xml writer\n";
			
			return;
		}
		
		rc = xmlTextWriterStartDocument( writer, NULL, "UTF-8", NULL );
		if ( rc < 0 ) {
			std::cerr << "testXmlwriterFilename: Error at xmlTextWriterStartDocument\n";
			
			return;
		}
		
		rc = xmlTextWriterStartElement( writer, BAD_CAST "table" );
		if ( rc < 0 ) {
			std::cerr << "testXmlwriterFilename: Error at xmlTextWriterStartElement\n";
			
			return;
		}
		
		wstring_toxmlChar( tbl.tblName, encBuffer );
		rc = xmlTextWriterWriteElement( writer, BAD_CAST "tablename", &encBuffer[0] );
		if ( rc < 0 ) {
			std::cerr << "testXmlwriterFilename: Error at xmlTextWriterWriteElement\n";
			
			return;
		}
		
		wstring_toxmlChar( tbl.tblType, encBuffer );
		rc = xmlTextWriterWriteElement( writer, BAD_CAST "tabletyp", &encBuffer[0] );
		if ( rc < 0 ) {
			std::cerr << "testXmlwriterFilename: Error at xmlTextWriterWriteElement\n";
			
			return;
		}
		
		rc = xmlTextWriterEndDocument( writer );
		if ( rc < 0 ) {
			std::cerr << "testXmlwriterFilename: Error at xmlTextWriterEndDocument\n";
			
			return;
		}

		xmlFreeTextWriter(writer);

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
}
