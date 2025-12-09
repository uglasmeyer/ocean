/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * Logficility.h
 *
 *  Created on: Mar 1, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef LOGFACILITY_H_
#define LOGFACILITY_H_

#include <Ocean.h>
#include <Utilities.h>
#include <Kbd.h>

#define coutf 			cout.flush()


constexpr string Line( int len = 58, char ch = '-' )
{
	string str {};
	for (int n = 0; n < len; n++)
	{
		str.append( 1, ch );
	}
	return str;
}


enum  LOG_e : uint8_t
{
	ERROR,
	DEBUG,
	INFO,
	WARN,
	DBG2,
	BINFO,
	TEST,
	PLAIN,
	TABLE,
	WAIT,
	LOG_SIZE
} ;
const range_T<LOG_e>		loglevel_range 	{ (LOG_e)0, (LOG_e)(LOG_SIZE - 1) };

const uint					LOGINDENT	= 20;
#define SETW 				setw( LOGINDENT )
#define NEWLOGLINE			setw( LOGINDENT + 10 )

// global Log facility structure
typedef 	bitset<LOG_SIZE>		logmask_t;

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


#define DESTRUCTOR( className )\
{\
		{ if( LogMask[ DBG2 ] )\
		cerr.flush() << "~" << className << endl; }\
};
const string 		logDir 		{ "/tmp/log/" };
const string 		logFileName	{ "Synthesizer" };
const string 		logFile		= logDir + logFileName + string(".log") ;

class Printer_class
{
	//https://stackoverflow.com/questions/9084099/re-opening-stdout-and-stdin-file-descriptors-after-closing-them	bool redirect 		= false;
	bool 	testFinished 	= false;
	int 	save_out 		= -1;//dup( STDOUT_FILENO );

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

#define LUNDEF 35



class Logfacility_class
{

public:
	const string			reset		= "\033[39m";
	const string 			boldon		= "\033[1m";
	const string			boldoff		= "\033[0m";
	const string			black		= "\033[30m";
	const string 			cyan 		= "\033[96m";
	const string 			green 		= "\033[92m";
	const string 			red 		= "\033[91m";
	const string 			magenta 	= "\033[95m";
	const string 			yellow  	= "\033[33m";
	const string 			blue 		= "\033[94m";
	const string			bblack		= boldon + black;
	const string 			bgreen		= boldon + green;
	const string 			bred		= boldon + red;
	const string 			bblue		= boldon + blue;
	const string 			byellow		= boldon + yellow;
	const string 			bmagenta	= boldon + magenta;
	const string			nocolor		= "";
	string 					className 		{ "" };
	string 					prefixClass 	{ "" };
	const string 			logFile	 		= logDir + logFileName + string(".log") ;
	const string 			endcolor		= boldoff + reset;

					Logfacility_class( string module  );
					Logfacility_class( );
	virtual			~Logfacility_class(  );

	void 			Set_Loglevel	( LOG_e level, bool on );
	string			GetColor		( uint id );
	string			GetendColor		( );
	void 			ResetLogMask	();
	void 			Show_loglevel	();
	string 			Error_text		( uint );
	void 			Init_log_file	();
	void 			Test_Logging	();

	void 			test_start		(const string& name);
	void 			test_end		(const string& name);


	template <class... ArgsT>
	string Info( ArgsT... args )
	{
		return Comment( INFO, args... );
	};
	template <class... ArgsT>
	kbdInt_t Wait( Kbd_base* kbd, ArgsT... args )
	{
		kbdInt_t key = NOKEY;
		if ( not is_atty )
		{
			return key;
		}
		Comment( LOG_e::WAIT, args... );
		if ( LogMask.test( LOG_e::WAIT ) )
		{
			key = kbd->GetKeyInt( 100 );
		}
		return key;
	};

	template <class... ArgsT>
	string Comment( const LOG_e& level, ArgsT... args )
	{
		LOG_e id = check_range( loglevel_range, level, "Comment" );
		if ( LogMask.test( id ) )
		{
			stringstream strs{};
			strs << dec;
			(( strs << args << " "	), ... );
			return cout_log( id, strs.str() );
		}
		return "";
	}

private:
	struct log_struct
	{
		string name {""};
		string color{""};
		log_struct( string n, string c )
		{
			name 	= n + ">";
			color 	= c;
		}
		virtual ~log_struct() = default;
	};
	typedef struct pair_struct
	{
		string 	key { "" };
		string 	str { "" };
	} pair_struct_t;
	typedef array<pair_struct_t, 64> error_arr_t ;

	error_arr_t error_arr { pair_struct() };

	// https://en.wikipedia.org/wiki/ANSI_escape_code#Colors



	vector<log_struct> Prefix_vec =
	{
			{"Error", bred },
			{"Debug", black },
			{"Info ", bblue },
			{"Warn ", bmagenta },
			{"Dbg2 ", yellow },
			{"bInfo", bgreen },
			{"Test ", blue },
			{""		, nocolor },
			{"Table", bblack },
			{"Wait ", bred }
	};

	string 	cout_log( LOG_e logid, string str );
	void 	seterrText();


};


#endif /* LOGFACILITY_H_ */


