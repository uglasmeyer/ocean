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
 * appstate.cpp
 *
 *  Created on: Apr 26, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include <data/Config.h>
#include <String.h>
#include <System.h>
#include <Table.h>


/**************************************************
 * source_struct
 *************************************************/

source_struct::source_struct( string srcdir )
{
	sourcedir		= srcdir;
	if ( sourcedir.length() == 0 )
		sourcedir 	= notnull( getenv("PWD") ) + "/../../";
	resourcedir		= resourceDir();
	files			= { resourcedir, archdir };
}


string source_struct::read_installfile()
{
	Comment( DEBUG, "reading install_file" );
	if ( sourcedir.length() == 0 )
		Exception( "no sourcedir given");
	string 			install 	= HOME + "/OceanBase/";
	const string	install_file= sourcedir + "install.txt";
	String 			S 			{};
	fstream 		Install 	{ install_file, fstream::in };

	if ( Install.is_open() )
	{
		Comment( DEBUG, "Reading ", install_file );
		while ( getline( Install, S.Str ))
		{
			S.normalize();
			vector_str_t arr = S.to_array( '=' );
			if( arr[0].compare( oceandev_env ) == 0 )
				return arr[1];
		}
	}
	Comment( DEBUG, "installdir= ", install );

	return install;

}

void source_struct::show_installdirs()
{
	for( string file : files)
	{
		Comment( BINFO, file );
	}
}
string source_struct::resourceDir()
{
	if( sourcedir.length() == 0 ) return "";
	string _dir = sourcedir + "/Resource/";
	if ( not filesystem::is_directory( _dir ) )
		Comment( ERROR,  "unknown resource directory ", resourcedir );
	return _dir;
}

#include <sys/utsname.h>

string getArch()
{
    struct utsname utsbuf;
    uname( &utsbuf );
    return notnull( utsbuf.machine );
};


string install_struct::oceanDir( )
{

	string oceandir = notnull( getenv( OCEANDIR ));
	if( oceandir.length() == 0 )
	{
		oceandir = homedir + "OceanDev/";
		Comment( WARN, "Using default", oceandir );
	}
	return oceandir;

	string basedir = "";
	const string Envvar = OCEANDIR;
	string Env = notnull( std::getenv( Envvar.data()) );
	if( filesystem::is_regular_file( Env + "/etc/synthesizer.cfg" ))
	{
		if( Env[Env.length()-1] == '/' )
		{
			basedir = Env;
		}
		else
		{
			basedir = Env + "/";
		}
		Comment( DEBUG, "using " + Envvar +" "+ basedir );
		return basedir;
	}
	Comment( INFO, "not in " + Envvar );

	// path
	string prgname = program_invocation_short_name;
	string file = searchPath( prgname );
	filesystem::path filepath = file;
	string parentpath = filepath.parent_path();
	filesystem::path cfgname = parentpath + "/../etc/synthesizer.cfg";
	if ( file.length() > 0 )
		if( filesystem::is_regular_file( cfgname ) )
		{
			basedir = parentpath + "/../" ;
			Comment( DEBUG, "using oceanbasedir " + basedir );
			return basedir;
		}
	Comment( INFO, "not in searchPATH" );

	// pwd
	string pwd = notnull( getenv( "PWD" ));
	string prgpwd = pwd + "/" + prgname ;
	if ( filesystem::is_regular_file( prgpwd + "../etc/synthesizer.cfg" ))
	{
		basedir = pwd + "/../";
		Comment( INFO, "using PWD basedir " + basedir );
		return basedir;
	}
	Comment( ERROR, pwd + " is not an Ocean basedir, or " + prgname + " not in PATH");
	Comment( ERROR, "or " + Envvar + " is incorrect");

	exit(1);

	return string("");
}
install_struct::install_struct()
: 	Logfacility_class( "install_struct" )
{};
dir_struct::dir_struct( )
	: install_struct()
{};
file_structure::file_structure()
	: dir_struct()
{};

string file_structure::baseName ( string _dir )
{

	String Str { _dir };
	vector_str_t arr = Str.to_array( '/' );
	string name = "";
	if( arr.size() > 1 )
		name = arr[ arr.size() - 2 ];
	Comment( DEBUG, "baseName: ", name );
	return name;
};
string file_structure::get_rec_filename( uint no )
{
	return filename + to_string( no ) ;
}


/***************************
 * Config_class
************************** */

Config_class::Config_class() :
	Logfacility_class( "Config_class" )
{
	className 			= Logfacility_class::className;
	prgName				= Process.name;
	fs					= &Fs;
	if( not ( Process.AppId == SETUPID ))
	{
		Comment( INFO, "Program name: ", prgName );
		cout << Line(80, 'v' )  << endl;
	}
	parse_cmdline();
	if( filesystem::is_regular_file( fs->config_file ) )
	{
		Read_config( fs->config_file );
	}
};


Config_class::~Config_class()
{ DESTRUCTOR( className ); };

void Config_class::setAppCWD()
{
	try
	{
		filesystem::current_path( fs->tmpdir );
		Info( "Updated directory", filesystem::current_path() );
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		Comment( ERROR, "Error:", e.what() );
	}
}

void Config_class::CreateInstalldirs( )
{
	auto create_dir=[ this ]( string _p )
	{
		Comment( DEBUG, "CreateInstalldir: ", _p );
		if ( filesystem::is_directory( _p ))
			return false;
		if ( filesystem::is_regular_file( _p ) )
		{
			filesystem::remove( _p);
		}
		return filesystem::create_directories( _p );
	};

	Comment( DEBUG, "Checking directory structure");
	ASSERTION( (fs->install_dirs.size() != 0 ),"DirStructure_class::Create",
				fs->install_dirs.size(),"not=0");
	for( string dir : fs->install_dirs )
	{
		if ( create_dir( dir ) )
		{
			Comment( INFO, "Synthesizer directory ", dir, " created");
		}
	}
}


void Config_class::Read_config(	string cfgfile )
{
	map<string, string> 	Get = {}; // @suppress("Invalid template argument")

	configfile = cfgfile;
	Comment( INFO, "Reading config file", configfile );

	ifstream cFile( configfile  );
	if ( not cFile.is_open() )
	{
		Exception("Couldn't open config file " + configfile);
	}
	String Line{""};
	String Name{""};
	string line;
	while ( getline( cFile, line ) )
	{
		Line = line;
		Line.replace_comment();
		Line.replace_char('\t', ' ');
		istringstream iss( Line.Str );
		string strr;
		while (getline(iss, strr, ','))
		{
			size_t delimiterPos	= strr.find("=");
			if ( delimiterPos == STRINGNOTFOUND )
			{
				;
			}
			else
			{
				string value      	= strr.substr(delimiterPos + 1);
				Name.Str        	= strr.substr(0, delimiterPos) ;
				Name.normalize();
				Name.to_lower();
				Get[Name.Str]		= value;
			}
		}
	}
	auto get_item = [ &Get ]( string old, string str )
		{
			string tmp = string( Get[ str ]);
			return ( tmp.length() == 0 ) ?  old : tmp;
		};

	auto get_value = [ &Get ]( key_t old, string str )
	{
		String 	Str	{""};
		string 	tmp = string( Get[ str ]);
		int		val = (key_t ) Str.to_int( tmp );
		string 	comment = "synthesizer.cfg entry " + str;
		const char*	char_comment = comment.data();
		ASSERTION( val > 0, char_comment, tmp, str );

		return ( tmp.length() == 0 ) ? old : (key_t) val ;
	};

	auto get_char = [ &Get ]( char old, string str )
	{
		string tmp			= string( Get[ str ] );
		return ( tmp.length() == 0 ) ? old : tmp[0];
	};

	Config.title		= get_item ( Config.title		, "title" );
	Config.author		= get_item ( Config.author		, "author" );
	Config.album		= get_item ( Config.album		, "album" );
	Config.Genre 		= get_item ( Config.Genre		, "genre" );
	Config.Term 		= get_item ( Config.Term		, "term" );
	Config.ffmpeg 		= get_item ( Config.ffmpeg		, "ffmpeg" );
	Config.SHM_key		= get_value( Config.SHM_key		, "shmkey" );
	Config.SDS_key		= get_value( Config.SDS_key		, "sdskey" );
	Config.Sem_key		= get_value( Config.Sem_key		, "semkey" );
	Config.temp_sec		= get_value( Config.temp_sec	, "temp_sec" );
	Config.record_sec	= get_value( Config.record_sec	, "record_sec" );
	Config.kbd_sec		= get_value( Config.kbd_sec		, "kbd_sec" );
	Config.MAXWAVFILES 	= get_value( Config.MAXWAVFILES	, "maxwavfiles");
	Config.debug		= get_char ( Config.debug		, "debug" );
	for( uint idx = 0; idx < MAXCONFIG; idx++  )
	{
		Config.sdskeys	[ idx ] = Config.SDS_key + idx;
	}
	Config.SHM_keyl = Config.SHM_key;
	Config.SHM_keyr = Config.SHM_key + 1;


	Config.test		= get_char( Config.test, "test" );
	std::set<char> yn = {'n', 'y', 0 };
	ASSERTION( yn.contains( Config.test ), "synthesizer.cfg test: ",Config.test, "y or n" ) ;
	if ( Config.test == 'y' )
	{
		Set_Loglevel( TEST, true );
	}
	if ( Config.debug == 'y' )
	{
		Set_Loglevel( DEBUG, true );
	}

}

void Config_class::Test()
{
	vector_str_t args = parse_cmdline();
	cout << show_str_items( args ) << endl;
	ASSERTION( args[1][0] == 't', "cmdline contains -t", args[1], "t" );
//	exit_proc( 0 );
}

vector_str_t Config_class::parse_cmdline()
{
	const 	string	pid 			= to_string( getpid() );
	const 	string	cmdfilename 	= "/proc/" + pid + "/cmdline";
			string	filename		{""};
			String	cmdline 		{""};

	getline( ifstream( cmdfilename ), cmdline.Str ) ;
	cmdline.to_array( '-' );
	for( string str : cmdline.Vec )
		if( str[0] == 'v' )
			if( str.length() == 2 ) // str is null-terminated
				Set_Loglevel( DEBUG, true );
	return cmdline.Vec;
}

void Config_class::Parse_argv( int argc, char* argv[] )
{
	String 			Str	{""};
	string 			next{""};
	char 			ch 	= 0;

	for ( int ndx = 1; ndx < argc; ndx++ )
	{

		string arg = argv[ ndx ];
		if( arg[0] ==  '-' )
		{
			( arg.length() > 1) ? ch = arg[1] : ch = 0;
			if ( (ndx + 1) == argc  )
				next = "" ;
			else
			{
				next.assign( argv[ ndx + 1 ] );
				ndx++;
			}
		}
		else // assign filename
		{
			ch = 0;
		}

		switch ( ch )
		{
			case 'c' : 	Config.channel		= Str.to_int( next ); break;
			case 'd' : 	Config.device 		= Str.to_int( next ); break;
			case 'k' : 	Config.SDS_key 		= Str.to_int( next ); break;
			case 'o' : 	Config.ch_offs		= Str.to_int( next ); break;
			case 'r' : 	Config.rate 		= Str.to_int( next ); break;
			case 't' : 	Config.test 		= 'y'				; break;
			case 'v' : 	Set_Loglevel( DEBUG, true )				; break;
			case 'A' : 	Config.archive		= 'y'				; break;
			case 'C' : 	Config.composer		= 'y'				; break;
			case 'D' : 	Config.dialog 		= 'y'				; break;
			case 'G' : 	Config.oceangui		= 'y'				; break;
			case 'I' : 	Config.installdir	= next = trailing_slash( next )	; break;
			case 'S' : 	Config.sourcedir	= next = trailing_slash( next )	; break;
			case 'V' :	Set_Loglevel( DEBUG, true );
						Set_Loglevel( DBG2, true )				; break;
			default  : 	Config.filename		= arg				; break;
		}
	}

	if( Process.AppId == SETUPID )
	{
		if ( Config.filename.length() > 0) Info( "Config.filename  ", Config.filename );
		if ( Config.sourcedir.length() > 0) Info( "Config.sourcedir ", Config.sourcedir);
		if ( Config.installdir.length() > 0) Info( "Config.installdir", Config.installdir);
	}
}

void Config_class::Show_Config( bool debug )
{
	if ( not debug )
		return;
	Table_class Table { "synthesizer.cfg", 20 };
	Table.AddColumn( "Config Entry", 20 );
	Table.AddColumn( "Value", 20  );
	Table.PrintHeader();
	Table.AddRow( "sampline rate" 		, Config.rate				);  		// -c
	Table.AddRow( "device nr" 			, Config.device	);  		// -d
	Table.AddRow( "channel offs"		, Config.ch_offs	); 		// -o
	Table.AddRow( "test classes" 		, Config.test		);  		// -t
	Table.AddRow( "dialog mode"			, Config.dialog	);  		// -D
	Table.AddRow( "composer"			, Config.composer	);  		// -D
	Table.AddRow( "oceangui"			, Config.oceangui	);  		// -D
	Table.AddRow( "Id3tool Title" 		, Config.title	); 		// -o
	Table.AddRow( "Id3tool Author"		, Config.author	);  		// -k
	Table.AddRow( "Id3tool Album" 		, Config.album	);  		//
	Table.AddRow( "Id3tool Genre" 		, Config.Genre	);  		// -t
	Table.AddRow( "MAXWAVFILES"			, Config.MAXWAVFILES	);  		// -t
	Table.AddRow( "Terminal"	 		, Config.Term		);  		// -D
	Table.AddRow( "shm key"			 	, Config.SHM_key	);  		// -k
	Table.AddRow( "sds_key" 			, Config.SDS_key	);  		// -D
	Table.AddRow( "sem_key" 			, Config.Sem_key	);  		// -D
	Table.AddRow( "sds keys"			, show_type( Config.sdskeys ) );
	Table.AddRow( "SHM_key left"		, Config.SHM_keyl );
	Table.AddRow( "SHM key right"		, Config.SHM_keyr );
	Table.AddRow( "ffmpeg" 				, Config.ffmpeg	);  		// -D
	Table.AddRow( "temp storage sec"	, Config.temp_sec	);
	Table.AddRow( "keyboard storage sec", Config.kbd_sec	);
	Table.AddRow( "record storage sec"	, Config.record_sec	);
	Table.AddRow( "Install directory"	, Config.installdir );
	Table.AddRow( "Source directory"	, Config.sourcedir );
	Table.AddRow( "File name"			, Config.filename );
}

string Config_class::trailing_slash( const string& dir )
{
	size_t	len		= dir.length();
	if( len == 0 )
		return "";
	size_t 	last = len-1;
	char	ch	= dir[last];
	if( ch == '/' )
		return dir;
	else
		return dir + '/';
}
string Config_class::Server_cmd( string term, string srv, string srvopt)
{

	string cmd = "unknown TERM or NOHUP";

	if ( strEqual( term, Config.Nohup ) )
		cmd = term + " " + srv + " " + srvopt + " &";

	if ( strEqual( term, Config.Term ) )
		cmd = term + " \"" + srv + " " + srvopt + "\" &";

	Comment( BINFO, "command: " , cmd );

	return cmd;
}






