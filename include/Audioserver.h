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
