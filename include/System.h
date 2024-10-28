/*

 *
 *  Created on: Jan 4, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <Ocean.h>
#include <String.h>

extern vector<string> 	List_directory( const string& path, const string& filter );
extern string 			searchPath( string filename );
extern void 			catch_signals( sighandler_t proc, vector<uint> sig_v );
extern void 			system_execute( const string& );
extern void 			Exception( const string& );
extern void 			Assert(	bool expr, const string message,
								const source_location = source_location::current() ); // @suppress("Type cannot be resolved") // @suppress("Function cannot be resolved")

#endif /* INCLUDE_COMMON_H_ */
