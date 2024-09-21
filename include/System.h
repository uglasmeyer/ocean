/*

 *
 *  Created on: Jan 4, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <Ocean.h>
#include <String.h>

extern void system_execute( const string& );
extern vector<string> List_directory( const string& path, const string& filter );
extern void Exception( const string& );
//extern void Wait(long  int );
extern string searchPath( string filename );

const static long int SECOND		= 1000000;
const static long int MILLISECOND	= 1000;
const static long int MICROSECOND	= 1;




#endif /* INCLUDE_COMMON_H_ */
