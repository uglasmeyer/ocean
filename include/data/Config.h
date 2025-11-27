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
 * Client.h
 *
 *  Created on: Sep 6, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_CONFIG_H_
#define DATA_CONFIG_H_

#include <data/Configbase.h>
#include <Logfacility.h>
#include <System.h>

struct source_struct
	:	virtual public 	Logfacility_class
{
	const string 		HOME 			= notnull( getenv("HOME") );
	string				sourcedir		;//= sourceDir();
	string 				resourcedir		;//= resourceDir();
	vector<string>		files			;//= { resourcedir, archdir };
	const string		architectur		= getArch();
	const string		archdir			= architectur + "/";
						source_struct	( string srcdir );
	string				getArch			();
	string 				read_installfile();
	string 				resourceDir		();
	void 				show_installdirs();

};

struct install_struct
:	virtual public 	Logfacility_class
{
	const string 		homedir 		= notnull( getenv("HOME") ) + "/";
	filesystem::path 	pwd				{ notnull( getenv("PWD") ) };

	const string		installdir	 	= notnull( getenv("OCEANDIR"));//read_installfile();
	const string		ARCH			= notnull( getenv("ARCH"));
	string				oceanDir		();

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
	const string	archbindir		= bindir  + ARCH + "/";
	const string	archlibdir		= libdir  + ARCH + "/";
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
	string 			get_rec_filename			( uint no );
    string 			baseName 					( string _dir );
					file_structure				();
	virtual 		~file_structure				() = default;

	const string 	filename 					= "synthesizer";
	string			oceanbasename				= baseName( installdir );
	const string 	snd_type 					= ".snd";	// instruments file extension
	static constexpr string
					nte_type					= ".nte";	// notes file extension
	const string 	wav_type 					= ".wav";
	static constexpr string
					xml_type					= ".musicxml";
	const string	Audio_bin					= AUDIOSERVER;
	const string 	Synth_bin					= SYNTHESIZER;
	const string 	Keyboard_bin				= SYNTHKBD;
	const string 	Composer_bin				= COMPOSER;
	const string	Ocean_bin					= OCEANGUI;
	const string 	Rtsp_bin					= RTSP;
	const string 	Comstack_bin				= SDSVIEW;
	const string 	audio_bin  					= bindir 		+ Audio_bin;
	const string 	synth_bin  					= bindir 		+ Synth_bin;
	const string 	composer_bin				= bindir		+ Composer_bin;
	const string 	comstack_bin				= bindir		+ Comstack_bin;
	const string 	ocean_bin					= bindir		+ Ocean_bin;
	const string 	rtsp_bin					= bindir		+ Rtsp_bin;
	const string 	ifd_file 					= libdir 		+ "ifd_data.bin";
	const string 	wav_file 					= musicdir 		+ filename + wav_type;
	const string 	mp3_file					= musicdir 		+ filename + ".mp3";
	const string 	raw_file 					= tmpdir 		+ filename + ".raw";
	const string	config_filename				= filename		+ ".cfg";
	const string 	config_file  				= etcdir		+ config_filename;
	const string	version_txt					= etcdir		+ "version.txt";
	const string	datacfg_file				= etcdir		+ "Data.cfg";
	const string 	counter_file 				= libdir 		+ "counter.bin";
	const string 	program_file				= includedir 	+ "main.synth";
	const string	prog_libfile				= includedir	+ "lib.inc";
	const string	prog_testfile				= includedir	+ "testcases.inc";
	const string	log_file					= logdir		+ "composer.log";
	const string	nohup_file					= logdir		+ "nohup.log";
	const string 	bkground_filename			= "Ocean.png";
	const string	bkground_file				= libdir		+ bkground_filename;
	const string	setup_filename				= "oceansetup.sh";
	const string	setup_file					= bindir		+ setup_filename;
	const string	ipctool_filename			= "clearipc.sh";
	const string	ipctool_file				= bindir		+ ipctool_filename;
	const string	deploy_filename 			= "oceandeploy.sh";
	const string	deploy_file					= homedir		+ deploy_filename;
	const string	reclog_file					= tmpdir		+ "session.log";
	const string	session_dump_file			= tmpdir		+ "session.lib";
	const string 	tarexclude_file				= "/tmp/tarexclude";
	const string	oceanrc_filename			= "ocean.rc";
	const string	oceanrc_file 				= etcdir	+ oceanrc_filename;
	const string	kbdnotes_name				= "keyboardNotes";
	const string	kbdnotes_file				= autodir + kbdnotes_name + nte_type;
	const string	template_xmlname			= "template" + xml_type;
	const string	template_xmlfile			= xmldir + template_xmlname;
	const string	install_txt					= "install.txt";
	const string	install_txtfile				= installdir + install_txt;
	const string	default_name				= "default";
	const string	default_nte					= default_name + nte_type;
	const string	default_snd					= default_name + snd_type;
};

class Config_class
:	Logfacility_class
{
	string 			className 			= "";

public:
	process_t		Process				{};
	file_structure	Fs					{};
	file_structure*	fs					= &Fs;
	string 			configfile 			{};
	string 			prgName				{};
	prgarg_t		Config 				= prgarg_struct();

	void 			CreateInstalldirs	();
	void 			Read_config			( string cfgfile );
	void 			Parse_argv			( int argc, char* argv[] );
	void 			Show_Config			( bool debug = true );
	string 			Server_cmd			( string term, string srv, string opt );
	void 			Test				();

					Config_class		() ;
	virtual 		~Config_class		();

private:
	vector_str_t	parse_cmdline		();

};





#endif /* DATA_CONFIG_H_ */
