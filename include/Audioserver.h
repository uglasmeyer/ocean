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

#include <Ocean.h>
#include <data/Memory.h>
#include <data/Interface.h>
#include <System.h>
#include <App.h>
#include <External.h>
#include <Wavedisplay.h>
#include <Keyboard.h>
#include <Exit.h>
#include <Mixer.h>


typedef struct device_struct
{
	uint 	Index;
	uint 	Id;
	string 	Name;
} device_struct_t;
device_struct_t DeviceDescription;


// classes and structures

Exit_class				Exit{};
Logfacility_class 		Log{ "AudioServer" };

Time_class				Timer{};
Time_class				RecTimer{};
Keyboard_class			Keyboard{};

Dataworld_class			DaTA( AUDIOID );
Application_class		App( &DaTA );
Config_class*			Cfg = DaTA.Cfg_p;
interface_t*			sds	= DaTA.sds_master;
Memory					mono_out	{ monobuffer_size };
External_class			External { DaTA.Cfg_p, sds };
ProgressBar_class		ProgressBar( &sds->RecCounter );

Wavedisplay_class		Wavedisplay	{ DaTA.Sds_p };

//cz * 100 = sr, mf / 1.2 = sr , => mf = cz * 100 * 1.2

// runtime parameter
buffer_t 		ncounter 		= 0;
uint			rcounter		= 0;
char 			shm_id 			= 0; // out_data = Shm_a
stereo_t*		shm_addr 		= nullptr;
const buffer_t 	chunksize		= max_frames / 48;// / 100;//441 , 512; // Audio server chunksize

uint			bufferFrames 	= chunksize;
bool 			done 			= false;

// ----------------------------------------------------------------
// RT Audio constant declarations



#define FORMAT RTAUDIO_SINT16 // 16bit signed integer


// end declaration section
// --------------------------------------------------------------------------------------------





#endif /* AUDIOSERVER_H_ */
