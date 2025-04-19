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
#include <Time.h>
#include <Table.h>



enum
{
	SEMAPHORE_NULL,
	PROCESSOR_WAIT,
	SEMAPHORE_EXIT,
	SYNTHESIZER_START,
	SEMAPHORE_STARTED,
	SEMAPHORE_RECORD,
	RTSP_STARTED,
	SEMAPHORE_TEST,
	SEMAPHORE_SYNCNOTES,
	SEMAPHORE_INITNOTES,
	AUDIOSERVER_SEM,
	SEMAPHORE_SENDDATA0,
	SEMAPHORE_SENDDATA1,
	SEMAPHORE_SENDDATA2,
	SEMAPHORE_SENDDATA3,
	SEMAPHORE_EVENT
};

static const uint 	SEMNUM_SIZE = 16;

class Semaphore_class  :
		virtual public Logfacility_class,
		virtual Time_class
{
	string className = "Semaphore_class";
public:
	Time_class	lock_timer{};

	Semaphore_class( Config_class* );
	~Semaphore_class();
	void 	Init	();
	void 	Aquire	( uint8_t semnum ); // increase the semaphore ( OP_INC )
	void 	Release	( uint8_t semnum );	// decrease the semaphore ( OP_DEC )
	void 	Reset	( uint8_t num );	// release val times

	void 	Lock	( uint8_t semnum );	// wait for release
	void 	Lock	( uint8_t semnum, uint timeout );	// wait for release
	int  	Getval	( uint8_t semnum , int op);

	string 	Stat	( uint8_t semnum );
	void	Test( );

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
	void 	Semop( const unsigned short& num, const short int& sop );
};



#endif /* SEMAPHORE_CLASS_H_ */
