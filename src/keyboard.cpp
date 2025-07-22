/*
 * keyboard.cpp
 *
 *  Created on: Aug 14, 2024
 *      Author: sirius
 */

#include <Keyboard.h>

Keyboard_class::Keyboard_class( Instrument_class* instr, Storage_class* StA ) :
	Logfacility_class("Keyboard") ,
	Kbd_base(),
	keyboardState_class( instr->sds )
{
	className 					= Logfacility_class::className;
	this->instrument 			= instr;
	this->sds					= instrument->sds;
	this->StA					= StA;
	this->Kbd_Data				= StA->Data;

	Oscgroup.SetWd				( instr->wd_p, &Osc->wp.frames );
	Oscgroup.SetScanner			( max_frames );
	instr->wd_p->Add_data_ptr	( Osc->typeId, Osc->roleId, StA->Data, &StA->StAparam.size );

	ASSERTION( 	StA->mem_ds.data_blocks == 4*max_frames, "StA->mem_ds.data_blocks",
				StA->mem_ds.data_blocks  , 4*max_frames);
	ASSERTION( 	StA->StAparam.size == 4*max_frames, "StA->param.size",
				StA->StAparam.size  , 4*max_frames);
}


Keyboard_class::~Keyboard_class() = default;



void Keyboard_class::gen_chord_data( )
{

	if ( kbd_note.note_vec.size() == 0 )//Kbd_key.nokey )
		return;
	Oscgroup.Data_Reset();
	uint 	delay_frames 	= sds->noteline_prefix.chord_delay * frames_per_msec;
	uint8_t n 				= 0;
	Osc->Set_volume( kbd_volume, FIXED );
	for ( kbd_note_t note : kbd_note.note_vec )
	{
		Oscgroup.Set_Note_Frequency( instrument->osc->wp.freq, note.frqidx, frqMode );
		Oscgroup.Phase_Reset();
		Oscgroup.Run_OSCs( n * delay_frames );
		n++;
	}
	kbd_note.note_vec.clear();
}

void Keyboard_class::Enable( bool is_kbd )
{
	enabled = is_kbd;

	if( is_kbd )
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
	Oscgroup.SetSlide( sliding * sds->OSC_wp.glide_effect );
	Oscgroup.Set_Duration( max_msec );
	osc_frames	= Osc->wp.frames;
	apply_Adsr();
}
void Keyboard_class::apply_Adsr()
{
	Osc->adsr.bps	= ADSR_flag;// * sds->OSC_adsr.bps;
	Osc->Set_adsr	( Osc->adsr );
}
bool Keyboard_class::decay(  )
{
	bool
	decay 	= ( decayCounter > releaseCounter  );
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
			for( uint i = 0; i<cnt; i++ )
			{
				Noteline.append( "-" );
			}
			coutf << Noteline << endl ;
			cnt = 0;
		}
	};

	if ( ( Kbd_key.hold ) ) //and ( decayCounter >= releaseCounter) )
	{
		cout << "-" ;
		return;
	}
	if( ADSR_flag )
		Osc->Reset_beat_cursor();

	show_cnt( duration_counter );

	Set_instrument();

	gen_chord_data();


	StA->Write_data( Osc->Mem.Data );//, max_frames );

	decayCounter	= attackCounter ;

	return;
}

void Keyboard_class::release(  )
{
//	Oscgroup.Data_Reset();
//	cout << "r" ;

	if ( Kbd_key.hold )
	{
		StA->Write_data( Osc->Mem.Data );
		cout << "h" ;
	}
	return ;
}

void Keyboard_class::Set_Kbdnote( key3struct_t key )
{
	Kbd_key		=  key;
	string nl = kbd_note.setNote( Kbd_key.key );
	if ( ( kbd_note.note_vec.size() == 0 ))// and //( kbd_note.Note.step == NONOTE ) and )
	{
		keyHandler( Kbd_key );
		if ( not decay() )
			release();
	}
	else
	{
		if( instrument->osc->kbd_trigger )
		{
			attack();
			Noteline.append( nl );
		}
	}
}

void Keyboard_class::ScanData()
{
	StA->scanner.Set_max_len( StA->scanner.mem_range.max );
	assert( StA->scanner.inc == min_frames );
	assert( sds->audioframes == StA->scanner.inc );
	Kbd_Data = StA->scanner.Next();
}

