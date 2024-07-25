/*
 * Variation.cpp
 *
 *  Created on: Apr 30, 2024
 *      Author: sirius
 */

#include <variation.h>

// https://en.cppreference.com/w/cpp/language/operators

Variation_class::Variation_class() :
Logfacility_class{ "Variation" }, Note_class{}
{};


void Variation_class::randomize_notes_octave( string str )
{

	size_t n = 0;

	while( n < Random_Notes.size() )
	{

		char ch = gen_random_note( str );//str[0];
		if ( ch == '\'' )
		{
			Random_Notes[n].chord[0].doct = 1;
			Random_Notes[n].str.push_back('\'');
		}
		if ( ch == ',' )
		{

			Random_Notes[n].chord[0].doct = -1;
			Random_Notes[n].str.push_back(',');
		}
		if ( TEST )
			Note_class::show_note( this->Random_Notes[n] );
		n++;
	}


}
void Variation_class::set_note_chars( uint prefix  )
{
	Note_class::Note_Chars = convention_notes[ prefix ];
	Note_class::Noteline_prefix.convention 	= prefix;
	Note_class::Noteline_prefix.flat 		= 0;
	Note_class::Noteline_prefix.sharp 		= 0;

}

void Variation_class::set_note_chars( uint flats, uint sharps  )
{
	Note_class::Note_Chars = convention_notes[ 2 ];
	Note_class::Noteline_prefix.convention 	= 2;
	Note_class::Noteline_prefix.flat 		= flats;
	Note_class::Noteline_prefix.sharp 		= sharps;


}


Variation_class::noteword_t Variation_class::list2vector( notelist_t notes )
{
	noteword_t word {};
	for( note_struct_t note : notes )
		word.push_back( note );
	return word;
}


void Variation_class::define_random_note_vector( string str )
{
	Note_class::Noteline_prefix.variation = 1;
	Charset_class V{ Note_class::Note_Chars + "." };
	Random_Notes.clear();
	verify_noteline( Note_class::Noteline_prefix, str);
	Random_Notes = list2vector( Note_class::notelist );
	show_note_list(Random_Notes);
//	randomize_notes_octave( str );
}


string Variation_class::input_filter( string input, Charset_class valid)
{
	string result { "" };
	std::copy_if( input.begin(), input.end(),
							back_inserter( result ),
							[&valid]( char ch ){ return valid.contains(ch);});
	return result;
}

void Variation_class::define_fix( string notes )
{
	auto set_octave = [ this  ]( string notes )
		{
			if ( notes[0] == '|' )
				if ( Octave_Set.contains( notes[1] ) )
					Note_class::Octave = (int)notes[1]-48;
		};
	if ( notes.length() < 4 )
	{
		Comment( ERROR, "Constant_chars contains less than 4 characters");
		exit(1);
	}

	if ( notes[0] == '-' )
	{
		Comment( ERROR, "Constant_chars must not start with specified character ");
		exit(1);
	}
	Charset_class Valid_set ( "r(),.-'| " + Note_class::OctaveChars + Note_class::Note_Chars );
	Constant_Set = notes ;
	set_octave( notes );
	Constant_Set = Constant_Set / Valid_set;
	Constant_chars = input_filter( notes, Valid_set);

}

void Variation_class::define_variable( string notes )
{
	define_random_note_vector( notes );
}

void Variation_class::define_rhythm( string rhythm )
{

	set_rhythm_line( rhythm );
	Rhythm_chars = rhythm;
}

Variation_class::noteword_t Variation_class::gen_random_note_word()
{
	note_struct_t 	note{};
	noteword_t 		word{};

	buffer_t duration 	= 0;

	while ( duration < Note_class::measure_duration )
	{
		note = gen_random_note( Random_Notes );

		duration += Note_class::min_duration;
		note.duration = Note_class::min_duration;
		char ch = gen_random_note( Random_Notes ).str[0];
		while ( ( ch == '-' ) and ( duration < Note_class::measure_duration ))
		{
			duration += Note_class::min_duration;
			note.duration += Note_class::min_duration;
			note.str.push_back( '-' );
			ch = gen_random_note( Random_Notes ).str[0];
		}
		note.octave = Note_class::Octave;
		word.push_back( note );
		if ( TEST )
			Note_class::show_note( note );
	}
	return word;

}

Variation_class::noteword_t Variation_class::str2words( string note_str )
{
	// take a notestring and separate it into one or more words of note structures
	noteword_t word{};
	if ( Note_class::verify_noteline( Noteline_prefix, note_str ) )
	{
		for( note_struct_t note : Note_class::notelist )
		{
			word.push_back( note );
		}
	}
	return word;
}
string Variation_class::insert_random(  )
{

	string note_str{};
	note_struct_t note;

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
		for ( note_struct_t& note : word )
		{
			if ( TEST )
				Note_class::show_note( note );
			if ( Octave_Set.contains( s0 ) )
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
    show_note_list( nw );
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
	std::for_each( word.begin(), word.end(), [oct]( note_struct_t& note ){ note.octave = oct; } );
}

string Variation_class::gen_noteline( string sentence_layout, string filename )
{
//	if ( Variable_chars.length() == 0 )
//	{
//		Comment( ERROR, "empty note set");
//		Comment( INFO, "define noteset first");
//		exit(1);
//	}

	Charset_class Valid_set { "cr-RS0123456789" };
	string valid_sentence_layout = input_filter( sentence_layout, Valid_set);

	string noteline{};
	vector<note_struct_t> note_word;
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
					for ( note_struct_t note : word )
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
				if ( ( Octave_Set.contains( ch )) )
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
	Note_class::set_rhythm_line( Rhythm_chars );
	if (  Note_class::verify_noteline( Note_class::Noteline_prefix, noteline ) )
	{
		Note_class::save( filename, Note_class::Noteline_prefix, noteline );
	}
	else
	{
		Comment( ERROR, "Note line rules were not confirmed.");
		exit(1);
	}
	return noteline;
}

void Variation_class::test()
{
	Set_Loglevel( TEST, true );
	Comment(TEST,"Test variation class ");

	define_random_note_vector("A'(AB)(A'B)(AB,)(B,)A'..");
	int i = 0;
	for( string str : { "A'", "(AB)", "(A'B)", "(AB,)", "(B,)", "A'" ,"." ,"." } )
	{
		assert( cmpstr( str, Random_Notes[i].str ) );
		i++;
	}

	Comment ( TEST, "Rhythm");
	define_rhythm("28");
	define_fix( "A'(AB)(A'B)(AB,)(B,)A'.." );
	gen_noteline("c", ".test" );
	vector<int> v = { 20,80,20,80,20,80,0,0 };
	i=0;
	for( note_struct_t note : notelist  )
	{
		cout << v[i] << " "  << note.volume <<" " << note.str << endl;
		assert( v[i] == note.volume  );
		i++;
	}


	define_variable("(EF)G'',,");
	define_fix( "afafASDF1234!§$_:;,.-#'+***=)(/{[]}ÄÖ<>|′^'");
	cout << "Constant_chars: " << Constant_chars << endl;
	assert( cmpstr( Constant_chars , "afafADF1234,.-')(|'" ));

	define_fix("fGHA");
	define_variable("EFG");
	gen_noteline("crcrc", "auto");


	define_fix( "|2CrF-" );
	define_variable ( "C" );

	string test_str = gen_noteline( "c", "auto") ;
	cout << "gen_noteline " << test_str << endl;
	assert( test_str[2] == 'C');

	define_variable("C");
	define_fix( "(AC)r(DC)r" );
	test_str = insert_random( );
	cout << test_str << "=" << "(AC)C(DC)C" << endl;
	assert( cmpstr( test_str, "(AC)C(DC)C ") );
	cout << test_str << endl;
	assert( Note_class::verify_noteline( Noteline_prefix, test_str ) );

	define_fix ( "|2(AC).(DC)r");
	define_variable ( "C" );
	test_str = insert_random( );
	cout << test_str << endl;
	assert( test_str[11] == 'C');
	gen_noteline("c", "auto");

	define_variable( "Cf.',");
	gen_noteline("-r", "auto");

	define_fix("fGBA");
	gen_noteline("ccR", "auto");
	string noteline = Note_class::get_note_line();
	Comment( BINFO, noteline );
	assert( noteline[11] == 'A' );
	int I;
	string T = Sentence[1][0].str;
	cout << T << endl;
	assert( T.compare("A") == 0 );

	define_fix("fGBA");
	gen_noteline("c4c", "auto" );
	noteline = Note_class::get_note_line();
	Comment( BINFO, noteline );
	I = Sentence[0][2].octave; 	assert( I == 4 );
	I = Sentence[1][2].octave;	assert( I == 2 );
	gen_noteline("4cc", "auto" );
	noteline = Note_class::get_note_line();
	Comment( BINFO, noteline );
	cout << "s " << dec << Sentence[0][2].octave << endl;
	I = Sentence[0][2].octave;	assert( I == 4 );
	I = Sentence[1][2].octave;	assert( I == 4 );


//	assert( false );
	Comment( BINFO, "Variation test OK");

}



