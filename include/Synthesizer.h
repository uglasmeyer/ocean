/*
 * Synthesizer.h
 *
 *  Created on: Sep 19, 2024
 *      Author: sirius
 */

#ifndef SYNTHESIZER_H_
#define SYNTHESIZER_H_

#include <Wavedisplay_base.h>
#include <External.h>
#include <Instrument.h>
#include <Keyboard.h>
#include <Spectrum.h>
#include <Wavedisplay.h>
#include <App.h>
#include <data/DataWorld.h>
#include <EventKeys.h>
#include <Ocean.h>
#include <Mixer.h>
#include <Progressbar.h>
#include <System.h>
#include <Exit.h>
#include <notes/Notes.h>
#include <notes/MusicXML.h>


class Event_class :
	virtual Logfacility_class
{
	string 				className = "";
	Interface_class* 	Sds;
	interface_t*		sds;
	interface_t*		sds_master;
	Instrument_class*	Instrument;
	Note_class*			Notes;
	Mixer_class*		Mixer;
	Wavedisplay_class*	Wavedisplay;
	Dataworld_class*	DaTA;
	Semaphore_class*	Sem;
	External_class*		External;
	ProgressBar_class*	ProgressBar;
	Musicxml_class*		MusicXML;

public:

	Event_class( Instrument_class* 	instrument,
				Note_class*			notes,
				Mixer_class*		mixer,
				Wavedisplay_class*	wavedisplay,
				Dataworld_class*	data,
				External_class*		external,
				ProgressBar_class*	progressbar,
				Musicxml_class*		musicxml ) :
					Logfacility_class("Event_class")
	{
		className = Logfacility_class::className;
		this->DaTA			= data;
		this->Sds			= DaTA->Sds_p;
		this->sds_master	= DaTA->sds_master;
		this->sds 			= Sds->addr;
		this->Sem			= DaTA->Sem_p;
		this->Instrument 	= instrument;
		this->Notes			= notes;
		this->Mixer			= mixer;
		this->Wavedisplay	= wavedisplay;
		this->External		= external;
		this->ProgressBar	= progressbar;
		this->MusicXML		= musicxml;
	};
	virtual ~Event_class()
	{};

	void Handler( uint key);

private:

};
#endif /* SYNTHESIZER_H_ */
