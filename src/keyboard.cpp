/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>
#include <notes/MusicXML.h>

Keyboard_class::Keyboard_class( Instrument_class* 	instr,
								Storage_class* 		StA,
								Note_class* 		notes )
	: Logfacility_class("Keyboard")
	, Kbd_base()
	, keyboardState_class( instr->sds )
	, wd_p( instr->wd_p )
	, scanner( &StA->scanner )

{
	className 					= Logfacility_class::className;
	this->instrument_p 			= instr;
	this->sds_p					= instrument_p->sds;
	this->sta_p					= StA;
	this->Kbd_Data				= StA->Data;
	this->Notes					= notes;
	this->fs					= instr->fs;
//	Oscgroup.SetWd				( instr->wd_p );

	Oscgroup.SetScanner			( max_frames );
	max_notes					= notes_per_sec * StA->param.storage_time;

	selfTest					();
}

void Keyboard_class::selfTest()
{
	ASSERTION( 	sta_p->mem_ds.data_blocks == sta_p->param.storage_time*frames_per_sec, "sta_p->mem_ds.data_blocks",
				sta_p->mem_ds.data_blocks  , sta_p->param.storage_time*frames_per_sec);
	ASSERTION( 	sta_p->param.size == sta_p->param.storage_time*frames_per_sec, "sta_p->param.size",
				sta_p->param.size  , sta_p->param.storage_time*frames_per_sec);
	assert( scanner->inc == min_frames );
	assert( sds_p->audioframes == scanner->inc );
}
Keyboard_class::~Keyboard_class()
{
	DESTRUCTOR( className )
}

void Keyboard_class::gen_chord_data( )
{

	if ( Pitch_vec.size() == 0 )//Kbd_key.nokey )
		return;
	Oscgroup.Data_Reset();
	uint 	delay_frames 	= sds_p->noteline_prefix.chord_delay * frames_per_msec;
	uint8_t n 				= 0;
	Osc->Set_volume( kbd_volume, FIXED );
	for ( pitch_t pitch : Pitch_vec )
	{
		Oscgroup.Set_Note_Frequency( sds_p, pitch.frqidx, frqMode );
		Oscgroup.Phase_Reset();
		Oscgroup.Run_OSCs( n * delay_frames );
		n++;
	}
	Pitch_vec.clear();
}

void Keyboard_class::Enable( bool is_kbd )
{
	Enabled = is_kbd;

	if( is_kbd )
	{
		sds_p->StA_state_arr[ STA_KEYBOARD ].play 	= true;
		sds_p->StA_amp_arr[ STA_KEYBOARD ] 		= 75;
		sds_p->mixer_state.kbd					= true;
		Info( "Keyboard is enabled");
	}
	else
	{
		sds_p->StA_state_arr[ STA_KEYBOARD ].play 	= false;
		sds_p->StA_amp_arr[ STA_KEYBOARD ] 		= 0;
		sds_p->mixer_state.kbd					= false;
	}
}
void Keyboard_class::Set_instrument( )
{
	Comment( DEBUG, "Update Instrument ");

	Oscgroup.SetInstrument( sds_p );

	// kbd specific settings
	Oscgroup.SetSlide( sliding * sds_p->features[OSCID].glide_effect );
	Oscgroup.Set_Duration( max_msec );
}

bool Keyboard_class::decay(  )
{
	bool 	decay	= ( decayCounter > releaseCounter  );
	if( decay )
	{
		duration_counter++;
		decayCounter--;
	}

	return decay;
}

void Keyboard_class::attack()
{
	auto show_cnt = [ this ]( uint& cnt )
	{
		if ( cnt > 0 )
		{
			noteline_cnt += ( cnt + 1 ); // + note
			if ( noteline_cnt > max_notes )
			{
				cnt = noteline_cnt - max_notes;
				for( uint i = 0; i<cnt; i++ )
				{
					Noteline.append( "-" );
				}
				Noteline.append(1,'\n' );
				noteline_cnt = 0;
			}
			else
				for( uint i = 0; i<cnt; i++ )
				{
					Noteline.append( "-" );
				}
			cnt = 0;
		}

	};

	auto store_note = [ this ](  )
	{
		if( Note_vec.size() == 0 )
			initNoteVector();
		Note_vec[Note_pos].volume = notes_default_volume;
		for ( pitch_t pitch : Pitch_vec )
		{
			Note_vec[ Note_pos ].chord.push_back( pitch );
			Note_vec[ Note_pos ].str.push_back( pitch.step_char );
		};
	};

	//-------------------------------------------------------------

	if ( ( Kbd_key.hold ) )
	{
		cout << "-" ;
		return;
	}
	if( ADSR_flag )
		Osc->Reset_beat_cursor();

	show_cnt( duration_counter );

	string note_str = Get_note_str();
	if( sta_p->state.Forget() )
		coutf  << note_str;
	else
	{
		store_note();
		Noteline.append( note_str );
		coutf << Noteline << endl;
	}
	gen_chord_data();

	scanner->Set_fillrange( sta_p->param.size );
	scanner->Set_wpos( scanner->rpos );
	sta_p->Write_data( Osc->Mem.Data );//, max_frames );

	decayCounter	= attackCounter ;

	return;
}

void Keyboard_class::release(  )
{
	if ( Kbd_key.hold )
	{
		sta_p->Write_data( Osc->Mem.Data );
		cout << "h" ;
	}
	return ;
}

void Keyboard_class::Dispatcher( kbdInt_t key )
{
	Kbd_key.Int		=  key;
	keyHandler( Kbd_key );

	if ( ( Pitch_vec.size() == 0 ) )
	{
		if ( not decay() )
			release();
	}
	else
	{
		if( kbd_trigger )
		{
			attack();
		}
	}
}

bool Keyboard_class::Save_notes()
{
	auto delete_leading_nulls = [ ]( string& str )
	{
		const set<char>	nullset { '-', PAUSE_CH };
		char 			ch 		= str[0];
		while ( nullset.contains( ch ) )
		{
			str 	= str.substr(1, str.length());
			ch 		= str[0];
		}
	};
	auto notevec2notelist = [ this ](  )
	{
		notelist_t notelist {};
		note_itr_t note_itr = notelist.begin();
		for( note_t note : Note_vec )
		{
			if( note.volume > 0 ) // not a rest note
			{
				notelist.push_back( note );
				note_itr++;
			}
			else
				note_itr->duration += note.duration;
		}
		Notes->Show_note_list( notelist );
		return notelist;
	};
	Comment( INFO, "Saving notes to file", "<tbd>" );

	delete_leading_nulls( Noteline );


	Musicxml_class MusicXML {};
	notelist_t notelist = notevec2notelist();
	MusicXML.Notelist2xmlFile( fs->kbdnotes_name , notelist );


	return true;

}

void Keyboard_class::initNoteVector()
{
	Note_vec.clear();
	for( uint8_t pos = 0; pos < max_notes; pos++ )
	{
		note_t note = note_struct( pos );
		note.volume = 0;  // indicates a rest note
		Note_vec.push_back( move(note) );
	}
	Note_pos = 0;
}

void Keyboard_class::ScanData()
{
	auto set_kbd_trigger = [ this ]( int value )
	{
		bool trigger = false;
		adsr_t adsr =instrument_p->osc->Get_adsr();
		if( adsr.bps == 0 )
			trigger = true;
		else
			trigger = not( value % ( frames_per_sec / adsr.bps ) );
		return trigger;
	};

	kbd_trigger = set_kbd_trigger( scanner->rpos );
	Kbd_Data = scanner->Next_read();
	if( sds_p->WD_status.roleId == KBDROLE )
		wd_p->Set_wdcursor( scanner->rpos, scanner->mem_range.max );

	Note_pos = ( Note_pos + 1 ) % max_notes;
}



/**************************************************
 * Kbd_pitch_class
 *************************************************/
void Kbd_pitch_class::Show_kbd_layout( int8_t base_oct )
{
	Table_class Table 	{ "Notes", 11 };
	Table.AddColumn		( "Octave", 8);
	Table.AddColumn		( " Note names", 3*7 );
	Table.AddColumn		( " Keyboard  ", 3*7 );
	Table.PrintHeader	();

	for ( uint n = 0; n < kbd_rows; n++ )
	{
		string 	str 	{};
		string	kbd		{};
		uint 	oct 	= kbd_rows - n - 1;
		for ( uint step = 0 ; step < oct_steps  ; step++ )
		{
			char kbdkey = keyboard_keys[oct][step];
			if(  kbdkey != '_' )
			{
				str 	= str + " " + OctChars_EN[step];
				kbd		= kbd + " " + kbdkey;
			}
		}
		Table.AddRow( oct + base_oct, str, kbd );
	}
}

Kbd_pitch_class::Kbd_pitch_class( interface_t* sds )
	: Logfacility_class( "Kbd_note_class" )
{
	this->className		= Logfacility_class::className;
	this->sds_p			= sds;
};

Kbd_pitch_class::Kbd_pitch_class()
	: Logfacility_class( "Kbd_note_class" )
{
	this->className		= Logfacility_class::className;
	this->sds_p			= nullptr;
}

string Kbd_pitch_class::Get_note_str(  )
{
	string str = "";
	if( Pitch_vec.size() == 1 )
		str = Pitch_vec[0].name ;
	else
	{
		str = "(";
		for( pitch_t note : Pitch_vec )
			str += note.name ;
		str += ")"  ;
	}
	str += " ";
	return str;
};

void Kbd_pitch_class::SetPitch( int key )
{
	auto base_pitch = [ this ]( char KEY  )
	{
		pitch 	= pitch_struct();
		for( uint row = 0; row < kbd_rows; row++ )
		{
			int pos 	= keyboard_keys[ row ].find( KEY );
			if ( pos > -1 )//(int)STRINGNOTFOUND )
			{
				char ch = OctChars_EN[pos];
				pitch = pitch_struct( row + base_octave, ch, alter_value(ch)  );
			}
		}
		return pitch;
	};

	if ( key == NOKEY )
		return ;

	int 		KEY 				= toupper( key );
	pitch_t 	pitch 				= base_pitch( KEY );

	if ( pitch.step == NONOTE )
		return ;

	sds_p->Kbd_state.frq= pitch.freq;
	strcpy( sds_p->Kbd_state.note, pitch.name.data() );
	Pitch_vec.push_back( pitch );

	if (( Chord.length() > 0 ) )
	{
		uint8_t idx 	= pitch.frqidx;
		for( char ch 	: Chord )
		{
			idx			= char2int(ch) + idx;

			pitch 		= pitch_struct( idx);
			Pitch_vec.push_back( pitch );
		}
	}
	return;
}

void Kbd_pitch_class::SetChord( char key )
{
	if( chord_keys.contains( key ))
	{
		Chord = get<0>( Chords_map[key] );
		sds_p->Kbd_state.chord_type = key;
	}
}

void Kbd_pitch_class::Kbd_pitch_Test()
{
	TEST_START( className );
	int pos 	= keyboard_keys[ 0 ].find( 'S' );
	ASSERTION( pos == 2, "keyboard_keys", pos, 2 );
	TEST_END( className );

}



