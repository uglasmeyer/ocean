/*
 * Mixerbase.h
 *
 *  Created on: Sep 5, 2024
 *      Author: sirius
 */

#ifndef MIXERBASE_H_
#define MIXERBASE_H_

//#include <data/Memorybase.h>

enum STAID_e : unsigned char
{
	STA_USER00 = 0,
	STA_USER01,
	STA_USER02,
	STA_USER03,
	STA_INSTRUMENT,
	STA_KEYBOARD,
	STA_NOTES,
	STA_EXTERNAL,
	STA_SIZE
};

constexpr string StAIdName( STAID_e sta_id )
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
	return "";
}
typedef array<STAID_e, STAID_e::STA_SIZE> 	StAarray_t; //SDS related

const 	StAarray_t 	StAMemIds =
{
	STA_USER00,
	STA_USER01,
	STA_USER02,
	STA_USER03,
	STA_INSTRUMENT,
	STA_KEYBOARD,
	STA_NOTES,
	STA_EXTERNAL
};
const range_T<STAID_e> staid_range{ (STAID_e)0, STA_SIZE };

struct mixer_state_struct
{	// SDS related
	bool 	sync			= true; // explicite sync mode
	bool 	notes			= false; // play notes
	bool 	external		= false; // external play or record
	bool	mute			= false; // mute master volume
	bool	kbd				= false; // play keyboard note
	bool	instrument		= true; // play instrument
} ;
typedef 	mixer_state_struct 	mixer_state_t;

#endif /* MIXERBASE_H_ */
