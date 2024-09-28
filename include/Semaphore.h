/*
 * Semaphore.h
 *
 *  Created on: Sep 21, 2024
 *      Author: sirius
 */

#ifndef SEMAPHORE_CLASS_H_
#define SEMAPHORE_CLASS_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <Config.h>

enum
{
	SEMAPHORE_NULL,
	PROCESSOR_WAIT,
	SEMAPHORE_EXIT,
	SEMAPHORE_START,
	SEMAPHORE_STARTED,
	SEMAPHORE_RECORD
};
static const uint 	SEMNUM_SIZE = 16;

class Semaphore_class  : virtual Logfacility_class, Config_class
{
public:

	Semaphore_class();
	~Semaphore_class();
	void 	Init	();
	void 	Aquire	( uint8_t semnum ); // increase the semaphore ( OP_INC )
	void 	Release	( uint8_t semnum );	// decrease the semaphore ( OP_DEC )
	void 	Lock	( uint8_t semnum );	// wait for release
	int  	Getval	( uint8_t semnum , int op);
	string 	Stat	( uint8_t semnum );

private:
	key_t SEM_KEY 			= 0;
	int semid				= 0;
	typedef struct
			sembuf 	semop_t;
	const uint8_t	SEMNUM	=  0;
	const int8_t	OP_DEC 	= -1;
	const int8_t	OP_INC 	=  1;
	const int8_t 	OP_WAIT	=  0;
	const size_t	N_OPS	=  1;
	const int8_t	SEM_INIT=  0; // initial semaphore value
	const int8_t	UNUSED	=  0;

	void 	init	();
};



#endif /* SEMAPHORE_CLASS_H_ */
