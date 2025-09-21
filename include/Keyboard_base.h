/*
 * Keyboard_base.h
 *
 *  Created on: Jun 4, 2025
 *      Author: sirius
 */

#ifndef KEYBOARD_BASE_H_
#define KEYBOARD_BASE_H_

#include <Ocean.h>
#include <Frequency.h>

const char			NONOTE 					= -1;
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
	frq_t			frq						= 0.0;
	char			note[3]					= { "XX" };

} kbd_state_t;

typedef struct note_value_struct
{
	int8_t 			step;
	int8_t			octave;
} note_value_t;
typedef struct kbd_note_struct
:	note_value_struct
{
	int 			frqidx;
	string			name;
	frq_t			freq;
	kbd_note_struct( int oct, int pos )
	{
		step 	= pos;
		octave 	= oct;
		frqidx 	= frqIndex( step, oct );
		name	= frqNamesArray[ frqidx ];
		freq	= frqArray[ frqidx ];
	}
	kbd_note_struct( int idx )
	{
		frqidx 	= check_range( frqarr_range, idx, "kbd_note_struct" );
		step 	= ( idx - C0 ) % oct_steps;
		octave	= ( idx - C0 - step ) / oct_steps;
		name	= frqNamesArray[ frqidx ];
		freq	= frqArray[ frqidx ];
	}

	~kbd_note_struct() = default;
	string show()
	{
		coutf << name;
		return name;
	}

} kbd_note_t;

#endif /* KEYBOARD_BASE_H_ */
