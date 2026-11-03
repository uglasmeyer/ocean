/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * Ocean.h
 *
 *  Created on: Dec 15, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */


#ifndef OCEAN_H
#define OCEAN_H

#include <algorithm>
#include <array>
#include <bitset>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip> // setfill cout ...
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory> // unique_ptr
#include <numeric>
#include <ranges>
#include <set>
#include <experimental/source_location> // Assert
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <signal.h> // signal()
#include <stdarg.h>
#include <stdlib.h> // exit
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>

using std::array;
using std::bitset;
using std::boolalpha;
using std::cin;
using std::clamp;
using std::cout;
using std::dec;
using std::endl;
using std::fstream;
using std::function;
using std::getline;
using std::hex;
using std::ifstream;
using std::istringstream;
using std::left;
using std::list;
using std::numbers::pi;
using std::numeric_limits;
using std::map;
using std::move;
using std::ofstream;
using std::right;
using std::setfill;
using std::setprecision;
using std::set;
using std::setw;
using std::string;
using std::stringstream;
using std::this_thread::sleep_for;
using std::to_string;
using std::thread;
using std::tuple;
using std::unordered_set;
using std::vector;

using std::experimental::source_location;

using std::filesystem::remove;
using std::filesystem::rename;
using std::filesystem::is_directory;
using std::filesystem::exists;
using std::filesystem::copy;
using std::filesystem::copy_options;
using std::filesystem::create_directories;
using std::filesystem::create_symlink;

using std::ranges::for_each;
using std::ranges::sort;
using std::ranges::unique_copy;

// https://en.cppreference.com/w/cpp/language/types
typedef unsigned long int 		buffer_t;
typedef float		 			Data_t; // range -32767 ... +32767
typedef int16_t		 			data_t; // range -32767 ... +32767
typedef double					phi_t;
typedef float 					frq_t;
typedef char					SDSId_t;
typedef long int 				kbdInt_t;
typedef vector<SDSId_t>			SdsId_vec_t;
typedef vector<Data_t>			DataVec_t;
typedef unordered_set<string>	uset_t;

struct 				Stereo_struct
{
	Data_t left;
	Data_t right;
};
typedef Stereo_struct
					Stereo_t;

struct 				stereo_struct
{
	data_t left;
	data_t right;
};
typedef stereo_struct
					stereo_t;

#define NoPID 		-1
#define USEFILENAME 0_uint
#define OVERWRITE	true
#define KEEPFILE	false


const uint8_t		sizeof_stereo		= sizeof(stereo_t);
const uint8_t		sizeof_Stereo		= sizeof(Stereo_t);
const uint8_t		sizeof_Data 		= sizeof(Data_t);
const uint8_t		sizeof_data 		= sizeof(data_t);

const uint			sample_rate			= 48000; //device default [frames / sec]
const buffer_t		frames_per_sec  	= sample_rate;
const buffer_t		frames_per_msec		= frames_per_sec / 1000 ;
const phi_t 		dt 					= 1.0 / ( frames_per_sec );	//seconds per frame

const uint8_t		max_slide_sec		= 8; // see dynamic
const uint16_t		stepduration_msec	= 250;
const uint8_t		max_sec 			= 2;
const uint16_t		max_msec	 		= max_sec * 1000;
const uint16_t		min_msec			= 125;
const int16_t		measure_parts		= (int16_t) (max_msec / min_msec); // = 16

const buffer_t		max_frames			= max_msec * frames_per_msec;
const buffer_t		min_frames			= min_msec * frames_per_msec;	// [frames]
const buffer_t		record_size 		= min_frames; // frames per record

const buffer_t		audio_frames 		= min_frames;
const buffer_t 		chunksize			= audio_frames / 2;

const uint			recduration 		= 3*60; // seconds
const uint			tmpduration 		= 30; 	// temp memory storage 30*frames_per_sec
const uint			kbdduration			= 2;
const buffer_t 		recordmemory_bytes 	= recduration*frames_per_sec * sizeof_Data; // 3 minutes 32Mb
const buffer_t 		monobuffer_bytes   	= max_frames * sizeof_Data;
const buffer_t 		audiobuffer_bytes   = audio_frames * sizeof_Data;
const buffer_t		Stereoaudio_bytes	= audio_frames * sizeof_Stereo;
const buffer_t		stereoaudio_bytes	= audio_frames * sizeof_stereo;
const Data_t		max_data_amp		= 4096*4;

const uint8_t		osc_default_volume	= 75; // %
const float			percent				= 0.01;

const frq_t			oct_base_freq 		= 16.3516;//C0
const int			oct_steps			= 12;
const uint8_t		oct_reference		= 3;
const uint			max_octave			= 7;
const uint 			min_octave 			= 0;
const string		OctChars_EN			= "CcDdEFfGgAaB";
const string		OctChars_DE			= "CcDdEFfGgAaH";

enum StateId_e : uint8_t
{
	OFFLINE		,
	RUNNING		,
	DEFAULT		,
	EXITSERVER	,
	RECORDSTART	,
	RECORDSTOP	,
	RECORDING	,
	STOPPING	,
	STARTING	,
	INACTIVE	,
	SLEEPING	,
	ENABLED		,
	DISABLED	,
	STATEID_SIZE
};

#include <Range.h>

const range_T<buffer_t>	frames_range		{ 0, max_frames };
const range_T<uint>		duration_range		{ min_msec, max_msec };
const range_T<int>		octave_range		{ min_octave, max_octave };
const range_T<int> 		frqslide_range		{ 0, ( max_slide_sec * 1000 ) / stepduration_msec };


#endif /* OCEAN_H */
