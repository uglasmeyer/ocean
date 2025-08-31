/*
 * playnotes.h
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_PLAYNOTES_H_
#define INCLUDE_PLAYNOTES_H_

#include <Spectrum.h>
#include <Instrument.h>
#include <notes/Notesbase.h>
#include <Oscbase.h>
#include <Osc.h>
#include <Ocean.h>
#include <System.h>
#include <Oscgroup.h>


class Note_class
		: virtual public Logfacility_class
		, virtual public Note_base
{
	string 				className 	= "";
	Instrument_class*	instrument 	;
	Storage_class*		StA			;

public:
	Oscgroup_class	Oscgroup		{ osc_struct::NOTESID, 2*monobuffer_bytes };
	Oscillator*		osc				= &Oscgroup.osc;
	Oscillator*		vco				= &Oscgroup.vco;
	Oscillator*		fmo				= &Oscgroup.fmo;
	Oscillator*		Osc				= &Oscgroup.osc;

	uint 			framePart 		= 0; // frame cursor
	string			Instrument_name { "" };
	bool			Restart			= false;
	uint8_t 		Octave			= noteline_prefix_default.Octave; // 55

	Dynamic_class	DynFrequency	{ frqarr_range };

	Data_t*			NotesData		= osc->MemData_p( );
	interface_t*	sds				= nullptr;

					Note_class( Instrument_class* 	instr,
								Storage_class*		sta); 	// used by notes (Synthesizer)
					Note_class( ); 							// used by Variation (Composer)
					Note_class( interface_t* sds ); 		// used by File_dialog (OceanGUI)
	virtual			~Note_class();


	string 			Read( string );
	void			Save( string, noteline_prefix_t , string  );
	void 			Set_instrument(Instrument_class *instrument);
	string 			Get_note_line ();
	string 			Get_rhythm_line (  );
	void 			Set_rhythm_line(string );
	bool			Set_notes_per_second( int );
	void			LoadMusicxml( const string& file );
	bool			Generate_data( );//Storage::Storage_class* mb );
	void			ScanData( Instrument_class* instrument );
	void 			SetSDS( noteline_prefix_t nlp );

	void			Set_prefix_octave( int );
	bool			Verify_noteline		( noteline_prefix_t nlp,
										string noteline );
	notelist_t 		Gen_notelist		( noteline_prefix_t prefix,
										string str );
	uint 			Calc_noteline_msec	( notelist_t notelist );
	void 			Append_noteline		( noteline_prefix_t prefix,
										string& noteline );

	void 			Test();
	void			Show_note(  note_t );
	void 			Start_note_itr();
	note_t			Char2note( char& ch );

	void			Set_notelist( const notelist_t& notelist );

	void Show_note_list( auto items ) // list or vector
	{
		stringstream strs;
		uint lineduration = 0;
		strs << "Chord         Vol  msec  Oct alt Freq| Oct alt Freq| Oct alt Freq|";
		Comment( DEBUG, strs.str() );

		uint count = 1;
		for( note_t note : items )
		{
			Show_note( note );
			lineduration += note.duration;
			uint mod = lineduration % measure_duration;
			if ( mod == 0 )
			{
				Comment( DEBUG, " Measure count: " + to_string(count));
				count++;
			}
		}
		strs.str("");
		strs << setw(17) << left  << "sentence length" <<
				setw(6) << right << dec << lineduration << " [msec]" ;
		Comment( DEBUG, strs.str() );
	}



private:

	string 			notefile_name 	= "";
	string 			notefile 		= "";
	string 			noteline		= "";
	uint8_t			noteline_sec 	= 0;
	size_t	 		noteline_len 	= 0;
	uint16_t		note_duration 	= 0; 	// consider the length of one note by counting "-"-chars

	string 			volumeline 		= "";
	size_t			volume_vec_len 	= 1;
	size_t			vcounter		= 0;
	vector<uint>    volume_vec 		{};
											// notevalue_struct.doct by one .
	size_t			parse_error		= 0;

	typedef notelist_t::iterator
					note_itr_t;
	note_itr_t  	note_itr 		= notelist.begin();

	string 			get_name();
	bool 			compiler ( noteline_prefix_t,  string );
	bool			set_file_name( string );
	size_t			noteline_position_parser( size_t );
	void 			gen_chord_data( 	const note_t&,
									const buffer_t&,
									const uint& duration,
									const bool& partnote );
	void 			change_alphabet_notes( noteline_prefix_t );
	void            set_volume_vector( string );
	void			fill_note_list();
	void			add_volume( note_itr_t );
	void			assign_freq();
	void			split_long_notes();

};

#endif /* INCLUDE_PLAYNOTES_H_ */
