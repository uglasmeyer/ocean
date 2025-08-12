/*
 * keyboard.h
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <Keyboard_base.h>
#include <data/Memory.h>
#include <Logfacility.h>
#include <Osc.h>
#include <Instrument.h>
#include <Kbd.h>
#include <Ocean.h>
#include <Oscgroup.h>
#include <Mixerbase.h>
#include <queue>

class Kbd_note_class
	: public kbd_state_struct
//	, public Note_base
{
public:
	const array<string, kbd_octaves>
					dflt_keyboard_keys	{  	string("A_S_DF_J_K_L") ,
											string("Q_W_ER_U_I_O") ,
											string("1_2_34_7_8_9")};
	array<string, kbd_octaves>
					keyboard_keys		{ dflt_keyboard_keys };

	/*
	 https://www.delamar.de/songwriting/akkorde-lernen-49754/#akkorde-bestimmen
	 https://de.wikipedia.org/wiki/American_Standard_Code_for_Information_Interchange
	 */
	std::map<char, string>
					Chords				{	{'y',string(""  )},// single // @suppress("Invalid arguments")
											{'x',string("43")},// Dur
											{'c',string("34")}, // Moll
											{'v',string("343")}, // Dur+
											{'b',string("433")}, // Moll+
											{'n',string("<")} // Power

										};
	set<char>		chord_keys			{ 'y', 'x', 'c', 'v', 'b', 'n' };
	string 			Chord				{ Chords['y'] };

	kbd_note_t		Note 				= kbd_note_struct( 0, NONOTE);
	string			noteline			{};
	vector<kbd_note_t>
					note_vec			{};

					Kbd_note_class() 	{};
	virtual 		~Kbd_note_class() 	= default;
	string 			setNote				( int key );
	string 			SetChord			( char key );
private:

} ;

class keyboardState_class
	: public virtual 		kbd_state_struct
//	, public virtual 		Note_base
{
	interface_t* 		sds;

	const range_T<uint>	sharps_range			{ 0, 3 }; // TODO reduced range
	const range_T<uint>	flats_range				{ 0, 2 }; // TODO reduced range
	frq_t				basefrq;

public:
	const range_T<int>	Kbdoctave_range			{ 1, max_kbd_octave };
	string				NoteNames				= OctChars;//convention_notes[ENGLISH];
	string				Noteline				{};
	Kbd_note_class		kbd_note 				{};


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
	frq_t				Get_basefrq				( );

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
	Kbd_base			Kbd						{};
	typedef std::queue<key3struct_t>
						key3_stack_t;
	key3_stack_t		key3_stack				{};

public:

	Data_t*				Kbd_Data;
	bool				enabled					= false;

						Keyboard_class			( Instrument_class*, Storage_class* );
						Keyboard_class			(); // see comstack
	virtual 			~Keyboard_class			();
	void 				Set_Kbdnote				( key3struct_t key );
	void 				Set_instrument			();
	void 				Enable					( bool iskbd );
	void 				ScanData				();
	void 				Show_help				( bool tty );


private:

	const int 			releaseCounter			= 0;
	const int 			attackCounter 			= frame_parts;//rint( max_frames / min_frames );
	int 				decayCounter 			= 0;
	const uint			kbd_duration			= max_msec;//	= ADSR_flag ? max_msec : min_msec; // [millisec]
	uint 				duration_counter 		= 0;// count the beats of note kbd_key
	uint				holdCounter				= 0;
	const uint			kbd_volume				= 75;
	key3struct_t		Kbd_key					= key3_struct( 0, 0, 0);

	feature_t			kbd_adsr				= feature_struct();
	bool				frqMode					= SLIDE;


	void 				attack					();
	void 				release					();
	bool 				decay					();
	void 				gen_chord_data			();

	// keyhandler.cpp

	void 				keyHandler				( key3struct_t kbd );
	string 				get_notenames			();
	void 				specialKey				();
	void 				exit_keyboard			();
	void 				notekey					( char key );
	void				set_bufferMode			();


};



#endif /* KEYBOARD_H_ */
