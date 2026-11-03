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
 * playnotes.cpp
 *
 *  Created on: Jan 17, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <data/Config.h>
#include <notes/Notes.h>

Note_class::Note_class( Dataworld_class* 	data ,
						Instrument_class* 	instr,
						Storage_class* 		sta,
						Musicxml_class*		xml_p )
	: Logfacility_class	("Note_class")
	, Interface_base	( data )
	, NotesCore_class	( data, xml_p )
//	, Oscgroup			( NOTESROLE, 2*monobuffer_bytes )
	, Note_itr_start	( "Note_itr start", &sds_master->Note_start )
	, Note_itr_end		( "Note_itr end  ", &sds_master->Note_end )
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
	Test_NotesCore		();
	testNotes			();
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
	auto change_oct = [] ( chord_t& chord, int8_t shift )
	{
		for ( pitch_t& pitch : chord )
		{
			pitch.frqidx += shift * oct_steps;
		}
	};

	Oscgroup.DataReset();

	if ( note.glide[0].glide )
	{
		if ( Oscgroup.osc.features.frq_slide_value == 0 )
			Oscgroup.SetFrqSlidevalue( Sds->frq_slide.default_slide_value );
	}
	else
		Oscgroup.SetFrqSlidevalue( 0 );

	Osc->Set_long_note( true );//note.longnote );//or longnote );
//	const uint	msec 			= ( note.longplay or sds_p->features[0].longplay ) ?
//								1000 : note.duration;
	Oscgroup.SetDuration		( max_msec );
	Osc->Set_volume				( note.volume );
	Oscgroup.SetBeatcursor		( 0 );

	chord_t chord = note.chord;
	if( Noteline_prefix.data.octave_shift != 0 )
		change_oct( chord, Noteline_prefix.data.octave_shift );
	Oscgroup.ChordData( sds_p, chord );
}


void Note_class::sta_write_data( const note_t& note )
{
	uint 	pitch_count = note.chord.size();

	if ( pitch_count > 0 )
	{
		float 	volume		= get_chord_volume( 1.0, pitch_count );
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


void Note_class::Readlock_wait( bool lock )
{
	auto waitloop = [ this ](  )
	{
		while( scanner->Read_lock() )
			std::this_thread::sleep_for( std::chrono::milliseconds(2) );
	};

	if( lock )
	{
		waitloop();
	}
	else
		scanner->Lock_read( false ); // reset from exit_proc/ do not block notes sync
}

bool Note_class::Generate_volatile_data( bool init )
{
	// generate maxframes of new data if begin or rpos exceeds maxdata

	if ( Note_clock( init ) )
		return true;

	Oscgroup.SetInstrument( sds_p );
	Oscgroup.osc.fourier.coeff_vec 	= instrument->Oscgroup.osc.fourier.coeff_vec;
	Oscgroup.vco.fourier.coeff_vec 	= instrument->Oscgroup.vco.fourier.coeff_vec;

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
		StA->Set_wdsize( scanner->mem_range.max );

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
	StA->Set_wdsize( scanner->fillrange.max );

	sds_p->NotesTypeId = XML_ID;

	return not init;
}

bool Note_class::Generate_cyclic_data(  )
{
	if( notelist.size() == 0 )
	{
		Comment( ERROR, "empty note list" );
		return false;
	}

	Comment( INFO, "generating cyclic data" );

	sds_p->NotesTypeId = NOTETYPE_e::BUSY_NTE; 	// disable new events other than
												// Semaphores from external processes

	StA->state.Forget( false );
	sds_p->StA_state_arr[STA_NOTES].forget = false;

	Oscgroup.SetInstrument( sds_p );
	Oscgroup.osc.fourier.coeff_vec 	= instrument->Oscgroup.osc.fourier.coeff_vec;
	Oscgroup.vco.fourier.coeff_vec 	= instrument->Oscgroup.vco.fourier.coeff_vec;

	note_itr = notelist.begin();

	scanner->Lock_read( true );	// disable Notes
	scanner->Set_wpos(0);
	StA->Clear_data			(0);

	while ( not note_itr_end() )
	{
		gen_chord_data( *note_itr );
		sta_write_data( *note_itr );
		note_itr_next();
	}
	scanner->Reset();
	scanner->Set_fillrange( sds_p->Noteline_sec * frames_per_sec );
	StA->Set_wdsize( scanner->fillrange.max );

	scanner->Lock_read( false );
	sds_p->NotesTypeId = NOTETYPE_e::NTE_ID;
	scanner->Show( logmask[ DEBUG], &scanner->fillrange );

	return true;
}




void Note_class::LoadMusicxml( const string& file )
{

}

//-------------------------------------------------------------

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

void Note_class::testNotes()
{

	TEST_START( className );
	int oct = 4;
	pitch_t pitch = pitch_struct( oct, 'a', alter_value('a') );
	ShowFrqTable();
	Assert_equal( pitch.step, int8_t(9) )  ;
	ASSERTION( pitch.alter == 1, "Assert test value ",(int)pitch.alter, 1  )  ;
	ASSERTION( pitch.frqidx == 84, "Assert test value ",(int)pitch.frqidx, 84  )  ;

	pitch = pitch_struct( 84 );
	ASSERTION( pitch.step == 9, "Assert test value ",(int)pitch.step, 9  )  ;
	ASSERTION( pitch.alter == 1, "Assert test value ",(int)pitch.alter, 1  )  ;
	ASSERTION( pitch.frqidx == 84, "Assert test value ",(int)pitch.frqidx, 84  )  ;
	ASSERTION( pitch.octave == 4, "Assert test value ",(int)pitch.octave, 4  )  ;
	Assert_equal( pitch.step_char, 'A' );

	Comment( TEST, "Note_class test start");


	const spectrum_t			TestFourierSpectrum =
	{
		.volidx  	{ 100, 0, 0, 0, 0 },
		.frqidx 	{ A3, 0, 0, 0, 0 },
		.wfid 		{ FOURIER,SINUS,SINUS,SINUS,SINUS, },
		.phi		= zero_phase,
		.osc 		= OSCID,
		.type		= spectrum_ns::FREQ
	};

	Osc->Set_spectrum		( TestFourierSpectrum, FIXED );
	instrument->Set			( "default" );
	osc->fourier.coeff_vec 	= instrument->Oscgroup.osc.fourier.coeff_vec;
	vco->fourier.coeff_vec 	= instrument->Oscgroup.vco.fourier.coeff_vec;

	Osc->OSC				( 0 );

	TEST_END( className );


}
