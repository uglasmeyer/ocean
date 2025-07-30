/*
 * Shmbase.h
 *
 *  Created on: Oct 2, 2024
 *      Author: sirius
 */

#ifndef DATA_MEMORYBASE_H_
#define DATA_MEMORYBASE_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <System.h>
#include <data/Statistic.h>
/*
 *
 */

typedef struct 		StA_param_struct
{
	string 			name 			= "";
	buffer_t		size 			= 0;	// number of buffer frames
	buffer_t		block_size		= 0;  	// numer of read frames
} 	StA_param_t;

struct 				StA_state_struct // memory array status
{
	// SDS structure
	bool 			play			= false; // play this memory array
	bool 			store			= false; // record into this memory array
	bool			filled			= false; // there record counter is > 0
	bool			forget			= false; // delete after read
};
typedef				StA_state_struct StA_status_t;

struct 				shm_data_struct
{
	uint			Id				= 0;		// interface id in the set of shm_data_structures
	bool			eexist			= false;	// becomes true if attached
	buffer_t 		bytes			= 0;		// in bytes
	int				sizeof_type		= sizeof( Stereo_t );
	key_t			key				= 0;		// shm key
	int 			shmid			= -1;		// shmid
	void* 			addr			= nullptr;
	string			hex				= "0x0";
};
typedef				shm_data_struct shm_ds_t;


class 				Shm_base :
	virtual public 	Logfacility_class
{
	string 			className 		= "";
public:
	shm_ds_t		shm_ds			= shm_data_struct();

	shm_ds_t* 		Get				( key_t key );
	void 			ShowDs			( shm_ds_t );
	void* 			Attach			( int id );
	void 			Detach			( void* );
	void 			Delete			( ); // SDSview only

	void			Test_Memory		();

					Shm_base		( buffer_t size );
	virtual 		~Shm_base		();

private:

};

struct 				mem_data_struct
{
	string			name 			= "memory";
	void*			addr			= nullptr;
	string			hex				= "0x0";
	uint 			sizeof_type 	= sizeof( Data_t );
	buffer_t 		size 			= max_frames; // define a block as a substructue on the memory data
	buffer_t		bytes			= size * sizeof_type;
	buffer_t		block_size		= min_frames;
	buffer_t		block_bytes		= block_size * sizeof_type;
	buffer_t 		data_blocks		= size / block_size;
	uint 			max_records		= data_blocks; // data_block == max-records // TODO verify
};
typedef				mem_data_struct	mem_ds_t;

class 				Memory_base :
	public virtual 	Logfacility_class
{
	string 			className 		= "";
public:
	mem_ds_t		mem_ds			= mem_data_struct();

	void 			DsInfo			();
	void* 			Init_void		();
	void 			SetDs			( size_t type_bytes );
	mem_ds_t* 		GetDs			();

					Memory_base		( buffer_t bytes );
					Memory_base		();
	virtual 		~Memory_base	();

};

#endif /* DATA_MEMORYBASE_H_ */
