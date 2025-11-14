/*
 * mixer.h
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
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
	void 				Update_sds_state( int id, interface_t* sds );
	void 				Set_play_mode	( const uint& id, const bool& play );
	void 				SetStA			();
	void 				SetStA			( Id_t mixerId );
	void 				Set_Wdcursor	();


	void 				TestMixer		();

private:
	void 				clear_memory	();
	void 				add_mono		( Data_t*, const uint& staId );
	void 				add_stereo		( Stereo_t* Data  );
};



#endif /* INCLUDE_MIXER_H_ */
