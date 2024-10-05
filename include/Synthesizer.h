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
#include <Logfacility.h>
#include <Record.h>
#include <Spectrum.h>
#include <Wavedisplay.h>
#include <App.h>
#include <data/DataWorld.h>
#include <Keys.h>
#include <Notes.h>
#include <Ocean.h>
#include <Synthmem.h>
#include <Mixer.h>
#include <System.h>

bool 					SaveRecordFlag 		= false;
bool 					RecordThreadDone 	= false;
string					Module 				= "Synthesizer";

Logfacility_class		Log( Module );
DirStructure_class		Dir;
Dataworld_class			DaTA(SYNTHID );
Application_class		App( &DaTA );
Semaphore_class*		Sem	= DaTA.Sem_p;
Mixer_class				Mixer ( DaTA.Sds.addr );
Instrument_class 		Instrument( DaTA.Sds.addr );
Note_class 				Notes;
Keyboard_class			Keyboard( 	&Instrument );
External_class 			External( 	&Mixer.StA[ MbIdExternal],
									&DaTA.Sds.addr->RecCounter,
									DaTA.Cfg_p);
Config_class*			Cfg 	= DaTA.Cfg_p;
Shared_Memory			Shm_L( sharedbuffer_size );
Shared_Memory			Shm_R( sharedbuffer_size );
Wavedisplay_class 		Wavedisplay;
ProgressBar_class		ProgressBar( &DaTA.Sds.addr->RecCounter );
Time_class				Timer( &DaTA.Sds.addr->time_elapsed );


extern void record_thead_fcn( 	Dataworld_class*,
								External_class* ,
								bool*,
								bool* );


const int 				EXITTEST			= 15;;


#endif /* SYNTHESIZER_H_ */
