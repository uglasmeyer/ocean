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
#include <Wavedisplay.h>
#include <Mixerbase.h>
#include <data/DataWorld.h>

/**************************************************
 * Mixer_class
 * provides and manages Storage Area
 *************************************************/
class Mixer_class :
	virtual public 			Logfacility_class,
	public virtual			Interface_base
{
	int 					beat_clock 		= 0;
	sta_role_map			sta_rolemap		= sta_role_map();
	Heap_Memory				RecMono			;
	Stereo_Memory<Stereo_t>	Out				;

public:

	StorageArray_t 			StA				;
	int						composer_cntr	= 0;		// note chunk counter
	Wavedisplay_class*		Wd_p			;
	mixer_state_t  			state 			= mixer_state_data();

							Mixer_class 	( Dataworld_class* 	data,
											Wavedisplay_class* 	wd );
	virtual 				~Mixer_class	();

	void	 				Add_Sound		(  Data_t* , Data_t*, Data_t* );
	void 					ResetStA		( const StAId_e& id );
	void 					SetStAs			();
	void 					SetStAProperties( StAId_e mixerId );
	void	 				StA_Wdcursor	();
	bool 					BeatClock		( const uint8_t& bps );
	bool					Cutdesk_isActive();


	void 					Add_mono		( Data_t*, StAId_e staId );
	void 					AddStereo		();
	void 					DumpStA			( Storage_class& sta );
	void					SetStA_Files	( string dir );
	void 					TestMixer		();

private:

	void 					auto_volume		( const StAId_e& id);
	void					initStA_data	();
	bool 					restoreStA		( Storage_class& sta );

};

/**************************************************
 * CutDesk_class
 * manage the cut desk facility
 *************************************************/
class CutDesk_class :
	virtual public 			Logfacility_class,
	virtual	public			Interface_base,
	virtual					sta_role_map,
	virtual public 			wavedisplay_struct
{
	record_range_t			record_range	;
	record_range_t			record_limits	;
	Mixer_class*			Mixer			;
	Storage_class*			StA				;
	Wavedisplay_class*		Wd				;
	string					StAName 		;
	uint16_t				record_steps	;

public:
	StAId_e					StAId			;
	range_T<buffer_t>		restore_range	;
	WD_data_t				restore_wddata	;

							CutDesk_class	( Mixer_class* Mixer );
	virtual 				~CutDesk_class	();
	void 					CursorUpdate	();
	void 					Cut				();
	bool 					LoopFnc			();
	void 					Setup			();
	void 					Restore			();
	void					Rotate			( Direction_e direction );
	void					Set_record_steps( uint8_t width );

private:

	bool 					setStAId		();
};


#endif /* INCLUDE_MIXER_H_ */
