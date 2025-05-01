/*
 * Logficility.cpp
 *
 *  Created on: Mar 1, 2024
 *      Author: sirius
 */

#include <Logfacility.h>

logmask_t 	LogMask 		= defaultLogMask;


Logfacility_class::Logfacility_class( string module )
{
					seterrText();
	size_t 			pos 			= module.find("_" );
					if ( pos > LOGINDENT )
						this->className = module;
					else
						this->className = module.substr(0, pos ) ;
	stringstream 	strs 			{};
					strs << setw(15) << right << className << ":";
					prefixClass 	= strs.str();
	string 			_path 			= string( logDir );
					filesystem::create_directories( _path );
};
Logfacility_class::Logfacility_class( )
{
					seterrText();
					this->className = "Log";
	stringstream 	strs 			{};
					strs << setw(15) << right << className << ":";
					prefixClass 	= strs.str();
	string 			_path 			= string( logDir );
	filesystem::create_directories( _path );

}

Logfacility_class::~Logfacility_class(  )
{
	if( LogMask[ DEBUG ])
		cout.flush() << "visited ~Logfacility_class." << className  << endl;
};

string	Logfacility_class::GetColor( uint id )
{
	if ( is_a_tty )
		return Prefix_vec[ id ].color;
	return Prefix_vec[ PLAIN ].color;
}
string	Logfacility_class::GetendColor( )
{
	if ( is_a_tty )
		return endcolor;
	return nocolor;
}

string Logfacility_class::cout_log( uint id, string str )
{
	set<int> 	ignore { ENOENT };
	if (( errno ) and not ignore.contains( errno ))
	{
		string txt	= Error_text( errno );
		if ( txt.length() > 0 )
			cout.flush() << txt << endl;
	}
	string 			prefix 	= prefixClass + Prefix_vec[ INFO].name;
	stringstream 	strs 	{};
	string 			endc 	= ( is_a_tty ) ? endcolor 	: nocolor;
	uint 			Id		= ( is_a_tty ) ? id 		: LOG::PLAIN;

	strs << Prefix_vec[ Id ].color << prefix << str << endc << endl;
	cout.flush() << strs.str();
	return str;
}
void Logfacility_class::ResetLogMask()
{
	LogMask = defaultLogMask;
}
#define NOERR 0
void Logfacility_class::seterrText()
{
	// defined in /usr/include/asm-generic/errno-base.h

	error_arr[NOERR] 	=   {"NOERR",	"no error"};
	error_arr[EPERM] 	=   {"EPERM",	"Operation not permitted"};
	error_arr[ENOENT] 	=   {"ENOENT",	"No such file or directory"};
	error_arr[ESRCH] 	=   {"ESRCH",	"No such process"};
	error_arr[EINTR] 	=   {"EINTR",	"Interrupted system call"};
	error_arr[EIO] 		=   {"EIO",		"I/O error"};
	error_arr[ENXIO] 	=   {"ENXIO",	"No such device or address "};
	error_arr[E2BIG] 	=   {"E2BIG",	"Argument list too long "};
	error_arr[ENOEXEC] 	= 	{"ENOEXEC",	"Exec format error"};
	error_arr[EBADF] 	=   {"EBADF",	"Bad file number"};
	error_arr[ECHILD] 	=   {"ECHILD",	"No child processes"};
	error_arr[EAGAIN] 	=   {"EAGAIN",	"Try again"};
	error_arr[ENOMEM] 	=   {"ENOMEM",	"Out of memory"};
	error_arr[EACCES] 	=   {"EACCES",	"Permission denied"};
	error_arr[EFAULT] 	=   {"EFAULT",	"Bad address"};
	error_arr[ENOTBLK] 	=   {"ENOTBLK",	"Block device required"};
	error_arr[EBUSY] 	=   {"EBUSY",	"Device or resource busy"};
	error_arr[EEXIST] 	=   {"EEXIST",	"File exists"};
	error_arr[EXDEV] 	=   {"EXDEV",	"Cross-device link"};
	error_arr[ENODEV] 	=   {"ENODEV",	"No such device"};
	error_arr[ENOTDIR] 	=   {"ENOTDIR",	"Not a directory"};
	error_arr[EISDIR] 	=   {"EISDIR",	"Is a directory"};
	error_arr[EINVAL] 	=   {"EINVAL",	"Invalid argument"};
	error_arr[ENFILE] 	=   {"ENFILE",	"File table overflow"};
	error_arr[EMFILE] 	=   {"EMFILE",	"Too many open files"};
	error_arr[ENOTTY] 	=   {"ENOTTY",	"Not a typewriter"};
	error_arr[ETXTBSY] 	=   {"ETXTBSY",	"Text file busy"};
	error_arr[EFBIG] 	=   {"EFBIG",	"File too large"};
	error_arr[ENOSPC] 	=   {"ENOSPC",	"No space left on device"};
	error_arr[ESPIPE] 	=   {"ESPIPE",	"Illegal seek"};
	error_arr[EROFS] 	=   {"EROFS",	"Read-only file system"};
	error_arr[EMLINK] 	=   {"EMLINK",	"Too many links"};
	error_arr[EPIPE] 	=   {"EPIPE",	"Broken pipe"};
	error_arr[EDOM] 	=   {"EDOM",	"Math argument out of domain of func"};
	error_arr[ERANGE] 	=   {"ERANGE",	"Math result not representable"};
}

void Logfacility_class::Init_log_file( )
{
	Comment( INFO, "Initialize Log file ");
	string _path = string( logFile );
	if (filesystem::exists( _path ))
		filesystem::remove( _path );
}

void Logfacility_class::Show_loglevel()
{
	Info( Line );
	string on = "";
	string logmode = ( is_a_tty ) ? "console logging" : "file logging";
	Comment( INFO, "Log level activation state with " + logmode );
	for ( int level = 0; level < LOGMAX; level++ )
	{
		on =  LogMask[ level ] ? "On" : "Off";
		Comment( level, "Log level " + Prefix_vec[ level ].name + " is " + on );
	}
	Info( Line );
}

string Logfacility_class::Error_text( uint err )
{
	string str = "";
	if ( ( err < error_arr.size() - 1 ) )
	{
		str = "[" + error_arr[err].key + "] " + error_arr[err].str;
		errno = 0;
		return str;
	}
	else
	{
		str = "error no " + to_string(err) + " to be defined";
		Comment( DEBUG , str);
		return "";
	}
}

void Logfacility_class::Set_Loglevel( int _level, bool _on )
{

	uint level = check_range(loglevel_range, _level );
	LogMask.set( level, _on );
}



void Logfacility_class::TEST_START( const string& name)
{
	Set_Loglevel( TEST, true) ;

	Comment( TEST, Line );
	Comment( TEST, "Test " + name + " start" ) ;
}

void Logfacility_class::TEST_END( const string& name )
{
	Set_Loglevel( TEST, true) ;
	Comment( TEST, "Test " + name + " finished" );
	Comment( TEST, Line );
	Set_Loglevel( TEST, false) ;
}

#include <String.h>
void Logfacility_class::Test_Logging( )
{
	LogMask = defaultLogMask;
	seterrText();
	TEST_START( className );
	Show_loglevel();
	ASSERTION( LogMask.count() == 5, "logmask count", LogMask.count(), 5  );
	ASSERTION( LogMask[ TEST ], "logmask", LogMask[TEST], true );
	string str = Error_text( EEXIST );
	ASSERTION( 	strEqual(str, "[EEXIST] File exists") , "error string",
				strEqual(str, "[EEXIST] File exists") , "[EEXIST] File exists" );
	Set_Loglevel( TEST, true );
	Comment( TEST, "Logfacility test start");
	stringstream True;
	True << boolalpha << LogMask[TEST];// no endl
	ASSERTION( 	strEqual( True.str(), "true"), "true",
				strEqual( True.str(), "true"), "=true" );
	int L = ( 4 | 8 );
	ASSERTION( L == 12, "OR", L, 12 )
	bitset<4> bs = 0b1100;
	ASSERTION( bs[WARN] | bs[ERROR], "bitset", bs.test(ERROR), true );
	ASSERTION( isTTY( stdout ), "isTTY", isTTY( stdout ), true);
	TEST_END( className );

}
