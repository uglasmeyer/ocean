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
 * Audioserver.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef AUDIOSERVER_H_
#define AUDIOSERVER_H_


#include <External.h>
#include <Wavedisplay.h>
#include <Thread.h>
#include <Audio.h>

#include <Appsymbols.h>




Audio_class				Audio				{ &Cfg };
Record_class			Record				{ &DaTA };
Appstate_class*			Appstate 			= &DaTA.Appstate;
Heap_Memory				mono_out			{ monobuffer_bytes }; // wave display data
Stereo_Memory<stereo_t>	stereo				{ max_frames * sizeof(stereo_t) };

Time_class				Timer				{};
ProgressBar_class		ProgressBar			{ &sds->RecCounter };
Wavedisplay_class		Wavedisplay			{ Sds_master };
AudioVolume_class		Volume				{ sds_master };
EventLog_class			Eventlog			{ &DaTA };



// runtime parameter
buffer_t 				ncounter 			= 0;
char 					shm_id 				= 0; // out_data = Shm_a
Stereo_t*				shm_addr 			= nullptr;

bool 					audio_out_done		= false;
buffer_t				audioframes			= audio_frames;

// ----------------------------------------------------------------
// RT Audio constant declarations





// end declaration section
// --------------------------------------------------------------------------------------------





#endif /* AUDIOSERVER_H_ */
