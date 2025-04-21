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


class Keyboard_class :	virtual  Logfacility_class,
						virtual  Note_base,
						virtual  public Keyboard_base
{

public:

	const String		KbdNote		{ "SDRFTGHUJIKOL" };

	Oscgroup_class		Oscgroup	{ osc_struct::KBDID };
	Oscillator*			osc			= &Oscgroup.osc;
	Oscillator*			vco			= &Oscgroup.vco;
	Oscillator*			fmo			= &Oscgroup.fmo;


	Instrument_class* 	instrument	= nullptr;
	key_struct_t		keystruct 	= key_struct();
	const int			NOKEY 		= -1;
	int 				prevKey 	= NOKEY;
	Data_t*				Kbd_Data	= osc->KbdData( 0 );

	Keyboard_class( Instrument_class* );
	Keyboard_class(); // see comstack
	virtual ~Keyboard_class();

	bool Attack(  );
	bool Release(  );
	bool Decay(  );
	int	 Kbdnote( );
	void KbdEvent();


private:
	const int 	releaseCounter	= 0;
	const int 	attackCounter 	= rint( max_frames / min_frames );
	int 		decayCounter 	= 0;
	const uint	kbd_duration	= max_milli_sec; // [millisec]
	const uint  kbd_volume		= 75;
	int 		note_key 		= NOKEY;

	void 		set_instrument( );

};



#endif /* KEYBOARD_H_ */
