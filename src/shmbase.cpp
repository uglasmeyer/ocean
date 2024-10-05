/*
 * Shmbase.cpp
 *
 *  Created on: Oct 2, 2024
 *      Author: sirius
 */

#include <data/Shmbase.h>

Shm_base::Shm_base( buffer_t size ) :
Logfacility_class( "Shm_base" )
{
	ds.size = size;
}

Shm_base::~Shm_base()
{
	Detach( ds.addr );
//	Comment( INFO , "Detach shared memory id: " + to_string( ds.id ) );
//	shmdt( ds.addr );
}

shm_ds_t* Shm_base::Get( key_t key )
{
	assert( ds.size > 0 );
	ds.key 	= key;
	shmget( ds.key, ds.size , S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL );
	ds.eexist = ( EEXIST == errno );

	ds.shmid = shmget ( ds.key, ds.size, S_IRUSR | S_IWUSR | IPC_CREAT );
	statistic.shm += ds.size;

	if ( ds.shmid < 0 )
	{
		Comment( ERROR, Error_text( errno ));
		Exception("cannot get shared memory" + to_string( ds.shmid ) );
	}
	ds.addr = Attach( ds.shmid ); //shmat (ds.id, 0, 0);

	return &ds;
}

void* Shm_base::Attach( int id )
{
	Comment( INFO, "attaching to id: "  + to_string( id ));
	void* addr = shmat( id, 0, 0 );
	if ( addr == (void * )-1 )
	{
		Comment( ERROR, Error_text( errno ) );
		return nullptr;
	}
	return addr;
}

void Shm_base::ShowDs( shm_ds_t ds )
{
	stringstream strs;
	strs << setw(20) << "Shared Memory data structure"  << endl;
	strs << setw(20) << "Id   : " << dec << ds.Id << endl;
	strs << setw(20) << "Addr : " << hex << ds.addr << endl;
	strs << setw(20) << "shmid: " << dec << ds.shmid   << endl;
	strs << setw(20) << "Key  : " << dec << ds.key << endl;	;
	strs << setw(20) << "Size : " << dec << ds.size << endl;
	strs << setw(20) << "Exist: " << boolalpha <<  ds.eexist  << endl;
	Comment( INFO, strs.str() );
}

void Shm_base::Detach( void* addr )
{
	Comment( INFO , "Detach shared memory id: " + to_string( ds.shmid ));
	if ( addr == nullptr )
	{
		Comment( ERROR, "Cannot detache nullptr" );
		return;
	}
	shmdt( addr );
}
