/*
 * keyboard.h
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <data/Memory.h>
#include <Logfacility.h>
#include <Osc.h>
#include <Instrument.h>
#include <Kbd.h>
#include <notes/Notesbase.h>
#include <Ocean.h>
#include <Oscgroup.h>
#include <Mixerbase.h>
#include <Keyboard_base.h>

class keyboardState_class :
	public virtual 		kbd_state_struct,
	public virtual 		Note_base
{
	interface_t* 		sds;

	const range_T<uint>	sharps_range			{ 0, 3 }; // TODO reduced range
	const range_T<uint>	flats_range				{ 0, 2 }; // TODO reduced range
/*	const String		default_kbdNote			{ 	string( "A_S_DF_J_K_L" ) +
													string( "Q_W_ER_U_I_O" ) +
													string( "1_2_34_7_8_9" )};
*/

public:
	static const int 	max_kbd_octave			= max_octave - 3 ;
	const range_T<int>	Kbdoctave_range			{ 1, max_kbd_octave };
	string				NoteNames				= convention_notes[ENGLISH];

	typedef struct Kbd_note_struct :
			kbd_state_struct,
			Note_base
	{
		const array<string, max_kbd_octave>
						dflt_keyboard			= {  string("A_S_DF_J_K_L") ,
													 string("Q_W_ER_U_I_O") ,
													 string("1_2_34_7_8_9")};
		array<string, max_kbd_octave>
						kbdNotes				{ dflt_keyboard };
		int 			step 					= NONOTE;
		int 			octave  				= base_octave;
		int 			frqidx 					= C0;

		Kbd_note_struct() {};
		~Kbd_note_struct() = default;

		void 			setNote					( int key )
		{
			int KEY = toupper( key );
			step	= NONOTE;
			for( uint oct = 0; oct < max_kbd_octave; oct++ )
			{
				size_t pos 	= kbdNotes[ oct ].find( KEY );
				if ( pos < STRINGNOTFOUND )
				{
					step 	= pos;
					octave 	= oct + base_octave;
					frqidx 	= Frequency_class::Index( octave, pos );
					break;
				}
			}
		}
		void show()
		{
			string	note_name 		= frqNamesArray[ frqidx ];
			cout.flush() << note_name ;// << "(" << frqidx << ")";
		}

	} kbd_note_t;
	kbd_note_t 			kbd_note 				{ };

						keyboardState_class		( interface_t* _sds );
	virtual 			~keyboardState_class() 	= default;

	void 				change_octave			( int oct );
	void 				increase_sharps			();
	void 				increase_flats			();
	void 				reset_sharps			();
	void 				reset_flats				();
	void 				toggle_applyADSR		();
	void 				set_slideMode			();
	void 				set_accidental			( uint pitches, int dir  );


} ;


class Keyboard_class :
	virtual public		Logfacility_class,
	virtual public		Kbd_base,
	virtual	public		sdsstate_struct,
	virtual public		keyboardState_class
{
	string 				className 				= "";
	Oscgroup_class		Oscgroup				{ osc_struct::KBDID, 2*monobuffer_bytes };
	Oscillator*			Osc						= &Oscgroup.osc;
	buffer_t			osc_frames				= 0;
	Instrument_class* 	instrument;
	interface_t*		sds;
	Storage_class*		StA;

public:
	// keyhandler.cpp

	Data_t*				Kbd_Data;

						Keyboard_class			( Instrument_class*, Storage_class* );
						Keyboard_class			(); // see comstack
	virtual 			~Keyboard_class			();
	bool 				Set_Kbdnote				();
	void 				NoteEvent				();
	void 				Set_instrument			();
	void 				Enable					();
	void 				ScanData				();
	void 				Show_keys				( bool tty );


private:


	const int 			releaseCounter			= 0;
	const int 			attackCounter 			= frame_parts;//rint( max_frames / min_frames );
	int 				decayCounter 			= 0;
	const uint			kbd_duration			= max_msec;//	= ADSR_flag ? max_msec : min_msec; // [millisec]
	uint 				duration_counter 		= 0;// count the beats of note kbd_key
	uint				holdCounter				= 0;
	const uint			kbd_volume				= 75;
	key3struct_t		Kbd_key					= key3_struct( 0, 0, 0);

	adsr_t				kbd_adsr				= adsr_struct();
	bool				frqMode					= SLIDE;


	void 				attack					();
	void 				release					();
	bool 				decay					();
	void 				set_kdb_note			();

	// keyhandler.cpp

	void 				keyHandler				( key3struct_t kbd );
	string 				get_notenames			();
	void 				specialKey				();
	void 				apply_Adsr				();

};



#endif /* KEYBOARD_H_ */
