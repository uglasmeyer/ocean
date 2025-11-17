//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * audioserver.h
 *
 *  Created on: Jan 24, 2024
 *      Author: Ulrich.Glasmeyer@web.de
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
	int  	chunk_size		= 16;    			 	/* size of FMT chunk in bytes (usually 16) */
	short 	format_tag		= 1;     				/* 1=PCM, 257=Mu-Law, 258=A-Law, 259=ADPCM */
	short 	num_chans		= 2;      				/* 1=mono, 2=stereo                        */
	int  	srate			= sample_rate;			/* Sampling rate in samples per second     */
	int  	bytes_per_sec 	= sample_rate*4;  		/* bytes per second quatsch= srate*bytes_per_samp */
	short 	bytes_per_samp	= sizeof_data;	 		/* 2=16-bit quatsch mono, 4=16-bit stereo          */
	short 	bits_per_samp	= 8*sizeof_data;  	/* Number of bits per sample               */
	char  	data[4]			= {'d','a','t','a'};   	/* "data"                       */
	long  	dlength        	= 1;		/* data length in bytes (filelength - 44)  			   */
} wav_struct_t;




#endif /* INCLUDE_WAV_H_ */
