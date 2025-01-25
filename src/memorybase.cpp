/*
 * Shmbase.cpp
 *
 *  Created on: Oct 2, 2024
 *      Author: sirius
 */

#include <data/Memorybase.h>

Memory_base::Memory_base( buffer_t size ) :
	Logfacility_class( "Memory_base" )
{
	ds.size = size;
	className = Logfacility_class::module;
	Comment( INFO, "pre-init memory size " + to_string( size ));

//	Log[ TEST ] = true;

};

Memory_base::Memory_base() :
	Logfacility_class( "Memory_base" )
{
	className = Logfacility_class::module;
	Comment( INFO, "pre-init memory size " + to_string( 0 ));
};

Memory_base::~Memory_base()
{};


void* Memory_base::Init_void()
{
	assert( ds.size > 0 );
	ds.addr = mmap(	NULL,
				ds.size  + 1  ,
				PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS,
				0, 0);
	ds.name			= Logfacility_class::module + " allocated";
	ds.mem_bytes	= ds.size;

	return ds.addr;
}
void Memory_base::Info()
{
	String S{""};
	Comment( INFO, "Name             : " + ds.name );
	Comment( INFO, "Memory bytes     : " + to_string( ds.mem_bytes ));
	Comment( TEST, "Addr             : " + S.to_hex(( long)ds.addr) );
	Comment( TEST, "Structure bytes  : " + to_string( ds.sizeof_data ));
	Comment( TEST, "Record size      : " + to_string( ds.block_size ));
	Comment( TEST, "data blocks      : " + to_string( ds.data_blocks ));
	Comment( TEST, "max data records : " + to_string( ds.max_records ));

	cout << endl;

}



Shm_base::Shm_base( buffer_t size ) :
Logfacility_class( "Shm_base" )
{
	ds.size = size;
	Comment( INFO, "pre-init shared memory size " + to_string( size ));

}

Shm_base::~Shm_base()
{
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
	void* addr = shmat( id, nullptr, SHM_RND );
	if ( addr == (void * )-1 )
	{
		Comment( ERROR, Error_text( errno ) );
		return nullptr;
	}
	return addr;
}

void Shm_base::ShowDs( shm_ds_t ds )
{
//	if ( not Log[ TEST ] ) return;
	stringstream strs;
	strs << SETW << "Shared Memory data structure"  << endl;
	strs << SETW << "Id   : " << dec << ds.Id << endl;
	strs << SETW << "Addr : " << hex << ds.addr << endl;
	strs << SETW << "shmid: " << dec << ds.shmid   << endl;
	strs << SETW << "Key  : " << dec << ds.key << endl;	;
	strs << SETW << "Size : " << dec << ds.size << endl;
	strs << SETW << "Exist: " << boolalpha <<  ds.eexist  << endl;
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

void Shm_base::Test_Memory()
{
	TEST_START( className );

	Shm_base shm1{1000};
	shm1.Get(100);
	shm1.ShowDs( shm1.ds);

	Shm_base shm2{1000};
	shm2.Get(200);
	shm2.ShowDs( shm2.ds);

	assert( shm1.ds.addr != shm2.ds.addr );


	TEST_END( className );
}
