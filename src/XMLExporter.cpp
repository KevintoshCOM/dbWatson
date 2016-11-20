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
#include <string>
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
  std::vector<xmlChar> encBuffer;
  
	for ( DbTableDesc tbl : this->m_tbls  )
    {

        std::string uri = wstring_tostring( tbl.tblName );
		uri.append( ".xml" );
				       
		writer = xmlNewTextWriterFilename( uri.c_str(), 0 );
		if ( writer == NULL ) {
			std::cerr << "xmlWriter: Error creating the xml writer\n";
			
			return;
		}
		
		rc = xmlTextWriterStartDocument( writer, NULL, "UTF-8", NULL );
		if ( rc < 0 ) {
			std::cerr << "xmlWriter: Error at xmlTextWriterStartDocument\n";
			
			return;
		}
		
		rc = xmlTextWriterStartElement( writer, BAD_CAST "table" );
		if ( rc < 0 ) {
			std::cerr << "xmlWriter: Error at xmlTextWriterStartElement\n";
			
			return;
		}
		
		wstring_toxmlChar( tbl.tblName, encBuffer );
		rc = xmlTextWriterWriteElement( writer, BAD_CAST "tablename", &encBuffer[0] );
		if ( rc < 0 ) {
			std::cerr << "xmlWriter: Error at xmlTextWriterWriteElement\n";
			
			return;
		}
		
		wstring_toxmlChar( tbl.tblType, encBuffer );
		rc = xmlTextWriterWriteElement( writer, BAD_CAST "tabletype", &encBuffer[0] );
		if ( rc < 0 ) {
			std::cerr << "xmlWriter: Error at xmlTextWriterWriteElement\n";
			
			return;
		}
		
		rc = xmlTextWriterEndDocument( writer );
		if ( rc < 0 ) {
			std::cerr << "xmlWriter: Error at xmlTextWriterEndDocument\n";
			
			return;
		}

		rc = xmlTextWriterStartElement( writer, BAD_CAST "columns" );
		if ( rc < 0 ) {
			std::cerr << "xmlWriter: Error at xmlTextWriterStartElement\n";
			return;
		}

		for ( DbColDesc cl : tbl.tblCols  )
		{
			rc = xmlTextWriterStartElement( writer, BAD_CAST "column" );
			if ( rc < 0 ) {
				std::cerr << "xmlWriter: Error at xmlTextWriterStartElement\n";
				return;
			}
			
			wstring_toxmlChar( cl.colName, encBuffer );
			rc = xmlTextWriterWriteElement( writer, BAD_CAST "columnname", &encBuffer[0] );
			if ( rc < 0 ) {
				std::cerr << "xmlWriter: Error at xmlTextWriterWriteElement\n";
				
				return;
			}
			
			wstring_toxmlChar( cl.colType, encBuffer );
			rc = xmlTextWriterWriteElement( writer, BAD_CAST "columntype", &encBuffer[0] );
			if ( rc < 0 ) {
				std::cerr << "xmlWriter: Error at xmlTextWriterWriteElement\n";
				
				return;
			}
			
			wstring_toxmlChar( cl.colLength, encBuffer );
			rc = xmlTextWriterWriteElement( writer, BAD_CAST "columnlength", &encBuffer[0] );
			if ( rc < 0 ) {
				std::cerr << "xmlWriter: Error at xmlTextWriterWriteElement\n";
				
				return;
			}
			
			wstring_toxmlChar( cl.colDefaultVal, encBuffer );
			rc = xmlTextWriterWriteElement( writer, BAD_CAST "columndefaultval", &encBuffer[0] );
			if ( rc < 0 ) {
				std::cerr << "xmlWriter: Error at xmlTextWriterWriteElement\n";
				
				return;
			}
			
			wstring_toxmlChar( cl.colNullable, encBuffer );
			rc = xmlTextWriterWriteElement( writer, BAD_CAST "columnnullable", &encBuffer[0] );
			if ( rc < 0 ) {
				std::cerr << "xmlWriter: Error at xmlTextWriterWriteElement\n";
				
				return;
			}
			
			/* Close Column */
			rc = xmlTextWriterEndElement( writer );
			if ( rc < 0 ) {
				std::cerr << "xmlWriter: Error at xmlTextWriterEndElement\n";
				
				return;
			}
		}

		//**Zer0Knowledge
		//closes all tags 
		xmlFreeTextWriter(writer);
    }
}
