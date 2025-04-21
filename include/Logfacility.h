/*
 * Logficility.h
 *
 *  Created on: Mar 1, 2024
 *      Author: sirius
 */

#ifndef LOGFACILITY_H_
#define LOGFACILITY_H_

#include <Ocean.h>
#include <bitset>

typedef vector<string> LogVector_t;

const 	uint					LOGIDENT	= 20;
const 	uint8_t 				LOGMAX 		= 8;
#define SETW 					setw( LOGIDENT )

// global Log facility structure
typedef 	bitset<8>			logmask_t;
enum 		LOG { ERROR, DEBUG, INFO, WARN, DBG2, BINFO, TEST, PLAIN } ;
constexpr auto setdefaultLogMask()
{
	logmask_t lm;
	for ( uint bit : { ERROR, INFO, WARN, BINFO } )
		lm.set( bit );
	return lm;
}
const 	logmask_t 	defaultLogMask 	= setdefaultLogMask();
extern 	logmask_t 	LogMask; // define as: logmask_t LogMask = defaultLogMask in cpp file
// end


class Logfacility_class
{
	const string 		logDir 		{ "/tmp/log/"};
	const string 		logFileName	{ "Synthesizer" };
	LogVector_t*		LogVector_p = nullptr;

public:
	const string 		Line 			= "---------------------------------------------------------";
	range_t<int>		loglevel_range 	{ 0, LOGMAX - 1 };
	string 				className 		= "";
	string 				prefixClass 	= "";
	const string 		logFile	 		{ logDir + logFileName + ".log" };

	void 	Set_Loglevel( int level, bool on );
	void 	ResetLogMask();
	void 	Show_loglevel();
	string 	Error_text( uint );
	void 	Init_log_file();
	void 	WriteLogFile();
	void 	StartFileLogging( LogVector_t* lvp );
	void 	Test_Logging();
	void 	TEST_START(const string& name);
	void 	TEST_END(const string& name);

	Logfacility_class( string className = "" );
	virtual ~Logfacility_class(  );

	template <class... ArgsT>
	string Info( ArgsT... args )
	{
		stringstream strs{};
		( strs <<  ... << args  ) ;
		return cout_log( INFO, strs.str() );
	};

	template <class... ArgsT>
	void Comment( const int& level, ArgsT... args )
	{
		int id = check_range( loglevel_range, level );
		if ( LogMask.test( id ) )
		{
			stringstream strs{};
			( strs <<  ... << args  ) ;
			cout_log( id, strs.str() );
		}
	}

private:

	typedef struct pair_struct
	{
		string 	key;
		string 	str;
	} pair_struct_t;
	array<pair_struct_t, 35> error_arr ;

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
		  string 	endc		= boldoff+ "\033[39m";
	const string 	bgreen		= boldon + green;
	const string 	bred		= boldon + red;
	const string	nocolor		= "";

	struct log_struct
	{
		string name;
		string color;
		log_struct( string n, string c )
		{
			name 	= n + ">";
			color 	= c;
		}
		~log_struct() = default;
	};
	const vector<log_struct> Prefix_vec =
	{
			{"Error", bred },
			{"Debug", magenta },
			{"Info ", black },
			{"Warn ", yellow },
			{"Dbg2 ", yellow },
			{"bInfo", bgreen },
			{"Test ", blue },
			{""		, nocolor }
	};

	string 	cout_log( uint logid, string str );
	void 	seterrText();


};


#endif /* LOGFACILITY_H_ */


