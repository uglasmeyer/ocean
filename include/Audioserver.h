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

// classes and structures

string Module 			= "AudioServer";

Logfacility_class 		Log( Module );
Dataworld_class			DaTA( AUDIOID );
Application_class		App( &DaTA );
interface_t*			sds	= nullptr;
Config_class*			Cfg = DaTA.Cfg_p;
Time_class				Timer;


// runtime parameter
buffer_t 		ncounter 		= 0;
char 			shm_id 			= 0; // out_data = Shm_a
stereo_t*		shm_addr 		= nullptr;
char 			mode 			= FREERUN;
const buffer_t 	chunksize		= max_frames / 100;//441 , 512; // Audio server chunksize

uint			bufferFrames 	= chunksize;
int 			record			= 0;
bool 			done 			= false;
bool*			Done			= &done;

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
