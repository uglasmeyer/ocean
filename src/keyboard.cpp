/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>

Keyboard_class::Keyboard_class( Instrument_class* 	instr,
								Storage_class* 		StA,
								Note_class* notes )
	: Logfacility_class("Keyboard")
	, Kbd_base()
	, keyboardState_class( instr->sds )

{
	className 					= Logfacility_class::className;
	this->instrument_p 			= instr;
	this->sds_p					= instrument_p->sds;
	this->sta_p					= StA;
	this->Kbd_Data				= StA->Data;
	this->notes_p				= notes;
	this->nlp					= notes->noteline_prefix_default;
	this->nlp.nps				= 8;
	max_noteline_cnt					= nlp.nps * max_sec * 4;
	Oscgroup.SetWd				( instr->wd_p );
	Oscgroup.SetScanner			( max_frames );

	ASSERTION( 	StA->mem_ds.data_blocks == 4*max_frames, "StA->mem_ds.data_blocks",
				StA->mem_ds.data_blocks  , 4*max_frames);
	ASSERTION( 	StA->StAparam.size == 4*max_frames, "StA->param.size",
				StA->StAparam.size  , 4*max_frames);
}

Keyboard_class::~Keyboard_class()
{
	DESTRUCTOR( className )
}

void Keyboard_class::gen_chord_data( )
{

	if ( kbd_note.note_vec.size() == 0 )//Kbd_key.nokey )
		return;
	Oscgroup.Data_Reset();
	uint 	delay_frames 	= sds_p->noteline_prefix.chord_delay * frames_per_msec;
	uint8_t n 				= 0;
	Osc->Set_volume( kbd_volume, FIXED );
	for ( kbd_note_t note : kbd_note.note_vec )
	{
		Oscgroup.Set_Note_Frequency( sds_p, note.frqidx, frqMode );
		Oscgroup.Phase_Reset();
		Oscgroup.Run_OSCs( n * delay_frames );
		n++;
	}
	kbd_note.note_vec.clear();
}

void Keyboard_class::Enable( bool is_kbd )
{
	Enabled = is_kbd;

	if( is_kbd )
	{
		sds_p->StA_state[ STA_KEYBOARD ].play 	= true;
		sds_p->StA_amp_arr[ STA_KEYBOARD ] 		= 75;
		sds_p->mixer_status.kbd					= true;
		Info( "Keyboard is enabled");
	}
	else
	{
		sds_p->StA_state[ STA_KEYBOARD ].play 	= false;
		sds_p->StA_amp_arr[ STA_KEYBOARD ] 		= 0;
		sds_p->mixer_status.kbd					= false;
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
			if ( noteline_cnt > max_noteline_cnt )
			{
				cnt = noteline_cnt - max_noteline_cnt;
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

	if ( ( Kbd_key.hold ) )
	{
		cout << "-" ;
		return;
	}
	if( ADSR_flag )
		Osc->Reset_beat_cursor();

	show_cnt( duration_counter );
	string note_str = kbd_note.Get_note_str();
	if( sta_p->state.forget )
		coutf  << note_str;
	else
	{
		Noteline.append( note_str );
		coutf << Noteline << endl;
	}
	gen_chord_data();
	sta_p->scanner.Set_wpos( sta_p->scanner.rpos );
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

void Keyboard_class::Set_Kbdnote( key3struct_t key )
{
	Kbd_key		=  key;
	keyHandler( Kbd_key );

	if ( ( kbd_note.note_vec.size() == 0 ) )
	{
		if ( not decay() )
			release();
	}
	else
	{
		if( instrument_p->osc->kbd_trigger )
		{
			attack();
		}
	}
}
void Keyboard_class::set_kbd_trigger( bool trigger )
{
	if( trigger )
	{
		if( sta_p->state.play )
			sds_p->Kbd_state.trigger = true;
		else
			sds_p->Kbd_state.trigger = false;
	}
}

void Keyboard_class::ScanData()
{
//	StA->scanner.Set_max_len( StA->scanner.mem_range.max );
	assert( sta_p->scanner.inc == min_frames );
	assert( sds_p->audioframes == sta_p->scanner.inc );
	Kbd_Data = sta_p->scanner.Next_read();
//	set_kbd_trigger( sta_p->scanner.trigger );
}



/*
 * Kbd_nte_class
 */

string Kbd_note_class::Get_note_str(  )
{
	string str = "";
	if( note_vec.size() == 1 )
		str = note_vec[0].name ;
	else
	{
		str = "(";
		for( kbd_note_t note : note_vec )
			str += note.name ;
		str += ")"  ;
	}
	str += " ";
	return str;
};

void Kbd_note_class::SetNote( int key )
{
	auto base_note = [ this ]( int KEY  )
	{
		Note 	= kbd_note_struct( 0, NONOTE);
		for( uint row = 0; row < kbd_octaves; row++ )
		{
			size_t pos 	= keyboard_keys[ row ].find( KEY );
			if ( pos < STRINGNOTFOUND )
			{
				Note = kbd_note_struct( row + base_octave, pos);
			}
		}
		return Note;
	};

	if ( key == NOKEY )
		return ;

	int 		KEY 	= toupper( key );
	kbd_note_t 	Note 	= base_note( KEY );

	if ( Note.step == NONOTE )
		return ;

	note_vec.push_back( Note );

	if (( Chord.length() > 0 ) )
	{
		uint8_t idx 	= Note.frqidx;

		for( char ch 	: Chord )
		{
			idx			= char2int(ch) + idx;

			Note 		= kbd_note_struct( idx);
			note_vec.push_back( Note );
		}
	}
	return;
}

string Kbd_note_class::SetChord( char key )
{
	string str = Chord;
	if( chord_keys.contains( key ))
		str = Chords[key];
	return str;
}




