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
#include <source_location> // Assert
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

const uint 			sizeof_data 		= sizeof(Data_t);
const double 		pi					= 3.1415926536;//3.141592654;

const uint			sample_rate			= 48000; //device dependent fix
const buffer_t		frames_per_sec  	= sample_rate;

const uint8_t		max_sec 			= 2;//8;
const uint16_t		max_milli_sec 		= max_sec * 1000;
const buffer_t		max_frames			= max_sec * frames_per_sec;
const buffer_t		audio_frames 		= frames_per_sec; // chunksize * 100

const uint			recduration 		= 3*60; // seconds
const uint			tmpduration 		= 30; 	// temp memory storage 30*frames_per_sec
const buffer_t 		recordmemory_bytes 	= recduration*frames_per_sec * sizeof_data; // 3 minutes 32Mb
const buffer_t 		monobuffer_size   	= max_frames * sizeof_data;
const Data_t		max_data_amp		= 4096*4;

const uint			osc_default_volume	= 100; // %
const float			oct_base_freq 		= 27.5/2.0;

// enumeration of applicatiom Id's
// see Config_class::typeMap


const float			LFO_limit			= 1.0;
const uint8_t		LFO_count			= 100;




#endif /* OCEAN_H */
