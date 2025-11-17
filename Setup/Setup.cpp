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
 * Setup.cpp
 *
 *  Created on: Jul 5, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */




#include <Appsymbols.h>
#include <ReleaseNotes.h>

file_structure* fs = Cfg.fs;

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
void create_tararchive()
{
	fstream Exclude {};
	Exclude.open( fs->tarexclude_file, fstream::out );
	Exclude.flush() << TESTPRG << "\n"
			".git\n"
			"archive\n"
			"log\n"
			"auto\n"
			"tmp\n"
			"lib/ifd*\n"
			".project\n"
			".settings\n"
			"gmon.out\n";
	Exclude.close();

	string arch		= rn_archive;
	string excl		= " --exclude-from=" + fs->tarexclude_file + " ";

	string cmd1 	= "cd " + fs->homedir ;
	string cmd2 	= "bash -c \"tar -cvf " + arch + excl + fs->oceanbasename + "\"";
	string cmd3		= "gzip " + arch;

	string cmd		= cmd1 + ";" + cmd2 + ";" + cmd3;
	Log.Info(  "executing: ", cmd );

	System_execute( cmd );

	deployment_archive = arch + ".gz";

	string cmd4		= "cksum " + deployment_archive + " >" + deployment_archive + ".cksum";
	string cmd5		= "shasum -a 256 " + deployment_archive +" > " + deployment_archive + ".sha256";
			cmd		= cmd1 + ";" + cmd4 + ";" + cmd5;
	Log.Info(  "executing: ", cmd );

	System_execute( cmd );


	string 	tag		= "v" + Version_No;
			cmd		= "gh release create " + tag + " " +
					deployment_archive + " " +
					deployment_archive + ".cksum " +
					deployment_archive + ".sha256 " +
					"--title ocean_sound_lab_" + tag +
					" --notes-file " + fs->git_dir + "RELEASE_NOTES.md";
	Log.Info		( cmd );

}

string get_check_sum( string _type )
	{
		string cksum_file = fs->homedir + rn_tgz + _type;
		fstream Cksum_file { cksum_file, fstream::in };
		string cksum;
		getline( Cksum_file, cksum  );
		return cksum + "\n";
	};


void Create_ReleaseNotes()
{
	string releasenotes_file = fs->git_dir + rn_filename;
	fstream Release_notes { releasenotes_file, fstream::out };

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
	const string 	bashrc_file 	= fs->homedir + ".bashrc";
	const string 	tmp_file		= "/tmp/bashrc.tmp";
	fstream 		Bashrc 				{};
					Bashrc.open		( bashrc_file, fstream::in);
	fstream 		Bashtmp 		{};
					Bashtmp.open	( tmp_file, fstream::out );

	// remove ocean.rc from bashrc file
	string 			line 			{};
	while ( getline( Bashrc, line ))
	{
		size_t pos 		= line.find( fs->oceanrc_filename );
		if ( pos == STRINGNOTFOUND )
		{
			Bashtmp << line << endl;
		}
	};

	// add ocean.rc to bashrc file
	Bashtmp << ". " << fs->oceanrc_file << endl;
	overwrite( tmp_file, bashrc_file );
}

void compare_environment()
{
	const string env = OCEANDIR;
	const string oceandir_env = notnull( std::getenv( env.data()) );
	const string install_dir	= fs->installdir;
	if ( not strEqual( install_dir, oceandir_env ) )
	{
		Log.Comment( WARN, "environment              OCEANDIR=", oceandir_env );
		Log.Comment( WARN, "is not equal to install directory=", fs->installdir );
	}
}
void create_oceanrc()
{
	fstream Oceanrc	{};
	Oceanrc.open	( fs->oceanrc_file, fstream::out );
	Oceanrc << "export " << OCEANDIR << "=$1" << endl;
	Oceanrc << "export PATH=$" << OCEANDIR << "/bin:$PATH" << endl;
	Oceanrc << "export LD_LIBRARY_PATH=$" << OCEANDIR << "/lib:$LD_LIBRARY_PATH"<< endl;
}

void symboliclink( string _src, string _sym, string _ext )
{
	auto overwrite_link = [ _src ]( string _link )
	{
		if( filesystem::exists( _link ))
			filesystem::remove( _link );
		filesystem::create_symlink( _src, _link );
	};
	if ( strEqual(_sym, "Synthesizer") )
	{
		overwrite_link( fs->bindir + "Keyboard");
	}
	string link = fs->bindir + _sym;
	if( strEqual( _ext, ".so" ) )
	{
		link = fs->libdir + _sym;
	}
	overwrite_link( link );
}

void install_binary( string _bin, string _ext )
{

	compare_environment();

	filesystem::path 	pwd				{ notnull( getenv("PWD") ) };
	string 				src_bin_file	= pwd.generic_string() + "/" + _bin;
	string				dst_bin_file	= fs->archbindir + _bin;
	if( strEqual( _ext, ".so" ) )
	{
		dst_bin_file = fs->archlibdir + _bin;
	}
	Log.Comment( INFO, "Installing binary: ", dst_bin_file );
	overwrite( src_bin_file, dst_bin_file );
	symboliclink( dst_bin_file, _bin, _ext );
}

void Setup_Test()
{
	fs->show_installdirs();
}

void ConvertOdt2Pdf( )
{
	Log.Info( "Converting odt to pdf" );

	string cmd1 = "cd " + fs->resourcedir;
	string cmd2 =  "libreoffice --headless --convert-to pdf --outdir " +
					fs->docdir + " " +
					rn_userdoc + ".odt";
	string	cmd = cmd1 + "; " + cmd2;
	Log.Info( "Executing: ", cmd );
	System_execute( cmd );
}

void Copy_3rdpartylibs()
{
	typedef vector<string> string_vec_t;
	string_vec_t lib_vec = {
		"/lib/x86_64-linux-gnu/libQt6Core.so.6",
		"/lib/x86_64-linux-gnu/libQt6Gui.so.6",
		"/lib/x86_64-linux-gnu/libQt6Widgets.so.6",
		"/usr/local/lib/librtaudio.so.7",
		"/lib/x86_64-linux-gnu/libtinyxml2.so.11"
	};
	for ( string lib : lib_vec )
	{
		if ( filesystem::exists( lib ) )
		{
			filesystem::copy( lib , fs->libdir, filesystem::copy_options::skip_existing );
		}
		else
		{
			Exception( "no such file to copy: " + lib );
		}
	}
}


int main(int argc, char **argv)
{

	bool full_setup = not filesystem::is_directory( fs->installdir );
	Cfg.CreateInstalldirs( );
	overwrite ( fs->resourcedir + fs->bkground_filename	, fs->bkground_file );
	overwrite ( fs->resourcedir + fs->setup_filename		, fs->setup_file );
	overwrite ( fs->resourcedir + fs->ipctool_filename	, fs->ipctool_file );
	overwrite ( fs->resourcedir + fs->config_filename		, fs->config_file );
	overwrite ( fs->resourcedir + fs->deploy_filename		, fs->deploy_file );
	init_file ( fs->instrumentdir + ".test2.snd"			, fs->resourcedir );
	overwrite ( fs->rc_snd_file							, fs->instrumentdir + fs->default_snd );
	overwrite ( fs->rc_nte_file							, fs->notesdir + fs->default_nte );
	init_file ( fs->program_file							, fs->resourcedir );
	init_file ( fs->prog_libfile							, fs->resourcedir );
	init_file ( fs->prog_testfile						, fs->resourcedir );
	overwrite ( fs->resourcedir + fs->template_xmlname	, fs->template_xmlfile );
	overwrite ( fs->git_dir + fs->install_txt		, fs->install_txtfile );

	Cfg.Parse_argv(argc, argv );
	if ( Cfg.Config.test == 'y' )
	{
		Setup_Test();
		exit(0);
	}

	if( Cfg.Config.filename.length() > 0 )
	{
		filesystem::path path { Cfg.Config.filename };

		install_binary( Cfg.Config.filename, path.extension() );
		if ( not full_setup )
			exit(0);
	}

	Log.Info( "Using Ocean Base Directory ", fs->installdir );
	bashrc_oceandir();
	create_oceanrc();

	// update git resources
	copy_files( fs->instrumentdir,
				fs->snd_type,
				fs->resourcedir + "Instruments/" );
	copy_files( fs->notesdir,
				fs->nte_type,
				fs->resourcedir + "Notes/" );
	copy_files( fs->includedir,
				".synth",
				fs->resourcedir );
	copy_files( fs->includedir,
				".inc",
				fs->resourcedir );


	using filesystem::perms;
	using filesystem::perm_options;

	permissions( fs->deploy_file	, perms::owner_exec, perm_options::add );
	permissions( fs->ipctool_file, perms::owner_exec, perm_options::add );

	ConvertOdt2Pdf( );

	Copy_3rdpartylibs();

	create_tararchive();

	Create_ReleaseNotes();
	return  0;
}
