/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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
#include <notes/MusicXML.h>

inline float set_sta_volume( Storage_class* sta_p, uint8_t chordlen )
{
	float volume	= sta_p->Volume();
	if ( chordlen > 1 )
	{
		volume 	=	volume * chordlen / ( chordlen + 1 );
	}
	assert(volume< 1.0);
	return volume;
}

Keyboard_class::Keyboard_class( Instrument_class* 	instr,
								Storage_class* 		sta,
								Note_class* 		notes )
	: Logfacility_class		("Keyboard")
	, Kbd_base				()
	, keyboardState_class	( instr->sds )
	, Oscgroup				( KBDROLE, 2*monobuffer_bytes )
	, wd_p					( instr->wd_p )
	, scanner				( &sta->scanner )
	, max_notes				( notes_per_sec * sta->param.storage_time )
	, Note_vec				( notes, instr->fs, max_notes )

{
	this->Osc				= &Oscgroup.osc;

	this->instrument_p 		= instr;
	this->Notes				= notes;
	this->sds_p				= instrument_p->sds;
	this->StA				= sta;
	this->Kbd_Data			= sta->Data;
	this->fs				= instr->fs;
	this->decayCounter 		= 0;
	this->duration_counter 	= 0;// count the beats of note kbd_key
	this->kbd_volume		= sds_p->StA_amp_arr[STA_KEYBOARD];
	this->sta_volume		= kbd_volume * percent;
	this->frqMode			= SLIDE;
	this->Enabled			= false;
	this->Pitch_old.clear	();

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
	assert( sds_p->audioframes == scanner->inc );
}

Keyboard_class::~Keyboard_class()
{
	DESTRUCTOR( className )
}

void Keyboard_class::gen_chord_data( pitch_vec_t pitch_vec )
{
	if ( pitch_vec.size() == 0 )//Kbd_key.nokey
		return;

	const uint 	delay_frames 	= 4 * sds_p->noteline_prefix.chord_delay * frames_per_msec;
	uint8_t 	n 				= 0;

	Oscgroup.Data_Reset			();
	if( bpsidx == 0 )
	{
		scanner->Set_wpos( scanner->rpos );
		StA->Write_data( Osc->Mem.Data, Osc->wp.frames, sta_volume );
	}
	for ( pitch_t pitch 		: pitch_vec )
	{
		Oscgroup.Set_Note_Frequency	( sds_p, pitch.frqidx, SLIDE );
		Oscgroup.Run_OSCs			( n * delay_frames );
		if( n < Pitch_old.size() )
			Oscgroup.Set_Note_Frequency	( sds_p, Pitch_old[n].frqidx, SLIDE );

		n++;
	}
	Pitch_old = pitch_vec;
}

void Keyboard_class::Enable( bool is_kbd )
{
	Enabled = is_kbd;

	if( is_kbd )
	{
		sds_p->StA_state_arr[ STA_KEYBOARD ].play 	= true;
		sds_p->StA_amp_arr[ STA_KEYBOARD ] 			= kbd_volume;
		Info( "Keyboard is enabled");
	}
	else
	{
		sds_p->StA_state_arr[ STA_KEYBOARD ].play 	= false;
		sds_p->StA_amp_arr[ STA_KEYBOARD ] 			= 0;
	}
}
void Keyboard_class::Set_instrument( )
{
	Comment( DEBUG, "Update Instrument ");
	Oscgroup.SetInstrument( sds_p );
	// kbd specific settings
	Oscgroup.Set_Duration( max_msec );
	Oscgroup.SetSlideFrq( sliding * sds_p->features[OSCID].slide_frq );
	Oscgroup.Set_kbdbps( sds_p->Kbd_state.bpsidx );
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
	//	coutf << "a";
	if( not StA->touched ) // act on an sta reset from outside
		Note_vec.Init();
	Note_vec.Add( Pitch_vec, StA->state.Forget(), duration_counter );

	Oscgroup.Set_Duration( max_msec );
	Oscgroup.Reset_beat_cursor();

	sta_volume 		= set_sta_volume( StA, Pitch_vec.size() );
	gen_chord_data( Pitch_vec );
	Pitch_vec.clear();

	scanner->Set_wpos( scanner->rpos );
	StA->Write_data( Osc->Mem.Data, Osc->wp.frames, sta_volume );

	decayCounter	= attackCounter ;

}

void Keyboard_class::release(  )
{
//	coutf << "r";
	if( sds_p->Kbd_state.bpsidx == 0 )
	{
		char mode = 1;
		if( mode == 0 )
		{
			Oscgroup.Set_Duration( min_msec );
		}
		else
		{
			decayCounter	= attackCounter ;
			Oscgroup.Set_Duration( max_msec );
		}
		gen_chord_data( Pitch_old );

		sta_volume 		= set_sta_volume( StA, Pitch_old.size() );
		scanner->Set_wpos( scanner->rpos );
		StA->Write_data( Osc->Mem.Data, Osc->wp.frames, sta_volume  );


		StA->state.Forget( true );
	}
}
void Keyboard_class::ScanData()
{
	scanner->Set_fillrange( StA->param.size );
	Kbd_Data = scanner->Next_read();
	Note_vec.Next();
}
void Keyboard_class::Dispatcher( kbdInt_t key, bool sync )
{
	auto keypressed = [ key, this ](  )
	{
		return ( Pitch_vec.size() > 0 );
	};

	Kbd_key.Int		=  key;
	keyHandler( Kbd_key );

	if ( ( keypressed() ) )
	{
		if( sync )
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
		else
		{
			;
		}
	}
}

bool Keyboard_class::Save_notes( bool save )
{
	if( save )
	{
		Note_vec.SaveNoteLine();
		Note_vec.SaveMusicxml();
	}
	return true;
}



/**************************************************
 * Noteline_struct
 *************************************************/
Noteline_struct::Noteline_struct()
{
	str 			= ""			;
}
void Noteline_struct::AddPitch( const pitch_vec_t& pv )
{
	string s = Get_note_str( pv );
	str.append( s );
	coutf << s ;
}
string Noteline_struct::Get_note_str( const pitch_vec_t& pv )
{
	string s = pv[0].name;
	if( pv.size() > 1 )
	{
		s = "(";
		for( pitch_t note : pv )
			s += note.name ;
		s += ")"  ;
	}
	return s;
};

void Noteline_struct::AddLenth( uint& cnt )
{
	if( str.length() == 0 )
		return; // dont add note duration for empty notes
	for( uint i = 0; ((i < cnt ) and ( i < measure_parts )); i++ )
	{
		str.push_back( INCDUR );
		cout << "-";
	}
	cout << endl;
	str.append( " " );
	cnt = 0;
};




/**************************************************
 * Notevector_struct
 *************************************************/
Notevector_struct::Notevector_struct( Note_class* notes, file_structure* fs, size_t size )
	: 	Noteline				()

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
//	SaveNoteLine();
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
	Noteline.str="";
	for( size_t pos = 0; pos < Size; pos++ )
	{
		note_t note = EmptyNote;
		note_vec.push_back( move(note) );
	}
	Note_pos = 0;
}
void Notevector_struct::Add( pitch_vec_t vec, bool forget, uint& cnt )//TODO
{
	if( forget )
		return;
	Noteline.AddLenth( cnt ); // prev pitch
	Noteline.AddPitch( vec ); // current pitch
	Store( vec );

}
void Notevector_struct::Store( pitch_vec_t& pv )
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
void Notevector_struct::SaveNoteLine()
{
	noteline_prefix_t	nlp			= Notes->nlp_default;
						nlp.nps		= 1000 / min_msec;
						Notes->Save	( Fs->kbdnotes_name, nlp, Noteline.str );
}
void Notevector_struct::LoadNoteLine()
{
	Noteline.str		= Notes->Read( Fs->kbdnotes_name );
}





/**************************************************
 * Kbd_pitch_class
 *************************************************/
Kbd_pitch_class::Kbd_pitch_class( interface_t* sds )
	: Logfacility_class( "Kbd_note_class" )
{
	this->sds_p			= sds;
};
Kbd_pitch_class::Kbd_pitch_class()
	: Logfacility_class( "Kbd_note_class" )
{
	this->sds_p			= nullptr;
}

void ShowPitch( pitch_t p )
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



char Kbd_pitch_class::SetPitch( int key )
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
		return NONOTE ;

	int 		KEY 				= toupper( key );
	pitch_t 	pitch 				= base_pitch( KEY );

	if ( pitch.step == NONOTE )
		return NONOTE;

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
//			ShowPitch( pitch );
			Pitch_vec.push_back( pitch );
		}
	}
	return Pitch_vec[0].step; // base pitch step
}

void Kbd_pitch_class::SetChord( char key )
{
	char KEY = toupper( key );
	if( chord_keys.contains( KEY ))
	{
		Chord = get<0>( Chords_map[KEY] );
		sds_p->Kbd_state.chord_type = KEY;
	}
}


void Kbd_pitch_class::Kbd_pitch_Test()
{
	TEST_START( className );
	int pos 	= keyboard_keys[ 0 ].find( 'S' );
	ASSERTION( pos == 2, "keyboard_keys", pos, 2 );
	Chord = get<0>( Chords_map['X'] );
	Assert_equal( strEqual( Chord, "43" ), true, Chord	);

	SetChord( 'X' );
	Assert_equal( strEqual( Chord, "43" ), true, Chord	);
	SetPitch( 'A' );
	Assert_equal( Pitch_vec.size(), (size_t)3 );
	TEST_END( className );

	for( const pitch_t& pitch : Pitch_vec)
	{
		ShowPitch( pitch );
	}

}

