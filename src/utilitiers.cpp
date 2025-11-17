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
