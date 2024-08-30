/*
 * synthesizer.h
 *
 *  Created on: Dec 15, 2023
 *      Author: sirius
 */
#ifndef SYNTHESIZER_H_
#define SYNTHESIZER_H_


#include <algorithm>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip> // setfill cout ...
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <random>
#include <ranges>
#include <sstream>
#include <string>
#include <thread>
#include <typeinfo>
#include <unordered_map>
#include <vector>


#include <assert.h>
#include <inttypes.h>
#include <math.h> // sin(), rint()
#include <signal.h> // signal()
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h> // usleep



using namespace std;

struct dir_struct
{
	 const string homedir 		= "/home/sirius/";
	 const string basedir 		= homedir + "Synthesizer/";
	 const string etcdir 		= basedir + "etc/";
	 const string bindir  		= basedir + "bin/";
	 const string libdir  		= basedir + "lib/";
	 const string tmpdir  		= basedir + "tmp/";
	 const string vardir		= basedir + "var/";
	 const string autodir 		= vardir  + "auto/";
	 const string musicdir 		= vardir  + "wav/";
	 const string logdir 		= vardir  + "log/";
	 const string docdir 		= basedir + "doc/";
	 const string instrumentdir	= etcdir + "Instruments/";
	 const string notesdir  	= etcdir + "Notes/";
	 const string includedir	= etcdir + "include/";

};

	// files
typedef struct file_structure
{
	const string filename 		= "synthesizer";
	const string notes_type 	= "kbd";
	const string sound_file 	= dir_struct().etcdir 		+ "test.kbd";
	const string audio_bin  	= dir_struct().bindir 		+ "AudioServer";
	const string synth_bin  	= dir_struct().bindir 		+ "Synthesizer";
	const string composer_bin	= dir_struct().bindir		+ "Composer";
	const string ifd_file 		= dir_struct().libdir 		+ "ifd_data.bin";
	const string wav_file 		= dir_struct().musicdir 	+ filename + ".wav";
	const string mp3_file		= dir_struct().musicdir 	+ filename + ".mp3";
	const string raw_file 		= dir_struct().tmpdir 		+ filename + ".raw";
	const string counter_file 	= dir_struct().libdir 		+ "counter.bin";
	const string log_file 		= dir_struct().logdir 		+ "Synthesizer.log";
	const string config_file  	= dir_struct().etcdir		+ "synthesizer.cfg";
	const string main_file 		= dir_struct().includedir 	+ "main.synth";
	const string doc_filename 	= "Synthesizer.odt";
	const string doc_file 		= dir_struct().docdir 		+ doc_filename;
} dir_struct_t;

#include <Logfacility.h>
#include <String.h>





typedef struct StA_status_struct // memory array status
{
	bool 	play			= false; // play this memory array
	bool 	store			= false; // record into this memory array
} StA_status_t;

typedef struct mixer_status_struct // mixer status
{
	bool 	play			= false; // explicite sync mode
	bool 	notes			= false; // play notes
	bool 	external		= false; // external play or record
	bool	mute			= false; // mute master volume
	bool	kbd				= false; // play keyboard note
} mixer_status_t;


typedef std::unordered_map<string,string>
							config_map_t ;
// https://en.cppreference.com/w/cpp/language/types
typedef unsigned long int 	buffer_t;
typedef float		 		Data_t; // range -32767 ... +32767
typedef signed short 		data_t; // range -32767 ... +32767

typedef struct stereo_struct
{
	data_t left;
	data_t right;
} stereo_t;

const uint 			sizeof_data 		= sizeof(Data_t);
const double 		pi					= 3.1415926536;//3.141592654;


const buffer_t		audio_frames 		= 48000; //2*chunksize * 43 ; // ~44100
const buffer_t		frames_per_sec  	= 48000;

const uint8_t		max_sec 			= 1;//8;
const uint16_t		max_milli_sec 		= max_sec * 1000;
const buffer_t		max_frames			= max_sec * audio_frames;

const uint			recduration 		= 3*60; // seconds
const uint			tmpduration 		= 30; // temp memory storage 30*frames_per_sec
const buffer_t 		recordmemory_bytes 	= recduration*frames_per_sec*sizeof(Data_t); // 3 minutes 32Mb
const buffer_t 		monobuffer_size   	= max_frames * sizeof(Data_t);
const buffer_t		stereobuffer_size 	= recduration*frames_per_sec * sizeof(stereo_t);
const buffer_t 		sharedbuffer_size 	= max_frames * sizeof(stereo_t );
const Data_t		max_data_amp		= 4096*4;
const uint8_t 		oct_base_freq 		= 55;

const uint			osc_default_volume	= 80; // %
const uint 			wavedisplay_len		= 512;

typedef struct bps_struct
{
	const vector<string>	Bps_str_vec = { "0","1","2","4","5","8"}; 		// Beats per second
	const vector<int>   	Bps_array 	= { 0,1,2,4,5,8 };
	string getbps_str( int id )
	{
		return Bps_str_vec[ id ];
	}
	int getbps_id( string str )
	{
		for ( size_t i = 0; i < Bps_str_vec.size(); i++ )
			if ( Bps_str_vec[i].compare( str ) == 0 )
				return (int)i;
		return -1;
	}
	uint8_t getbps( int id )
	{
		return Bps_array[ id ];
	}

} bps_struct_t;

const float			LFO_limit			= 1.0;
const uint8_t		LFO_count			= 100;

typedef struct Server_struct
{

	string shell = "xterm -e ";
	string cmd( string srv, string opt )
	{
		return shell + "'(" + srv + " " + opt + ")' &";
	};
} Server_struc_t;

const string		Audio_Srv	= file_structure().audio_bin;
const string 		Synthesizer	= file_structure().synth_bin;
const string 		Composer 	= file_structure().composer_bin;

const vector<string> wavedisplay_str_vec =
{
    "Audio Out",
	"MAIN",
    "VCO ",
    "FMO ",
    "External IN"
};

const vector<string> wavedisplay_type_str_vec =
{
	"Full",
	"Flow",
	"Debug"
};
enum { FULLID, FLOWID, DEBUGID };

extern void system_execute( const string );


#endif /* SYNTHESIZER_H_ */
