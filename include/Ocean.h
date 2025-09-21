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
#include <numbers>
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

typedef struct trigger_data_struct
{
	bool state 	= false ;
	bool active = false;
} trigger_data_t;

typedef struct trigger_struct
{
	trigger_data_t* trigger_data;
	trigger_data_t	local_data;

	trigger_struct( trigger_data_t* _data )
	{
		trigger_data 	= _data;
	}
	trigger_struct()
	{
		trigger_data	= &local_data;
	}
	~trigger_struct() = default;

	void set_state( bool _state ) // producer
	{
		trigger_data->state 	= _state;
	}
	void set_active( bool _active ) // controller
	{
		trigger_data->active 	= _active;
	}
	bool get() // action
	{
		bool state = ( trigger_data->active and trigger_data->state);
		trigger_data->state		= false;
		return state;
	}
} trigger_t;

const size_t		sizeof_stereo		= sizeof(stereo_t);
const size_t		sizeof_Stereo		= sizeof(Stereo_t);
const uint 			sizeof_Data 		= sizeof(Data_t);
const uint 			sizeof_data 		= sizeof(data_t);
const phi_t 		pi					= numbers::pi; //3.1415926536;//;

const uint			sample_rate			= 48000; //device dependent fix
const buffer_t		frames_per_sec  	= sample_rate;
const buffer_t		frames_per_msec		= frames_per_sec / 1000 ;

const uint8_t		max_sec 			= 2;
const uint16_t		max_msec	 		= max_sec * 1000;
const uint16_t		min_msec			= 125;
const uint16_t		frame_parts			= (uint16_t) (max_msec / min_msec);

const buffer_t		max_frames			= max_msec * frames_per_msec;
const buffer_t		min_frames			= min_msec * frames_per_msec;

const buffer_t		audio_frames 		= min_frames;//frames_per_sec; // chunksize * 100
const buffer_t 		chunksize			= min_frames / 2;//300;//min_frames/10;///5;//max_frames / 48;// / 100;//441 , 512; // Audio server chunksize

const uint			recduration 		= 3*60; // seconds
const uint			tmpduration 		= 30; 	// temp memory storage 30*frames_per_sec
const uint			kbdduration			= 2;
const buffer_t 		recordmemory_bytes 	= recduration*frames_per_sec * sizeof_Data; // 3 minutes 32Mb
const buffer_t 		monobuffer_bytes   	= max_frames * sizeof_Data;
const Data_t		max_data_amp		= 4096*4;

const uint			osc_default_volume	= 75; // %
const frq_t			oct_base_freq 		= 16.3516;//27.5/2.0 = C0
const float			percent				= 0.01;

template< typename T >
struct range_T
{
	T 		min ;
	T 		max ;
	size_t 	len = max - min;
};
/*
template< typename T >
struct range_T
{
	T min ;
	T max ;
};
*/
const range_T<int>		volidx_range		{ 0, 100 };
const range_T<buffer_t>	frames_range		{ 0, max_frames };
const range_T<uint>		duration_range		{ min_msec, max_msec };

template< typename T>
constexpr T check_range( range_T<T> r, T val, string errmsg )
{
	if( val < r.min )
	{
		cout.flush() << "WARNING: " << errmsg
									<< ": "
									<< val
									<< " adjusted to min boundaries "
									<< r.min << endl;
		return r.min;
	}
	if (val > r.max )
	{
		cout.flush() << "WARNING: " << errmsg
									<< ": "
									<< val
									<< " adjusted to max boundaries "
									<< r.max << endl;
//		static_assert(false);
		return r.max;
	}
	return val;
}

template< typename T>
constexpr T check_cycle( range_T<T> r, T val, string err  )
{
	if( r.max == 0 ) return 0;
	if( val < r.min )
		return r.max;
	if ( val > r.max)
		return val % r.max;//r.min;//val - r.max;
	return val;
}

template< typename T>
constexpr T check_cycle2( range_T<T> r, T val, string err  )
{
	if( val < r.min )
		return r.max - ((r.min - val ) % r.len);
	if ( val > r.max )
		return r.min + ((val - r.max ) % r.len);
	return val;
}

template<typename T>
string show_range( range_T<T> range )
{
	stringstream strs {};
	strs << range.min << "..." << range.max ;
	return strs.str();
}

#define ALLITEMS	-1
#define DEFAULT_ID	-1

enum APPID
{
	AUDIOID,
	SYNTHID,
	COMPID,
	GUI_ID,
	SDSVIEWID,
	RTSPID,
	KEYBOARDID,
	SETUPID,
	TESTPRGID,
	NOID
};
#define AUDIOSERVER "AudioServer"
#define SYNTHESIZER	"Synthesizer"
#define COMPOSER	"Composer"
#define OCEANGUI	"OceanGUI"
#define SDSVIEW		"SDSview"
#define RTSP		"Rtsp"
#define SYNTHKBD	"Keyboard"
#define SETUP		"Setup"
#define TESTPRG		"Testprj"


template<typename T>
constexpr string AppIdName( const T& app_id )
{
	switch ( app_id )
	{
		case APPID::AUDIOID		: return AUDIOSERVER;
		case APPID::SYNTHID		: return SYNTHESIZER;
		case APPID::COMPID		: return COMPOSER;
		case APPID::GUI_ID		: return OCEANGUI;
		case APPID::SDSVIEWID	: return SDSVIEW;
		case APPID::RTSPID		: return RTSP;
		case APPID::KEYBOARDID	: return SYNTHKBD;
		case APPID::SETUPID		: return SETUP;
		case APPID::TESTPRGID	: return TESTPRG;
		case APPID::NOID		: return "No Process";
		default 		: 	{
							cout << "WARN: unknown application id: " << (int)app_id << endl;
							return "No Process";
							};
	}
}
constexpr string to_hex( long addr )
{
	stringstream strs;
	strs << "0x" << uppercase << hex << addr ;
	return strs.str();
}
typedef struct osc_struct
{
	const vector<string> roles =
	{
		"Instrument",
		"Notes",
		"Keyboard",
		"External",
		"Audio",
		"ADSR"
	};
	enum { INSTRID, NOTESID, KBDID, EXTID, AUDIOOUTID, ADSRID };

	const vector<string> oscNames =
	{
		"VCO",
		"FMO",
		"OSC"
	};
	enum { VCOID, FMOID, OSCID };
} osc_roles_t;
const  vector<char> oscIds  =
{
	osc_struct::VCOID,
	osc_struct::FMOID,
	osc_struct::OSCID
};
#define OSCIDSIZE 3

const vector<string> slidermodes =
{
		"Fixed",
		"Slide",
		"Combine"
};
enum DYNAMIC
{ FIXED, SLIDE, COMBINE }; // frequency and volume change mode


const uint				oct_steps		= 12;
const static uint		max_octave		= 7;
const uint 				min_octave 		= 0;
const string			OctChars		= "CcDdEFfGgAaB";

#define ASSERTION(	 expr , message, input, expected )\
	if ( not (expr) ) \
	{\
	printf( "file: ( %s ) line: ( %d ) in function: ( %s )\n", __FILE__, __LINE__, __func__ );\
	cout 	<< message 									<< endl\
			<< "input    value: " << (input) 			<< endl\
			<< "expected value: " << dec << (expected) 	<< endl;\
	exit (0); \
	};



#endif /* OCEAN_H */
