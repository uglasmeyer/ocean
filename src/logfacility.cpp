/*
 * Logficility.cpp
 *
 *  Created on: Mar 1, 2024
 *      Author: sirius
 */

#include <Logfacility.h>

array<bool, LOGMAX> LogMask { true, false, true, true, false, true, false };


Logfacility_class::Logfacility_class( string module)
{
	this->className = module.substr(0, 12) ;
	setup();
};

Logfacility_class::~Logfacility_class(  )
{
	if( LogMask[ DEBUG ])
		cout.flush() << "visited ~Logfacility_class." << className  << endl;
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

	string _path = string( logDir );
	filesystem::create_directories( _path );

}

void Logfacility_class::Init_log_file()
{
	Comment( INFO, "Initialize Log file ");
	string _path = string( logFile );
	if (filesystem::exists( _path ))
		filesystem::remove( _path );
}
void Logfacility_class::WriteLogFile()
{
	if( not LogVector_p )
		return;
	if( LogVector_p->size() == 0 )
		return;

	fstream	LogFILE ;
	LogFILE.open( "/tmp/log/Synthesizer.log", fstream::out );
	if( not LogFILE.is_open() )
	{
		cout << "cannot open logfile to write " << endl;
		return;
	}
	for ( string str : *LogVector_p )
	{
		LogFILE.flush() << str << endl;
	}
}

void Logfacility_class::StartFileLogging( LogVector_t* lvp )
{
	this->LogVector_p = lvp;
}

void Logfacility_class::Show_loglevel()
{
	string on;
	Comment( INFO, "Log level activation state");
	for ( int level = 0; level < LOGMAX; level++ )
	{
		on =  LogMask[ level ] ? "On" : "Off";
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
	if(( level < 0 ) or ( level > LOGMAX -1 ))
	{
		Info( "Loglevel out of bounds in ", className );
		return;
	}
	LogMask[ level ] = on;
}


void Logfacility_class::Comment( const int& level, const string logcomment )
{
	if ( level < LOGMAX + 1 )
	{
		if ( LogMask[ level ] )
		{
			comment_str = className + ":" +  Prefix[ level] ;
			cout.flush() 	<< Color[level] << SETW << comment_str << logcomment << endc << endl;
			string strs { comment_str + logcomment };
			if ( LogVector_p )
				LogVector_p->push_back( strs );
		}
	}
}
/*
template <class... ArgsT>
void Logfacility_class::Info( ArgsT... args )
{
	(std::cout << ... << args  ) << "\n";
//	Comment( INFO, text );
}
*/

void Logfacility_class::TEST_START( const string& name)
{
	Set_Loglevel( TEST, true) ;

	Comment( TEST, Line );
	Comment( TEST, "Test " + name + " start" ) ;
}

void Logfacility_class::TEST_END( const string& name )
{
	Comment( TEST, "Test " + name + " finished" );
	Comment( TEST, Line );
	Set_Loglevel( TEST, false) ;
}

#include <String.h>

void Logfacility_class::Test_Logging( )
{
//	InfoT( "Variatic", " arguments");

	string str = Error_text( EEXIST );
	ASSERTION( strEqual( str, "[EEXIST] File exists" ), "error string", str, "[EEXIST] File exists" );
	Set_Loglevel( TEST, true );
	Comment( TEST, "Logfacility test start");
	stringstream True;
	True << boolalpha << LogMask[TEST];// no endl
	ASSERTION( strEqual( True.str(), "true"), "true", True.str(), "=true" );
	Comment( TEST, "Logfacility test OK");

}
