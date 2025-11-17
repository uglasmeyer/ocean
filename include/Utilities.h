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
 * Utilities.h
 *
 *  Created on: Oct 17, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OCEANUTILITIES_H_
#define OCEANUTILITIES_H_

#include <csignal>
#include <iostream>
#include <string>
#include <vector>
#include <experimental/source_location>

extern unsigned char operator""_uint( unsigned long long int x);
using namespace std;

extern void Assert_equalstr( string lhs, string rhs, string message = " == ",
		const experimental::source_location location = experimental::source_location::current());
extern void show_source_location( const experimental::source_location location );
extern bool	fcomp( const float& a, const float& b, float precision = 1E-4 );
extern void Exception( string message = "", int signal = SIGHUP,
		const experimental::source_location location = experimental::source_location::current());


template<class Compare>
void Show_Assert( Compare lhs, Compare rhs, string message )
{
	cout 	<< message 							<< endl;
	cout	<< "lhs    value: " << dec << lhs	<< endl;
	cout	<< "rhs    value: " << dec << rhs 	<< endl;
	raise ( SIGHUP );
};


template< typename Type >
vector<Type> Iota_T( Type beg, Type size )
{
	vector<Type> v{};
	for( Type n = beg; n < (beg+size); n++ )
		v.push_back( n );
	return v;
};


template<class Compare>
void Assert_equal( Compare lhs, Compare rhs, string message = " == ",
		const experimental::source_location location = experimental::source_location::current())
{
	bool comp = ( lhs == rhs );
	if ( not comp )
	{
		show_source_location( location );
		Show_Assert( lhs, rhs, message );
	};
};

template<class Compare>
void Assert_unequal( Compare lhs, Compare rhs, string message = " != ",
		const experimental::source_location location = experimental::source_location::current())
{
	if ( not ( lhs != rhs  ) )
	{
		show_source_location( location );
		Show_Assert( lhs, rhs, message );
	};
};


template<class Compare>
void Assert_lt( Compare lhs, Compare rhs, string message = " < ",
		const experimental::source_location location = experimental::source_location::current())
{
	if ( not ( lhs < rhs  ) )
	{
		show_source_location( location );
		Show_Assert( lhs, rhs, message );
	};
};


#define ASSERTION(	 expr , message, input, expected )\
	if ( not (expr) ) \
	{\
	printf( "file: ( %s ) line: ( %d ) in function: ( %s )\n", __FILE__, __LINE__, __func__ );\
	cout 	<< message 									<< endl\
			<< "input    value: " << (input) 			<< endl\
			<< "expected value: " << dec << (expected) 	<< endl;\
	raise ( SIGHUP ); \
	};




#endif /* UTILITIES_H_ */
