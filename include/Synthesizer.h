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
#include <Record.h>
#include <Spectrum.h>
#include <Wavedisplay.h>
#include <App.h>
#include <data/DataWorld.h>
#include <Keys.h>
#include <Notes.h>
#include <Ocean.h>
#include <Mixer.h>
#include <System.h>

bool 					SaveRecordFlag 		= false;
bool 					RecordThreadDone 	= false;
string					Module 				= "Synthesizer";

Logfacility_class		Log( Module );
DirStructure_class		Dir;
Dataworld_class			DaTA( SYNTHID );
Application_class		App( &DaTA );
Mixer_class				Mixer ( nullptr);// DaTA.Sds_master );
Instrument_class 		Instrument{ nullptr };
Note_class 				Notes;
Keyboard_class			Keyboard( 	&Instrument );
External_class 			External( 	&Mixer.StA[ MbIdExternal],
									DaTA.Cfg_p);
Wavedisplay_class 		Wavedisplay;
ProgressBar_class		ProgressBar( nullptr );
Statistic_class 		Statistic{ Log.module };

Semaphore_class*		Sem	= DaTA.Sem_p;
Config_class*			Cfg = DaTA.Cfg_p;




const int 				EXITTEST			= 15;;


#endif /* SYNTHESIZER_H_ */
