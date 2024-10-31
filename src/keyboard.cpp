/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>

Keyboard_class::Keyboard_class( Instrument_class* instr ) :
	Logfacility_class("Keyboard"),
	Oscillator_base(),
	Keyboard_base()
{
	this->instrument 	= instr;
}

Keyboard_class::Keyboard_class() :
	Logfacility_class("Keyboard"),
	Oscillator_base(),
	Keyboard_base()
{

}

Keyboard_class::~Keyboard_class()
{
};


void Keyboard_class::setup(  )
{
	if ( not instrument ) return;
	// copy class Oscillator
	osc.wp 			= instrument->main.wp;
	osc.vp 			= instrument->main.vp;
	osc.vp.data		= vco.Mem.Data;
	osc.fp 			= instrument->main.fp;
	osc.fp.data		= fmo.Mem.Data;
	osc.adsr 		= instrument->main.adsr;
	osc.spectrum	= instrument->main.spectrum;

	vco.wp 			= instrument->vco.wp;
	vco.vp 			= instrument->vco.vp;
	vco.fp 			= instrument->vco.fp;
	vco.spectrum	= instrument->vco.spectrum;

	fmo.wp 			= instrument->fmo.wp;
	fmo.vp 			= instrument->fmo.vp;
	fmo.fp 			= instrument->fmo.fp;
	fmo.spectrum	= instrument->fmo.spectrum;
}

bool Keyboard_class::Decay(  )
{
	if( decayCounter > releaseCounter ) decayCounter--;
	return ( decayCounter > releaseCounter );

}

bool Keyboard_class::Attack( int key, uint8_t octave, uint8_t amp )
{
	auto set_osc_frequency 	= [ this, key, octave ]( Oscillator* osc )
		{
			float 	freq 	= this->Calc_frequency( octave*12 + key );//osc->wp.frequency * ( 12 + key ) / 12.0  ;
			osc->Set_frequency( freq );
			osc->OSC( 0 );
		};

	if (( key == NOKEY ) or ( decayCounter > releaseCounter ))
		return false;

	decayCounter = attackCounter;

	setup();
	for ( Oscillator* osc : osc_group )
		set_osc_frequency( osc );

	return true;
}

bool Keyboard_class::Release(  )
{
	decayCounter = releaseCounter;
	osc.Mem.Clear_data( 0 );
	return true;
}

int Keyboard_class::Kbdnote()
{
	// transforms anykey into notevalue

	keystruct = GetKey();

	int anykey = keystruct.key;
	if ( anykey == AppExit )
	{
		Comment( WARN, "Exit by user request");
		raise( SIGINT );
	}
	// check if key is in set KbdNote
	size_t note = KbdNote.Str.find( anykey );
	if ( note == STRINGNOTFOUND )
	{
		return NOKEY; // not a note key
	}
	else
	{
		return (int)note;
	}

}




