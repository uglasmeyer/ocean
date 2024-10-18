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

const array<string,5> wavedisplay_str_vec =
{
    "Audio Out",
	"MAIN",
    "VCO ",
    "FMO ",
    "External IN"
};

enum
{
	AUDIOOUT
};

const uint 				wavedisplay_len		= 512;
typedef array< Data_t,	wavedisplay_len> 	wd_arr_t;

#endif /* WAVEDISPLAY_BASE_H_ */
