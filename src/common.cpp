/*
 * common.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */
#include <Common.h>
#include <Version.h>
#include <algorithm>
#include <ranges>
using namespace std;

Logfacility_class Log_common{"System"};


void Wait( long int N)
{
	usleep( N ); //microseconds
}


prgarg_struct_t parse_argv( int argc, char* argv[] )
{
	// https://en.cppreference.com/w/c/language/main_function

	prgarg_struct_t params;
	String 			Str{""};
	string 			next{""};

	for ( int ndx = 1; ndx < argc; ndx ++ )
	{
		string arg = argv[ ndx ];
		( ndx + 1 == argc  ) ? next = "" : next = argv[ ndx + 1 ];
		if ( arg.compare("-c") == 0 )
			params.channel	= Str.secure_stoi( next );
		if ( arg.compare("-r") == 0 )
			params.rate 	= Str.secure_stoi( next );
		if ( arg.compare("-d") == 0 )
			params.device 	= Str.secure_stoi( next );
		if ( arg.compare("-o") == 0 )
			params.ch_offs	= Str.secure_stoi( next );
		if ( arg.compare("-k") == 0 )
		{
			params.shm_key_a= Str.secure_stoi( next );
			params.shm_key_b= params.shm_key_a + 11;
		}
		if ( arg.compare("-t") == 0 )
			params.test 	= 'y';
		if ( arg.compare("-D") == 0 )
			params.dialog 	= 'y';
	}

	return params;
}

void show_prgarg_struct( prgarg_struct_t args )
{
	stringstream strs{};
	strs << setw(20) << left << "channel" << dec << args.channel 		<<endl;  		// -c
	strs << setw(20) << left << "sampline rate" << dec << args.rate		<<endl;  		// -c
	strs << setw(20) << left << "device nr" << dec << args.device		<<endl;  		// -d
	strs << setw(20) << left << "channel offs" << dec << args.ch_offs	<<endl; 		// -o
	strs << setw(20) << left << "shm key A" << dec << args.shm_key_a	<<endl;  		// -k
	strs << setw(20) << left << "shm key B" << dec << args.shm_key_b	<<endl;  		//
	strs << setw(20) << left << "test classes" << dec << args.test		<<endl;  		// -t
	strs << setw(20) << left << "dialog mode" << dec << args.dialog		<<endl;  		// -D
	Log_common.Comment( INFO, strs.str() );
}

void system_execute( const string& cmd )
{
	std::cout.flush();
	int ret = system( cmd.data() );
	if ( ret != 0 )
	{
		Log_common.Comment( ERROR, cmd);
		Log_common.Comment( ERROR, "check out system error message ");
		Exception( );//raise( SIGINT );
	}
}

void Exception( )
{
	cout << "A common exception occurred " << endl;
	cout << "See above the detail, or visit the Synthesizer.log file for more information" <<endl;
	cout << Log_common.Line << endl;
	cout << "Press <Ctrl>d to enter the common exit procedure" << endl;
	string s;
	cin >> s;
	raise( SIGINT );
}

void show_items( const auto& arr )
{
	for(string item : arr )
		cout << item << " ";
	cout << endl;
}

bool cmpstr( const string& a, const string& b )
{
	return ( a.compare( b ) == 0 );
}


void DirStructure_class::Create()
{
	Comment(INFO,"Checking directory structure");

	for( string dir : dirs )
	{
		if( filesystem::create_directories( dir ) )
			Log_common.Comment( INFO, "Synthesizer directory " + dir + " created");
	}
}

void Config_class::read_synthesizer_config(	 )
{
	ifstream cFile( file_structure().config_file );

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
                Get[Name.Str]= value;
            }
        }
    }
    else
    {
        Comment(ERROR, "Couldn't open config file ");
    }
	return ;
// example:    shm_key_a=stoi( configmap.at("shm_key_a") );

//    std::cout << shm_key_a << endl;

}

vector_str_t List_directory( const string& path, const string& filter )
{
    vector_str_t dir_entry_vec{};

	const std::filesystem::path dir{ path };
    if ( not filesystem::exists(path) )
    {
    	Log_common.Comment( ERROR, "Directory does not exists:\n " + path );
    	return dir_entry_vec;
    }
    for (auto const& dir_entry : std::filesystem::directory_iterator{ dir })
    {
    	string extension = dir_entry.path().extension();
    	if( cmpstr(extension, filter) )
    	{
    		dir_entry_vec.push_back( dir_entry.path().filename() );
    	}
    }
    sort( dir_entry_vec.begin(), dir_entry_vec.end() );

    return dir_entry_vec;
}
