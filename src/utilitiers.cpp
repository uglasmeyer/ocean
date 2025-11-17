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
