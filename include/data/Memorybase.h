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
	buffer_t		size 			= 0;
	buffer_t		block_size		= 0;
} 	StA_param_t;

struct 				StA_state_struct // memory array status
{
	bool 			play			= false; // play this memory array
	bool 			store			= false; // record into this memory array
	bool			filled			= false;
};
typedef				StA_state_struct StA_status_t;

struct 				shm_data_struct
{
	uint			Id				= 0;		// interface id in the set of shm_data_structures
	bool			eexist			= false;	// becomes true if attached
	buffer_t 		bytes			= 0;		// in bytes
	key_t			key				= 0;		// shm key
	int 			shmid			= -1;		// sjmid
	void* 			addr			= nullptr;
	string			hex				= "0x0";
};
typedef				shm_data_struct shm_ds_t;


class 				Shm_base :
	virtual public 	Logfacility_class
{
public:
	string 			className 		= "";
	shm_ds_t		shm_ds			= shm_data_struct();

	shm_ds_t* 		Get				( key_t key );
	void 			ShowDs			( shm_ds_t );
	void* 			Attach			( int id );
	void 			Detach			( void* );

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

	void 			Info			();
	void* 			Init_void		();
	void 			SetDs			( size_t type_bytes, buffer_t bs = min_frames );
	mem_ds_t* 		GetDs			();

					Memory_base		( buffer_t size );
					Memory_base		();
	virtual 		~Memory_base	();

};

#endif /* DATA_MEMORYBASE_H_ */
