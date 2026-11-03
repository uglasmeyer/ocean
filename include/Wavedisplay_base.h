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
		"size"
		"Cursor",	// Cut desk
		"Fft amplitude",
		"Fft display",
		"Adsr"
	};
	const vector<string> fftmodes =
	{
		"A(τ)",
		"A(ω)"
	};
	enum WDMODE_t : unsigned char // classification of the display data used by gen_wave_data
	{
		FULL_WD,	// generate wf data of the sta consisting of the fillrange
		FLOW_WD,	// generate scrolling data
		DEBUG_WD,	// split display right/left and generate high resolution wf data
		WD_MODE_SIZE, // button_types vector size (3)
		RANGE_WD,	// activate cutdesk loop fnct and range cursors
		FFTA_WD,	// internal advise -> generate Fourier coefficients out of the main window display data
		FFTD_WD,	// internal advise -> generate fft base data and show it in the main display
		ADSR_WD,	// generate adsr and fft display data
		SPEC_WD		// fourier display data out of spectrum_arr data
	};
	enum Direction_e : uint8_t // cutdesk cursor directions
	{
		BACK_LEFT,
		BACK_RIGHT,
		FRONT_LEFT,
		FRONT_RIGHT,
		GOTO_END,
		ROTATE_RIGHT,
		ROTATE_LEFT,
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

const int 				wavedisplay_len	= 512; // restrict 2^N, N>2
const int 				adsrdisplay_len	= 256;
const size_t			fft_display_len = 256;

typedef struct WD_data_struct
{	// SDS related

	OSCID_e 			oscId 			= OSCID;
	RoleId_e			roleId 			= INSTRROLE;
	bool				adsrmode		= false;	// generate adsr and fft  display data
	bool				spectrum_view	= false; 	// spectrum dialog is visible or not
	WdModeID_t 			mode			= wavedisplay_t::FULL_WD;
													// cursor direction of Cutdesk bounds
	float				zoom			= 1.0;		// Wavedisplay zoon in&out
	Direction_e			direction		= wavedisplay_t::NO_direction;
	uint8_t				record_steps	= 4;		// cursor stepwidth of Cutdesk bounds
	buffer_t			frames			= 0; 		// max number of frames presented in the display
													// len=param.size / minframes is constant
	cursor_t			cursor			{ 0, 0, wavedisplay_len };
} WD_data_t;


const uint8_t 			WD_OSC_SIZE 	= NOOSCID;
const size_t 			WD_ROLES_SIZE 	= ROLE_SIZE-1; //exclude ADSRROLE
const range_T<int>		wd_mode_range 	{ 0, wavedisplay_t::WD_MODE_SIZE };

typedef array< Data_t,	wavedisplay_len>wd_arr_t;
typedef array< Data_t,	adsrdisplay_len>ad_arr_t;
typedef array< Data_t,	fft_display_len>fftarr_t;


#endif /* WAVEDISPLAY_BASE_H_ */
