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
#include "../Setup/ReleaseNotes.h"

// musicsml, variation
Note_class::Note_class( )
	: Note_class::Logfacility_class("Note_class")
{
	this->className = Logfacility_class::className;
	init_note_table();
}

Note_class::Note_class( file_structure* fs )
	: Note_class::Note_class()
{
	this->fs = fs;
}

// File_dialog
Note_class::Note_class( interface_t* _sds, Config_class* cfg )
	: Note_class::Note_class()
//	: Note_class::Logfacility_class("Note_class")
//	, Note_class::Note_class Oscgroup ( NOTESROLE, 2*monobuffer_bytes )
{
	this->sds		= _sds;
	this->fs		= cfg->fs;
	this->Instrument_name.assign( sds->Instrument );
	init_note_table();
}

// Synthesizer
Note_class::Note_class( Instrument_class* 	instr,
						Storage_class* 		sta )
	: Note_class::Logfacility_class("Note_class")
	, Note_itr_start( "Note_itr start", &instr->sds->Note_start )
	, Note_itr_end	( "Note_itr end  ", &instr->sds->Note_end )
{
	this->sds 		= instr->sds;
	this->className = Logfacility_class::className;
	this->instrument= instr;
	this->fs		= instr->fs;
	this->wd		= instr->wd_p;
	this->StA		= sta;
	this->scanner	= &sta->scanner;
	this->Instrument_name.assign( sds->Instrument );

	Oscgroup.SetScanner( max_frames );
	init_note_table();
	TestNotes();
}

Note_class::~Note_class( )
{
	DESTRUCTOR( className )
}

void Note_class::init_note_table()
{
	Note_table.opt.Ident = 0;
	Note_table.AddColumn( "No.", 4 );
	Note_table.AddColumn( "Chord", 20 );
	Note_table.AddColumn( "Vol", 3 );
	Note_table.AddColumn( "msec",5 );
	Note_table.AddColumn( "|", 1);
	Note_table.AddColumn( "Oct", 4 );
	Note_table.AddColumn( "alt", 4 );
	Note_table.AddColumn( "Frq", 5 );
	Note_table.AddColumn( "Oct", 4 );
	Note_table.AddColumn( "alt", 4 );
	Note_table.AddColumn( "Frq", 5 );
	Note_table.AddColumn( "Oct", 4 );
	Note_table.AddColumn( "alt", 4 );
	Note_table.AddColumn( "Frq", 5 );
}
void Note_class::show_noteline_duration( const uint16_t& msec,
										uint& total_duration,
										uint& count,
										uint& m_duration )
{
	total_duration 	+= msec;
	m_duration 		+= msec;
	uint mod 		= total_duration % (measure_duration);
	if ( mod < msec )
	{
		Comment( INFO, " Measure number: ", (int)count, ", Measure duration:", m_duration );
		count++;
		m_duration 	= 0;
	};
}

string Note_class::Get_rhythm_line()
{
	return volumeline;
}

void Note_class::Set_rhythm_line( string rhythm )
{
	set_volume_vector( rhythm );
}
string Note_class::Get_note_line ()
{
	return noteline;
}
void Note_class::set_volume_vector( string volline )
{
	if ( volline.length() == 0 )
		this->volumeline = to_string( notes_default_volume / 10 );
	else
		this->volumeline = volline;

	volume_vec.clear();
	set<int> valid = range_set('1', '9'); // 1...9
	for ( char ch : this->volumeline )
	{
		if ( valid.contains( ch ))
			volume_vec.push_back( char2int( ch ) * 10);
		else
			Info( "Rhythm line value " , ch, "not valid" );
	}

	volume_vec_len = volume_vec.size();
	if ( volume_vec_len == 0 ) // no valid chars
	{
		this->volumeline = to_string( notes_default_volume / 10 );
		volume_vec.push_back( notes_default_volume );
		volume_vec_len = volume_vec.size();
	}
	assert( volume_vec.size() > 0 );
}
void Note_class::ScanData( )
{
	NotesData 	= StA->scanner.Next_read();
//	if( sds->WD_status.roleId == NOTESROLE )
//		wd->Set_wdcursor( scanner->rpos, scanner->mem_range.max );
}

void Note_class::gen_chord_data(const note_t&	note,
								const uint& 	duration,
								const bool& 	longnote
								)
{
	Oscgroup.Data_Reset();

	uint 			glide_effect 	= osc->features.glide_effect;
	if ( note.glide[0].glide )
		Oscgroup.SetSlide( 100 );
	else
		Oscgroup.SetSlide( 0 );

	Osc->Set_long_note( note.longnote or longnote );

	Osc->Set_beatcursor( 0 );

	uint 			msec 			= duration;
	if ( note.longplay or sds->features[0].longplay )
		msec = 1000;//max_msec;

	const int8_t	frqidx_shift	= oct_steps * sds->noteline_prefix.octave_shift;
	const uint 		frame_delay		= sds->noteline_prefix.chord_delay * frames_per_msec;
	uint 			n 				= 0;
	for ( pitch_t pitch : note.chord )
	{
		uint8_t frqidx = check_range( frqarr_range, GetFrqIndex( pitch ) + frqidx_shift, "gen_chord_data" );
		Oscgroup.Set_Osc_Note( sds, frqidx, msec, note.volume, SLIDE );
		Oscgroup.Phase_Reset();
		Oscgroup.Run_OSCs( n*frame_delay );
		n++;
	}

	Osc->features.glide_effect = glide_effect ;

	return ;
}


void Note_class::sta_write_data( uint duration )
{
	StA->Write_data( Osc->Mem.Data );//, max_frames );
	StA->scanner.Next_write( duration * frames_per_msec );
};

int read_cnt = 0;
bool Note_class::Generate_volatile_data( bool init )
{
	// generate maxframes of new data if begin or rpos exceeds maxdata
	read_cnt++;
//	coutf << "read_cnt/scanner.rpos " << read_cnt << "/" << StA->scanner.rpos << endl;
	if ( read_cnt < measure_parts )
	{
		if ( not init )
			return false;
	}
	read_cnt = 0;
//	coutf << StA->scanner.rpos % min_frames << endl;
//	Assert_equal( (int)(StA->scanner.rpos % min_frames), 0, "Notes scanner.rpos" );
	Oscgroup.SetInstrument( sds );

	int msec_elapsed = 0;

	while( ( msec_elapsed < measure_duration ) and ( not note_itr_end() ) )
	{
		gen_chord_data( *note_itr, note_itr->duration, false );
		sta_write_data( note_itr->duration );
		Show_note( *note_itr, true );
		msec_elapsed += note_itr->duration;
		note_itr_next();
	}
	if( note_itr_end() )
	{
		note_itr = notelist.begin();
	}
	StA->state.Forget( true );

	return true;
}

bool Note_class::Generate_cyclic_data(  )
{
	Oscgroup.SetInstrument( sds );
	Start_note_itr();
	StA->Reset();

	while ( not note_itr_end() )
	{
		gen_chord_data( *note_itr, note_itr->duration, false );
		sta_write_data( note_itr->duration );
		note_itr_next();
	}
	StA->state.Forget( false );
	StA->scanner.Set_rpos( 0 );
	StA->scanner.Show( LogMask[ DEBUG], &StA->scanner.fillrange );
	return true;
}

void Note_class::SetSDS( noteline_prefix_t nlp )
{
	if( sds )
		sds->noteline_prefix = nlp;
}

void Note_class::Set_noteline_prefix( noteline_prefix_t nlp )
{
	this->Noteline_prefix = nlp;
	Show_noteline_prefix( nlp );
	Verify_noteline( nlp, noteline );
}

bool Note_class::Set_notes_per_second( int notes_per_second )
{	// [ 1,2,4,5, 8 ] notes per second

	char nps_char = int2char( notes_per_second );//(char)(notes_per_second + 48 );
	if ( NpsChars.Set.contains( nps_char ) )
	{
		this->Noteline_prefix.nps 	= notes_per_second;
		return true;
	}
	else
	{
		Comment( ERROR, "set notes per second to " + to_string( notes_per_second ) + " rejected ");
		Comment( INFO, "not in list: " + NPS_string );
		this->Noteline_prefix.nps 	= nlp_default.nps ;
		return false;
	}
}

string Note_class::get_name()
{
	return notefile_name;
}

bool Note_class::set_file_name( string str )
{
	if ( str.length() == 0 )
		return false;

	notefile_name = str;
	notefile = "";
	for ( string dir : { fs->notesdir , fs->autodir} )
	{
		string filename = dir + str + fs->nte_type;
		if ( filesystem::exists( filename) )
			notefile = filename;
	};
	if ( notefile == "" )
	{
		notefile = fs->notesdir + str  + fs->nte_type;
		Comment( ERROR, "note file " + notefile + " not yet found");
		return false;
	}
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
		Note_itr_end.set_state	( true );
		return false;
	}
	if( StA )
		StA->Reset();//scanner.Set_rpos( 0 );
	read_cnt = 0;

	note_itr 		= notelist.begin();
	Note_itr_start.set_state( true );
//	StA->scanner.Set_fillrange( StA->param.size );
	StA->scanner.Set_rpos( 0 );
	StA->scanner.Set_wpos( 0 );
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


string Note_class::Read( string str )
{
	String 			Line{""};
	fstream 		File;
	vector_str_t 	Line_arr;

	if ( not set_file_name( str ) )
		return "";

	Comment(INFO, "setup notes for " + get_name() );

	File.open( notefile, fstream::in );

	volumeline 	= "";
	noteline 	= "";
	getline( File, Line.Str );

	do
	{
			Line.replace_comment();
			vector_str_t arr = Line.to_array( '=' );

			switch ( arr[0][0] )
			{
				case 'V' :
				{
					volumeline.append( arr[1] );
					break;
				}
				case 'N' :
				{
					noteline.append( arr[1] );
					break;
				}
				case 'P' :
					Noteline_prefix = String_to_noteline_prefix( arr[1] );
					SetSDS( Noteline_prefix );
					break;
				case '+' :
				{
					break;
				}
				default :
				{
					noteline.append( arr[0] );
					break;
				}
			} // end switch
	} while( getline( File, Line.Str));

	File.close();

	Noteline_prefix.octave_shift = sds->noteline_prefix.octave_shift;
	Noteline_prefix.variation = 0;
	Set_rhythm_line( volumeline );
	if ( Verify_noteline( Noteline_prefix, noteline ) )
	{
		return noteline;
	}
	else
		return "";

}

void Note_class::Save( string str, noteline_prefix_t prefix, string nl_str )
{
	set_file_name(  str );
	fstream File( notefile, fstream::out );
	Comment(DEBUG,Error_text( errno ));
	if ( ! File.is_open())
	{
		Comment( ERROR, "Input file: " + notefile + " does not exist" );
		return ;
	}
	Comment( INFO, "saving notes " + noteline + "\nto file: " + notefile );

	string noteline_prefix = Noteline_prefix_to_string( prefix );
	File <<
		"#See the documentation file " + rn_userdoc + ".pdf\n" <<
		"#about the specification of Prefix, Notes and Volume \n" <<
		"P=" << noteline_prefix << "\n" <<
		"N=" << nl_str << "\n" <<
		"V=" << volumeline  <<	endl;

	File.close();
}

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
	ASSERTION( pitch.step_char == 'A', "Assert test value ",  pitch.step_char, 'A'  )  ;

	Instrument_name = "NotesTest";
	Comment( TEST, "Note_class test start");

	// Note Frequency
	sds->spectrum_arr[OSCID].frqidx[0] = A3;
	sds->spectrum_arr[VCOID].frqidx[0] = A4;
	sds->spectrum_arr[FMOID].frqidx[0] = A2;
	Oscgroup.Set_Note_Frequency( sds, A2, FIXED );
	ASSERTION( osc->wp.freq == GetFrq(A2), "osc freq", osc->wp.freq, GetFrq(A2) );
	ASSERTION( vco->wp.freq == GetFrq(A3), "osc freq", vco->wp.freq, GetFrq(A3) );
	ASSERTION( fmo->wp.freq == GetFrq(A1), "osc freq", fmo->wp.freq, GetFrq(A1) );

	// Align
	string nline = "D2--";
	notelist_t nl { Gen_notelist( nlp_default, nline ) };
	Show_note_list( nl );
	Align_measure( nlp_default, nline );
	int msec = Calc_notelist_msec( nl );
	ASSERTION( msec == measure_duration, "Calc_noteline_msec", msec, measure_duration );


	nline = "D3----";
	nl =  Gen_notelist( nlp_default, nline ) ;
	msec = Calc_notelist_msec( nl );
	ASSERTION( msec == 2000, "Calc_noteline_msec", msec, 2000L );
	coutf << nline << endl;
//	ASSERTION( nline.length() == 8, "Append_noteline", nline.length(), 8L );

	Comment( TEST, "long notes ");// long note
	Set_rhythm_line("5");
	Verify_noteline( nlp_default, "A-D--B----d-");
	ASSERTION( noteline_sec == 4, "seconds", (int)noteline_sec, 4L );
	ASSERTION( min_duration == 250, "min_duration ", min_duration, 250L );
	Start_note_itr();
	ASSERTION( fcomp(note_itr->chord[0].freq, 220 ), "", note_itr->chord[0].freq , 220 );
	advance(note_itr, 3 );
	ASSERTION( note_itr->longnote, "Longnote", note_itr->longnote, true  );
	advance(note_itr, 1 );
	ASSERTION( not note_itr->longnote, "Longnote", note_itr->longnote, false );

	ASSERTION( note_itr->chord[0].step == 2, "Assert test value ",
			(int)note_itr->chord[0].step  ,2 );

	Comment( TEST, "fill note list");
	Set_rhythm_line("5");
	bool vn = Verify_noteline(Noteline_prefix, "ABCD-");
	assert( noteline_sec == 2 );
	assert( vn );

	Comment( TEST, "set volume");
	Set_rhythm_line("844448");
	Verify_noteline(Noteline_prefix, "|2(ACE)--|3G-EC-A--.B");
	int i = 0;
	volume_vec={80,40,40,40,40,0,80,0,0,0};
	for( note_t note : notelist )
	{
		ASSERTION( note.volume == volume_vec[i],"i="+to_string(i), note.volume, volume_vec[i]);
		i++;
	}

	Comment ( TEST, "long gliding");
	Set_rhythm_line("5");
	Verify_noteline(Noteline_prefix, "CA----->G...BCD");

	set_volume_vector( "yyyyy01289xxxxx" );
	assert( volume_vec_len == 4 );
	assert( volume_vec[0] == 10 );
	assert( volume_vec[3] == 90 );

	set_volume_vector("");
	assert( volume_vec_len == 1);
	assert( volume_vec[0] == notes_default_volume );

	volumeline = "9797";
	Set_prefix_octave( 0 );
	noteline = "|3F--G(AC)-(DC)-";
	Noteline_prefix = String_to_noteline_prefix( "4,0,4,0,0");
	Save(".testcase", Noteline_prefix, noteline );

	volumeline 	= "yyyyy";
	noteline 	= "xxxxx";
	noteline=Read(".testcase");
	ASSERTION( Noteline_prefix.Octave == 4, "", (int)Noteline_prefix.Octave , 3) ;
	assert( noteline_sec == 2 );
	assert( noteline.compare("|3F--G(AC)-(DC)-") == 0 );
	assert( volumeline.compare("9797") == 0 );

	Verify_noteline( Noteline_prefix,  "|3F--|2G(AC)-|3(DC)-");
	ASSERTION( Octave == 3, "Octave", (int) Octave, 3 );
	Start_note_itr();
	//	for ( note_struct_t note : notelist )
	//		cout << dec<<(int)note.octave << endl;
	advance(note_itr, 1);
	ASSERTION( note_itr->chord[0].octave == 2, "chord octave", note_itr->chord[0].octave, 2 );

	set_volume_vector("");
	assert( volume_vec[0] == notes_default_volume );

	note_itr = notelist.begin();
	advance(note_itr, 2);
	cout << note_itr->duration ;
	assert( note_itr->duration == 2*min_duration );


	noteline=".---";
	Verify_noteline( Noteline_prefix, noteline );
	Start_note_itr();
	assert( note_itr->volume == 0 );
	ASSERTION( 	note_itr->duration == measure_duration, "duration",
				note_itr->duration, measure_duration );

	Comment( TEST, "garbage test" );
	noteline="-xyz";
	Set_notes_per_second( 1 );
	Verify_noteline( Noteline_prefix, noteline );
	assert( min_duration == 1000 );

	noteline=",xyz";
	Set_notes_per_second( 5 );
	Verify_noteline( Noteline_prefix, noteline );
	assert( Noteline_prefix.nps == 5 );
	assert( min_duration == 200 );

	noteline="'xyz";
	Set_notes_per_second( 4 ); // default
	Verify_noteline( Noteline_prefix, noteline );
	assert( min_duration == 250 );

	Verify_noteline( Noteline_prefix, "|4(AB)--->|5A" );
	Start_note_itr();
	note_t note = *note_itr;
	ASSERTION( note.glide[0].chord.step == 9, "", note.glide[0].chord.step, 9 );
	ASSERTION( note.glide[0].chord.alter == 0,"", note.glide[0].chord.alter, 0 );
	Show_note( note );
	ASSERTION ( fcomp(	CalcFreq( oct_base_freq, note.glide[0].chord ), 880, 2E-4 ) , "CalcFreq",
						CalcFreq( oct_base_freq, note.glide[0].chord ), 880 );


	Verify_noteline( Noteline_prefix, "|2A'AA,A,AAA");
	Start_note_itr();
	for( int freq : { 220, 110, 55, 55, 110, 110, 110 } )
	{
		Show_note( *note_itr );
		cout << freq << " = " << note_itr->chord[0].freq  << endl;
		assert( abs( note_itr->chord[0].freq  - freq ) < 1);
		note_itr_next();
	}

	ASSERTION( Verify_noteline( Noteline_prefix, "|,A(A,a)(A'a)(Aa,)(a,)A'.."),
			"Verify Noteline |,A(A,a)(A'a)(Aa,)(a,)A'..", false, true );
	note_itr = notelist.begin();
	for( float freq : { 220, 110, 440, 220, 117, 440, } )
	{
		ASSERTION( abs( note_itr->chord[0].freq - freq) < 1, "", note_itr->chord[0].freq, freq );
		cout << note_itr->chord[0].freq << " :ok" << endl;
		note_itr_next();
	}

	pitch_struct A(A3);
	pitch_struct B = A;
	ASSERTION( A.frqidx == B.frqidx, "pitch_struct=", (int)B.frqidx, (int)A3);

	Gen_notelist( nlp_default, "A2A3-A4--A5---");

	pitch = pitch_struct( 10,'B',0 );
	Assert_equal( (int)pitch.frqidx, (int)((max_octave)*oct_steps +11 + C0) );

//	assert (false);
	Comment( TEST, "Note_class test done ");

	TEST_END( className );


}
