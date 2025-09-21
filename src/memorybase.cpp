/*
 * Shmbase.cpp
 *
 *  Created on: Oct 2, 2024
 *      Author: sirius
 */

#include <data/Memorybase.h>

Memory_base::Memory_base( buffer_t bytes ) :
	Logfacility_class( "Memory_base" )
{
	mem_ds.bytes = bytes;
	className = Logfacility_class::className;
	Info( "pre-init memory size ", bytes );
};

Memory_base::Memory_base() :
	Logfacility_class( "Memory_base" )
{
	className = Logfacility_class::className;
//	Comment( INFO, "pre-init memory size " + to_string( 0 ));
};

Memory_base::~Memory_base()
{
	DESTRUCTOR( className )
};


void* Memory_base::Init_void()
{
	assert( mem_ds.bytes > 0 );
	mem_ds.addr = mmap(	NULL,
				mem_ds.bytes ,// + 1  ,
				PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS,
				0, 0);
	mem_ds.name			= Logfacility_class::className + " allocated";
	mem_ds.hex			= to_hex( (long) mem_ds.addr );

	return mem_ds.addr;
}
void Memory_base::DsInfo()
{
	Comment( INFO, "Name             : " + mem_ds.name );
	Comment( INFO, "Memory bytes     : " + to_string( mem_ds.bytes ));
	Comment( TEST, "Addr             : " + to_hex(( long)mem_ds.addr) );
	Comment( TEST, "Structure bytes  : " + to_string( mem_ds.sizeof_type ));
	Comment( TEST, "Record size      : " + to_string( mem_ds.size ));
	Comment( TEST, "data blocks      : " + to_string( mem_ds.data_blocks ));
	Comment( TEST, "max data records : " + to_string( mem_ds.max_records ));

	cout << endl;

}



Shm_base::Shm_base( buffer_t bytes ) :
	Logfacility_class( "Shm_base" )
{
	shm_ds.bytes 		= bytes;
	className			= Logfacility_class::className;
	Info( "pre-init shared memory bytes: " , bytes );
}
Shm_base::Shm_base()
	: Logfacility_class( "Shm_base" )
{
	className			= Logfacility_class::className;
	Info( "Instanciate shared memory " );
}

Shm_base::~Shm_base()
{
}

shm_ds_t* Shm_base::Get( key_t key )
{
	assert( shm_ds.bytes > 0 );
	shm_ds.key 	= key;
	shmget( shm_ds.key, shm_ds.bytes , S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL );
	shm_ds.eexist = ( EEXIST == errno );


	shm_ds.shmid = shmget ( shm_ds.key, shm_ds.bytes, S_IRUSR | S_IWUSR | IPC_CREAT );

	if ( shm_ds.shmid < 0 )
	{
		Comment( ERROR, Error_text( errno ));
		ShowDs( shm_ds );
		EXCEPTION("cannot get shared memory" + to_string( shm_ds.shmid ) );
	}
	else
	{
		;//ds.eexist = true;
	}
	shm_ds.addr = Attach( shm_ds.shmid ); //shmat (ds.id, 0, 0);
	statistic.shm += shm_ds.bytes;
	shm_ds.hex = to_hex( (long) shm_ds.addr );

	return &shm_ds;
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
	strs << SETW << "Shared Memory data structure " + className << endl;
	strs << SETW << "Id    : " << dec << ds.Id << endl;
	strs << SETW << "Addr  : " << hex << ds.addr << endl;
	strs << SETW << "shmid : " << dec << ds.shmid   << endl;
	strs << SETW << "Key   : " << dec << ds.key << endl;	;
	strs << SETW << "Bytes : " << dec << ds.bytes << endl;
	strs << SETW << "blocks: " << dec << ds.bytes/ds.sizeof_type << endl;

	strs << SETW << "Exist: " << boolalpha <<  ds.eexist  << endl;
	Comment( INFO, strs.str() );
}

void Shm_base::Detach( void* addr )
{

	Comment( INFO , "Detach shared memory id: " + to_string( shm_ds.shmid ));

	if ( addr )
		shmdt( addr );

}

void Shm_base::Delete()
{
	shmid_ds dsbuf = shmid_ds();
	Comment( INFO , "Mark shared memory id: ", shm_ds.shmid, " to be destroyed" );

	if ( shm_ds.addr )
	{
		shmctl( shm_ds.shmid, IPC_RMID, &dsbuf );
		Info( "done, # attached: ", dsbuf.shm_nattch );
	}
	else
		Comment( ERROR, "Null " );
}

void Shm_base::Test_Memory()
{
	TEST_START( className );

	Shm_base shm1{1000};
	shm1.Get(100);
	shm1.ShowDs( shm1.shm_ds);

	Shm_base shm2{1000};
	shm2.Get(200);
	shm2.ShowDs( shm2.shm_ds);

	assert( shm1.shm_ds.addr != shm2.shm_ds.addr );

	shm1.Delete();
	shm2.Delete();

	TEST_END( className );
}
