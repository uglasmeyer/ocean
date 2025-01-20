/*
 * Wavedisplay_base.h
 *
 *  Created on: Sep 9, 2024
 *      Author: sirius
 */

#ifndef WAVEDISPLAY_BASE_H_
#define WAVEDISPLAY_BASE_H_

#include <Ocean.h>


enum
{
	FULLID,
	FLOWID,
	DEBUGID
};

typedef struct wavedisplay_struct
{
	const vector<string> names =
	{
		"Audio Out",
		"External IN",
		"Instrument",
		"Notes",
		"Keyboard"
	};
	const vector<string> oscs =
	{
		"VCO",
		"FMO",
		"OSC"
	};
	const vector<string> types =
	{
		"Full",
		"Flow",
		"Debug"
	};
} wavedisplay_t;

const size_t WD_OSC_SIZE = 3;
const size_t WD_DISPLAY_SIZE = 5;;
const size_t WD_DEBUG_SIZE = 3;;

enum
{
	AUDIOOUT,
	EXTERNALIN
};


const size_t wavedisplay_len		= 512;

typedef array< Data_t,	wavedisplay_len> 	wd_arr_t;

#endif /* WAVEDISPLAY_BASE_H_ */
