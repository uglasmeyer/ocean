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
#include <complex>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip> // setfill cout ...
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory> // unique_ptr
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
#include <stdlib.h> // exit
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
typedef float 				frq_t;
typedef char				Id_t;
typedef vector<Id_t>		SdsId_vec_t;
typedef long int 			kbdInt_t;

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


const size_t		sizeof_stereo		= sizeof(stereo_t);
const size_t		sizeof_Stereo		= sizeof(Stereo_t);
const uint 			sizeof_Data 		= sizeof(Data_t);
const uint 			sizeof_data 		= sizeof(data_t);
const uint			sample_rate			= 48000; //device default
const buffer_t		frames_per_sec  	= sample_rate;
const buffer_t		frames_per_msec		= frames_per_sec / 1000 ;

const uint8_t		max_sec 			= 2;
const uint16_t		max_msec	 		= max_sec * 1000;
const uint16_t		min_msec			= 125;
const uint16_t		measure_parts		= (uint16_t) (max_msec / min_msec); // = 16

const buffer_t		max_frames			= max_msec * frames_per_msec;
const buffer_t		min_frames			= min_msec * frames_per_msec;

const buffer_t		audio_frames 		= min_frames;
const buffer_t 		chunksize			= audio_frames / 2;

const uint			recduration 		= 3*60; // seconds
const uint			tmpduration 		= 30; 	// temp memory storage 30*frames_per_sec
const uint			kbdduration			= 2;
const buffer_t 		recordmemory_bytes 	= recduration*frames_per_sec * sizeof_Data; // 3 minutes 32Mb
const buffer_t 		monobuffer_bytes   	= max_frames * sizeof_Data;
const Data_t		max_data_amp		= 4096*4;

const uint			osc_default_volume	= 75; // %
const frq_t			oct_base_freq 		= 16.3516;//27.5/2.0 = C0
const float			percent				= 0.01;

const int				oct_steps		= 12;
const static uint		max_octave		= 7;
const uint 				min_octave 		= 0;
const string			OctChars_EN		= "CcDdEFfGgAaB";
const string			OctChars_DE		= "CcDdEFfGgAaH";

template< typename T >
struct range_T
{
	T 		min;
	T 		max;
	size_t	len = max - min;
};

const range_T<uint8_t>	percent_range		{ 0, 100 };
const range_T<int>		volidx_range		{ 0, 100 };
const range_T<buffer_t>	frames_range		{ 0, max_frames };
const range_T<uint>		duration_range		{ min_msec, max_msec };
const range_T<int>		octave_range		{ min_octave, max_octave };

template< typename T>
constexpr bool in_range( range_T<T> range, T val )
{
	if ( val < range.min )
		return false;
	if ( val > range.max )
		return false;
	return true;
}
template< typename T>
constexpr T check_range( range_T<T> r, T val, string errmsg = "" )
{
	if( val < r.min )
	{
		if ( errmsg.length() > 0 )
			cout.flush() << "WARNING: " << errmsg
									<< ": "
									<< val
									<< " adjusted to min boundaries "
									<< r.min << endl;
		return r.min;
	}
	if (val > r.max )
	{
		if ( errmsg.length() > 0 )
			cout.flush() << "WARNING: " << errmsg
									<< ": "
									<< val
									<< " adjusted to max boundaries "
									<< r.max << endl;
		return r.max;
	}
	return val;
};

template< typename T>
constexpr T check_cycle( range_T<T> r, T val, string err  )
{
	if( r.max == 0 ) return 0;
	if( val < r.min )
		return r.max;
	if ( val > r.max)
		return val % r.max;//r.max % val;////r.min;//val - r.max;
	return val;
};

template< typename T>
constexpr T check_cycle2( range_T<T> r, T val, string err  )
{
	if( r.len == 0 )
		return 0;
	if( val < r.min )
		return r.max - ((r.min - val ) % r.len);
	if ( val > r.max )
		return r.min + ((val - r.max ) % r.len);
	return val;
};

template<typename T>
string show_range( range_T<T> range )
{
	stringstream strs {};
	strs << (int)range.min << "..." << (int)range.max ;
	return strs.str();
};

#define ALLITEMS	-1
#define DEFAULT_ID	-1
#define NoPID 		-1



template< typename Enum>
Enum& operator++(Enum& _enum)
{
	_enum = static_cast<Enum>(static_cast<int>(_enum) + 1);
	return _enum;
};

// Postfix increment operator (x++)
template< typename Enum>
Enum operator++( Enum& _enum, int)
{
	Enum temp = _enum;
	++_enum; // Use the prefix increment logic
	return temp;
};

constexpr string to_hex( long addr )
{
	stringstream strs;
	strs << "0x" << uppercase << hex << addr ;
	return strs.str();
};

enum OSCID_e : unsigned char
{
	VCOID,
	FMOID,
	OSCID,
	NOOSCID
};
const  vector<OSCID_e> oscIds  =
{
	VCOID,
	FMOID,
	OSCID
};
#define OSCIDSIZE 3

typedef struct osc_struct
{

	const vector<string> typeNames =
	{
		"VCO",
		"FMO",
		"OSC",
		""
	};
} osc_roles_t;

enum RoleId_e : unsigned char
{
	INSTRROLE,
	NOTESROLE,
	KBDROLE,
	EXTERNALROLE,
	AUDIOROLE,
	ADSRROLE,
	USER00ROLE,
	USER01ROLE,
	USER02ROLE,
	USER03ROLE,
	ROLE_SIZE
};
const array<string, ROLE_SIZE> roleNames =
{
	"Instrument",
	"Notes",
	"Keyboard",
	"External",
	"Audio",
	"ADSR",
	"UserR+",
	"UserL-",
	"UserR-",
	"UserL+"
};

const vector<string> slidermodes =
{
	"Fixed",
	"Slide",
	"Combine"
};
enum DYNAMIC : unsigned char
{ FIXED, SLIDE, COMBINE }; // frequency and volume change mode

enum StAId_e : unsigned char
{
	STA_USER00 = 0,
	STA_USER01,
	STA_USER02,
	STA_USER03,
	STA_INSTRUMENT,
	STA_KEYBOARD,
	STA_NOTES,
	STA_EXTERNAL,
	STA_SIZE
};





#endif /* OCEAN_H */
