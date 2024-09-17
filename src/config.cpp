#include <Config.h>
#include <Common.h>
#include <String.h>

Logfacility_class Log_config("Config");

Config_class::Config_class( string Module ) : Logfacility_class( Module )
{
	BaseDir( );
	Read_synthesizer_config();
};
Config_class::~Config_class()
{

};

void Config_class::Read_synthesizer_config(	 )
{

	std::unordered_map<string, string> 	Get = {}; // @suppress("Invalid template argument")
	Comment( INFO, "using basedir " + basedir );
	configfile = basedir + "/etc/synthesizer.cfg";
	Comment(INFO, "Reading config file " + configfile );

	ifstream cFile( configfile  );

	if (cFile.is_open()) {
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
        Config.title	= Get["title"];
    	Config.author	= Get["author"];
    	Config.album	= Get["album"];
    	Config.Genre 	= Get["genre"];
    	Config.Term 	= Get["term"];
    	string shmkey	= Get["shmkey"]; // @suppress("Invalid arguments")
    	Config.shm_key_a= stoi( shmkey.data() );
    	Config.shm_key_b= Config.shm_key_a+1;

    }
    else
    {

        Comment( ERROR, "Couldn't open config file " + configfile );
        exit(1);
    }
// example:    shm_key_a=stoi( configmap.at("shm_key_a") );

//    std::cout << shm_key_a << endl;

}

void Config_class::Test()
{
	cout << getenv( "$0" );
}

void Config_class::Parse_argv( int argc, char* argv[] )
{
	// https://en.cppreference.com/w/c/language/main_function

	String 			Str{""};
	string 			next{""};

	prgname = argv[0];
	cout << prgname << endl;
	for ( int ndx = 1; ndx < argc; ndx ++ )
	{
		string arg = argv[ ndx ];
		( ndx + 1 == argc  ) ? next = "" : next = argv[ ndx + 1 ];
		if ( arg.compare("-c") == 0 )
			Config.channel	= Str.secure_stoi( next );
		if ( arg.compare("-r") == 0 )
			Config.rate 	= Str.secure_stoi( next );
		if ( arg.compare("-d") == 0 )
			Config.device 	= Str.secure_stoi( next );
		if ( arg.compare("-o") == 0 )
			Config.ch_offs	= Str.secure_stoi( next );
		if ( arg.compare("-k") == 0 )
		{
			Config.shm_key_a= Str.secure_stoi( next );
			Config.shm_key_b= Config.shm_key_a + 1;
		}
		if ( arg.compare("-t") == 0 )
			Config.test 	= 'y';
		if ( arg.compare("-D") == 0 )
			Config.dialog 	= 'y';
	}

}
void Config_class::Show_prgarg_struct( )
{
	stringstream strs{""};
	strs << setw(20) << left << "\nchannel" 	<< dec << Config.channel 	<<endl;  		// -c
	strs << setw(20) << left << "sampline rate" << dec << Config.rate		<<endl;  		// -c
	strs << setw(20) << left << "device nr" 	<< dec << Config.device		<<endl;  		// -d
	strs << setw(20) << left << "channel offs"	<< dec << Config.ch_offs	<<endl; 		// -o
	strs << setw(20) << left << "shm key A" 	<< dec << Config.shm_key_a	<<endl;  		// -k
	strs << setw(20) << left << "shm key B" 	<< dec << Config.shm_key_b	<<endl;  		//
	strs << setw(20) << left << "test classes" 	<< dec << Config.test		<<endl;  		// -t
	strs << setw(20) << left << "dialog mode"	<< dec << Config.dialog		<<endl;  		// -D


	strs << setw(20) << left << "Id3tool Title" << Config.title	<<endl; 		// -o
	strs << setw(20) << left << "Id3tool Author"  << Config.author	<<endl;  		// -k
	strs << setw(20) << left << "Id3tool Album" << Config.album	<<endl;  		//
	strs << setw(20) << left << "Id3tool Genre" << Config.Genre		<<endl;  		// -t
	strs << setw(20) << left << "Terminal" << Config.Term		<<endl;  		// -D
	Comment( WARN, strs.str() );
}

string Config_class::BaseDir()
{
	string pwd = std::getenv( "PWD" );
	if ( filesystem::exists( pwd + "/OceanGUI" ))
	{
		basedir = pwd + "/../";
		return basedir;
	}
	else
	{
		string filename = searchPath( prgname );
		if ( filename.length() > 0 )
		{
			string dir = filesystem::path(filename).root_path();
			basedir = dir + "/../" ;
			return basedir;
		}
	}

	Comment( ERROR, pwd + " is not an Ocean basedir");
	exit(1);

	return string("");
}

string Config_class::Server_cmd( string term, string srv, string opt)
{
	return term + " '(" + srv + " " + opt + ")' &";
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
//	basedir			= Config_class::basedir;

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
		autodir
	};

};

void DirStructure_class::Test()
{
	Set_Loglevel( TEST, true );

}

DirStructure_class::DirStructure_class() :
	Logfacility_class("DirStructure")
{
	setDir();
};


