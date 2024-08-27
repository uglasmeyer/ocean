/*
 * playnotes.h
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_PLAYNOTES_H_
#define INCLUDE_PLAYNOTES_H_

#include <Spectrum.h>

#include <common.h>
#include <Instrument.h>
#include <Oscbase.h>
#include <osc.h>
#include <Notesbase.h>
#include <Synthesizer.h>

extern bool cmpstr( const string , const string  );

using namespace std;

class Note_class :  virtual public Logfacility_class, virtual public Note_base
{
public:

	Oscillator	 	main		{ NOTESID };
	Oscillator	 	vco			{ VCOID };
	Oscillator	 	fmo			{ FMOID };

	vector<Oscillator*> osc_group { &vco, &fmo, &main };

	string			Instrument_name { "" };
	uint8_t			noteline_sec 	= 0;
	const string 	RhythmChars 	= "123456789";
	const string 	OctaveChars 	= "12345";

	string 			Note_Chars		= convention_notes[ noteline_prefix_default.convention ];
	uint8_t 		Octave			= noteline_prefix_default.Octave;
	const uint16_t	measure_duration= max_milli_sec; // 1 sec.
	uint16_t 		min_duration 	= measure_duration / noteline_prefix_default.nps;  //milli seconds

	noteline_prefix_t
					Noteline_prefix	= noteline_prefix_default; // D=default, N=numeric
	notelist_t 		notelist		{};

	Instrument_class* instrument;

	Note_class( ); // used by Variation
	Note_class( Instrument_class* instr ); // used by Synthesizer

	~Note_class();

	string 			Read( string );
	void			Save( string, noteline_prefix_t , string  );
	void 			Set_osc_track( );
	string 			Get_note_line ();
	string 			Get_rhythm_line (  );
	void 			Set_rhythm_line(string );
	bool			Set_notes_per_second( int );
	bool			Generate_note_chunk( Storage::Storage_class* 		mb );
	void			Set_base_octave( uint );
	void			Set_prefix_octave( int );
	bool			Verify_noteline( noteline_prefix_t, string );
	void 			Test();
	void			Show_note(  note_struct_t );
	void 			Start_note_itr();

	template< typename T>
	void Show_note_list( T items )
	{
		stringstream strs;
		uint lineduration = 0;
		strs << "Chord         Vol  msec Oct dOc  Freq|Oct dOc  Freq|Oct dOc  Freq|";
		Comment( INFO, strs.str() );

		for( note_struct_t note : items )
		{
			Show_note( note );
			lineduration += note.duration;
			if ( ( lineduration % measure_duration ) == 0 )
				cout << endl;
		}
		strs.str("");
		strs << setw(17) << left  << "sentence length" <<
				setw(6) << right << dec << lineduration << " [msec]" ;
		Comment( INFO, strs.str() );
	}

private:
	uint16_t		note_duration 	= 0; 	// consider the length of one note by counting "-"-chars
	string 			Notefile_name 	= "";
	string 			Notefile 		= "";
	string 			Noteline		= "";
	string 			Volumeline 		= "";
	strlen_t		volume_vec_len 	= 1;
	strlen_t		vcounter		= 0;
	int				octave_shift  	= 0;
	strlen_t 		noteline_len 	= 0;
	vector<uint>    volume_vec 		{};

	note_struct_t 	note_buffer 	= note_struct();

	typedef notelist_t::iterator
					note_itr_t;
	note_itr_t  	note_itr 		= notelist.begin();


	string 			get_name();
	void 			set_note_list ( noteline_prefix_t,  string );
	void			set_name( string );
	strlen_t		Noteline_position_parser( unsigned long int );
	void 			note2memory( note_struct_t, buffer_t );
	void 			change_alphabet_notes( noteline_prefix_t );
	void 			submit_data(Storage::Storage_class* 		mb);
	void            set_volume_vector( string );
	int 			notechar2Value( char );
	void			fill_note_list();
	void			add_volume( note_itr_t );
	void			assign_freq();
	float	 		calc_freq ( uint8_t , notevalue_struct_t );
	note_struct_t	Char2note( char );
	void			split_long_notes();

};

#endif /* INCLUDE_PLAYNOTES_H_ */
