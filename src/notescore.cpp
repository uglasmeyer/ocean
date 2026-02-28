/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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
#include <data/Config.h>
#include <../Setup/ReleaseNotes.h>
#include <fstream>

NotesCore_class::NotesCore_class( Dataworld_class* data ) 	// File_dialog, variation
	: Logfacility_class		( "NotesCore_class" )
	, Interface_base		( data )
	, Note_table 			()
	, Musicxml				( data->Cfg_p->fs )

{
	this->notefile_name 	= "";
	this->noteline_len 		= 0;
	this->notenumber		= 0;
	this->note_duration 	= 0; 	// consider the length of one note by counting "-"-chars
	this->noteline_duration	= 0;
	this->vcounter			= 0;
	this->parse_error		= 0;
	this->timestamp 		= 0;
	this->scoretime 		= 0;
	this->notefile			= "";
	this->noteline			= "";
	this->noteline_sec 		= 0;
	this->volumeline 		= "";
	this->volume_vec_len 	= 1;
	this->Instrument_name.assign( sds_p->Instrument );
	this->Octave			= nlp_default.Octave; // 55

	volume_vec.clear		();
	init_note_table			();
	Test_NotesCore			();
};
NotesCore_class::~NotesCore_class()
{
	DESTRUCTOR( className );
};

string NotesCore_class::Get_rhythm_line()
{
	return volumeline;
}

bool NotesCore_class::Set_notes_per_second( int notes_per_second )
{
	if ( NpsChars.Bps_set.contains( notes_per_second ) )
	{
		this->Noteline_prefix.nps 	= notes_per_second;
		return true;
	}
	else
	{
		Comment( ERROR, "set notes per second to", notes_per_second ," rejected ");
		Comment( INFO, "not in list: ", show_items( NpsChars.Bps_vec ) );
		this->Noteline_prefix.nps 	= nlp_default.nps ;
		return false;
	}
}

string NotesCore_class::Get_note_line ()
{
	return noteline;
}

void NotesCore_class::Set_nlp( noteline_prefix_t nlp )
{
	if( sds_p )
		sds_p->noteline_prefix = nlp;
}

void NotesCore_class::Set_noteline_prefix( noteline_prefix_t nlp )
{
	this->Noteline_prefix = nlp;
	Show_noteline_prefix( nlp );
	Verify_noteline( nlp, noteline );
}

string NotesCore_class::Read( string filename )
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
					Set_nlp( Noteline_prefix );
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

	Noteline_prefix.octave_shift 	= sds_p->noteline_prefix.octave_shift;
	Noteline_prefix.variation 		= 0;
	Set_rhythm_line					( volumeline );

	if ( Verify_noteline( Noteline_prefix, noteline ) )
	{
		return noteline;
	}
	else
		return "";
}

void NotesCore_class::Save( string name, noteline_prefix_t prefix, string nl_str )
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

void NotesCore_class::Set_rhythm_line( string rhythm )
{
	set_volume_vector( rhythm );
}

void NotesCore_class::init_note_table()
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
void NotesCore_class::show_noteline_duration( const uint16_t& msec,
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

void NotesCore_class::set_volume_vector( string volline )
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

string NotesCore_class::get_name()
{
	return notefile_name;
}

bool NotesCore_class::set_file_name( string str )
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

void NotesCore_class::Align_measure( noteline_prefix_t prefix, string& noteline )
{
	notelist_t nl { Gen_notelist( prefix, noteline ) };
	uint nl_duration_msec = Calc_notelist_msec( nl );
	uint mod = ( nl_duration_msec % measure_duration ) / min_duration;
	for ( uint n = 0; n < mod; n++ )
	{
		noteline.append( "." );
	}

}
uint NotesCore_class::Calc_notelist_msec( notelist_t notelist )
	{
		uint duration = 0;
		for( note_t note : notelist )
		{
			duration += note.duration;
		}
		return duration;
	};


void NotesCore_class::Set_xmlnotelist( const notelist_t& nlst )
{
	notelist = nlst;
	sds_p->Noteline_sec = musicxml.scoreduration / 1000;

	Noteline_prefix = nlp_default;
	Noteline_prefix.Octave = 0;
	Show_note_list( nlst );
}

notelist_t NotesCore_class::Gen_notelist( noteline_prefix_t prefix, string str )
{
	notelist.clear(	);
	compiler( prefix, str );

	return notelist;
}

bool NotesCore_class::Verify_noteline( noteline_prefix_t prefix, string str )
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
	if ( sds_p )
		sds_p->Noteline_sec = noteline_sec;

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
		if ( sds_p )
			sds_p->Noteline_sec = noteline_sec;
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

void NotesCore_class::Show_note( note_t note, bool debug )
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


note_t NotesCore_class::Char2note( char& ch )
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

void NotesCore_class::Set_prefix_octave( int oct )
{
	char oct_ch = int2char( oct );
	if ( not OctaveChars.Set.contains( oct_ch ) ) return; // (nil)
	Noteline_prefix.Octave = oct;
	Octave = oct;

}

size_t NotesCore_class::position_parser(  size_t pos )
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

void NotesCore_class::change_alphabet_notes( noteline_prefix_t prefix )
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

void NotesCore_class::assign_freq()
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
void NotesCore_class::split_long_notes()
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

void NotesCore_class::fill_note_list()
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

void NotesCore_class::add_volume( note_itr_t itr )
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

bool NotesCore_class::compiler ( noteline_prefix_t prefix, string str )
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

void NotesCore_class::Test_NotesCore()
{
	TEST_START( className );

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
	note_itr = notelist.begin();
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
	note_itr = notelist.begin();
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
	note_itr = notelist.begin();
	assert( note_itr->volume == 0 );
	ASSERTION( 	note_itr->duration == measure_duration, "duration",
				note_itr->duration, measure_duration );

	Comment( TEST, "garbage test" );
	noteline=",xyz";
	Set_notes_per_second( 5 );
	Verify_noteline( Noteline_prefix, noteline );
	Assert_equal( (int)Noteline_prefix.nps, 5 );
	assert( min_duration == 200 );

	noteline="-xyz";
	Set_notes_per_second( 1 );
	Show_noteline_prefix( Noteline_prefix );
	Verify_noteline( Noteline_prefix, noteline );
	Assert_equal( (int)min_duration, 1000 );


	noteline="'xyz";
	Set_notes_per_second( 4 ); // default
	Verify_noteline( Noteline_prefix, noteline );
	assert( min_duration == 250 );

	Verify_noteline( Noteline_prefix, "|4(AB)--->|5A" );
	note_itr = notelist.begin();
	note_t note = *note_itr;
	ASSERTION( note.glide[0].chord.step == 9, "", note.glide[0].chord.step, 9 );
	ASSERTION( note.glide[0].chord.alter == 0,"", note.glide[0].chord.alter, 0 );
	Show_note( note );
	ASSERTION ( fcomp(	CalcFreq( oct_base_freq, note.glide[0].chord ), 880, 2E-4 ) , "CalcFreq",
						CalcFreq( oct_base_freq, note.glide[0].chord ), 880 );


	Verify_noteline( Noteline_prefix, "|2A'AA,A,AAA");
	note_itr = notelist.begin();
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

	pitch_struct A(A3);
	pitch_struct B = A;
	ASSERTION( A.frqidx == B.frqidx, "pitch_struct=", (int)B.frqidx, (int)A3);

	Gen_notelist( nlp_default, "A2A3-A4--A5---");

	pitch_t pitch = pitch_struct( 10,'B',0 );
	Assert_equal( (int)pitch.frqidx, (int)((max_octave)*oct_steps +11 + C0) );

//	assert (false);
	Comment( TEST, "Note_class test done ");

	TEST_END( className );

}

void NotesCore_class::Test_Musicxml()
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
	Assert_equal(	duration,  6000 );

	TEST_END( className );
}


