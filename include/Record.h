/*
 * Record.h
 *
 *  Created on: Mar 28, 2024
 *      Author: sirius
 */

#ifndef RECORD_H_
#define RECORD_H_

#include <data/Interface.h>
#include <Ocean.h>
#include <Synthmem.h>

class ProgressBar_class : virtual Logfacility_class
{
public:
	// static parameter
	uint8_t* RecCounter;
	ProgressBar_class(uint8_t* ); // addr of the counter to watch
	virtual ~ProgressBar_class();

	// dynamic parameter
	uint* counter;
	uint max_counter = 1;
	bool active = false;

	void Setup( uint );
	void Set( uint* count, uint max );
	void Unset();
	void Reset();
	void Update( );
};

#endif /* RECORD_H_ */
