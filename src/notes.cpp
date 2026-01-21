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

// File_dialog
Note_class::Note_class( interface_t* _sds, file_structure* _fs )
	: Logfacility_class("Note_class")
	, Musicxml( _fs )
{
	this->sds		= _sds;
	this->fs		= _fs;
	this->Instrument_name.assign( sds->Instrument );
	init_note_table();
}

// Synthesizer
Note_class::Note_class( Instrument_class* 	instr,
						Storage_class* 		sta )
	: Logfacility_class("Note_class")
	, Musicxml		( instr->fs )
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
}

void Note_class::gen_chord_data( const note_t& note )
{
	Oscgroup.Data_Reset();

	uint 			glide_effect 	= osc->features.slide_frq;
	if ( note.glide[0].glide )
		Oscgroup.SetSlideFrq( 100 );
	else
		Oscgroup.SetSlideFrq( 0 );

	Osc->Set_long_note( note.longnote );//or longnote );

	Osc->Set_beatcursor( 0 );

	uint 			msec 			= note.duration;
	if ( note.longplay or sds->features[0].longplay )
		msec = 1000;//max_msec;

//	const int8_t	frqidx_shift	= oct_steps * sds->noteline_prefix.octave_shift;
	const uint 		frame_delay		= sds->noteline_prefix.chord_delay * frames_per_msec;
	uint 			n 				= 0;
	for ( pitch_t pitch : note.chord )
	{
		Oscgroup.Set_Osc_Note( sds, pitch.frqidx, msec, note.volume, SLIDE );
		Oscgroup.Run_OSCs( n*frame_delay );
		n++;
	}

	Osc->features.slide_frq = glide_effect ;

	return ;
}


void Note_class::sta_write_data( const note_t& note )
{
	uint 	pitch_count = note.chord.size();

	if ( pitch_count > 0 )
	{
		float 	volume 		= StA->Volume() / pitch_count;
		StA->Write_data( Osc->Mem.Data, Osc->wp.frames, volume );
		StA->scanner.Next_wpos( note.duration * frames_per_msec );
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

//	coutf << StA->scanner.rpos % min_frames << endl;
//	Assert_equal( (int)(StA->scanner.rpos % min_frames), 0, "Notes scanner.rpos" );
	Oscgroup.SetInstrument( sds );
	if( note_itr_end() )
	{
		Note_itr_start.SetState( true );
		Note_itr_end.SetState( true );

		note_itr = notelist.begin();
	}
	if( init )
	{
		StA->scanner.Reset();
		StA->scanner.Lock_read( true );
		StA->param.wdsize = StA->scanner.mem_range.max;

		Note_itr_start.SetState( true );
	note_itr = notelist.begin();
	}
	StA->state.Forget( true );
	sds->StA_state_arr[STA_NOTES].forget = true;

	int msec_elapsed = 0;
	while( ( msec_elapsed < measure_duration ) and ( not note_itr_end() ) )
	{
		gen_chord_data	( *note_itr );
		sta_write_data	( *note_itr );
		Show_note		( *note_itr, true );
		msec_elapsed 	+= note_itr->duration;
		note_itr_next	();
	}
	StA->scanner.Lock_read( false );
	if( StA[ STA_NOTES ].scanner.fillrange.max > 0 )
		StA[ STA_NOTES ].param.wdsize = StA[ STA_NOTES ].scanner.fillrange.max;


	return not init;
}

bool Note_class::Generate_cyclic_data(  )
{
	StA->state.Forget( false );
	sds->StA_state_arr[STA_NOTES].forget = false;

	Oscgroup.SetInstrument( sds );
	Comment( INFO, "generating cyclic data" );

	if( note_itr ==  notelist.begin() )
	{
		Comment( ERROR, "empty note list" );
		return false;
	}
	note_itr = notelist.begin();

	StA->scanner.Set_rpos(0);
	StA->scanner.Set_wpos(0);
	StA->scanner.Set_fillrange(0);
	StA->Clear_data			(0);

	while ( not note_itr_end() )
	{
		gen_chord_data( *note_itr );
		sta_write_data( *note_itr );
		note_itr_next();
	}
	StA->scanner.Set_rpos( 0 );
	StA->scanner.Set_fillrange(0);
	StA->scanner.Set_fillrange( sds->Noteline_sec * frames_per_sec );
	if( StA->scanner.fillrange.max > 0 )
		StA->param.wdsize = StA->scanner.fillrange.max;
	StA->scanner.Show( logmask[ DEBUG], &StA->scanner.fillrange );
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
		Note_itr_end.SetState	( true );
		return false;
	}
	if( StA )
		StA->Reset();//scanner.Set_rpos( 0 );
	read_cnt = 0;

	note_itr 		= notelist.begin();
	Note_itr_start.SetState( true );
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


string Note_class::Read( string filename )
{
	String 			Line{""};
	fstream 		File;
	vector_str_t 	Line_arr;

	if ( not set_file_name( filename ) )
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

	Noteline_prefix.octave_shift 	= sds->noteline_prefix.octave_shift;
	Noteline_prefix.variation 		= 0;
	Set_rhythm_line					( volumeline );

	if ( Verify_noteline( Noteline_prefix, noteline ) )
	{
		return noteline;
	}
	else
		return "";
}

void Note_class::Save( string name, noteline_prefix_t prefix, string nl_str )
{
			set_file_name(  name );
	fstream File( notefile, fstream::out );
	Comment	(DEBUG,Error_text( errno ));

	if ( not File.is_open())
	{
		Comment( ERROR, "Input file: " + notefile + " does not exist" );
		return ;
	}
	Comment( INFO, "saving notes:", nl_str );
	Comment( INFO, "to file: ", notefile );

	string noteline_prefix_str = Noteline_prefix_to_string( prefix );

	File <<
		"#See the documentation file " + rn_userdoc + ".pdf\n" <<
		"#about the specification of Prefix, Notes and Volume \n" <<
		"P=" << noteline_prefix_str << "\n" <<
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
	Assert_equal( osc->spectrum.frqidx[0], A2 );
	Assert_equal( vco->spectrum.frqidx[0], A3 );
	Assert_equal( fmo->spectrum.frqidx[0], A1 );

	Oscgroup.Set_Note_Frequency(sds, A3, FIXED );
	Assert_equal( osc->spectrum.frqidx[0], A3 );
	Assert_equal( vco->spectrum.frqidx[0], A4 );
	Assert_equal( fmo->spectrum.frqidx[0], A2 );


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
void Note_class::Test_Musicxml()
{
	TEST_START( className );

	note_t note{};
	typedef struct testvalue_struct
	{
		string 	testid;
		int 	duration;
		int 	noteduration; // multipe of 1/4/divisions
		string 	type;
		bool 	dot;
	} testvalue_t;
	vector<testvalue_t> testvec =
	{
		{"test1", 125, 				1, "16th"	, false },
		{"test1", 250, 				2, "eighth"	, false },
		{"test2", 500, 				4, "quarter", false },
		{"test3", 750, 				6, "quarter", true	},
		{"test4", 1000, 			8, "half"	, false	},
		{"test5", 1500, 			12, "half"	, true	},
		{"test6", measure_duration,	16, "whole"	, false	},
		{"test7", 1875, 			15, "16th"	, false	}

	};
	string teststr = "";
	for( testvalue_t test : testvec )
	{
		teststr = test.testid + " duration";
		note.duration = test.duration;
		xmlnote_value_t xmlnote_value = xmlnote_value_struct( note, musicxml.divisions );
		ASSERTION( xmlnote_value.duration == test.noteduration, teststr,
				(int)xmlnote_value.duration, (int)test.noteduration );
		teststr = test.testid + " type";
		ASSERTION( strEqual(xmlnote_value.type, test.type) , teststr,
							xmlnote_value.type, test.type);
		ASSERTION( xmlnote_value.dot == test.dot , teststr,
							(bool) xmlnote_value.dot, (bool)test.dot);
	}


	noteline_prefix_t nlp {};
	nlp.nps = 4;
	string Noteline = "A2B3-C4D3-.D3A4..A2-B3-C4-D3-A-B-";
	notelist_t nl = Gen_notelist( nlp, Noteline );
	Show_note_list( nl );
	Musicxml.Notelist2xmlFile( "test" , nl );


	musicxml = Musicxml.XmlFile2notelist(  "test"  );
	Show_note_list( musicxml.notelist ); // @suppress("Invalid arguments")
	cout << "instrument_name: " <<  musicxml.instrument_name<<endl;
	cout << "divisions      : " <<  musicxml.divisions <<endl;
	cout << "beats          : " <<  musicxml.beats<<endl;

	musicxml_t testxml = Note_base::musicxml_struct();
	int beats = musicxml.beats;
	ASSERTION(	beats == testxml.beats, "Music xml beats",beats, testxml.beats );
	int divisions = musicxml.divisions;
	ASSERTION(	divisions == testxml.divisions, "Music xml divisions",divisions, testxml.divisions );
	int duration = musicxml.scoreduration;
	ASSERTION(	duration == 6000, "Music xml scoreduration",	duration, 6000L );

	TEST_END( className );
}
