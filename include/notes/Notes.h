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
	Instrument_class*	instrument 	= nullptr;
	Storage_class*		StA			= nullptr;

public:
	Oscgroup_class		Oscgroup				{ NOTESROLE, 2*monobuffer_bytes };
	Oscillator*			osc						= &Oscgroup.osc;
	Oscillator*			vco						= &Oscgroup.vco;
	Oscillator*			fmo						= &Oscgroup.fmo;
	Oscillator*			Osc						= &Oscgroup.osc;
	file_structure*		fs						= nullptr;
	Wavedisplay_class*	wd						= nullptr;
	Scanner_class*		scanner					= nullptr;
	string				Instrument_name 		{ "" };
	Trigger_class		Note_itr_start			;
	Trigger_class		Note_itr_end			;
	uint8_t 			Octave					= nlp_default.Octave; // 55

	Dynamic_class		DynFrequency			{ frqarr_range };

	Data_t*				NotesData				= osc->MemData_p( );
	interface_t*		sds						= nullptr;

					Note_class				( Instrument_class* instr,
											Storage_class*	sta);// used by notes (Synthesizer)
					Note_class				( ); 				// used by Musicxcl
					Note_class				( file_structure* fs );// variation
					Note_class				( interface_t* sds,
											Config_class* cfg  ); // used by File_dialog (OceanGUI)
	virtual			~Note_class				();


	string 			Read					( string );
	void			Save					( string, noteline_prefix_t , string  );
	string 			Get_note_line 			();
	string 			Get_rhythm_line 		();
	void 			Set_rhythm_line			(string );
	bool			Set_notes_per_second	( int );
	void			Set_noteline_prefix		( noteline_prefix_struct nlp );

	void			LoadMusicxml			( const string& file );
	bool			Generate_cyclic_data	();
	bool 			Generate_volatile_data	( bool init = false );
	void			ScanData				();
	void 			SetSDS					( noteline_prefix_t nlp );

	void			Set_prefix_octave		( int );
	bool			Verify_noteline			( noteline_prefix_t nlp,
											string noteline );
	notelist_t 		Gen_notelist			( noteline_prefix_t prefix,
											string str );
	uint 			Calc_notelist_msec		( notelist_t notelist );
	void 			Align_measure			( noteline_prefix_t prefix,
											string& noteline );

	void 			TestNotes					();
	void			Show_note				( note_t, bool debug=false );

	bool 			Start_note_itr			();
	note_t			Char2note				( char& ch );

	void			Set_notelist			( const notelist_t& notelist );


	Table_class 	Note_table 				{ };

	template< class Class>
	void 			Show_note_list			( Class& items ) // list or vector
	{

		Note_table.PrintHeader();
		uint duration 	= 0;
		uint m_duration	= 0;
		uint count 		= 1;
		for( note_t note : items )
		{
			Show_note( note, true );
			show_noteline_duration	( note.duration, duration, count, m_duration );

		}
		Note_table.AddRow( "sentence length", duration, "[msec]" );
	}



private:

	string 			notefile_name 			= "";
	string 			notefile 				= "";
	string 			noteline				= "";
	uint8_t			noteline_sec 			= 0;
	size_t	 		noteline_len 			= 0;
	uint16_t		notenumber				= 0;
	uint16_t		note_duration 			= 0; 	// consider the length of one note by counting "-"-chars
	uint16_t		noteline_duration		= 0;

	string 			volumeline 				= "";
	size_t			volume_vec_len 			= 1;
	size_t			vcounter				= 0;
	vector<uint>    volume_vec 				{};
											// notevalue_struct.doct by one .
	size_t			parse_error				= 0;
	int 			timestamp 				= 0;
	uint 			scoretime 				= 0;


	void			note_itr_next			();
	bool 			note_itr_end			();

	void 			sta_write_data			( uint duration );
	string 			get_name				();
	bool 			compiler 				( noteline_prefix_t,  string );
	bool			set_file_name			( string );
	size_t			position_parser( size_t );
	void 			gen_chord_data			( const note_t&,
											const uint& duration,
											const bool& partnote );
	void 			change_alphabet_notes	( noteline_prefix_t );
	void            set_volume_vector		( string );
	void			fill_note_list			();
	void			add_volume				( note_itr_t );
	void			assign_freq				();
	void			split_long_notes		();

	void			init_note_table			();
	void			show_noteline_duration	( const uint16_t& msec,
											uint& duration,
											uint& count,
											uint& m_duration );
};


#endif /* INCLUDE_PLAYNOTES_H_ */
