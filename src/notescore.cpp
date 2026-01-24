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
 * notescore.cpp
 *
 *  Created on: Oct 27, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <notes/Notes.h>


void Note_class::Align_measure( noteline_prefix_t prefix, string& noteline )
{
	notelist_t nl { Gen_notelist( prefix, noteline ) };
	uint nl_duration_msec = Calc_notelist_msec( nl );
	uint mod = ( nl_duration_msec % measure_duration ) / min_duration;
	for ( uint n = 0; n < mod; n++ )
	{
		noteline.append( "." );
	}

}
uint Note_class::Calc_notelist_msec( notelist_t notelist )
	{
		uint duration = 0;
		for( note_t note : notelist )
		{
			duration += note.duration;
		}
		return duration;
	};


void Note_class::Set_xmlnotelist( const notelist_t& nlst )
{
	notelist = nlst;
	sds->Noteline_sec = musicxml.scoreduration / 1000;

	Noteline_prefix = nlp_default;
	Noteline_prefix.Octave = 0;
	Show_note_list( nlst );
}

notelist_t Note_class::Gen_notelist( noteline_prefix_t prefix, string str )
{
	notelist.clear(	);
	compiler( prefix, str );

	return notelist;
}

bool Note_class::Verify_noteline( noteline_prefix_t prefix, string str )
{
	auto brackets_aligned = [str]()
	{
		int c = 0;
		char p = 0;
		for ( char ch : str )
		{
			switch (ch )
			{
				case NOTE::BRA 	: {c++; break;}
				case NOTE::KET	: {c--; break;}
				default		: break;
			} // switch
			if( ( c == 0 ) and ( p == NOTE::BRA ) )
				return false; // empty bracket
			p = ch;
			if( c < 0 )return false; // more CLOSE than OPEN
		} //for
		return ( c == 0 );
	}; // lambda

	// pre check
	noteline_sec 		= 0;
	if ( sds )
		sds->Noteline_sec = noteline_sec;

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

	min_duration 			= 1000 / prefix.nps;
	rest_note.duration		= min_duration;
	if ( not compiler( prefix, str ) )
	{
		return false ;
	}
	if ( not prefix.variation )
		fill_note_list();
	Show_note_list( notelist );
	// post check
	uint noteline_msec 	= Calc_notelist_msec( notelist );
	int mod 			= noteline_msec % measure_duration ;
	if ( mod == 0 )
	{
		noteline_sec = noteline_msec / 1000;
		if ( sds )
			sds->Noteline_sec = noteline_sec;
		Comment(INFO, "Note line duration: " + to_string( noteline_sec ) + " [sec]");
		return true;
	}
	else
	{
		Comment( ERROR, "note line sec ", (int)noteline_msec, " is not aligned to " +
				to_string(prefix.nps) + " * " +
				to_string(min_duration) + " = " +
				to_string(measure_duration) + " [msec]" );
		return false;
	}

	return true;
}

void Note_class::Show_note( note_t note, bool debug )
{
	string longnote = note.longnote ? "L" : "|";
	stringstream strs{};
	for ( pitch_t pitch : note.chord )
	{
				strs <<
				setw(4) << right << dec << (int)pitch.octave <<
				setw(4) << right << pitch.alter <<
				setw(5) << right << dec << rint( pitch.freq ) <<"|";
	}
	if( debug or LogMask[DEBUG])
		Note_table.AddRow( 	note.number,
							note.str,
							(int)note.volume,
							(int)note.duration,
							longnote,
							strs.str());
}


note_t Note_class::Char2note( char& ch )
{
	pitch_t pitch 			= pitch_struct( Octave, ch, alter_value(ch) );

			note_buffer.chord.push_back( pitch );
			note_buffer.str.push_back(ch);
			note_buffer.duration    		= min_duration; // will be updated later
			note_buffer.glide[0].chord.step	= note_buffer.chord[0].step;
	if ( note_buffer.chord[0].step 	== NONOTE )
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

size_t Note_class::position_parser(  size_t pos )
{
	// parse a single noteline position and apply changes to the note_itr


	auto get_note_char = [ this ]( size_t p )
	{
		if ( p < noteline_len )
			return noteline[ p ];
		else
			return ' ';
	};

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
			Comment( ERROR, "Out of bounds occured at position ",pos, "of" );
			Comment( ERROR, noteline );
			return true;
		}
		else
			return false;
		};

	char note_char 	= noteline[pos];
	note_itr 		= notelist.end();
	note_itr--;
	switch (note_char )
	{
		case NOTE::LONGPLAY :
		{
			note_itr->longplay = true;
			note_itr->str.append(1, NOTE::LONGPLAY );
			break;
		}
		case NOTE::SLIDE : // allowed: >F or >|3F
		{
			if ( out_of_bounds( pos ) ) return noteline_len; // test note_itr
			note_itr->str.push_back( NOTE::SLIDE );
			note_itr->glide[0].glide = true;
			pos++; //
			if ( noteline[pos] == NOTE::NEWOCT ) // oct change case >|3F
			{
				note_itr->str.push_back( noteline[pos] );
				pos++;
				if ( out_of_bounds( pos ) ) return noteline_len; // test pos
				int oct = get_oct_value( noteline[pos] );
				if ( oct > 0 )
				{
					note_itr->str.push_back( noteline[pos] );
					pos++;
					if ( out_of_bounds( pos ) ) return noteline_len; // test pos
					char ch = noteline[pos];
					note_itr->str.push_back( ch );
					note_itr->glide[0].chord = pitch_struct( oct, ch, alter_value(ch) );
				}
			}
			else // simple case >F
			{
				char ch = noteline[pos];
				note_itr->glide[0].chord = pitch_struct( Octave, ch, alter_value(ch) );
				note_itr->str.push_back( ch );
			}
			break;
		}
		case NOTE::BRA : //(
		{
			set_duration();
			note_buffer = note_struct(); // clear note_buffer;
			note_buffer.str = "(";
			pos++;
			int nc_pos = 0;
			char ch = noteline[pos];
			while( ch != NOTE::KET )
			{
				if ( Note_Chars.Set.contains( ch ) )
				{
					Char2note( noteline[pos] ); // append note_buffer
					nc_pos++;
				}
				if ( ch == NOTE::INCOCT )
				{
					note_buffer.chord.back().octave += 1;
					note_buffer.str.push_back(ch);
				}
				if ( ch == NOTE::DECOCT )
				{
					note_buffer.chord.back().octave -= 1;
					note_buffer.str.push_back(ch);
				}
				pos++;
				ch = noteline[pos];
			}
			pos--; // close bracket pos
			break;
		}
		case NOTE::KET :
		{
			note_buffer.str.append( ")" );
			notelist.push_back( note_buffer );
			break;
		}
		case NOTE::INCDUR :
		{
			if ( out_of_bounds( pos ) ) return noteline_len;
			note_duration++; // increase tHe duration of a note
			note_itr->str.push_back( NOTE::INCDUR );
			break;
		}
		case NOTE::INCOCT :
		{
			if ( out_of_bounds( pos ) ) return pos++;
			note_itr->chord.back().octave += 1;
			note_itr->str.push_back( NOTE::INCOCT );
			break;
		}
		case NOTE::DECOCT :
		{
			if ( out_of_bounds( pos ) ) return pos++;
			note_itr->chord.back().octave -= 1;
			note_itr->str.push_back( NOTE::DECOCT );
			break;
		}
		case NOTE::NEWOCT : // nextchar may define a new octave
		{
			pos++;

			if ( noteline[pos] == NOTE::INCOCT ) { Octave += 1; break; }
			if ( noteline[pos] == NOTE::DECOCT ) { Octave -= 1; break; }

			int oct = get_oct_value( noteline[pos] );
			if ( oct < 0 )
				return parse_error;
			Octave = oct;
			return pos;
			break;
		}
		case NOTE::IGNORE :
		{
			break;
		}
		case NOTE::LINEBREAK :
		{
			note_itr = notelist.begin();
			break;
		}
		default : 	// this note-char is not a special character (<>-|.)
		{
			if (( Note_Chars.Set.contains( note_char ) ) or
				( note_char == NOTE::PAUSE ))
			{	// this note-char is a note that must be stored in the note-list
				set_duration(); // duration of the previou note
				note_buffer = note_struct( notenumber ); // clear note_buffer;
				notenumber++;
				if ( OctaveChars.Set.contains( get_note_char( pos + 1 ) ) )
				{	// the next notechar specifies the octave of the current note explicitely
					// e.g. A4
					Octave = char2int( noteline[ pos + 1 ] );
					pos++;
				}
				note_t note = Char2note( note_char );
				notelist.push_back( note );
				note_buffer = note_struct(); // prepare note_buffer for the next note
			}
			else
			{
				;
			}
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
			itr->chord[i].frqidx = GetFrqIndex( itr->chord[i] );
		}
		if ( itr->glide[0].glide )
		{
			itr->glide[0].chord.freq = CalcFreq( oct_base_freq, itr->glide[0].chord );
			itr->glide[0].chord.frqidx = GetFrqIndex( itr->glide[0].chord );
		}
		else
		{
			itr->glide[0].chord.freq = itr->chord[0].freq;
			itr->glide[0].chord.frqidx = itr->chord[0].frqidx;
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
			if ( note_buffer.glide[0].glide )
			{
				float dfreq = note_buffer.glide[0].chord.freq - note_buffer.chord[0].freq;
				note_buffer.glide[0].chord.freq = note_buffer.chord[0].freq + dfreq * (note_buffer.duration-rest)/(note_buffer.duration);
			}
			note_buffer.duration = note_buffer.duration - rest;
			notelist.insert( itr, note_buffer );
			//new
			itr->duration = rest;
			if ( note_buffer.glide[0].glide )
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

	uint noteline_msec = Calc_notelist_msec( notelist );
	noteline_sec = 0;

	int a = ( noteline_msec % measure_duration );
	int b = ( measure_duration - a ) % measure_duration;
	int mod = ( b ) / min_duration;
	Info( "filling up notelist by ", mod*min_duration, " msec" );

	note_itr_t itr = notelist.end();
	if( itr != notelist.begin() )
	{
		Info( "filling up notelist by ", mod*min_duration, " msec" );
		itr--;
		itr->duration += ( mod*min_duration) ;
		for (int n = 0; n < mod; ++n)
			itr->str.push_back('-');
	}
	return;

	if ( mod > 0 )
	{
//		cout << mod ;
		for (int n = 0; n < mod; ++n)
		{
//			coutf << "." << endl;
			notelist.push_back( rest_note );
			noteline.append(".");
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
	noteline_len = str.length();

	if ( noteline_len  == 0 )
		return false;

	noteline 	  		= str;
	notenumber			= 0;

	string prefix_str 	= convention_names[ prefix.convention ];
	String Note_Chars 	{ convention_notes[ prefix.convention ] };
	Octave 				= prefix.Octave;
	min_duration 		= 1000 / prefix.nps;

	Comment(INFO, "Instrument : " + Instrument_name );
	Comment(INFO, "Notes name : " + notefile_name );
	Show_noteline_prefix( prefix );
	Comment(INFO, "Note line  : " + noteline );
	Comment(INFO, "Rhythm line: " + volumeline );


	parse_error = noteline_len + 1;
	notelist.clear();
	set_volume_vector( volumeline );

	note_buffer			= note_struct();
	note_duration		= 0;

	size_t char_pos 	= 0;
	while( char_pos < noteline_len )
	{
		char_pos = position_parser( char_pos );
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

	return true;
}



