/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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
#include <Osctypes.h>
#include <Oscwaveform.h>

template< typename T >
struct cursor_T
{
	T		min = 0;
	T		cur = 0;
	T		max = 0;

	size_t 	len = ( max - min );
};

struct wavedisplay_struct
{
	const vector<string> button_types =
	{
		"Full",
		"Flow",
		"Debug"
	};
	const vector<string> types =
	{
		"Full",
		"Flow",
		"Debug",
		"Cursor",
		"Fft",
		"Adsr"
	};
	const vector<string> fftmodes =
	{
		"A(τ)",
		"A(ω)"
	};
	enum WDMODE_t : unsigned char // classification of the display data calculation by gen_cxwave_data
	{
		FULLID,
		FLOWID,
		DEBUGID,
		CURSORID,
		FFTID,
		ADSRID
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
typedef range_T<int16_t>
						record_range_t;
typedef cursor_T<int16_t>
						cursor_t;

const int 				wavedisplay_len	= 512;
const int 				adsrdisplay_len	= 256;

struct WD_data_struct
{	// SDS related

	OSCID_e 			oscId 			= OSCID;
	RoleId_e			roleId 			= INSTRROLE;
	bool 				fftmode			= false;
	bool				adsrmode		= false;
	WdModeID_t 			wd_mode			= wavedisplay_t::FULLID;
	Direction_e			direction		= wavedisplay_t::NO_direction; // GUI bounds cursor direction
	buffer_t			frames			= 0; // max number of frames presented in the display
	cursor_t			cursor			{ 0, 0, wavedisplay_len }; // len=param.size / minframes is constant
} ;

typedef WD_data_struct 	WD_data_t;

const uint8_t 			WD_OSC_SIZE 	= NOOSCID;
const size_t 			WD_ROLES_SIZE 	= ROLE_SIZE-1; //exclude ADSRROLE
const uint8_t 			WD_MODE_SIZE 	= wavedisplay_struct().button_types.size();


typedef array< Data_t,	wavedisplay_len>wd_arr_t;
typedef array< Data_t,	adsrdisplay_len>ad_arr_t;




#endif /* WAVEDISPLAY_BASE_H_ */
