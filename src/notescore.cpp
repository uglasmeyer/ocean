/*
 * notescore.cpp
 *
 *  Created on: Oct 27, 2024
 *      Author: sirius
 */
#include <notes/Notes.h>






auto calc_noteline_msec = []( Note_class* C )
	{
		uint duration = 0;
		for( Note_class::note_t note : C->notelist )
		{
			duration += note.duration;
		}
		return duration;
	};


void Note_class::Set_notelist( const notelist_t& nlst )
{
	notelist = nlst;
	Start_note_itr();
//	noteline_sec = calc_noteline_msec( this ) / measure_duration;

	Noteline_prefix = noteline_prefix_default;
	Noteline_prefix.Octave = 0;
//	add_volume( note_itr );
	Show_note_list( nlst ); // @suppress("Invalid arguments")
}



bool Note_class::Verify_noteline( noteline_prefix_t prefix, string str ) // used by GUI
{


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

	if ( not compiler( prefix, str ) )
		return false ;

	// post check
	uint noteline_msec = calc_noteline_msec( this );
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

Note_class::note_t Note_class::Start_note_itr()
{
	note_itr = notelist.begin();
	return *note_itr;
}


void Note_class::Show_note( note_t note )
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
	for ( pitch_t pitch : note.chord )
	{
				strs <<
				setw(4) << right << dec << (int)pitch.octave <<
				setw(4) << right << pitch.alter <<
				setw(5) << right << dec << rint( pitch.freq ) <<"|";
	}

	if ( note.glide[0].note )
	{
		strs 	<< ">" << setw(12) << right << dec << rint(note.glide[0].chord.freq);
	}
	Comment( DEBUG, strs.str()  );

}

int Note_class::Notechar2Step( char note_char )
{
	if ( Note_Chars.Set.contains( note_char ) )
		return Note_Chars.Str.find( note_char );
	else
		return -12;
}

Note_class::note_t Note_class::Char2note( char& ch )
{
	pitch_t pitch 			= pitch_struct();
	pitch.step 				= Notechar2Step( ch );
	pitch.step_char			= ch;
	pitch.alter				= 0;
	pitch.octave			= Octave;
	pitch.freq 				= CalcFreq( oct_base_freq, pitch );

	note_buffer.chord.push_back( pitch );
	note_buffer.str.push_back(ch);
	note_buffer.duration    		= min_duration; // will be updated later
	note_buffer.glide[0].chord.step	= note_buffer.chord[0].step;
	if ( note_buffer.chord[0].step 	< 0 ) // pause is silence
		note_buffer.volume 			= 0;
	else
		note_buffer.volume 			= notes_default_volume;//note volume is changed by mixer anf Volumeline
	return note_buffer;
}

void Note_class::Set_prefix_octave( int oct )
{
	char oct_ch = int2char( oct );
	if ( not OctaveChars.Set.contains( oct_ch ) ) return; // (nil)
	Noteline_prefix.Octave = oct;
	Octave = oct;
}

size_t Note_class::noteline_position_parser(  size_t pos )
{
	// parse a single noteline position and apply changes to the note_itr

	auto get_oct_value = [ this ]( char ch )
		{
			return ( OctaveChars.Set.contains( ch ) ) ? char2int( ch )  : -1 ;
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
			note_itr->glide[0].note = true;
			pos++; //
			if ( Noteline[pos] == '|' ) // oct change case >|3F
			{
				note_itr->str.push_back('|');
				pos++;
				if ( out_of_bounds( pos ) ) return noteline_len; // test pos
				int oct = get_oct_value( Noteline[pos] );
				if ( oct > 0 )
				{
					note_itr->glide[0].chord.octave	= oct;// - note_itr->chord[0].octave ;
					note_itr->str.push_back( Noteline[pos] );
					pos++;
					if ( out_of_bounds( pos ) ) return noteline_len; // test pos
					note_itr->str.push_back( Noteline[pos] );
					note_itr->glide[0].chord.step 	= Notechar2Step( Noteline[pos] );
				}
			}
			else // simple case >F
			{
				note_itr->glide[0].chord.alter 	= 0;
				note_itr->glide[0].chord.step = Notechar2Step( Noteline[pos] );
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
				if ( Note_Chars.Set.contains( ch ) )
				{
					Char2note( Noteline[pos] ); // append note_buffer
					nc_pos++;
				}
				if ( ch == '\'' )
				{
					note_buffer.chord.back().octave += 1;
					note_buffer.str.push_back(ch);
				}
				if ( ch == ',' )
				{
					note_buffer.chord.back().octave -= 1;
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
			note_itr->chord.back().octave += 1;
			note_itr->str.push_back( '\'' );
			break;
		}
		case ',' :
		{
			if ( out_of_bounds( pos ) ) return pos++;
			note_itr->chord.back().octave -= 1;
			note_itr->str.push_back(',');
			break;
		}
		case '|' : // nextchar may define a new octave
		{
			pos++;

			if ( Noteline[pos] == '\'' ) Octave += 1;
			if ( Noteline[pos] == ','  ) Octave -= 1;

			int oct = get_oct_value( Noteline[pos] );
			if ( oct < 0 )
				return parse_error;
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
	if ( prefix.convention != ALPHABET ) return;


	for( note_struct& note : notelist )
	{
		for( uint sharp = 0; sharp < prefix.sharp; sharp++ )
		{
			for ( pitch_t& chord : note.chord )
			{
				if( chord.step == sharp_pitch[sharp])
				{
					chord.step +=1;
					note.str.append("#");
				}
			}
		}
		for( uint flat = 0; flat < prefix.flat; flat++ )
		{
			for ( pitch_t& chord : note.chord )
			{
				if( chord.step == flat_pitch[flat])
				{
					chord.step -=1;
					note.str.append("b");
				}
			}
		}
	}
}

void Note_class::assign_freq()
{
	note_itr_t itr = notelist.begin();
	while( itr != notelist.end( ))
	{
		for( size_t i = 0; i < itr->chord.size() ; i++ )
		{
			itr->chord[i].freq = CalcFreq( oct_base_freq, itr->chord[i] );
		}
		if ( itr->glide[0].note )
		{
			itr->glide[0].chord.freq = CalcFreq( oct_base_freq, itr->glide[0].chord );
		}
		else
		{
			itr->glide[0].chord.freq = itr->chord[0].freq;
		}
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
			if ( note_buffer.glide[0].note )
			{
				float dfreq = note_buffer.glide[0].chord.freq - note_buffer.chord[0].freq;
				note_buffer.glide[0].chord.freq = note_buffer.chord[0].freq + dfreq * (note_buffer.duration-rest)/(note_buffer.duration);
			}
			note_buffer.duration = note_buffer.duration - rest;
			notelist.insert( itr, note_buffer );
			//new
			itr->duration = rest;
			if ( note_buffer.glide[0].note )
			{
				itr->glide[0].chord.freq = CalcFreq( oct_base_freq, itr->glide[0].chord );
				itr->chord[0].freq = note_buffer.glide[0].chord.freq;
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
			for( note_t note : notelist )
			{
				duration += note.duration;
			}
			return duration;
		};

	uint noteline_msec = calc_noteline_msec();
	noteline_sec = 0;

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

bool Note_class::compiler ( noteline_prefix_t prefix, string str )
{
	string prefix_str 	= convention_names[ prefix.convention ];
	String Note_Chars 	{ convention_notes[ prefix.convention ] };
	Octave 				= prefix.Octave;
	Noteline 	  		= str;

	Comment(INFO, "Instrument : " + Instrument_name );
	Comment(INFO, "Notes name : " + Notefile_name );
	Show_noteline_prefix( prefix );
	Comment(INFO, "Note line  : " + Noteline );
	Comment(INFO, "Rhythm line: " + Volumeline );

	noteline_len = Noteline.length();

	if ( noteline_len  == 0 )
		return false;
	parse_error = noteline_len + 1;
	notelist.clear();
	set_volume_vector( Volumeline );

	note_buffer			= note_struct();
	note_duration		= 0;

	size_t char_pos 	= 0;
	while( char_pos < noteline_len )
	{
		char_pos = noteline_position_parser( char_pos );
		char_pos++;
	};

	if ( char_pos >= parse_error )
		return false;

	change_alphabet_notes( prefix );
	assign_freq();
	add_volume( note_itr );

	if ( not prefix.variation )
	{
		split_long_notes();
		fill_note_list();
	}

	Show_note_list( notelist ); // @suppress("Invalid arguments")
	return true;
}



