/*
 * Audioserver.h
 *
 *  Created on: Aug 28, 2024
 *      Author: sirius
 */

#ifndef AUDIOSERVER_H_
#define AUDIOSERVER_H_


#include <External.h>
#include <Wavedisplay.h>
#include <Thread.h>
#include <AudioVolume.h>
#include <Audio.h>

#include <Appsymbols.h>

Audio_class				Audio				{ &Cfg };
Appstate_class*			Appstate 			= &DaTA.Appstate;
Memory					mono_out			{ monobuffer_bytes };
Stereo_Memory<stereo_t>	stereo				{ max_frames*sizeof(stereo_t) };

Time_class				Timer				{};
Time_class				RecTimer			{};
External_class			External 			{ &Cfg, sds };
ProgressBar_class		ProgressBar			{ &sds->RecCounter };
Wavedisplay_class		Wavedisplay			{ Sds_master };
AudioVolume_class		Volume				{ sds_master };
EventLog_class			Eventlog			{ &DaTA };
Trigger_class			Note_start			{ "Notes start", &sds_master->Note_start };
Trigger_class			Note_end			{ "Notes end  ", &sds_master->Note_end };


// runtime parameter
buffer_t 				ncounter 			= 0;
uint					rcounter			= 0; // # of recorded audioframes
char 					shm_id 				= 0; // out_data = Shm_a
Stereo_t*				shm_addr 			= nullptr;
buffer_t 				audioframes 		= audio_frames;//sds->audioframes;

bool 					audio_out_done		= false;


// ----------------------------------------------------------------
// RT Audio constant declarations





// end declaration section
// --------------------------------------------------------------------------------------------





#endif /* AUDIOSERVER_H_ */
