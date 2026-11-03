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
 * playnotes.h
 *
 *  Created on: Jan 17, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_PLAYNOTES_H_
#define INCLUDE_PLAYNOTES_H_

#include <Instrument.h>
#include <notes/MusicXML.h>



struct noteline_prefix_struct :
	Logfacility_class,
	bps_struct
{
	Table_class Table {"Noteline Prefix", 22 };
	nlp_data_t	data 		;
	nlp_data_t*	data_p		;

				noteline_prefix_struct	( nlp_data_t* data_p );
				~noteline_prefix_struct	() 	= default;
	void		From_string				( const string& str );
	void 		Nlp_data_p				( nlp_data_t* p );
	string 		To_string				( nlp_data_t nlp );
	void 		Show_nlp				();
	nlp_data_t	Read					();
	void		Save					();
	void		selftest				();

};
typedef noteline_prefix_struct noteline_prefix_t;

/**************************************************
 * NotesCore_class
 *************************************************/
class NotesCore_class
	: virtual public	Interface_base
	, virtual public 	Note_base

{
	uint16_t			notenumber				;
	uint16_t			note_duration 			;
	uint16_t			noteline_duration		;
	size_t				vcounter				;
	size_t				parse_error				;
	int 				timestamp 				;
	uint 				scoretime 				;
	uint8_t				noteline_sec 			;
	size_t				volume_vec_len 			;
	vector<uint>    	volume_vec 				;
	Table_class 		Note_table 				;

	dircontent_struct	notefiles				;
	string 				Name 					;
	string				notefiles_dir			;
	string 				notefile 				;

public:
	string				Instrument_name 		;
	cstring_T<cstring_long>		Noteline				;
	cstring_T<cstring_long>		Rythmline				;
	noteline_prefix_t	Noteline_prefix 		;

	uint8_t 			Octave					;
	Musicxml_class*		Musicxml_p				;

						NotesCore_class			( Dataworld_class* data, Musicxml_class* xml );
	virtual 			~NotesCore_class		();

	note_t				Char2note				( char& ch );
	notelist_t 			Gen_notelist			();
	string 				Read					( string filename );
	void				Save					( string filename );
	void				Save					( string filename, nlp_data_t npl, string nl  );
	void 				SetDir					( string subdir );
	bool				Set_new_name			( string );
	void 				Set_Octave_shift		( uint8_t octave );
	void				Set_notes_per_second	( int );
	void				Set_prefix_octave		( int );
	void 				Set_rhythm_line			( string );
	void				Set_xmlnotelist			( const notelist_t& notelist );
	void				Show_note				( const note_t&, bool debug=false );

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

	void 				Test_Musicxml			();
	void 				Test_NotesCore			();
	bool				Verify_noteline			( nlp_data_t nlp, string noteline );
	bool				Verify_noteline			( note_ns::ORIGIN_e origin );

private:

	void				add_volume				( note_itr_t );
	void 				align_measure			();
	void				assign_freq				();
	uint 				calc_notelist_msec		( notelist_t notelist );
	void 				change_alphabet_notes	();
	bool 				compile 				();
	void				fill_note_list			();
	string 				get_name				();
	void				init_note_table			();
	size_t				position_parser			( size_t );
	void				select					( string name );
	void            	set_volume_vector		( string );
	void				split_long_notes		();
	void				show_noteline_duration	( const uint16_t& msec,
												uint& duration,
												uint& count,
												uint& m_duration );
	bool				verify_noteline			();

};



/**************************************************
 * Note_class
 * interface between notes core the oscgroup and the mixer
 * using the settings of the current instrument
 *************************************************/
class Note_class
//	: virtual public 	Note_base
	: public virtual 	Interface_base
	, public virtual	NotesCore_class
{
	Instrument_class*	instrument 				;
	Storage_class*		StA						;
	Oscgroup_class		Oscgroup				{ NOTESROLE, 2*monobuffer_bytes };
	Oscillator*			osc						;
	Oscillator*			vco						;
	Oscillator*			fmo						;
	Oscillator*			Osc						;
	uint8_t 			read_cnt 				;


public:
	Scanner_class*		scanner					;
	Trigger_class		Note_itr_start			;
	Trigger_class		Note_itr_end			;

						Note_class				( Dataworld_class* data,
												Instrument_class* instr,
												Storage_class*	sta,
												Musicxml_class* xml_p );	// Synthesizer
	virtual				~Note_class				();
	void				LoadMusicxml			( const string& file );
	bool				Generate_cyclic_data	();
	bool 				Generate_volatile_data	( bool init = false );
	void 				Readlock_wait			( bool lock );
//	bool 				SetNoteline				();

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
