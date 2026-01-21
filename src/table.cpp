/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * table.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Table.h>

Table_class::Table_class( string title, uint ident ) :
	Logfacility_class("Table_class")
{
	className 	= Logfacility_class::className ;
	opt.Titel	= title;
	opt.Ident	= ident;
};
Table_class::Table_class( tableopt_t _opt  ) :
	Logfacility_class("Table_class")
{
	className 	= Logfacility_class::className ;
	opt			= _opt;
}
Table_class::~Table_class() {};


void Table_class::AddColumn( string name, uint width )
{
	header_v.push_back({name, width} );
}

void Table_class::PrintHeader()
{

	string color 		= GetColor( TABLE, opt.FILE );
	string endcolor 	= GetendColor( opt.FILE );

	*opt.FILE << color << setw( opt.Ident ) << left << setfill('.') << opt.Titel << endcolor ;
	if ( opt.Titel.length() > opt.Ident )
		*opt.FILE << endl;
	for( header_t row : header_v )
	{
		*opt.FILE  << dec << setw(row.width) << left << setfill(' ')<< row.txt << opt.Separator;
	}
	*opt.FILE << '\n';
}
string Table_class::cout_row( string txt )
{
	string color 		= GetColor( TABLE, opt.FILE );
	string endc			= GetendColor( opt.FILE );
	string colorline 	= color + txt + endc + opt.Crlf;

	if( opt.FILE == &cout )
		*opt.FILE << colorline;
	else
		*opt.FILE << txt << endl;

	return colorline;
}
void Table_class::TestTable()
{
	AddColumn( "Index", 8);
	AddColumn( "Property", 15 );
	AddColumn( "number", 6 );
	PrintHeader();
	AddRow("hello", "world ",2);
	AddRow( 1,2,numbers::pi);
	AddRow( 1,2,3,4 );
	AddRow(1, ':', "Hello");
}



