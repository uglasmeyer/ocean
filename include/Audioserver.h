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



// classes and structures

string Module 			= "AudioServer";

Logfacility_class 		Log( Module );
Dataworld_class			DaTA( AUDIOID );
Application_class		App( &DaTA );
interface_t*			sds	= DaTA.Sds_master;
Config_class*			Cfg = DaTA.Cfg_p;
External_class			External { Cfg, sds };
ProgressBar_class		ProgressBar( &sds->RecCounter );

Time_class				RecTimer;
Time_class				Timer;

Memory					mono_out	{ monobuffer_size };
Wavedisplay_class		Wavedisplay	{ DaTA.Sds_p };



// runtime parameter
buffer_t 		ncounter 		= 0;
uint			rcounter		= 0;
char 			shm_id 			= 0; // out_data = Shm_a
stereo_t*		shm_addr 		= nullptr;
char 			mode 			= FREERUN;
const buffer_t 	chunksize		= max_frames / 100;//441 , 512; // Audio server chunksize

uint			bufferFrames 	= chunksize;
bool 			done 			= false;

// ----------------------------------------------------------------
// RT Audio constant declarations

typedef struct device_struct
{
	uint 	Index;
	uint 	Id;
	string 	Name;
} device_struct_t;
device_struct_t DeviceDescription;



#define FORMAT RTAUDIO_SINT16 // 16bit signed integer



// end declaration section
// --------------------------------------------------------------------------------------------





#endif /* AUDIOSERVER_H_ */
