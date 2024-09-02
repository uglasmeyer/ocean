/*
 * mixer.h
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_MIXER_H_
#define INCLUDE_MIXER_H_

//#include <bits/stdint-uintn.h>
#include <osc.h>
#include <Instrument.h>
#include <synthmem.h>
#include <External.h>
#include <notes.h>
#include <Synthesizer.h>
#include <wav.h>

using namespace std;

static const uint 	MbIdExternal 	= MbSize - 1;
static const uint 	MbIdNotes 		= MbSize - 2;
static const uint	MbIdKeyboard	= MbSize - 3;


class Loop_class
{
public:
	uint16_t beg=0, end=0, counter=0, counts=0, max_counts=0;
	int8_t step = 0;
	bool active = false;

	Loop_class(){};
	~Loop_class(){};

	void Start( uint16_t beg, uint16_t end, uint8_t step );
	void Next( uint16_t* );
	void Next( uint8_t*  );
	void Test();

};

using namespace Storage;
class Mixer_class : virtual public Logfacility_class
{
	// provides and manages memory array
public:

	const vector<uint>  MemIds 		= {0, 1, 2, 3, 4, 5,  MbIdExternal, MbIdNotes };
	const vector<uint>  RecIds 		= {0, 1, 2, 3, 4, 5,  MbIdExternal };
	const vector<uint>  UsrIds		= {0, 1, 2, 3, 4 };


	typedef vector<Storage_class> StorageArray_t;
	StorageArray_t 		StA;
	uint8_t				master_volume	= 100;
	mixer_status_t  	status 			= mixer_status_struct();
	int					composer		= 0;		// note chunk counter

	Memory 				Mono_tmp{ monobuffer_size }; // Wavedisplay output
	Memory 				Mono_out{ monobuffer_size }; // Wavedisplay output
	Memory 				Out_L	{ monobuffer_size };// Output buffer long
	Memory				Out_R	{ monobuffer_size };//


	Mixer_class (  );
	~Mixer_class(){};


	void Store_noteline( uint8_t, Note_class* );
	void Add_Sound(  Instrument_class* , stereo_t*  );
	void Test();

private:
	void clear_memory();
	void add_mono( Data_t*, const uint8_t&, const uint& );
	void stereo_out( stereo_t*, const uint8_t& );


};



#endif /* INCLUDE_MIXER_H_ */
