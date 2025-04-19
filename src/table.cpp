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
	Title 		= title;
	Ident		= ident;
};
Table_class::Table_class( fstream* F, char s ) :
	Logfacility_class("Table_class")
{
	className 	= Logfacility_class::className ;
	separator 	= s;
	File		= F;
}
Table_class::~Table_class() {};


void Table_class::AddColumn( string name, uint width )
{
	header_v.push_back({name, width} );
}


void Table_class::PrintHeader()
{
	if ( Title.length() == 0 )
		cout << setw( Ident ) << left << "";
	else
		cout << setw( Ident ) << left << setfill('.') << Title ;

	for( header_t row : header_v )
	{
		if ( not File )
			 cout  << dec << setw(row.width) << left << setfill(' ')<< row.txt << separator;
		else
			*File  << dec << setw(row.width) << left << setfill(' ')<< row.txt << separator;
	}
	if( not File )
		cout << endl;
	else
		*File << '\n';
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



