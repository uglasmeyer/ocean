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
 * Setup.cpp
 *
 *  Created on: Jul 5, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */



#include <Exit.h>
#include <data/Config.h>

#include <Build.h>

Exit_class				Exit			{};
Config_class			Cfg				{};
Logfacility_class		Log				( Cfg.prgName );
Kbd_base				Kbd 			{};

file_structure* 		Bin				;



void exit_proc( int signal )
{
	Log.Set_Loglevel( DBG2, false );
	exit(0);
}

void Conditional( string question, string action )
{
	Log.Info( action );
	kbdInt_t key = Log.Wait( &Kbd, question, "y/(b)reak?" );
	if( key == 'y' )
		System_execute( action );
	if ( key == 'b' )
	{
		raise(1);
	}
}

void overwrite ( string _src, string _dst )
{
	auto over_write = filesystem::copy_options::overwrite_existing;
	filesystem::copy( _src , _dst, over_write );
};
void init_file( string _src, string _dst )
{
	auto mode = filesystem::copy_options::skip_existing;
	filesystem::copy( _src , _dst, mode );
}

void copy_files( string dir, string filter, string destination )
{
	if ( destination.length() == 0 ) return;
	Log.Comment( DEBUG, "copy files to ", destination );

	vector<string> files = List_directory(dir, filter );
	for(string file : files )
	{
		Log.Info( "copy ", file );
		overwrite( dir + file, destination + file );
	}
}

string deployment_archive = "";
void create_tararchive( source_struct Ss )
{
	fstream Exclude {};
	Exclude.open( Bin->tarexclude_file, fstream::out );
	Exclude.flush() << TESTPRG << "\n"
			".git\n"
			"archive\n"
			"log\n"
			"auto\n"
			"tmp\n"
			"var/ifd*\n"
			"var/StA_data.bin*\n"
			".project\n"
			".settings\n"
			"gmon.out\n"
			"nohup.out\n";
	Exclude.close();

	string arch		= rn_archive;
	string excl		= " --exclude-from=" + Bin->tarexclude_file + " ";

	string cmd1 	= "cd " + Bin->homedir ;
	string cmd2 	= "bash -c \"tar -cvf " + arch + excl + Bin->oceanbasename + "\"";
	string cmd3		= "gzip " + arch;

	string cmd		= cmd1 + ";" + cmd2 + ";" + cmd3;
	Log.Info(  "executing: ", cmd );

	System_execute( cmd );
	string cmd4		= "cksum " + rn_tgz + " >" + rn_cksum;
	string cmd5		= "shasum -a 256 " + rn_tgz + " > " + rn_sha256;
			cmd		= cmd1 + ";" + cmd4 + ";" + cmd5;
	Log.Info(  "executing: ", cmd );

	System_execute( cmd );

}


void bashrc_oceandir()
{
	const string 	bashrc_file 	= Bin->homedir + ".bashrc";
	const string 	tmp_file		= "/tmp/bashrc.tmp";
	fstream 		Bashrc 				{};
					Bashrc.open		( bashrc_file, fstream::in);
	fstream 		Bashtmp 		{};
					Bashtmp.open	( tmp_file, fstream::out );

	// remove ocean.rc from bashrc file
	string 			line 			{};
	while ( getline( Bashrc, line ))
	{
		size_t pos 		= line.find( Bin->oceanrc_filename );
		if ( pos == STRINGNOTFOUND )
		{
			Bashtmp << line << endl;
		}
	};

	// add ocean.rc to bashrc file
	Bashtmp << ". " << Bin->oceanrc_file << endl;
	overwrite( tmp_file, bashrc_file );
}

void compare_environment()
{
	const string env = OCEANDIR;
	const string oceandir_env = notnull( std::getenv( env.data()) );
	const string install_dir	= Bin->installdir;
	if ( not strEqual( install_dir, oceandir_env ) )
	{
		Log.Comment( WARN, "environment              OCEANDIR=", oceandir_env );
		Log.Comment( WARN, "is not equal to install directory=", Bin->installdir );
	}
}
void create_oceanrc()
{
	Log.Info( "Creating the", Bin->oceanrc_file );
	fstream Oceanrc	{};
	Oceanrc.open	( Bin->oceanrc_file, fstream::out );

	Oceanrc << "export " << OCEANDIR << "=" << Bin->installdir << endl;
	Oceanrc << "export ARCH=" << getArch() << endl;
	Oceanrc << "PATH=$(echo $PATH | sed \"s|$OCEANDIR/bin:||g\")" << endl;
	Oceanrc << "LD_LIBRARY_PATH=$(echo $LD_LIBRARY_PATH | sed \"s|$OCEANDIR/lib:||g\")" << endl;
	Oceanrc << "export PATH=$" << OCEANDIR << "/bin/$ARCH:$PATH" << endl;
	Oceanrc << "export LD_LIBRARY_PATH=$" << OCEANDIR << "/lib/$ARCH:$LD_LIBRARY_PATH"<< endl;
}

void symboliclink( string _src, string _sym, string _ext )
{
	Log.Info( "Creating the symbolic link structure");

	auto overwrite_link = [ _src ]( string _link )
	{
		if( filesystem::exists( _link ))
			filesystem::remove( _link );
		filesystem::create_symlink( _src, _link );
	};
	if ( strEqual(_sym, "Synthesizer") )
	{
		overwrite_link( Bin->bindir + "Keyboard");
	}
	string link = Bin->bindir + _sym;
	if( strEqual( _ext, ".so" ) )
	{
		link = Bin->libdir + _sym;
	}
	overwrite_link( link );
}

void install_binary( string _bin, string _ext )
{

	compare_environment();

	filesystem::path 	pwd				{ notnull( getenv("PWD") ) };
	string 				src_bin_file	= pwd.generic_string() + "/" + _bin;
	string				dst_bin_file	= Bin->archbindir + _bin;
	if( strEqual( _ext, ".so" ) )
	{
		dst_bin_file = Bin->archlibdir + _bin;
	}
	Log.Comment( INFO, "Installing binary: ", dst_bin_file );
	overwrite( src_bin_file, dst_bin_file );
	//	symboliclink( dst_bin_file, _bin, _ext ); obsolete
}

void Setup_Test( source_struct Ss )
{
	cout << "Architecture: " << getArch() << endl;
	cout << "Source Dir:   " << Ss.sourcedir << endl;
	Ss.show_installdirs();
}

void ConvertOdt2Pdf( source_struct Ss )
{
	Log.Info( "Converting odt to pdf" );

	string cmd1 = "cd " + Ss.resourcedir;
	string cmd2 =  "libreoffice --headless --convert-to pdf --outdir " +
					Bin->docdir + " " +
					rn_userdoc + ".odt";
	string	cmd = cmd1 + "; " + cmd2;
	Log.Info( "Executing: ", cmd );
	System_execute( cmd );
}

void Copy_3rdpartylibs( source_struct* Src )
{

	Log.Info( "Copying 3rd party libraries" );
	string systemlibdir = "/lib/" + Src->architectur + "-linux-gnu/";
	typedef vector<string> string_vec_t;
	string_vec_t lib_vec = {
		systemlibdir + "libQt6Core.so.6",
		systemlibdir + "libQt6Gui.so.6",
		systemlibdir + "libQt6Widgets.so.6",
		"/usr/local/lib/librtaudio.so.7",
		systemlibdir + "libtinyxml2.so.10",
		systemlibdir + "libtinyxml2.so.11"
	};
	for ( string lib : lib_vec )
	{
		if ( filesystem::exists( lib ) )
		{
			filesystem::copy( lib , Bin->archlibdir, filesystem::copy_options::skip_existing );
		}
		else
		{
			Log.Comment( WARN, "no such file to copy:", lib );
		}
	}
}

void Setup_runtime( source_struct* Src, file_structure* Bin )
{

	Cfg.CreateInstalldirs( );

	Copy_3rdpartylibs( Src );

	init_file ( Src->resourcedir + "Notes/" + Bin->default_nte, Bin->notesdir );
	init_file ( Src->resourcedir + "Instruments/" + Bin->default_snd, Bin->instrumentdir );
	overwrite ( Src->resourcedir + "Instruments/.test2.snd", Bin->instrumentdir );
	init_file ( Src->resourcedir + Bin->bkground_filename, Bin->bkground_file );
	overwrite ( Src->resourcedir + Bin->ipctool_filename, Bin->ipctool_file );
	init_file ( Src->resourcedir + Bin->config_filename	, Bin->config_file );
	overwrite ( Src->resourcedir + Bin->template_xmlname	, Bin->template_xmlfile );
	overwrite ( Src->sourcedir + Bin->install_txt		, Bin->install_txtfile );

	init_file ( Src->resourcedir + Bin->program_filename, Bin->includedir );
	init_file ( Src->resourcedir + Bin->prog_libfilename, Bin->includedir );
	init_file ( Src->resourcedir + Bin->prog_testfilename,Bin->includedir );

	Log.Info( "Using Ocean Base Directory ", Bin->installdir );
	bashrc_oceandir();
	create_oceanrc();

	using filesystem::perms;
	using filesystem::perm_options;

	permissions( Bin->deploy_file	, perms::owner_exec, perm_options::add );
	permissions( Bin->ipctool_file	, perms::owner_exec, perm_options::add );
}

int main(int argc, char **argv)
{
	Cfg.Parse_argv(argc, argv );
	Bin 		= Cfg.fs;

	// CDT install binary
	if( Cfg.Config.filename.length() > 0 )
	{
		Cfg.CreateInstalldirs( );
		filesystem::path path { Cfg.Config.filename };

		install_binary( Cfg.Config.filename, path.extension() );
		exit(0);
	}

	source_struct 	Src 		{ Cfg.Config.sourcedir };
	Build_class 	Build 		{ Src.sourcedir, Bin };

	if ( Cfg.Config.test == 'y' )
	{
		Setup_Test( Src );
	}
	Log.Set_Loglevel( WAIT, true );

	// Setup runtime
	Setup_runtime( &Src, Bin );

	if ( Cfg.Config.Deploy )
	{
		Deploy_class	Deploy		{ Src.sourcedir, Bin };
		Deploy.commit();
		Build.remote( "rio" );
		Build.local();
		// create screen shot
		// test OceanGUI on rio
		// test Synthesizer
		ConvertOdt2Pdf( Src );
		create_tararchive( Src );
		Deploy.Create_ReleaseNotes();
		Deploy.Update_Gitdir();
		Deploy.Github();
		// edit Version.h change version number

	}
	return  0;
}
