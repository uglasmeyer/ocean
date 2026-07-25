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
 * system.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <System.h>
#include <fstream>
#include <iostream>

extern Logfacility_class Log;

bool has_cmd_processor() // e.g. /bin/sh or cmd.exe
{
	bool ret = system( 0x0 );
	return ret;
}

int System_execute( const string& cmd, bool _noexcept )
{
	coutf;
	int ret = system( cmd.data() );
	if ( ret != 0 )
	{
		if ( _noexcept )
		{
			Log.Comment( ERROR, "executed with error" );
			return ret;
		}
		Exception( cmd + "\ncheck out system error message " );
	}
	return 0;
}

int System_execute_bg( const string& _cmd, string log1 )
// starts process _cmd in background and
// returns the process id if sucessful
// otherwise -1
{
	const 	string 	pid_file 	= "/tmp/test.pid";
	const	string	cmd_nolog	= "nohup " +
									_cmd +
									" &\njobs -p > " +
									pid_file;
	const 	string 	cmd_log		= "nohup " +
									_cmd +
									" 2>&1 >> " +
									log1 +
									" &\njobs -p > " +
									pid_file;
			string 	retstr		= "";
			int		ret			= -1;

	string 			cmd			= cmd_nolog;
	if( log1.length() > 0 )
		cmd = cmd_log;
	coutf << "Executing: " << cmd << endl;
	ret = system( cmd.data() );
	ret = -1;
	getline( ifstream( pid_file ), retstr );
	if( retstr.length() > 0 )
		ret		= stoi( retstr );
	filesystem::remove( pid_file );
	return ret;
}


bool Is_running_process( const int& pid )
{
	stringstream strs {};
	strs << "/proc/" << dec  <<  pid ;
	bool isdir = filesystem::is_directory( strs.str() ) ;
	return isdir ;
}

void Touch_file( string file )
{
	// create empty file
	fstream	FILE{ file, fstream::out };
	FILE	<< "" ;
	FILE.close();
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

size_t loadData( string file, void* data, size_t bytes )
{	// copy file to memory

	auto filesize = [ file ]( )
	{
	    filesystem::path filePath = file;
	    if( filesystem::exists( filePath ) )
	    	return filesystem::file_size( filePath );
	    else
	    	return size_t( 0 );
	};

	if( file.length() == 0 )
		return 0;
	if( not data )
		return 0;
	if( not filesystem::exists( file ) )
		return 0;
	if ( bytes == 0 )
		bytes = filesize();
	if ( bytes == 0)
		return 0;

	ifstream		fd	{ file, ios::binary };
	fd.read( reinterpret_cast<char*>( data ), bytes );
	fd.close();

	return bytes;
}

bool dumpData( string file, void* data, size_t bytes )
{	// copy memory to file

	auto dir = [ file ]()
	{
		filesystem::path path { file };
		return path.root_path();
	};

	if( file.length() == 0 )
		return false;
	if( not data )
		return false;
	if( not filesystem::exists( dir() ))
		return false;
	Remove_file( file );

	std::fstream		FD	{ file, ios_base::binary|ios_base::out };
	try
	{
		FD.write( reinterpret_cast<char*>( data ), bytes );
	}
	catch( const std::exception& e )
	{
		coutf << e.what() << endl;
		return false;
	}
	return true;
}

bool Write_template_text( string file, string text, bool overwrite )
{

	if( filesystem::exists( file ) )
		if( not overwrite )
			return true;

	fstream FILE { file, fstream::out };
	FILE << text;
	if( Log.Ignore_error_set.contains( errno ) )
		return true;

	Log.Comment( ERROR, "System" );
	Exception( "Cannot write default " + file );
	return false;

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


/**************************************************
 * dircontent_struct
 *************************************************/
bool is_pathvec( string_vec_t dirs )
{
	for( filesystem::path path : dirs )
		if ( not filesystem::exists( path ) )
		{
			Log.Comment( ERROR, path.generic_string(), "is not a directory" );
			return false;
		}
	return true;
}
dircontent_struct::dircontent_struct( string_vec_t dirs, string _ext )
	: searchPath( dirs ), ext( _ext )
{
	this->select_strong	= false;
	if ( not is_pathvec	( dirs ) )
		Log.Comment( WARN, "search path", show_type( searchPath), "partly exists" );
	updateContent		();
};
void dircontent_struct::UpdatePath( string_vec_t dirs )
{
	searchPath			= dirs;
	updateContent		();
}
bool dircontent_struct::Is_empty()
{
	return ( dir_entry_vec.size() == 0 );
}
bool dircontent_struct::Is_file( string name )
{
	string file = Getfile( name );
	return ( file.length() > 0 );
}
void dircontent_struct::updateContent()
{
	dir_entry_vec.clear	();
	filename_vec.clear	();

	if( searchPath.size() == 0 )
		return;

	list_directories();

	// make unique entries in the filename vector
	std::ranges::sort	( filename_vec );
    auto last 			= std::unique(filename_vec.begin(), filename_vec.end());
    filename_vec.erase	(last, filename_vec.end());
}

void dircontent_struct::Create( string name )
{
	if( searchPath.size() < 2 )					// searchPath must be subdir, basedir
		Exception( "search path length" );

	string	file0	= searchPath[0] + name + ext;
	if( not filesystem::exists( file0 ) )		//if not in subdir
	{
		string file1 = searchPath[1] + name + ext;
		if( not filesystem::exists( file1 ) )	//if not in basedir
		{
			Touch_file( file0 );
		}
		else
		{
			if( strEqual( file1, file0 ) )
				return;
			filesystem::copy( file1, file0 );	// create from template in basedir
		}
		updateContent();
	}
	// nothing to do
}

string dircontent_struct::Select( string& name, const string& default_name )
{	// return dir/name.extension
	if( name.length() == 0 )
		name = default_name;

	string selected = Getfile( name );
	if( selected.length() == 0 )
	{
		if( select_strong )
		{
			string filenames = show_type( filename_vec );
			Log.Comment( INFO, name, "not in", filenames );
			Exception( "No selection" );
		}

		name = default_name;
		selected = Getfile( name );
	}

	if( selected.length() == 0 )
	{
		Log.Comment( ERROR, default_name + ext, "not in", show_type( searchPath ), "but" );
		Exception( show_type( dir_entry_vec ));
	}
	return selected;
}
string dircontent_struct::Select_strong( string& name, const string& default_name )
{
	select_strong	= true;
	string select 	= Select( name, default_name );
	select_strong 	= false;
	return select	;
}

string dircontent_struct::Getfile( string name )
{
	for( string& file : dir_entry_vec )
	{
		if( file.find( "/" + name + ext ) < STRINGNOTFOUND )
			return file;
	}

	Log.Comment( ERROR, "filename", name, ext, "not in", show_type( searchPath ) );
	Log.Comment( WARN, "Inconsistent file structure" );
	return "";
}

// fill filename_vec and dir_enty_vec
void dircontent_struct::list_directories()
{
	filesystem::path fspath{};
	for( string dir : searchPath )
	{
		fspath = filesystem::path( dir );
	    if ( filesystem::is_directory( fspath ) )
	    {
	        for (auto const& dir_entry : filesystem::directory_iterator{ fspath })
	        {
	        	string extension = dir_entry.path().extension();
	        	if( strEqual( extension, ext) )
	        	{
	        		filename_vec.push_back( dir_entry.path().stem() );
	    			dir_entry_vec.push_back( dir_entry.path() );
	        	}
	        }
	    }
	    else
	    {
	    	Log.Comment( ERROR, "Directory does not exists:", dir );
	    }
	}
}
