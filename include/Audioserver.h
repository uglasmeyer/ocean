/*
 * Audioserver.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef AUDIOSERVER_H_
#define AUDIOSERVER_H_


#include <External.h>
#include <Wavedisplay.h>
#include <Thread.h>
#include <Audio.h>

#include <Appsymbols.h>




Audio_class				Audio				{ &Cfg };
Record_class			Record				{ &DaTA };
Appstate_class*			Appstate 			= &DaTA.Appstate;
Heap_Memory				mono_out			{ monobuffer_bytes }; // wave display data
Stereo_Memory<stereo_t>	stereo				{ max_frames * sizeof(stereo_t) };

Time_class				Timer				{};
ProgressBar_class		ProgressBar			{ &sds->RecCounter };
Wavedisplay_class		Wavedisplay			{ Sds_master };
AudioVolume_class		Volume				{ sds_master };
EventLog_class			Eventlog			{ &DaTA };



// runtime parameter
buffer_t 				ncounter 			= 0;
char 					shm_id 				= 0; // out_data = Shm_a
Stereo_t*				shm_addr 			= nullptr;

bool 					audio_out_done		= false;
buffer_t				audioframes			= audio_frames;

// ----------------------------------------------------------------
// RT Audio constant declarations





// end declaration section
// --------------------------------------------------------------------------------------------





#endif /* AUDIOSERVER_H_ */
