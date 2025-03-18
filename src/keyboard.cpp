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

	Oscgroup.osc 	= instrument->Oscgroup.osc;
	Oscgroup.vco	= instrument->Oscgroup.vco;
	Oscgroup.fmo 	= instrument->Oscgroup.fmo;

	Oscgroup.osc.vp.data = Oscgroup.vco.Mem.Data;
	Oscgroup.osc.fp.data = Oscgroup.fmo.Mem.Data;
	Oscgroup.osc.Set_duration( 500 );
	Oscgroup.osc.Set_adsr( Oscgroup.osc.adsr );
	Oscgroup.osc.Reset_cursor();
}

bool Keyboard_class::Decay(  )
{
	bool decay = ( decayCounter > releaseCounter );
	if( decay )
		decayCounter--;
	cout << "DECAY " << (int)decayCounter << endl;

	return decay;

}


bool Keyboard_class::Attack( int key, uint8_t octave )
{
	auto set_kdb_note 	= [ this, key, octave ]( )
		{
			Note_base::pitch_t pitch = pitch_struct();
			pitch.octave 	= octave;
			pitch.step 		= key ;
			int frqidx = GetFrqIndex( pitch );
			Oscgroup.Set_Osc_Note( frqidx, kbd_duration, kbd_volume, instrument->sds->slidermode );
		};

	if (( key == NOKEY ) or ( decayCounter > releaseCounter ))
		return false;

	cout << "ATTACK KEY: " << key << endl;
	decayCounter = attackCounter;

	set_instrument();
	set_kdb_note(  );

	Oscgroup.Run_Oscgroup( 0 );

	return true;
}

bool Keyboard_class::Release(  )
{
	cout << "RELEASE" << endl;
	decayCounter = releaseCounter;
	osc->Mem.Clear_data( 0 );

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




