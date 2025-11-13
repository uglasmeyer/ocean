/*
 * EventQue.h
 *
 *  Created on: Mar 25, 2025
 *      Author: sirius
 */

#ifndef DATA_EVENTQUE_H_
#define DATA_EVENTQUE_H_

//#include <Logfacility.h>
//#include <data/Sdsbase.h>
#include <data/SharedDataSegment.h>

// controll of access to SDS.deque and SDS.eventptr
class EventQue_class :
		virtual Logfacility_class,
		EventPtr_struct
{
	string 			className	= "";
public:
	interface_t*	addr		= nullptr;
	eventptr_t		eventptr;
	bool			repeat		= false;
	EVENTKEY_e		prev_event	= NULLKEY;

	EventQue_class(  )  :
		Logfacility_class("EventQue_class"),
		eventptr()
	{
		className = Logfacility_class::className;
	}


	virtual ~EventQue_class(){};

	void 			setup( interface_t* _addr );
	void 			reset();
	void 			add( EVENTKEY_e event );
	EVENTKEY_e 		get();
	string 			show();
};




#endif /* DATA_EVENTQUE_H_ */
