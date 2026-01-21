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
	int						composer		= 0;		// note chunk counter
	Heap_Memory				RecMono_mem			;
	Stereo_Memory<Stereo_t>	Out				;
	Dynamic_class			DynVolume		;
	Wavedisplay_class*		Wd_p			;
	interface_t*	 		sds				;
	interface_t*			sds_master		;
	sta_role_map			sta_rolemap		= sta_role_map();
	mixer_state_t  			state 			= mixer_state_struct();

							Mixer_class 	( Dataworld_class* 	data,
											Wavedisplay_class* 	wd );
	virtual 				~Mixer_class	();

	void	 				Add_Sound		(  Data_t* , Data_t*, Data_t*, Stereo_t*  );
	void 					ResetStA		( const StAId_e& id );
	void 					SetStAs			();
	void 					SetStAProperties( StAId_e mixerId );
	void	 				StA_Wdcursor	();
	void 					BeatClock		( const uint8_t& bps );


	void 					Add_mono		( Data_t*, const uint& staId );
	void 					Add_stereo		( Stereo_t* Data  );
	void 					TestMixer		();

private:

	void 					clear_temporary_memory	();
	void 					auto_volume		( const StAId_e& id);
	bool 					setFillState	( StAId_e id );
	void 					dumpStA			( Storage_class& sta );
	bool 					restoreStA		( Storage_class& sta );
	void 					set_play_mode	( const StAId_e& id, const bool& play );

};

/**************************************************
 * CutDesk_class
 *************************************************/
class CutDesk_class :
	virtual public 			Logfacility_class,
	virtual					sta_role_map,
	virtual public 			wavedisplay_struct
{
	record_range_t			record_range	;
	record_range_t			record_limits	;
	string 					className 		= "";
	Mixer_class*			Mixer			;
	Storage_class*			StA				;
	interface_t*			sds				;
	Wavedisplay_class*		Wd				;
	string					StAName 		;
	uint16_t				step_records	;

public:
	StAId_e					StAId			;
	range_T<buffer_t>		restore_range	;

							CutDesk_class	( Mixer_class* Mixer );
	virtual 				~CutDesk_class	();
	void 					CursorUpdate	();
	void 					Cut				();
	void 					Display			();
	void 					Setup			();
	void 					Restore			();

private:
	bool 					setStAId		();
	void 					reset			();
};

#endif /* INCLUDE_MIXER_H_ */
