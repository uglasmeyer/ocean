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
 * Event.h
 *
 *  Created on: Apr 15, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <data/DataWorld.h>
#include <Instrument.h>
#include <notes/Notes.h>
#include <Keyboard.h>
#include <notes/MusicXML.h>
#include <Mixer.h>
#include <data/Sdsbase.h>

class Event_class
	: virtual public 	Logfacility_class
	, virtual			osc_struct
	, 					sdsstate_struct
{
	string 				className = "";

	Interface_class* 	Sds;
	Interface_class*	Sds_master;
	interface_t*		sds;
	interface_t*		sds_master;
	Instrument_class*	Instrument;
	Note_class*			Notes;
	Keyboard_class*		Keyboard;
	Mixer_class*		Mixer;
	Cutter_class*		Cutter;
	Wavedisplay_class*	Wavedisplay;
	Dataworld_class*	DaTA;
	Semaphore_class*	Sem;
	External_class*		External;
	ProgressBar_class*	ProgressBar;
	Musicxml_class*		MusicXML;
	EventQue_class*		EventQue;
	Appstate_class*		Appstate;

public:

	Event_class(Instrument_class* 	instrument,
				Note_class*			notes,
				Keyboard_class*		keyboard,
				Mixer_class*		mixer,
				Wavedisplay_class*	wavedisplay,
				Dataworld_class*	data,
				External_class*		external,
				ProgressBar_class*	progressbar,
				Musicxml_class*		musicxml,
				Cutter_class*		cutter )
		: Logfacility_class("Event_class")
		, sdsstate_struct()
	{
		className = Logfacility_class::className;
		this->DaTA			= data;
		this->Sds			= DaTA->Sds_p;
		this->Sds_master	= DaTA->Sds_master;
		this->sds_master	= DaTA->sds_master;
		this->sds 			= Sds->addr;
		this->Sem			= DaTA->Sem_p;
		this->EventQue		= &DaTA->Sds_p->Eventque;
		this->Instrument 	= instrument;
		this->Notes			= notes;
		this->Keyboard		= keyboard;
		this->Mixer			= mixer;
		this->Wavedisplay	= wavedisplay;
		this->External		= external;
		this->ProgressBar	= progressbar;
		this->MusicXML		= musicxml;
		this->Appstate		= &DaTA->Appstate;
		this->Cutter		= cutter;
	};
	virtual ~Event_class() = default;

	void Handler( );
	void TestHandler();

private:

};




#endif /* EVENT_H_ */
