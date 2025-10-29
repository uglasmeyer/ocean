/*

 *
 *  Created on: Jan 4, 2024
 *      Author: sirius
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
