/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>

Keyboard_class::Keyboard_class( Instrument_class* instr, Storage_class* StA ) :
	Logfacility_class("Keyboard"),
	Keyboard_base(),
	state_struct(),
	scanner( StA->Data, min_frames, 2*max_frames )
{
	className 			= Logfacility_class::className;
	this->instrument 	= instr;
	this->sds			= instrument->sds;
	this->StA			= StA;
	this->Kbd_Data		= StA->Data;
	Oscgroup.SetWd		( instr->wd_p, &Osc->wp.frames );
	instr->wd_p->Add_data_ptr( Osc->typeId, Osc->roleId, StA->Data, &scanner.max_pos );
	ASSERTION( 	StA->mem_ds.data_blocks == 2*max_frames, "StA->mem_ds.data_blocks",
			StA->mem_ds.data_blocks , 2*max_frames);
	ASSERTION( 	StA->mem_ds.data_blocks == scanner.max_pos, "scanner.max_pos",
			StA->mem_ds.data_blocks , scanner.max_pos );

}

Keyboard_class::Keyboard_class() :
	Logfacility_class("Keyboard"),
	Keyboard_base(),
	state_struct(),
	scanner( nullptr, 0, 0 )


{
	className 			= Logfacility_class::className;
	instrument 			= nullptr;
	sds					= nullptr;
}

Keyboard_class::~Keyboard_class() = default;


auto show_note = [  ]( uint idx, uint& cnt )
{
	string	note_name 			= frqNamesArray[ idx ];
			CursorHomeCol();
			cout.flush() << note_name ;// << "(" << frqidx << ")";
			if ( cnt > 0 )
				cout.flush() << "," << cnt << "|";
			cnt = 0;
};
void Keyboard_class::set_kdb_note()
{
	if ( Kbd_key.nokey )
		return;
	uint 	frqidx 	= Index( base_octave, kbd_note );
			Oscgroup.Set_Frequency( frqidx, frqMode );
			Osc->Set_volume( kbd_volume, FIXED );
			show_note( frqidx, duration_counter );
}

void Keyboard_class::Enable()
{
	if( is_atty )
	{
		sds->StA_state[ STA_KEYBOARD ].play 	= true;
		sds->StA_amp_arr[ STA_KEYBOARD ] 		= 75;
		sds->mixer_status.kbd					= true;
		Set_instrument();
		Info( "Keyboard is enabled");
	}
	else
	{
		sds->StA_state[ STA_KEYBOARD ].play 	= false;
		sds->StA_amp_arr[ STA_KEYBOARD ] 		= 0;
		sds->mixer_status.kbd					= false;
	}
}
void Keyboard_class::Set_instrument( )
{
	if ( not instrument )
	{
		Comment( ERROR, "Undefined keyboard instrument");
		return;
	}
	Comment(DEBUG, "Update Instrument ");
	Oscgroup.Instrument_fromSDS( sds );
	Oscgroup.Set_Connections( sds );
	Oscgroup.SetSlide( sliding * slidingstep );
	Oscgroup.Set_Duration( max_msec );
	osc_frames	= Osc->wp.frames;
	Osc->Set_adsr( Osc->adsr );
}

bool Keyboard_class::decay(  )
{
	bool 		decay 		= ( decayCounter > releaseCounter  );
	if( decay )
	{
		duration_counter++;
		decayCounter--;
	}

	return decay;
}


bool Keyboard_class::attack()
{
	if (( kbd_note == NONOTE ) )
	{
		return false;
	}
	if ( ( Kbd_key.hold ) and ( decayCounter > releaseCounter) )
	{
		decayCounter++;
		cout << "-" ;
		return true;
	}
	if( ADSR_flag )
		Osc->Reset_beat_cursor();

	Oscgroup.Data_Reset();

	Set_instrument();
	set_kdb_note();
	Oscgroup.Phase_Reset();
	ASSERTION(	Osc->mem_frames >= max_frames, "Osc->mem_frames",
				Osc->mem_frames, max_frames);
	Oscgroup.Run_OSCs( 0 );

/*	int del = scanner.pos ;
	for ( buffer_t n = 0; n < Osc->wp.frames; n++ )
	{
		StA->Data[ del ] = 0;
		del = del - 1;
		if ( del < 0 ) del = scanner.max_pos;
	}
	*/
	buffer_t offs = scanner.pos;
	assert( osc_frames == max_frames );
	for ( buffer_t n = 0; n < max_frames; n++ )
	{
		StA->Data[ offs ] += Osc->Mem.Data[n];
		offs = ( offs + 1 ) % ( 2*max_frames );
	}



	decayCounter	= attackCounter ;

	return 	true;
}

void Keyboard_class::release(  )
{
//	Oscgroup.Data_Reset();
	return ;
}

bool Keyboard_class::Set_Kbdnote()
{
	if ( not is_atty )
		return false;

	auto noteStep = [ this ]( int key )
	{
		// transforms anykey into notevalue and
		// check if key is in set KbdNote

		int 	KEY 	= toupper( key );
		size_t	step 	= kbdNotes.Str.find( KEY );
		if ( step 	==	STRINGNOTFOUND )
			return 		NONOTE;
		else
			return (char)step;
	};

	Kbd_key		=  GetKeystruct( false );

	kbd_note 	=  noteStep( Kbd_key.key );
	if ( kbd_note == NONOTE )
	{
		keyHandler( Kbd_key );
	}
	else
	{
		NoteEvent();
	}
	return true; // is_atty
}

void Keyboard_class::NoteEvent( )
{
	attack( );
	if ( ADSR_flag )
		if ( not decay() )
			release();
}

void Keyboard_class::ScanData()
{
	assert( scanner.inc == min_frames );
	assert( sds->audioframes == scanner.inc );
	Kbd_Data = scanner.next();
}

