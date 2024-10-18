/*
 * Configbase.h
 *
 *  Created on: Oct 17, 2024
 *      Author: sirius
 */

#ifndef CONFIGBASE_H_
#define CONFIGBASE_H_

#include <Ocean.h>
#include <Version.h>


static const uint MAXCONFIG 	= 4;
typedef	array<key_t, MAXCONFIG>	keys_arr_t;
typedef array<string,APP_SIZE > type_map_t;

typedef struct prgarg_struct
{
		~prgarg_struct(){};
		uint 		channel 	= 2;  		// -c
		uint 		rate 		= audio_frames;	// -r
		uint 		device 		= 0;		// -d
		uint 		ch_offs 	= 0; 		// -o
		char 		test 		= 'n';		// -t run sanity check on classes and exit = 'y'
		char 		dialog		= 'n';		// -d dialog mode of the composer = 'y'
		char 		composer	= 'n';		// rtsp -C
		char 		oceangui	= 'n';		// rtsp -G
		string 		Genre		= "Alternative";
		string 		author		= "U.G.";
		string		title		= "Experimental";
		string		album		= Application;
		string		Term		= "xterm -e ";
		string 		ffmpeg 		= "ffmpeg";
		key_t 		SHM_key 	= 0x100; 	// -k
		key_t		SHM_keyl	= SHM_key;
		key_t		SHM_keyr	= SHM_key+1;
		key_t		SDS_key		= 0x6666;
		uint		SDS_id		= 0;
		key_t		Sem_key		= 0x9999;
		keys_arr_t 	sdskeys 	{};
//		keys_arr_t	shmkeys_l	{};
//		keys_arr_t	shmkeys_r	{};
		string 		appcfg		= "S0.cfg";  // TODO name of Synthesizer instances
		char		clear		= 'n';


} prgarcg_struct_t;





#endif /* CONFIGBASE_H_ */
