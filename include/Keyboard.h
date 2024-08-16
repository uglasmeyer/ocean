/*
 * keyboard.h
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <synthesizer.h>
#include <synthmem.h>
#include <Logfacility.h>
#include <osc.h>
#include <Instrument.h>
#include <mixer.h>

using namespace Storage;

class Keyboard_class :	virtual public Logfacility_class,
						virtual public Oscillator_base
{

public:

	const string		KbdNote		{ "SDRFTGHUJIKOL" };

	Oscillator 			main		{ KBDID };
	Oscillator 			vco			{ VCOID };
	Oscillator 			fmo			{ FMOID };

	vector<Oscillator*> osc_group { &vco, &fmo, &main };

	Instrument_class* 	instrument;
	Storage_class* 		StA;

	char 				prevKey 	= 0;

	Keyboard_class( Instrument_class*, Storage_class* );
	~Keyboard_class();

	void Setup(  );
	void Attack( char, uint8_t );
	void Release();
	size_t Kbdnote( char );

};



#endif /* KEYBOARD_H_ */
