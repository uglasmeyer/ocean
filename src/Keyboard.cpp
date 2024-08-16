/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>

Keyboard_class::Keyboard_class( Instrument_class* instr, Storage::Storage_class* StA  ) :
	Logfacility_class("Keyboard"),
	Oscillator_base()
{
	this->instrument 	= instr;
	this->StA			= StA;
}

Keyboard_class::~Keyboard_class(){};

void Keyboard_class::Setup(  )
{


	// copy class Oscillator
	main.wp 		= instrument->main.wp;
	main.vp 		= instrument->main.vp;
	main.vp.data	= vco.Mem.Data;
	main.fp 		= instrument->main.fp;
	main.fp.data	= fmo.Mem.Data;
	main.adsr 		= instrument->main.adsr;
	main.spectrum	= instrument->main.spectrum;

	vco.wp 			= instrument->vco.wp;
	vco.vp 			= instrument->vco.vp;
	vco.fp 			= instrument->vco.fp;
	vco.spectrum	= instrument->vco.spectrum;

	fmo.wp 			= instrument->fmo.wp;
	fmo.vp 			= instrument->fmo.vp;
	fmo.fp 			= instrument->fmo.fp;
	fmo.spectrum	= instrument->fmo.spectrum;
}

void Keyboard_class::Attack( char key, uint8_t amp )
{
	auto set_osc_frequency 	= [ key ]( Oscillator* osc )
		{
			float 	freq 	= osc->wp.frequency * ( 12 + key ) / 12.0  ;
			osc->set_frequency( freq );
			osc->OSC( 0 );
		};

	if ( key == prevKey ) return;
	Comment( INFO, "Attack: " + NoteName[ (int)key ] );

	Setup();
	for ( Oscillator* osc : osc_group )
		set_osc_frequency( osc );

	StA->reset_counter();
	StA->status.store 	= true;
	StA->status.play 	= true;
	StA->Amp 			= amp;
	prevKey 			= key;
	StA->store_block(main.Mem.Data);
}

void Keyboard_class::Release()
{
	if ( prevKey != 0 )
		Comment( INFO, "Release: " + NoteName[ (int)prevKey ] );

	StA->reset_counter();

	StA->status.play 	= false;
	StA->status.store 	= false;

	prevKey 			= 0;
	StA->clear_data( 0 );

}

size_t Keyboard_class::Kbdnote( char key)
{
	// check if key is in set KbdNote
	return KbdNote.find( key );
}




