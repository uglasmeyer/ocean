/*
 * Table.h
 *
 *  Created on: Mar 10, 2025
 *      Author: sirius
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <Ocean.h>
#include <Logfacility.h>

class Table_class :
		Logfacility_class
{
	string className = "";
public:

	char separator = ' ';
	fstream* File = nullptr;

	Table_class(  ) :
		Logfacility_class("Table_class")
	{
		className 	= Logfacility_class::className ;
	};
	Table_class( fstream* F, char s ) :
		Logfacility_class("Table_class")
	{
		className 	= Logfacility_class::className ;
		separator 	= s;
		File		= F;
	}
	~Table_class() {};

	struct header_struct
	{
		string 	txt;
		uint 	width;
	};
	typedef header_struct header_t;
	vector<header_t> header_v {};

	void AddColumn( string name, uint width )
	{
		header_v.push_back({name, width} );
	}

	template< typename... Args>
	void AddRow( Args... args)
	{
		uint argc = sizeof...(args);
		if ( argc > header_v.size() )
		{
			Comment(ERROR, "Invalid number of columns: ", argc, " expected: ", header_v.size());
			return;
		}
		uint n	= 0;
		if ( not File )
		{
			((  cout << dec << left << setw(header_v[n++].width)  << args << separator), ...);
			cout << endl;
		}
		else
		{
			if( File->is_open() )
			{
				(( *File << dec << left << setw(header_v[n++].width)  << args << separator), ...);
				*File << '\n';
			}
		}
	}

	void PrintHeader()
	{
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

	void TestTable()
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


private:
};



#endif /* TABLE_H_ */
