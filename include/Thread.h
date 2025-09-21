/*
 * Thread.h
 *
 *  Created on: Apr 16, 2025
 *      Author: sirius
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <Logfacility.h>
#include <data/Semaphore.h>

typedef function< void( void )> loop_function_t;

class Thread_class :
	virtual Logfacility_class
{
	string 				className 		= "";
	uint 				SemId;
	Semaphore_class* 	Sem_p;
	bool 				thread_done 	= false;
	loop_function_t 	loop_fnc;
	string 				Name 			= "";

public:

	Thread_class( 	Semaphore_class* sem,
					uint semid,
					loop_function_t fnc,
					const string& name ) :
		Logfacility_class("Thread_class")
	{
		Set_Loglevel( INFO, true );
		className 	= Logfacility_class::className;
		SemId 		= semid;
		Sem_p		= sem;
		loop_fnc 	= fnc;
		Name		= name;
	};

	~Thread_class()
		{ DESTRUCTOR( className )};
	void Loop( )
	{

		Info( "Loop started on semaphore ", Sem_p->SemName( SemId )  );
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		while( true )
		{
			Sem_p->Lock( SemId );
			if( thread_done )
				break;
			else
				this->loop_fnc(  );
		}
		Info( "Loop terminated on semaphore ", Sem_p->SemName( SemId ) );

	}

	void StopLoop()
	{
		thread_done = true;
		Sem_p->Reset( SemId );
	}

private:
};




#endif /* THREAD_H_ */
