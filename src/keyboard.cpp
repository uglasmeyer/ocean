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
	state_struct(),
	Oscgroup_class( osc_struct::KBDID )
{
	className = Logfacility_class::className;
	this->instrument 	= instr;
	SetWd( instr->wd_p );

}

Keyboard_class::Keyboard_class() :
	Logfacility_class("Keyboard"),
	Keyboard_base(),
	state_struct(),
	Oscgroup_class( osc_struct::KBDID )
{
	className = Logfacility_class::className;
}

Keyboard_class::~Keyboard_class() = default;

void Keyboard_class::set_instrument(  )
{
	if ( not instrument ) return;
	// copy class Oscillator
	osc 	= *instrument->osc;
	vco		= *instrument->vco;
	fmo 	= *instrument->fmo;

	Set_Duration( max_msec );
	osc.Set_adsr( osc.adsr );
	osc.Reset_beat_cursor();
}

bool Keyboard_class::decay(  )
{
	bool 		decay 		= ( decayCounter > releaseCounter );
	buffer_t	cursor 		= attackCounter - decayCounter;
	buffer_t 	frame_offset= min_frames * cursor;
				Kbd_Data 	= osc.GetData_p( frame_offset );
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
		uint 	frqidx = Index( octave, note_key );
				note_name = frqNamesArray[ frqidx ];
				Set_Osc_Note( frqidx, kbd_duration, kbd_volume, FIXED );
				cout << note_name << " ";
	};

	if (( note_key == NOKEY ) )
		return false;

	buffer_t offs = ( attackCounter - decayCounter ) * min_frames;
	osc.Shift_data( offs );

	decayCounter = attackCounter;

	set_instrument();
	set_kdb_note(  );

//	osc->Data_reset();
	Run_OSCs( 0 );

	return true;
}

bool Keyboard_class::release(  )
{
//	cout << "RELEASE " ;
	decayCounter = releaseCounter;
	osc.Data_reset();

	return true;
}

int Keyboard_class::Kbdnote()
{
	// transforms anykey into notevalue

	if ( not is_a_tty ) return 0;
	keystruct 	= GetKey();

	if ( keystruct == AppExit )
	{
		Comment( WARN, "Exit by user request");
		instrument->sds->Synthesizer = EXITSERVER;
	}
	int anykey 		= keystruct.key;
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
	if ( not is_a_tty ) return;
	note_key = key;
	if ( not decay() )
		release();
	if ( Attack( ) )
	{
		decay();
		Set_ch( 0 );
	}
}


