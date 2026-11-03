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


/**************************************************
 * StAstate_class
 *************************************************/
StAstate_class::StAstate_class() :
	StA_state_data()
{
};

void StAstate_class::Forget( bool flag )
{
	forget = flag;
}
bool StAstate_class::Forget()
{
	return forget;
}
void StAstate_class::Store( bool flag )
{
	store = flag;
}
bool StAstate_class::Store()
{
	return store;
}
void StAstate_class::Play( bool flag )
{
	play = flag;
}
bool StAstate_class::Play()
{
	return play;
}
void StAstate_class::Filled( bool flag )
{
	filled = flag;
}
bool StAstate_class::Filled()
{
	return filled;
}
void StAstate_class::Start( bool flag )
{
	start = flag;
}
bool StAstate_class::Start()
{
	return start;
}
StA_state_t StAstate_class::Get()
{	// consider the order of StA_state
	return	{ play, store, filled, forget, start };
}
void StAstate_class::Set( StA_state_t state )
{
	play	= state.play;
	store	= state.store;
	filled	= state.filled;
	forget	= state.forget;
	start	= state.start;
}

/**************************************************
 * Trigger_class
 *************************************************/
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

void Trigger_class::SetState( bool _state ) // producer
{
	trigger_data->state 	= _state;
}
void Trigger_class::SetActive( bool _active ) // controller
{
	trigger_data->active 	= _active;
}
bool Trigger_class::Get() // action
{
	bool state = ( trigger_data->active and trigger_data->state);
	if( state ) cout << "trigger " << name << endl;
	trigger_data->state		= false;
	return state;
}
string Trigger_class::Str()
{
	stringstream strs;
	strs 	<< "Trigger: " 	<< name
			<< "active " 	<< (int)trigger_data->active
			<< "state  " 	<< (int)trigger_data->state	;
	return strs.str();
}


	// 0             pos             <          max      <   Max
	// |-------------|--------------------------|------------|
	// |-----|-----|-----|-----|-----|-----|-----|-----|-----| inc
	// |-----------------|-----------------|-----------------| wrt = x*inc

Scanner_class::Scanner_class( Data_t* _ptr, buffer_t _max )
	: Logfacility_class	( "Scanner_class" )
	, mem_range			{ 0, _max }
	, fillrange			{ 0, 0 }
{
	this->rpos 			= 0;
	this->wpos			= 0;
	this->Data 			= &_ptr[ 0 ];
	this->readlock		= false;

}

void Scanner_class::Show( bool debug, void* p )
{
	if ( not debug ) return;

	if ((not p) or ( p == &rpos )) Info( "Read position    : ", rpos );
	if ((not p) or ( p == &wpos )) Info( "Write position   : ", wpos );
	if ((not p) or ( p == &mem_range.max )) Info( "Memory max       : ", mem_range.max );
	if ((not p) or ( p == &fillrange.max )) Info( "fillrange max    : ", fillrange.max );
	if ((not p) or ( p == &inc )) Info( "Read frames      : ", inc );
	if ((not p) or ( p == &trigger )) Info( "Read trigger.state		: ", trigger );
}

Data_t* Scanner_class::Next_read()
{
	if ( ( fillrange.max == 0 ) or ( readlock ) )
	{
		return nullptr;
	}
	Data_t*
	data 			= &Data[ rpos ];
	assert( fillrange.len > 0 );
	rpos 			= check_cycle2( fillrange, rpos + inc );
	trigger			= ( rpos < inc ); // indicates a next cycle or start cycle
	return data;
}

buffer_t Scanner_class::Next_wpos( buffer_t n )
{
	Set_wpos( wpos + n );
	return wpos;
}

Data_t* Scanner_class::Set_rpos( buffer_t n )
{
	rpos = check_cycle( fillrange, n, __builtin_FUNCTION() );
	return &Data[rpos];
}
buffer_t Scanner_class::Set_wpos( buffer_t n )
{
	wpos = check_cycle( mem_range, n, __builtin_FUNCTION() );
	return wpos;
}
void Scanner_class::Lock_read( bool flag )
{
	readlock = flag;
}
bool Scanner_class::Read_lock()
{
	return readlock;
}
void Scanner_class::Reset()
{
	rpos	= 0;
	wpos	= 0;
	fillrange.max 	= 0;
	fillrange.len	= 0;
}
void Scanner_class::Set_fillrange( buffer_t n )
{
//	coutf << fillrange.max << endl;
	if ( n < fillrange.max ) // do not decrease max
		return;
	fillrange.max 	= check_range( mem_range, n, "Set_fillrange" );
	fillrange.len	= fillrange.max - fillrange.min;
}
bool Scanner_class::Get_filled()
{
	return ( fillrange.max > 0 );
}



/**************************************************
 * Heap_Memory
 *************************************************/
Heap_Memory::Heap_Memory( buffer_t bytes ) :
	Logfacility_class	( "Memory" ),
	Memory_base			( sizeof_Data, bytes )
{
};


/**************************************************
 * Storage_class
 *************************************************/
Storage_class::Storage_class( StAId_e id, StA_param_t _param ) :
	Logfacility_class		( "Storage_class" ),
	Memory_base				( sizeof_Data, _param.size*sizeof_Data ),
	param					( _param.name, _param.storage_time, _param.sta_dir ),
	DynVolume				( DYNFACE_e::VOL, volidx_range, volidx_range ),
	scanner 				( Memory_base::Data, _param.size )
{
	this->Id				= id;

	Set_filename			( "" );
	Reset					();
	DsInfo					( param.name );

} ;

void Storage_class::Set_wdsize( buffer_t size )
{
	if( size > 0 )
		param.wdsize = size;
}

void Storage_class::Write_data( Data_t* src, buffer_t frames, const float volume )
{
	buffer_t offs 		= scanner.wpos;
	is_zero 			= false;
	for ( buffer_t n = 0; n < frames; n++ )
	{
		Data[ offs ] 	+= src[n] * volume;
		offs			= ( offs + 1 ) % ( scanner.mem_range.max );
	}

	scanner.Set_fillrange( scanner.wpos + frames );
}

void Storage_class::Store_record( Data_t* src )
{
	if ( not state.Store() )
		return;

	buffer_t	offs 		= scanner.wpos;
				is_zero	= false;
	for ( buffer_t n = 0; n < record_size; n++ )
	{
		Data[offs + n] = src[n];
	}

	scanner.Set_fillrange	( offs + record_size );
	assert( scanner.fillrange.max > 0 );
	state.Store				( scanner.fillrange.max < scanner.mem_range.max );
	records 				= scanner.fillrange.max / record_size;
	scanner.Next_wpos		( record_size );
}

void Storage_class::Set_records( uint  _records )
{
	Assert_lt(  _records , mem_ds.max_records + 1, "mem_ds.max_records" );

	buffer_t	data_blocks		= _records * record_size;
	records						= _records;
	if( data_blocks == 0 )	// do not set wdsize to 0
	{
		scanner.Reset			();
		Clear_data				( 0 );
	}
	else
	{
		scanner.Set_fillrange	( data_blocks );
		scanner.Set_rpos		( 0 );
		scanner.Set_wpos		( data_blocks ); // append next write
		param.wdsize			= data_blocks;
		state.Filled			( true );
		is_zero					= false;
	}
	Info( "StA", (int)Id, "loaded", records, "records");
}

uint* Storage_class::Store_counter()
{
	return &records;
}

void Storage_class::Reset( )
{
	Comment				( DEBUG, "Reset counter ", (int)Id );
	records 			= 0;
	state.Set			( default_StA_state );
//	Store			( false );
//	state.Start			( false );
//	state.Filled		( false );
	scanner.Reset		();
	Clear_data			( 0 );
}

bool Storage_class::Set_filename( string subdir )
{
	string newdirname	= trailing_dirslash( param.sta_dir + subdir );
	bool changed		= true;
	if( strEqual( newdirname, dirname ))
		changed	= false;
	if( not changed )
		return false;

	dirname = newdirname;
	if( not exists( dirname ) )
	{
		create_directories( dirname );
	}
	filename			= "StA_data"	+ to_string( Id ) + ".bin";
	file 				= dirname + filename ;
	if( not exists( file ) )
	{
		Touch_file( file );
	}
	return true;
}

void Storage_class::Record_mode( bool flag )
{
	state.Store( flag );
}



void Storage_class::Volume( uint8_t vol, DYNAMIC_e mode )
{
	DynVolume.Setup( vol, mode );
;
}
float Storage_class::Volume()
{	// returns volume in %
	return DynVolume.GetCurrent().future_f;
}





