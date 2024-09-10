/*
 * playnotes.h
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_PLAYNOTES_H_
#define INCLUDE_PLAYNOTES_H_

#include <Common.h>
#include <Spectrum.h>

#include <Instrument.h>
#include <Oscbase.h>
#include <Osc.h>
#include <Notesbase.h>
#include <Synthesizer.h>


using namespace std;

class Note_class :  virtual public Logfacility_class,
					virtual public Note_base
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


	Note_class( ); // used by Variation
	~Note_class();

	string 			Read( string );
	void			Save( string, noteline_prefix_t , string  );
	void 			Set_osc_track( Instrument_class* );
	string 			Get_note_line ();
	string 			Get_rhythm_line (  );
	void 			Set_rhythm_line(string );
	bool			Set_notes_per_second( int );
	bool			Generate_note_chunk( );//Storage::Storage_class* mb );
	void			Set_base_octave( uint );
	void			Set_prefix_octave( int );
	bool			Verify_noteline( noteline_prefix_t, string );
	void 			Test();
	void			Show_note(  note_t );
	void 			Start_note_itr();
	uint16_t 		Octave_freq( uint8_t oct );

	const float 	max_frequency 	= Octave_freq ( max_octave + 1 );

	template< typename T>
	void Show_note_list( T items )
	{
		stringstream strs;
		uint lineduration = 0;
		strs << "Chord         Vol  msec Oct dOc  Freq|Oct dOc  Freq|Oct dOc  Freq|";
		Comment( INFO, strs.str() );

		for( note_t note : items )
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
	uint8_t			notes_default_volume
									= 80;
	string 			Notefile_name 	= "";
	string 			Notefile 		= "";
	string 			Noteline		= "";
	string 			Volumeline 		= "";
	size_t			volume_vec_len 	= 1;
	size_t			vcounter		= 0;
	int				octave_shift  	= 0;
	size_t	 		noteline_len 	= 0;
	vector<uint>    volume_vec 		{};

	note_t 	note_buffer 	= note_struct();

	typedef notelist_t::iterator
					note_itr_t;
	note_itr_t  	note_itr 		= notelist.begin();


	string 			get_name();
	void 			compiler ( noteline_prefix_t,  string );
	void			set_file_name( string );
	size_t			noteline_position_parser( size_t );
	void 			note2memory( const note_t&, const buffer_t& );
	void 			change_alphabet_notes( noteline_prefix_t );
	void            set_volume_vector( string );
	int 			notechar2Value( char );
	void			fill_note_list();
	void			add_volume( note_itr_t );
	void			assign_freq();
	float	 		calc_freq ( uint8_t , notevalue_t );
	note_t			char2note( char );
	void			split_long_notes();

};

#endif /* INCLUDE_PLAYNOTES_H_ */
