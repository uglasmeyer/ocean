/*
 * Ocean.h
 *
 *  Created on: Dec 15, 2023
 *      Author: sirius
 */
#ifndef OCEAN_H
#define OCEAN_H


#include <algorithm>
#include <array>
#include <cstdlib>
#include <complex>
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
#include <set>
#include <experimental/source_location> // Assert
#include <stdexcept>
#include <sstream>
#include <string>
#include <string_view> // Assert
#include <thread>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <math.h> // sin(), rint()
#include <signal.h> // signal()
#include <stdarg.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>



using namespace std;

// https://en.cppreference.com/w/cpp/language/types
typedef unsigned long int 	buffer_t;
typedef float		 		Data_t; // range -32767 ... +32767
typedef signed short 		data_t; // range -32767 ... +32767
typedef vector<Data_t>		DataVec_t;
typedef double				phi_t;
struct stereo_struct
{
	data_t left;
	data_t right;
};
typedef stereo_struct 		stereo_t;

const uint 			sizeof_Data 		= sizeof(Data_t);
const uint 			sizeof_data 		= sizeof(data_t);
const phi_t 		pi					= numbers::pi;//3.1415926536;//3.141592654;

const uint			sample_rate			= 48000; //device dependent fix
const buffer_t		frames_per_sec  	= sample_rate;

const uint8_t		max_sec 			= 2;
const uint16_t		max_milli_sec 		= max_sec * 1000;
const uint16_t		min_milli_sec		= 250;

const buffer_t		max_frames			= max_sec * frames_per_sec;
const buffer_t		min_frames			= frames_per_sec * min_milli_sec / 1000;
const buffer_t		audio_frames 		= frames_per_sec; // chunksize * 100

const uint			recduration 		= 3*60; // seconds
const uint			tmpduration 		= 30; 	// temp memory storage 30*frames_per_sec
const buffer_t 		recordmemory_bytes 	= recduration*frames_per_sec * sizeof_Data; // 3 minutes 32Mb
const buffer_t 		monobuffer_size   	= max_frames * sizeof_Data;
const Data_t		max_data_amp		= 4096*4;

const uint			osc_default_volume	= 75; // %
const float			oct_base_freq 		= 16.3516;//27.5/2.0 = C0

template< typename T >
struct range_t
{
		T min ;
		T max ;
};

template< typename T>
T check_range( range_t<T> r, T val )
{
	if( val < r.min )
		return r.min;
	if (val > r.max )
		return r.max;
	return val;
}


typedef struct osc_struct
{
	const vector<string> roles =
	{
		"Instrument",
		"Notes",
		"Keyboard",
		"External",
		"Audio"
	};
	enum { INSTRID, NOTESID, KBDID, EXTID, AUDIOID };

	const vector<string> types =
	{
		"VCO",
		"FMO",
		"OSC"
	};
	enum { VCOID, FMOID, OSCID };


} osc_roles_t;

enum { STEP, FIXED, SLIDE }; // frequency and volume change mode

const uint				oct_steps		= 12;
const uint				max_octave		= 6;
const uint 				min_octave 		= 0;
const string			OctChars		= "CcDdEFfGgAaB";

#define ASSERTION(	 expr , message, input, expected )\
	printf( "test: %s\n", (message) );\
	if ( not (expr) ) \
	{\
	printf( "file: ( %s ) line: ( %d ) in function: ( %s )\n", __FILE__, __LINE__, __func__ );\
	cout 	<< message 							<< '\n'\
			<< "input    value: " << (input) 		<< '\n'\
			<< "expected value: " << dec << (expected) 	<< endl;\
	raise( SIGINT ); \
	};


#endif /* OCEAN_H */
