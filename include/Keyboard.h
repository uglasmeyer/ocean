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
 * keyboard.h
 *
 *  Created on: Aug 14, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <data/Memory.h>
#include <Keyboard_base.h>
#include <Logfacility.h>
#include <Osc.h>
#include <Instrument.h>
#include <Kbd.h>
#include <Ocean.h>
#include <Oscgroup.h>
#include <Mixerbase.h>
#include <notes/Notes.h>

/**************************************************
 * Kbd_pitch_class
 *************************************************/
class Kbd_pitch_class
	: virtual Logfacility_class
	, public kbd_state_struct
{
	typedef map<char, tuple<string, string>> chords_map_t;

	constexpr set<char> init_chord_keys( chords_map_t chords )
	{
		set<char> chars {};
		for( auto [ch, str] : chords )
		{
			chars.insert( ch );// @suppress("Invalid arguments")
		}
		assert( chars.size() > 0);
		return chars;
	}
	string				className			= "";
	interface_t*		sds_p				;

public:
	chords_map_t Chords_map		// @suppress("Invalid arguments")
	{
		{'Y',{ string(""  )	, string("single") }},
		{'X',{ string("43")	, string("Dur   ") }},
		{'C',{ string("34")	, string("Moll  ") }},
		{'V',{ string("343"), string("Dur+  ") }},
		{'B',{ string("433"), string("Moll+ ") }},
		{'N',{ string("<")	, string("Power ") }}
	};
	set<char>			chord_keys			= init_chord_keys( Chords_map );
											//{ 'y', 'x', 'c', 'v', 'b', 'n' };
	const array<string, kbd_rows>
						dflt_keyboard_keys	{  	string("A_S_DF_J_K_L") ,
												string("Q_W_ER_U_I_O") ,
												string("1_2_34_7_8_9")};
	array<string, kbd_rows>
						keyboard_keys		{ dflt_keyboard_keys };

	/*
	 https://www.delamar.de/songwriting/akkorde-lernen-49754/#akkorde-bestimmen
	 https://de.wikipedia.org/wiki/American_Standard_Code_for_Information_Interchange
	 */

	pitch_t				pitch 				= pitch_struct();
	string 				Chord				= get<0>(Chords_map['Y']);

	pitch_vec_t			Pitch_vec			{};


						Kbd_pitch_class		( interface_t* sds);
						Kbd_pitch_class		(); // keyboard_dialog
	virtual 			~Kbd_pitch_class	() 	= default;
	void 				SetPitch			( int key );
	void 				SetChord			( char key );
	string 				Get_note_str		();
	void 				Show_kbd_layout		( int8_t base_oct );
	void 				Kbd_pitch_Test		();

private:
} ;

/**************************************************
 * keyboardState_class
 *************************************************/
class keyboardState_class :
	public Kbd_pitch_class
{
	interface_t* 		sds;
	const range_T<uint>	sharps_range			{ 0, 3 }; // TODO reduced range
	const range_T<uint>	flats_range				{ 0, 2 }; // TODO reduced range
	frq_t				basefrq;

protected:
	const range_T<int>	Kbdoctave_range			{ 1, max_kbd_octave };

						keyboardState_class		( interface_t* _sds );
						~keyboardState_class() 	= default;

	void 				change_octave			( int inc );
	void 				set_octave				( int oct );
	void 				increase_sharps			();
	void 				increase_flats			();
	void 				reset_sharps			();
	void 				reset_flats				();
	void 				set_kbdbps				();
	void 				set_slideMode			();
	void 				set_accidental			( uint pitches, int dir  );
	void 				set_accidental			( step_vec_t vec  );

} ;

/**************************************************
 * Keyboard_class
 *************************************************/
class Keyboard_class
	: virtual public	Logfacility_class
	, virtual			osc_struct
	, virtual public	Kbd_base
	, virtual public	sdsstate_struct
	, virtual public	keyboardState_class
{
	string 				className 				= "";
	Oscgroup_class		Oscgroup				{ KBDROLE, 2*monobuffer_bytes };
	Oscillator*			Osc						= &Oscgroup.osc;
//	unique_ptr<Wavedisplay_class>	wd_p;

	Instrument_class* 	instrument_p			;
	interface_t*		sds_p					;
	Wavedisplay_class*	wd_p					;
	Storage_class*		sta_p					;
	vector<note_t>		Note_vec				{};
	uint8_t				Note_pos				= 0;
	file_structure*		fs						;
	Note_class*			Notes					;

	Scanner_class*		scanner					;

public:

	Data_t*				Kbd_Data;
	bool				Enabled					= false;

						Keyboard_class			( 	Instrument_class*,
													Storage_class*,
													Note_class* );
						Keyboard_class			(); // see comstack
	virtual 			~Keyboard_class			();
	void 				Dispatcher				( kbdInt_t key );
	void 				Set_instrument			();
	void 				Enable					( bool iskbd );
	void 				ScanData				();
	void 				Show_help				( bool tty );
	void				Set_key					();
	bool 				Save_notes				();

private:

	uint				max_notes				= 0;//notes_per_sec * 4*max_sec;
	const int 			releaseCounter			= 0;
	const int 			attackCounter 			= measure_parts;//rint( max_frames / min_frames );
	int 				decayCounter 			= 0;
	uint 				duration_counter 		= 0;// count the beats of note kbd_key
	uint				holdCounter				= 0;
	const uint			kbd_volume				= 75;
	uint8_t 			sta_volume				= kbd_volume;
	kbdkey_t			Kbd_key					{};
	feature_t			kbd_adsr				= feature_struct();
	bool				frqMode					= SLIDE;
	bool				kbd_trigger				= false;
	string				Noteline				{};
	uint				noteline_cnt			= 0;
	uint8_t				notes_per_sec			= 8;

	void				selfTest				();
	void 				attack					();
	void 				release					();
	bool 				decay					();
	void 				gen_chord_data			();
	// keyhandler.cpp

	void 				keyHandler				( kbdkey_t kbd );
	void 				notekey					( char key );
	void				set_bufferMode			( bool forget);
	void 				initNoteVector			();


};



#endif /* KEYBOARD_H_ */
