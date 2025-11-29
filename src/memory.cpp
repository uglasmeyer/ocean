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
 * synthshm.cpp
 *
 *  Created on: Jan 16, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <data/Memory.h>
#include <String.h>
#include <System.h>


//-----------------------------------------------------------------------------

Trigger_class::Trigger_class( string _name, trigger_data_t* _data )
{
	trigger_data 	= _data;
	name 			= _name;
}
Trigger_class::Trigger_class()
{
	trigger_data	= &local_data;
	name			= "local";
}

void Trigger_class::set_state( bool _state ) // producer
{
	trigger_data->state 	= _state;
}
void Trigger_class::set_active( bool _active ) // controller
{
	trigger_data->active 	= _active;
}
bool Trigger_class::get() // action
{
	bool state = ( trigger_data->active and trigger_data->state);
	if( state ) cout << "trigger " << name << endl;
	trigger_data->state		= false;
	return state;
}


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
	if ((not p) or ( p == &trigger )) Info( "Read trigger.state		: ", trigger );

}
Data_t* Scanner_class::Next_read()
{
	if ( fillrange.max == 0 )
	{
		return nullptr; // data is empty
	}
	Data_t*
	data 			= &Data[ rpos ];
	rpos 			= ( rpos + inc ) % fillrange.max;//check_cycle( fillrange, rpos + inc, "Next" );
	trigger			= ( rpos < inc ); // indicates a next cycle or start cycle
	return data;
}

void Scanner_class::Next_write( buffer_t n )
{
	Set_wpos( wpos + n );;
}
void Scanner_class::Reset()
{
	rpos	= 0;
	wpos	= 0;
	fillrange.max = 0;
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
	if ( n == 0 )
	{
		fillrange.max = n;
		return;
	}
	if( fillrange.max == mem_range.max ) // don't change if full
		return;
	fillrange.max 	= check_range( mem_range, n, "Set_fillrange" );
}

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------

Heap_Memory::Heap_Memory( buffer_t bytes ) :
	Logfacility_class( "Memory" ),
	Memory_base( bytes )
{
	className = Logfacility_class::className;
};

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

Storage_class::Storage_class( StA_param_t param ) :
	Logfacility_class( "Storage_class" ),
	Memory_base( param.size*sizeof(Data_t) )
{
	this->param 	= param;
	className 		= Logfacility_class::className;
	DsInfo( param.name );
	Reset( );
} ;

void Storage_class::Setup( StA_param_t _param )
{
	param 			= _param;
	mem_ds.bytes 	= sizeof(Data_t) * param.size;
	mem_ds.size		= min_frames;//param.block_size;
	mem_ds.block_size=param.block_size;
	Init_data( mem_ds.bytes );
	DsInfo( param.name );
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
	if ( not state.Store() ) return;
	buffer_t start = record_counter* mem_ds.size;
	for ( buffer_t n = 0; n < mem_ds.size; n++ )
	{
		Data[start + n] = src[n];
	}
	record_counter 	= ( record_counter + 1 );
	record_data 	= record_counter * mem_ds.size;
	scanner.Set_fillrange( record_data );
	if ( record_counter == mem_ds.max_records - 2 )
		state.Store( false ) ;
}


void Storage_class::Set_store_counter( uint  n )
{
	Assert_lt(  n , mem_ds.max_records, "mem_ds.max_records" );

	record_counter 		= n;
	record_data 		= record_counter * mem_ds.size;
	if ( read_counter > n )
		read_counter  	= 0;
	scanner.Set_rpos		(0);
	scanner.Set_fillrange	( record_data );
}

void Storage_class::Reset( )
{
	Comment( DEBUG, "Reset counter ", (int)Id );
	record_counter 		= 0;
	record_data 		= 0;
	read_counter  		= 0;
	state.Store			( false );
	state.Filled		( false );
	scanner.Reset		();
	Clear_data			(0);
//	coutf << "clear sta" << (int)Id << " " << mem_ds.data_blocks << endl;
}



void Storage_class::Record_mode( bool flag )
{
	state.Store( flag );
	if ( flag )
		read_counter = 0;
}

uint* Storage_class::Get_storeCounter_p()
{
	return &record_counter;
}





//-----------------------------------------------------------------------------

Shared_Memory::Shared_Memory( buffer_t bytes ) :
	Logfacility_class("Shm"),
	Shm_base( bytes ),
	shm_range{0,bytes/shm_ds.sizeof_type}
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
