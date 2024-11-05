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
	this->className = Logfacility_class::module;
}

Note_class::~Note_class( )
{
}

void Note_class::Set_instrument(Instrument_class *instrument) {
	Instrument_name = instrument->Name;
	Comment(TEST, "Update notes instrument:  " + Instrument_name);
	// copy class Oscillator
	osc.wp = instrument->main.wp;
	osc.vp = instrument->main.vp;
	osc.fp = instrument->main.fp;
	osc.vp.data = vco.Mem.Data;
	osc.fp.data = fmo.Mem.Data;
	osc.adsr = instrument->main.adsr;
	osc.spectrum = instrument->main.spectrum;
	vco.wp = instrument->vco.wp;
	vco.vp = instrument->vco.vp;
	vco.fp = instrument->vco.fp;
	vco.spectrum = instrument->vco.spectrum;
	fmo.wp = instrument->fmo.wp;
	fmo.vp = instrument->fmo.vp;
	fmo.fp = instrument->fmo.fp;
	fmo.spectrum = instrument->fmo.spectrum;
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
			Info2( 3, "Rhythm line value " , ch, "not valid" );
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
								const bool& longnote) // TODO working
{
	auto run_osc_group = [ this ]( const buffer_t& offs )
		{
			for ( Oscillator* osc : this->osc_group )
				osc->OSC( offs );
		};

	uint wp_glide_effect = osc.wp.glide_effect;
	const float vco_wp_frequency = vco.wp.frequency;
	const float fmo_wp_frequency = fmo.wp.frequency;

	if ( note.glide[0].note )
		osc.wp.glide_effect = 100;

	osc.Set_long( note.longnote or longnote );

	for ( pitch_t pitch : note.chord )
	{

		float
		fnew = CalcFreq( vco_wp_frequency, pitch );
		vco.Set_start_freq(fnew);
		vco.wp.frequency 	= fnew;
		vco.wp.msec 		= duration;

		fnew = CalcFreq( fmo_wp_frequency, pitch );
		fmo.Set_start_freq(fnew);
		fmo.wp.frequency	= fnew;
		fmo.wp.msec 		= duration;


		fnew = CalcFreq(  oct_base_freq , pitch );
		osc.Set_start_freq( fnew);
		osc.wp.frequency	= fnew;
		osc.wp.volume 		= note.volume ;
		osc.wp.msec 		= duration;
//		cout << fnew << " | " << pitch.freq << " | " << note.duration << endl;
		run_osc_group( offs );
	}

	vco.wp.frequency	= vco_wp_frequency;
	fmo.wp.frequency	= fmo_wp_frequency;
	osc.wp.glide_effect= wp_glide_effect;
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

	this->osc.Mem.Clear_data( 0 );

	restart_note_itr();

	buffer_t 	frame_offset = (frames_per_sec * timestamp)  / 1000 ;
	bool 		partnote = false;//( timestamp != 0);
	int 		lastduration = 0;
	bool		longnote = false;// ( timestamp != 0 );
	while ( note_itr != notelist.end() )
	{

		if ( timestamp == max_milli_sec )
		{
			timestamp = 0;
			return true; // good
		}
		if ( timestamp >  max_sec*1000 )
		{	// considers the end pause to finish
			timestamp = timestamp % max_milli_sec;

			Comment(INFO, "take over " + to_string( timestamp ) + "[msec]");
			note_itr++;
			return false;
		}

		uint duration = rint( note_itr->duration * 2.0 ) ;// musicxml.tempo;
		scoretime += duration;
		if ( timestamp + duration > max_milli_sec )
		{
			lastduration = duration;
			duration = max_milli_sec - timestamp ;
			partnote = true;
			cout << "part start ";
		}
		note2memory( *note_itr, frame_offset, duration, longnote );

		longnote = false;
		timestamp = timestamp + duration;
		frame_offset = (frames_per_sec * timestamp)  / 1000 ;

		if ( partnote )
		{ 	//unaligned end of the measure

			timestamp = lastduration - duration;
			cout << "T: " << timestamp << endl;
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
		Comment(ERROR, "Notes are not aligned by " + to_string( timestamp ) + "[msec]");
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

void Note_class::set_file_name( string str )
{
	Notefile_name = str;
	Notefile = "";
	for ( string dir : { file_structure().Dir.notesdir , file_structure().Dir.autodir} )
	{
		string filename = dir + str + file_structure().nte_type;
		if ( filesystem::exists( filename) )
			Notefile = filename;
	};
	if ( Notefile == "" )
	{
		Notefile = file_structure().Dir.notesdir + str  + file_structure().nte_type;
		Comment( ERROR, "note file " + Notefile + " not yet found");
	}
}

string Note_class::Read( string str )
{
	String 			Line{""};
	fstream 		File;
	vector_str_t 	Line_arr;

	set_file_name(str);
	Comment(INFO, "setup notes for " + get_name() );

	File.open( Notefile, fstream::in );
	if ( ! File.is_open())
	{
		Comment( ERROR, "Input file: " + Notefile + " does not exist" );
		return "";
	}
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
	Verify_noteline( Noteline_prefix, Noteline );
	Start_note_itr();

	return Noteline;

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
	Note_base::TestNoteBase();

	Assert( Notechar2Step( 'A' ) == 0, "Assert test value " + to_string( Notechar2Step( 'A' ) ) );

	Instrument_name = "NotesTest";
	Set_Loglevel(TEST, true );
	Comment( TEST, "Note_class test start");

	Comment( TEST, "long notes ");// long note
	Set_rhythm_line("5");
	Verify_noteline( Noteline_prefix, "C-D--B----d-");
	note_itr = notelist.begin();
	advance(note_itr, 2 );
	assert( note_itr->longnote );
	advance(note_itr, 1 );
	assert( not note_itr->longnote );
	advance(note_itr, 1 );
	assert( note_itr->longnote );
	assert( noteline_sec == 3 );
	assert( note_itr->chord[0].step == 2 );

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
		cout << dec << volume_vec[i] << " " << note.volume << endl;
		assert( note.volume == volume_vec[i] );
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
	Noteline_prefix = String_to_noteline_prefix( "2,0,4,0,0");
	Save(".testcase", Noteline_prefix, Noteline );

	Volumeline 	= "yyyyy";
	Noteline 	= "xxxxx";
	Noteline=Read(".testcase");

	assert( noteline_sec == 2 );
	assert( Noteline.compare("|3F--G(AC)-(DC)-") == 0 );
	assert( Volumeline.compare("9797") == 0 );

	Verify_noteline( Noteline_prefix,  "|3F--|2G(AC)-|3(DC)-");
	assert( Octave == 3 );
	note_itr = notelist.begin();
//	for ( note_struct_t note : notelist )
//		cout << dec<<(int)note.octave << endl;
	advance(note_itr, 2);
	assert( note_itr->octave == 2 );

	set_volume_vector("");
	assert( volume_vec[0] == notes_default_volume );

	note_itr = notelist.begin();
	advance(note_itr, 2);
	cout << note_itr->duration ;
	assert( note_itr->duration == 2*min_duration );


	Noteline=".---";
	Verify_noteline( Noteline_prefix, Noteline );
	note_itr = notelist.begin();

	assert( note_itr->volume == 0 );
	assert( note_itr->duration == 4*min_duration );
//	cout << note_itr->chord[0].value << " " << note_itr->chord[0].doct << endl;
//	cout << Octave << endl;
//	cout << calc_freq( Octave, note_itr->chord[0] ) << endl;
	assert( Calc_freq( Octave, note_itr->chord[0] ) <  0.1 );
	//assert( false );

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

	Verify_noteline( Noteline_prefix, "|2(AB)--->|3A" );
	note_t note = *notelist.begin();
	assert( note.glide[0].chord.step == 0 );
	assert( note.glide[0].chord.alter == 1 );
	assert ( ( Calc_freq( note.octave,  note.glide[0].chord ) - 440 ) < 1E-8 );
	Comment( INFO, "Test Note_class OK" );

	Verify_noteline( Noteline_prefix, "A'(AB)(A'B)(AB,)(B,)A'..");
	note_itr = notelist.begin();
	for( int freq : { 220, 110, 220, 110, 61, 220, 0, 0 } )
	{
//		printf("%d = %f %d\n", freq, calc_freq( 1, note_itr->chord[0]), note_itr->chord[0].doct ) ;
		assert( Calc_freq( 1, note_itr->chord[0] ) - freq < 1);
		note_itr++;
	}

	Verify_noteline( Noteline_prefix, "A|'AA,A|,AAA");
	note_itr = notelist.begin();
	for( int freq : { 110, 220, 110, 220, 55, 55, 55 } )
	{
		Show_note( *note_itr );
//		cout << freq << " = " << calc_freq( 1, note_itr->chord[0] ) << endl;
		assert( Calc_freq( 1, note_itr->chord[0] ) - freq < 1);
		note_itr++;
	}


//	assert (false);
	Comment( TEST, "Note_class test done ");
}
