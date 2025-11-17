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

 *
 *  Created on: Jan 4, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <Ocean.h>
#include <String.h>
#include <Exit.h>

extern vector<string> 	List_directory		( const string& path, const string& filter );
extern string 			searchPath			( string filename );
extern void 			System_execute		( const string& cmd, bool noexect = false );
extern int 				System_execute_bg	( const string& _cmd);
extern void 			System_Test			();
extern bool 			Is_running_process	( const pid_t& pid );
extern bool 			Is_running_process	( const string& pidstr );

extern void 			Remove_file			( string file );
extern void 			Rename_file			( string old_name, string new_name );


#endif /* INCLUDE_COMMON_H_ */
