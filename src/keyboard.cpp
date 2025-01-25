/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>

Keyboard_class::Keyboard_class( Instrument_class* instr ) :
	Logfacility_class("Keyboard"),
	Keyboard_base()
{
	this->instrument 	= instr;
	Oscgroup.SetWd( instr->wd_p );

}

Keyboard_class::Keyboard_class() :
	Logfacility_class("Keyboard"),
	Keyboard_base()
{

}

Keyboard_class::~Keyboard_class()
{
};


void Keyboard_class::set_instrument(  )
{
	if ( not instrument ) return;
	// copy class Oscillator
	Oscgroup.osc.wp 			= instrument->Oscgroup.osc.wp;
	Oscgroup.osc.vp 			= instrument->Oscgroup.osc.vp;
	Oscgroup.osc.vp.data		= Oscgroup.vco.Mem.Data;
	Oscgroup.osc.fp 			= instrument->Oscgroup.osc.fp;
	Oscgroup.osc.fp.data		= Oscgroup.fmo.Mem.Data;
	Oscgroup.osc.adsr 		= instrument->Oscgroup.osc.adsr;
	Oscgroup.osc.adsr.bps	= 1;
	Oscgroup.osc.spectrum	= instrument->Oscgroup.osc.spectrum;

	Oscgroup.vco.wp 			= instrument->Oscgroup.vco.wp;
	Oscgroup.vco.vp 			= instrument->Oscgroup.vco.vp;
	Oscgroup.vco.fp 			= instrument->Oscgroup.vco.fp;
	Oscgroup.vco.spectrum	= instrument->Oscgroup.vco.spectrum;

	Oscgroup.fmo.wp 			= instrument->Oscgroup.fmo.wp;
	Oscgroup.fmo.vp 			= instrument->Oscgroup.fmo.vp;
	Oscgroup.fmo.fp 			= instrument->Oscgroup.fmo.fp;
	Oscgroup.fmo.spectrum	= instrument->Oscgroup.fmo.spectrum;

}

bool Keyboard_class::Decay(  )
{
	if( decayCounter > releaseCounter )
		decayCounter--;
	return ( decayCounter > releaseCounter );

}


bool Keyboard_class::Attack( int key, uint8_t octave )
{
	auto set_kdb_note 	= [ this, key, octave ]( )
		{
			Note_base::pitch_t pitch = pitch_struct();
			pitch.octave 	= octave;
			pitch.step 		= key ;
			Oscgroup.Set_Osc_Note(pitch, duration, 100 );
		};

	if (( key == NOKEY ) or ( decayCounter > releaseCounter ))
		return false;

	decayCounter = attackCounter;

	set_instrument();
	set_kdb_note(  );
	Oscgroup.osc.Set_adsr( Oscgroup.osc.adsr );

	Oscgroup.Run_Oscgroup( 0 );

	return true;
}

bool Keyboard_class::Release(  )
{
	decayCounter = releaseCounter;
	Oscgroup.osc.Mem.Clear_data( 0 );
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




