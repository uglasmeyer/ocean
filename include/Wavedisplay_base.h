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
	const vector<string> types =
	{
		"Full",
		"Flow",
		"Debug"
	};
} wavedisplay_t;

const size_t WD_OSC_SIZE 		= 3;//wavedisplay_struct().oscs.size();
const size_t WD_DISPLAY_SIZE 	= osc_struct().roles.size();
const size_t WD_DEBUG_SIZE 		= wavedisplay_struct().types.size();



const size_t wavedisplay_len		= 512;

typedef array< Data_t,	wavedisplay_len> 	wd_arr_t;

#endif /* WAVEDISPLAY_BASE_H_ */
