/*
 * Logficility.cpp
 *
 *  Created on: Mar 1, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include 			<Logfacility.h>

logmask_t 			LogMask 			= defaultLogMask;

Logfacility_class::Logfacility_class( string module )
{
					seterrText();
	int 			pos 				= module.find("_" );
	int				max_len				= LOGINDENT - 5;
					this->className 	= module;
	( pos > max_len ) ?	this->className = module.substr(0, pos) :
						this->className = module.substr(0, max_len );
	stringstream 	strs 				{};
					strs << setw( max_len ) << right << this->className << ":" ;
					prefixClass 		= strs.str();
	string 			_path 				= string( logDir );
					filesystem::create_directories( _path );
};
Logfacility_class::Logfacility_class( )
{
	Logfacility_class( "Log" );
}

Logfacility_class::~Logfacility_class(  )
{
	DESTRUCTOR( className )
};

string	Logfacility_class::GetColor( uint id )
{
	if ( is_atty )
		return Prefix_vec[ id ].color;
	return Prefix_vec[ PLAIN ].color;
}
string	Logfacility_class::GetendColor( )
{
	if ( is_atty )
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
	string 			prefix 	= prefixClass + Prefix_vec[ id ].name;
	stringstream 	strs 	{};
	string 			endc 	= ( is_atty ) ? endcolor 	: nocolor;
	uint 			Id		= ( is_atty ) ? id 		: LOG::PLAIN;
	size_t			pos		= str.find( '\n' );
	if ( pos < string::npos )
	{
		string s0 = str.substr(0, pos +1);
		string s1 = str.substr( pos +1 );
		stringstream s {};
		s << s0 << NEWLOGLINE << s1;
		str =  s.str();
	}
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
	error_arr[LUNDEF] 	=   {"undefined",	"unknown"};
}

void Logfacility_class::Init_log_file( )
{
	Comment( INFO, "Initialize Log file ");
	string _path = string( "/tmp/fout.tst" );//( logFile );
	if (filesystem::exists( _path ))
		filesystem::remove( _path );
}

void Logfacility_class::Show_loglevel()
{
//	Info( Line() );
	string on = "";
	string logmode = ( is_atty ) ? "console logging" : "file logging";
	Comment( INFO, "Log level activation state with " + logmode );
	for ( int level = 0; level < LOGMAX; level++ )
	{
		on =  LogMask[ level ] ? "On" : "Off";
		Comment( level, "Log level " + Prefix_vec[ level ].name + " is " + on );
	}
	Info( Line() );
}

string Logfacility_class::Error_text( uint err )
{
	string str = "";
	if ( ( err > LUNDEF ) )
	{
		err = LUNDEF;
	}
	str = "[" + error_arr[err].key + "] " + error_arr[err].str;
	errno = 0;
	return str;
}

void Logfacility_class::Set_Loglevel( int _level, bool _on )
{

	uint level = check_range(loglevel_range, _level,"Set_Loglevel" );
	LogMask.set( level, _on );
}

void Logfacility_class::test_start( const string& name)
{
	Set_Loglevel( TEST, true) ;
	Comment( TEST, "Test " + name + " start" ) ;
}

void Logfacility_class::test_end( const string& name )
{

	Set_Loglevel( TEST, true) ;
	cerr << GetColor( BINFO ) << "Test " + name + " finished" << endcolor << endl;
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
	ASSERTION( Printer.redirect != isTTY( stdout ), "isTTY", isTTY( stdout ), Printer.redirect );
	TEST_END( className );

}



Printer_class::Printer_class(bool _redirect )
{
	redirect = _redirect;
	fflush(stdout);
	if (redirect)
	{
		store();
		redirect = freopen(logFile.data(), "w+", stdout);
		redirect = true;
	}
};

Printer_class::~Printer_class()
{
	restore();
	if ( not testFinished )
		cerr.flush() <<  "cat " << logFile << endl;
}
void Printer_class::Close()
{
	restore();
	testFinished = true;
}

void Printer_class::store()
{
	if ( save_out >= 0 ) return;
	if( redirect )
	{
		save_out = dup( STDOUT_FILENO );
	}
}

void Printer_class::restore()
{
	if ( save_out < 0 ) return;
	if ( redirect )
	{
		dup2( save_out, STDOUT_FILENO );//, O_CLOEXEC );
		redirect = false;
	}
}


