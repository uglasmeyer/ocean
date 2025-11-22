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
	virtual public 		Logfacility_class
{
	string 				className		= "";
	Dataworld_class* 	DaTA			;
	Wavedisplay_class*	Wd_p			;
	sta_role_map		sta_rolemap		= sta_role_map();
	// provides and manages memory array
public:

	const vector<STAID_e>	AllIds		= Iota_T<STAID_e>( STA_USER00, STA_SIZE );
	const vector<STAID_e>	RecIds 		= {STA_USER00, STA_USER01, STA_USER02, STA_USER03, STA_EXTERNAL };
	const vector<STAID_e>	UsrIds		= {STA_USER00, STA_USER01, STA_USER02, STA_USER03, STA_INSTRUMENT };
	const vector<uint>	HghIds			= {4, 5, 6, 7 };
	const set<uint>		LowIds			= {0, 1, 2, 3 };


	typedef vector<Storage_class>		StorageArray_t;
	StorageArray_t 		StA;

	mixer_state_t  		state 			= mixer_state_struct();
	int					composer		= 0;		// note chunk counter

	Heap_Memory 		Mono			{ monobuffer_bytes };
	Heap_Memory 		Mono_out		{ monobuffer_bytes };
	Stereo_Memory<Stereo_t>
						Out				{ Stereo_Memory<Stereo_t>::stereobuffer_bytes };


	Dynamic_class		DynVolume		{ volidx_range };

	interface_t* 		sds				= nullptr;
	interface_t*		sds_master		= nullptr;
						Mixer_class 	( Dataworld_class* 	data,
										Wavedisplay_class* 	wd );
	virtual 			~Mixer_class	();

//	void Store_noteline( uint8_t, Note_class* );
	void 				Add_Sound		(  Data_t* , Data_t*, Data_t*, Stereo_t*  );
	void 				Clear_StA_status( StA_state_arr_t& );
	void 				Set_play_mode	( const STAID_e& id, const bool& play );
	void 				SetStAs			();
	void 				SetStA			( STAID_e mixerId );
	void 				Set_Wdcursor	();


	void 				TestMixer		();

private:
	void 				clear_memory	();
	void 				add_mono		( Data_t*, const uint& staId );
	void 				add_stereo		( Stereo_t* Data  );
};



#endif /* INCLUDE_MIXER_H_ */
