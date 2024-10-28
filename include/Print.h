/*
 * Print.h
 *
 *  Created on: Oct 25, 2024
 *      Author: sirius
 */

#ifndef PRINT_H_
#define PRINT_H_

#include <Ocean.h>
#include <Logfacility.h>



class Print_class
{
	string className = "";
public:
	fstream File;

	Print_class( string name )
	{
		this->className = name;
		File.open( "/tmp/log/S.txt", fstream::app );
	};
	virtual ~Print_class()
	{
		File.close();
	};
	const string 	boldon		= "\033[1m";
	const string	boldoff		= "\033[0m";
	const string	black		= "\033[30m";
	const string 	cyan 		= "\033[96m";
	const string 	green 		= "\033[92m";
	const string 	red 		= "\033[91m";
	const string 	magenta 	= "\033[95m";
	const string 	yellow  	= "\033[33m";
	const string 	blue 		= "\033[94m";
	const string 	endc		= boldoff+ "\033[39m";
	const string 	bgreen		= boldon + green;
	const string 	bred		= boldon + red;

	const vector<string> Levelname	= {
		"error", 	"debug", 	"info " ,"warn ", "dbg2 "    , "binfo", "test"};
	const vector<string> Prefix 	= {
		"Error> ",	"Debug> ",   "Info > "	,"Warn > ", "Dbg2 >" , "Info > ", "Test > "};
	const vector<string> Color 		= {
		bred, 		magenta, 	green 	, yellow, yellow , bgreen, blue };

	template<class T, class... Targs>
	void Info2( size_t count, T logcomment, Targs... Fargs) // recursive variadic function
	{
		Logfacility_class LogPrint{className};
	    size_t argc = sizeof...( Fargs );
		if ( (count-1) == (argc)  )
		{

			string format = LogPrint.module + ":" +  "INFO > " ;
			File.flush() <<  SETW << format  << logcomment << " ";
			cout << Color[INFO] <<  SETW << format << logcomment << " ";
		}
		else
		{
			if ( argc == 0 )
			{
				cout << logcomment << endc << endl;
				File.flush() << logcomment << endl;
				return;
			}
			cout << logcomment << " ";
			File.flush() << logcomment << " " ;
		}
		Info2( count, Fargs... );
	}
private:
	void Info2( size_t& ){}; // redirect

};



#endif /* PRINT_H_ */
