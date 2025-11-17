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
 * Semaphore.h
 *
 *  Created on: Sep 21, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef SEMAPHORE_CLASS_H_
#define SEMAPHORE_CLASS_H_

#include <data/Config.h>
#include <Ocean.h>
#include <Logfacility.h>
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
	SEMAPHORE_KBD,
	SEMAPHORE_SENDDATA0,
	SEMAPHORE_SENDDATA1,
	SEMAPHORE_SENDDATA2,
	SEMAPHORE_SENDDATA3,
	SEMAPHORE_EVENT
};

static const uint 	SEMNUM_SIZE = 16;

class Semaphore_class  :
	virtual public 	Logfacility_class
{
	string 			className = "";
public:
	Time_class		Locktimer{};

			Semaphore_class	( key_t key );
	virtual	~Semaphore_class();
	void 	Init	();
	void 	Aquire	( uint8_t semnum ); // increase the semaphore ( OP_INC )
	void 	Release	( uint8_t semnum );	// decrease the semaphore ( OP_DEC )
	void 	Reset	( uint8_t num );	// release val times

	void 	Lock	( uint8_t semnum );	// wait for release
	bool 	Lock	( uint8_t semnum, uint timeout );	// wait for release
	int  	Getval	( uint8_t semnum , int op);
	string 	State	( uint8_t num);
	string 	SemName	( uint num );


	void	Test	();

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
	void 	Semop	( const unsigned short& num, const short int& sop );

	map< int, string > semnum_map // @suppress("Invalid arguments")
	{
		{SEMAPHORE_NULL,"SEMAPHORE_NULL"},
		{PROCESSOR_WAIT,"PROCESSOR_WAIT"},
		{SEMAPHORE_EXIT,"SEMAPHORE_EXIT"},
		{SYNTHESIZER_START,"SYNTHESIZER_START"},
		{SEMAPHORE_STARTED,"SEMAPHORE_STARTED"},
		{SEMAPHORE_RECORD,"SEMAPHORE_RECORD"},
		{RTSP_STARTED,"RTSP_STARTED"},
		{SEMAPHORE_TEST,"SEMAPHORE_TEST"},
		{SEMAPHORE_SYNCNOTES,"SEMAPHORE_SYNCNOTES"},
		{SEMAPHORE_INITNOTES,"SEMAPHORE_INITNOTES"},
		{SEMAPHORE_KBD,"SEMAPHORE_KBD"},
		{SEMAPHORE_SENDDATA0,"SEMAPHORE_SENDDATA0"},
		{SEMAPHORE_SENDDATA1,"SEMAPHORE_SENDDATA1"},
		{SEMAPHORE_SENDDATA2,"SEMAPHORE_SENDDATA2"},
		{SEMAPHORE_SENDDATA3,"SEMAPHORE_SENDDATA3"},
		{SEMAPHORE_EVENT,"SEMAPHORE_EVENT"}
	};
};


#endif /* SEMAPHORE_CLASS_H_ */
