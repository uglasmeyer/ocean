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
