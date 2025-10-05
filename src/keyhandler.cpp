/*
 * keyhandler.cpp
 *
 *  Created on: May 10, 2025
 *      Author: sirius
 */

#include <Keyboard.h>
#include <cstring> // strcpy

/*
 * keyboardState_class
 */

keyboardState_class::keyboardState_class( interface_t* _sds) :
	Kbd_pitch_class( _sds )

{
	sds 	 				= _sds;
	base_octave				= sds->Kbd_state.base_octave;
	sharps					= sds->Kbd_state.sharps;
	ADSR_flag				= sds->Kbd_state.ADSR_flag;
	sliding					= sds->Kbd_state.sliding;
	change_octave			( 0 );
};


void keyboardState_class::change_octave( int inc )
{
	base_octave					= check_range( 	Kbdoctave_range,
												base_octave  + inc,
												"change_octave" );
	sds->Kbd_state.base_octave	= base_octave;
	basefrq						= frqArray[ frqIndex( 0, base_octave ) ];
}
void keyboardState_class::set_octave( int oct )
{
	base_octave					= check_range( 	Kbdoctave_range, oct, "change_octave" );
	basefrq						= frqArray[ frqIndex( 0, base_octave ) ];
}
void keyboardState_class::set_accidental( uint loc, int dir  )
{
	for( uint oct = 0; oct < kbd_rows; oct++)
	{
		keyboard_keys[oct][ (loc + dir)%oct_steps ] = keyboard_keys[oct][ loc ];
		keyboard_keys[oct][ loc     ]	= '_';
	}
};
void keyboardState_class::set_accidental( step_vec_t vec  )
{
	keyboard_keys		= dflt_keyboard_keys;

	for( uint n = 0; n < sharps; n++ )
	{
		int8_t dir = 1;
		for( uint row = 0; row < kbd_rows; row++)
		{
			uint8_t keyloc = vec[n];
			uint8_t newloc= (keyloc+dir) % oct_steps;
			keyboard_keys[row][newloc] = keyboard_keys[row][ keyloc ];
			keyboard_keys[row][ keyloc ]	= '_';
		}
	}
	for( uint n = 0; n < flats; n++ )
	{
		int8_t dir = -1;
		for( uint row = 0; row < kbd_rows; row++)
		{
			uint8_t keyloc = vec[n] ;
			int8_t newloc	= keyloc + dir;
			if( ( newloc ) < 0 )
				newloc = oct_steps - 1;
			keyboard_keys[row][newloc] = keyboard_keys[row][ keyloc ];
			keyboard_keys[row][ keyloc     ]	= '_';
		}
	}
};
Note_base Nb {};
void keyboardState_class::increase_sharps()
{
	sharps 						= check_range( sharps_range, sharps + 1, "increase_sharps" );
	sds->Kbd_state.sharps 		= sharps;

	if ( sharps > 0 )
	{
		uint location			= ( Nb.sharp_pitch[ sharps-1 ] ) ;
		set_accidental( location, 1 );
	}

}

void keyboardState_class::increase_flats()
{
	flats 						= check_range( flats_range, flats + 1, "increase_flats" );
	sds->Kbd_state.flats 		= flats;
	if ( flats > 0 )
	{
		uint location			= ( Nb.flat_pitch[ flats-1 ] ) ;
		set_accidental( location, -1 );
	}

}
void keyboardState_class::reset_sharps()
{
	keyboard_keys				= dflt_keyboard_keys;
	sharps 						= 0;
	sds->Kbd_state.sharps 		= 0;
}
void keyboardState_class::reset_flats()
{
	keyboard_keys				= dflt_keyboard_keys;
	flats 						= 0;
	sds->Kbd_state.flats 		= 0;
}
void keyboardState_class::toggle_applyADSR()
{
	// overwrite Instrument beat counter
	ADSR_flag 					= not ADSR_flag;
	sds->Kbd_state.ADSR_flag 	= ADSR_flag;
}
void keyboardState_class::set_slideMode()
{
	sliding 					= not sliding;
	sds->Kbd_state.sliding 		= sliding;
}


/*
 * Keyboard_class
 */



void Keyboard_class::Show_help( bool tty )
{
	if ( not tty ) return;
	int base_oct = (int) sds_p->Kbd_state.base_octave;

	Show_kbd_layout( base_oct ) ;
	Info( "Instrument: ", instrument_p->Name );
	Table_class Table { "Keyboard ", 11 };
	Table.AddColumn( "Keys"		, 9 );
	Table.AddColumn( "Function"	, 30 );
	Table.AddColumn( "State"	, 10 );
	Table.AddColumn( "Unit"		, 10 );
	Table.PrintHeader();

	Table.AddRow( "F1", "keyboard Keys"			);
	Table.AddRow( "F2", "increase sharps "		, (int) sds_p->Kbd_state.sharps, "#" );
	Table.AddRow( "F3", "reset sharps "			);
	Table.AddRow( "F4", "toggle decay mode "	, (int) sds_p->Kbd_state.ADSR_flag * sds_p->adsr_arr[OSCID].bps ,"B psec");
	Table.AddRow( "F5", "slide duration "		, to_string ((int)sds_p->Kbd_state.sliding *
													Osc->features.glide_effect), "[ % ]");
	Table.AddRow( "F6", "Keyboard buffer "		, bool_str( sta_p->state.forget, "forget", "persist" ));
	Table.AddRow( "F7", "increase flats "		, (int) sds_p->Kbd_state.flats, "b" );
	Table.AddRow( "F8", "reset flats "			);
	Table.AddRow( "F9", "save notes"			);
	Table.AddRow( "+/-" , "inc/dec octave"		, base_oct, "base"  );
	Table.AddRow( "#" , "show Frequency Table" 	);
	Table.AddRow( "ESC", "Exit keyboard"		);
}


void Keyboard_class::notekey( char ch )
{
	SetChord( ch );
	SetPitch( ch ) ;

	if ( Pitch_vec.size() > 0 )
	{
		Set_instrument();

		if( sds_p->StA_amp_arr[STA_KEYBOARD] == 0 )
		{
			sds_p->StA_amp_arr[STA_KEYBOARD] = sta_volume;
		}
	}
}
void Keyboard_class::set_bufferMode( bool forget )
{
	sta_p->state.forget 	= forget ;
	if( forget )
	{
		sta_volume = sds_p->StA_amp_arr[STA_KEYBOARD];
		sds_p->StA_amp_arr[STA_KEYBOARD] = 0;
	}
	else
	{
		sds_p->StA_amp_arr[STA_KEYBOARD] = sta_volume;
	}
	Note_vec.clear();
	Noteline 			= "";
}

void Keyboard_class::Set_key( ) // TODO
{

	bool forget			= sds_p->StA_state[STA_KEYBOARD].forget;
	base_octave 		= sds_p->Kbd_state.base_octave;
	sliding 			= sds_p->Kbd_state.sliding;
	sharps	 			= sds_p->Kbd_state.sharps;
	flats 				= sds_p->Kbd_state.flats;
	ADSR_flag 			= sds_p->Kbd_state.ADSR_flag;

	set_octave			( base_octave );
	set_accidental		( Nb.sharp_pitch );
	set_accidental		( Nb.flat_pitch );
	set_bufferMode		( not forget );
	sds_p->StA_state[ STA_KEYBOARD ].forget = forget;
	if( sds_p->StA_amp_arr[STA_KEYBOARD] == 0 )
	{
		sds_p->StA_amp_arr[STA_KEYBOARD] = sta_volume;
	}

	kbdInt_t key 		= sds_p->Kbd_state.key;
	sds_p->Kbd_state.key= 0;
	Dispatcher			( key );
}


Frequency_class Frequency {};
void Keyboard_class::keyHandler( kbdkey_t kbd )
{
	string 	noteline 	= "";
	bool 	tainted 	= true;
	sds_p->Kbd_state.key = 0;
	switch (kbd.Int )
	{
		case '+' :	{ change_octave(  1 ) 		; break; }
		case '-' :	{ change_octave( -1 ) 		; break; }
		case '#' :	{ tainted = false;Frequency.ShowFrqTable()	; break; }
		case F1	:	{ 					; break; }
		case F2 : 	{ increase_sharps()	; break; }
		case F3 : 	{ reset_sharps(); 	; break; }
		case F4 :	{ toggle_applyADSR(); break; }
		case F5 : 	{ set_slideMode(); 		break; }
		case F6	: 	{ set_bufferMode( not sta_p->state.forget );		break; }
		case F7 : 	{ increase_flats();		break; }
		case F8 : 	{ reset_flats(); 			break; }
		case F9 : 	{ tainted = not Save_notes();	break; }
																															//				default :	{ tainted = true	; break; }
		default : 	{ notekey( kbd.Int );
						tainted	= false;
						break; }
	}
	if ( tainted )
	{
		ClearScreen();
		Show_help( is_atty );
	}
}
