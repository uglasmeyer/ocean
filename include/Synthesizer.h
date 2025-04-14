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
#include <data/EventQue.h>

typedef function< void( void )> loop_function_t;

class Thread_class :
	virtual Logfacility_class
{
	string className = "";
	uint SemId;
	Semaphore_class* Sem_p;
	bool thread_done = false;
	loop_function_t loop_fnc;
	string Name = "";

public:

	Thread_class( 	Semaphore_class* sem,
					uint semid,
					loop_function_t fnc,
					const string& name ) :
		Logfacility_class("Thread_class")
	{
		Set_Loglevel( INFO, true );
		className 	= Logfacility_class::className;
		SemId 		= semid;
		Sem_p		= sem;
		loop_fnc 	= fnc;
		Name		= name;
	};

	~Thread_class() = default;
	void Loop( )
	{

		Info( "Loop started on semaphore ", SemId  );
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		while( true )
		{
			Sem_p->Lock( SemId );
			if( thread_done )
				break;
			this->loop_fnc(  );
		}
		Info( "Loop terminated on semaphore ", SemId  );

	}

	void StopLoop()
	{
		thread_done = true;
		Sem_p->Reset( SemId );
		Logfacility_class::ResetLogMask();
	}

private:
};

class Event_class :
	virtual public Logfacility_class
{
	string 				className = "";

	Interface_class* 	Sds;
	Interface_class*	Sds_master;
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
	EventQue_class*		EventQue;

public:

	Event_class(Instrument_class* 	instrument,
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
		this->Sds_master	= DaTA->Sds_master;
		this->sds_master	= DaTA->sds_master;
		this->sds 			= Sds->addr;
		this->Sem			= DaTA->Sem_p;
		this->EventQue		= &DaTA->Sds_p->Eventque;
		this->Instrument 	= instrument;
		this->Notes			= notes;
		this->Mixer			= mixer;
		this->Wavedisplay	= wavedisplay;
		this->External		= external;
		this->ProgressBar	= progressbar;
		this->MusicXML		= musicxml;
	};
	virtual ~Event_class() = default;

	void Handler( );

private:

};
#endif /* SYNTHESIZER_H_ */
