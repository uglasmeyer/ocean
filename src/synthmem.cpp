/*
 * synthshm.cpp
 *
 *  Created on: Jan 16, 2024
 *      Author: sirius
 */

#include <Synthmem.h>


//-----------------------------------------------------------------------------


void Memory::clear_data( Data_t value )
{
	for ( buffer_t n = 0; info.data_blocks > n; n++ )
	{
		Data[n] = value;
	}
}

void Memory::init_data( buffer_t size)
{
	info.name			= "Data Memory";
	info.mem_bytes		= size;
	info.sizeof_data 	= sizeof(Data_t);
	info.data_blocks 	= info.mem_bytes / info.sizeof_data;
	info.max_records	= info.data_blocks / info.block_size;
	info.record_bytes  	= info.block_size * info.sizeof_data;
	if ( not ( size - info.sizeof_data * info.max_records * info.block_size == 0 ))
		exit(1);

//	if ( Data == nullptr )
		Data = (Data_t*) mmap(	NULL,
								size  + 1  ,
								PROT_READ | PROT_WRITE,
								MAP_PRIVATE | MAP_ANONYMOUS,
								0, 0);
	String S{""};
	info.addr 			= S.to_hex( (long)Data );

}

void Memory::Info( string name )
{
	info.name = name;
	Info();
}

void Memory::Info(  )
{
	Comment( INFO, "Name             : " + info.name );
	Comment( INFO, "Memory bytes     : " + to_string( info.mem_bytes ));
	Comment( TEST, "Addr             : " + info.addr);
	Comment( TEST, "Structure bytes  : " + to_string( info.sizeof_data ));
	Comment( TEST, "Frame size       : " + to_string( info.block_size ));
	Comment( TEST, "data blocks      : " + to_string( info.data_blocks ));
	Comment( TEST, "Bytes/per record : " + to_string( info.record_bytes ));
	Comment( TEST, "max data records : " + to_string( info.max_records ));
	cout << endl;
}


//-----------------------------------------------------------------------------


void Stereo_Memory::init_data( buffer_t size)
{
	info.name			= "Audio Memory";
	info.mem_bytes		= size;
	info.sizeof_data 	= sizeof(stereo_t);
	info.data_blocks 	= info.mem_bytes / info.sizeof_data;
	info.max_records	= info.data_blocks / info.block_size;
	info.record_bytes  	= info.block_size * info.sizeof_data;
	if ( not ( size - info.sizeof_data * info.max_records * info.block_size == 0 ))
		exit(1);

	stereo_data 			= (stereo_t*) mmap(	NULL,
									size  + 1  ,
									PROT_READ | PROT_WRITE,
									MAP_PRIVATE | MAP_ANONYMOUS,
									0, 0);
	String S{""};
	info.addr 			= S.to_hex( (long)stereo_data );

}

void Stereo_Memory::clear_data()
{
	stereo_t stereo = { 0,0 };
	for ( buffer_t n = 0 ; n < info.data_blocks ; n++ )
	{
		stereo_data[n] = stereo;
	}

}

void Stereo_Memory::Info( string name )
{
	info.name = name;
	Info();
}

void Stereo_Memory::Info()
{
	Comment( INFO, "Name             : " + info.name );
	Comment( INFO, "Memory bytes     : " + to_string( info.mem_bytes ));
	Comment( TEST, "Addr             : " + info.addr);
	Comment( TEST, "Structure bytes  : " + to_string( info.sizeof_data ));
	Comment( TEST, "Record size      : " + to_string( info.block_size ));
	Comment( TEST, "data blocks      : " + to_string( info.data_blocks ));
	Comment( TEST, "Bytes/per record : " + to_string( info.record_bytes ));
	Comment( TEST, "max data records : " + to_string( info.max_records ));
	Comment( TEST, "record counter   : " + to_string( info.record_counter ));

	cout << endl;

}
//-----------------------------------------------------------------------------



using namespace Storage;

void Storage_class::Setup( StA_struct_t p )
{
	StAparam 		= p;
	max_counter 	= p.size/block_size;
	buffer_t Bytes 	= max_counter*sizeof(Data_t)*block_size;
	init_data( Bytes );
	info.name 		= p.name;
	clear_data(0);

}

void Storage_class::Store_block( Data_t* src )
{
	if ( not status.store ) return;
	if ( store_counter > info.max_records - 2 ) return;
	buffer_t start = store_counter* block_size;
	for ( buffer_t n = 0; n < block_size; n++ )
	{
		Data[start + n] = src[n];
	}
	store_counter 		= ( store_counter + 1 );
	if ( store_counter == info.max_records - 2 )
		status.store = false ;
}

Data_t* Storage_class::Get_next_block()
{
/*	cout << dec <<
			(int)Id <<
			": read " << status.read_counter <<
			" store " << status.store_counter <<
			" Amp " << (int)Amp << endl;
*/
	if ( not status.play ) return nullptr;
	if ( store_counter == 0 ) return nullptr;
	Data_t* ptr = get_block( read_counter );
	read_counter = ( read_counter + 1 ) % store_counter ;
	assert( read_counter <= store_counter );
	return ptr;
}

Data_t* Storage_class::get_block( uint id)
{
	return &Data[id * block_size];
}

void 	Storage_class::Set_store_counter( uint  n )
{
	assert( n < info.max_records );
	store_counter 	= n;
	read_counter  	= 0;
}

void 	Storage_class::Reset_counter( )
{
	store_counter 	= 0;
	read_counter  	= 0;
	status.play 	= false;
	status.store 	= false;
}

string 	Storage_class::Play_mode( bool flag )
{
	if ( store_counter == 0 ) // nothing to play
		status.play = false;
	else
		status.play = flag;
	return (status.play) ? "ON" : "OFF";
}

string Storage_class::Record_mode( bool flag )
{
	status.store = flag;
	if ( flag )
		read_counter = 0;
	return (status.store) ? "ON" : "OFF";
}

void Storage_class::Mute()
{
	Comment(INFO, "mute " + StAparam.name );
	status.play 	= false;
}


void Storage_class::Playnotes( bool flag )
{
	string onoff = flag ? " on" : " off";
	cout << "play " + StAparam.name + onoff << endl;
	Comment(INFO, "play " + StAparam.name + onoff );
	status.play = flag;
	status.store= false;
}


void Storage_class::pause()
{
;
}

//-----------------------------------------------------------------------------


Shared_Memory::~Shared_Memory()
{
	Comment( INFO, "detach SHM interface from id: " + to_string( shm_info.id));
	shmdt(shm_info.addr);
//	shmctl(shm_info.id, IPC_RMID, NULL);
}

void Shared_Memory::clear()
{
	for ( buffer_t n = 0; n < max_frames ; n++ )
	{
		addr[n] = {0,0};
	}
}

void Shared_Memory::buffer( buffer_t size, key_t key )
{
	  /* Allocate a shared memory segment. */
	int shmflg = S_IRUSR | S_IWUSR | IPC_CREAT;
	int shm_id = shmget ( key, size, shmflg );
	if ( shm_id < 0 )
	{
		Comment(ERROR, "cannot get shared memory");
		exit ( 1 );
	}
	addr = (stereo_t*) shmat (shm_id, 0, 0);
	shm_info = { size, key, shm_id, addr };

	return ;
};

void Shared_Memory::info()
{
	Comment( INFO, "Shared Memory info" );
	Comment( INFO, "Id  : " + to_string( shm_info.id   ) );
	Comment( INFO, "Key : " + to_string( shm_info.key  ) );
	Comment( INFO, "Size: " + to_string( shm_info.size ) );
}


