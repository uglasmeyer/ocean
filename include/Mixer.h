/*
 * mixer.h
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_MIXER_H_
#define INCLUDE_MIXER_H_

#include <Osc.h>
#include <External.h>
#include <Ocean.h>
#include <Mixerbase.h>
#include <Notes.h>
#include <data/Memory.h>

using namespace std;



class Loop_class
{
public:
	uint16_t beg=0, end=0, counter=0, counts=0, max_counts=0;
	int8_t step = 0;
	bool active = false;

	Loop_class(){};
	~Loop_class(){};

	void Start( uint16_t beg, uint16_t end, uint8_t step );
	void Next( uint16_t& );
	void Next( uint8_t&  );
	void Test();

};

static const uint 	MbIdExternal 	= MbSize - 1;
static const uint 	MbIdNotes 		= MbSize - 2;
static const uint 	MbIdKeyboard 	= MbSize - 3;
static const uint   MbIdInstrument	= MbSize - 4;

class Mixer_class : virtual public Logfacility_class,
					virtual public Mixer_base
{
	// provides and manages memory array
public:

	const vector<uint>  MemIds 		= {0, 1, 2, 3, 4, 5, 6, MbIdExternal };
	const vector<uint>  RecIds 		= {0, 1, 2, 3, 4, 5, 6, MbIdExternal };
	const vector<uint>  UsrIds		= {0, 1, 2, 3, 4, 5, 6 };
	const vector<uint>	SycIds		= {0, 1, 2, 3, 6, 7 };
	const vector<uint>	HghIds		= {4, 5, 6, 7 };


	typedef vector<Storage_class>
						StorageArray_t;

	StorageArray_t 		StA;
	uint8_t				master_volume	= 100;
	mixer_status_t  	status 			= mixer_status_struct();
	int					composer		= 0;		// note chunk counter

	Memory 				Mono	{ monobuffer_size }; 	// Wavedisplay output
	Memory 				Mono_out{ monobuffer_size };// Wavedisplay output
	Memory 				Out_L	{ monobuffer_size };// Output buffer long
	Memory				Out_R	{ monobuffer_size };//

	Loop_class 			master_amp_loop;
	Loop_class			record_amp_loop;

	interface_t* 		sds				= nullptr;				;
	Mixer_class ( interface_t* sds );
	~Mixer_class();

	void Setup( interface_t* sds );
	void Store_noteline( uint8_t, Note_class* );
	void Add_Sound(  Data_t* , Data_t*, Data_t*, stereo_t*  );
	void Clear_StA_status( StA_state_arr_t& );
	void Update_ifd_status_flags( interface_t* sds );
	void Volume_control( interface_t* sds );
	void Set_mixer_state( const uint& id, const bool& play );

	void Test();

private:
	void clear_memory();
	void add_mono( Data_t*, const uint8_t&, const uint& );
	void stereo_out( stereo_t*, const uint8_t& );


};



#endif /* INCLUDE_MIXER_H_ */
