/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * Thread.h
 *
 *  Created on: Apr 16, 2025
 *      Author: Ulrich.Glasmeyer@web.de
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
