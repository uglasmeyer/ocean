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
		this->Appstate		= &DaTA->Appstate;
	};
	virtual ~Event_class() = default;

	void Handler( );
	void TestHandler();

private:

};




#endif /* EVENT_H_ */
