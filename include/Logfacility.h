/*
 * Logficility.h
 *
 *  Created on: Mar 1, 2024
 *      Author: sirius
 */

#ifndef LOGFACILITY_H_
#define LOGFACILITY_H_

#include <Ocean.h>

template< typename T >
constexpr bool isTTY ( const T io )
{
    if (isatty(fileno( io )))
    	return true; // "stdout is tty"
    else
       return false; 	//"stdout is not tty");
};
const bool is_a_tty	= isTTY( stdout );


inline void	ClearScreen()
{
	if ( is_a_tty )
		std::cout << "\x1B[2J\x1B[H";
}

enum 		LOG { ERROR, DEBUG, INFO, WARN, DBG2, BINFO, TEST, PLAIN, TABLE } ;

const uint8_t 				LOGMAX 		= 9;
const uint					LOGINDENT	= 20;
#define SETW 				setw( LOGINDENT )

// global Log facility structure
typedef 	bitset<LOGMAX>		logmask_t;

constexpr logmask_t setdefaultLogMask()
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


	const string 		logDir 		{ "/tmp/log/" };
	const string 		logFileName	{ "Synthesizer" };



public:
	const string	reset		= "\033[39m";
	const string 	boldon		= "\033[1m";
	const string	boldoff		= "\033[0m";
	const string	black		= "\033[30m";
	const string 	cyan 		= "\033[96m";
	const string 	green 		= "\033[92m";
	const string 	red 		= "\033[91m";
	const string 	magenta 	= "\033[95m";
	const string 	yellow  	= "\033[33m";
	const string 	blue 		= "\033[94m";
	const string	bblack		= boldon + black;
	const string 	bgreen		= boldon + green;
	const string 	bred		= boldon + red;
	const string 	bblue		= boldon + blue;
	const string 	byellow		= boldon + yellow;
	const string	nocolor		= "";
	const string 		Line 			{ "---------------------------------------------------------" };
	const range_t<int>	loglevel_range 	{ 0, LOGMAX - 1 };
	string 				className 		{ "" };
	string 				prefixClass 	{ "" };
	const string 		logFile	 		= logDir + logFileName + string(".log") ;
	const string 		endcolor		= boldoff + reset;

	Logfacility_class( string module  );
	Logfacility_class( );
	virtual ~Logfacility_class(  );

	void 	Set_Loglevel( int level, bool on );

	string	GetColor( uint id );
	string	GetendColor( );
	void 	ResetLogMask();
	void 	Show_loglevel();
	string 	Error_text( uint );
	void 	Init_log_file();
	void 	Test_Logging();

	void 	TEST_START(const string& name);
	void 	TEST_END(const string& name);


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
		string 	key { };
		string 	str { };
	} pair_struct_t;
	typedef array<pair_struct_t, 64> error_arr_t ;

	error_arr_t error_arr { pair_struct() };
	void seterrText();

	// https://en.wikipedia.org/wiki/ANSI_escape_code#Colors


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
	vector<log_struct> Prefix_vec =
	{
			{"Error", bred },
			{"Debug", magenta },
			{"Info ", bblue },
			{"Warn ", byellow },
			{"Dbg2 ", yellow },
			{"bInfo", bgreen },
			{"Test ", blue },
			{""		, nocolor },
			{"Table", bblue }
	};


	string 	cout_log( uint logid, string str );


};


#endif /* LOGFACILITY_H_ */


