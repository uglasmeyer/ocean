/*
 * Notenase.cpp
 *
 *  Created on: Jul 3, 2024
 *      Author: sirius
 */

#include <notes/Notesbase.h>
#include <System.h>
#include <Exit.h>

Note_base::Note_base () :
	Logfacility_class("Note_base"),
	Frequency_class()
{
	className = Logfacility_class::className;
};

Note_base::~Note_base()
{
};


uint8_t Note_base::GetFrqIndex( const pitch_t& nvs)
{
	int 		step	= nvs.step + nvs.alter;
	int 		oct 	= nvs.octave;
	if ( step 	< 0 )
	{
				step 	+= oct_steps;
				oct 	-=  1;
	}
	int			octave	= abs( oct );// + octave_shift );
	uint8_t 	key 	= Index( octave, step );
	return key ;
}
float Note_base::CalcFreq ( const float& base,  pitch_t& nvs )
{

	int 		key 	= GetFrqIndex( nvs );
	return 		Frequency_class::Calc(base, key);
};
int Note_base::Notechar2Step( char note_char )
{
	if ( Note_Chars.Set.contains( note_char ) )
		return Note_Chars.Str.find( note_char );
	else
		return -12;
}
void Note_base::Set_base_octave( uint diff )
{
	octave_shift = octave_shift + ( 2*diff - 1);
	if ( octave_shift < 0 )
		octave_shift = 0;
}

void Note_base::Set_noteline_prefix( noteline_prefix_t nlp )
{
	this->Noteline_prefix = nlp;
	Show_noteline_prefix( nlp );
}
void Note_base::Show_noteline_prefix( noteline_prefix_t nlp )
{
	Table_class Table {"Noteline Prefix", 22 };
	Table.AddColumn( "Prefix", 20 );
	Table.AddColumn( "Value", 15 );
	Table.PrintHeader();

	string convention = to_string( nlp.convention) + ":" + convention_names[nlp.convention ];
	Table.AddRow( "Convention"	,  convention );
	Table.AddRow( "Octave"		, (int) nlp.Octave );
	Table.AddRow( "Notes per sec",(int) nlp.nps );
	Table.AddRow( "Flats"		, (int) nlp.flat );
	Table.AddRow( "Sharps"		, (int) nlp.sharp );
	Table.AddRow( "Chord delay"	, (int) nlp.chord_delay );
}

string Note_base::Noteline_prefix_to_string( noteline_prefix_t nlp )
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

Note_base::noteline_prefix_t Note_base::String_to_noteline_prefix( string str )
{
	auto range_error = [ this ]( auto val, vector<size_t> range )
	{
		Comment ( ERROR, "Out of Range [" + to_string( range[0] ) + "," +
											to_string( range[1] ) + "}" );
		if ( LogMask[ TEST ] ) return;
		EXCEPTION( "Cannot assign noteline_prefix " +
				 to_string( val ) +
				" to noteline_structure" );//raise( SIGINT );
	};

	noteline_prefix_t nlp;
	String S = str;
	vector_str_t arr = S.to_unique_array(',');

	if ( arr.size() < 5 )
	{
		if ( LogMask[ TEST ] ) return noteline_prefix_default;
		EXCEPTION( "Cannot assign noteline_prefix of length < 5" );//raise( SIGINT );
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
		if ( LogMask[ TEST ] ) return noteline_prefix_default;
		EXCEPTION( "Cannot assign notes per second " + arr[2] + " to noteline_structure" );//raise( SIGINT );
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

	// chord_delay
	if ( arr.size() > 6 )
	{
		nlp.chord_delay = S.to_int( arr[6] );
	}
	else
	{
		nlp.chord_delay = 0;
	}
	return nlp;

}

void Note_base::TestNoteBase()
{

	TEST_START("Notes_base");

	Show_noteline_prefix( noteline_prefix_default );
	Comment( TEST, Noteline_prefix_to_string( noteline_prefix_default ));
	string nlp_str = "2,1,4,0,0,0,0";
	noteline_prefix_t nlp = String_to_noteline_prefix(nlp_str);
	Show_noteline_prefix(nlp);

	nlp_str = "1 ";
	nlp = String_to_noteline_prefix(nlp_str);
	nlp_str = "8, 8, 8, 7, 8, 0, 0";
	nlp = String_to_noteline_prefix(nlp_str);

	TEST_END("Notes_base");
}



