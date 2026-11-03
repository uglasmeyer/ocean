/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * Keyboard_base.h
 *
 *  Created on: Jun 4, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef KEYBOARD_BASE_H_
#define KEYBOARD_BASE_H_



const char			kbd_rows				= 3;
const int 			max_kbd_octave			= max_octave - kbd_rows ;

const range_T<uint8_t>	sharps_range		{ 0, 3 };
const range_T<uint8_t>	flats_range			{ 0, 3 };
const range_T<uint8_t>	Kbdoctave_range		{ 0, max_kbd_octave };

typedef struct kbd_state_data
{	// SDS related
	uint8_t			base_octave 			= 3; 	// current min kbd octave changeble by +/-
	uint8_t			sharps					= 0; 	// range(0 .. 3 ) notebase::sharp_pitch
	uint8_t			flats					= 0; 	// range(0 .. 2 ) notebase::flat_pitch
	uint8_t			bpsval					= 1;	// apply ADSR data
	bool			sliding					= false;// use dynamic class on frequency
	char			trigger					= 0;	// record trigger is inactive
													// 1 Wait for event
													// 2 scan pos reached the begin of the sta buffer
	bool			savenotes				= false;// trigger to save keyboard notes
	kbdInt_t		key						= 0x0;	// QT::key
	frq_t			frq						= 0.0;	// OceanGUI display frequency
	char			note[3]					= {"XX"};//OceanGUI display key
	char			chord_type				= 'Y';	// OceanGUI display chord type
} kbd_state_t;


#endif /* KEYBOARD_BASE_H_ */
