/*
 * Record.h
 *
 *  Created on: Mar 28, 2024
 *      Author: sirius
 */

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include <data/Interface.h>
#include <data/Memory.h>
#include <Ocean.h>

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

	void Setup( uint8_t* sds );
	void SetValue( uint );
	void Set( uint* count, uint max );
	void Unset();
	void Reset();
	void Update( );
};

#endif /* PROGRESSBAR_H_ */
