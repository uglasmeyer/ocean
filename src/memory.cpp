/*
 * synthshm.cpp
 *
 *  Created on: Jan 16, 2024
 *      Author: sirius
 */

#include <data/Memory.h>
#include <String.h>
#include <System.h>


	// 0             pos             <          max      <   Max
	// |-------------|--------------------------|------------|
	// |-----|-----|-----|-----|-----|-----|-----|-----|-----| inc
	// |-----------------|-----------------|-----------------| wrt = x*inc

Scanner_class::Scanner_class( Data_t* _ptr, buffer_t _inc, buffer_t _max )
	: Logfacility_class( "Scanner_class" )
	, mem_range{ 0, _max}
	, fillrange{ 0, 0 }
{
	className		= Logfacility_class::className;
	rpos 			= 0;
	wpos			= 0;
	Data 			= &_ptr[ 0 ];
	inc				= audio_frames;	// min_frames;//_inc;
	wrt				= _inc; 		// use set_wrt_len to change
}

void Scanner_class::Show( bool debug, void* p )
{
	if ( not debug ) return;

	if ((not p) or ( p == &rpos )) Info( "Read position    : ", rpos );
	if ((not p) or ( p == &wpos )) Info( "Write position   : ", wpos );
	if ((not p) or ( p == &mem_range.max )) Info( "Memory max       : ", mem_range.max );
	if ((not p) or ( p == &fillrange.max )) Info( "fillrange max    : ", fillrange.max );
	if ((not p) or ( p == &inc )) Info( "Read frames      : ", inc );
	if ((not p) or ( p == &wrt )) Info( "Write frames     : ", wrt );
	if ((not p) or ( p == &trigger )) Info( "Read trigger		: ", trigger );

}
Data_t* Scanner_class::Next_read()
{
	if ( fillrange.max == 0 )
	{
		return nullptr; // data is empty
	}
	Data_t*
	data 			= &Data[ rpos ];
//	Set_rpos( rpos + inc );
	rpos 			= ( rpos + inc ) % fillrange.max;//check_cycle( fillrange, rpos + inc, "Next" );
	trigger 		= ( rpos < inc ); // indicates a next cycle or start cycle
	return data;
}

void Scanner_class::Next_write( buffer_t n )
{
	Set_wpos( wpos + n );;
}
Data_t* Scanner_class::Set_rpos( buffer_t n )
{
	rpos = check_cycle( fillrange, n, "Set_pos" );
	return &Data[n];
}
buffer_t Scanner_class::Set_wpos( buffer_t n )
{
	wpos = check_cycle( mem_range, n, "Set_wpos" );
	return wpos;
}
void Scanner_class::Set_wrt_len( buffer_t n )
{
	wrt 		= check_range( mem_range, n, "Set_wrt_len");
}
void Scanner_class::Set_fillrange( buffer_t n )
{
	if( fillrange.max == mem_range.max )
		return;
	fillrange.max 	= check_range( mem_range, n, "Set_max_len" );
}

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


void Memory::Clear_data( Data_t value )
{
	for ( buffer_t n = 0; mem_ds.data_blocks > n; n++ )
	{
		Data[n] = value;
	}
}
void Memory_base::SetDs( size_t type_size )
{
	// terminology :
	// sizeof_data	-> 	data_bytes	= sizeof(unit)
	//					block_bytes = data_bytes*units
	//					mem_bytes 	= block_bytes*blocks (blocks=sec)
	//					mem_blocks	=

				mem_ds.sizeof_type 	= type_size;
				mem_ds.data_blocks 	= mem_ds.bytes / type_size;  //max_frames
//				mem_ds.size			= bs; //min_frames
				mem_ds.max_records	= mem_ds.data_blocks / mem_ds.size ;

	buffer_t 	bytes 				= mem_ds.sizeof_type * mem_ds.max_records * mem_ds.size;
	ASSERTION( mem_ds.bytes == bytes , "init memory", (int)mem_ds.bytes , bytes );
}

mem_ds_t* Memory_base::GetDs()
{
	return &mem_ds;
}

void Memory::Init_data( )
{
	Data = ( Data_t* ) Init_void();

	SetDs( sizeof( Data_t));
	statistic.data += mem_ds.bytes;

}

void Memory::DsInfo( string name )
{
	mem_ds.name = name;
	Memory_base::DsInfo();
}


//-----------------------------------------------------------------------------







//-----------------------------------------------------------------------------


void Storage_class::Setup( StA_param_t param )
{
	StAparam 		= param;
	mem_ds.bytes 	= sizeof(Data_t) * param.size;
	mem_ds.size		= min_frames;//param.block_size;
	mem_ds.block_size=param.block_size;
	Memory::Init_data( );
	Memory::DsInfo( param.name );
	Reset();
}

void Storage_class::Write_data( Data_t* src )//, const buffer_t& wrt )
{
	buffer_t offs = scanner.wpos;
	for ( buffer_t n = 0; n < scanner.wrt; n++ )
	{
		Data[ offs ] += src[n];
		offs = ( offs + 1 ) % ( scanner.mem_range.max );
	}
	scanner.Set_fillrange( scanner.wpos + scanner.wrt );
}

void Storage_class::Store_block( Data_t* src )
{
	if ( not state.store ) return;
	buffer_t start = record_counter* mem_ds.size;
	for ( buffer_t n = 0; n < mem_ds.size; n++ )
	{
		Data[start + n] = src[n];
	}
	record_counter 	= ( record_counter + 1 );
	record_data 	= record_counter * mem_ds.size;
	scanner.Set_fillrange( record_data );
	if ( record_counter == mem_ds.max_records - 2 )
		state.store = false ;
}


void 	Storage_class::Set_store_counter( uint  n )
{
	assert( n < mem_ds.max_records );

	record_counter 		= n;
	record_data 		= record_counter * mem_ds.size;
	if ( read_counter > n )
		read_counter  	= 0;
	scanner.Set_rpos		(0);
	scanner.Set_fillrange	( record_data );
}

void 	Storage_class::Reset( )
{
	Comment( DEBUG, "Reset counter ", (int)Id );
	record_counter 		= 0;
	record_data 		= 0;
	read_counter  		= 0;
	state.store			= false;
	state.filled		= false;
	scanner.Set_rpos	(0);
	scanner.Set_wpos	(0);
	scanner.Set_fillrange ( 0 );
//	scanner.Set_fillrange ( 0 );
	Clear_data			(0);
}
void Storage_class::Playnotes( bool flag )
{
	state.play 			= flag;
	state.store			= false;
	string onoff 		= flag ? " on" : " off";
	cout << "play " + StAparam.name + onoff << endl;
	Comment(INFO, "play " + StAparam.name + onoff );

}
string 	Storage_class::Play_mode( bool flag )
{
//	Comment(INFO, "mute " + StAparam.name );
	state.play 			= flag;
//	if( scanner.fillrange.max == 0 )
//		state.play = false;
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





//-----------------------------------------------------------------------------

Shared_Memory::Shared_Memory( buffer_t size ) :
	Logfacility_class("Shm"),
	Shm_base( size ),
	shm_range{0,size/shm_ds.sizeof_type}
{
	className = Logfacility_class::className;
};

Shared_Memory::~Shared_Memory()
{
}

void Shared_Memory::Clear( const buffer_t& _frames )
{
	if ( not addr )
	{
		Comment( ERROR, "shm undefined");
		return;
	}
	buffer_t frames = check_range( shm_range, _frames, "Clear shm" );
	for ( buffer_t n = 0; n < frames ; n++ )
	{
		addr[n] = {0,0};
	}
}

void Shared_Memory::Stereo_buffer( key_t key )
{
	this->ds 		= *Get( key );
	this->ds.addr 	= (Stereo_t*) this->ds.addr;
	this->addr 		= (Stereo_t*) this->ds.addr;
}
