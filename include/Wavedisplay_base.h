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
 * Wavedisplay_base.h
 *
 *  Created on: Sep 9, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef WAVEDISPLAY_BASE_H_
#define WAVEDISPLAY_BASE_H_

#include <Ocean.h>

struct wavedisplay_struct
{
	const vector<string> types =
	{
		"Full",
		"Flow",
		"Debug",
		"Cursor"
	};
	const vector<string> guitypesxxx =
	{
		"Full",
		"Flow",
		"Debug"
	};
	const vector<string> fftmodes =
	{
		"A(τ)",
		"A(ω)"
	};
	enum WDMODE_t
	{
		FULLID,
		FLOWID,
		DEBUGID,
		CURSORID,
		FFTID
	};
	enum Direction_e
	{
		BACK_LEFT,
		BACK_RIGHT,
		FRONT_LEFT,
		FRONT_RIGHT,
		GOTO_END,
		NO_direction
	};

};
typedef wavedisplay_struct
						wavedisplay_t;
typedef wavedisplay_t::WDMODE_t
						WdModeID_t;
typedef wavedisplay_t::Direction_e
						Direction_e;
struct WD_data_struct
{	// SDS related

	OSCID_e 			oscId 			= OSCID;
	RoleId_e			roleId 			= INSTRROLE;
	bool 				fftmode			= false;
	WdModeID_t 			wd_mode			= wavedisplay_t::FULLID;
	uint16_t			cursor			= 0; // display current sound location in case of full wd_mode
	Direction_e			direction		= wavedisplay_t::NO_direction; // cut cursor direction
	buffer_t			min				= 0; // cut record window min
	buffer_t			max				= 1; // cut record window max
	buffer_t			max_records		= 1; // memory max records
} ;

typedef WD_data_struct 	WD_data_t;

const uint8_t 			WD_OSC_SIZE 	= NOOSCID;
const size_t 			WD_ROLES_SIZE 	= ROLE_SIZE;
const uint8_t 			WD_MODE_SIZE 	= wavedisplay_struct().types.size();

const size_t 			wavedisplay_len	= 512;

typedef array< Data_t,	wavedisplay_len>wd_arr_t;

typedef complex<double>	cd_t;
typedef vector<cd_t>	cd_vec_t;
extern wd_arr_t 		fft				(cd_vec_t data, bool invert);

#endif /* WAVEDISPLAY_BASE_H_ */
