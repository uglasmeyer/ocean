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
 * Notesbase.h
 *
 *  Created on: Jun 20, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_NOTESBASE_H_
#define INCLUDE_NOTESBASE_H_

#include <data/Config.h>
#include <Ocean.h>
#include <Frequency.h>
#include <Oscbase.h>
#include <String.h>
#include <Table.h>

// noteline control characters
namespace NOTE {
const char IGNORE 		= ' ';
const char PAUSE 		= '.';
const char INCDUR 		= '-';
const char SLIDE		= '>';
const char INCOCT		= '\'';
const char DECOCT		= ',';
const char NEWOCT		= '|';
const char LINEBREAK	= '\n';
const char BRA			= '(';
const char KET			= ')';
const char LONGPLAY		= '!';
}
/*
 * References
 https://blog.sheetmusicplus.com/2015/12/30/learn-how-to-read-sheet-music-notes/
 */

const vector<string> NotesExtension { file_structure::xml_type, file_structure::nte_type };
enum NOTETYPE_e
{
	XML_ID,
	NTE_ID
};

const range_T<uint8_t>	npsidx_range{ 0, 8 };
const range_T<uint8_t>	accidental_range{ 0_uint, 7_uint };

typedef vector<int> step_vec_t ;

constexpr step_vec_t init_pitch( string ac )
{ 	// every char in ac will be translated into a note pos in OctChars
	// ac char must be in OctChars
	step_vec_t pitch_vec {};
	for( uint n = 0; n < ac.length(); n++)
	{
		size_t pos = OctChars_EN.find( ac[n] );
		if( pos < STRINGNOTFOUND )
			pitch_vec.push_back( pos );
		else
		{
			coutf << "Error: " << ac[n] << " not in " << OctChars_EN << endl;
			exit(0);
		}
	}
	return pitch_vec;
};
const step_vec_t 		flat_pitch 		= init_pitch( "BEADGCF" );//{ 11,4,9,5,10,3,8 }; 	// BEADGCF
const step_vec_t 		sharp_pitch		= init_pitch( "FCGDAEB" );//{ 8,3,10,5,0,7,2 };		// FCGDAEB

#define NONOTE 		-12

constexpr char alter_value( char ch )
{
	if ( ch == toupper(ch) )
	{
		return 0;
	}
	else
		return 1;
}
constexpr int8_t step_value( char ch )
{
	char CH = toupper(ch);
	size_t pos = OctChars_EN.find( CH );
	if ( pos == STRINGNOTFOUND )
	{
		pos = OctChars_DE.find( CH );
		if( pos == STRINGNOTFOUND )
			return NONOTE;
	}
	return pos;
}

typedef struct note_value_struct
{
	int8_t 	step 		= NONOTE;
	int8_t	octave		= 0;
	int		alter 		= 0 ; 	// -1,0, +1
	char	step_char	= NOTE::PAUSE;
	uint8_t	frqidx		= 1 ;

	note_value_struct() = default;
	note_value_struct( int oct, char ch, char alt )
	{
		octave 			= check_range( octave_range, oct, "note_value_struct" );
		step_char 		= toupper(ch);
		step			= step_value( ch );
		alter			= ( alt  == 0 ) ? 0 : alt/abs(alt) ;
		frqidx 			= frqIndex( step + alter, octave );
	}
	note_value_struct( int idx )
	{
		frqidx 		= check_range( frqarr_range, idx, "kbd_note_struct" );
		int pos 	= ( idx - C0 ) % oct_steps;
		octave		= ( idx - C0 - pos ) / oct_steps;
		char ch 	= OctChars_EN[ pos ];
		alter		= alter_value( ch );
		step 		= pos - alter;
		step_char 	= toupper(ch);
	}
	~note_value_struct() = default;
} note_value_t;

typedef struct pitch_struct :
		note_value_struct
{
	string				name		= "";
	float				freq		= 0.0;

	pitch_struct()
		: note_value_struct()
	{
		name	= "Pause";//frqNamesArray[ frqidx ];
		freq	= 1.0;//frqArray[ frqidx ];
	}
	pitch_struct( int idx ) // keyboard, notes
		: note_value_struct( idx )
	{
		name	= frqNamesArray[ frqidx ];
		freq	= frqArray[ frqidx ];
	}
	pitch_struct( int oct, char ch, int alt ) // notes, musicxml
		: note_value_struct( oct, ch, alt )
	{
		name	= frqNamesArray[ frqidx ];
		freq	= frqArray[ frqidx ];
	}
	~pitch_struct() = default;

} pitch_t;

typedef struct glide_struct
{
	pitch_t				chord		= pitch_struct( A3 ) ;		// eg. B-->F (glide = F)
	bool				glide		= false;
} glide_t ;

const uint8_t	notes_default_volume = 80;
typedef vector<pitch_t>	pitch_vec_t;
typedef struct 	note_struct
{
	int16_t				number		= -1;
	note_struct(){};
	note_struct( char ch, uint8_t vol ) // Restnote
	{
		str.push_back	(ch);
		chord.push_back	( pitch_struct() );
		volume			= vol;
	}
	note_struct( int16_t nr )
	{
		number = nr;
	}
	~note_struct() = default;
	string 				str 		= ""; 	// humen readable
	pitch_vec_t			chord		{ };	// notes are generated at the same time
	uint16_t 			duration 	= 125; 	// msec
	uint16_t			volume 		= notes_default_volume;//0; 	// percentage of max_volume
	uint8_t 			octave 		= 0; 	// variation only
	vector<glide_t>		glide		{{ pitch_struct( A3 ), false }};
	bool				longnote	= false;// identify a note that jumps over the measure boundary
	bool				longplay	= false;// generate sound max_msec or duration
} note_t;

const uint16_t	measure_duration	= 2000; // 2 sec.

/**************************************************
 * Note_base
 *************************************************/
class Note_base :
	virtual public	Logfacility_class,
	virtual public 	Frequency_class
{

	template <typename T>
	constexpr string octchar_T( T min, T max )
	{
		string str{};
		for( T i = min; i < max; i++ )
		{
			str.push_back( char(i + 48) );
		}
		return str;
	};


public:

	const String			OctaveChars		{ octchar_T(min_octave, max_octave ) };
	const String			NpsChars		{ "12458" };
	const string			NPS_string 		{ "1 2 4 5 8" };

	const vector_str_t 		convention_notes{ 	OctChars_EN,
												"0123456789AB",
												"C%D%EF%G%A%B",
												OctChars_DE};
	const vector_str_t 		convention_names{ 	"English",
												"Numeric",
												"Alphabet",
												"German"};
	enum 					conventionId_t { 	ENGLISH,
												NUMERIC,
												ALPHABET,
												GERMAN };
	const set<int>			conventionId_set { 	ENGLISH,
												NUMERIC,
												ALPHABET,
												GERMAN};


	// describes the default of a noteline


	typedef struct noteline_prefix_struct
	{	// SDS related
		uint8_t		Octave		= 3;
		int8_t		octave_shift= 0; 	// interpreter : set octave+ | set orctave-
		uint8_t	 	convention	= 0;
		uint8_t		nps			= 4;	// notes per second 1,2,4,5,8
		uint8_t		flat		= 0; 	// number of flats  in the key signature - BEADGCF
		uint8_t		sharp		= 0;	// number of sharps in the key signature - FCGDAEB
		uint8_t		variation	= 0;	// 0 no variation, 1 variable note
		int			chord_delay = 0;	// msec delay between each note of a chord
	} noteline_prefix_t;
	const noteline_prefix_t nlp_default = noteline_prefix_struct();
	noteline_prefix_t		Noteline_prefix	= nlp_default; // D=default, N=numeric
	String 					Note_Chars		{ convention_notes[ nlp_default.convention ] };


	typedef list<note_t>	notelist_t	;
	notelist_t 				notelist	{};
	typedef notelist_t::iterator
							note_itr_t	;
	note_itr_t  			note_itr 	;
	note_t 					note_buffer ;
	note_t 					EmptyNote	;

	uint16_t 				min_duration;  //milli seconds

	note_t					rest_note	;


	struct 	musicxml_struct
	{
		string 			instrument_name = "Piano";
		int				divisions 		= 4;
		int				beats			= 16;
		int				beat_type		= 16;
		uint			tempo			= max_sec;
		uint			scoreduration 	= 0; // unit milli second
		notelist_t 		notelist 		{};
	};
	musicxml_struct		musicxml		= musicxml_struct();


	void 				Show_noteline_prefix( noteline_prefix_struct nlp );
	string 				Noteline_prefix_to_string( noteline_prefix_struct nlp );
	noteline_prefix_t	String_to_noteline_prefix( const string& str );
	noteline_prefix_t	Set_base_octave( uint );
	float	 			CalcFreq ( const float& freq ,  pitch_t& pitch );
	int 				Notechar2Step( char note_char );
	uint8_t 			GetFrqIndex( const pitch_t& nvs);
	void 				TestNoteBase();


						Note_base ();
	virtual 			~Note_base();


private:

};

typedef Note_base::noteline_prefix_struct	noteline_prefix_t;
typedef Note_base::notelist_t				notelist_t;
typedef Note_base::musicxml_struct			musicxml_t;
typedef Note_base::note_itr_t				note_itr_t;


#endif /* INCLUDE_NOTESBASE_H_ */
