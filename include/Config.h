/*
 * Client.h
 *
 *  Created on: Sep 6, 2024
 *      Author: sirius
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <Logfacility.h>
#include <Configbase.h>
#include <System.h>

struct install_struct
:	virtual public 	Logfacility_class
{
	const string 		homedir 		= notnull( getenv("HOME") ) + "/";
	filesystem::path 	pwd				{ notnull( getenv("PWD") ) };

	const string		git_dir			= gitDir();
	const string 		resourcedir		= resourceDir();
	const string		architectur		= getArch();
	const string		installdir	 	= read_installfile();
	const vector<string>files			= { git_dir		, resourcedir,
											architectur	, installdir };
	string				getArch		();
	string				baseDir			();
	string 				read_installfile();
	string				gitDir			();
	string 				resourceDir		();
	void 				show_installdirs();

						install_struct	() ;
	virtual				~install_struct	()
							{ DESTRUCTOR( className ); };

};
struct dir_struct
	: virtual install_struct
{
	const string 	etcdir 			= installdir + "etc/";
	const string 	bindir  		= installdir + "bin/";
	const string 	libdir  		= installdir + "lib/";
	const string 	tmpdir  		= installdir + "tmp/";
	const string 	docdir 			= installdir + "doc/";
	const string 	vardir			= installdir + "var/";
	const string	archbindir		= bindir  + architectur;
	const string	archlibdir		= libdir  + architectur;
	const string 	autodir 		= vardir  + "auto/";
	const string 	musicdir 		= vardir  + "wav/";
	const string 	rtspdir			= etcdir  + "rtsp/";
	const string 	instrumentdir	= etcdir  + "Instruments/";
	const string 	notesdir  		= etcdir  + "Notes/";
	const string 	includedir		= etcdir  + "include/";
	const string 	xmldir			= etcdir  + "musicxml/";
	const string 	logdir			= tmpdir ;

	vector<string> install_dirs = // list of directories to be created
	{
		installdir,
		docdir,
		etcdir,
		bindir,
		archbindir,
		archlibdir,
		libdir,
		logdir,
		tmpdir,
		vardir,
		musicdir,
		instrumentdir,
		notesdir,
		includedir,
		autodir,
		rtspdir,
		xmldir
	};
			dir_struct	();
	virtual	~dir_struct	() = default;

};

struct file_structure
	: virtual dir_struct
{
	string 			get_rec_filename( uint no );
    string 			baseName 		( string _dir );
					file_structure	();
	virtual 		~file_structure	() = default;

	const string 	filename 		= "synthesizer";
	string			oceanbasename	= baseName( installdir );
	const string 	snd_type 		= ".snd";	// instruments file extension
	const string	nte_type		= ".nte";	// notes file extension
	const string 	wav_type 		= ".wav";
	const string 	xml_type		= ".musicxml";
	const string	Audio_bin		= AUDIOSERVER;
	const string 	Synth_bin		= SYNTHESIZER;
	const string 	Keyboard_bin	= SYNTHKBD;
	const string 	Composer_bin	= COMPOSER;
	const string	Ocean_bin		= OCEANGUI;
	const string 	Rtsp_bin		= RTSP;
	const string 	Comstack_bin	= SDSVIEW;
	const string 	audio_bin  		= bindir 		+ Audio_bin;
	const string 	synth_bin  		= bindir 		+ Synth_bin;
	const string 	composer_bin	= bindir		+ Composer_bin;
	const string 	comstack_bin	= bindir		+ Comstack_bin;
	const string 	ocean_bin		= bindir		+ Ocean_bin;
	const string 	rtsp_bin		= bindir		+ Rtsp_bin;
	const string 	ifd_file 		= libdir 		+ "ifd_data.bin";
	const string 	wav_file 		= musicdir 		+ filename + wav_type;
	const string 	mp3_file		= musicdir 		+ filename + ".mp3";
	const string 	raw_file 		= tmpdir 		+ filename + ".raw";
	const string	config_filename	= filename		+ ".cfg";
	const string 	config_file  	= etcdir		+ config_filename;
	const string	version_txt		= etcdir		+ "version.txt";
	const string	datacfg_file	= etcdir		+ "Data.cfg";
	const string 	counter_file 	= libdir 		+ "counter.bin";
	const string 	program_file	= includedir 	+ "main.synth";
	const string	prog_libfile	= includedir	+ "lib.inc";
	const string	prog_testfile	= includedir	+ "testcases.inc";
	const string	log_file		= logdir		+ "composer.log";
	const string	nohup_file		= logdir		+ "nohup.log";
	const string 	doc_filename 	= "Ocean.odt";
	const string 	doc_file 		= docdir 		+ doc_filename;
	const string 	bkground_filename= "Ocean.png";
	const string	bkg_file		= libdir		+ bkground_filename;
	const string	setup_filename	= "oceansetup.sh";
	const string	setup_file		= bindir		+ setup_filename;
	const string	ipctool_filename= "clearipc.sh";
	const string	ipctool_file	= bindir		+ ipctool_filename;
	const string	deploy_filename = "oceandeploy.sh";
	const string	deploy_file		= homedir		+ deploy_filename;
	const string	reclog_file		= tmpdir		+ "session.log";
	const string	session_dump_file=tmpdir		+ "session.lib";
	const string 	tarexclude_file	= "/tmp/tarexclude";
	const string	oceanrc_filename= "ocean.rc";
	const string	oceanrc_file 	= etcdir		+ oceanrc_filename;
	const string	default_nte		= "default" + nte_type;
	const string	rc_nte_file		= resourcedir + "Notes/" + default_nte;
	const string	default_snd		= "default" + snd_type;
	const string	rc_snd_file		= resourcedir + "Instruments/" + default_snd;
	const string	kbdnotes_name	= "keyboardNotes";
	const string	kbdnotes_file	= autodir + kbdnotes_name + nte_type;
};

class Config_class
:	Logfacility_class
{
	string 			className 			= "";

public:

	string 			configfile 			{ };
	string 			prgname				{ program_invocation_short_name };
	prgarg_t		Config 				= prgarg_struct();

	void 			Read_config			( string cfgfile );
	void 			Parse_argv			( int argc, char* argv[] );
	void 			Show_Config			(  );
	string 			Server_cmd			( string term, string srv, string opt );
	void 			Test				();

					Config_class		( string Module ) ;
	virtual 		~Config_class		();
};





#endif /* CONFIG_H_ */
