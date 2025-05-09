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


class Note_class :  virtual public Logfacility_class,
					virtual public Note_base
{
	string className = "";
public:

	typedef struct musicxml_struct
	{
		string 		instrument_name = "";
		int			divisions 		= -1;
		int			beats			= -1;
		uint		tempo			= max_sec;
		uint		scoreduration 	= 0; // unit milli second
		notelist_t 	notelist 		{};
	} musicxml_t;
	musicxml_t		musicxml		= musicxml_struct();

	Oscgroup_class	Oscgroup		{ osc_struct::NOTESID };
	Oscillator*		osc				= &Oscgroup.osc;
	Oscillator*		vco				= &Oscgroup.vco;
	Oscillator*		fmo				= &Oscgroup.fmo;

	uint 			framePart 		= 0; // frame cursor
	string			Instrument_name { "" };
	uint8_t			noteline_sec 	= 0;
	bool			Restart			= false;
	String 			Note_Chars		{ convention_notes[ noteline_prefix_default.convention ] };
	uint8_t 		Octave			= noteline_prefix_default.Octave; // 55
	const uint16_t	measure_duration= 1000;//max_milli_sec; // 1 sec.
	const float 	max_frequency 	= frequency_range.max;

	uint16_t 		min_duration 	= measure_duration / noteline_prefix_default.nps;  //milli seconds

	note_t 			note_buffer 	= note_struct();
	const note_t	pause_note		= {".",{pitch_struct()},min_duration,0,0,{glide_struct()},false };
	Dynamic_class	DynFrequency	{ frqarr_range };

	Data_t*			NotesData		= osc->MemData_p( );
	Note_class( Wavedisplay_class* wd ); // used by Variation
	Note_class( ); // used by Variation
	~Note_class();


	string 			Read( string );
	void			Save( string, noteline_prefix_t , string  );
	void 			Set_instrument(Instrument_class *instrument);
	string 			Get_note_line ();
	string 			Get_rhythm_line (  );
	void 			Set_rhythm_line(string );
	bool			Set_notes_per_second( int );
	void			LoadMusicxml( const string& file );
	bool			Generate_note_chunk( );//Storage::Storage_class* mb );
	void			ScanData( Instrument_class* instrument );
	void			Set_prefix_octave( int );
	bool			Verify_noteline( noteline_prefix_t, string );
	void 			Test();
	void			Show_note(  note_t );
	void 			Start_note_itr();
	note_t			Char2note( char& ch );

	int 			Notechar2Step( char );

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
	uint16_t		note_duration 	= 0; 	// consider the length of one note by counting "-"-chars
	string 			Notefile_name 	= "";
	string 			Notefile 		= "";
	string 			Noteline		= "";
	string 			Volumeline 		= "";
	size_t			volume_vec_len 	= 1;
	size_t			vcounter		= 0;
											// notevalue_struct.doct by one .
	size_t	 		noteline_len 	= 0;
	size_t			parse_error		= 0;
	vector<uint>    volume_vec 		{};


	typedef notelist_t::iterator
					note_itr_t;
	note_itr_t  	note_itr 		= notelist.begin();

	string 			get_name();
	bool 			compiler ( noteline_prefix_t,  string );
	bool			set_file_name( string );
	size_t			noteline_position_parser( size_t );
	void 			note2memory( 	const note_t&,
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
