//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * EventQue.h
 *
 *  Created on: Mar 25, 2025
 *      Author: Ulrich.Glasmeyer@web.de
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
