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


string Keyboard_class::show_kbd_notenames( )
{
	Table_class Table { "Notes", 11 };
	Table.AddColumn( "Octave", 8);
	Table.AddColumn( " Note names", 3*7 );
	Table.AddColumn( " Keyboard  ", 3*7 );
	Table.PrintHeader();
	stringstream 	strs 	{};
	for ( uint n = 0; n < kbd_octaves; n++ )
	{
		string 		str 	{};
		string		kbd		{};
		uint oct 			= kbd_octaves - n - 1;
		for ( uint step = 0 ; step < oct_steps  ; step++ )
		{
			if( kbd_note.keyboard_keys[oct][step] != '_' )
			{
				str 		= str + " " + NoteNames[step];
				kbd			= kbd + " " + kbd_note.keyboard_keys[oct][step];
			}
		}
		Table.AddRow( oct, str, kbd );
	}
	return strs.str();
}

void Keyboard_class::Show_help( bool tty )
{
	if ( not tty ) return;

	show_kbd_notenames() ;
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
	Table.AddRow( "+/-" , "inc/dec octave"		, (int) sds_p->Kbd_state.base_octave, "base"  );
	Table.AddRow( "#" , "show Frequency Table" 	);
	Table.AddRow( "ESC", "Exit keyboard"		);
}

void Keyboard_class::exit_keyboard()
{
	Comment( WARN, "Exit by user request");
	sds_p->Keyboard = EXITSERVER;
}
void Keyboard_class::notekey( char key )
{
	kbd_note.Chord 	= kbd_note.SetChord( key );
	kbd_note.SetNote( key ) ;

	if ( kbd_note.note_vec.size() > 0 )
	{
		Set_instrument();

		if( sds_p->StA_amp_arr[STA_KEYBOARD] == 0 )
		{
			sds_p->StA_amp_arr[STA_KEYBOARD] = sta_volume;
		}
	}
}
void Keyboard_class::set_bufferMode()
{
	sta_p->state.forget 	= not sta_p->state.forget ;
	if( sta_p->state.forget )
	{
		sta_volume = sds_p->StA_amp_arr[STA_KEYBOARD];
		sds_p->StA_amp_arr[STA_KEYBOARD] = 0;
	}
	else
	{
		sds_p->StA_amp_arr[STA_KEYBOARD] = sta_volume;
	}
	Noteline 			= "";
}

bool Keyboard_class::save_notes()
{
	auto delete_leading_nulls = [ this ](  )
	{
		set<char> nullset { '-', '.' };
		int pos = 0;
		char ch = Noteline[pos];
		while ( nullset.contains( ch ) )
		{
			Noteline[pos] = ' ';
			pos++;
		}
	};
	Comment( INFO, "Saving notes to file", "<tbd>" );

	delete_leading_nulls();
	notes_p->Align_measure( nlp, Noteline );
	if ( notes_p->Verify_noteline( nlp, Noteline ) )
	{
		Comment( INFO, Noteline );
		notes_p->Save( fs.kbdnotes_name, nlp, Noteline );
		return true;
	}
	else
		return false;

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
		case F9 : { tainted = save_notes();	break; }

		default : { tainted = false;		break; }
	}
	if ( tainted )
		Show_help( is_atty );
}
Frequency_class Frequency {};
void Keyboard_class::keyHandler( key3struct_t kbd )
{
	string 	noteline 	= "";
	bool 	tainted 	= false;
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
				case S1	: 	{ specialKey()		; break; }
				case F1	:	{ tainted = true	; break; }
				case F2 : 	{ increase_sharps()	; break; }
				case F3 : 	{ reset_sharps(); 	; break; }
				case F4 :	{ toggle_applyADSR(); break; }
				default :	{ tainted = true	; break; }
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
