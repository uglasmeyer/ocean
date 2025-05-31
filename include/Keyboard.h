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

class Keyboard_class :
	virtual  Logfacility_class,
	virtual  public Note_base,
	virtual  public Keyboard_base,
	state_struct
{

	string 				className 				= "";
	Oscgroup_class		Oscgroup				{ osc_struct::KBDID, 2*monobuffer_bytes };
	Oscillator*			Osc						= &Oscgroup.osc;
	buffer_t			osc_frames				= 0;
	Instrument_class* 	instrument;
	interface_t*		sds;
	Storage_class*		StA;
	scanner_t			scanner;

public:

	Data_t*				Kbd_Data;

	// keyboard.cpp

						Keyboard_class			( Instrument_class*, Storage_class* );
						Keyboard_class			(); // see comstack
	virtual 			~Keyboard_class			();
	bool 				Set_Kbdnote				();
	void 				NoteEvent				();
	void 				Set_instrument			();
	void 				Enable					();
	void 				ScanData				();

private:

	const int 			releaseCounter			= 0;
	const int 			attackCounter 			= frame_parts;//rint( max_frames / min_frames );
	int 				decayCounter 			= 0;
	const uint			kbd_duration			= max_msec;//	= ADSR_flag ? max_msec : min_msec; // [millisec]
	uint 				duration_counter 		= 0;// count the beats of note kbd_key
	uint				holdCounter				= 0;
	const uint			kbd_volume				= 75;
	int 				kbd_note 				= NONOTE;
	int					attack_note				= NONOTE;
	key3struct_t		Kbd_key					= key3_struct( 0, 0, 0);

	const String		default_kbdNote			{ 	string( "A_S_DF_J_K_L" ) +
													string( "Q_W_ER_U_I_O" ) +
													string( "1_2_34_7_8_9" )
												};
	int					base_octave				= 3; // current min kbd octave changeble by +/-
	const int			kbd_octave				= 3; // # of octaves on the keyboard
	const int 			max_kbd_octave			= max_octave - ( kbd_octave - 1);
	range_t<int>		octave_range			{ 0, max_kbd_octave };


	String				kbdNotes				{ default_kbdNote.Str };
	uint				sharps					= 0; // range(0 .. 6 ) notebase::sharp_pitch
	bool				ADSR_flag				= true;
	adsr_t				kbd_adsr				= adsr_struct();
	const char			NONOTE 					= -1;
	bool				frqMode					= SLIDE;
	bool				sliding					= false;
	const int			slidingstep				= 10;
	range_t<uint>		sharps_range			{ 0, 3 }; // TODO reduced range

	bool 				attack					();
	void 				release					();
	bool 				decay					();
	void 				set_kdb_note			();


	// keyhandler.cpp

	void 				keyHandler				( key3struct_t kbd );
	void 				change_octave			( int oct );
	string 				get_notenames			();
	void				reset_sharps			();
	void 				specialKey				();
	void 				toggle_applyADSR		();
	void 				apply_Adsr				( bool flag );
	void 				increase_sharps			();
	void 				set_frqMode				();
	void 				show_keys				();

};



#endif /* KEYBOARD_H_ */
