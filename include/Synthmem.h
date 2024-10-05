/*
 * sharedmem.h
 *
 *  Created on: Dec 15, 2023
 *      Author: sirius
 */

#ifndef SYNTHSHM_H_
#define SYNTHSHM_H_

using namespace std;

#include <Ocean.h>
#include <Logfacility.h>
#include <data/Shmbase.h>


typedef struct mem_data_struct
{
	string			name 			= "memory";
	void*			addr			= nullptr;
	buffer_t		size			= 0;
	buffer_t 		mem_bytes		= 0;
	const buffer_t 	block_size 		= max_frames;
	uint 			sizeof_data 	= 0;
	buffer_t 		data_blocks		= 0;
	uint 			max_records		= 0;
//	uint 			record_bytes 	= 0;
//	uint 			record_counter	= 0;
} mem_ds_t;


class Memory_base : public virtual Logfacility_class
{
	string className = "Memory_base";
public:
	mem_ds_t	ds	= mem_data_struct();

	void 	Info();
	void* 	Init_void();
	void 	Gen_ds( size_t ds_size);

	Memory_base( buffer_t size );
	Memory_base() ;
	virtual ~Memory_base();

};

class Memory : virtual public Logfacility_class, virtual public Memory_base
{
	string className = "Memory";
public:
	Data_t* 	Data = nullptr;;
	Memory( buffer_t size ) :
		Logfacility_class( "Memory" ),
		Memory_base( size )
	{
		Init_data();
	};
	Memory( ) :
		Logfacility_class( "Memory" )
	{
		Comment( INFO, "pre-init " + className );
	};
	virtual ~Memory( )
	{};

	void Init_data(  );
	void Clear_data( Data_t );
	void Info( string );
};

typedef struct stereo_struct
{
	data_t left;
	data_t right;
} stereo_t;

const buffer_t		stereobuffer_size 	= recduration*frames_per_sec * sizeof(stereo_t);
const buffer_t 		sharedbuffer_size 	= max_frames * sizeof(stereo_t );

class Stereo_Memory : virtual public Logfacility_class, public virtual Memory_base
{
	string className = "Stereo_Memory";
public:
	stereo_t* stereo_data = nullptr;

	Stereo_Memory(buffer_t size) :
		Logfacility_class( ""),
		Memory_base( size )
	{
		init_data();
	}
	virtual ~Stereo_Memory( )
	{};

	void Clear_data(  );
	void Info( string );

private:
	void init_data(  );

};

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

class Storage_class :  virtual public Logfacility_class, virtual public Memory
{
	string className = "Storage_class";
public:
	// dynamic properties
//	uint 			max_counter 	= 0;
	uint8_t 		Amp				= osc_default_volume; // same as in GUI application
	StA_struct_t 	StAparam		= StA_struct();
	string 			Name			= "";
	uint8_t 		Id				= 0xFF;
	uint 			record_data		= 0;

	StA_status_t state = StA_status_struct();

	void 	Store_block( Data_t* ) ;
	Data_t* Get_next_block();
	string 	Record_mode( bool );
	string 	Play_mode( bool );
	void	Playnotes( bool );
	void 	Setup( StA_struct_t);
	void 	Set_store_counter( uint n);
	void 	Reset_counter();
	uint*	Get_storeCounter_p();

	Storage_class( ) :
		Logfacility_class( "" ),
		Memory()
	{} ;
	virtual ~Storage_class()
	{};

private:

	uint read_counter 	= 0;
	uint record_counter	= 0;

	Data_t* get_block( uint );

};


class Shared_Memory : virtual public Logfacility_class, virtual Shm_base
{

public:
	stereo_t* 	addr 		= nullptr;

	Shared_Memory( buffer_t size );
	virtual ~Shared_Memory();

	void Stereo_buffer( key_t key );
	void Clear();


};




#endif /* SYNTHSHM_H_ */
