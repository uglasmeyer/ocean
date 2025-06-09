/*
 * Audioserver.h
 *
 *  Created on: Aug 28, 2024
 *      Author: sirius
 */

#ifndef AUDIOSERVER_H_
#define AUDIOSERVER_H_

#include <RtAudio.h>
#define RTAUDIO
#include <string>
extern void errorCallback( RtAudioErrorType /*type*/, const std::string& errorText );

RtAudio rtapi( RtAudio::LINUX_PULSE, &errorCallback );


#include <External.h>
#include <Wavedisplay.h>
#include <Keyboard.h>
#include <Thread.h>
#include <AudioVolume.h>

#include <Appsymbols.h>

typedef struct device_struct
{
	uint 	Index;
	uint 	Id;
	string 	Name;
} device_struct_t;
device_struct_t DeviceDescription;


// classes and structures



Appstate_class*			Appstate 			= &DaTA.Appstate;
Memory					mono_out			{ monobuffer_bytes };
Stereo_Memory<stereo_t>	stereo				{ max_frames*sizeof(stereo_t) };

Time_class				Timer				{};
Time_class				RecTimer			{};
External_class			External 			{ &Cfg, sds };
ProgressBar_class		ProgressBar			{ &sds->RecCounter };
Wavedisplay_class		Wavedisplay			{ Sds_master };
AudioVolume_class		Volume				{ sds_master };

// runtime parameter
buffer_t 				ncounter 			= 0;
uint					rcounter			= 0;
char 					shm_id 				= 0; // out_data = Shm_a
Stereo_t*				shm_addr 			= nullptr;
buffer_t 				audioframes 		= audio_frames;//sds->audioframes;
uint					bufferFrames 		= chunksize;
bool 					done 				= false;


// ----------------------------------------------------------------
// RT Audio constant declarations



#define FORMAT RTAUDIO_SINT16 // 16bit signed integer


// end declaration section
// --------------------------------------------------------------------------------------------





#endif /* AUDIOSERVER_H_ */
