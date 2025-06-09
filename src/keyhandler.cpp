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
};


void keyboardState_class::change_octave( int inc )
{
	kbd_note.base_octave		= check_range( Kbdoctave_range, kbd_note.base_octave  + inc, "change_octave" );
	sds->Kbd_state.base_octave	= kbd_note.base_octave;
}
void keyboardState_class::set_accidental( uint loc, int dir  )
{
	for( uint oct = 0; oct < max_kbd_octave; oct++)
	{
		kbd_note.kbdNotes[oct][ (loc + dir)%oct_steps ] = kbd_note.kbdNotes[oct][ loc ];
		kbd_note.kbdNotes[oct][ loc     ]	= '_';
	}
};
void keyboardState_class::increase_sharps()
{
	sharps 						= check_range( sharps_range, sharps + 1, "increase_sharps" );
	sds->Kbd_state.sharps 		= sharps;

	if ( sharps > 0 )
	{
		uint location			= ( sharp_pitch[ sharps-1 ] ) ;
		set_accidental( location, 1 );
	}

}
void keyboardState_class::increase_flats()
{
	flats 						= check_range( flats_range, flats + 1, "increase_flats" );
	sds->Kbd_state.flats 		= flats;
	if ( flats > 0 )
	{
		uint location			= ( flat_pitch[ flats-1 ] ) ;
		set_accidental( location, -1 );
	}

}
void keyboardState_class::reset_sharps()
{
	kbd_note.kbdNotes			= kbd_note.dflt_keyboard;
	sharps 						= 0;
	sds->Kbd_state.sharps 		= 0;
}
void keyboardState_class::reset_flats()
{
	kbd_note.kbdNotes			= kbd_note.dflt_keyboard;
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
	for ( uint n = 0; n < max_kbd_octave; n++ )
	{
		string 		str 	{};
		uint oct 			= max_kbd_octave - n - 1;
		for ( uint step = 0 ; step < oct_steps  ; step++ )
		{
			if( kbd_note.kbdNotes[oct][step] != '_' )
			{
				str 		= str + " " + NoteNames[step];//frqNamesArray[idx-step_shift];
			}
		}
		Table.AddRow( oct, str );
	}
	return strs.str();
}



void Keyboard_class::Show_keys( bool tty)
{
	if ( not tty ) return;
	cout << endl;
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
													Osc->wp.glide_effect), "[ % ]");
	Table.AddRow( "F6", "Keyboard buffer "		, bool_str( StA->state.forget, "forget", "persist" ));
	Table.AddRow( "F7", "increase flats "		, (int) sds->Kbd_state.flats, "b" );
	Table.AddRow( "F8", "reset flats "			);
	Table.AddRow( "+/-" , "inc/dec octave"		, (int) sds->Kbd_state.base_octave, "base"  );
	Table.AddRow( "#" , "show Frequency Table" 	);
//	Table.AddRow( "Note", kbdNotes.Str		 	);
	Table.AddRow( "ESC", "Exit keyboard"		);
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
		case F6	: { StA->state.forget = not StA->state.forget ;	break; }
		case F7 : { increase_flats();;break; }
		case F8 : { reset_flats(); 	;break; }
		default : { tainted = false;break; }
	}
	if ( tainted )
		Show_keys( is_atty );
}
void Keyboard_class::keyHandler( key3struct_t kbd )
{
	bool tainted = true;
	switch (kbd.key )
	{
		case '+' :	{ change_octave(  1 ) 	; break; }
		case '-' :	{ change_octave( -1 ) 	; break; }
		case '#' :	{ ShowFrqTable()				; break;}
		case ESC :
		{
			switch( kbd.val1 )
			{
				case 0 :	{ Comment( WARN, "Exit by user request");
										instrument->sds->Synthesizer = EXITSERVER; break; }
				case F0	: { specialKey(); break; }
				case F1	: { tainted = false; Show_keys( is_atty ); break; }
				case F2 : { increase_sharps();;break; }
				case F3 : { reset_sharps(); 	;break; }
				case F4 : { toggle_applyADSR();break; }
				default : { tainted = false; break; }
			};
			break;
		}
		default : { tainted = false; break; }
	}
	if ( tainted )
		Show_keys( is_atty );
}
