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
//#include <data/Memory.h>
//#include <data/Interface.h>
#include <System.h>
//#include <App.h>
#include <External.h>
#include <Wavedisplay.h>
#include <Keyboard.h>
//#include <Exit.h>
#include <Mixer.h>
#include <Thread.h>
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
Memory					mono_out			{ monobuffer_size };
Time_class				Timer				{};
Time_class				RecTimer			{};
Keyboard_class			Keyboard			{};
External_class			External 			{ DaTA.Cfg_p, sds };
ProgressBar_class		ProgressBar			{ &sds->RecCounter };
Wavedisplay_class		Wavedisplay			{ DaTA.Sds_p };


// runtime parameter
buffer_t 				ncounter 			= 0;
uint					rcounter			= 0;
char 					shm_id 				= 0; // out_data = Shm_a
stereo_t*				shm_addr 			= nullptr;
const buffer_t 			chunksize			= max_frames / 48;// / 100;//441 , 512; // Audio server chunksize
buffer_t 				audioframes 		= sds->audioframes;
uint					bufferFrames 		= chunksize;
bool 					done 				= false;

extern void save_record_fcn();
Thread_class			SaveRecord			{ DaTA.Sem_p,
											  SEMAPHORE_RECORD,
											  save_record_fcn,
											  "save record" };

// ----------------------------------------------------------------
// RT Audio constant declarations



#define FORMAT RTAUDIO_SINT16 // 16bit signed integer


// end declaration section
// --------------------------------------------------------------------------------------------





#endif /* AUDIOSERVER_H_ */
