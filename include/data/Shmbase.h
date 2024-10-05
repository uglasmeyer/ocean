/*
 * Shmbase.h
 *
 *  Created on: Oct 2, 2024
 *      Author: sirius
 */

#ifndef DATA_SHMBASE_H_
#define DATA_SHMBASE_H_

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
	int 		shmid		= -1;	// sjmid
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

	Shm_base( buffer_t size );
	virtual ~Shm_base();

private:

};

#endif /* DATA_SHMBASE_H_ */
