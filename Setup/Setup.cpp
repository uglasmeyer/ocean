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

file_structure* 		Bin				;



void exit_proc( int signal )
{
	Log.Set_Loglevel( DBG2, false );
	exit(0);
}

void overwrite ( string _src, string _dst )
{
	if ( filesystem::is_regular_file( _dst ) )
		filesystem::remove( _dst );
	if ( filesystem::exists( _src ) )
		filesystem::copy( _src , _dst );
	else
	{
		Log.Comment( ERROR, "no such file:", _src );
		exit(0);
	}
};
void init_file( string _dst_file, string _rc_dir )
{
	filesystem::path path { _dst_file };
	string filename = path.filename();
	string src_file = _rc_dir + filename;
	Log.Comment( DEBUG, "init demo file: ", _dst_file );

	if( filesystem::is_directory( _rc_dir ) )
	{
		if( filesystem::is_regular_file( src_file ) )
		{
			if( not filesystem::exists( _dst_file ) )
			{
				filesystem::copy( src_file, _dst_file );
			}
			else
				Log.Comment( DEBUG, _dst_file, " already exists");
		}
		else
			Log.Comment( ERROR, src_file, " is not a regular file");

	}
	else
		Log.Comment( ERROR, _rc_dir, " is not a directory");
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


	string 	tag		= "v" + Version_No;
			cmd		= "gh release create " + tag + " " +
					deployment_archive + " " +
					deployment_archive + ".cksum " +
					deployment_archive + ".sha256 " +
					"--title ocean_sound_lab_" + tag +
					" --notes-file " + Ss.sourcedir + rn_filename;
	Log.Info		( cmd );

}

string get_check_sum( string _type )
	{
		string cksum_file = Bin->homedir + rn_tgz + _type;
		fstream Cksum_file { cksum_file, fstream::in };
		string cksum;
		getline( Cksum_file, cksum  );
		return cksum + "\n";
	};


void Create_ReleaseNotes( source_struct Ss )
{
	string releasenotes_file = Ss.sourcedir + rn_filename;
	Log.Info( "Creating the Release note file", releasenotes_file);

	ofstream Release_notes { releasenotes_file };//, fstream::out };

	const string rn_verification =
	R"(Verification
	- verify cksum (as provided by packager):
	)"  +
	get_check_sum( ".cksum" ) +
	"- verify SHA-256 locally: \n" +
	get_check_sum( ".sha256" ) +
	"- Verify: shasum -a 256 -c ocean_sound_lab_4.0.2-37.tar.gz.sha256" + rn_newline;

	Release_notes 	<< rn_title
					<< rn_summary
					<< rn_include
					<< rn_changes
					<< rn_installation
					<< rn_verification
					<< rn_license
					<< rn_notes
					<< rn_changelog;

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
void create_oceanrc( source_struct Ss )
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

//#include <System.h>
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

void Copy_3rdpartylibs( source_struct Ss )
{

	Log.Info( "Copying 3rd party libraries" );
	string systemlibdir = "/lib/" + Ss.architectur + "-linux-gnu/";
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


int main(int argc, char **argv)
{
	Cfg.Parse_argv(argc, argv );

	source_struct 	Src 		{ Cfg.Config.sourcedir };
					Bin 		= Cfg.fs;
	Build_class 	Build 		{ Src.sourcedir, Bin };
	Deploy_class	Deploy		{ Src.sourcedir };

	if ( Cfg.Config.test == 'y' )
	{
		Setup_Test( Src );
//		Build.remote( "rio" );
		Build.local();
		exit(0);
	}
	bool full_setup = not filesystem::is_directory( Bin->installdir );

	if( Cfg.Config.filename.length() > 0 )
	{
		filesystem::path path { Cfg.Config.filename };

		install_binary( Cfg.Config.filename, path.extension() );
		if ( not full_setup )
			exit(0);
	}

	const string	rc_nte_file					= Src.resourcedir + "Notes/" + Bin->default_nte;
	const string	rc_snd_file					= Src.resourcedir + "Instruments/" + Bin->default_snd;


	Cfg.CreateInstalldirs( );

	overwrite ( Src.resourcedir + Bin->bkground_filename	, Bin->bkground_file );
//	overwrite ( Src.resourcedir + Bin->setup_filename		, Bin->setup_file );
	overwrite ( Src.resourcedir + Bin->ipctool_filename	, Bin->ipctool_file );
	overwrite ( Src.resourcedir + Bin->config_filename	, Bin->config_file );
	overwrite ( Src.resourcedir + Bin->deploy_filename	, Bin->deploy_file );
	init_file ( Bin->instrumentdir + ".test2.snd"		, Src.resourcedir + "Instruments/" );

	overwrite ( rc_snd_file								, Bin->instrumentdir + Bin->default_snd );
	overwrite ( rc_nte_file								, Bin->notesdir + Bin->default_nte );
	init_file ( Bin->program_file						, Src.resourcedir );
	init_file ( Bin->prog_libfile						, Src.resourcedir );
	init_file ( Bin->prog_testfile						, Src.resourcedir );
	overwrite ( Src.resourcedir + Bin->template_xmlname	, Bin->template_xmlfile );
	overwrite ( Src.sourcedir + Bin->install_txt		, Bin->install_txtfile );


	Log.Info( "Using Ocean Base Directory ", Bin->installdir );
	bashrc_oceandir();
	create_oceanrc( Src );

	// update git resources
	copy_files( Bin->instrumentdir,
				Bin->snd_type,
				Src.resourcedir + "Instruments/" );
	copy_files( Bin->notesdir,
				Bin->nte_type,
				Src.resourcedir + "Notes/" );
	copy_files( Bin->includedir,
				".synth",
				Src.resourcedir );
	copy_files( Bin->includedir,
				".inc",
				Src.resourcedir );

	using filesystem::perms;
	using filesystem::perm_options;

	permissions( Bin->deploy_file	, perms::owner_exec, perm_options::add );
	permissions( Bin->ipctool_file	, perms::owner_exec, perm_options::add );

	Copy_3rdpartylibs( Src );

	if( Cfg.Config.archive == 'y' )
	{
		ConvertOdt2Pdf( Src );
		create_tararchive( Src );
		Create_ReleaseNotes( Src );
		Deploy.Update_Gitdir();
	}
	return  0;
}
