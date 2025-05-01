/*
 * EventQue.h
 *
 *  Created on: Mar 25, 2025
 *      Author: sirius
 */

#ifndef DATA_EVENTQUE_H_
#define DATA_EVENTQUE_H_

class EventQue_class :
		virtual Logfacility_class,
		EventPtr_struct
{
public:
	interface_t*	addr		= nullptr;
	eventptr_t		eventptr;
	bool			repeat		= false;
	uint8_t			prev_event	= NULLKEY;

	EventQue_class(  )  :
		Logfacility_class("EventQue_class"),
		eventptr()
	{}


	virtual ~EventQue_class(){};

	void setup( interface_t* _addr );
	void reset();
	void add( uint8_t event );
	uint8_t get();
	string show();
};




#endif /* DATA_EVENTQUE_H_ */
