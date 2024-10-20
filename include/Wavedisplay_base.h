/*
 * Wavedisplay_base.h
 *
 *  Created on: Sep 9, 2024
 *      Author: sirius
 */

#ifndef WAVEDISPLAY_BASE_H_
#define WAVEDISPLAY_BASE_H_

#include <Ocean.h>

const vector<string> wavedisplay_type_str_vec =
{
	"Full",
	"Flow",
	"Debug"
};

enum
{
	FULLID,
	FLOWID,
	DEBUGID
};

const uint WD_SIZE = 5;
const array<string,WD_SIZE> wavedisplay_str_arr =
{
    "Audio Out",
    "External IN",
	"MAIN",
    "VCO",
    "FMO"
};

enum
{
	AUDIOOUT
};

const uint 				wavedisplay_len		= 512;
typedef array< Data_t,	wavedisplay_len> 	wd_arr_t;

#endif /* WAVEDISPLAY_BASE_H_ */
