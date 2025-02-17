/*
 * Logficility.cpp
 *
 *  Created on: Mar 1, 2024
 *      Author: sirius
 */

#include <Logfacility.h>



using namespace std;



Logfacility_class::Logfacility_class( string module )
{
	this->module = module ;
	setup();
};

Logfacility_class::~Logfacility_class(  )
{

	if( LogVector.size() == 0 )
		return;
	fstream		LogFILE { "/tmp/log/Synthesizer.log", fstream::app };
	for ( string str : LogVector )
	{
		LogFILE.flush() << str;
	}
	LogVector.clear();
	cout << "visited ~" << className << "." << module << endl;

};

void Logfacility_class::setup()
{
	error_vector.clear();
	error_vector.push_back(  {"NOERR","no error"});
	error_vector.push_back(  {"EPERM","Operation not permitted"});
	error_vector.push_back(  {"ENOENT","No such file or directory"});
	error_vector.push_back(  {"ESRCH","No such process"});
	error_vector.push_back(  {"EINTR","Interrupted system call"});
	error_vector.push_back(  {"EIO","I/O error"});
	error_vector.push_back(  {"ENXIO","No such device or address "});
	error_vector.push_back(  {"E2BIG","Argument list too long "});
	error_vector.push_back(  {"ENOEXEC","Exec format error"});
	error_vector.push_back(  {"EBADF","Bad file number"});
	error_vector.push_back(  {"ECHILD","No child processes"});
	error_vector.push_back(  {"EAGAIN","Try again"});
	error_vector.push_back(  {"ENOMEM","Out of memory"});
	error_vector.push_back(  {"EACCES","Permission denied"});
	error_vector.push_back(  {"EFAULT","Bad address"});
	error_vector.push_back(  {"ENOTBLK","Block device required"});
	error_vector.push_back(  {"EBUSY","Device or resource busy"});
	error_vector.push_back(  {"EEXIST","File exists"});
	error_vector.push_back(  {"EXDEV","Cross-device link"});
	error_vector.push_back(  {"ENODEV","No such device"});
	error_vector.push_back(  {"ENOTDIR","Not a directory"});
	error_vector.push_back(  {"EISDIR","Is a directory"});
	error_vector.push_back(  {"EINVAL","Invalid argument"});
	error_vector.push_back(  {"ENFILE","File table overflow"});
	error_vector.push_back(  {"EMFILE","Too many open files"});
	error_vector.push_back(  {"ENOTTY","Not a typewriter"});
	error_vector.push_back(  {"ETXTBSY","Text file busy"});
	error_vector.push_back(  {"EFBIG","File too large"});
	error_vector.push_back(  {"ENOSPC","No space left on device"});
	error_vector.push_back(  {"ESPIPE","Illegal seek"});
	error_vector.push_back(  {"EROFS","Read-only file system"});
	error_vector.push_back(  {"EMLINK","Too many links"});
	error_vector.push_back(  {"EPIPE","Broken pipe"});

	filesystem::create_directories( logDir);

}

void Logfacility_class::Init_log_file()
{
	Comment( INFO, "Initialize Log file ");
	if (filesystem::exists( logFile ))
		filesystem::remove( logFile );
}

void Logfacility_class::Show_loglevel()
{
	string on;
	Comment( INFO, "Log level activation state");
	for ( int level = 0; level < logmax+1; level++ )
	{
		on =  Log[ level ] ? "On" : "Off";
		Comment( INFO, "Log level " + Levelname[ level ] + " is " + on );
	}
}

string Logfacility_class::Error_text( uint err )
{
	string str = "";
	if ( ( err < error_vector.size() - 1 ) )
	{
		return "[" + error_vector[err].key + "] " + error_vector[err].str;
	}
	else
	{
		str = "error no " + to_string(err) + " to be defined";
		Comment( DEBUG , str);
		return str;
	}
}

void Logfacility_class::Set_Loglevel( int level, bool on )
{
	if ( level > logmax ) level=logmax;
	if ( level < 2 ) level = 1;
	Log[ level ] = on;
}


void Logfacility_class::Comment( const int& level, const string& logcomment )
{
	stringstream strs {};

	if (level < logmax + 1 )
	{
		if ( Log[ level ] )
		{
			comment_str = module + ":" +  Prefix[ level] ;
			cout.flush() 	<< Color[level] << SETW << comment_str << logcomment << endc << endl;
//			LogFILE.flush() << setw(20) << comment_str << logcomment << endl;
			strs << setw(20) << comment_str << logcomment << endl;
			LogVector.push_back( strs.str() );
		}
	}
}


void Logfacility_class::Info( string text )
{
	Comment( INFO, text );
}


void Logfacility_class::TEST_START( const string& name)
{
	Set_Loglevel( TEST, true) ;

	Comment( TEST, "Test " + name + " start" ) ;
}

void Logfacility_class::TEST_END( const string& name )
{
	Comment( TEST, "Test " + name + " finished" );
	Set_Loglevel( TEST, false) ;
}


void Logfacility_class::Test_Logging( )
{
//	InfoT( "Variatic", " arguments");

	string str = Error_text( EEXIST );
	assert( str.compare( "[EEXIST] File exists") 		== 0 );
	Set_Loglevel( TEST, true );
	Comment( TEST, "Logfacility test start");
	stringstream True;
	True << boolalpha << Log[TEST];// no endl
	assert( True.str().compare("true") == 0 );
	Comment( TEST, "Logfacility test OK");

}
