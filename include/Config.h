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

	vector<string> dirs = // list of directories to be created
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
typedef 			dir_struct 			dir_t;



class Config_class : virtual Logfacility_class
{
	string 			className 			= "";

public:

	string 			basedir 			{""};
	string 			configfile 			{""};
	string 			prgname				{	program_invocation_short_name };
	prgarg_struct	Config 				= prgarg_struct();

					Config_class		( string Module ) ;
	virtual 		~Config_class		();

	void 			Read_config			( string cfgfile );
	void 			Parse_argv			( int argc, char* argv[] );
	void 			Show_Config			(  );
	string 			Server_cmd			( string term, string srv, string opt );
	void 			Test				();

private:
	string 			baseDir				();

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

typedef struct file_structure :
		dir_struct

{
	file_structure() : dir_struct()
	{};
	~file_structure() = default;
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
	const string 	config_file  	= etcdir		+ filename + ".cfg";
	const string	version_txt		= etcdir		+ "version.txt";
	const string	datacfg_file	= etcdir		+ "Data.cfg";
	const string 	counter_file 	= libdir 		+ "counter.bin";
	const string 	program_file	= includedir 	+ "main.synth";
	const string	log_file		= logdir		+ "composer.log";
	const string	nohup_file		= logdir		+ "nohup.log";
	const string 	doc_filename 	= "Ocean.odt";
	const string 	doc_file 		= docdir 		+ doc_filename;
	const string	reclog_file		= tmpdir		+ "session.log";
	const string	session_dump_file=tmpdir		+ "session.lib";

    string get_rec_filename( uint no )
    {
    	return filename + to_string( no ) ;
    }
} dir_struct_t;


#endif /* CONFIG_H_ */
