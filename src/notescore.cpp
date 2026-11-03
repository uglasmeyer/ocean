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
#include <Default.h>
#include <fstream>

/**************************************************
 * NotesCore_class
 * manage notes from different sources
 * - GUI file dialog
 * - Composer
 * - musicxml files
 * - note files
 * - generated notes (variation)
 * - keyboard notes
 * all notes must pass the verify function
 * the verify function has two interfaces
 * verify() to sds acts on notes data ( files, variation, keyboard )
 * -> note files with feedback to sds depending on the variation flag
 * verify( sds ) acts on received data from sds ( GUI, composer )
 * sds -> internal data
 *
*************************************************/

NotesCore_class::NotesCore_class( Dataworld_class* data, Musicxml_class* xml ) 	// File_dialog, variation
	: Logfacility_class		( "NotesCore_class" )
	, Interface_base		( data )
	, Note_table 			()
	, notefiles				( { fs->notesdir, fs->autodir }, fs->nte_type )
	, Noteline				( &sds_p->Noteline, SDSLONG_SIZE )
	, Rythmline				( &sds_p->Rythmline, SDSLONG_SIZE )
	, Noteline_prefix		( &sds_p->Noteline_prefix )

{
	this->Musicxml_p		= xml;
	this->notenumber		= 0;
	this->note_duration 	= 0;
	this->noteline_duration	= 0;
	this->vcounter			= 0;
	this->parse_error		= 0;
	this->timestamp 		= 0;
	this->scoretime 		= 0;
	this->noteline_sec 		= 0;
	this->Rythmline.set		( "" );
	this->Noteline_prefix.data= nlp_data();
	this->volume_vec_len 	= 1;
	this->Instrument_name.assign( sds_p->Instrument );
	this->Octave			= Noteline_prefix.data.Octave; // 55
	this->notefiles_dir		= "";
	this->notefile			= "";

	this->Name 				= default_name;
	Write_template_text 	( fs->default_nte_template_file	, default_nte_text, KEEPFILE );
	Write_template_text		( fs->testnte_file				, notes_test_text, OVERWRITE );
	SetDir					( sds_master->Session );
	select					( sds_p->Notes );

	volume_vec.clear		();
	init_note_table			();
};
NotesCore_class::~NotesCore_class()
{
	DESTRUCTOR( className );
};


void NotesCore_class::Set_notes_per_second( int nps )
{
	Noteline_prefix.data.nps = Noteline_prefix.check_bps( nps );
	Noteline_prefix.Save();
	verify_noteline();
	if( nps == Noteline_prefix.data.nps ) return;
	Comment( WARN, nps, "not supported" );
	Comment( INFO, "using", (int)Noteline_prefix.data.nps, "instead" );
}

void NotesCore_class::Set_Octave_shift( uint8_t octave )
{
	Noteline_prefix.data.octave_shift = octave - oct_reference;
}

//bool NotesCore_class::Set_noteline_prefix( nlp_data_t nlp )
//{
//	// take nlp from sds modify existing Noteline
//	// the sds noteline prefix values are as long temporary as not Saved
//	// but the sds noteline prefixes are used to gen notes instead
//	// the internal Noteline prefix is a reference to identify changes
//
//
//	Set_Octave_shift( nlp.Octave );
//	return Verify_noteline( Noteline_prefix.data, Noteline.get() ) ;
//}

string NotesCore_class::Read( string name )
{
	select( name );

	String 			Line{""};
	string_vec_t 	Line_arr;
	fstream 		File;

	File.open( notefile, fstream::in );

	string nl		= "";
	string rl		= "";
	getline( File, Line.Str );

	do
	{
			Line.replace_comment();
			string_vec_t arr = Line.to_array( '=' );

			switch ( arr[0][0] )
			{
				case 'V' :
				{
					rl.append( arr[1] );
					break;
				}
				case 'N' :
				{
					nl.append( arr[1] );
					break;
				}
				case 'P' :
					Noteline_prefix.From_string( arr[1] );
					break;
				case '+' :
				{
					break;
				}
				default :
				{
					nl.append( arr[0] );
					break;
				}
			} // end switch
	} while( getline( File, Line.Str));

	File.close();

	Noteline.set( nl );
	Set_Octave_shift( Noteline_prefix.data.Octave );
	set_volume_vector( rl );

	return ( Verify_noteline( note_ns::FILE ) ) ? nl : "";
}

void NotesCore_class::Save( string name )
{
	Rythmline.read();
	Save( name, Noteline_prefix.Read(), Noteline.read() );
}
void NotesCore_class::Save( string name, nlp_data nlp, string nl )
{
	if( not	Set_new_name( name ) )
		return;

	fstream File( notefile, fstream::out );
	Comment	(DEBUG,Error_text( errno ));

	if ( not File.is_open())
	{
		Comment( ERROR, "Input file: " + notefile + " does not exist" );
		return ;
	}
	Comment( INFO, "saving notes:", nl );
	Comment( INFO, "to file: ", notefile );

	// make octave_shift temporary
	nlp.octave_shift 	= 0;
	nlp.Octave			= oct_reference;
	string noteline_prefix_str = Noteline_prefix.To_string( nlp );
	File <<
		"#See the documentation file " + rn_userdoc + ".pdf\n" <<
		"#about the specification of Prefix, Notes and Volume \n" <<
		"P=" << noteline_prefix_str << "\n" <<
		"N=" << nl << "\n" <<
		"V=" << Rythmline.get()  <<	endl;
	File.close();
}

void NotesCore_class::Set_rhythm_line( string rhythm )
{
	Rythmline.save( rhythm );
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
		this->Rythmline.set( to_string( notes_default_volume / 10 ) );
	else
		this->Rythmline.set( volline );

	volume_vec.clear();
	for ( char ch : this->Rythmline.get() )
	{
		if ( in_range( rhythm_vol_range, ch ) )
			volume_vec.push_back( char2int( ch ) * 10);
		else
			Info( "Rhythm line value " , ch, "not valid" );
	}

	volume_vec_len = volume_vec.size();
	if ( volume_vec_len == 0 ) // no valid chars
	{
		this->Rythmline.set( to_string( notes_default_volume / 10 ) );
		volume_vec.push_back( notes_default_volume );
		volume_vec_len = volume_vec.size();
	}
	assert( volume_vec.size() > 0 );
}

string NotesCore_class::get_name()
{
	return Name;
}
void NotesCore_class::select( string name )
{
	notefile= notefiles.Select( name, Name );
	Name	= name;
	Comment( INFO, "Notes Name: ", Name );
}

void NotesCore_class::SetDir( string subdir )
{
	notefiles_dir			= fs->SubDir( fs->notesdir, subdir );
	notefiles.UpdatePath	( { notefiles_dir, fs->autodir, fs->notesdir } );
	select					( Name );
	Musicxml_p->SetDir			( subdir );
}

bool NotesCore_class::Set_new_name( string name )
{
	if ( name.length() == 0 )
		return false;

	notefiles.Create( name );
	select( name );

	return true;
}

void NotesCore_class::align_measure()
{
	notelist_t nl 			{ Gen_notelist() };
	uint nl_duration_msec	= calc_notelist_msec( nl );
	uint 	mod				= ( nl_duration_msec % measure_duration ) / min_duration;
	for ( uint n = 0; n < mod; n++ )
	{
		Noteline.set( Noteline.get() + "." ) ;
	}

}
uint NotesCore_class::calc_notelist_msec( notelist_t notelist )
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

	Noteline_prefix.data = nlp_default;
	Noteline_prefix.data.Octave = 0;
	Show_note_list( nlst );
}

notelist_t NotesCore_class::Gen_notelist()
{
	notelist.clear	();
	compile		();

	return notelist;
}

bool NotesCore_class::Verify_noteline( nlp_data_t nlp, string str )
{
	Noteline_prefix.data	= nlp;
	Noteline.set( str )			;
	return verify_noteline	();
}

bool NotesCore_class::Verify_noteline( note_ns::ORIGIN_e origin )
{
	switch ( origin) 	{
		case note_ns::VARIATION 	:
		{
			Noteline_prefix.data.origin = origin;
			break;
		}
		case note_ns::FILE			:
		case note_ns::KEYBOARD		:
		{
			Noteline_prefix.data.origin = origin;
			Noteline_prefix.Save();
			Noteline.save();
			Rythmline.save();
			break;
		}
		case note_ns::COMPOSE		:
		case note_ns::GUI			:
		{
			Noteline_prefix.Read();
			Noteline.read();
			Rythmline.read();
			set_volume_vector( Rythmline.get() );
			Set_Octave_shift( Noteline_prefix.data.Octave );
			break;
		}
		default 				: { break; }
	} // switch  origin
	return verify_noteline();
}

bool NotesCore_class::verify_noteline()
{
	auto brackets_aligned = [ this ]()
	{
		int		c = 0;
		char	p = 0;
		for ( char ch : Noteline.get() )
		{
			switch (ch )
			{
				case note_ns::BRA 	: {c++; break;}
				case note_ns::KET	: {c--; break;}
				default		: break;
			} // switch
			if( ( c == 0 ) and ( p == note_ns::BRA ) )
				return false; // empty bracket
			p = ch;
			if( c < 0 )
				return false; // more CLOSE than OPEN
		} //for
		return ( c == 0 );
	}; // lambda

	// pre check
	noteline_sec 		= 0;
	if ( sds_p )
		sds_p->Noteline_sec = noteline_sec;

	if ( Noteline.len == 0 )
	{
		Comment( WARN, "Empty note line ");
		return false;
	}
	if ( not brackets_aligned() )
	{
		Comment( ERROR, "brackets () are not aligned" );
		return false;
	}

	min_duration 			= 1000 / Noteline_prefix.data.nps;
	rest_note.duration		= min_duration;
	if ( not compile() )
	{
		Comment( ERROR, "Notes not verified" );
		return false ;
	}
	if ( Noteline_prefix.data.origin != note_ns::VARIATION )
		fill_note_list();

	Show_note_list( notelist );

	// post check
	uint noteline_msec 	= calc_notelist_msec( notelist );
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
				to_string( Noteline_prefix.data.nps) + " * " +
				to_string(min_duration) + " = " +
				to_string(measure_duration) + " [msec]" );
		return false;
	}

	return true;
}

void NotesCore_class::Show_note( const note_t& note, bool debug )
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
	oct = check_range( octave_range, oct );
	Noteline_prefix.data.Octave = oct;
	Octave = oct;

}

size_t NotesCore_class::position_parser(  size_t pos )
{
	// parse a single noteline position and apply changes to the note_itr


	auto get_note_char = [ this ]( size_t p )
	{
		if ( p < Noteline.len )
			return Noteline.get()[ p ];
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
		if (( note_itr == notelist.end() ) or ( pos > Noteline.len - 1 ))
		{
			Comment( ERROR, "Out of bounds occured at position ",pos, "of" );
			Comment( ERROR, Noteline.get());
			return true;
		}
		else
			return false;
		};

	char note_char 	= Noteline.get()[pos];
	note_itr 		= notelist.end();
	note_itr--;
	switch (note_char )
	{
		case note_ns::LONGPLAY :
		{
			note_itr->longplay = true;
			note_itr->str.append(1, note_ns::LONGPLAY );
			break;
		}
		case note_ns::SLIDE : // allowed: >F or >|3F
		{
			if ( out_of_bounds( pos ) ) return Noteline.len; // test note_itr
			note_itr->str.push_back( note_ns::SLIDE );
			note_itr->glide[0].glide = true;
			pos++; //
			if ( Noteline.get()[pos] == note_ns::NEWOCT ) // oct change case >|3F
			{
				note_itr->str.push_back( Noteline.get()[pos] );
				pos++;
				if ( out_of_bounds( pos ) ) return Noteline.len; // test pos
				int oct = get_oct_value( Noteline.get()[pos] );
				if ( oct > 0 )
				{
					note_itr->str.push_back( Noteline.get()[pos] );
					pos++;
					if ( out_of_bounds( pos ) ) return Noteline.len; // test pos
					char ch = Noteline.get()[pos];
					note_itr->str.push_back( ch );
					note_itr->glide[0].chord = pitch_struct( oct, ch, alter_value(ch) );
				}
			}
			else // simple case >F
			{
				char ch = Noteline.get()[pos];
				note_itr->glide[0].chord = pitch_struct( Octave, ch, alter_value(ch) );
				note_itr->str.push_back( ch );
			}
			break;
		}
		case note_ns::BRA : //(
		{
			set_duration();
			note_buffer = note_struct( notenumber++ ); // clear note_buffer;
			note_buffer.str = "(";
			pos++;
			char ch = Noteline.get()[pos];
			while( ch != note_ns::KET )
			{
				if ( Note_Chars.Set.contains( ch ) )
				{
					Char2note( Noteline.get()[pos] ); // append note_buffer
				}
				if ( ch == note_ns::INCOCT )
				{
					note_buffer.chord.back().octave += 1;
					note_buffer.str.push_back(ch);
				}
				if ( ch == note_ns::DECOCT )
				{
					note_buffer.chord.back().octave -= 1;
					note_buffer.str.push_back(ch);
				}
				pos++;
				ch = Noteline.get()[pos];
			}
			pos--; // close bracket pos
			break;
		}
		case note_ns::KET :
		{
			note_buffer.str.append( ")" );
			notelist.push_back( note_buffer );
			break;
		}
		case note_ns::INCDUR :
		{
			if ( out_of_bounds( pos ) ) return Noteline.len;
			note_duration++; // increase tHe duration of a note
			note_itr->str.push_back( note_ns::INCDUR );
			break;
		}
		case note_ns::INCOCT :
		{
			if ( out_of_bounds( pos ) ) return pos++;
			note_itr->chord.back().octave += 1;
			note_itr->str.push_back( note_ns::INCOCT );
			break;
		}
		case note_ns::DECOCT :
		{
			if ( out_of_bounds( pos ) ) return pos++;
			note_itr->chord.back().octave -= 1;
			note_itr->str.push_back( note_ns::DECOCT );
			break;
		}
		case note_ns::NEWOCT : // nextchar may define a new octave
		{
			pos++;

			if ( Noteline.get()[pos] == note_ns::INCOCT ) { Octave += 1; break; }
			if ( Noteline.get()[pos] == note_ns::DECOCT ) { Octave -= 1; break; }

			int oct = get_oct_value( Noteline.get()[pos] );
			if ( oct < 0 )
				return parse_error;
			Octave = oct;
			return pos;
			break;
		}
		case note_ns::IGNORE :
		{
			break;
		}
		case note_ns::LINEBREAK :
		{
			note_itr = notelist.begin();
			break;
		}
		default : 	// this note-char is not a special character (<>-|.)
		{
			if (( Note_Chars.Set.contains( note_char ) ) or
				( note_char == note_ns::PAUSE ))
			{	// this note-char is a note that must be stored in the note-list
				set_duration(); // duration of the previou note
				note_buffer = note_struct( notenumber ); // clear note_buffer;
				if ( OctaveChars.Set.contains( get_note_char( pos + 1 ) ) )
				{	// the next notechar specifies the octave of the current note explicitely
					// e.g. A4
					Octave = char2int( Noteline.get()[ pos + 1 ] );
					pos++;
				}
				note_t note = Char2note( note_char );
				notelist.push_back( note );

				// prepare note_buffer for the next note
				note_buffer = note_struct( notenumber++ );
			}
			else
			{
				;
			}
			break;
		}
	} // switch
	if ( ( pos == Noteline.len - 1 )  )
		set_duration();
	return pos;
}

void NotesCore_class::change_alphabet_notes()
{
	// change alphabet notes only
	if ( Noteline_prefix.data.convention != ALPHABET ) return;


	for( note_struct& note : notelist )
	{
		for( uint sharp = 0; sharp < Noteline_prefix.data.sharp; sharp++ )
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
		for( uint flat = 0; flat < Noteline_prefix.data.flat; flat++ )
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

	uint noteline_msec = calc_notelist_msec( notelist );
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
		for (int n = 0; n < mod; ++n)
		{
			notelist.push_back( rest_note );
			Noteline.append(".");
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

bool NotesCore_class::compile ()
{
	if ( Noteline.len  == 0 )
		return false;

	notenumber			= 0;

	string prefix_str 	= convention_names[ Noteline_prefix.data.convention ];
	String Note_Chars 	{ convention_notes[ Noteline_prefix.data.convention ] };
	Octave 				= Noteline_prefix.data.Octave;
	min_duration 		= 1000 / Noteline_prefix.data.nps;

	Comment(INFO, "Instrument : " + Instrument_name );
	Comment(INFO, "Notes name : " + Name );
	Noteline_prefix.Show_nlp();
	Comment(INFO, "Note line  : " + Noteline.get());
	Comment(INFO, "Rhythm line: " + Rythmline.get() );


	parse_error 		= Noteline.len + 1;
	notelist.clear		();
	Set_rhythm_line( Rythmline.get() );

	note_buffer			= note_struct( 0 );
	note_duration		= 0;

	size_t char_pos 	= 0;
	while( char_pos < Noteline.len )
	{
		char_pos = position_parser( char_pos );
		char_pos++;
	};

	if ( char_pos >= parse_error )
		return false;

	change_alphabet_notes	();
	assign_freq				();
	add_volume				( note_itr );

	if ( Noteline_prefix.data.origin != note_ns::VARIATION )
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
	Noteline.set( "D2--" );
	notelist_t nl { Gen_notelist() };
	Show_note_list( nl );
	align_measure();
	uint16_t msec = calc_notelist_msec( nl );
	Assert_equal( msec, measure_duration, "Calc_noteline_msec" );


	Noteline.set( "D3----" );
	nl =  Gen_notelist() ;
	msec = calc_notelist_msec( nl );
	ASSERTION( msec == 2000, "Calc_noteline_msec", msec, 2000L );
	coutf << Noteline.get() << endl;
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
	bool vn = Verify_noteline(Noteline_prefix.data, "ABCD-");
	assert( noteline_sec == 2 );
	assert( vn );

	Comment( TEST, "set volume");
	Set_rhythm_line("844448");
	Verify_noteline(Noteline_prefix.data, "|2(ACE)--|3G-EC-A--.B");
	int i = 0;
	volume_vec={80,40,40,40,40,0,80,0,0,0};
	for( note_t note : notelist )
	{
		ASSERTION( note.volume == volume_vec[i],"i="+to_string(i), note.volume, volume_vec[i]);
		i++;
	}

	Comment ( TEST, "long gliding");
	Set_rhythm_line("5");
	Verify_noteline(Noteline_prefix.data, "CA----->G...BCD");

	set_volume_vector( "yyyyy01289xxxxx" );
	assert( volume_vec_len == 4 );
	assert( volume_vec[0] == 10 );
	assert( volume_vec[3] == 90 );

	set_volume_vector("");
	assert( volume_vec_len == 1);
	assert( volume_vec[0] == notes_default_volume );

	Rythmline.set( "9797" );
	Set_prefix_octave( 0 );
	Noteline.set( "|3F--G(AC)-(DC)-" );
	Noteline_prefix.From_string( "4,0,4,0,0" );
	Save(".testcase", Noteline_prefix.data, Noteline.get() );

	Rythmline.set( "yyyyy" );
	Noteline.set( "xxxxx" );
	Read(".testcase" );
	Assert_equal( Noteline_prefix.data.Octave, oct_reference ) ;
	assert( noteline_sec == 2 );
	Assert_equal( Noteline.get(), string( "|3F--G(AC)-(DC)-" ) );
	Assert_equal( Rythmline.get(), string( "9797" ) );

	Verify_noteline( Noteline_prefix.data,  "|3F--|2G(AC)-|3(DC)-");
	ASSERTION( Octave == 3, "Octave", (int) Octave, 3 );
	note_itr = notelist.begin();
	//	for ( note_struct_t note : notelist )
	//		cout << dec<<(int)note.octave << endl;
	advance(note_itr, 1);
	ASSERTION( note_itr->chord[0].octave == 2, "chord octave", note_itr->chord[0].octave, 2 );
	advance(note_itr, 1 );
	Assert_equal( note_itr->str, string( "(AC)-" ), "chord name" );
	Assert_equal( (int)note_itr->number, 2 );
	advance(note_itr, 1 );
	Assert_equal( (int)note_itr->number, 3 );

	set_volume_vector("");
	assert( volume_vec[0] == notes_default_volume );

	note_itr = notelist.begin();
	advance(note_itr, 2);
	cout << note_itr->duration ;
	assert( note_itr->duration == 2*min_duration );


	Noteline.set( ".---" );
	Verify_noteline( Noteline_prefix.data, Noteline.get());
	note_itr = notelist.begin();
	assert( note_itr->volume == 0 );
	ASSERTION( 	note_itr->duration == measure_duration, "duration",
				note_itr->duration, measure_duration );

	Comment( TEST, "garbage test" );
	Noteline.set( ",xyz" );
	Set_notes_per_second( 5 );
	Verify_noteline( Noteline_prefix.data, Noteline.get());
	Assert_equal( (int)Noteline_prefix.data.nps, 5 );
	assert( min_duration == 200 );

	Noteline.set( "-xyz" );
	Set_notes_per_second( 1 );
	Noteline_prefix.Show_nlp();
	Verify_noteline( Noteline_prefix.data, Noteline.get());
	Assert_equal( (int)min_duration, 1000 );


	Noteline.set( "'xyz" );
	Set_notes_per_second( 4 ); // default
	Verify_noteline( Noteline_prefix.data, Noteline.get());
	assert( min_duration == 250 );

	Verify_noteline( Noteline_prefix.data, "|4(AB)--->|5A" );
	note_itr = notelist.begin();
	note_t note = *note_itr;
	ASSERTION( note.glide[0].chord.step == 9, "", note.glide[0].chord.step, 9 );
	ASSERTION( note.glide[0].chord.alter == 0,"", note.glide[0].chord.alter, 0 );
	Show_note( note );
	ASSERTION ( fcomp(	CalcFreq( oct_base_freq, note.glide[0].chord ), 880, 2E-4 ) , "CalcFreq",
						CalcFreq( oct_base_freq, note.glide[0].chord ), 880 );


	Verify_noteline( Noteline_prefix.data, "|2A'AA,A,AAA");
	note_itr = notelist.begin();
	for( int freq : { 220, 110, 55, 55, 110, 110, 110 } )
	{
		Show_note( *note_itr );
		cout << freq << " = " << note_itr->chord[0].freq  << endl;
		assert( abs( note_itr->chord[0].freq  - freq ) < 1);
		note_itr++;
	}

	Noteline_prefix.data.Octave = 4;
	ASSERTION( Verify_noteline( Noteline_prefix.data, "|,A(A,a)(A'a)(Aa,)(a,)A'.."),
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

	Noteline.set( "A2A3-A4--A5---" );
	Gen_notelist();

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


	Noteline_prefix.data = nlp_default;
	Noteline_prefix.data.nps = 4;
	Noteline.set( "A2B3-C4D3-.D3A4..A2-B3-C4-D3-A-B-" );
	notelist_t nl = Gen_notelist();
	Show_note_list( nl );
	Musicxml_p->SetDir( "" );
	Musicxml_p->Notelist2xmlFile( fs->test_name , nl );
	musicxml = Musicxml_p->XmlFile2notelist(  fs->test_name  );
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

/**************************************************
 * noteline_prefix_struct
 *************************************************/
noteline_prefix_struct::noteline_prefix_struct( nlp_data_t* nlp )
{
	Table.AddColumn		( "Prefix", 20 );
	Table.AddColumn		( "Value", 15 );
	Table.PrintHeader	();
	this->data_p		= nlp;
	this->data 			= nlp_data();
	selftest			();
};

void noteline_prefix_struct::Nlp_data_p( nlp_data_t* p )
{
	data_p = p;
}
nlp_data_t	noteline_prefix_struct::Read()
{
	data = *data_p;
	return data;
}
void noteline_prefix_struct::Save()
{
	*data_p = data;
}
void noteline_prefix_struct::Show_nlp()
{
	string conv_name	= convention_names[data.convention];
	string convention 	= to_string( data.convention) + " (" + conv_name + ")";
	Table.AddRow( "Convention"	,  convention );
	Table.AddRow( "Octave"		, (int) data.Octave );
	Table.AddRow( "Notes per sec",(int) data.nps );
	Table.AddRow( "Flats"		, (int) data.flat );
	Table.AddRow( "Sharps"		, (int) data.sharp );
	Table.AddRow( "Chord delay"	, (int) data.chord_delay );
}

string noteline_prefix_struct::To_string( nlp_data_t nlp )
{
	stringstream strs{""};
	strs 	<< (int) nlp.Octave 		<<","
			<< (int) nlp.convention		<<","
			<< (int) nlp.nps 			<<","
			<< (int) nlp.flat			<<","
			<< (int) nlp.sharp 			<<","
			<< (int) nlp.chord_delay;
	return strs.str();
}

void noteline_prefix_struct::From_string( const string& str )
{

	String 				S 	{ str };
	string_vec_t 		arr = S.to_unique_array(',');

	if ( arr.size() < 6 )
	{
		Comment( ERROR, "Cannot assign noteline_prefix of length < 5" );
		Comment( WARN, "using default" );
		data = nlp_data();
		return ;
	}

	data.Octave		= check_range( octave_range		, 			S.to_int( arr[0] ) );
	data.convention	= check_range( convention_range	,			S.to_int( arr[1] ) );
	data.nps 		= check_bps				(					S.to_int( arr[2] ) );
	data.flat		= check_range( accidental_range	, (uint8_t)	S.to_int( arr[3] ) );
	data.sharp		= check_range( accidental_range	, (uint8_t)	S.to_int( arr[4] ) );
	data.chord_delay=( arr.size() > 5 ) ?
					  check_range( percent_range	, (uint8_t) S.to_int( arr[5] ) ) : 0 ;

}


void noteline_prefix_struct::selftest()
{
	string nlp_str = "2,1,4,0,0,0,0";
	From_string( nlp_str );

	Show_nlp();

	nlp_str		= "1 ";
	From_string	( nlp_str );
	nlp_str		= "8, -1, 11, 7, 8, 110, 0";
	From_string	( nlp_str );
	Assert_equal( (int)data.Octave		, (int)max_octave, 	"Octave" );
	Assert_equal( (int)data.convention	, 0, 	"Convention" );
	Assert_equal( (int)data.nps			, 16, 	"notes per sec" );
	Assert_equal( (int)data.flat		, 7,	"flats" );
	Assert_equal( (int)data.sharp		, 7, 	"sharps" );
	Assert_equal( (int)data.sharp		, 7, 	"sharps" );
	Assert_equal( (int)data.chord_delay	, 100, 	"delay" );

}


