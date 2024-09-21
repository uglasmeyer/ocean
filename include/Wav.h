/*
 * audioserver.h
 *
 *  Created on: Jan 24, 2024
 *      Author: sirius
 */
//https://forum.arduino.cc/t/creating-a-wav-file-header/314260/4
#ifndef INCLUDE_WAV_H_
#define INCLUDE_WAV_H_

#include <Ocean.h>

typedef struct wav_struct
{
	char  	riff[4]			= {'R','I','F','F'}; 	/* "RIFF"                      */
	int  	flength			= 10000;        		// file length in bytes                    			   *
	char 	wave[4]			= {'W','A','V','E'};   	/* "WAVE"                       */
	char  	fmt[4]			= {'f','m','t',' '};   	/* "fmt "                       */
	int  	chunk_size		= 16;     	/* size of FMT chunk in bytes (usually 16) */
	short 	format_tag		= 1;     	/* 1=PCM, 257=Mu-Law, 258=A-Law, 259=ADPCM */
	short 	num_chans		= 2;      	/* 1=mono, 2=stereo                        */
	int  	srate			= max_frames;    /* Sampling rate in samples per second     */
	int  	bytes_per_sec 	= max_frames*4;  /* bytes per second quatsch= srate*bytes_per_samp */
	short 	bytes_per_samp	= 2; 		/* 2=16-bit quatsch mono, 4=16-bit stereo          */
	short 	bits_per_samp	= 8*2;  	/* Number of bits per sample               */
	char  	data[4]			= {'d','a','t','a'};   	/* "data"                       */
	long  	dlength        	= 1;		/* data length in bytes (filelength - 44)  			   */
} wav_struct_t;




#endif /* INCLUDE_WAV_H_ */
