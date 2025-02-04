/*
 * synthshm.cpp
 *
 *  Created on: Jan 16, 2024
 *      Author: sirius
 */

#include <data/Memory.h>
#include <String.h>
#include <System.h>

string className =  "Memory";
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


void Memory::Clear_data( Data_t value )
{
	for ( buffer_t n = 0; ds.data_blocks > n; n++ )
	{
		Data[n] = value;
	}
}
void Memory_base::SetDs( size_t data_size)
{
	// terminology :
	// sizeof_data	-> 	data_bytes	= sizeof(unit)
	//					block_bytes = data_bytes*units
	//					mem_bytes 	= block_bytes*blocks (blocks=sec)
	//					mem_blocks	=

	ds.mem_bytes	= ds.size;
	ds.sizeof_data 	= data_size;
	ds.data_blocks 	= ds.size / data_size;
	ds.max_records	= ds.data_blocks / ds.block_size;

	if ( not ( ds.size - ds.sizeof_data * ds.max_records * ds.block_size == 0 ))
		Exception( "init memory" );

}

mem_ds_t* Memory_base::GetDs()
{
	return &ds;
}

void Memory::Init_data( )
{
	Data = ( Data_t* ) Init_void();

	SetDs( sizeof( Data_t));
	statistic.data += ds.mem_bytes;
}

void Memory::Info( string name )
{
	ds.name = name;
	Memory_base::Info();
}


//-----------------------------------------------------------------------------


void Stereo_Memory::Init_data( buffer_t size )
{
	Memory_base::ds.size = size;
	stereo_data = ( stereo_t* ) Init_void();

	SetDs( sizeof_stereo );
	statistic.stereo += ds.mem_bytes;
	ds.name			= Logfacility_class::module;
}

void Stereo_Memory::Clear_data()
{
	stereo_t stereo = { 0,0 };
	for ( buffer_t n = 0 ; n < ds.data_blocks ; n++ )
		stereo_data[n] = stereo;

}

void Stereo_Memory::Info( string name )
{
	ds.name = name;
	Memory_base::Info();
}

//-----------------------------------------------------------------------------


void Storage_class::Setup( StA_struct_t param )
{
	StAparam 		= param;
	//ds.max_records	= param.size / ds.block_size;
	//	max_counter 	= param.size/ds.block_size;
	//ds.size 		= max_counter * sizeof(Data_t) * ds.block_size;
	ds.size 		= sizeof(Data_t) * param.size;
	Memory::Init_data( );
	Memory::Info( param.name );
	Memory::Clear_data(0);
	Set_store_counter(0);

}

void Storage_class::Store_block( Data_t* src )
{
	if ( not state.store ) return;
	if ( record_counter > ds.max_records - 2 ) return;
	buffer_t start = record_counter* ds.block_size;
	for ( buffer_t n = 0; n < ds.block_size; n++ )
	{
		Data[start + n] = src[n];
	}
	record_counter 	= ( record_counter + 1 );
	record_data 	= record_counter * ds.block_size;
	if ( record_counter == ds.max_records - 2 )
		state.store = false ;
}

Data_t* Storage_class::Get_next_block()
{

	if ( not state.play ) return nullptr;
	if ( record_counter == 0 ) return nullptr;
	Data_t* ptr = get_block( read_counter );
	read_counter = ( read_counter + 1 ) % record_counter ;
	assert( read_counter <= record_counter );

	return ptr;
}

Data_t* Storage_class::get_block( uint id)
{
	return &Data[id * ds.block_size];
}

void 	Storage_class::Set_store_counter( uint  n )
{
	assert( n < ds.max_records );
	record_counter 	= n;
	record_data 	= record_counter * ds.block_size;
	read_counter  		= 0;
}

void 	Storage_class::Reset_counter( )
{
	record_counter 	= 0;
	record_data 	= 0;
	read_counter  	= 0;
	state.store	= false;
}

string 	Storage_class::Play_mode( bool flag )
{
//	Comment(INFO, "mute " + StAparam.name );
	state.play = flag;
	return (state.play) ? "ON" : "OFF";
}

string Storage_class::Record_mode( bool flag )
{
	state.store = flag;
	if ( flag )
		read_counter = 0;
	return (state.store) ? "ON" : "OFF";
}

uint*	Storage_class::Get_storeCounter_p()
{
	return &record_counter;
}

void Storage_class::Playnotes( bool flag )
{
	string onoff = flag ? " on" : " off";
	cout << "play " + StAparam.name + onoff << endl;
	Comment(INFO, "play " + StAparam.name + onoff );
	state.play = flag;
	state.store= false;
}



//-----------------------------------------------------------------------------

Shared_Memory::Shared_Memory( buffer_t size ) :
		Logfacility_class("Shm"),
		Shm_base( size )
{
};

Shared_Memory::~Shared_Memory()
{
}

void Shared_Memory::Clear()
{
	if ( not addr )
	{
		Comment( ERROR, "shm undefined");
		return;
	}

	for ( buffer_t n = 0; n < max_frames ; n++ )
	{
		addr[n] = {0,0};
	}
}

void Shared_Memory::Stereo_buffer( key_t key )
{
	this->ds 		= *Get( key );
	this->ds.addr 	= (stereo_t*) this->ds.addr;
	this->addr 		= (stereo_t*) this->ds.addr;
}
