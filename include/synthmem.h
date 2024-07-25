/*
 * sharedmem.h
 *
 *  Created on: Dec 15, 2023
 *      Author: sirius
 */

#ifndef SYNTHSHM_H_
#define SYNTHSHM_H_

using namespace std;

#include <synthesizer.h>
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
	char 		mode = 'c';
/* mode:
* a : ring buffer: max size 30 seconds. write until stop or buffer is full.
* play until stop or store cursor reached.
* set read cursor to start after stop.
*
* s : file record: play until store counter or buffer is full.
* write until stop or buffer end
*
* c : notes style: store block, play block
* set cursor to start with next_block
*
*/
} StA_struct_t;

class Storage_class :  public Memory, virtual public  Logfacility_class
{
public:
	// dynamic properties
	uint 			max_counter 	= 0;
	uint8_t 		Amp				= 100; // same as in GUI application
	const buffer_t 	block_size 		= max_frames;
	StA_struct_t 	StAparam		= StA_struct();
	string 			Name			= "";
	uint8_t 		Id				= 0xFF;

	Storage_class( ) : Logfacility_class("Storage") {} ;
	virtual ~Storage_class(){};


	ma_status_t status =
	{
		.play 	= false,
		.store	= false
	} ;


	uint read_counter 	= 0;
	uint store_counter 	= 0;

	void 	store_block( Data_t* ) ;
	Data_t* get_next_block();
	string 	record_mode( bool );
	string 	play_mode( bool );
	void 	mute();
	void	playnotes( bool );
	void 	setup( StA_struct_t);
	void 	set_store_counter( uint n);
//	void 	set_read_counter( uint n);
	void 	reset_counter();

private:

	Data_t* get_block( uint );
	void	pause();


};
}


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
