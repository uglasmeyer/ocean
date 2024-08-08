/*
 * Logficility.h
 *
 *  Created on: Mar 1, 2024
 *      Author: sirius
 */

#ifndef LOGFACILITY_H_
#define LOGFACILITY_H_

#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;



class Logfacility_class
{
public:
	static const int 	logmax 		= 6;
	array<bool,logmax+1> Log { true, false, true, true, false, true, false };
	enum { ERROR, DEBUG, INFO, WARN, DBG2, BINFO, TEST } ;

	Logfacility_class( string  );
	virtual ~Logfacility_class(  )
	{	cout.flush();
	};


	// make all true to enable debugging of initialization phase
	void Comment( int, const string& );
	void Set_Loglevel( int, bool );
	void Show_loglevel();
	string error_text( int );
	void init_log_file();
	void test();


private:
	string module = "";

	typedef struct pair_struct
	{
		int 	key;
		string 	str;
	} pair_struct_t;
	vector<pair_struct_t> error_vector {};


	// https://en.wikipedia.org/wiki/ANSI_escape_code#Colors

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

	const array<string, logmax+1> Levelname = {
			"error", 	"debug", 	"info " ,"warn ", "dbg2 "    , "binfo", "test"};
	const string Prefix[logmax+1] 	= {
			"Error> ",	"Debug> ",   "Info > "	,"Warn > ", "Dbg2 >" , "Info > ", "Test > "};
	const string Color [logmax+1]	= {
			bred, 		magenta, 	green 	, yellow, yellow , bgreen, blue };

	void setup();


};

#endif /* LOGFACILITY_H_ */
