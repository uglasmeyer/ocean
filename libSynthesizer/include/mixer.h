/*
 * mixer.h
 *
 *  Created on: Jan 17, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_MIXER_H_
#define INCLUDE_MIXER_H_



#include <bits/stdint-uintn.h>
#include <synthesizer.h>
#include <sys/mman.h>
#include <string>
#include <osc.h>
#include <Instrument.h>
#include <synthmem.h>
#include <External.h>
#include <notes.h>
#include <wav.h>

using namespace std;

static const uint 	MbSize 			= 8;
static const uint 	MbIdExternal 	= MbSize - 1;
static const uint 	MbIdNotes 		= MbSize - 2;


class Loop_class
{
public:
	uint16_t beg=0, end=0, counter=0, counts=0, max_counts=0;
	int8_t step = 0;
	bool active = false;
	Loop_class(){};
	~Loop_class(){};

	void start( uint16_t beg, uint16_t end, uint8_t step );
	void next( uint16_t* );
	void next( char*  );
	void test();

};

using namespace Storage;
class Mixer_class : virtual public Logfacility_class
{
	// provides and manages memory array
public:

	const vector<uint>  MemIds 		= {0, 1, 2, 3, 4, 5,  MbIdExternal, MbIdNotes };
	const vector<uint>  RecIds 		= {0, 1, 2, 3, 4, 5,  MbIdExternal };
	typedef vector<Storage_class> StorageArray_t;
	StorageArray_t 		StA;
	Stereo_Memory* 		stereo {};
	uint8_t				master_volume	= 100;

	Mixer_class ( Stereo_Memory*  );
	~Mixer_class(){};

	Memory 				Mono_tmp{ monobuffer_size }; // Wavedisplay output
	Memory 				Mono_out{ monobuffer_size }; // Wavedisplay output
	Memory 				Out_L{ monobuffer_size };// Output buffer long
	Memory				Out_R{ monobuffer_size };//
	mi_status_t  status =
	{
		.play 		= true, // The keyboard can manipulate the
										// frequency of the MAIN osc,
		.notes		= false, 	// set Notes on/off
		.external 	= false, 	// set external wav file input on/off
		.mute		= false,	// add main osc to output volume / mute-unmute
		.unused		= false	// OSC is part of the mix stack
	};
	int				composer		= 0;		// note chunk counter

	void add_noteline( uint8_t, Note_class* );
	void add(  Instrument_class* , stereo_t*, bool  );
	void test();

private:
	void clear_memory();
	void add_mono( Data_t*, uint8_t, uint );
	void stereo_out( stereo_t*, uint8_t );
	void add_record( stereo_t*, uint8_t );


};



#endif /* INCLUDE_MIXER_H_ */
