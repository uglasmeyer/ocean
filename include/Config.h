/*
 * Client.h
 *
 *  Created on: Sep 6, 2024
 *      Author: sirius
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <Version.h>

static const uint MAXCONFIG 	= 3;
typedef	array<key_t, MAXCONFIG>	keys_arr_t;

typedef struct prgarg_struct
{
		~prgarg_struct(){};
		uint channel 	= 2;  		// -c
		uint rate 		= audio_frames;	// -r
		uint device 	= 0;		// -d
		uint ch_offs 	= 0; 		// -o
		char test 		= 'n';		// -t run sanity check on classes and exit = 'y'
		char dialog		= 'n';		// -d dialog mode of the composer = 'y'
		char composer	= 'n';		// rtsp -C
		char oceangui	= 'n';		// rtsp -G
		string Genre	= "Alternative";
		string author	= "U.G.";
		string title	= "Experimental";
		string album	= Application;
		string Term		= "xterm -e ";
		string ffmpeg 	= "ffmpeg";
		key_t 	shm_key_l 	= rate; 	// -k
		key_t 	shm_key_r 	= rate+1; 	//
		key_t	SDS_key		= 66529;
		uint	SDS_id		= 0;
		key_t	Sem_key		= 11234;
		keys_arr_t 	sds_arr = {};
		string 		appcfg	= "S0.cfg";  // TODO name of Synthesizer instances
} prgarg_struct_t;


class Config_class : virtual Logfacility_class
{
public:

	string basedir 		{""};
	string configfile 	{""};
	string prgname		{""};
	prgarg_struct		Config = prgarg_struct();


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
	void Show_prgarg_struct(  );
	string Server_cmd( string term, string srv, string opt );
	void Test();

private:
	string baseDir();

};




class DirStructure_class : virtual Logfacility_class
{
public:

	string homedir 		= "/home/sirius/";
	string basedir 		= homedir + "Synthesizer/";
	string etcdir 		= basedir + "etc/";
	string bindir  		= basedir + "bin/";
	string libdir  		= basedir + "lib/";
	string tmpdir  		= basedir + "tmp/";
	string vardir		= basedir + "var/";
	string autodir 		= vardir  + "auto/";
	string musicdir 	= vardir  + "wav/";
	string docdir 		= basedir + "doc/";
	string rtspdir		= etcdir + "rtsp/";
	string instrumentdir= etcdir + "Instruments/";
	string notesdir  	= etcdir + "Notes/";
	string includedir	= etcdir + "include/";

	void Create();
	void setDir( );
	void Test();

	DirStructure_class();
	virtual ~DirStructure_class(){};

private:
	vector <string> dirs {};
};

typedef struct file_structure
{
	DirStructure_class Dir{};

	const string 	filename 		= "synthesizer";
	const string 	file_type 		= ".kbd";
	const string 	wav_file_type 	= ".wav";
	const string	Audio_bin		= "AudioServer";
	const string 	Synth_bin		= "Synthesizer";
	const string 	Comp_bin		= "Composer";
	const string	Ocean_bin		= "OceanGUI";
	const string 	Rtsp_bin		= "rtsp";
	string 			sound_file 		= Dir.etcdir 		+ "test.kbd";
	string 			audio_bin  		= Dir.bindir 		+ Audio_bin;
	string 			synth_bin  		= Dir.bindir 		+ Synth_bin;
	string 			composer_bin	= Dir.bindir		+ Comp_bin;
	string 			ocean_bin		= Dir.bindir		+ Ocean_bin;
	string 			rtsp_bin		= Dir.bindir		+ Rtsp_bin;
	string 			ifd_file 		= Dir.libdir 		+ "ifd_data.bin";
	string 			wav_file 		= Dir.musicdir 		+ filename + ".wav";
	string 			mp3_file		= Dir.musicdir 		+ filename + ".mp3";
	string 			raw_file 		= Dir.tmpdir 		+ filename + ".raw";
	string 			config_file  	= Dir.etcdir		+ filename + ".cfg";
	string			datacfg_file	= Dir.etcdir		+ "Data.cfg";
	string 			counter_file 	= Dir.libdir 		+ "counter.bin";
	string 			program_file	= Dir.includedir 	+ "main.synth";
	const string 	doc_filename 	= "Ocean.odt";
	string 			doc_file 		= Dir.docdir 		+ doc_filename;
} dir_struct_t;


#endif /* CONFIG_H_ */
