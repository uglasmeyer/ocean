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


	Table_class( ) :
		Logfacility_class("Table_class")
	{
	};
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
			cout << "Invalid number of columns" << endl;
			return;
		}
		uint n	= 0;
		((std::cout << dec << left << setw(header_v[n++].width)  << args ), ...);
		cout << endl;
	}

	void PrintHeader()
	{
		for( header_t row : header_v )
		{
			cout  << dec << setw(row.width) << left << setfill(' ')<< row.txt;
		}
		cout << endl;
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
