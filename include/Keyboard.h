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

class Keyboard_class :	virtual  Logfacility_class,
						virtual  Oscillator_base,
						virtual  Note_base,
						virtual  public Keyboard_base
{

public:

	const String		KbdNote		{ "SDRFTGHUJIKOL" };
	Oscillator 			osc			{ KBDID };
	Oscillator 			vco			{ VCOID };
	Oscillator 			fmo			{ FMOID };

	vector<Oscillator*> osc_group { &vco, &fmo, &osc };
	constexpr static  string
						oscgrouo_name = wavedisplay_struct().names[KBDID];

	Instrument_class* 	instrument	= nullptr;
	key_struct_t		keystruct 	= key_struct();
	const int			NOKEY 		= -1;
	int 				prevKey 	= NOKEY;

	Keyboard_class( Instrument_class* );
	Keyboard_class(); // see comstack
	virtual ~Keyboard_class();

	bool Attack( int, uint8_t, uint8_t );
	bool Release(  );
	bool Decay(  );
	int	 Kbdnote( );

private:
	const int 	releaseCounter	= 0;
	const int 	attackCounter 	= 7; // TODO depends on the performance
	int 		decayCounter 	= 0;

	void setup(  );

};



#endif /* KEYBOARD_H_ */
