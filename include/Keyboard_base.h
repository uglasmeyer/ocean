/*
 * Keyboard_base.h
 *
 *  Created on: Jun 4, 2025
 *      Author: sirius
 */

#ifndef KEYBOARD_BASE_H_
#define KEYBOARD_BASE_H_

#include <Ocean.h>


const char			kbd_rows				= 3;
const int 			max_kbd_octave			= max_octave - kbd_rows ;


typedef struct kbd_state_struct
{	// SDS related
	int 			base_octave 			= 3; 	// current min kbd octave changeble by +/-
	uint			sharps					= 0; 	// range(0 .. 3 ) notebase::sharp_pitch
	uint			flats					= 0; 	// range(0 .. 2 ) notebase::sharp_pitch
	bool			ADSR_flag				= true;	// apply ADSR data
	bool			sliding					= false;// use wp.glide on frequency
	char			trigger					= 0;	// record trigger is inactive
													// 1 Wait for event
													// 2 scan pos reached the begin of the sta buffer
	kbdInt_t		key						= 0x0;	// QT::key
	frq_t			frq						= 0.0;	// OceanGUI display frequency
	char			note[3]					= {"XX"};//OceanGUI display key
	Id_t			chord_type				= 'y';	// OceanGUI display chord type

} kbd_state_t;


#endif /* KEYBOARD_BASE_H_ */
