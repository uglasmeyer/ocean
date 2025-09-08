/*
 * playnotes.cpp
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
 */

#include <Config.h>
#include <notes/Notes.h>

Note_class::Note_class( )
	: Note_class::Logfacility_class("Note_class")
	, Note_base()
{
	this->sds 		= nullptr;
	this->instrument= nullptr;
	this->StA		= nullptr;
	this->className = Logfacility_class::className;
}
Note_class::Note_class( interface_t* _sds)
	: Note_class::Logfacility_class("Note_class")
	,  Note_base()
{
	this->className = Logfacility_class::className;
	this->sds		= _sds;
	this->instrument= nullptr;
	this->StA		= nullptr;
}

Note_class::Note_class( Instrument_class* 	instr,
						Storage_class* 		sta )
	: Note_class::Logfacility_class("Note_class")
	, Note_base()
{
	this->className = Logfacility_class::className;
	this->instrument= instr;
	this->StA		= sta;
	this->sds		= instr->sds;

	Oscgroup.SetWd( instr->wd_p );
	Oscgroup.SetScanner( max_frames );
}

Note_class::~Note_class( )
{
	DESTRUCTOR( className )
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
	NotesData = StA->scanner.Next_read();
}

void Note_class::gen_chord_data(const note_t& note,
								const buffer_t& offs,
								const uint& duration,
								const bool& longnote
								)
{

	Oscgroup.Data_Reset();

	uint glide_effect = osc->features.glide_effect;

	if ( note.glide[0].glide )
		Oscgroup.SetSlide( 100 );
	else
		Oscgroup.SetSlide( 0 );

	osc->Set_long_note( note.longnote or longnote );

	uint frame_delay 	= sds->noteline_prefix.chord_delay * frames_per_msec;
	uint n 				= 0;
	for ( pitch_t pitch : note.chord )
	{
		uint8_t key = GetFrqIndex( pitch );
		Oscgroup.Set_Osc_Note( instrument->osc->wp.freq, key, duration, note.volume, SLIDE );
		Oscgroup.Phase_Reset();
		Oscgroup.Run_OSCs( offs + n*frame_delay );
		n++;
	}

	osc->features.glide_effect = glide_effect ;

	return ;
}


void Note_class::sta_write_data( uint duration )
{
	StA->Write_data( Osc->Mem.Data );//, max_frames );
	StA->scanner.Next_write( duration * frames_per_msec );
};

bool Note_class::Generate_volatile_data( bool init )
{
	auto restart_note_itr = [ this ]()
	{
		if (( note_itr == notelist.end() ) or ( Restart )) // the global note iter shall be restarted.
		{
			Start_note_itr();
		}
		Restart = false;
	};

	if( StA->scanner.rpos < max_frames )
		if ( not init )
			return false;

	Oscgroup.SetInstrument( sds );
	StA->Reset();
	restart_note_itr();
	int duration = 0;
	while( ( duration < 2000 ) and (note_itr != notelist.end() ) )
	{
		gen_chord_data( *note_itr, 0, note_itr->duration, false );
		sta_write_data( note_itr->duration );

		duration += note_itr->duration;
		note_itr++;
	}
	if( init )
		Comment( INFO, "volatile data initialized" );

	return true;
}

bool Note_class::Generate_cyclic_data(  )
{
	Oscgroup.SetInstrument( sds );
	Start_note_itr();
	StA->Reset();

	while ( note_itr != notelist.end() )
	{
		gen_chord_data( *note_itr, 0, note_itr->duration, false );
		sta_write_data( note_itr->duration );
		note_itr++;
	}
	StA->scanner.Set_rpos( 0 );
	StA->scanner.Show( false );
	return true;
}

void Note_class::SetSDS( noteline_prefix_t nlp )
{
	if( sds )
		sds->noteline_prefix = nlp;
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
	return notefile_name;
}

bool Note_class::set_file_name( string str )
{
	if ( str.length() == 0 )
		return false;

	notefile_name = str;
	notefile = "";
	for ( string dir : { file_structure().notesdir , file_structure().autodir} )
	{
		string filename = dir + str + file_structure().nte_type;
		if ( filesystem::exists( filename) )
			notefile = filename;
	};
	if ( notefile == "" )
	{
		notefile = file_structure().notesdir + str  + file_structure().nte_type;
		Comment( ERROR, "note file " + notefile + " not yet found");
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


	Noteline_prefix.variation = 0;
	Set_rhythm_line( volumeline );
	if ( Verify_noteline( Noteline_prefix, noteline ) )
	{
		Start_note_itr();
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
		"#See the documentation file " + file_structure().doc_filename + "\n" <<
		"#about the specification of Prefix, Notes and Volume \n" <<
		"P=" << noteline_prefix << "\n" <<
		"N=" << nl_str << "\n" <<
		"V=" << volumeline  <<	endl;

	File.close();
}

void Note_class::Test()
{

	TEST_START( className );
	ASSERTION( Notechar2Step( 'A' ) == 9, "Assert test value ",(int)Notechar2Step( 'A' ), 9  )  ;

	Instrument_name = "NotesTest";
	Comment( TEST, "Note_class test start");

	string nl = "D3----";
	Align_measure( noteline_prefix_default,  nl );
	coutf << nl << endl;
	ASSERTION( nl.length() == 8, "Append_noteline", nl.length(), 8L );
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
	assert( note_itr->duration == 4*min_duration );

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
		note_itr++;
	}

	ASSERTION( Verify_noteline( Noteline_prefix, "|,A(A,a)(A'a)(Aa,)(a,)A'.."),
			"Verify Noteline |,A(A,a)(A'a)(Aa,)(a,)A'..", false, true );
	note_itr = notelist.begin();
	for( float freq : { 220, 110, 440, 220, 117, 440, } )
	{
		ASSERTION( abs( note_itr->chord[0].freq - freq) < 1, "", note_itr->chord[0].freq, freq );
		cout << note_itr->chord[0].freq << " :ok" << endl;
		note_itr++;
	}


	Gen_notelist( noteline_prefix_default, "A2A3-A4--A5---");
	Generate_cyclic_data();

//	assert (false);
	TEST_END( className );


	Comment( TEST, "Note_class test done ");
}
