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

//static const uint 					STA_SIZE 				= 8;


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

typedef array<uint8_t, STA_SIZE> StAarray_t;

constexpr StAarray_t f()
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

	typedef struct mixer_status_struct
	{
		// SDS structure
		bool 	sync			= true; // explicite sync mode
		bool 	notes			= false; // play notes
		bool 	external		= false; // external play or record
		bool	mute			= false; // mute master volume
		bool	kbd				= false; // play keyboard note
		bool	instrument		= false; // play instrument
	} mixer_status_t;

	static constexpr StAarray_t StAMemIds = f();
//	StAMemconstexpr Stam  			= { 0, 1, 2, 3, 4, 5, 6, 7 };


	typedef	array< StA_status_t	, STA_SIZE>	StA_state_arr_t;
};

#endif /* MIXERBASE_H_ */
