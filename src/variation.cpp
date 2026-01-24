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
 * Variation.cpp
 *
 *  Created on: Apr 30, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Variation.h>

// https://en.cppreference.com/w/cpp/language/operators

Variation_class::Variation_class( interface_t* sds, Config_class* cfg )
	: Logfacility_class{ "Variation_class" }
	, Note_class( sds, cfg->fs )
{
	Note_class::Instrument_name = "Variation";
	Nlp_variation.variation = 1;
	Nlp_variation.nps		= 2;
	selfTest();
};


void Variation_class::randomize_notes_octave( string str )
{

	size_t n = 0;

	while( n < Random_Notes.size() )
	{

		char ch = gen_random_note( str );//str[0];
		if ( ch == '\'' )
		{
			Random_Notes[n].chord[0].alter = 1;
			Random_Notes[n].str.push_back('\'');
		}
		if ( ch == ',' )
		{

			Random_Notes[n].chord[0].alter = -1;
			Random_Notes[n].str.push_back(',');
		}
		if ( LogMask[TEST] )
			Note_class::Show_note( this->Random_Notes[n] );
		n++;
	}


}
void Variation_class::Set_note_chars( uint prefix  )
{
	Note_class::Note_Chars = convention_notes[ prefix ];
	Note_class::Noteline_prefix.convention 	= prefix;
	Note_class::Noteline_prefix.flat 		= 0;
	Note_class::Noteline_prefix.sharp 		= 0;

}

void Variation_class::Set_note_chars( uint flats, uint sharps  )
{
	Note_class::Note_Chars = convention_notes[ 2 ];
	Note_class::Noteline_prefix.convention 	= 2;
	Note_class::Noteline_prefix.flat 		= flats;
	Note_class::Noteline_prefix.sharp 		= sharps;


}


Variation_class::noteword_t Variation_class::list2vector( notelist_t notes )
{
	noteword_t word {};
	for( note_t note : notes )
		word.push_back( note );
	return word;
}


void Variation_class::define_random_note_vector( string str )
{
	Note_class::Noteline_prefix = Nlp_variation;
	Random_Notes.clear();
	notelist_t nl = Gen_notelist( Noteline_prefix, str );
	Random_Notes = list2vector( nl );
	Note_class::Show_note_list(Random_Notes);
}


string Variation_class::input_filter( string input, set<char> valid)
{
	string result { "" };
	std::copy_if( input.begin(), input.end(),
							back_inserter( result ),
							[valid]( char ch ){ return valid.contains(ch);});
	return result;
}

void Variation_class::Define_fix( string notes )
{
	auto set_octave = [ this  ]( string notes )
		{
			if ( notes[0] == '|' )
				if ( Note_class::OctaveChars.Set.contains( notes[1] ) )
					Note_class::Octave = char2int(notes[1]);
		};
	if ( notes.length() < 4 )
	{
		Exception( "Constant_chars contains less than 4 characters" );
	}

	if ( notes[0] == '-' )
	{
		Exception( "Constant_chars must not start with specified character " );
	}
	String Valid { "r(),.-'| " + Note_class::OctaveChars.Str + Note_class::Note_Chars.Str };
	set_octave( notes );
	Constant_chars = input_filter( notes, Valid.Set);

}

void Variation_class::Define_variable( string notes )
{
	define_random_note_vector( notes );
}

void Variation_class::Define_rhythm( string rhythm )
{

	Set_rhythm_line( rhythm );
	Rhythm_chars = rhythm;
}

Variation_class::noteword_t Variation_class::gen_random_note_word()
{
	note_t 		note		{};
	noteword_t 	word		{};

	buffer_t 	duration 	= 0;

	while ( duration < measure_duration )
	{
		note = gen_random_note( Random_Notes );

		duration 		+=Note_class::min_duration;
		note.duration 	= Note_class::min_duration;
		char ch = gen_random_note( Random_Notes ).str[0];
		while ( ( ch == '-' ) and ( duration < measure_duration ))
		{
			duration 		+= Note_class::min_duration;
			note.duration 	+= Note_class::min_duration;
			note.str.push_back( '-' );
			ch 				= gen_random_note( Random_Notes ).str[0];
		}
		note.octave = Note_class::Octave;
		word.push_back( note );
		if ( LogMask[TEST] )
			Note_class::Show_note( note );
	}
	return word;

}

Variation_class::noteword_t Variation_class::str2words( string note_str )
{
	// take a notestring and separate it into one or more words of note structures
	noteword_t word{};
	if ( Note_class::Verify_noteline( Noteline_prefix, note_str ) )
	{
		for( note_t note : Note_class::notelist )
		{
			word.push_back( note );
		}
	}
	return word;
}
string Variation_class::insert_random(  )
{

	string note_str{};
	note_t note;

	for( char ch : Constant_chars )
	{
		if ( ch == 'r' ) // random
		{
			note = gen_random_note( Random_Notes ); // the duration must be min_duration
			note_str.append( note.str );
		}
		else
		{
			note_str.push_back(ch) ;
		}
	}
	note_str.push_back(' ');
	return note_str;
}

string Variation_class::scan_sentence( char s0 )
{
	string sentence_line{};
	size_t duration = 0;
	cout << "sentence line :" << endl;

	for( noteword_t& word : Sentence )
	{
		for ( note_t& note : word )
		{
			if ( LogMask[TEST] )
				Note_class::Show_note( note );
			if ( Note_class::OctaveChars.Set.contains( s0 ) )
				note.octave = s0 - 48;
			sentence_line.append( "|" + to_string(note.octave) + note.str );
			duration += note.duration;
		}
		cout << endl;
	}
	return sentence_line;
}
void Variation_class::reverse_word( int i )
{
	Comment( INFO, "generating reverse word " + to_string(i ) );
	size_t len = Sentence.size();
	int m = i % len;
	noteword_t Nm { Sentence[m]};
    std::reverse( Nm.begin(), Nm.end() );
    noteword_t nw {Nm};
    Show_note_list( nw );
    Sentence[m] = nw;
}

void Variation_class::swap_word( int i, int j )
{
	notesentence_t newsentence{};
	size_t len = Sentence.size();
	int m = i % len;
	int n = j % len;
	noteword_t Nn { };
	Nn = Sentence[n] ;
	noteword_t Nm { };
	Nm = Sentence[m];

	int pos = 0;
	for( noteword_t word : Sentence )
	{
		if ( pos == n )
			newsentence.push_back( Nm );
		if ( pos == m )
			newsentence.push_back( Nn );
		if ( ( pos != n ) and ( pos != m ) )
			newsentence.push_back( word );
		pos++;
	}
	Sentence.clear();
	Sentence = newsentence;
}

void Variation_class::set_octave( int oct, noteword_t& word )
{
	std::for_each( word.begin(), word.end(), [oct]( note_t& note ){ note.octave = oct; } );
}

string Variation_class::Gen_noteline( string sentence_layout, string filename )
{

	String Valid { "cr-RS0123456789" };
	string valid_sentence_layout = input_filter( sentence_layout, Valid.Set);

	string noteline{};
	vector<note_t> note_word;
	Sentence.clear();

	size_t pos = 0;
	while( pos < valid_sentence_layout.length() )

	{
		char ch = valid_sentence_layout[ pos ];
		cout << "char " << (char)ch << endl;
		switch ( ch )
		{
			case 'c' :
				{
					string note_str = insert_random();
					noteline.append( note_str );
					noteword_t word = str2words( note_str );
					Sentence.push_back( word );
					break;
				}
			case 'r' :
				{
					noteword_t word = gen_random_note_word();
					Sentence.push_back( word );
					for ( note_t note : word )
						noteline.append( note.str );
					break;
				}
			case '-' :
				{
					noteword_t word = str2words( ".---");
					Sentence.push_back( word );
					noteline.append( ".---");
					break;
				}
			case 'S' :
				{
					int i = valid_sentence_layout[pos +1] -48;
					int j = valid_sentence_layout[pos +2] -48;
					swap_word(i, j);
					cout << dec << i << " " << j << endl;
					pos += 2;
					break;
				}
			case 'R' :
				{	if ( pos >0 )
						reverse_word( pos - 1);
					break;
				}
			default :
			{
				if ( ( Note_class::OctaveChars.Set.contains( ch )) )
				{
					if ( pos > 0 )
					{
						int last = Sentence.size()-1;
						set_octave( ch-48, Sentence[last] );
					}
				}
				else
				{
					Comment(ERROR, "case default points to unknown value " + to_string(ch) );
					exit(1);
				}
				break;
			}
		} // switch ch
		pos++;
	} // for
	noteline = scan_sentence( valid_sentence_layout[0] ) ;

	Note_class::Noteline_prefix.variation = 0;
	Note_class::Set_rhythm_line( Rhythm_chars );
	if (  Note_class::Verify_noteline( Note_class::Noteline_prefix, noteline ) )
	{
		Note_class::Save( filename, Note_class::Noteline_prefix, noteline );
	}
	else
	{
		Exception( "Note line rules were not confirmed." );
	}
	return noteline;
}

void Variation_class::selfTest()
{
	TEST_START( this->className );

	define_random_note_vector("A'(AB)(A'B)(AB,)(B,)A'..");
	int i = 0;
	for( string str : { "A'", "(AB)", "(A'B)", "(AB,)", "(B,)", "A'" ,"." ,"." } )
	{
		assert( strEqual( str, Random_Notes[i].str ) );
		i++;
	}

	Comment ( TEST, "Rhythm");
	Define_rhythm("28");
	Define_fix( "A'(AB)(A'B)(AB,)(B,)A'.." );
	Gen_noteline("c", ".test" );
	vector<int> v = { 20,80,20,80,20,80,0,0 };
	i=0;
	for( note_t note : notelist  )
	{
		cout << v[i] << " "  << note.volume <<" " << note.str << endl;
		assert( v[i] == note.volume  );
		i++;
	}

	Note_base::Note_Chars = OctChars_EN;
	Define_fix( "afafASDF1234!§$_:;,.-#'+***=)(/{[]}ÄÖ<>|′^'");
	Assert_equalstr( Constant_chars , "afafADF1234,.-')(|'") ;

	Define_fix("fGHA");
	Define_variable("EFG");
	Gen_noteline("crcrc", "auto");


	Define_fix( "|2CrF-CrF-" );
	Define_variable ( "C" );

	string test_str = Gen_noteline( "c", "auto") ;
	cout << "gen_noteline " << test_str << endl;
	assert( test_str[2] == 'C');

	Define_variable("C");
	Define_fix( "(AC)r(DC)r" );
	test_str = insert_random( );
	cout << test_str << "=" << "(AC)C(DC)C" << endl;
	assert( strEqual( test_str, "(AC)C(DC)C ") );
	cout << test_str << endl;
	ASSERTION( Note_class::Verify_noteline( Noteline_prefix, test_str ),"test_str", test_str, "");

	Define_fix ( "|2(AC).(DC)r");
	Define_variable ( "C" );
	test_str = insert_random( );
	cout << test_str << endl;
	assert( test_str[11] == 'C');
	Gen_noteline("c", "auto");

	Define_variable( "Cf.',");
	Gen_noteline("-r", "auto");

	Define_fix("fGBA");
	Gen_noteline("ccR", "auto");
	string noteline = Note_class::Get_note_line();
	Comment( BINFO, noteline );
	assert( noteline[11] == 'A' );
	int I;
	string T = Sentence[1][0].str;
	ASSERTION( strEqual( T, "A" ), "sentence.str", T, "A" );

	Define_fix("|2AGBA");
	Gen_noteline("c4c", "auto" );
	noteline = Note_class::Get_note_line();
	Note_class::Noteline_prefix.Octave = 2;
	Comment( BINFO, noteline );
	I = Sentence[0][2].octave; 	assert( I == 4 );
	I = Sentence[1][2].octave;	assert( I == 0 );
	Gen_noteline("4cc", "auto" );
	noteline = Note_class::Get_note_line();
	Comment( BINFO, noteline );
	cout << "s " << dec << Sentence[0][2].octave << endl;
	I = Sentence[0][2].octave;	assert( I == 4 );
	I = Sentence[1][2].octave;	assert( I == 4 );


	TEST_END( this->className );

}



