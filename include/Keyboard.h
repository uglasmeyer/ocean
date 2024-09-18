/*
 * keyboard.h
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <Logfacility.h>
#include <Osc.h>
#include <Instrument.h>
#include <Kbd.h>
#include <Mixer.h>
#include <Synthesizer.h>
#include <Synthmem.h>

using namespace Storage;

class Keyboard_class :	virtual  Logfacility_class,
						virtual  Oscillator_base,
						virtual  Note_base,
						virtual  public Keyboard_base
{

public:

	const string		KbdNote		{ "SDRFTGHUJIKOL" };

	Oscillator 			osc		{ KBDID };
	Oscillator 			vco			{ VCOID };
	Oscillator 			fmo			{ FMOID };

	vector<Oscillator*> osc_group { &vco, &fmo, &osc };

	Instrument_class* 	instrument	= nullptr;
	key_struct_t		keystruct 	= key_struct();
	const int			NOKEY 		= -1;
	int 				prevKey 	= NOKEY;

	Keyboard_class( Instrument_class* );
	Keyboard_class(); // see comstack
	~Keyboard_class();

	bool Attack( int, uint8_t );
	bool Release(  );
	bool Decay(  );

	int	 Kbdnote( );
	bool Counter( int );

private:
	int releaseCounter	= 0;
	int decayCounter 	= 0;
	int attackCounter 	= 7;

	void setup(  );

};



#endif /* KEYBOARD_H_ */
