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

static const uint 	MAXCONFIG 	= 4;
static const string	oceandir_env= "OCEANDIR";
typedef	array<key_t,MAXCONFIG>	keys_arr_t;

struct prgarg_struct
{
		uint 		channel 	= 2;  		// -c
		uint 		rate 		= sample_rate;	// -r
		uint 		device 		= 0;		// -d
		uint 		ch_offs 	= 0; 		// -o
		uint 		MAXWAVFILES	= 5; // max numbers of file names generated automatically
		char		clear		= 'n';		// -X clear process array
		char 		test 		= 'n';		// -t run sanity check on classes and exit = 'y'
		char 		dialog		= 'n';		// -d dialog mode of the composer = 'y'
		char 		composer	= 'n';		// start rtsp with option -C
		char 		oceangui	= 'n';		// start rtsp with option -G
		string 		Genre		= "Alternative";
		string 		author		= "U.G.";
		string		title		= "Experimental";
		string		album		= Application;
		string		Term		= "xterm -e ";
		string		Nohup		= "nohup";
		string 		ffmpeg 		= "ffmpeg";
		string		installdir	= "";		 // Setup -I ...
		string		filename	= "";
		key_t		Sem_key		= 0x9999;
		key_t 		SHM_key 	= 0x100; 	// -k
		key_t		SHM_keyl	= SHM_key;
		key_t		SHM_keyr	= SHM_key+1;
		key_t		SDS_key		= 0x6666;
		keys_arr_t 	sdskeys 	{};
		uint		temp_sec	= 30; 	// seconds storage in StA
		uint 		record_sec	= 180; 	// seconds storage
		uint		kbd_sec		= 2;	// seconds of keyboard stoarage in StA

} ;

typedef prgarg_struct prgarg_t;





#endif /* CONFIGBASE_H_ */
