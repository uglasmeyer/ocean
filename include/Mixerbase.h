/*
 * Mixerbase.h
 *
 *  Created on: Sep 5, 2024
 *      Author: sirius
 */

#include <Ocean.h>
#include <Synthmem.h>

#ifndef MIXERBASE_H_
#define MIXERBASE_H_

static const uint 	MbSize 			= 8;

class Mixer_base
{
public:
	Mixer_base(){};
	~Mixer_base(){};

	typedef struct mixer_status_struct // mixer status
	{
		bool 	play			= false; // explicite sync mode
		bool 	notes			= false; // play notes
		bool 	external		= false; // external play or record
		bool	mute			= false; // mute master volume
		bool	kbd				= false; // play keyboard note
		bool	instrument		= false; // play instrument
	} mixer_status_t;


	typedef array< uint8_t, MbSize> 				StA_amp_arr_t;
	typedef	array<Storage::StA_status_t, MbSize>	StA_state_arr_t;


};



#endif /* MIXERBASE_H_ */
