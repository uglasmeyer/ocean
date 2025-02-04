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

extern vector<string> 	List_directory( const string& path, const string& filter );
extern string 			searchPath( string filename );
extern void 			system_execute( const string& );
extern void 			Exception( const string& );
extern void 			System_Test();

template< typename T>
bool Is_running_process( const T& pid )
{
	stringstream strs {};
	strs << "/proc/" << pid ;

	bool isdir = filesystem::is_directory( strs.str() ) ;
	return isdir ;
}

#endif /* INCLUDE_COMMON_H_ */
