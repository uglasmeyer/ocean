/*
 * Record.h
 *
 *  Created on: Mar 28, 2024
 *      Author: sirius
 */

#ifndef RECORD_H_
#define RECORD_H_

#include <Interface.h>
#include <Synthesizer.h>
#include <Synthmem.h>

class ProgressBar_class : virtual public Logfacility_class
{
public:
	// static parameter
	ifd_t* ifd_addr;
	ProgressBar_class(ifd_t* addr) : Logfacility_class("Record")
	{
		this->ifd_addr = addr;
		this->counter = 0;
	};
	virtual ~ProgressBar_class(){};

	// dynamic parameter
	uint* counter;
	uint max_counter = 1;
	bool active = false;

	void Set_progress_bar( uint );
	void Set( uint* count, uint max );
	void Unset();
	void Reset();
	void Progress_bar_update( );
};

#endif /* RECORD_H_ */
