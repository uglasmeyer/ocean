/*
 * Table.h
 *
 *  Created on: Mar 10, 2025
 *      Author: sirius
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <Logfacility.h>

class Table_class :
	virtual Logfacility_class
{
	string 		className 	= "";
	char 		separator 	= ' ';
	uint		Ident		= 0;
	string 		Title		= "";
	fstream* 	File 		= nullptr;
	struct header_struct
	{
		string 	txt;
		uint 	width;
	};
	typedef header_struct header_t;
	vector<header_t> header_v {};

public:
	Table_class( string title = "", uint ident = LOGIDENT );
	Table_class( fstream* F, char s ) ;
	virtual ~Table_class();

	void AddColumn( string name, uint width );

	template< typename... Args>
	string AddRow( Args... args)
	{
		uint argc = sizeof...(args);
		if ( argc > header_v.size() )
		{
			Comment(ERROR, "Invalid number of columns: ", argc, " expected: ", header_v.size());
			return "";
		}
		stringstream strs {};
		uint n	= 0;
		strs << setw( Ident ) << left << "";
		((  strs << dec << left << setw(header_v[n++].width)  << args << separator), ...);
		cout << strs.str() << endl;

		if( File )
		{
			*File << strs.str();
			*File << '\n';
		}
		return strs.str();

	};

	void PrintHeader();
	void TestTable();

private:
};



#endif /* TABLE_H_ */
