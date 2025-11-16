/*
 * utilitiers.cpp
 *
 *  Created on: Oct 17, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Utilities.h>

void Exception( string message, int signal, const experimental::source_location location )
{
	cout << "An application exception occured at:" << endl;\
	show_source_location( location );
	cout << message << endl;
	raise( signal );
};

void show_source_location( const experimental::source_location location )
{
			cout << "file: "
              << location.file_name() << '('
              << location.line() << ':'
              << location.column() << ") `"
              << location.function_name() << "`: \n";
}

void Assert_equalstr( string lhs, string rhs, string message ,
		const experimental::source_location location )
{
	bool comp = ( lhs.compare( rhs ) == 0 );
	if ( not comp )
	{
		show_source_location( location );
		Show_Assert( lhs, rhs, message );
	};
};


unsigned char operator""_uint( unsigned long long int x)
{
	return (uint8_t)x;
}
