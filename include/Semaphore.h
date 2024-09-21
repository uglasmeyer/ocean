/*
 * Semaphore.h
 *
 *  Created on: Sep 21, 2024
 *      Author: sirius
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <Ocean.h>
#include <Logfacility.h>


class Semaphore_class  : Logfacility_class
{
public:
	int SEM_KEY = 0x1234;
	int semid	= 0;
    struct sembuf sem_op;

	const uint 	SEMNUM	=  0;
	const int	OP_DEC 	= -1;
	const int	OP_INC 	=  1;
	const int 	OP_WAIT	=  0;
	const int	SEM_INIT=  0; // initial semaphore value

	Semaphore_class();
	~Semaphore_class();
	void init();
	void aquire(); // increase the semaphore ( OP_INC )
	void release();	// decrease the semaphore ( OP_DEC )
	void lock();	// wait for release
	int  getval();
};



#endif /* SEMAPHORE_H_ */
