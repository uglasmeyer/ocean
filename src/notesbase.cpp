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
 * Notenase.cpp
 *
 *  Created on: Jul 3, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <notes/Notesbase.h>
#include <System.h>
#include <Exit.h>

const 	range_T<uint8_t>chord_range { 1, 4 };
float get_chord_volume( float volume, uint8_t chordlen )
{
	if( chordlen == 0)
		return 0.0;
	chordlen 	= check_range( chord_range, chordlen, "set_sta_volume" );
	float	vol = volume;
	if ( chordlen > 1 ) // 2,3,4  --> volume = 3/4, 2,3/ 1,2
		vol 	=	( volume * ( 5.0 - chordlen ) ) / ( 6.0 - chordlen );

	assert(vol < 1.0001);
	return vol;
}

Note_base::Note_base () :
	Logfacility_class("Note_base"),
	Frequency_class()
{
	this->min_duration			= 1000 / nlp_default.nps;  //milli seconds
	this->note_buffer 			= note_struct();
	this->notelist.clear		();
	this->note_itr 				= notelist.begin();
	this->Note_Chars			= { convention_notes[ nlp_default.convention ] };
	this->rest_note				= note_struct( note_ns::PAUSE, 0 );
	this->rest_note.duration	= min_duration;
	this->EmptyNote				= rest_note;
	this->EmptyNote.chord.clear	();

}

Note_base::~Note_base()
{
	DESTRUCTOR( className )
};


uint8_t Note_base::GetFrqIndex( const pitch_t& nvs)
{
	int 		step	= nvs.step + nvs.alter;
	int 		oct 	= nvs.octave;
	if ( step 	< 0 )
	{
				step 	+= oct_steps;
				oct 	-=  1;
	}
	int			octave	= abs( oct );// + octave_shift );
	uint8_t 	key 	= Index( octave, step );
	return key ;
}
float Note_base::CalcFreq ( const float& base,  pitch_t& nvs )
{

	int 		key 	= GetFrqIndex( nvs );
	return 		Frequency_class::Calc(base, key);
};
int Note_base::Notechar2Step( char note_char )
{
	if ( Note_Chars.Set.contains( note_char ) )
		return Note_Chars.Str.find( note_char );
	else
		return NONOTE;
}


void Note_base::TestNoteBase()
{

	TEST_START("Notes_base");


	TEST_END("Notes_base");
}



