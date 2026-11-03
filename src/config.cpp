/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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

#include <fstream>
#include <data/Config.h>
#include <String.h>
#include <System.h>
#include <Table.h>
#include <sys/utsname.h>
#include <Default.h>

using std::ifstream;
using namespace std::filesystem;
string getArch()
{
    struct utsname uts_data;
    uname( &uts_data );
    return notnull( uts_data.machine );
};


/**************************************************
 * source_struct
 *************************************************/



source_struct::source_struct( string srcdir )
{
	if ( srcdir.length() == 0 )
		srcdir 	= trailing_dirslash( notnull( getenv("OCEANSRC") ) );
	if ( srcdir.length() == 0 )
		srcdir  = trailing_dirslash( notnull( getenv("PWD") ) ) + "../";

	sourcedir		= srcdir;
	resourcedir		= resourceDir();
	files			= { resourcedir, archdir };
}

string source_struct::resourceDir()
{
	string _dir = sourcedir + "Resource/";
	if ( not is_directory( _dir ) )
		Comment( ERROR,  "unknown resource directory ", resourcedir );
	return _dir;
}


/**************************************************
 * install_struct
 *************************************************/
string install_struct::oceanDir( )
{

	string oceandir = trailing_dirslash( notnull( getenv( OCEANDIR )) );
	if( oceandir.length() == 0 )
	{
		oceandir = homedir + "OceanDev/";
		Comment( WARN, "Using default", oceandir );
	}
	return oceandir;
}

install_struct::install_struct()
: 	Logfacility_class( "install_struct" )
{};

/**************************************************
 * dir_struct
 *************************************************/
dir_struct::dir_struct( )
	: install_struct()
{};

/**************************************************
 * file_structure
 *************************************************/
file_structure::file_structure()
	: dir_struct()
{};

string file_structure::baseName ( string _dir )
{

	String Str { _dir };
	string_vec_t arr = Str.to_array( '/' );
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
string file_structure::SubDir( string basedir, string subdir )
{
	if( subdir.length() == 0 )
		return basedir;

	string dir = trailing_dirslash( basedir + subdir );
	if( not exists( dir ) )
		create_directories( dir );

	return dir;
}


/***************************
 * Config_class
************************** */

Config_class::Config_class() :
	Logfacility_class( "Config_class" )
{
	prgName				= Process.name;
	fs					= &Fs;
	if( not ( Process.AppId == SETUPID ))
	{
		Comment( INFO, "Program name: ", prgName );
		cout << Line(80, 'v' )  << endl;
	}
	parse_cmdline();
	Write_template_text( fs->config_file, config_template_text, KEEPFILE );
	Read_config( fs->config_file );
};


Config_class::~Config_class()
{
	DESTRUCTOR( className );
};

void Config_class::setAppCWD( string cwd )
{
	try
	{
		current_path( cwd );
		Info( "Updated directory", current_path() );
	}
	catch (const filesystem_error& e)
	{
		Comment( ERROR, "Error:", e.what() );
	}
}

void Config_class::CreateInstalldirs( )
{
	auto create_dir=[ this ]( string _p )
	{
		Comment( DEBUG, "CreateInstalldir: ", _p );
		if ( is_directory( _p ))
			return false;
		if ( is_regular_file( _p ) )
		{
			remove( _p);
		}
		return create_directories( _p );
	};

	Comment( INFO, "Checking directory structure");
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
	Comment( DEBUG, "Reading config file", configfile );

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
	Config.Logview		= get_item ( Config.Logview		, "logviewer" );

	for( uint idx = 0; idx < MAXCONFIG; idx++  )
	{
		Config.sdskeys	[ idx ] = Config.SDS_key + idx;
	}
	if( Process.data_process )
	{
		Config.SHM_keyl = Config.SHM_key;
		Config.SHM_keyr = Config.SHM_key + 1;
	}

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
	string_vec_t args = parse_cmdline();
	cout << show_str_items( args ) << endl;
	ASSERTION( args[1][0] == 't', "cmdline contains -t", args[1], "t" );
}

string_vec_t Config_class::parse_cmdline()
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
			case 'c' : 	{ Config.channel		= Str.to_int( next ); break; }
			case 'd' : 	{ Config.device 		= Str.to_int( next ); break; }
			case 'i' :	{ Config.clearipc		= 'y'				; break; }
			case 'k' : 	{ Config.SDS_key 		= Str.to_int( next ); break; }
			case 'o' : 	{ Config.ch_offs		= Str.to_int( next ); break; }
			case 'r' : 	{ Config.rate 		= Str.to_int( next ); break; }
			case 't' : 	{ Config.test 		= 'y'				; break; }
			case 'v' : 	{ Set_Loglevel( DEBUG, true )				; break; }
			case 'A' : 	{ Config.archive		= 'y'				; break; }
			case 'C' : 	{ Config.composer		= 'y'				; break; }
			case 'D' : 	{ Config.dialog 		= 'y'				; break; }
			case 'G' : 	{ Config.oceangui		= 'y'				; break; }
			case 'I' : 	{ Config.installdir	= next = trailing_dirslash( next )	; break; }
			case 'S' : 	{ Config.sourcedir	= next = trailing_dirslash( next )	; break; }
			case 'V' :	{ Set_Loglevel( DEBUG, true );
						Set_Loglevel( DBG2, true )				; break; }
			case 'Y' :	{ Config.Deploy		= 'y'				; break; }
			default  : 	{ Config.filename	= arg				; break; }
		}
	}

	if( Process.AppId == SETUPID )
	{
		if ( Config.filename.length() 	> 0) Info( "Config.filename  ", Config.filename );
		if ( Config.sourcedir.length() 	> 0) Info( "Config.sourcedir ", Config.sourcedir);
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
	Table.AddRow( "Log Viewer"			, Config.Logview	);
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

string Config_class::Logviewer( string file )
{
	return Config.Logview + " -title " + file + " -e tail -f -n 1000 " +  file  ;
}





