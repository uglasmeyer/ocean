/*
 * playnotes.cpp
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
 */

#include <Config.h>
#include <notes/Notes.h>

Note_class::Note_class()
: Note_class::Logfacility_class("Notes"),
  Note_base()
{
	this->className = Logfacility_class::className;
}

Note_class::Note_class( Wavedisplay_class* wd )
: Note_class::Logfacility_class("Notes"),
  Note_base()
{
	this->className = Logfacility_class::className;
	Oscgroup.SetWd(wd);
}

Note_class::~Note_class( )
{
}

void Note_class::Set_instrument(Instrument_class *instrument) {
	Instrument_name = instrument->Name;
	Comment(TEST, "Update notes instrument:  " + Instrument_name);
	// copy class Oscillator

	Oscgroup.osc 	= instrument->Oscgroup.osc;
	Oscgroup.vco	= instrument->Oscgroup.vco;
	Oscgroup.fmo 	= instrument->Oscgroup.fmo;

	osc->Connect_frq_data(fmo);
	osc->Connect_vol_data(vco);


//	Oscgroup.Set_Duration( max_milli_sec );
//	Oscgroup.osc.Set_adsr( Oscgroup.osc.adsr );

	return;
}

string Note_class::Get_rhythm_line()
{
	return Volumeline;
}

void Note_class::Set_rhythm_line( string rhythm )
{
	set_volume_vector( rhythm );
}
string Note_class::Get_note_line ()
{
	return Noteline;
}

void Note_class::set_volume_vector( string volline )
{
	if ( volline.length() == 0 )
		this->Volumeline = to_string( notes_default_volume / 10 );
	else
		this->Volumeline = volline;

	volume_vec.clear();
	set<int> valid = range_set('1', '9'); // 1...9
	for ( char ch : this->Volumeline )
	{
		if ( valid.contains( ch ))
			volume_vec.push_back( char2int( ch ) * 10);
		else
			Info( "Rhythm line value " , ch, "not valid" );
	}

	volume_vec_len = volume_vec.size();
	if ( volume_vec_len == 0 ) // no valid chars
	{
		this->Volumeline = to_string( notes_default_volume / 10 );
		volume_vec.push_back( notes_default_volume );
		volume_vec_len = volume_vec.size();
	}
	assert( volume_vec.size() > 0 );
}

void Note_class::note2memory( 	const note_t& note,
								const buffer_t& offs,
								const uint& duration,
								const bool& longnote
								)
{

	uint wp_glide_effect = Oscgroup.osc.wp.glide_effect;

	if ( note.glide[0].glide )
		Oscgroup.SetSlide( 100 );
	else
		Oscgroup.SetSlide( 0 );

	Oscgroup.osc.Set_long_note( note.longnote or longnote );
	osc->adsr.hall = 0;
	Oscgroup.osc.Gen_adsrdata( ( duration * frames_per_msec ) );

	for ( pitch_t pitch : note.chord )
	{

		uint8_t key = GetFrqIndex( pitch );
		Oscgroup.Set_Osc_Note( key, duration, note.volume, SLIDE );
		Oscgroup.Run_OSCs(offs);

	}

	Oscgroup.osc.Set_glide( wp_glide_effect );

	return ;
}

int timestamp = 0;
uint scoretime = 0;
bool Note_class::Generate_note_chunk( )
{ 	// generate the memory track for positon n = note_pointer tp
	// n = note_pointer + chunk_len

	auto restart_note_itr = [ this ]()
	{
		if (( note_itr == notelist.end() ) or ( Restart )) // the global note iter shall be restarted.
		{
			Start_note_itr();
			timestamp = 0 ;
			scoretime = 0;
		}
		Restart = false;
	};

	Oscgroup.Data_Reset();
//	for( Oscillator* osc : Oscgroup.member )
//		osc->Data_reset()Mem.Clear_data( 0 );

	restart_note_itr();

	buffer_t 	frame_offset = (frames_per_msec * timestamp)  ;
	bool 		partnote = false;//( timestamp != 0);
	int 		lastduration = 0;
	bool		longnote = false;// ( timestamp != 0 );
	while ( note_itr != notelist.end() )
	{

		if ( timestamp == max_msec )
		{
			timestamp = 0;
			return true; // good
		}
		if ( timestamp >  max_sec*1000 )
		{	// considers the end pause to finish
			timestamp = timestamp % max_msec;

			Comment( DBG2, "take over " + to_string( timestamp ) + "[msec]");
			note_itr++;
			return false;
		}

		uint duration = rint( note_itr->duration );//* 2.0 ) ;// musicxml.tempo;
		scoretime += duration;
		if ( timestamp + duration > max_msec )
		{
			lastduration = duration;
			duration = max_msec - timestamp ;
			partnote = true;
//			cout << "part start ";
		}
		note2memory( *note_itr, frame_offset, duration, longnote );

		longnote = false;
		timestamp = timestamp + duration;
		frame_offset = (frames_per_sec * timestamp)  / 1000 ;

		if ( partnote )
		{ 	//unaligned end of the measure

			timestamp = lastduration - duration;
//			cout << "T: " << timestamp << endl;
			if ( scoretime >= noteline_sec * 1000 )
				note_itr++;
			return false;
		}
		else
			note_itr++;
	}
	note_itr = notelist.begin(); // track done , time is over

	if ( timestamp == 0 )
	{
		return true;
	}
	else
	{ 	// there was an uncomplete measure at the end of the notelist
		Comment(DBG2, "Notes are not aligned by " + to_string( timestamp ) + "[msec]");
		timestamp = 0;
	}
	return false;
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
		this->Noteline_prefix.nps 	= noteline_prefix_default.nps ;
		return false;
	}
}

string Note_class::get_name()
{
	return Notefile_name;
}

bool Note_class::set_file_name( string str )
{
	if ( str.length() == 0 )
		return false;

	Notefile_name = str;
	Notefile = "";
	for ( string dir : { file_structure().notesdir , file_structure().autodir} )
	{
		string filename = dir + str + file_structure().nte_type;
		if ( filesystem::exists( filename) )
			Notefile = filename;
	};
	if ( Notefile == "" )
	{
		Notefile = file_structure().notesdir + str  + file_structure().nte_type;
		Comment( ERROR, "note file " + Notefile + " not yet found");
		return false;
	}
	return true;
}

void Note_class::LoadMusicxml( const string& file )
{

}

string Note_class::Read( string str )
{
	String 			Line{""};
	fstream 		File;
	vector_str_t 	Line_arr;

	if ( not set_file_name( str ) )
		return "";

	Comment(INFO, "setup notes for " + get_name() );

	File.open( Notefile, fstream::in );

	Volumeline 	= "";
	Noteline 	= "";
	getline( File, Line.Str );

	do
	{
			Line.replace_comment();
			vector_str_t arr = Line.to_array( '=' );

			switch ( arr[0][0] )
			{
				case 'V' :
				{
					Volumeline.append( arr[1] );
					break;
				}
				case 'N' :
				{
					Noteline.append( arr[1] );
					break;
				}
				case 'P' :
					Noteline_prefix = String_to_noteline_prefix( arr[1] );
					break;
				case '+' :
				{
					break;
				}
				default :
				{
					Noteline.append( arr[0] );
					break;
				}
			} // end switch
	} while( getline( File, Line.Str));

	File.close();

	Noteline_prefix.variation = 0;
	Set_rhythm_line( Volumeline );
	if ( Verify_noteline( Noteline_prefix, Noteline ) )
	{
		Start_note_itr();
		return Noteline;
	}
	else
		return "";

}

void Note_class::Save( string str, noteline_prefix_t prefix, string nl_str )
{
	set_file_name(  str );
	fstream File( Notefile, fstream::out );
	Comment(DEBUG,Error_text( errno ));
	if ( ! File.is_open())
	{
		Comment( ERROR, "Input file: " + Notefile + " does not exist" );
		return ;
	}
	Comment( INFO, "saving notes " + Noteline + "\nto file: " + Notefile );

	string noteline_prefix = Noteline_prefix_to_string( prefix );
	File <<
		"#See the documentation file " + file_structure().doc_filename + "\n" <<
		"#about the specification of Prefix, Notes and Volume \n" <<
		"P=" << noteline_prefix << "\n" <<
		"N=" << nl_str << "\n" <<
		"V=" << Volumeline  <<	endl;

	File.close();
}

void Note_class::Test()
{

	TEST_START( className );
	ASSERTION( Notechar2Step( 'A' ) == 9, "Assert test value ","A", 9  )  ;

	Instrument_name = "NotesTest";
	Comment( TEST, "Note_class test start");

	Comment( TEST, "long notes ");// long note
	Set_rhythm_line("5");
	Verify_noteline( Noteline_prefix, "A-D--B----d-");
	Start_note_itr();
	ASSERTION( fcomp(note_itr->chord[0].freq, 220 ), "", note_itr->chord[0].freq , 220 );
	advance(note_itr, 2 );
	ASSERTION( note_itr->longnote, "Longnote", note_itr->longnote, true  );
	advance(note_itr, 1 );
	ASSERTION( not note_itr->longnote, "Longnote", note_itr->longnote, false );
	advance(note_itr, 1 );
	ASSERTION( note_itr->longnote, "Longnote", note_itr->longnote, true );
	ASSERTION( noteline_sec == 3, "seconds", noteline_sec, 3 );
	ASSERTION( note_itr->chord[0].step == 11, "Assert test value ", note_itr->chord[0].step  , 11 );

	Comment( TEST, "fill note list");
	Set_rhythm_line("5");
	bool vn = Verify_noteline(Noteline_prefix, "ABCD-");
	assert( noteline_sec == 2 );
	assert( vn );

	Comment( TEST, "set volume");
	Set_rhythm_line("844448");
	Verify_noteline(Noteline_prefix, "|2(ACE)--|3G-EC-A--.B");
	int i = 0;
	volume_vec={80,40,40,40,40,40,0,80,0,0,0};
	for( note_t note : notelist )
	{
		ASSERTION( note.volume == volume_vec[i],"", note.volume, volume_vec[i]);
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

	Volumeline = "9797";
	Set_prefix_octave( 0 );
	Noteline = "|3F--G(AC)-(DC)-";
	Noteline_prefix = String_to_noteline_prefix( "4,0,4,0,0");
	Save(".testcase", Noteline_prefix, Noteline );

	Volumeline 	= "yyyyy";
	Noteline 	= "xxxxx";
	Noteline=Read(".testcase");
	ASSERTION( Noteline_prefix.Octave == 4, "", (int)Noteline_prefix.Octave , 3) ;
	assert( noteline_sec == 2 );
	assert( Noteline.compare("|3F--G(AC)-(DC)-") == 0 );
	assert( Volumeline.compare("9797") == 0 );

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


	Noteline=".---";
	Verify_noteline( Noteline_prefix, Noteline );
	Start_note_itr();
	assert( note_itr->volume == 0 );
	assert( note_itr->duration == 4*min_duration );

	Comment( TEST, "garbage test" );
	Noteline="-xyz";
	Set_notes_per_second( 1 );
	Verify_noteline( Noteline_prefix, Noteline );
	assert( min_duration == 1000 );

	Noteline=",xyz";
	Set_notes_per_second( 5 );
	Verify_noteline( Noteline_prefix, Noteline );
	assert( Noteline_prefix.nps == 5 );
	assert( min_duration == 200 );

	Noteline="'xyz";
	Set_notes_per_second( 4 ); // default
	Verify_noteline( Noteline_prefix, Noteline );
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
		note_itr++;
	}

	ASSERTION( Verify_noteline( Noteline_prefix, "|,A(A,a)(A'a)(Aa,)(a,)A'.."),
			"Verify Noteline |'A(A,a)(A'a)(Aa,)(a,)A'..", false, true );
	note_itr = notelist.begin();
	for( float freq : { 220, 110, 440, 220, 117, 440, } )
	{
		ASSERTION( abs( note_itr->chord[0].freq - freq) < 1, "", note_itr->chord[0].freq, freq );
		cout << note_itr->chord[0].freq << " :ok" << endl;
		note_itr++;
	}



//	assert (false);
	TEST_END( className );

	Comment( TEST, "Note_class test done ");
}
