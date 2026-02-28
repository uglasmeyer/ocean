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
 * playnotes.h
 *
 *  Created on: Jan 17, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_PLAYNOTES_H_
#define INCLUDE_PLAYNOTES_H_

#include <Spectrum.h>
#include <data/DataWorld.h>
#include <Instrument.h>
#include <Oscbase.h>
#include <Osc.h>
#include <Ocean.h>
#include <System.h>
#include <Oscgroup.h>
#include <notes/Notesbase.h>
#include <notes/MusicXML.h>

#include <Logfacility.h>

/**************************************************
 * NotesCore_class
 *************************************************/
class NotesCore_class
	: virtual public 	Logfacility_class
	, virtual public 	Note_base
	, public virtual 	Interface_base

{
	string 				notefile_name 			;
	size_t	 			noteline_len 			;
	uint16_t			notenumber				;
	uint16_t			note_duration 			;
	uint16_t			noteline_duration		;
	size_t				vcounter				;
	size_t				parse_error				;
	int 				timestamp 				;
	uint 				scoretime 				;
	string 				notefile 				;
	string 				noteline				;
	uint8_t				noteline_sec 			;
	string 				volumeline 				;
	size_t				volume_vec_len 			;
	vector<uint>    	volume_vec 				;
	Table_class 		Note_table 				;

public:
	string				Instrument_name 		;
	uint8_t 			Octave					;
	Musicxml_class		Musicxml				;


						NotesCore_class			( Dataworld_class* data );
	virtual 			~NotesCore_class		();

	void 				Align_measure			( noteline_prefix_t prefix,
												string& noteline );
	uint 				Calc_notelist_msec		( notelist_t notelist );
	note_t				Char2note				( char& ch );
	notelist_t 			Gen_notelist			( noteline_prefix_t prefix,
												string str );
	string 				Get_note_line 			();
	string 				Get_rhythm_line 		();
	string 				Read					( string filename );
	void				Save					( string, noteline_prefix_t , string  );
	void 				Set_nlp					( noteline_prefix_t nlp );
	void				Set_noteline_prefix		( noteline_prefix_t nlp );
	bool				Set_notes_per_second	( int );
	void				Set_prefix_octave		( int );
	void 				Set_rhythm_line			( string );
	void				Set_xmlnotelist			( const notelist_t& notelist );
	void				Show_note				( note_t, bool debug=false );
	template< class Class>
	void 				Show_note_list			( Class& items ) // list or vector
	{

		Note_table.PrintHeader	();
		uint duration 			= 0;
		uint m_duration			= 0;
		uint count 				= 1;
		for( note_t note : items )
		{
			Show_note( note, true );
			show_noteline_duration	( note.duration, duration, count, m_duration );

		}
		Note_table.AddRow( "sentence length", duration, "[msec]" );
	}
	void 				Test_NotesCore			();
	void 				Test_Musicxml			();
	bool				Verify_noteline			( noteline_prefix_t nlp,
												string noteline );
private:

	void				add_volume				( note_itr_t );
	void				assign_freq				();
	void 				change_alphabet_notes	( noteline_prefix_t );
	bool 				compiler 				( noteline_prefix_t prefix,
												string str );
	void				fill_note_list			();
	string 				get_name				();
	void				init_note_table			();
	size_t				position_parser			( size_t );
	bool				set_file_name			( string );
	void            	set_volume_vector		( string );
	void				split_long_notes		();
	void				show_noteline_duration	( const uint16_t& msec,
												uint& duration,
												uint& count,
												uint& m_duration );

};



/**************************************************
 * Note_class
 *************************************************/
class Note_class
	: virtual public 	Note_base
	, public virtual 	Interface_base
	, public virtual	NotesCore_class
{
	Instrument_class*	instrument 				;
	Storage_class*		StA						;
	Oscgroup_t			Oscgroup				;
	Oscillator*			osc						;
	Oscillator*			vco						;
	Oscillator*			fmo						;
	Oscillator*			Osc						;
	Scanner_class*		scanner					;
	uint8_t 			read_cnt 				;

public:
	Trigger_class		Note_itr_start			;
	Trigger_class		Note_itr_end			;

						Note_class				( Dataworld_class* data,
												Instrument_class* instr,
												Storage_class*	sta );	// Synthesizer
	virtual				~Note_class				();
	void				LoadMusicxml			( const string& file );
	bool				Generate_cyclic_data	();
	bool 				Generate_volatile_data	( bool init = false );
	Data_t*				ScanData				();

private:
	void				note_itr_next			();
	bool 				note_itr_end			();
	bool	 			Note_clock				( bool init );
	void 				sta_write_data			( const note_t& note );
	void 				gen_chord_data			( note_t& note );
	void 				testNotes				();

};


#endif /* INCLUDE_PLAYNOTES_H_ */
