/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>

Keyboard_class::Keyboard_class( Instrument_class* instr ) :
	Logfacility_class("Keyboard"),
	Keyboard_base(),
	state_struct()
{
	className = Logfacility_class::className;
	this->instrument 	= instr;
	Oscgroup.SetWd( instr->wd_p );

}

Keyboard_class::Keyboard_class() :
	Logfacility_class("Keyboard"),
	Keyboard_base()
{
	className = Logfacility_class::className;
}

Keyboard_class::~Keyboard_class() = default;

void Keyboard_class::set_instrument(  )
{
	if ( not instrument ) return;
	// copy class Oscillator

	Oscgroup.osc 	= instrument->Oscgroup.osc;
	Oscgroup.vco	= instrument->Oscgroup.vco;
	Oscgroup.fmo 	= instrument->Oscgroup.fmo;

	Oscgroup.Set_Duration( max_milli_sec );
	Oscgroup.osc.Set_adsr( Oscgroup.osc.adsr );
	Oscgroup.osc.Reset_beat_cursor();
}

bool Keyboard_class::Decay(  )
{
	bool 		decay 		= ( decayCounter > releaseCounter );
	buffer_t	cursor 		= attackCounter - decayCounter;
	buffer_t 	frame_offset= min_frames * cursor;
				Kbd_Data 	= osc->KbdData( frame_offset );
	if( decay )
		decayCounter--;
//	cout <<  cursor ;

	return decay;

}


bool Keyboard_class::Attack()
{
	auto set_kdb_note 	= [ this ]( )
	{
		uint 	octave = instrument->sds->noteline_prefix.Octave;
		uint 	frqidx = FrqIndex( octave, note_key );
				note_name = frqNamesArray[ frqidx ];
				Oscgroup.Set_Osc_Note( frqidx, kbd_duration, kbd_volume, FIXED );
				cout << note_name << " ";
	};

	if (( note_key == NOKEY ) )
		return false;

	decayCounter = attackCounter;

	set_instrument();
	set_kdb_note(  );

	osc->Data_reset();
	Oscgroup.Run_OSCs( 0 );

	return true;
}

bool Keyboard_class::Release(  )
{
//	cout << "RELEASE " ;
	decayCounter = releaseCounter;

	return true;
}

int Keyboard_class::Kbdnote()
{
	// transforms anykey into notevalue

		keystruct 	= GetKey();

	int anykey 		= keystruct.key;
	if ( anykey == AppExit )
	{
		Comment( WARN, "Exit by user request");
		instrument->sds->Synthesizer = EXITSERVER;
	}
	// check if key is in set KbdNote
	size_t note 	= KbdNote.Str.find( anykey );
	if ( note == STRINGNOTFOUND )
	{
		return NOKEY; // not a note key
	}
	else
	{
		return (int)note;
	}

}

void Keyboard_class::KbdEvent( uint key )
{
	note_key = key;
	if ( not Decay() )
		Release();
	if ( Attack( ) )
	{
		Decay();
		Set_ch( 0 );
	}
}


