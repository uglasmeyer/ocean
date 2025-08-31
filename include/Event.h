/*
 * Event.h
 *
 *  Created on: Apr 15, 2025
 *      Author: sirius
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <data/DataWorld.h>
#include <Instrument.h>
#include <notes/Notes.h>
#include <Keyboard.h>
#include <notes/MusicXML.h>
#include <Mixer.h>

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
	Wavedisplay_class*	Wavedisplay;
	Dataworld_class*	DaTA;
	Semaphore_class*	Sem;
	External_class*		External;
	ProgressBar_class*	ProgressBar;
	Musicxml_class*		MusicXML;
	EventQue_class*		EventQue;

public:

	Event_class(Instrument_class* 	instrument,
				Note_class*			notes,
				Keyboard_class*		keyboard,
				Mixer_class*		mixer,
				Wavedisplay_class*	wavedisplay,
				Dataworld_class*	data,
				External_class*		external,
				ProgressBar_class*	progressbar,
				Musicxml_class*		musicxml )
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
	};
	virtual ~Event_class() = default;

	void Handler( );
	void TestHandler();

private:

};




#endif /* EVENT_H_ */
