/*
 * keyboard.h
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <synthesizer.h>
#include <Logfacility.h>
#include <osc.h>
#include <Instrument.h>

class Keyboard_class :	virtual public Logfacility_class,
						virtual public Oscillator_base
{

public:
	Oscillator main_osc{ "KBD" };
	Oscillator vco_osc{ "VCO" };
	Oscillator fmo_osc{ "FMO" };
	Instrument_class* instrument;

	Keyboard_class( Instrument_class* );
	~Keyboard_class();

	void Set_osc_track(  );


};



#endif /* KEYBOARD_H_ */
