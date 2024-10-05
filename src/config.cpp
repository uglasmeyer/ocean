#include <Config.h>
#include <String.h>
#include <System.h>



Config_class::Config_class( string Module ) :
	Logfacility_class( Module )
{
	prgname = program_invocation_name;
	Comment( INFO, "Program name: " + prgname );
	baseDir( );
	Read_config( file_structure().config_file );
	Read_config( file_structure().datacfg_file);
};

Config_class::~Config_class()
{

};

void Config_class::Read_config(	string cfgfile )
{
	std::unordered_map<string, string> 	Get = {}; // @suppress("Invalid template argument")

	String Str("");
	configfile = cfgfile;
	Comment(INFO, "Reading config file " + configfile );

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

			if( tmp.length() == 0 )
				return old;
			else
			{
				return tmp;
			}
		};
	auto get_value = [ &Get ]( key_t old, string str )
		{
			String Str("");
			string tmp = string( Get[ str ]);
			if( tmp.length() == 0 )
				return old;
			else
			{
				return (key_t) Str.to_int( tmp );
			}
		};
	string getstr = "";
	Config.title	= get_item( Config.title, "title" );
	Config.author	= get_item( Config.author, "author" );
	Config.album	= get_item( Config.album, "album" );
	Config.Genre 	= get_item( Config.Genre, "genre" );
	Config.Term 	= get_item( Config.Term, "term" );
	Config.ffmpeg 	= get_item( Config.ffmpeg, "ffmpeg" );
	getstr = "";
	getstr.push_back( Config.test );
	getstr			= get_item( getstr , "test");
	Config.test		= (getstr.length() == 0 ) ? Config.test : getstr[0];
	Config.appcfg	= get_item( Config.appcfg, "appcfg" );
	Config.shm_key_l= get_value ( Config.shm_key_l, "shmkey" );
	Config.shm_key_r= Config.shm_key_l+1;
	Config.SDS_key	= get_value ( Config.SDS_key, "sdskey" );
	Config.Sem_key	= get_value ( Config.Sem_key, "semkey" );
	for( uint n = 0; n < MAXCONFIG; n++  )
	{
		string str = "sds" + to_string(n);
		Config.sds_arr[ n ] = get_value( 0, str );
	}

	Show_prgarg_struct();
	assert( ( Config.test == 'n' ) or ( Config.test == 'y' ) or (Config.test == 0 )) ;
// example:    shm_key_a=stoi( configmap.at("shm_key_a") );

//    std::cout << shm_key_a << endl;

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
		if ( arg.compare("-K") == 0 )
			Config.SDS_id = Str.to_int( next );
		if ( arg.compare("-t") == 0 )
			Config.test 	= 'y';
		if ( arg.compare("-D") == 0 )
			Config.dialog 	= 'y';
		if ( arg.compare("-S") == 0 )
			Config.appcfg = next;
	}

}
void Config_class::Show_prgarg_struct( )
{
	stringstream strs{""};
	strs << setw(20) << left << "\nchannel" 	<< dec << 	Config.channel 	<<endl;  		// -c
	strs << setw(20) << left << "sampline rate" << dec << 	Config.rate		<<endl;  		// -c
	strs << setw(20) << left << "device nr" 	<< dec << 	Config.device	<<endl;  		// -d
	strs << setw(20) << left << "channel offs"	<< dec << 	Config.ch_offs	<<endl; 		// -o
	strs << setw(20) << left << "shm key R" 	<< dec << 	Config.shm_key_l<<endl;  		// -k
	strs << setw(20) << left << "shm key L" 	<< dec << 	Config.shm_key_r<<endl;  		//
	strs << setw(20) << left << "sds_key" 		<< 			Config.SDS_key	<<endl;  		// -D
	strs << setw(20) << left << "sem_key" 		<< 			Config.Sem_key	<<endl;  		// -D
	strs << setw(20) << left << "test classes" 	<<  	 	Config.test		<<endl;  		// -t
	strs << setw(20) << left << "dialog mode"	<<  		Config.dialog	<<endl;  		// -D
	strs << setw(20) << left << "composer"		<<  		Config.composer	<<endl;  		// -D
	strs << setw(20) << left << "oceangui"		<< 			Config.oceangui	<<endl;  		// -D
	strs << setw(20) << left << "Id3tool Title" << 			Config.title	<<endl; 		// -o
	strs << setw(20) << left << "Id3tool Author"<< 			Config.author	<<endl;  		// -k
	strs << setw(20) << left << "Id3tool Album" << 			Config.album	<<endl;  		//
	strs << setw(20) << left << "Id3tool Genre" << 			Config.Genre	<<endl;  		// -t
	strs << setw(20) << left << "Terminal" 		<< 			Config.Term		<<endl;  		// -D
	strs << setw(20) << left << "ffmpeg" 		<< 			Config.ffmpeg	<<endl;  		// -D
	strs << setw(20) << left << "appcfg" 		<< 			Config.appcfg	<<endl;
	strs << setw(20) << left << "sds keys"		<< show_items( Config.sds_arr ) << endl;

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
		Comment( INFO, "using " + Envvar + basedir );
		return basedir;
	}
	Comment( INFO, "not in " + Envvar );

	string file = searchPath( prgname );
	filesystem::path filepath = file;
	string parentpath = filepath.parent_path();
	filesystem::path cfgname = parentpath + "/../etc/synthesizer.cfg";
	cout << file << endl;
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

string Config_class::Server_cmd( string term, string srv, string opt)
{
	string cmd = "";
	if ( term.length() == 0 )
	{
		cmd = srv;
		cmd.append(" ");
		cmd.append( opt );
		cmd.append( " &" );
	}
	else
	{
		cmd = term;
		cmd.append( " '");
		cmd.append(srv),
		cmd.append(" ");
		cmd.append(opt);
		cmd.append("' &");
	}
	Comment( INFO, "command:" + cmd );

	return cmd;
}



void DirStructure_class::Create()
{
	Comment(INFO,"Checking directory structure");

	for( string dir : dirs )
	{
		if( Log[ TEST ])
		{
			 Comment( TEST, "test simulating dir " + dir );
		}
		else
		{
			if( filesystem::create_directories( dir ) )
				Comment( INFO, "Synthesizer directory " + dir + " created");
		}
	}
}

void DirStructure_class::setDir(  )
{

	etcdir 			= basedir + "etc/";
	bindir  		= basedir + "bin/";
	libdir  		= basedir + "lib/";
	tmpdir  		= basedir + "tmp/";
	vardir			= basedir + "var/";
	autodir 		= vardir  + "auto/";
	musicdir 		= vardir  + "wav/";
	logdir 			= "/tmp/log/";
	docdir 			= basedir + "doc/";
	instrumentdir	= etcdir + "Instruments/";
	notesdir  		= etcdir + "Notes/";
	includedir		= etcdir + "include/";
	rtspdir			= etcdir + "rtsp/";
	dirs =
	{
		homedir,
		basedir,
		etcdir,
		bindir,
		libdir,
		tmpdir,
		vardir,
		musicdir,
		instrumentdir,
		notesdir,
		includedir,
		autodir,
		rtspdir
	};

};

void DirStructure_class::Test()
{
	Set_Loglevel( TEST, true );
//	string t = getenv_str( "PATH" );
//	cout << t  << endl;
//	assert( t.length() > 0 );

}

DirStructure_class::DirStructure_class() :
	Logfacility_class("DirStructure")
{
	setDir();
};


