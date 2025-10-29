/*
 * Utilities.h
 *
 *  Created on: Oct 17, 2025
 *      Author: sirius
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <csignal>
#include <iostream>
#include <string>
#include <experimental/source_location>


using namespace std;




extern void show_source_location( const experimental::source_location location );

template<class Compare>
void Show_Assert( Compare lhs, Compare rhs, string message )
{
	cout 	<< message 								<< endl;
	cout	<< "input    value: " << lhs 			<< endl;
	cout	<< "expected value: " << dec << rhs 	<< endl;
	raise ( SIGHUP );
};

template<class Compare>
void Assert_equal( Compare lhs, Compare rhs, string message = " == ",
		const experimental::source_location location = experimental::source_location::current())
{
	if ( not ( lhs == rhs  ) )
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
