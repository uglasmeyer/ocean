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
 * mixer.h
 *
 *  Created on: Jan 17, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_MIXER_H_
#define INCLUDE_MIXER_H_

#include <Osc.h>
#include <Oscbase.h>
#include <External.h>
#include <Ocean.h>
#include <Wavedisplay.h>
#include <Mixerbase.h>
#include <data/DataWorld.h>
#include <data/Memory.h>
#include <Dynamic.h>
#include <notes/Notes.h>

class Mixer_class :
	virtual public 			Logfacility_class
{
	string 					className		= "";
	int 					beat_clock 		= 0;

	// provides and manages memory array
public:

	StorageArray_t 			StA				;
	Dataworld_class* 		DaTA			;
	mixer_state_t  			state 			= mixer_state_struct();
	int						composer		= 0;		// note chunk counter
	Heap_Memory				Mono			{ monobuffer_bytes };
	Heap_Memory 			Mono_out		{ monobuffer_bytes };
	Stereo_Memory<Stereo_t>	Out				{ Stereo_Memory<Stereo_t>::stereobuffer_bytes };
	Dynamic_class			DynVolume		{ volidx_range };
	Wavedisplay_class*		Wd_p			;
	interface_t*	 		sds				= nullptr;
	interface_t*			sds_master		= nullptr;
	sta_role_map			sta_rolemap		= sta_role_map();

							Mixer_class 	( Dataworld_class* 	data,
											Wavedisplay_class* 	wd );
	virtual 				~Mixer_class	();

	void	 				Add_Sound		(  Data_t* , Data_t*, Data_t*, Stereo_t*  );
	void 					ResetStA		( const StAId_e& id );
	void 					Set_play_mode	( const StAId_e& id, const bool& play );
	void 					SetStAs			();
	void 					SetStAProperties( StAId_e mixerId );
	void	 				StA_Wdcursor	();
	void 					BeatClock		( const uint8_t& bps );
	void 					Set_staVolume	( const StAId_e& id, uint8_t vol );


	void 					Add_mono		( Data_t*, const uint& staId );
	void 					Add_stereo		( Stereo_t* Data  );
	void 					TestMixer		();

private:

	void 					clear_memory	();
	void 					auto_volume		( const StAId_e& id);
	bool 					setFillState	( StAId_e id );
	void 					dumpStA			( Storage_class& sta );
	bool 					restoreStA		( Storage_class& sta );

};

/**************************************************
 * Cutter_class
 *************************************************/
class Cutter_class :
	virtual public 			Logfacility_class,
	virtual					sta_role_map,
	virtual public 			wavedisplay_struct
{
	string 					className 		= "";
	StorageArray_t*			StA				= nullptr;
	interface_t*			sds				= nullptr;
	Wavedisplay_class*		Wd				= nullptr;
	string					StAName 		= "";
	range_T<buffer_t>		record_range	= { 0, 0, 0 };
	range_T<buffer_t>		fillrange		= { 0, 0, 0 };
	const uint				step_records	= measure_parts*min_frames; // one second max_frames/min_frames

public:
	StAId_e					StAId;
	bool					setup_done		= false;
	Data_t*					cut_data		= nullptr;
	buffer_t				cut_bytes		= 0;

							Cutter_class	( Mixer_class* Mixer );
	virtual 				~Cutter_class	();
	void 					CursorUpdate	();
	void 					Cut				();
	void 					Display			();
	void 					Setup			();
	void 					Restore			();

private:
	bool 					setStAId		();

};

#endif /* INCLUDE_MIXER_H_ */
