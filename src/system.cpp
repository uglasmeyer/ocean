//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * common.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <System.h>

bool has_cmd_processor() // e.g. /bin/sh or cmd.exe
{
	bool ret = system( 0x0 );
	return ret;
}

void System_execute( const string& cmd, bool _noexcept )
{
	coutf;
	int ret = system( cmd.data() );
	if ( _noexcept )
	{
		coutf <<  "WARNING" << cmd << " ignored return value " << ret << endl;
		return;
	}
	if ( ret != 0 )
	{
		Exception( cmd + "\ncheck out system error message " );
	}
}

int System_execute_bg( const string& _cmd)
// starts process _cmd in background and
// returns the process id if sucessful
// otherwise -1
{
	const 	string 	pid_file 	= "/tmp/test.pid";
	const 	string 	cmd 		= "nohup " + _cmd + " &\n jobs -p > " + pid_file;
			string 	retstr		= "";
			int		ret			= -1;

	coutf << "Executing: " << cmd << endl;
	ret = system( cmd.data() );
	ret = -1;
	getline( ifstream( pid_file ), retstr );
	if( retstr.length() > 0 )
		ret		= stoi( retstr );
	filesystem::remove( pid_file );
	return ret;
}

vector<string> List_directory( const string& path, const string& filter )
{
    vector<string> dir_entry_vec{};

	const std::filesystem::path dir{ path };
    if ( not filesystem::exists(dir) )
    {
    	coutf << "ERROR" << "Directory does not exists:\n " << path << endl;
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

bool Is_running_process( const int& pid )
{
	stringstream strs {};
	strs << "/proc/" << dec  <<  pid ;
	bool isdir = filesystem::is_directory( strs.str() ) ;
	return isdir ;
}
bool Is_running_process( const string& pidstr )
{
	int pid = stoi( pidstr );
	return Is_running_process( pid );
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

void Remove_file( string file )
{
	if ( filesystem::exists( file ) )
		filesystem::remove( file);
	if ( filesystem::exists( file ))
		Exception( "cannot remove file: " + file );

}
void Rename_file( string old_name, string new_name )
{
	Remove_file( new_name );
    filesystem::rename( old_name, new_name);

	if ( filesystem::exists( old_name ))
		Exception( "cannot remove file: " + old_name );
}
void System_Test()
{
	Logfacility_class Log_common{"System"};

	Log_common.test_start( "System" );
	Printer_class Printer( true );
	int pid = getpid();
	ASSERTION( Is_running_process( (int)  pid  ), "Process", Is_running_process( (int) pid ), (int)pid) ;
	ASSERTION( not Is_running_process(  -1 ), "Process", Is_running_process(  -1 ), false ) ;
	Printer.Close();
	Log_common.test_end( "System" );

}


