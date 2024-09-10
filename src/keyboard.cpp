/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>

Keyboard_class::Keyboard_class( Instrument_class* instr, Storage::Storage_class* StA  ) :
	Logfacility_class("Keyboard"),
	Oscillator_base(),
	Keyboard_base()
{
	this->instrument 	= instr;
	this->StA			= StA;
}

Keyboard_class::Keyboard_class() :
	Logfacility_class("Keyboard"),
	Oscillator_base(),
	Keyboard_base()
{

}

Keyboard_class::~Keyboard_class(){};

void Keyboard_class::setup(  )
{
	if ( not instrument ) return;
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

bool Keyboard_class::decay( int key )
{
	if( prevKey == key )
		return true;
	else
		return false;

}

bool Keyboard_class::Attack( int key, uint8_t amp )
{
	auto set_osc_frequency 	= [ this, key ]( Oscillator* osc )
		{
			int Octave = 3;
			float 	freq 	= this->Calc_frequency( Octave*12 + key );//osc->wp.frequency * ( 12 + key ) / 12.0  ;
			osc->Set_frequency( freq );
			osc->OSC( 0 );
		};

	if ( not StA ) return false;
	if ( key == NOKEY ) return false;
	if ( decay( key ) ) return false;

	setup();
	for ( Oscillator* osc : osc_group )
		set_osc_frequency( osc );
/*
	StA->reset_counter();
	StA->status.store 	= true;
	StA->status.play 	= true;
	StA->Amp 			= amp;
	StA->store_block(main.Mem.Data);
*/
	prevKey = key;
	return true;
}

bool Keyboard_class::Release( int key )
{
	if ( not StA ) return false;
	if (( key == NOKEY ) and ( prevKey != NOKEY ))
//	if ( true )
	{

		StA->Reset_counter();
		StA->Play_mode( false );

		prevKey 			= NOKEY;
		StA->clear_data( 0 );
		return true;
	}
	else
	{
		prevKey = key;
		return false;
	}
}

int Keyboard_class::Kbdnote()
{
	// transforms anykey into notevalue

	keystruct = GetHold();

	int anykey = keystruct.key;
//	cout << anykey;

	// check if key is in set KbdNote
	size_t note = KbdNote.find( anykey );
	if ( note == STRINGNOTFOUND )
	{
		return NOKEY; // not a note key
	}
	else
	{
		return (int)note;
	}

}




