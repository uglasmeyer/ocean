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
const bool is_atty	= isTTY( stdout );

inline string line( const uint& col, char ch )
{
	string str{};
	for( uint n = 0; n< col; n++ )
		str.append(ch,1);
	return str;
}
inline void	ClearScreen()
{
	if ( is_atty )
		std::cout.flush() << "\x1B[2J\x1B[H";
}
inline void CursorHome()
{
	if ( is_atty )
		std::cout.flush() << "\x1B[H" ;
}
inline void CursorHomeCol()
{
	if ( is_atty )
	{
		std::cout.flush() << "\x1B[0G" ;
		std::cout.flush() << line(80, ' ' );
		std::cout.flush() << "\x1B[0G" ;
	}
}
inline void CursorPos( uint x, uint y )
{
	if ( is_atty )
		std::cout.flush() << "\x1B[" << y << ";" << x << "H" ;
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



const string 		logDir 		{ "/tmp/log/" };
const string 		logFileName	{ "Synthesizer" };
const string 		logFile		= logDir + logFileName + string(".log") ;

class Printer_class
{
	//https://stackoverflow.com/questions/9084099/re-opening-stdout-and-stdin-file-descriptors-after-closing-them	bool redirect 		= false;
	bool 	testFinished 	= false;
	int 	save_out 		= dup( STDOUT_FILENO );

public:
	bool	redirect		= false;
			Printer_class	(bool redirect );
	virtual ~Printer_class	();
	void 	Close			();

private:
	void 	store			();
	void 	restore			();
};

#define TEST_START( classname )\
	test_start( classname ); Printer_class Printer( true );

#define TEST_END( classname )\
	Printer.Close(); test_end( classname );

#define coutf cout.flush()

class Logfacility_class
{

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
	const string 	bmagenta	= boldon + magenta;
	const string	nocolor		= "";
	const string 		Line 			{ "---------------------------------------------------------" };
	const range_T<int>	loglevel_range 	{ 0, LOGMAX - 1 };
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

	void 	test_start(const string& name);
	void 	test_end(const string& name);


	template <class... ArgsT>
	string Info( ArgsT... args )
	{
		stringstream strs{};
		strs << dec;
		( strs <<  ... << args  ) ;
		return cout_log( INFO, strs.str() );
	};

	template <class... ArgsT>
	void Comment( const int& level, ArgsT... args )
	{
		int id = check_range( loglevel_range, level, "Comment" );
		if ( LogMask.test( id ) )
		{
			stringstream strs{};
			strs << dec;
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
			{"Warn ", bmagenta },
			{"Dbg2 ", yellow },
			{"bInfo", bgreen },
			{"Test ", blue },
			{""		, nocolor },
			{"Table", bblack }
	};

	string 	cout_log( uint logid, string str );


};


#endif /* LOGFACILITY_H_ */


