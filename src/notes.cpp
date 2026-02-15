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
 * playnotes.cpp
 *
 *  Created on: Jan 17, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <data/Config.h>
#include <notes/Notes.h>

Note_class::Note_class( Dataworld_class* 	data ,
						Instrument_class* 	instr,
						Storage_class* 		sta )
	: Logfacility_class	("Note_class")
	, Interface_base	( data )
	, NotesCore_class	( data )
	, Oscgroup			( NOTESROLE, 2*monobuffer_bytes )
	, Note_itr_start	( "Note_itr start", &sds_p->Note_start )
	, Note_itr_end		( "Note_itr end  ", &sds_p->Note_end )
{
	this->instrument	= instr;
	this->StA			= sta;
	this->scanner		= &sta->scanner;
	this->osc			= &Oscgroup.osc;
	this->vco			= &Oscgroup.vco;
	this->fmo			= &Oscgroup.fmo;
	this->Osc			= &Oscgroup.osc;
	this->read_cnt		= 0;

	Oscgroup.SetScanner	( max_frames );
	TestNotes			();
}

Note_class::~Note_class( )
{
	DESTRUCTOR( className )
}



Data_t* Note_class::ScanData( )
{
	return scanner->Next_read();
}

void Note_class::gen_chord_data( note_t& note )
{
	Oscgroup.Data_Reset();

	if ( note.glide[0].glide )
		Oscgroup.SetSlider_Frq( sds_p->features[OSCID].slide_frq );
	else
		Oscgroup.SetSlider_Frq( 0 );

	Osc->Set_long_note( true );//note.longnote );//or longnote );


	const uint	msec 			= ( note.longplay or sds_p->features[0].longplay ) ?
								1000 : note.duration;
	Oscgroup.Set_Duration		( msec );
	Osc->Set_spectrum_volume	( note.volume );
	Osc->Set_beatcursor			( 0 );

	Oscgroup.ChordData( sds_p, note.chord );
}


void Note_class::sta_write_data( const note_t& note )
{
	uint 	pitch_count = note.chord.size();

	if ( pitch_count > 0 )
	{
		float 	volume		= get_chord_volume( StA->Volume(), pitch_count );
		StA->Write_data		( Osc->Mem.Data, Osc->wp.frames, volume );
		scanner->Next_wpos	( note.duration * frames_per_msec );
	}
};



bool Note_class::Note_clock( bool init )
{
	if( init ) return false;
	read_cnt = ( read_cnt + 1 ) % measure_parts;
	return ( read_cnt == 0 ) ? false : true;
};

bool Note_class::Generate_volatile_data( bool init )
{
	// generate maxframes of new data if begin or rpos exceeds maxdata

	if ( Note_clock( init ) )
		return true;

//	coutf << scanner->rpos % min_frames << endl;
//	Assert_equal( (int)(scanner->rpos % min_frames), 0, "Notes scanner.rpos" );
	Oscgroup.SetInstrument( sds_p );
	if( note_itr_end() )
	{
		Note_itr_start.SetState( true );
		Note_itr_end.SetState( true );

		note_itr = notelist.begin();
	}
	if( init )
	{
		scanner->Reset();
		scanner->Lock_read( true );
		StA->param.wdsize = scanner->mem_range.max;

		Note_itr_start.SetState( true );
		note_itr = notelist.begin();
	}
	StA->state.Forget( true );
	sds_p->StA_state_arr[STA_NOTES].forget = true;

	int msec_elapsed = 0;
	while( ( msec_elapsed < measure_duration ) and ( not note_itr_end() ) )
	{
		gen_chord_data	( *note_itr );
		sta_write_data	( *note_itr );
		Show_note		( *note_itr, true );
		msec_elapsed 	+= note_itr->duration;
		note_itr_next	();
	}
	scanner->Lock_read( false );
	if( StA[ STA_NOTES ].scanner.fillrange.max > 0 )
		StA[ STA_NOTES ].param.wdsize = StA[ STA_NOTES ].scanner.fillrange.max;


	return not init;
}

bool Note_class::Generate_cyclic_data(  )
{
	StA->state.Forget( false );
	sds_p->StA_state_arr[STA_NOTES].forget = false;

	Oscgroup.SetInstrument( sds_p );
	Comment( INFO, "generating cyclic data" );

	if( note_itr ==  notelist.begin() )
	{
		Comment( ERROR, "empty note list" );
		return false;
	}
	note_itr = notelist.begin();

	scanner->Set_rpos(0);
	scanner->Set_wpos(0);
	scanner->Set_fillrange(0);
	StA->Clear_data			(0);

	while ( not note_itr_end() )
	{
		gen_chord_data( *note_itr );
		sta_write_data( *note_itr );
		note_itr_next();
	}
	scanner->Set_rpos( 0 );
	scanner->Set_fillrange(0);
	scanner->Set_fillrange( sds_p->Noteline_sec * frames_per_sec );
	if( scanner->fillrange.max > 0 )
		StA->param.wdsize = scanner->fillrange.max;
	scanner->Show( logmask[ DEBUG], &scanner->fillrange );
	return true;
}




void Note_class::LoadMusicxml( const string& file )
{

}

//-------------------------------------------------------------
bool Note_class::Start_note_itr()
{
	Info( "Start_note_itr");
	if ( note_itr == notelist.end() )
	{
		Comment( WARN, "Empty notelist" );
		if( StA )
			StA->Reset();
		Note_itr_end.SetState	( true );
		return false;
	}
	if( StA )
		StA->Reset();//scanner.Set_rpos( 0 );
	read_cnt = 0;

	note_itr 		= notelist.begin();
	Note_itr_start.SetState( true );
//	scanner->Set_fillrange( StA->param.size );
	scanner->Set_rpos( 0 );
	scanner->Set_wpos( 0 );
	return true;
}

bool Note_class::note_itr_end()
{
	bool state = ( note_itr == notelist.end() );
	return state;
}
void Note_class::note_itr_next()
{
	note_itr++;
}
//-------------------------------------------------------------



void Note_class::TestNotes()
{

	TEST_START( className );
	int oct = 4;
	pitch_t pitch = pitch_struct( oct, 'a', alter_value('a') );
	ShowFrqTable();
	ASSERTION( pitch.step == 9, "Assert test value ",(int)pitch.step, 9  )  ;
	ASSERTION( pitch.alter == 1, "Assert test value ",(int)pitch.alter, 1  )  ;
	ASSERTION( pitch.frqidx == 84, "Assert test value ",(int)pitch.frqidx, 84  )  ;


	pitch = pitch_struct( 84 );
	ASSERTION( pitch.step == 9, "Assert test value ",(int)pitch.step, 9  )  ;
	ASSERTION( pitch.alter == 1, "Assert test value ",(int)pitch.alter, 1  )  ;
	ASSERTION( pitch.frqidx == 84, "Assert test value ",(int)pitch.frqidx, 84  )  ;
	ASSERTION( pitch.octave == 4, "Assert test value ",(int)pitch.octave, 4  )  ;
	Assert_equal( pitch.step_char, 'A' );

	Comment( TEST, "Note_class test start");

	// Note Frequency
	sds_p->spectrum_arr[OSCID].frqidx[0] = A3;
	sds_p->spectrum_arr[VCOID].frqidx[0] = A4;
	sds_p->spectrum_arr[FMOID].frqidx[0] = A2;
	Oscgroup.Set_Note_Frequency( sds_p, A2, FIXED );
	Assert_equal( osc->spectrum.frqidx[0], A2 );
	Assert_equal( vco->spectrum.frqidx[0], A3 );
	Assert_equal( fmo->spectrum.frqidx[0], A1 );

	Oscgroup.Set_Note_Frequency(sds_p, A3, FIXED );
	Assert_equal( osc->spectrum.frqidx[0], A3 );
	Assert_equal( vco->spectrum.frqidx[0], A4 );
	Assert_equal( fmo->spectrum.frqidx[0], A2 );


	TEST_END( className );


}
