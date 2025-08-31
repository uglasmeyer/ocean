/*
 * Setup.cpp
 *
 *  Created on: Jul 5, 2025
 *      Author: sirius
 */




#include <Appsymbols.h>

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
void create_tararchive()
{
	fstream Exclude {};
	Exclude.open( fs.tarexclude_file, fstream::out );
	Exclude.flush() << ".git\narchive\nwav/\nlog/\nauto/\ntmp/\nlib/ifd*\nTestprg\n";
	Exclude.close();

	string cmd1 	= "cd " + fs.homedir ;
	string arch		= fs.homedir + "ocean_sound_lab_" + Version_No + ".tar ";
	string excl		= "--exclude-from=" + fs.tarexclude_file + " ";
	string cmd2 	= "bash -c \"tar -cvf " + arch + excl + fs.oceanbasename + "\"";

	Log.Info(  "executing: ", cmd1, ";", cmd2 );
	System_execute( cmd1 + ";" + cmd2 );
}
void bashrc_oceandir()
{
	const string 	bashrc_file 	= fs.homedir + ".bashrc";
	const string 	tmp_file		= "/tmp/bashrc.tmp";
	fstream 		Bashrc 				{};
					Bashrc.open		( bashrc_file, fstream::in);
	fstream 		Bashtmp 		{};
					Bashtmp.open	( tmp_file, fstream::out );

	// remove ocean.rc from bashrc file
	string 			line 			{};
	while ( getline( Bashrc, line ))
	{
		size_t pos 		= line.find( fs.oceanrc_filename );
		if ( pos == STRINGNOTFOUND )
		{
			Bashtmp << line << endl;
		}
	};

	// add ocean.rc to bashrc file
	Bashtmp << ". " << fs.oceanrc_file << endl;
	overwrite( tmp_file, bashrc_file );
}

void compare_environment()
{
	const string env = OCEANDIR;
	const string oceandir_env = notnull( std::getenv( env.data()) );
	const string install_dir	= fs.installdir;
	if ( not strEqual( install_dir, oceandir_env ) )
	{
		Log.Comment( WARN, "environment              OCEANDIR=", oceandir_env );
		Log.Comment( WARN, "is not equal to install directory=", fs.installdir );
	}
}
void create_oceanrc()
{
	fstream Oceanrc	{};
	Oceanrc.open	( fs.oceanrc_file, fstream::out );
	Oceanrc << "export ARCH=`uname -p`" << endl;
	Oceanrc << "export " << oceandir_env << "=" << fs.installdir << endl;
	Oceanrc << "export OCEANTESTCASE=oceantestcase" << endl;
	Oceanrc << "export PATH=" << fs.bindir << ":.:$PATH" << endl;
	Oceanrc << "export LD_LIBRARY_PATH=" << fs.libdir << ":$LD_LIBRARY_PATH"<< endl;
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
		overwrite_link( fs.bindir + "Keyboard");
	}
	string link = fs.bindir + _sym;
	if( strEqual( _ext, ".so" ) )
	{
		link = fs.libdir + _sym;
	}
	overwrite_link( link );
}

void install_binary( string _bin, string _ext )
{

	compare_environment();

	filesystem::path 	pwd				{ notnull( getenv("PWD") ) };
	string 				src_bin_file	= pwd.generic_string() + "/" + _bin;
	string				dst_bin_file	= fs.archbindir + _bin;
	if( strEqual( _ext, ".so" ) )
	{
		dst_bin_file = fs.archlibdir + _bin;
	}
	Log.Comment( INFO, "Installing binary: ", dst_bin_file );
	overwrite( src_bin_file, dst_bin_file );
	symboliclink( dst_bin_file, _bin, _ext );
}
void CreateInstalldirs( )
{
	auto create_dir=[ ]( string _p )
	{
		Log.Comment( DEBUG, "CreateInstalldir: ", _p );
		if ( filesystem::is_directory( _p ))
			return false;
		if ( filesystem::is_regular_file( _p ) )
		{
			filesystem::remove( _p);
		}
		return filesystem::create_directories( _p );
	};

	Log.Comment( DEBUG, "Checking directory structure");
	ASSERTION( (fs.install_dirs.size() != 0 ),"DirStructure_class::Create",
				fs.install_dirs.size(),"not=0");
	for( string dir : fs.install_dirs )
	{
		if ( create_dir( dir ) )
		{
			Log.Comment( BINFO, "Synthesizer directory " + dir + " created");
		}
	}
}

void Setup_Test()
{
	fs.show_installdirs();
}


int main(int argc, char **argv)
{
	bool full_setup = not filesystem::is_directory( fs.installdir );
//	if( full_setup )
	{
		CreateInstalldirs( );
	}
	overwrite ( fs.resourcedir + fs.bkground_filename	, fs.bkg_file );
	overwrite ( fs.resourcedir + fs.setup_filename		, fs.setup_file );
	overwrite ( fs.resourcedir + fs.ipctool_filename	, fs.ipctool_file );
	overwrite ( fs.resourcedir + fs.config_filename		, fs.config_file );
	overwrite ( fs.resourcedir + fs.doc_filename		, fs.doc_file );
	overwrite ( fs.resourcedir + fs.deploy_filename		, fs.deploy_file );
	init_file ( fs.instrumentdir + ".test2.snd"			, fs.resourcedir );
	overwrite ( fs.rc_snd_file							, fs.instrumentdir + fs.default_snd );
	overwrite ( fs.rc_nte_file							, fs.notesdir + fs.default_nte );
	init_file ( fs.program_file							, fs.resourcedir );
	init_file ( fs.prog_libfile							, fs.resourcedir );
	init_file ( fs.prog_testfile						, fs.resourcedir );
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

	Log.Info( "Using Ocean Base Directory ", fs.installdir );
	bashrc_oceandir();
	create_oceanrc();


	copy_files( fs.instrumentdir,
				fs.snd_type,
				fs.resourcedir + "Instruments/" );
	copy_files( fs.notesdir,
				fs.nte_type,
				fs.resourcedir + "Notes/" );
	copy_files( fs.includedir,
				".synth",
				fs.resourcedir );
	copy_files( fs.includedir,
				".inc",
				fs.resourcedir );


	using filesystem::perms;
	using filesystem::perm_options;

	permissions( fs.deploy_file	, perms::owner_exec, perm_options::add );
	permissions( fs.ipctool_file, perms::owner_exec, perm_options::add );


	create_tararchive();

}
