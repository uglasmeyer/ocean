/*
 * Mixerbase.h
 *
 *  Created on: Sep 5, 2024
 *      Author: sirius
 */

#include <data/Memory.h>
#include <Ocean.h>

#ifndef MIXERBASE_H_
#define MIXERBASE_H_


enum STAID
{
	STA_USER00,
	STA_USER01,
	STA_USER02,
	STA_USER03,
	STA_INSTRUMENT,
	STA_KEYBOARD,
	STA_NOTES,
	STA_EXTERNAL,
	STA_SIZE
};

template<typename T>
constexpr string 	StAIdName( const T& sta_id )
{
	switch ( sta_id )
	{
		case STA_USER00		: return "StA User 0";
		case STA_USER01		: return "StA User 1";
		case STA_USER02		: return "StA User 2";
		case STA_USER03		: return "StA User 3";
		case STA_INSTRUMENT	: return "StA_Instrument";
		case STA_KEYBOARD	: return "StA Keyboard";
		case STA_NOTES		: return "StA Notes";
		case STA_EXTERNAL	: return "StA External";
		default 		: 	{
							cout << "WARN: unknown Storage Area id: " << ( int)sta_id << endl;
							return "";
							};
	}
}

typedef array<uint8_t, STA_SIZE> StAarray_t; //SDS related

constexpr StAarray_t genMemIds()
{
	StAarray_t a{};
	std::iota(a.begin(), a.end(), 0 );
	return a;
};

class Mixer_base
{
public:
	Mixer_base(){};
	~Mixer_base(){};

	struct mixer_state_struct
	{
		// SDS structure
		bool 	sync			= true; // explicite sync mode
		bool 	notes			= false; // play notes
		bool 	external		= false; // external play or record
		bool	mute			= false; // mute master volume
		bool	kbd				= false; // play keyboard note
		bool	instrument		= true; // play instrument
	} ;

	static constexpr StAarray_t StAMemIds = genMemIds();

	typedef	array< StA_state_t	, STA_SIZE>	StA_state_arr_t;
}; /*Mixer_base*/


typedef 	Mixer_base::mixer_state_struct 	mixer_state_t;
const 		mixer_state_t					default_mixer_state = Mixer_base::mixer_state_struct();
typedef		Mixer_base::StA_state_arr_t		StA_state_arr_t;
const 		StA_state_t 					default_StA_state 	= StA_state_struct();

constexpr 	StA_state_arr_t init_StA_state_arr ()
{
	StA_state_arr_t A {};
	for( size_t n = 0; n < A.size(); n++ )
		 A[n] = default_StA_state;
	return A;
}
const 		StA_state_arr_t 				default_sta_state_arr= init_StA_state_arr();

#endif /* MIXERBASE_H_ */
