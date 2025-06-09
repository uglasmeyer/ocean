/*
 * Keyboard_base.h
 *
 *  Created on: Jun 4, 2025
 *      Author: sirius
 */

#ifndef KEYBOARD_BASE_H_
#define KEYBOARD_BASE_H_

const char			NONOTE 					= -1;


typedef struct kbd_state_struct
{
	// SDS
	int 				base_octave 			= 3; 	// current min kbd octave changeble by +/-
	uint				sharps					= 0; 	// range(0 .. 3 ) notebase::sharp_pitch
	uint				flats					= 0; 	// range(0 .. 2 ) notebase::sharp_pitch
	bool				ADSR_flag				= true;	// apply ADSR data
	bool				sliding					= false;// use wp.glide on frequency
} kbd_state_t;



#endif /* KEYBOARD_BASE_H_ */
