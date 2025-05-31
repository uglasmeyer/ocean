/*
 * keyhandler.cpp
 *
 *  Created on: May 10, 2025
 *      Author: sirius
 */

#include <Keyboard.h>

void Keyboard_class::show_keys()
{
	cout << endl;
	Table_class Table { "Keyboard Keys", 20 };
	Table.AddColumn( "Key", 10 );
	Table.AddColumn( "Function", 20 );
	Table.PrintHeader();
	Table.AddRow( "F1", "keyboard Keys");
	Table.AddRow( "F2", "increase sharps # ");
	Table.AddRow( "F3", "reset sharps ");
	Table.AddRow( "F4", "toggle decay mode ");
	Table.AddRow( "F5", "slide duration ");
	Table.AddRow( "F6", "Update Instrument ");
	Table.AddRow( "F7", "Clear keyboard buffer ");

	Table.AddRow( "+" , "increase octave" );
	Table.AddRow( "-" , "decrease octave" );
	Table.AddRow( "#" , "show Frequency Table" );
	Table.AddRow("Note", kbdNotes.Str );
	Table.AddRow( "ESC", "Exit keyboard");
}

string Keyboard_class::get_notenames( )
{
	stringstream strs {};
	for ( int oct = kbd_octave-1; oct >= 0; oct-- )
	{
		for ( uint step = 0 ; step < oct_steps  ; step++ )
			if( kbdNotes.Str[step] != '_' )
			{
				uint idx = Index( base_octave + oct, step );
				strs << frqNamesArray[ idx ] << " ";
			}
		strs << endl;
	}
	return strs.str();
}
void Keyboard_class::increase_sharps()
{
	sharps = check_range( sharps_range, sharps + 1 );
	uint sharp = sharp_pitch[ sharps ]-1;
	kbdNotes.Str[sharp + 1 ] = kbdNotes.Str[ sharp ];
	kbdNotes.Str[sharp  ] = '_';
	cout << endl;
}
void Keyboard_class::set_frqMode()
{
	sliding = not sliding;
	int slidePercent = sliding * slidingstep;
	Oscgroup.SetSlide( slidePercent );

	Info( "slide frequency: ", slidePercent,  "[%]" );
}
void Keyboard_class::reset_sharps(  )
{
	kbdNotes 	= default_kbdNote.Str;
	sharps 		= 0;
}
void Keyboard_class::change_octave( int oct )
{
	base_octave = check_range( octave_range, oct );
	cout << endl;
	Info( "Base octave is: ", (int) base_octave );
}
void Keyboard_class::apply_Adsr( bool flag )
{
	ADSR_flag 				= flag;
	Osc->Set_adsr	( Osc->adsr );
}

void Keyboard_class::toggle_applyADSR( )
{
	bool flag = not ADSR_flag;
	cout << endl;
	string alpha = flag ? "Yes" : "No" ;
	Info( "Apply ADSR: ", alpha );
	apply_Adsr( flag );
}

void Keyboard_class::specialKey()
{
	key3struct_t special = GetKeystruct( false );
	if ( special.val0 != ASC )
	{
		return;
	}
	switch( special.key )
	{
		case F5 : { set_frqMode(); 		break; }
		case F6 : { Set_instrument(); 	break; }
		case F7	: { StA->Clear_data( 0.0 );	break; }
		default : { show_keys();break; }
	}

}
void Keyboard_class::keyHandler( key3struct_t kbd )
{
	switch (kbd.key )
	{
		case '+' :	{ change_octave( base_octave + 1 ) 	; break; }
		case '-' :	{ change_octave( base_octave - 1 ) 	; break; }
		case '#' :	{ ShowFrqTable()				; break;}
		case ESC :
		{
			switch( kbd.val1 )
			{
				case 0 :	{ Comment( WARN, "Exit by user request");
										instrument->sds->Synthesizer = EXITSERVER; break; }
				case F0	: { specialKey(); break; }
				case F1	: { show_keys(); break; }
				case F2 : { increase_sharps();Info( "Note layout:\n", get_notenames() );break; }
				case F3 : { reset_sharps(); 	Info( "Note layout:\n", get_notenames() );break; }
				case F4 : { toggle_applyADSR(); break; }
				default : break;
			}
			default : break;
		}
	}
}
