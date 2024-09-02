/*
 * Audioserver.h
 *
 *  Created on: Aug 28, 2024
 *      Author: sirius
 */

#ifndef AUDIOSERVER_H_
#define AUDIOSERVER_H_


/*
// Platform-dependent sleep routines.
#if defined( WIN32 )
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif
*/

#include <RtAudio.h>
#define RTAUDIO

#include <Synthesizer.h>
#include <synthmem.h>
#include <Interface.h>
#include <common.h>
#include <App.h>

// classes and structures

string Module 			= "AudioServer";

Logfacility_class 		Log( Module );
Shared_Memory 			Shm_a;
Shared_Memory			Shm_b;
Interface_class 		IFD ;
Application_class		App( Module, AUDIOID, &IFD.addr->AudioServer );


// runtime parameter
ifd_t*			ifd				= IFD.addr;
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
