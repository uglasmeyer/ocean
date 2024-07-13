/*
 * Notesbase.h
 *
 *  Created on: Jun 20, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_NOTESBASE_H_
#define INCLUDE_NOTESBASE_H_

#include <synthesizer.h>


/*
 * References
 * https://blog.sheetmusicplus.com/2015/12/30/learn-how-to-read-sheet-music-notes/
 */


class Note_base : virtual public Logfacility_class
{
public:

	const uint				max_octave		= 5;
	const uint 				min_octave 		= 1;
	const vector<int> 		Notes_per_Sec	{1,2,4,8};
	const string			NPS_string 		{"1,2,4,8"};
	const vector<int> 		flat_pitch 		= {2,7,0,5,10,3,8};
	const vector<int> 		sharp_pitch		= {8,3,10,5,0,7,2};
	const vector_str_t 		convention_notes= { "AaBCcDdEFfGg", "0123456789AB", "ABCDEFG" };
	const vector_str_t 		convention_names= { "English", "Numeric", "Alphabet" };

	// describes the default of a noteline

	typedef struct root2_struct
	{
		vector<float>	vec{};
		root2_struct( uint max )
		{
			for( uint n = 0; n < 12*max; n++ )
			{
				float x = pow(2, n/12.0) ;
				vec.push_back( x );
			}
		}
	} root2_struc_t;

	typedef struct noteline_prefix_struct
	{
		uint 	Octave		= 1;
		uint 	convention	= 0;
		uint 	nps			= 4;	// notes per second 1,2,4,8
		uint	flat		= 0; 	// number of flats  in the key signature - BEADGCF
		uint	sharp		= 0;	// number of sharps in the key signature - FCGDAEB
		uint	variation	= 0;	// 0 no variation, 1 variable note
	} noteline_prefix_t;
	const noteline_prefix_t
	noteline_prefix_default = noteline_prefix_struct();


	typedef struct notevalue_struct{
		signed char 		doct 		= 0 ; 	// -1,0, +1
		int 				value		= 0 ;	// -12, 0, 1...12
		float				freq		= 0.0;
	} notevalue_struct_t;
	notevalue_struct_t		notevalue_buffer = notevalue_struct();

	typedef vector<notevalue_struct_t>
							chord_struct_t;

	typedef struct glide_struct
	{
		notevalue_struct_t	chord		= notevalue_struct() ;		// eg. B-->F (glide = F)
		bool				note		= false;
	} glide_struc_t ;

	typedef struct 	note_struct
	{
		string 				str 		= ""; 	// humen readable
		chord_struct_t		chord		{};		// notes are generated at the same time
		uint16_t 			duration 	= 0; 	// msec
		uint16_t			volume 		= 0; 	// percentage of max_volume
		uint8_t 			octave 		= 0; 	// 1...9 ( * 55 ) = base frequency ot the octave
		glide_struc_t		glide		= glide_struct();
		bool				longnote	= false;// identify a note that jumps over the measure boundary

	} note_struct_t;
	typedef list<note_struct_t>
							notelist_t;

	root2_struc_t			root2 = root2_struct( max_octave);

	Note_base () : Logfacility_class("NotesBase")
	{
	};
	~Note_base(){};

	void 				show_noteline_prefix( noteline_prefix_t nlp );
	string 				noteline_prefix_to_string( noteline_prefix_t nlp );
	noteline_prefix_t 	string_to_noteline_prefix( string str );
	void 				test();

};



#endif /* INCLUDE_NOTESBASE_H_ */
