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
 * Notenase.cpp
 *
 *  Created on: Jul 3, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <notes/Notesbase.h>
#include <System.h>
#include <Exit.h>

Note_base::Note_base () :
	Logfacility_class("Note_base"),
	Frequency_class()
{
	this->min_duration			= 1000 / nlp_default.nps;  //milli seconds
	this->className 			= Logfacility_class::className;
	this->note_buffer 			= note_struct();
	this->notelist.clear		();
	this->note_itr 				= notelist.begin();
	this->Noteline_prefix		= nlp_default; // D=default, N=numeric
	this->Note_Chars			= { convention_notes[ nlp_default.convention ] };
	this->rest_note				= note_struct( PAUSE, 0 );
	this->rest_note.duration	= min_duration;
	this->EmptyNote				= rest_note;
	this->EmptyNote.chord.clear	();

}

Note_base::~Note_base()
{
	DESTRUCTOR( className )
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
		return NONOTE;
}
noteline_prefix_t Note_base::Set_base_octave( uint diff )
{
	int8_t 				change	= 2 * diff -1; // diff=0 or 1
	noteline_prefix_t 	nlp 	= Noteline_prefix;
	int 				oct 	= check_range( 	octave_range,
								Noteline_prefix.Octave + change,
								"Set_base_octave" );
	nlp.octave_shift 			= oct - nlp.Octave;
//	Set_noteline_prefix( nlp );
	return nlp;
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

noteline_prefix_t Note_base::String_to_noteline_prefix( const string& str )
{
	auto range_error = [ this ]( auto val, vector<size_t> range )
	{
		Comment ( ERROR, "Out of Range [", range[0], range[1], "]" );
		if ( LogMask[ TEST ] )
			return 0;
		Comment( ERROR,  "Cannot assign noteline_prefix ", (int)val, "to noteline_structure" );
		raise( SIGINT );
		return 0;
	};

	noteline_prefix_t 	nlp = nlp_default;
	String 				S 	{ str };
	vector_str_t 		arr = S.to_unique_array(',');

	if ( arr.size() < 5 )
	{
		if ( LogMask[ TEST ] ) return nlp_default;
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
		if ( LogMask[ TEST ] ) return nlp_default;
		Exception( "Cannot assign notes per second " + arr[2] + " to noteline_structure" );//raise( SIGINT );
	}
	else
		nlp.nps = char2int( nps_ch );


	set<int> 	pref_set 	= range_set( 0, 7 );
	int			val 		= char2int( arr[3][0] );
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

	Show_noteline_prefix( nlp_default );
	Comment( TEST, Noteline_prefix_to_string( nlp_default ));
	string nlp_str = "2,1,4,0,0,0,0";
	noteline_prefix_t nlp = String_to_noteline_prefix(nlp_str);
	Show_noteline_prefix(nlp);

	nlp_str = "1 ";
	nlp = String_to_noteline_prefix(nlp_str);
	nlp_str = "8, 8, 8, 7, 8, 0, 0";
	nlp = String_to_noteline_prefix(nlp_str);

	TEST_END("Notes_base");
}



