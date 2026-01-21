/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * Shmbase.cpp
 *
 *  Created on: Oct 2, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <data/Memorybase.h>
#include <System.h>

/**************************************************
 * Memory_base
 *************************************************/
Memory_base::Memory_base( uint8_t type_bytes, buffer_t bytes ) :
	Logfacility_class( "Memory_base" )
	, mem_ds( sizeof_Data, bytes )
{
//	mem_ds.bytes 	= bytes;
	className 		= Logfacility_class::className;
	Init_data( bytes);
	Comment( DEBUG, "init memory size ", bytes );
};

Memory_base::~Memory_base()
{
	DESTRUCTOR( className )
};
void Memory_base::SetDs( size_t type_size )
{
	// terminology :
	// sizeof_data	-> 	data_bytes	= sizeof_unit)
	//					block_bytes = data_bytes*units
	//					mem_bytes 	= block_bytes*blocks (blocks=sec)
	//					mem_blocks	=

	buffer_t 	bytes 				= mem_ds.sizeof_type * mem_ds.max_records * mem_ds.record_size;
	Assert_equal( mem_ds.bytes, bytes );
}

mem_ds_t* Memory_base::GetDs()
{
	return &mem_ds;
}

void Memory_base::Init_data( buffer_t _bytes )
{
	Data = ( Data_t* ) Init_void( sizeof_Data, _bytes );

//	SetDs( sizeof_Data );
	Assert_equal( mem_ds.bytes, mem_ds.sizeof_type * mem_ds.max_records * mem_ds.record_size );
	statistic.data += mem_ds.bytes;
}

#include <Table.h>
void* Memory_base::Init_void( uint8_t typesize, buffer_t bytes )
{
	assert( bytes > 0 );
	mem_ds = mem_data_struct( typesize, bytes );
	mem_ds.addr = mmap(	NULL,
				bytes ,// + 1  ,
				PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS,
				0, 0);
	mem_ds.name			= Logfacility_class::className + " allocated";
	mem_ds.hex			= to_hex( (long) mem_ds.addr );
//	mem_ds.bytes 		= bytes;

	return mem_ds.addr;
}
void Memory_base::DsInfo( string name )
{
	Table_class T {};
	T.AddColumn( "Data type", 20 );
	T.AddColumn( "Value", 10 );
	if( LogMask[DEBUG] )
	{
		T.PrintHeader();
		T.AddRow( "Name", name );
		T.AddRow( "Memory bytes", mem_ds.bytes );
		T.AddRow( "Addr", to_hex(( long)mem_ds.addr) );
	}
	if( LogMask[TEST] )
	{
		T.AddRow( "Block bytes", mem_ds.sizeof_type );
		T.AddRow( "Record size", mem_ds.record_size );
		T.AddRow( "data blocks", mem_ds.data_blocks );
		T.AddRow( "max data records", mem_ds.max_records );
	}
}
void Memory_base::Clear_data( Data_t value )
{
	for ( buffer_t n = 0; n < mem_ds.data_blocks; n++ )
	{
		Data[n] = value;
	}
}

/**************************************************
 * Shm_base
 * Interface SDS
 *************************************************/
Shm_base::Shm_base( int type_size, buffer_t bytes, key_t key ) :
	Logfacility_class( "Shm_base" ),
	shm_ds( type_size, bytes, key )
{
	className			= Logfacility_class::className;
	if( key > 0 )
	{
		Get(key );
		Comment( DEBUG, "pre-init shared memory bytes: " , bytes );
	}
};

Shm_base::~Shm_base()
{
	DESTRUCTOR( className )
}

shm_ds_t* Shm_base::Get( key_t key )
{
	assert( shm_ds.bytes > 0 );
	shm_ds.key 	= key;

	// shm segment exists?
	shmget( shm_ds.key, shm_ds.bytes , S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL );
	shm_ds.eexist = ( EEXIST == errno );
	errno = 0;

	shm_ds.shmid = shmget ( shm_ds.key, shm_ds.bytes, S_IRUSR | S_IWUSR | IPC_CREAT );

	if ( shm_ds.shmid < 0 )
	{
		Comment( ERROR, Error_text( errno ));
		Set_Loglevel( DEBUG, true );
		ShowDs( shm_ds );
		Exception("cannot get shared memory" + to_string( shm_ds.shmid ), SIGILL );
	}

	shm_ds.addr 	= Attach( shm_ds.shmid );
	statistic.shm 	+= shm_ds.bytes;
	shm_ds.hex 		= to_hex( (long) shm_ds.addr );

	return &shm_ds;
}

void Shm_base::Clear()
{
	if ( not shm_ds.addr )
	{
		ShowDs( shm_ds );
		Exception( "shm undefined");
	}
	uint8_t* addr = (uint8_t*) shm_ds.addr;
	for ( buffer_t n = 0; n < shm_ds.bytes ; n++ )
	{
		addr[n] = 0;
	}
}

void* Shm_base::Attach( int id )
{
	Info( "attaching to id:", int( id ));
	void* addr = shmat( id, nullptr, SHM_RND );
	if ( addr == (void*)-1 )
	{
		Comment( ERROR, Error_text( errno ) );
		errno = 0;
		return nullptr;
	}
	return addr;
}

void Shm_base::ShowDs( shm_ds_t ds )
{
	stringstream strs;
	strs << SETW << "Shared Memory data structure " + className << endl;
	strs << SETW << "Id    : " << dec << ds.Id << endl;
	strs << SETW << "Addr  : " << hex << ds.addr << endl;
	strs << SETW << "shmid : " << dec << ds.shmid   << endl;
	strs << SETW << "Key   : " << dec << ds.key << endl;	;
	strs << SETW << "Bytes : " << dec << ds.bytes << endl;
	strs << SETW << "blocks: " << dec << ds.bytes/ds.sizeof_type << endl;
	strs << SETW << "Exist: "  << boolalpha <<  ds.eexist  << endl;
	Comment( (DEBUG), strs.str() );
}

void Shm_base::Detach( void* addr )
{

	Info( "Detach shared memory id:", int(shm_ds.shmid ) );

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

	Shm_base shm1{ sizeof(int), 1000, 100 };
	shm1.ShowDs( shm1.shm_ds);

	Shm_base shm2{ sizeof(int), 1000, 200 };
	shm2.ShowDs( shm2.shm_ds);

	assert( shm1.shm_ds.addr != shm2.shm_ds.addr );

	shm1.Delete();
	shm2.Delete();

	Assert_equal( errno, 0 );
	TEST_END( className );
}
