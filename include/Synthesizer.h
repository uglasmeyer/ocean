/*
 * Synthesizer.h
 *
 *  Created on: Sep 19, 2024
 *      Author: sirius
 */

#ifndef SYNTHESIZER_H_
#define SYNTHESIZER_H_

#include <External.h>
#include <Instrument.h>
#include <Keyboard.h>
#include <Spectrum.h>
#include <Wavedisplay.h>
#include <App.h>
#include <data/DataWorld.h>
#include <Keys.h>
#include <Notes.h>
#include <Ocean.h>
#include <Mixer.h>
#include <Progressbar.h>
#include <System.h>
#include <Print.h>

const string			Module 				= "Synthesizer";
const int 				EXITTEST			= 15;;


class Core_class :
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

public:

	Core_class( Instrument_class* 	instrument,
				Note_class*			notes,
				Mixer_class*		mixer,
				Wavedisplay_class*	wavedisplay,
				Dataworld_class*	data,
				External_class*		external,
				ProgressBar_class*	progressbar) : Logfacility_class("Core_class")
	{
		className = Logfacility_class::module;
		this->DaTA			= data;
		this->Sds			= DaTA->Sds_p;
		this->sds_master	= DaTA->Sds_master;
		this->sds 			= Sds->addr;
		this->Sem			= DaTA->Sem_p;
		this->Instrument 	= instrument;
		this->Notes			= notes;
		this->Mixer			= mixer;
		this->Wavedisplay	= wavedisplay;
		this->External		= external;
		this->ProgressBar	= progressbar;
	};
	virtual ~Core_class()
	{};

	void processKey( char);

private:

};
#endif /* SYNTHESIZER_H_ */
