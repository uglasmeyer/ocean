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

string					Module 				= "Synthesizer";

Logfacility_class		Log( Module );
DirStructure_class		Dir;
Dataworld_class			DaTA( SYNTHID );

Wavedisplay_class 		Wavedisplay{ DaTA.Sds_p };
Wavedisplay_class*		wd_p = &Wavedisplay;

Application_class		App( &DaTA );

interface_t*			sds = DaTA.GetSdsAddr();
Dataworld_class*		DaTA_p = &DaTA;
Mixer_class				Mixer{ DaTA_p, wd_p } ;// DaTA.Sds_master );
Instrument_class 		Instrument{ sds, wd_p };
Note_class 				Notes;
Keyboard_class			Keyboard( 	&Instrument );
External_class 			External( 	&Mixer.StA[ MbIdExternal],
									DaTA.Cfg_p);
ProgressBar_class		ProgressBar( &sds->RecCounter );
Statistic_class 		Statistic{ Log.module };

Semaphore_class*		Sem	= DaTA.Sem_p;
Config_class*			Cfg = DaTA.Cfg_p;


const uint 				Sync_Semaphore 	= SEMAPHORE_SENDDATA0 + DaTA.SDS_Id;


const int 				EXITTEST			= 15;;


#endif /* SYNTHESIZER_H_ */
