/*
 * Logficility.cpp
 *
 *  Created on: Mar 1, 2024
 *      Author: sirius
 */

#include <iostream>
#include <Logfacility.h>
#include <Synthesizer.h>
#include <filesystem>
using namespace std;


Logfacility_class::Logfacility_class( string module )
{
	this->module = module ;
	setup();
};
Logfacility_class::~Logfacility_class(  )
	{	//cout.flush();
	};
void Logfacility_class::setup()
{
	error_vector.clear();
	error_vector.push_back(  {EPERM,"Operation not permitted"});
	error_vector.push_back(  {ENOENT,"No such file or directory"});
	error_vector.push_back(  {ESRCH,"No such process"});
	error_vector.push_back(  {EINTR,"Interrupted system call"});
	error_vector.push_back(  {EIO,"I/O error"});
	error_vector.push_back(  {ENXIO,"No such device or address "});
	error_vector.push_back(  {E2BIG,"Argument list too long "});
	error_vector.push_back(  {ENOEXEC,"Exec format error"});
	error_vector.push_back(  {EBADF,"Bad file number"});
	error_vector.push_back(  {ECHILD,"No child processes"});
	error_vector.push_back(  {EAGAIN,"Try again"});
	error_vector.push_back(  {ENOMEM,"Out of memory"});
	error_vector.push_back(  {EACCES,"Permission denied"});
	error_vector.push_back(  {EFAULT,"Bad address"});
	error_vector.push_back(  {ENOTBLK,"Block device required"});
	error_vector.push_back(  {EBUSY,"Device or resource busy"});
	error_vector.push_back(  {EEXIST,"File exists"});
	error_vector.push_back(  {EXDEV,"Cross-device link"});
	error_vector.push_back(  {ENODEV,"No such device"});
	error_vector.push_back(  {ENOTDIR,"Not a directory"});
	error_vector.push_back(  {EISDIR,"Is a directory"});
	error_vector.push_back(  {EINVAL,"Invalid argument"});
	error_vector.push_back(  {ENFILE,"File table overflow"});
	error_vector.push_back(  {EMFILE,"Too many open files"});
	error_vector.push_back(  {ENOTTY,"Not a typewriter"});
	error_vector.push_back(  {ETXTBSY,"Text file busy"});
	error_vector.push_back(  {EFBIG,"File too large"});
	error_vector.push_back(  {ENOSPC,"No space left on device"});
	error_vector.push_back(  {ESPIPE,"Illegal seek"});
	error_vector.push_back(  {EROFS,"Read-only file system"});
	error_vector.push_back(  {EMLINK,"Too many links"});
	error_vector.push_back(  {EPIPE,"Broken pipe"});

	filesystem::create_directories(dir_struct().logdir);

}

void Logfacility_class::Init_log_file()
{
	Comment( INFO, "Initialize Log file ");
	if (filesystem::exists( file_structure().log_file ))
		filesystem::remove( file_structure().log_file );
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

string Logfacility_class::Error_text( int err )
{
	string str = "";
	for ( pair_struct_t pair : error_vector )
	{
		if ( pair.key == err )
		{
			return pair.str;
		}
	}
	str = "error no " + to_string(err) + " to be defined";
	Comment( DEBUG , str);
	return str;
}

void Logfacility_class::Set_Loglevel( int level, bool on )
{
	if ( level > logmax ) level=logmax;
	if ( level < 2 ) level = 1;
	Log[ level ] = on;
}

void Logfacility_class::Comment( int level, const string& logcomment )
{
	if (level < logmax + 1 )
	{
		if ( Log[ level ] )
		{
			string comment_str = module + ":" +  Prefix[ level] ;
			cout.flush() 	<< Color[level] << setw(20) << comment_str << logcomment << endc << endl;
			fstream LOGFILE(file_structure().log_file, fstream::app);
			LOGFILE.flush() << setw(20) << comment_str << logcomment << endl;
		}
	}
}

void Logfacility_class::Test_Logging( )
{
	auto truefalse = []( bool Bool )
		{
			return Bool ? "true" : "false";
		};
	string str = Error_text( EEXIST );
	assert( str.compare( "File exists") 		== 0 );
	Set_Loglevel( TEST, true );
	Comment( TEST, "Logfacility test start");
	cout << truefalse( Log[TEST] ) << endl;
	string True = truefalse("true");
	assert( True.compare("true") == 0 );
	Comment( TEST, "Logfacility test OK");

}
