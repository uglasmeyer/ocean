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
#include <data/Memory.h>
#include <data/DataWorld.h>
#include <Dynamic.h>
#include <notes/Notes.h>



class Mixer_class :
	virtual public Logfacility_class,
	virtual public Mixer_base
{
	string className = "";
	Dataworld_class* 	DaTA;

	// provides and manages memory array
public:

	const vector<uint>  MemIds 		= {0, 1, 2, 3, 4, 5, 6, STA_EXTERNAL };
	const vector<uint>  RecIds 		= {0, 1, 2, 3, 6, STA_EXTERNAL };
	const vector<uint>  UsrIds		= {0, 1, 2, 3, 4, 5, 6 };
	const vector<uint>	SycIds		= { };
	const vector<uint>	HghIds		= {4, 5, 6, 7 };


	typedef vector<Storage_class>
						StorageArray_t;
	StorageArray_t 		StA;

	mixer_status_t  	status 			= mixer_status_struct();
	int					composer		= 0;		// note chunk counter

	Memory 				Mono			{ monobuffer_bytes };
	Memory 				Mono_out		{ monobuffer_bytes };
	Stereo_Memory<Stereo_t>
						Out				{ Stereo_Memory<Stereo_t>::stereobuffer_bytes };


	Dynamic_class		DynVolume		{ volidx_range };

	interface_t* 		sds				= nullptr;
	interface_t*		sds_master		= nullptr;
	Mixer_class ( 	Dataworld_class* 	data,
					Wavedisplay_class* 	wd );
	virtual ~Mixer_class();

	void Store_noteline( uint8_t, Note_class* );
	void Add_Sound(  Data_t* , Data_t*, Data_t*, stereo_t*  );
	void Clear_StA_status( StA_state_arr_t& );
	void Update_sds_state( interface_t* sds );
	void Set_mixer_state( const uint& id, const bool& play );
	bool GetSyncState();
	void SetStA();
	void TestMixer();

private:
	void clear_memory();
	void add_mono( Data_t*, const uint& staId );
	void add_stereo( stereo_t* data  );
};



#endif /* INCLUDE_MIXER_H_ */
