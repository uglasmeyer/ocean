/*
 * playnotes.cpp
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
 */

#include <notes.h>


bool cmpstr( const string a, const string b )
{
	return ( a.compare( b ) == 0 );
}

Note_class::Note_class()
: Note_class::Logfacility_class("Notes"),
  Note_base()
{

	instrument = nullptr;

}

Note_class::Note_class( Instrument_class* instr )
: Note_class::Logfacility_class("Notes"),
  Note_base()
{

	instrument = instr;

}

Note_class::~Note_class()
{
}

void Note_class::Set_osc_track(  )
{
	Comment( INFO, "Set osc track");

	// copy class Oscillator
	main.wp 		= instrument->main.wp;
	main.vp 		= instrument->main.vp;
	main.fp 		= instrument->main.fp;
	main.vp.data	= vco.Mem.Data;
	main.fp.data	= fmo.Mem.Data;
	main.adsr 		= instrument->main.adsr;
	main.spectrum	= instrument->main.spectrum;

	vco.wp 			= instrument->vco.wp;
	vco.vp 			= instrument->vco.vp;
	vco.fp 			= instrument->vco.fp;
	vco.spectrum	= instrument->vco.spectrum;

	fmo.wp 			= instrument->fmo.wp;
	fmo.vp 			= instrument->fmo.vp;
	fmo.fp 			= instrument->fmo.fp;
	fmo.spectrum	= instrument->fmo.spectrum;

	Set_prefix_octave( main.wp.fstruct.oct );

	return;
}

void Note_class::Set_base_octave( uint diff )
{
	octave_shift = octave_shift + ( 2*diff - 1);
	if ( octave_shift < 0 )
		octave_shift = 0;
}

bool Note_class::Verify_noteline( noteline_prefix_t prefix, string str ) // used by GUI
{

	auto calc_noteline_msec = [this]()
		{
			uint duration = 0;
			for( note_struct_t note : notelist )
			{
				duration += note.duration;
			}
			return duration;
		};

	auto brackets_aligned = [str]()
		{
			int c=0;
			for ( char ch : str )
			{
				switch (ch )
				{
				case '(' : {c++; break;}
				case ')' : {c--; break;}
				default  : break;
				} // switch
				if ( c < 0 ) return false;
			} //for
			return ( c == 0 );
		}; // lambda

	// pre check
	if ( str.length() == 0 )
	{
		Comment( WARN, "Empty note line ");
		return false;

	}
	if ( not brackets_aligned() )
	{
		Comment( ERROR, "brackets () are not aligned" );
		return false;
	}

	min_duration 			= measure_duration / prefix.nps;

	set_note_list( prefix, str );

	// post check
	uint noteline_msec = calc_noteline_msec();
	noteline_sec = 0;

	int mod = noteline_msec % measure_duration ;
	if ( mod == 0 )
	{
		noteline_sec = noteline_msec / measure_duration;
		Comment(INFO, "Note line duration: " + to_string( noteline_sec ) + " [sec]");
		return true;
	}
	else
	{
		Comment( ERROR, "notes are not aligned to " +
				to_string(prefix.nps) + " * " +
				to_string(min_duration) + " = " +
				to_string(prefix.nps*min_duration) + " [msec]" );
		noteline_sec = 0;
		return false;
	}
	if ( noteline_sec > tmpduration )
	{
		Comment( ERROR, "The total duration of this sentence ");
		Comment( ERROR, "exceeds the storange duration of" +to_string( tmpduration ) + " seconds." );
		noteline_sec = 0;
		return false;
	}
	return true;
}

void Note_class::Start_note_itr()
{
	note_itr = notelist.begin();
}

float Note_class::calc_freq ( uint8_t oct, notevalue_struct_t nvs )
{
	if ( nvs.value < 0 )
		return 0.0;
	uint8_t		octave	= abs( oct + nvs.doct );
	uint16_t 	base 	= oct_base_freq  << ( octave_shift + octave ) ;
	float	 	freq 	= base * root2.vec[ nvs.value ];
	return 		freq;
};

void Note_class::Show_note( note_struct_t note )
{
	stringstream strs;
	strs << dec  << setfill(' ') 	<< left ;
	strs << setw(12) << left  << note.str 	;
	strs << setw(5)  << right << dec << (int)note.volume ;
	strs << setw(6)  << right << dec << (int)note.duration;
	if ( note.longnote )
		strs << "L";
	else
		strs << "|";
	for ( notevalue_struct_t notevalue : note.chord )
	{
				strs <<
				setw(4) << right << dec << (int)note.octave <<
				setw(4) << right << dec << (int)notevalue.doct <<
				setw(5) << right << dec << rint( notevalue.freq ) <<"|";
	}
	if ( note.glide.note )
	{
		strs 	<< ">" << setw(5) << right << dec << rint(note.glide.chord.freq);
	}
	Comment( INFO, strs.str()  );

}

int Note_class::notechar2Value( char note_char )
{
	size_t pos	= Note_Chars.find( note_char );
	if ( pos != string::npos )
		return pos;
	else
		return -12; // freq = 0
}

Note_class::note_struct_t Note_class::Char2note( char ch )
{
	note_buffer.str.push_back(ch);
	notevalue_buffer 				= notevalue_struct();
	notevalue_buffer.value 			= notechar2Value( ch );
	note_buffer.chord.push_back( notevalue_buffer );
	note_buffer.octave				= Octave;
	note_buffer.duration    		= min_duration; // will be updated later
	note_buffer.glide.chord. value	= note_buffer.chord[0].value;
	if ( note_buffer.chord[0].value < 0 ) // pause is silence
		note_buffer.volume 			= 0;
	else
		note_buffer.volume 			= osc_default_volume;//note volume is changed by mixer anf Volumeline
	return note_buffer;
}
void Note_class::Set_prefix_octave( int oct )
{
	char oct_ch = 48+oct;
	if ( OctaveChars.find( oct_ch ) == string::npos ) return; // (nil)
	Noteline_prefix.Octave = oct;
	Octave = oct;
}

strlen_t Note_class::Noteline_position_parser(  strlen_t pos )
{
	// parse a single noteline position and apply changes to the note_itr

	auto get_oct_value = [ this ]( char ch )
		{
			size_t found = OctaveChars.find( ch );
			return ( found == string::npos ) ? (size_t)Octave : found + 1;
		};

	auto set_duration = [ this ]()
		{
		if ( note_duration > 0 )
			{ // update the existing note by increasing the length of the note
				note_itr->duration 	= min_duration * ( note_duration + 1 ); // +1 is the duration of the stored note value there
				note_duration	= 0;
			}
		};

	auto out_of_bounds = [ this ]( size_t pos )
		{
		if (( note_itr == notelist.end() ) or ( pos > noteline_len - 1 ))
		{
			Comment( ERROR, "Our of bounds occured at position " + to_string(pos) + " of " );
			Comment( ERROR, Noteline );
			return true;
		}
		else
			return false;
		};


	char note_char = Noteline[pos];
	note_itr = notelist.end();
	note_itr--;
	switch (note_char )
	{
		case '>' : // allowed: >F or >|3F
		{
			if ( out_of_bounds( pos ) ) return noteline_len; // test note_itr
			note_itr->str.push_back( '>' );
			note_itr->glide.note = true;
			pos++; //
			if ( Noteline[pos] == '|' ) // oct change case >|3F
			{
				note_itr->str.push_back('|');
				pos++;
				if ( out_of_bounds( pos ) ) return noteline_len; // test pos
				int oct = get_oct_value( Noteline[pos] );
				if ( oct > 0 )
				{
					note_itr->glide.chord.doct 	= oct - note_itr->octave ;
					pos++;
					if ( out_of_bounds( pos ) ) return noteline_len; // test pos
					note_itr->str.push_back( Noteline[pos] );
					note_itr->glide.chord.value 	= notechar2Value( Noteline[pos] );
				}

//				return pos;// new pos
			}
			else // simple case >F
			{
				note_itr->glide.chord.doct 	= 0;
				note_itr->glide.chord.value = notechar2Value( Noteline[pos] );
				note_itr->str.push_back( Noteline[pos] );
			}
			break;
		}
		case '(' :
		{
			set_duration();
			note_buffer = note_struct(); // clear note_buffer;
			note_buffer.str = "(";
			pos++;
			int nc_pos = 0;
			char ch = Noteline[pos];
			while( ch != ')')
			{
				size_t p = Note_Chars.find(ch);
				if (  p != string::npos )
				{
					Char2note( Noteline[pos] ); // append note_buffer
					nc_pos++;
				}
				if ( ch == '\'' )
				{
					note_buffer.chord[nc_pos-1].doct = +1;
					note_buffer.str.push_back(ch);
				}
				if ( ch == ',' )
				{
					note_buffer.chord[nc_pos-1].doct = -1;
					note_buffer.str.push_back(ch);
				}
				pos++;
				ch = Noteline[pos];
			}
			pos--; // close bracket pos
			break;
		}
		case ')' :
		{
			note_buffer.str.append( ")" );
			notelist.push_back( note_buffer );
			break;
		}
		case '-' :
		{
			if ( out_of_bounds( pos ) ) return noteline_len;
			note_duration++; // increase tge duration of a note
			note_itr->str.push_back( '-' );
			break;
		}
		case '\'' :
		{
			if ( out_of_bounds( pos ) ) return pos++;
			note_itr->chord.back().doct = +1;
			note_itr->str.push_back( '\'' );
			break;
		}
		case ',' :
		{
			if ( out_of_bounds( pos ) ) return pos++;
			note_itr->chord.back().doct = -1;
			note_itr->str.push_back(',');
			break;
		}
		case '|' : // nextchar may define a new octave
		{
			pos++;
			int oct = get_oct_value( Noteline[pos] );
			Set_prefix_octave( oct );
			return pos;
			break;
		}
		case ' ' :
		{
			//do nothing, just break
			break;
		}
		default : 	// this note-char is not a special character (<>-|)
					// => this note-char is a note that must be stored in the note-list
		{
			set_duration(); // duration of the previou note
			note_buffer = note_struct(); // clear note_buffer;
			Char2note( note_char );
			note_buffer.octave = Octave;
			notelist.push_back( note_buffer );
			note_buffer = note_struct(); // clear note_buffer once more
			break;
		}
	} // switch
	if ( ( pos == noteline_len - 1 )  )
		set_duration();
	return pos;
}

void Note_class::change_alphabet_notes( noteline_prefix_t prefix )
{
	// change alphabet notes only
	if ( prefix.convention != 2 ) return;


	for( note_struct& note : notelist )
	{
		for( uint sharp = 0; sharp < prefix.sharp; sharp++ )
		{
			for ( notevalue_struct_t& chord : note.chord )
			{
				if( chord.value == sharp_pitch[sharp])
					chord.value +=1;
			}
		}
		for( uint flat = 0; flat < prefix.flat; flat++ )
		{
			for ( notevalue_struct_t& chord : note.chord )
			{
				if( chord.value == flat_pitch[flat])
					chord.value -=1;
			}
		}
	}
}

void Note_class::assign_freq()
{
	note_itr_t itr = notelist.begin();
	while( itr != notelist.end( ))
	{
		for( size_t i = 0; i<itr->chord.size() ; i++ )
		{
			itr->chord[i].freq = calc_freq( itr->octave, itr->chord[i] );
		}
		if ( itr->glide.note )
		{
			itr->glide.chord.freq = calc_freq( itr->octave, itr->glide.chord );
		}
		else
			itr->glide.chord.freq = itr->chord[0].freq;
		itr++;
	}
}
void Note_class::split_long_notes()
{
	note_itr_t itr = notelist.begin();
	uint sdur = 0;
	while( itr != notelist.end( ))
	{
		uint dur = sdur + itr->duration;
		sdur = ( sdur + itr->duration ) % measure_duration;
		uint rest 	= sdur % measure_duration;
		if ( dur > measure_duration )
		{
			// old
			note_buffer = *itr;
			if ( note_buffer.glide.note )
			{
				float dfreq = note_buffer.glide.chord.freq - note_buffer.chord[0].freq;
				note_buffer.glide.chord.freq = note_buffer.chord[0].freq + dfreq * (note_buffer.duration-rest)/(note_buffer.duration);
			}
			note_buffer.duration = note_buffer.duration - rest;
			notelist.insert( itr, note_buffer );
			//new
			itr->duration = rest;
			if ( note_buffer.glide.note )
			{
				itr->glide.chord.freq = calc_freq(itr->octave, itr->glide.chord );
				itr->chord[0].freq = note_buffer.glide.chord.freq;
			}
			itr->longnote = true;
		}
		itr++;
	}
}

void Note_class::fill_note_list()
{
	auto calc_noteline_msec = [this]()
		{
			uint duration = 0;
			for( note_struct_t note : notelist )
			{
				duration += note.duration;
			}
			return duration;
		};

	uint noteline_msec = calc_noteline_msec();
	noteline_sec = 0;
	note_struct_t	pause_note		= {".",{{0,-12}},min_duration,0,0,{0,-12},false };

	int mod = noteline_msec % measure_duration ;
	if ( mod > 0 )
	{
		uint16_t dur = 0;
		while ( dur < measure_duration - mod )
		{
			notelist.push_back( pause_note );
			dur += min_duration;
		}
	}
}

void Note_class::add_volume( note_itr_t itr )
{
	auto set_volume = [ this ]( note_itr_t itr )
		{
		if( itr->volume != 0 )
			itr->volume = volume_vec[ this->vcounter ];

		assert( itr->volume < 100 ); // volume 0 ... 90 %
		};

	itr = notelist.begin();
	vcounter = 0;
	while( itr != notelist.end( ))
	{
		set_volume( itr );
		vcounter = ( vcounter + 1 ) % ( volume_vec_len );
		itr++;
	}
}

void Note_class::set_note_list ( noteline_prefix_t prefix, string str )
{
	string prefix_str 	= convention_names[ prefix.convention ];
	Note_Chars 			= convention_notes[ prefix.convention ];
	Octave 				= prefix.Octave;
	Noteline 	  		= str;

	Comment(INFO, "Instrument : " + Instrument_name );
	Comment(INFO, "Notes name : " + Notefile_name );
	show_noteline_prefix( prefix );
	Comment(INFO, "Note line  : " + Noteline );
	Comment(INFO, "Rhythm line: " + Volumeline );

	noteline_len = Noteline.length();

	if ( noteline_len  == 0 ) return;
	notelist.clear();
	set_volume_vector( Volumeline );

	note_buffer			= note_struct();
	note_duration		= 0;

	strlen_t char_pos 	= 0;
	while( char_pos < noteline_len )
	{
		char_pos = Noteline_position_parser( char_pos );
		char_pos++;
	}

	change_alphabet_notes( prefix );
	assign_freq();
	add_volume( note_itr );

	if ( not prefix.variation )
	{
		split_long_notes();
		fill_note_list();
	}

	Show_note_list( notelist );
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
		this->Volumeline = to_string( osc_default_volume / 10 );
	else
		this->Volumeline = volline;

	volume_vec.clear();
	for ( char ch : this->Volumeline )
	{
		if (( ch > 48 ) and ( ch < 58 ))
			volume_vec.push_back( (ch - 48) * 10);
	}

	volume_vec_len = volume_vec.size();
	if ( volume_vec_len == 0 ) // no valid chars
	{
		this->Volumeline = to_string( osc_default_volume / 10 );
		volume_vec.push_back( osc_default_volume );
		volume_vec_len = volume_vec.size();
	}
	assert( volume_vec.size() > 0 );
}

void Note_class::note2memory( note_struct_t note, buffer_t offs ) // TODO working
{
	auto run_osc_group = [ this ]( buffer_t offs )
		{
			for ( Oscillator* osc : this->osc_group )
				osc->OSC( offs );
		};

	assert( instrument != nullptr );

	float fnew = 0;
	float fglide = note.glide.chord.freq;

	uint16_t tmp_glide_freq = main.wp.glide_effect;
	if ( note.glide.note )
		main.wp.glide_effect = 100;

	main.Set_long( note.longnote );

	for ( notevalue_struct_t notevalue : note.chord )
	{
		fnew = ( notevalue.freq * 	vco.wp.frequency ) /
									instrument->main.wp.frequency;
		vco.wp.frequency 	= fnew;
		vco.wp.msec 		= note.duration;

		fnew = ( notevalue.freq * 	fmo.wp.frequency ) /
									instrument->main.wp.frequency ;
		fmo.wp.frequency	= fnew;
		fmo.wp.msec 		= note.duration;

		main.Set_start_freq( notevalue.freq );
		main.wp.frequency	= fglide;
		main.wp.volume 		= note.volume ;
		main.wp.msec 		= note.duration;

		run_osc_group( offs );
	}

	main.wp.glide_effect = tmp_glide_freq;
	return ;
}

void Note_class::submit_data(Storage::Storage_class* 		mb)
{
	mb->reset_counter();
	mb->status.store = true;
	mb->store_block( this->main.Mem.Data );
}

bool Note_class::Generate_note_chunk( Storage::Storage_class* 		mb )
{ 	// generate the memory track for positon n = note_pointer tp
	// n = note_pointer + chunk_len
	auto restart_note_itr = [ this ]()
	{
		if ( note_itr == notelist.end() ) // the global note iter shall be restarted.
		{
			note_itr = notelist.begin(); // track , good
		}
	};


	int timestamp = 0;
	this->main.Mem.clear_data( 0 );

	restart_note_itr();

	while ( note_itr != notelist.end() )
	{
		buffer_t frame_offset = (max_frames * timestamp)  / 1000 ;

		Show_note( *note_itr );
		Note_class::note2memory( *note_itr, frame_offset );
		timestamp = timestamp + note_itr->duration;

		note_itr++;
		if ( timestamp == max_sec*1000 )
		{
			submit_data(mb);
			return true; // good
		}
		if ( timestamp >  max_sec*1000 )
		{
			Comment(WARN, "Notes are not aligned by " + to_string( timestamp ) + "[msec]");
			return false;
		}
	}
	note_itr = notelist.begin(); // track done , time is over

	if ( timestamp == 0 )
	{
		submit_data( mb );
		return true;
	}
	else
	{
		Comment(WARN, "Notes are not aligned by " + to_string( timestamp ) + "[msec]");
	}
	return false;
}

bool Note_class::Set_notes_per_second( int notes_per_second )
{	// [ 1,2,4,8 ] notes per second

	auto check_nps = [ this ]( int i )
		{
		for( int n : Notes_per_Sec )
			if ( n==i) return true;
		return false;
		};


	if ( check_nps( notes_per_second ) )
	{
		this->Noteline_prefix.nps 	= notes_per_second;
		return true;
	}
	else
	{
		Comment( ERROR, "set notes per second to " + to_string( notes_per_second ) + " rejected ");
		Comment( INFO, "not in list: " + NPS_string );
		this->Noteline_prefix.nps 	= 4;
		return false;
	}
}

string Note_class::get_name()
{
	return Notefile_name;
}

void Note_class::set_name( string str )
{
	Notefile = "";
	for ( string dir : { dir_struct().notesdir , dir_struct().autodir} )
	{
		string filename = dir + str + "." + file_structure().notes_type;
		if ( filesystem::exists( filename) )
			Notefile = filename;
	};
	if ( Notefile == "" )
	{
		Notefile = dir_struct().notesdir + str + "." + file_structure().notes_type;
		Comment( ERROR, "note file " + Notefile + " not yet found");
	}
}

string Note_class::Read( string str )
{
	String Line{""};
	fstream File;
	set_name(str);
	vector_str_t Line_arr;
	Comment(INFO, "setup notes for " + get_name() );

	File.open( Notefile, fstream::in );
	if ( ! File.is_open())
	{
		Comment( ERROR, "Input file: " + Notefile + " does not exist" );
		return "";
	}
	Volumeline = "";
	Noteline = "";
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
					Noteline_prefix = string_to_noteline_prefix( arr[1] );
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

	Note_class::Noteline_prefix.variation = 0;
	Set_rhythm_line( Volumeline );
	Verify_noteline( Noteline_prefix, Noteline );
	Start_note_itr();

	return Noteline;

}

void Note_class::Save( string str, noteline_prefix_t prefix, string nl_str )
{
	set_name(  str );
	fstream File( Notefile, fstream::out );
	Comment(DEBUG,error_text( errno ));
	if ( ! File.is_open())
	{
		Comment( ERROR, "Input file: " + Notefile + " does not exist" );
		return ;
	}
	Comment( INFO, "saving notes " + Noteline + "\nto file: " + Notefile );

	string noteline_prefix = noteline_prefix_to_string( prefix );
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
	Note_base::test();

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
	assert( note_itr->chord[0].value == 2 );

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
	for( note_struct_t note : notelist )
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
	assert( volume_vec[0] == osc_default_volume );

	Volumeline = "9797";
	Set_prefix_octave( 0 );
	Noteline = "|3F--G(AC)-(DC)-";
	Noteline_prefix = string_to_noteline_prefix( "2,0,4,0,0");
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
	assert( volume_vec[0] == osc_default_volume );

	note_itr = notelist.begin();
	advance(note_itr, 2);
	cout << note_itr->duration ;
	assert( note_itr->duration == 2*min_duration );


	Noteline=".---";
	Verify_noteline( Noteline_prefix, Noteline );
	note_itr = notelist.begin();

	assert( note_itr->volume == 0 );
	assert( note_itr->duration == 4*min_duration );
	cout << note_itr->chord[0].value << " " << note_itr->chord[0].doct << endl;
	cout << Octave << endl;
	cout << calc_freq( Octave, note_itr->chord[0] ) << endl;
	assert( calc_freq( Octave, note_itr->chord[0] ) == 0 );
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
	note_struct_t note = *notelist.begin();
	assert( note.glide.chord.value == 0 );
	assert( note.glide.chord.doct == 1 );
	assert ( ( calc_freq( note.octave,  note.glide.chord ) - 440 ) < 1E-8 );
	Comment( INFO, "Test Note_class OK" );

	Verify_noteline( Noteline_prefix, "A'(AB)(A'B)(AB,)(B,)A'..");
	note_itr = notelist.begin();
	for( int freq : { 220, 110, 220, 110, 61, 220, 0, 0 } )
	{

		assert( calc_freq( 1, note_itr->chord[0] ) - freq < 1);
		note_itr++;
	}

//	assert (false);
	Comment( TEST, "Note_class test done ");
}
