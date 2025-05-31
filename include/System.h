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
extern void 			system_execute		( const string& );
extern void 			System_Test			();
extern bool 			Is_running_process	( const pid_t& pid );

#endif /* INCLUDE_COMMON_H_ */
