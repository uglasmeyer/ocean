/*
 * Notenase.cpp
 *
 *  Created on: Jul 3, 2024
 *      Author: sirius
 */

#include <notes/Notesbase.h>
#include <System.h>

Note_base::Note_base () :
Logfacility_class("NotesBase")
{
	for( uint n = 0; n < root2_limit; n++ )
	{
		float x = pow(2.0, (n+3)/12.0) ; // 3 considers A0 = 55/4
		root2.push_back( x );
	}
};

Note_base::~Note_base()
{};

// https://de.wikipedia.org/wiki/Frequenzen_der_gleichstufigen_Stimmung
float Note_base::CalcFreq ( const float& base,  pitch_t& nvs )
{
	int step	= nvs.step + nvs.alter;
	int oct 	= nvs.octave;
	if ( step 	< 0 )
	{
		step 	+= 12;
		oct 	-=  1;
	}
	uint8_t	octave	= abs( oct  + octave_shift );
	int		key 	= octave *12 + step;
	return 	Calc_frequency( base, key );
};


float Note_base::Calc_freq ( uint8_t oct, pitch_t nvs )
{
	int key = nvs.step + nvs.alter;
	if ( key < 0 )
	{
		key += 12;
		oct -=  1;
	}
	uint8_t	octave	= abs( oct  + octave_shift );
	return 	Calc_frequency( oct_base_freq, octave * 12 + key );
};

float Note_base::Calc_frequency( const float& base, const int& key )
{
	Assert( not( key < 0 ), "key" + to_string(key) );
	if ( abs( key ) > root2_limit )
	{
		Comment( WARN, "Octave key " + to_string( key ) + " adjusted to limit " + to_string( root2_limit ) );
		return root2[ root2_limit ] * base;
	}
	return root2[ key ] * base;
}

void Note_base::Set_base_octave( uint diff )
{
	octave_shift = octave_shift + ( 2*diff - 1);
	if ( octave_shift < 0 )
		octave_shift = 0;
}

float Note_base::Octave_freq( uint8_t oct )
{
	if ( oct == 0 ) return oct_base_freq;
	uint oct2 = (uint) oct_base_freq * 2;
	return oct2 << ( oct - 1 );
}




void Note_base::Set_noteline_prefix( noteline_prefix_t nlp )
{
	this->Noteline_prefix = nlp;
	Show_noteline_prefix( nlp );
}
void Note_base::Show_noteline_prefix( noteline_prefix_t nlp )
{
	stringstream strs{ "\n" };
	string nlp_str = convention_names[nlp.convention ];
	strs << "Note prefix:" << endl;
	strs << setw(20) << left << "Octave: " 			<< (int) nlp.Octave << endl;
	strs << setw(20) << left << "Convention: " 		<< (int) nlp.convention << ":" << nlp_str << endl;
	strs << setw(20) << left << "Notes per sec: " 	<< (int) nlp.nps << endl;
	strs << setw(20) << left << "Flats: " 			<< (int) nlp.flat << endl;
	strs << setw(20) << left << "Sharps: " 			<< (int) nlp.sharp << endl;
	Comment( INFO, strs.str() );
}

string Note_base::Noteline_prefix_to_string( noteline_prefix_t nlp )
{
	stringstream strs{""};
	strs 	<< (int) nlp.Octave 		<<","
			<< (int) nlp.convention		<<","
			<< (int) nlp.nps 			<<","
			<< (int) nlp.flat			<<","
			<< (int) nlp.sharp ;
	return strs.str();
}

Note_base::noteline_prefix_t Note_base::String_to_noteline_prefix( string str )
{
	auto range_error = [ this ]( auto val, vector<size_t> range )
		{
			Comment ( ERROR, "Out of Range [" + to_string( range[0] ) + "," +
												to_string( range[1] ) + "}" );
			if ( Log[ TEST ] ) return;
			Exception( "Cannot assign noteline_prefix " +
					 to_string( val ) +
					" to noteline_structure" );//raise( SIGINT );
		};

	String S = str;
	noteline_prefix_t nlp;
	vector_str_t arr = S.to_unique_array(',');

	if ( arr.size() < 5 )
	{
		if ( Log[ TEST ] ) return noteline_prefix_default;
		Exception( "Cannot assign noteline_prefix of length < 5" );//raise( SIGINT );
	}

	char oct_ch = arr[0][0];
	if ( OctaveChars.Set.contains( oct_ch ) )
		nlp.Octave = char2int( oct_ch );
	else
		range_error( char2int( oct_ch ), { min_octave, max_octave } );

	nlp.convention = char2int(arr[1][0] );
	if ( not conventionId_set.contains( nlp.convention ))
		range_error( nlp.convention, {0, convention_names.size() } );


	char nps_ch = arr[2][0];
	if ( not NpsChars.Set.contains( nps_ch ) )
	{
		if ( Log[ TEST ] ) return noteline_prefix_default;
		Exception( "Cannot assign notes per second " + arr[2] + " to noteline_structure" );//raise( SIGINT );
	}
	else
		nlp.nps = char2int( nps_ch );


	set<int> pref_set = range_set( 0, 7 );

	int
	val = char2int( arr[3][0] );
	if ( pref_set.contains( val ) )
		nlp.flat = val;
	else
		range_error( val, {0, 7} );

	val = char2int( arr[4][0] );
	if ( pref_set.contains( val ) )
		nlp.sharp = val;
	else
		range_error( val, {0, 7} );

	return nlp;

}

void Note_base::TestNoteBase()
{
	TEST_START("Notes_base");

	Show_noteline_prefix( noteline_prefix_default );
	Comment( TEST, Noteline_prefix_to_string( noteline_prefix_default ));
	string nlp_str = "2,1,4,0,0,";
	noteline_prefix_t nlp = String_to_noteline_prefix(nlp_str);
	Show_noteline_prefix(nlp);

	nlp_str = "1 ";
	nlp = String_to_noteline_prefix(nlp_str);
	nlp_str = "8, 8, 8, 7, 8";
	nlp = String_to_noteline_prefix(nlp_str);

	TEST_END("Notes_base");
}



