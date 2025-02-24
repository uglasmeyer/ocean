#include <Config.h>
#include <String.h>
#include <System.h>
#include <data/Memorybase.h>
#include <Exit.h>

Config_class::Config_class( string Module ) :
	Logfacility_class( Module )
{

	prgname = program_invocation_name;
	string shortname = program_invocation_short_name;
	Comment( INFO, "Program name: " + shortname );
	this->basedir = baseDir( );
	Read_config( file_structure().config_file );
};

Config_class::~Config_class()
{

};

void Config_class::Read_config(	string cfgfile )
{
	std::unordered_map<string, string> 	Get = {}; // @suppress("Invalid template argument")

	String Str{""};
	configfile = cfgfile;
	Comment(INFO, "Reading config file " + configfile );

	ifstream cFile( configfile  );
	if ( not cFile.is_open() )
	{
		EXCEPTION("Couldn't open config file " + configfile);
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
			String Name        	= strr.substr(0, delimiterPos);
			string value      	= strr.substr(delimiterPos + 1);
			Name.normalize();
			Name.to_lower();
			Get[Name.Str]= value;
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
			ASSERTION( val > 0, "invalid synthesizer.cfg entry: ", tmp, str );

			return ( tmp.length() == 0 ) ? old : (key_t) val ;
		};

	auto get_char = [ &Get ]( char old, string str )
	{
		string tmp			= string( Get[ str ] );
		return ( tmp.length() == 0 ) ? old : tmp[0];
	};

	Config.title		= get_item ( Config.title, "title" );
	Config.author		= get_item ( Config.author, "author" );
	Config.album		= get_item ( Config.album, "album" );
	Config.Genre 		= get_item ( Config.Genre, "genre" );
	Config.Term 		= get_item ( Config.Term, "term" );
	Config.ffmpeg 		= get_item ( Config.ffmpeg, "ffmpeg" );
	Config.SHM_key		= get_value( Config.SHM_key, "shmkey" );
	Config.SDS_key		= get_value( Config.SDS_key, "sdskey" );
	Config.Sem_key		= get_value( Config.Sem_key, "semkey" );
	Config.temp_sec		= get_value( Config.temp_sec, "temp_sec" );
	Config.record_sec	= get_value( Config.record_sec, "record_sec" );

	for( uint idx = 0; idx < MAXCONFIG; idx++  )
	{
		Config.sdskeys	[ idx ] = Config.SDS_key + idx;
	}
	Config.SHM_keyl = Config.SHM_key;
	Config.SHM_keyr = Config.SHM_key + 1;


	Config.test		= get_char( Config.test, "test" );
	std::set<char> yn = {'n', 'y', 0 };
	ASSERTION( yn.contains( Config.test ), "invalid synthesizer.cfg entry: ",Config.test, "y or n" ) ;
//	Show_Config();
}

void Config_class::Test()
{
	cout << "getenv $0" << notnull( getenv( "$0" ) );
}

void Config_class::Parse_argv( int argc, char* argv[] )
{
	// https://en.cppreference.com/w/c/language/main_function

	String 			Str{""};
	string 			next{""};

	Comment(INFO, "Evaluating startup arguments");


	for ( int ndx = 1; ndx < argc; ndx ++ )
	{
		string arg = argv[ ndx ];
		( ndx + 1 == argc  ) ? next = "" : next = argv[ ndx + 1 ];
		if ( arg.compare("-c") == 0 )
			Config.channel	= Str.to_int( next );
		if ( arg.compare("-C") == 0 )
			Config.composer	= 'y';
		if ( arg.compare("-G") == 0 )
			Config.oceangui	= 'y';
		if ( arg.compare("-r") == 0 )
			Config.rate 	= Str.to_int( next );
		if ( arg.compare("-d") == 0 )
			Config.device 	= Str.to_int( next );
		if ( arg.compare("-o") == 0 )
			Config.ch_offs	= Str.to_int( next );
		if ( arg.compare("-k") == 0 )
			Config.SDS_key = Str.to_int( next );
		if ( arg.compare("-t") == 0 )
			Config.test 	= 'y';
		if ( arg.compare("-D") == 0 )
			Config.dialog 	= 'y';
		if ( arg.compare("-X") == 0 ) // force clear proc register
			Config.clear = 'y';

	}

}

void Config_class::Show_Config( )
{
	stringstream strs{""};

	auto lline = [ &strs ]( string str, auto val )
	{
		strs << SETW << right << str << setw(40) << right <<	val <<endl;;
	};
	strs << endl;
	lline( "sampline rate" 	, 	Config.rate		);  		// -c
	lline( "device nr" 		, 	Config.device	);  		// -d
	lline( "channel offs"	, 	Config.ch_offs	); 		// -o
	lline( "test classes" 	, 	Config.test		);  		// -t
	lline( "dialog mode"	,	Config.dialog	);  		// -D
	lline( "composer"		,  	Config.composer	);  		// -D
	lline( "oceangui"		, 	Config.oceangui	);  		// -D
	lline( "Id3tool Title" , 	Config.title	); 		// -o
	lline( "Id3tool Author", 	Config.author	);  		// -k
	lline( "Id3tool Album" , 	Config.album	);  		//
	lline( "Id3tool Genre" , 	Config.Genre	);  		// -t
	lline( "Terminal" 		, 	Config.Term		);  		// -D
	lline( "shm key"	 	, 	Config.SHM_key	);  		// -k
	lline( "sds_key" 		, 	Config.SDS_key	);  		// -D
	lline( "sem_key" 		, 	Config.Sem_key	);  		// -D
	lline( "sds keys"		, show_type( Config.sdskeys ) );
	lline( "SHM_key left"	,	Config.SHM_keyl );
	lline( "SHM key right"	, 	Config.SHM_keyr );
	lline( "ffmpeg" 		, 	Config.ffmpeg	);  		// -D
	lline( "temp storage sec", Config.temp_sec	);
	lline( "record storage sec", Config.record_sec	);

	Comment( INFO, strs.str() );
}

string Config_class::baseDir()
{
	const char* envvar = "OCEANDIR";
	string Envvar( envvar );
	string Env = notnull( std::getenv( envvar) );
	if( filesystem::is_regular_file( Env + "/etc/synthesizer.cfg" ))
	{
		basedir = Env + "/";
		Comment( INFO, "using " + Envvar +" "+ basedir );
		return basedir;
	}
	Comment( INFO, "not in " + Envvar );

	string file = searchPath( prgname );
	filesystem::path filepath = file;
	string parentpath = filepath.parent_path();
	filesystem::path cfgname = parentpath + "/../etc/synthesizer.cfg";
	if ( file.length() > 0 )
		if( filesystem::is_regular_file( cfgname ) )
		{
			basedir = parentpath + "/../" ;
			Comment( INFO, "using basedir " + basedir );
			return basedir;
		}
	Comment( INFO, "not in searchPATH" );
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

string Config_class::Server_cmd( string term, string srv, string srvopt)
{
	string cmd = "";
	if ( term.length() == 0 )
	{
		cmd = srv;
		cmd.append(" ");
		cmd.append( srvopt );
		cmd.append( " &" );
	}
	else
	{
		cmd = term;
		cmd.append( " '");
		cmd.append(srv),
		cmd.append(" ");
		cmd.append(srvopt);
		cmd.append("' &");
	}
	Comment( INFO, "command:" + cmd );

	return cmd;
}



void DirStructure_class::Create()
{
	Comment(INFO,"Checking directory structure");
	ASSERTION( ( dir_struct().dirs.size() != 0 ),"DirStructure_class::Create",dir_struct().dirs.size(),"not=0");
	for( string dir : dir_struct().dirs )
	{
		if( LogMask[ TEST ])
		{
			 Comment( TEST, "Create: " + dir );
		}
		else
		{
			if( filesystem::create_directories( dir ) )
				Comment( INFO, "Synthesizer directory " + dir + " created");
		}
	}
}
/*
void DirStructure_class::setDir(  )
{


	dir_t dir = dir_struct();

};
*/
void DirStructure_class::Test()
{
	TEST_START( className );
	Set_Loglevel( TEST, true );
	string t = notnull(getenv( "PATH" ));
//	cout << t  << endl;
	assert( t.length() > 0 );
	Create();
	TEST_END( className );
}

DirStructure_class::DirStructure_class() :
	Logfacility_class("DirStructure")
{
	className = Logfacility_class::className;
//	setDir();
};


