/*
 * common.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */
#include <Logfacility.h>
#include <System.h>

void system_execute( const string& cmd )
{
	std::cout.flush();
	int ret = system( cmd.data() );

	if ( ret != 0 )
	{
		EXCEPTION( cmd + "\ncheck out system error message " );
	}
}

vector<string> List_directory( const string& path, const string& filter )
{
    vector<string> dir_entry_vec{};

	const std::filesystem::path dir{ path };
    if ( not filesystem::exists(dir) )
    {
//    	Log_common.Comment( ERROR, "Directory does not exists:\n " + path );
    	return dir_entry_vec;
    }
    for (auto const& dir_entry : std::filesystem::directory_iterator{ dir })
    {
    	string extension = dir_entry.path().extension();
    	if( strEqual(extension, filter) )
    	{
    		dir_entry_vec.push_back( dir_entry.path().filename() );
    	}
    }
    std::ranges::sort( dir_entry_vec );//.begin(), dir_entry_vec.end() );

    return dir_entry_vec;
}


string searchPath( string file )
{
	string filename = std::filesystem::path( file ).filename( );
	string Path = notnull( getenv( "PATH" )) ;
	if( Path.length() == 0 )
		return "";

	std::replace ( Path.begin(), Path.end(), ':', '\n' );
	istringstream input;
	input.str( Path );
	for (std::string dir; std::getline( input , dir);)
	{
		string file = dir + "/" + filename ; // @suppress("Symbol shadowing")
		if (filesystem::exists( file ) ) return file;
	}

    return string("");
}

void System_Test()
{
	Logfacility_class Log_common{"System"};

	Log_common.test_start( "System" );
	Printer_class Printer( true );
	uint8_t pid = getpid();
	ASSERTION( Is_running_process( (int)  pid  ), "Process", Is_running_process( (int) pid ), true) ;
	ASSERTION( Is_running_process(  to_string( pid ) ), "Process", Is_running_process(  pid ), true) ;
	ASSERTION( not Is_running_process(  -1 ), "Process", Is_running_process(  -1 ), false ) ;
	Printer.Close();
	Log_common.test_end( "System" );

}


