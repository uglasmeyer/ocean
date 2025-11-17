//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


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

	string color 		= GetColor( TABLE );
	string endcolor 	= GetendColor();

	cout << color << setw( opt.Ident ) << left << setfill('.') << opt.Titel << endcolor ;
	if ( opt.Titel.length() > opt.Ident )
		cout << endl;
	for( header_t row : header_v )
	{
		if ( not opt.FILE )
			 cout  << dec << setw(row.width) << left << setfill(' ')<< row.txt << opt.Separator;
		else
			*opt.FILE  << dec << setw(row.width) << left << setfill(' ')<< row.txt << opt.Separator;
	}
	if( not opt.FILE )
		cout << endl;
	else
		*opt.FILE << '\n';
}
string Table_class::cout_row( string txt )
{
	string color 		= GetColor( TABLE );
	string endc			= GetendColor();
	string colorline 	= color + txt + endc + opt.Crlf;

	cout << colorline;
	if( opt.FILE )
	{
		if( not opt.FILE->is_open() )
		{
			Comment(WARN, "file not open");
			return colorline;
		}

		*opt.FILE << txt << opt.Crlf;
	}
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



