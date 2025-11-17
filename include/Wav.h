/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

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
