/*
 * keyhandler.cpp
 *
 *  Created on: May 10, 2025
 *      Author: sirius
 */

#include <Keyboard.h>

/*
 * keyboardState_class
 */

keyboardState_class::keyboardState_class( interface_t* _sds )
{
	sds 	 				= _sds;
	kbd_note.base_octave	= sds->Kbd_state.base_octave;
	sharps					= sds->Kbd_state.sharps;
	ADSR_flag				= sds->Kbd_state.ADSR_flag;
	sliding					= sds->Kbd_state.sliding;
	change_octave			( 0 );
};


frq_t keyboardState_class::Get_basefrq()
{
	return basefrq;
}

void keyboardState_class::change_octave( int inc )
{
	kbd_note.base_octave		= check_range( Kbdoctave_range, kbd_note.base_octave  + inc, "change_octave" );
	sds->Kbd_state.base_octave	= kbd_note.base_octave;
	basefrq						= frqArray[ frqIndex( 0, kbd_note.base_octave ) ];
}
void keyboardState_class::set_accidental( uint loc, int dir  )
{
	for( uint oct = 0; oct < kbd_octaves; oct++)
	{
		kbd_note.keyboard_keys[oct][ (loc + dir)%oct_steps ] = kbd_note.keyboard_keys[oct][ loc ];
		kbd_note.keyboard_keys[oct][ loc     ]	= '_';
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
	kbd_note.keyboard_keys		= kbd_note.dflt_keyboard_keys;
	sharps 						= 0;
	sds->Kbd_state.sharps 		= 0;
}
void keyboardState_class::reset_flats()
{
	kbd_note.keyboard_keys			= kbd_note.dflt_keyboard_keys;
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


string Keyboard_class::get_notenames( )
{
	Table_class Table { "Notes", 11 };
	Table.AddColumn( "Octave", 8);
	Table.AddColumn( " Note names", 3*7 );
	Table.PrintHeader();
	stringstream 	strs 	{};
	for ( uint n = 0; n < kbd_octaves; n++ )
	{
		string 		str 	{};
		uint oct 			= kbd_octaves - n - 1;
		for ( uint step = 0 ; step < oct_steps  ; step++ )
		{
			if( kbd_note.keyboard_keys[oct][step] != '_' )
			{
				str 		= str + " " + NoteNames[step];//frqNamesArray[idx-step_shift];
			}
		}
		Table.AddRow( oct, str );
	}
	return strs.str();
}



void Keyboard_class::Show_help( bool tty )
{
	if ( not tty ) return;
	get_notenames() ;
	Table_class Table { "Keyboard ", 11 };
	Table.AddColumn( "Keys"		, 9 );
	Table.AddColumn( "Function"	, 30 );
	Table.AddColumn( "State"	, 10 );
	Table.AddColumn( "Unit"		, 10 );
	Table.PrintHeader();

	Table.AddRow( "F1", "keyboard Keys"			);
	Table.AddRow( "F2", "increase sharps "		, (int) sds->Kbd_state.sharps, "#" );
	Table.AddRow( "F3", "reset sharps "			);
	Table.AddRow( "F4", "toggle decay mode "	, (int) sds->Kbd_state.ADSR_flag * sds->OSC_adsr.bps ,"B psec");
	Table.AddRow( "F5", "slide duration "		, to_string ((int)sds->Kbd_state.sliding *
													Osc->feature.glide_effect), "[ % ]");
	Table.AddRow( "F6", "Keyboard buffer "		, bool_str( StA->state.forget, "forget", "persist" ));
	Table.AddRow( "F7", "increase flats "		, (int) sds->Kbd_state.flats, "b" );
	Table.AddRow( "F8", "reset flats "			);
	Table.AddRow( "+/-" , "inc/dec octave"		, (int) sds->Kbd_state.base_octave, "base"  );
	Table.AddRow( "#" , "show Frequency Table" 	);
	Table.AddRow( "ESC", "Exit keyboard"		);
}

void Keyboard_class::exit_keyboard()
{
	Comment( WARN, "Exit by user request");
	sds->Keyboard = EXITSERVER;
}
void Keyboard_class::notekey( char key )
{
	kbd_note.Chord 	= kbd_note.SetChord( key );
	string
	noteline 		= kbd_note.setNote( key ) ;
	if ( noteline.length() > 0 )
	{
		Noteline.append( noteline );
		if( sds->StA_amp_arr[STA_KEYBOARD] == 0 )
		{
			sds->StA_amp_arr[STA_KEYBOARD] = sta_volume;
		}
	}
}
void Keyboard_class::set_bufferMode()
{
	StA->state.forget 	= not StA->state.forget ;
	if( StA->state.forget )
	{
		sta_volume = sds->StA_amp_arr[STA_KEYBOARD];
		sds->StA_amp_arr[STA_KEYBOARD] = 0;
	}
	else
	{
		sds->StA_amp_arr[STA_KEYBOARD] = sta_volume;
	}
	Noteline 			= "";
}
void Keyboard_class::specialKey()
{
	key3struct_t special = GetKeystruct( false );
	if ( special.val0 != ASC )
	{
		return;
	}
	bool tainted = true;
	switch( special.key )
	{
		case F5 : { set_slideMode(); 		break; }
		case F6	: { set_bufferMode();		break; }
		case F7 : { increase_flats();		break; }
		case F8 : { reset_flats(); 			break; }
		default : { tainted = false;		break; }
	}
	if ( tainted )
		Show_help( is_atty );
}
Frequency_class Frequency {};
void Keyboard_class::keyHandler( key3struct_t kbd )
{
	string 	noteline 	= "";
	bool 	tainted 	= true;
	switch (kbd.key )
	{
		case '+' :	{ change_octave(  1 ) 		; break; }
		case '-' :	{ change_octave( -1 ) 		; break; }
		case '#' :	{ Frequency.ShowFrqTable()	; break; }
		case ESC :
		{
			switch( kbd.val1 )
			{
				case 0  :	{ exit_keyboard()	; break; }
				case S0	: 	{ specialKey()		; break; }
				case F1	:	{ tainted = true	; break; }
				case F2 : 	{ increase_sharps()	; break; }
				case F3 : 	{ reset_sharps(); 	; break; }
				case F4 :	{ toggle_applyADSR(); break; }
				default :	{ tainted = false	; break; }
			};
			break;
		}
		default : { notekey( kbd.key );
					tainted	= false;
					break; }
	}
	if ( tainted )
	{
		ClearScreen();
		Show_help( is_atty );
	}
}
