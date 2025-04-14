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

struct dir_struct
{
	const string homedir 		= notnull( getenv("HOME") ) + "/";
	const string basedir 		= homedir + "OceanBase/";
	const string etcdir 		= basedir + "etc/";
	const string bindir  		= basedir + "bin/";
	const string libdir  		= basedir + "lib/";
	const string tmpdir  		= basedir + "tmp/";
	const string vardir			= basedir + "var/";
	const string autodir 		= vardir  + "auto/";
	const string musicdir 		= vardir  + "wav/";
	const string docdir 		= basedir + "doc/";
	const string rtspdir		= etcdir  + "rtsp/";
	const string instrumentdir	= etcdir  + "Instruments/";
	const string notesdir  		= etcdir  + "Notes/";
	const string includedir		= etcdir  + "include/";
	const string xmldir			= etcdir  + "musicxml/";
	const string logdir			= tmpdir ;

	vector<string> dirs =
	{
		basedir,
		docdir,
		etcdir,
		bindir,
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

};
typedef dir_struct dir_t;

class Config_class : virtual Logfacility_class
{
public:

	string basedir 		{""};
	string configfile 	{""};
	string prgname		{""};
	prgarg_struct		Config 	= prgarg_struct();


	typedef struct Server_struct
	{
		string cmd( string term, string srv, string opt )
		{
			return term + " '(" + srv + " " + opt + ")' &";
		};
	} Server_struc_t;

	Config_class( string Module ) ;
	virtual ~Config_class();

	void Read_config( string cfgfile );
	void Parse_argv( int argc, char* argv[] );
	string Show_Config(  );
	string Server_cmd( string term, string srv, string opt );
	void Test();

private:
	string baseDir();

};

class DirStructure_class : virtual Logfacility_class
{
	string className = "";
public:


	void Create();
//	void setDir( );
	void Test();

	DirStructure_class();
	virtual ~DirStructure_class(){};

private:
//	vector <string> dirs = dir_struct().dirs;
};

typedef struct file_structure
{
	dir_t Dir = dir_struct();

	const string 	filename 		= "synthesizer";
	const string 	snd_type 		= ".snd";	// instruments file extension
	const string	nte_type		= ".nte";	// notes file extension
	const string 	wav_type 		= ".wav";
	const string 	xml_type		= ".musicxml";
	const string	Audio_bin		= "AudioServer";
	const string 	Synth_bin		= "Synthesizer";
	const string 	Composer_bin	= "Composer";
	const string	Ocean_bin		= "OceanGUI";
	const string 	Rtsp_bin		= "rtsp";
	const string 	Comstack_bin	= "Comstack";
	const string 	audio_bin  		= Dir.bindir 		+ Audio_bin;
	const string 	synth_bin  		= Dir.bindir 		+ Synth_bin;
	const string 	composer_bin	= Dir.bindir		+ Composer_bin;
	const string 	comstack_bin	= Dir.bindir		+ Comstack_bin;
	const string 	ocean_bin		= Dir.bindir		+ Ocean_bin;
	const string 	rtsp_bin		= Dir.bindir		+ Rtsp_bin;
	const string 	ifd_file 		= Dir.libdir 		+ "ifd_data.bin";
	const string 	wav_file 		= Dir.musicdir 		+ filename + wav_type;
	const string 	mp3_file		= Dir.musicdir 		+ filename + ".mp3";
	const string 	raw_file 		= Dir.tmpdir 		+ filename + ".raw";
	const string 	config_file  	= Dir.etcdir		+ filename + ".cfg";
	const string	version_txt		= Dir.etcdir		+ "version.txt";
	const string	datacfg_file	= Dir.etcdir		+ "Data.cfg";
	const string 	counter_file 	= Dir.libdir 		+ "counter.bin";
	const string 	program_file	= Dir.includedir 	+ "main.synth";
	const string	log_file		= Dir.logdir		+ "composer.log";
	const string	nohup_file		= Dir.logdir		+ "nohup.log";
	const string 	doc_filename 	= "Ocean.odt";
	const string 	doc_file 		= Dir.docdir 		+ doc_filename;
	const string	reclog_file		= Dir.tmpdir		+ "session.log";
	const string	session_dump_file=Dir.tmpdir		+ "session.lib";

    string get_rec_filename( uint no )
    {
    	return filename + to_string( no ) ;
    }
} dir_struct_t;


#endif /* CONFIG_H_ */
