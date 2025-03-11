/*
 * Logficility.h
 *
 *  Created on: Mar 1, 2024
 *      Author: sirius
 */

#ifndef LOGFACILITY_H_
#define LOGFACILITY_H_

#include <Ocean.h>




#define SETW setw(20)
typedef vector<string> LogVector_t;

enum { ERROR, DEBUG, INFO, WARN, DBG2, BINFO, TEST } ;
const 	uint8_t 	LOGMAX 		= 7;
extern 	array<bool, LOGMAX > LogMask;

class Logfacility_class
{
	const string 		logDir 		{ "/tmp/log/"};
	const string 		logFileName	{ "Synthesizer" };
	const string 		logFile	 	{ logDir + logFileName + ".log" };
	LogVector_t*		LogVector_p = nullptr;

public:
	const string 		Line = "-----------------------------------------------------------";
	string 				className 	= "";

	void Comment( const int& level, const string logcomment );
	void Set_Loglevel( int level, bool on );
	void Show_loglevel();
	string Error_text( uint );

//	void Info( string str );
	void Init_log_file();
	void WriteLogFile();
	void StartFileLogging( LogVector_t* lvp );
	void Test_Logging();
	void TEST_START(const string& name);
	void TEST_END(const string& name);

	Logfacility_class( string className = "" );
	virtual ~Logfacility_class(  );

	template <class... ArgsT>
	void Info( ArgsT... args )
	{
		string format = className + ":" +  Prefix[INFO] ;
		cout.flush() << Color[INFO] <<  SETW << format <<  " ";
		( cout <<  ... << args  ) << endc << endl;
	};

/*	template<class T, class... Targs>
	void Info2( size_t count, T logcomment, Targs... Fargs) // recursive variadic function
	{
	    size_t argc = sizeof...( Fargs );
		if ( (count-1) == (argc)  )
		{

			string format = className + ":" +  Prefix[INFO] ;
			cout.flush() << Color[INFO] <<  SETW << format << logcomment << " ";
		}
		else
		{
			if ( argc == 0 )
			{
				cout << logcomment << endc << endl;
				return;
			}
			cout << logcomment << " ";
		}
		Info2( count, Fargs... );
	}
*/

private:

	typedef struct pair_struct
	{
		string 	key;
		string 	str;
	} pair_struct_t;
	vector<pair_struct_t> error_vector ;

	string comment_str = "";
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

	const vector<string> Levelname	= {
		"error", 	"debug", 	"info " ,"warn ", "dbg2 "    , "binfo", "test"};
	const vector<string> Prefix 	= {
		"Error> ",	"Debug> ",   "Info > "	,"Warn > ", "Dbg2 >" , "Info > ", "Test > "};
	const vector<string> Color 		= {
		bred, 		magenta, 	green 	, yellow, yellow , bgreen, blue };

	void setup();
	void Info2( size_t& ){}; // redirect

};


#endif /* LOGFACILITY_H_ */


