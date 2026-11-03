/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Keyboard.h>

inline float get_sta_volume( Storage_class* sta_p, uint8_t chordlen )
{ //
	float 	volume			= sta_p->Volume();
	return 	get_chord_volume( volume, chordlen );
}

Keyboard_class::Keyboard_class( Dataworld_class*	data,
								Instrument_class* 	instr,
								Storage_class* 		sta,
								NotesCore_class* 	notes )
	: Logfacility_class		( "Keyboard_class" )
	, Interface_base		( data )
	, keyboardState_class	( sds_p )
//	, Oscgroup				( KBDROLE, 2*monobuffer_bytes )
	, wd_p					( instr->wd_p )
	, scanner				( &sta->scanner )
	, max_notes				( notes_per_sec * sta->param.storage_time )
	, Note_vec				( notes, instr->fs, max_notes )

{
	this->Osc				= &Oscgroup.osc;
	this->instrument 		= instr;
	this->Notes				= notes;
	this->StA				= sta;
	this->fs				= instr->fs;
	this->decayCounter 		= 0;
	this->duration_counter 	= 0;// count the beats of note kbd_key
	this->kbd_volume		= sds_p->StA_amp_arr[STA_KEYBOARD].amp;
	this->sta_volume		= kbd_volume * percent;
	this->frqMode			= SLIDE;
	this->enabled			= false;
	this->Chord_old.clear	();
	for_each( Oscgroup.member, [ this ]( Oscillator* osc )
		{ osc->Set_beatclock( &sds_p->Kbd_state.bpsval ); } );

	Oscgroup.SetDuration	( max_msec );
	Oscgroup.SetScanner		( max_frames );
	scanner->Set_fillrange	( StA->param.size );
	selfTest				();
}

void Keyboard_class::selfTest()
{
	ASSERTION( 	StA->mem_ds.data_blocks ==StA->param.storage_time*frames_per_sec, "sta_p->mem_ds.data_blocks",
				StA->mem_ds.data_blocks  , StA->param.storage_time*frames_per_sec);
	ASSERTION( 	StA->param.size == StA->param.storage_time*frames_per_sec, "sta_p->param.size",
				StA->param.size  , StA->param.storage_time*frames_per_sec);
	assert( scanner->inc == min_frames );
	assert( audio_frames == scanner->inc );
	Assert_equal( 'A', SetPitch( 'k' ) );
	sharps = 1;
	Assert_equal( 'f', (char)SetPitch( 'F' ) );

}

Keyboard_class::~Keyboard_class()
{
	DESTRUCTOR( className )
}

void Keyboard_class::gen_chord_data( chord_t chord )
{
	if ( chord.size() == 0 )//Kbd_key.nokey
		return;
	Comment( DEBUG, "gen_chord_data" );

	Oscgroup.DataReset			();
	if( sds_p->Kbd_state.bpsval == 0 ) // clear forward
	{
		scanner->Set_wpos( scanner->rpos );
		StA->Write_data( Osc->Mem.Data, Osc->wp.frames, 0.0 );
	}

	Oscgroup.ChordData( sds_p, chord );
	Chord_old = chord;
}

void Keyboard_class::Enable( bool is_kbd )
{
	enabled = is_kbd;
	if( is_kbd )
	{
		sds_p->StA_state_arr[ STA_KEYBOARD ].start 	= true;
		sds_p->StA_state_arr[ STA_KEYBOARD ].play 	= true;
		sds_p->StA_amp_arr[ STA_KEYBOARD ].amp		= kbd_volume;
		Info( "Keyboard is enabled");
	}
	else
	{
		sds_p->StA_state_arr[ STA_KEYBOARD ].play 	= false;
		sds_p->StA_amp_arr[ STA_KEYBOARD ].amp		= 0;
	}
}
void Keyboard_class::set_instrument( )
{
	Oscgroup.SetInstrument( sds_p );
	// kbd specific settings
	// override instrument settings
	if( sliding ) // F5 set slide mode
	{
		if ( Oscgroup.osc.features.frq_slide_value == 0 )
			Oscgroup.SetFrqSlidevalue( Sds->frq_slide.default_slide_value );
	}
	else
		Oscgroup.SetFrqSlidevalue( 0 );

	Oscgroup.osc.fourier.coeff_vec 	= instrument->Oscgroup.osc.fourier.coeff_vec;
	Oscgroup.vco.fourier.coeff_vec 	= instrument->Oscgroup.vco.fourier.coeff_vec;
}

bool Keyboard_class::decay(  )
{
//	coutf << "d";

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
	Comment( DEBUG, "attack" );
	if( StA->is_zero ) 	// act on an sta reset from outside
	{
		Note_vec.Init();
		scanner->Set_rpos(0);
	}

	Note_vec.Add			( chord, StA->state.Forget(), duration_counter );

	set_instrument			();
	Oscgroup.KbdAttack		(); // overwrite Instrument setting from sds
	Oscgroup.ResetBeatCursor();

	gen_chord_data			( chord );

	sta_volume 				= get_sta_volume( StA, chord.size() );
	scanner->Set_wpos		( scanner->rpos );
	StA->Write_data			( Osc->Mem.Data, Osc->wp.frames, sta_volume );

	decayCounter			= attackCounter ;
	chord.clear				();

}

void Keyboard_class::release(  )
{
	if( Chord_old.size() == 0 )
		return;
	Comment( DEBUG, "release" );

	if ( sds_p->Kbd_state.bpsval > 0 )
		return;

	// continuse mode
	decayCounter		= attackCounter ;
	gen_chord_data		( Chord_old );

	sta_volume 			= get_sta_volume( StA, Chord_old.size() );
	scanner->Set_wpos	( scanner->rpos );
	StA->Write_data		( Osc->Mem.Data, Osc->wp.frames, sta_volume  );

	StA->state.Forget	( true );
}
Data_t* Keyboard_class::ScanData()
{
	scanner->Set_fillrange( StA->param.size );
	Data_t*	Kbd_Data = scanner->Next_read();
	Note_vec.Next();
	return Kbd_Data;
}
void Keyboard_class::Dispatcher( bool sync )
{
	auto keypressed = [ this ](  )
	{
		return ( chord.size() > 0 );
	};
	auto continues_mode = [ this ](  )
	{
		return ( sds_p->Kbd_state.bpsval == 0 );
	};

	SetKbdKey();

	if ( ( keypressed() ) )
	{
		if ( continues_mode() )
			sync = true;

		if( sync ) // mixer-> kbdclock is triggered
		{
			attack();
		}
	}
	else
	{
		if( not decay() )
		{
			release();
		}
	}
}

bool Keyboard_class::Save_notes( bool save )
{
	if( save )
	{
		if( Note_vec.SaveNoteLine( StA->Volume() ) )
			Note_vec.SaveMusicxml();
		else
			Comment( ERROR, "Keyboard Notes not verified" );
	}
	return true;
}

void Keyboard_class::Keyboard_test()
{
	TEST_START( className );

	sds_p->Kbd_state.key = 'A';
	set_octave( 3 );
	SetKbdKey();
	Assert_equal( (int)pitch.frqidx, (int)C3 );
	Assert_equal( chord.size(), size_t(1) );
	chord.clear				();
	Assert_equal( chord.size(), size_t(0) );

	sds_p->Kbd_state.key = 'K';
	set_octave( 2 );
	SetKbdKey();
	Assert_equal( (int)pitch.frqidx, (int)A2 );

	attack();

	sds_p->Kbd_state.key = 'I';
	SetKbdKey();
	Assert_equal( (int)pitch.frqidx, (int)A3 );

	Sds->frq_slide.Set( Sds->frq_slide.default_slide_value );
	sds_p->Kbd_state.sliding	= false;
	toggle_slideMode();
	Assert_equal( sliding, true );

	attack();
	Assert_equal( int( Oscgroup.osc.features.frq_slide_value ), (int)Sds->frq_slide.default_slide_value );
	dynamic_state_t state = Oscgroup.osc.DynFrequency.GetCurrent();
	Assert_equal( state.mode, FIXED );
	Assert_equal( state.future, (int)A3 );

	TEST_END( className );
}

/**************************************************
 * KbdNoteline_struct
 *************************************************/
KbdNoteline_struct::KbdNoteline_struct()
{
	str 			= ""			;
}
void KbdNoteline_struct::AddPitch( const chord_t& pv )
{
	string s = Get_note_str( pv );
	str.append( s );
	coutf << s << " ";
}
string KbdNoteline_struct::Get_note_str( const chord_t& pv )
{
	string s = pv[0].name;
	if( pv.size() > 1 )
	{
		s = "(";
		for( pitch_t pitch : pv )
			s += pitch.name ;
		s += ")"  ;
	}
	return s;
};

void KbdNoteline_struct::AddLenth( uint& cnt )
{
	if( str.length() == 0 )
		return; // dont add note duration for empty notes
	for( uint i = 0; ((i < cnt ) and ( i < measure_parts )); i++ )
	{
		str.push_back( note_ns::INCDUR );
		cout << "-";
	}
	cout << endl;
	str.append( " " );
	cnt = 0;
};




/**************************************************
 * Notevector_struct
 *************************************************/
Notevector_struct::Notevector_struct( NotesCore_class* notes, file_structure* fs, size_t size )
	: 	KbdNoteline				()

{
	this->Notes				= notes;
	this->Fs				= fs;
	this->Size				= size;

	this->EmptyNote			= Notes->EmptyNote;
	this->EmptyNote.duration= 0;
	this->EmptyNote.str		= "";
	this->Note_pos			= 0;

	LoadNoteLine			();
	Init					();
}
Notevector_struct::~Notevector_struct()
{
}

bool Notevector_struct::SaveMusicxml()
{
	auto notevec2notelist = [ this ](  )
	{
		notelist_t 	notelist	{};
		note_itr_t 	note_itr	= notelist.begin();
		uint16_t	nr			= 0;
		for( note_t note 		: note_vec )
		{
			if( note.chord.size() > 0 ) // not a rest note
			{
				note.number		= nr;
				note.duration	= min_msec;
				notelist.push_back( note );
				note_itr++;
				nr++;
			}
			else
				note_itr->duration += min_msec;
		}
		Notes->Show_note_list( notelist );
		return notelist;
	};

	Musicxml_class MusicXML { Fs };

	notelist_t notelist = notevec2notelist();
	MusicXML.Notelist2xmlFile( Fs->kbdnotes_name , notelist );

	return true;
}

void Notevector_struct::Init()
{
	note_vec.clear();
	KbdNoteline.str="";
	for( size_t pos = 0; pos < Size; pos++ )
	{
		note_t note = EmptyNote;
		note_vec.push_back( move(note) );
	}
	Note_pos = 0;
}
void Notevector_struct::Add( chord_t vec, bool forget, uint& cnt )//TODO
{
	if( forget )
		return;
	KbdNoteline.AddLenth( cnt ); // prev pitch
	KbdNoteline.AddPitch( vec ); // current pitch
	Store( vec );

}
void Notevector_struct::Store( chord_t& pv )
{
	if( note_vec.size() == 0 )
		Init();
	note_vec[Note_pos].volume = notes_default_volume;
	for ( pitch_t pitch : pv )
	{
		note_vec[ Note_pos ].chord.push_back( pitch );
		note_vec[ Note_pos ].str.push_back( pitch.step_char );
	};
};
void Notevector_struct::Next()
{
	Note_pos = ( Note_pos + 1 ) % Size;
}
bool Notevector_struct::SaveNoteLine( float volume )
{
	nlp_data_t 	nlp				= Notes->nlp_default;
				nlp.nps			= 1000 / min_msec;
	Notes->Noteline_prefix.data = nlp;
	Notes->Noteline.set			( KbdNoteline.str );
	uint8_t		vol				= check_range( range_T<uint8_t>{1, 9}, uint8_t( volume * 10 ) );
	Notes->Rythmline.set		( to_string( vol ) );
	bool		verified		= Notes->Verify_noteline( note_ns::KEYBOARD );
	if ( verified )
	{
		Notes->Save	( Fs->kbdnotes_name, nlp, KbdNoteline.str );
		Notes->Sds->Write_str( NOTENAMESTR, Fs->kbdnotes_name );
	}
	return verified;
}

void Notevector_struct::LoadNoteLine()
{
	KbdNoteline.str		= Notes->Read( Fs->kbdnotes_name );
}





/**************************************************
 * Kbd_pitch_class
 *************************************************/
Kbd_pitch_class::Kbd_pitch_class( interface_t* _sds )
	: Logfacility_class( "Kbd_note_class" )
{
	this->sds			= _sds;
};


void Kbd_pitch_class::ShowPitch( pitch_t p )
{
	Table_class T {"Pitch"};
	T.AddColumn( "Data" , 20 );
	T.AddColumn( "Value", 10 );
	T.PrintHeader();
	T.AddRow( "Name", p.name );
	T.AddRow( "Frequency", p.freq );
	T.AddRow( "Frqidx", (int)p.frqidx );
	T.AddRow( "Octave", (int)p.octave );
	T.AddRow( "Alter", (int)p.alter );
	T.AddRow( "Step", (int)p.step );
	T.AddRow( "Char", p.step_char );
}

void Kbd_pitch_class::Show_kbd_layout( int8_t base_oct )
{
	Table_class Table 	{ "Notes", 11 };
	Table.AddColumn		( "Octave", 8);
	Table.AddColumn		( " Note names", 3*7 );
	Table.AddColumn		( " Keyboard  ", 3*7 );
	Table.PrintHeader	();

	for ( uint kbdrow = 0; kbdrow < kbd_rows; kbdrow++ )
	{
		string 	notestr 	{};
		string	kbd_str		{};
		uint 	oct 	= kbd_rows - kbdrow - 1;
		for ( uint step = 0 ; step < oct_steps  ; step++ )
		{
			char 	kbd_key = keyboard_keys[oct][step];
					kbd_str.push_back( kbd_key );
			char 	note_ch = SetPitch( kbd_key );
					notestr.push_back( note_ch );
		}
		Table.AddRow( oct + base_oct, notestr, kbd_str );
	}
}

char Kbd_pitch_class::SetPitch( char kbdkey )
{
	pitch 		= pitch_struct();
	uint index 	= 0;
	char KEY 	= toupper( kbdkey );
	if ( KEY == '_')
		return ' ';
	for( uint row = 0; row < kbd_rows; row++ )
	{
		int pos 	= keyboard_keys[ row ].find( KEY );
		if ( pos > -1 )//(int)STRINGNOTFOUND )
		{
			index = frqIndex( pos, row + base_octave );
			for( uint8_t n = 0; n < sharps; n++ )
			{
				if( pos == sharp_pitch[n] )
					index += 1;
			}
			for( uint8_t n = 0; n < flats; n++ )
			{
				if( pos == flat_pitch[n] )
					index -= 1;
			}
			pitch = pitch_struct( index );
		}
	}
	if ( index == 0 )
		return ' ';
	return frqNamesArray[index][0];
}

char Kbd_pitch_class::SetPitchVec( int key )
{
	if ( key == NOKEY )
		return NONOTE ;

	SetPitch( key );

	if ( pitch.step == NONOTE )
		return NONOTE;

	sds->Kbd_state.frq= pitch.freq;
	strcpy( sds->Kbd_state.note, pitch.name.data() );
	chord.push_back( pitch );

	if (( Chordrule_map.length() > 0 ) )
	{
		uint8_t idx 	= pitch.frqidx;
		for( char ch 	: Chordrule_map )
		{
			idx				= char2int(ch) + idx;

			pitch_t pitch 	=	pitch_struct( idx);
			chord.push_back( pitch );
			//			ShowPitch( pitch );
		}
	}
	return chord[0].step; // base pitch step
}

void Kbd_pitch_class::SetChord( char key )
{
	char KEY = toupper( key );
	if( chord_keys.contains( KEY ))
	{
		Chordrule_map = get<0>( Chords_map[KEY] );
		sds->Kbd_state.chord_type = KEY;
	}
}


void Kbd_pitch_class::Kbd_pitch_Test()
{
	TEST_START( className );
	int pos 	= keyboard_keys[ 0 ].find( 'S' );
	ASSERTION( pos == 2, "keyboard_keys", pos, 2 );
	Chordrule_map = get<0>( Chords_map['X'] );
	Assert_equal( strEqual( Chordrule_map, "43" ), true, Chordrule_map	);

	SetChord( 'X' );
	Assert_equal( strEqual( Chordrule_map, "43" ), true, Chordrule_map	);
	SetPitchVec( 'A' );
	Assert_equal( chord.size(), (size_t)3 );

	for( const pitch_t& pitch : chord)
	{
		ShowPitch( pitch );
	}
	chord.clear();
	SetChord( 'Y' );
	TEST_END( className );

}

