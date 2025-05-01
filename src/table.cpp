/*
 * table.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: sirius
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
//	if ( opt.Titel.length() == 0 )
//		cout << setw( opt.Ident ) << left << "";
//	else
	string color = Logfacility_class::bblack;
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



