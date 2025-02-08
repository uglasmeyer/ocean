/*
 * Notesbase.h
 *
 *  Created on: Jun 20, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_NOTESBASE_H_
#define INCLUDE_NOTESBASE_H_

#include <Ocean.h>
#include <Oscbase.h>
#include <String.h>
#include <Config.h>


/*
 * References
 * https://blog.sheetmusicplus.com/2015/12/30/learn-how-to-read-sheet-music-notes/
 */

const vector<string>		NotesExtension { file_structure().xml_type, file_structure().nte_type };

class Note_base : virtual public Logfacility_class
{
	string 					className		= "";
public:
	const string			notes_ext		= file_structure().nte_type;
	const String 			OctaveChars 	{ "0123456" }; // oct_base = 27.5
	const uint				max_octave		= (uint) OctaveChars.Str.length()-1;
	const uint 				min_octave 		= 0;

	const String			NpsChars		{ "12458" };
	const string			NPS_string 		{ "1 2 4 5 8" };
	const vector<int> 		flat_pitch 		{ 2,7,0,5,10,3,8 };
	const vector<int> 		sharp_pitch		{ 8,3,10,5,0,7,2 };

	const vector_str_t 		convention_notes{ 	"CcDdEFfGgAaB",
												"0123456789AB",
												"C%D%EF%G%A%B",
												"CcDdEFfGgAaH"};
	const vector_str_t 		convention_names{ 	"English",
												"Numeric",
												"Alphabet",
												"German"};
	enum 					conventionId_t { 	ENGLISH,
												NUMERIC,
												ALPHABET,
												GERMAN };
	set<int> 				conventionId_set { 	ENGLISH,
												NUMERIC,
												ALPHABET,
												GERMAN};

	static const uint8_t	notes_default_volume = 80;

	// describes the default of a noteline


	typedef struct noteline_prefix_struct
	{
		uint8_t		Octave		= 3;
		uint8_t	 	convention	= 0;
		uint8_t		nps			= 4;	// notes per second 1,2,4,5,8
		uint8_t		flat		= 0; 	// number of flats  in the key signature - BEADGCF
		uint8_t		sharp		= 0;	// number of sharps in the key signature - FCGDAEB
		uint8_t		variation	= 0;	// 0 no variation, 1 variable note
	} noteline_prefix_t;
	const noteline_prefix_t
				noteline_prefix_default = noteline_prefix_struct();
	noteline_prefix_t
					Noteline_prefix	= noteline_prefix_default; // D=default, N=numeric


	typedef struct pitch_struct
	{
		char				step_char	= ' ';
		int			 		alter 		= 0 ; 	// -1,0, +1
		int 				step		= -12 ;	// -12, 0, 1...12
		uint 				octave		= 2	;
		float				freq		= 0.0;
	} pitch_t;

	typedef struct glide_struct
	{
		pitch_t			chord		= pitch_struct() ;		// eg. B-->F (glide = F)
		bool				note		= false;
	} glide_t ;

	typedef struct 	note_struct
	{
		string 				str 		= ""; 	// humen readable
		vector<pitch_t>		chord		{ };		// notes are generated at the same time
		uint16_t 			duration 	= 0; 	// msec
		uint16_t			volume 		= notes_default_volume;//0; 	// percentage of max_volume
		uint8_t 			octave 		= 0; 	// 1...9 ( * 55 ) = base frequency ot the octave
		vector<glide_t>		glide		{{ pitch_struct(), false }};//= glide_struct();
		bool				longnote	= false;// identify a note that jumps over the measure boundary

	} note_t;
	typedef list<note_t>	notelist_t;
	notelist_t 		notelist		{};



	void 				Show_noteline_prefix( noteline_prefix_t nlp );
	void				Set_noteline_prefix( noteline_prefix_t nlp );
	string 				Noteline_prefix_to_string( noteline_prefix_t nlp );
	noteline_prefix_t 	String_to_noteline_prefix( string str );

	void				Set_base_octave( uint );
	float	 			Octave_freq( uint8_t oct );
//	float	 			Calc_freq ( uint8_t , pitch_t );
	float	 			CalcFreq ( const float& freq ,  pitch_t& pitch );

	void 				TestNoteBase();


	Note_base ();
	virtual ~Note_base();


private:

	int			octave_shift  	= 0; 	// interpreter : set octave+ | set orctave-
	Frequency_class	noteFreq		{ };

	float 		calc_frequency(const float& base,  const int& key );

};



#endif /* INCLUDE_NOTESBASE_H_ */
