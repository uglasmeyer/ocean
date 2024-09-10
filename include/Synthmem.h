/*
 * sharedmem.h
 *
 *  Created on: Dec 15, 2023
 *      Author: sirius
 */

#ifndef SYNTHSHM_H_
#define SYNTHSHM_H_

using namespace std;

#include <Synthesizer.h>

// terminology :
// sizeof_data	-> 	data_bytes	= sizeof(unit)
//					block_bytes = data_bytes*units
//					mem_bytes 	= block_bytes*blocks (blocks=sec)
//					mem_blocks	=

typedef struct info_struct
{
	string		name 			= "memory";
	string		addr			= "0x0";
	buffer_t 	mem_bytes		= 0;
	buffer_t 	block_size 		= max_frames;
	uint 		sizeof_data 	= 0;
	buffer_t 	data_blocks		= 0;
	uint 		max_records		= 0;
	uint 		record_bytes 	= 0;
	uint 		record_counter	= 0;

} mem_info_t;


class Memory : virtual public Logfacility_class
{
public:
	Data_t* 	Data{};
	mem_info_t 	info;
	Memory() : Logfacility_class("Memory"){}
	Memory( buffer_t size ) : Logfacility_class("Memory")
	{
		init_data( size );
	}
	virtual ~Memory( ){}

	void clear_data( Data_t );
	void init_data( buffer_t );
	void Info( string );
	void Info();
};

typedef struct stereo_struct
{
	data_t left;
	data_t right;
} stereo_t;

const buffer_t		stereobuffer_size 	= recduration*frames_per_sec * sizeof(stereo_t);
const buffer_t 		sharedbuffer_size 	= max_frames * sizeof(stereo_t );

class Stereo_Memory : virtual public Logfacility_class
{
public:
	stereo_t* stereo_data {};
	mem_info_t info;
	Stereo_Memory(buffer_t size) : Logfacility_class("Stereo Memory")
	{
		init_data(size);
	}
	virtual ~Stereo_Memory( ){};

	void clear_data(  );
	void init_data( buffer_t );
	void Info( string );
	void Info();
};



namespace Storage{

typedef struct StA_struct
{
	string 		name = "";
	buffer_t	size = max_frames;
} StA_struct_t;

typedef struct StA_status_struct // memory array status
{
	bool 	play			= false; // play this memory array
	bool 	store			= false; // record into this memory array
} StA_status_t;

class Storage_class :  public Memory, virtual public Logfacility_class
{
public:
	// dynamic properties
	uint 			max_counter 	= 0;
	uint8_t 		Amp				= osc_default_volume; // same as in GUI application
	const buffer_t 	block_size 		= max_frames;
	StA_struct_t 	StAparam		= StA_struct();
	string 			Name			= "";
	uint8_t 		Id				= 0xFF;

	StA_status_t state = StA_status_struct();


	Storage_class( ) : Logfacility_class("Storage") {} ;
	virtual ~Storage_class(){};
	void 	Store_block( Data_t* ) ;
	Data_t* Get_next_block();
	string 	Record_mode( bool );
	string 	Play_mode( bool );
	void	Playnotes( bool );
	void 	Setup( StA_struct_t);
	void 	Set_store_counter( uint n);
	void 	Reset_counter();
	uint*	Get_storeCounter_p();

private:

	uint read_counter 	= 0;
	uint store_counter 	= 0;

	Data_t* get_block( uint );
	void	pause();
}; // Storage_class
} // namespace Storage


class Shared_Memory : virtual public Logfacility_class
{

public:
	// https://stackoverflow.com/questions/7237540/how-to-use-shared-memory-to-communicate-between-two-processes
	stereo_t* 	addr 		= NULL;		// = buffer( buffer_size);
	Shared_Memory() : Logfacility_class("Shm") {};
	virtual ~Shared_Memory();

	void buffer( buffer_t, key_t );
	void info();
	void clear();


private:
	typedef struct shm_info_struct
	{
		buffer_t 	size;
		key_t		key;
		int 		id;
		void* 		addr;
	} shm_info_t;
	shm_info_t 		shm_info;
};




#endif /* SYNTHSHM_H_ */
