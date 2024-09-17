/*
 * Client.h
 *
 *  Created on: Sep 6, 2024
 *      Author: sirius
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <Synthesizer.h>
#include <Logfacility.h>
#include <Version.h>


typedef struct prgarg_struct
{
		~prgarg_struct(){};
		uint channel 	= 2;  		// -c
		uint rate 		= audio_frames;	// -r
		uint device 	= 0;		// -d
		uint ch_offs 	= 0; 		// -o
		uint shm_key_a 	= rate; 	// -k
		char test 		= 'n';		// -t run sanity check on classes and exit = 'y'
		char dialog		= 'n';		// -d dialog mode of the composer = 'y'
		uint shm_key_b 	= rate + 11;	//
		string Genre	= "Alternative";
		string author	= "U.G.";
		string title	= "Experimental";
		string album	= Application;
		string Term		= "xterm -e ";

} prgarg_struct_t;


class Config_class : virtual Logfacility_class
{
public:

	string basedir = "";
	string configfile = "";
	string prgname		= "";
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

	void Read_synthesizer_config();
	string BaseDir();
	void Parse_argv( int argc, char* argv[] );
	void Show_prgarg_struct(  );
	string Server_cmd( string term, string srv, string opt );
	void Test();
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
	string 			sound_file 		= Dir.etcdir 		+ "test.kbd";
	string 			audio_bin  		= Dir.bindir 		+ Audio_bin;
	string 			synth_bin  		= Dir.bindir 		+ Synth_bin;
	string 			composer_bin	= Dir.bindir		+ Comp_bin;
	string 			ifd_file 		= Dir.libdir 		+ "ifd_data.bin";
	string 			wav_file 		= Dir.musicdir 		+ filename + ".wav";
	string 			mp3_file		= Dir.musicdir 		+ filename + ".mp3";
	string 			raw_file 		= Dir.tmpdir 		+ filename + ".raw";
	string 			counter_file 	= Dir.libdir 		+ "counter.bin";
	string 			config_file  	= Dir.etcdir		+ "synthesizer.cfg";
	string 			program_file	= Dir.includedir 	+ "main.synth";
	const string 	doc_filename 	= "Synthesizer.odt";
	string 			doc_file 		= Dir.docdir 		+ doc_filename;
} dir_struct_t;

/*
const string		Audio_Srv	= file_structure().audio_bin;
const string 		Synthesizer	= file_structure().synth_bin;
const string 		Composer 	= file_structure().composer_bin;
*/

#endif /* CONFIG_H_ */
