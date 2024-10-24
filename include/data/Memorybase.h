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



typedef struct shm_data_struct
{
	uint		Id		= 0;		// interface id in the set of shm_data_structures
	bool		eexist	= false;	// becomes true if attached
	buffer_t 	size	= 0;		// in bytes
	key_t		key		= 0;		// shm key
	int 		shmid	= -1;		// sjmid
	void* 		addr	= nullptr;
} shm_ds_t;



class Shm_base : virtual Logfacility_class
{
	string className = "Shm_base";
public:
	shm_ds_t	ds	= shm_data_struct();

	shm_ds_t* 	Get( key_t key );
	void 		ShowDs( shm_ds_t );
	void* 		Attach( int id );
	void 		Detach( void* );

	void		Test_Memory();

	Shm_base( buffer_t size );
	virtual ~Shm_base();

private:

};

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
} mem_ds_t;


class Memory_base : public virtual Logfacility_class
{
	string className = "";
public:
	mem_ds_t	ds	= mem_data_struct();

	void 	Info();
	void* 	Init_void();
	void 	SetDs( size_t ds_size);

	Memory_base( buffer_t size );
	Memory_base() ;
	virtual ~Memory_base();

};

#endif /* DATA_MEMORYBASE_H_ */
