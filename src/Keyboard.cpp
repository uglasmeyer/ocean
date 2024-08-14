/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>

Keyboard_class::Keyboard_class( Instrument_class* instr ) :
	Logfacility_class("Keyboard"),
	Oscillator_base()
{
	this->instrument = instr;
}

Keyboard_class::~Keyboard_class(){};

void Keyboard_class::Set_osc_track(  )
{
	Comment( INFO, "Set osc track");

	// copy class Oscillator
	main_osc.wp 		= instrument->main.wp;
	main_osc.vp 		= instrument->main.vp;
	main_osc.fp 		= instrument->main.fp;
	main_osc.vp.data	= vco_osc.Mem.Data;
	main_osc.fp.data	= fmo_osc.Mem.Data;
	main_osc.adsr 		= instrument->main.adsr;
	main_osc.spectrum	= instrument->main.spectrum;

	vco_osc.wp 			= instrument->vco.wp;
	vco_osc.vp 			= instrument->vco.vp;
	vco_osc.fp 			= instrument->vco.fp;
	vco_osc.spectrum	= instrument->vco.spectrum;

	fmo_osc.wp 			= instrument->fmo.wp;
	fmo_osc.vp 			= instrument->fmo.vp;
	fmo_osc.fp 			= instrument->fmo.fp;
	fmo_osc.spectrum	= instrument->fmo.spectrum;


}




