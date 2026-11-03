/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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
 * EventQue.h
 *
 *  Created on: Mar 25, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_EVENTQUE_H_
#define DATA_EVENTQUE_H_

#include <data/SharedDataSegment.h>

/**************************************************
 * EventQue_class
 * management of the SDS.deque structure and
 * SDS.eventptr structure
 *
 *************************************************/
class EventQue_class :
		virtual Logfacility_class,
		EventPtr_data
{
	eventptr_t		eventptr	;
	EVENTKEY_e		prev_event	;
public:
	interface_t*	sds			;
	bool			Repeat		;

					EventQue_class	( void* _sds );
	virtual 		~EventQue_class	(){};

	void 			Reset			();
	void 			Add				( EVENTKEY_e event );
	EVENTKEY_e 		Get				();
	string 			Show			();
};


typedef vector<EVENTKEY_e> eventkey_vec_t;
typedef struct event_buffer_struct
{
	eventkey_vec_t Keybuffer;
	EventQue_class* eventque;

	event_buffer_struct( EventQue_class* que)
	{
		Keybuffer	= {};
		eventque	= que;
	};
	~event_buffer_struct() = default;
	void add( eventkey_vec_t vec )
	{
		for_each( vec, [ this ]( EVENTKEY_e key)
				{ Keybuffer.push_back( key) ;});
	};
	void release()
	{	// add event_buffer to eventque
		for_each( Keybuffer, [ this ]( EVENTKEY_e key )
				{ eventque->Add( key );	} );
		Keybuffer.clear();
	}
} event_buffer_t;


#endif /* DATA_EVENTQUE_H_ */
