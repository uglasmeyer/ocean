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
 * sharedmem.h
 *
 *  Created on: Dec 15, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef SYNTHSHM_H_
#define SYNTHSHM_H_


#include <Ocean.h>
#include <Logfacility.h>
#include <data/Memorybase.h>
#include <Dynamic.h>

/***************************
 * trigger_struct
 **************************/

typedef struct trigger_data_struct
{
	bool state 		= false ;
	bool active 	= false;
} trigger_data_t;

class Trigger_class
{
	trigger_data_t* trigger_data; 	// watch dog point
	trigger_data_t	local_data;		// dummy
	string			name 			= "";

public:
					Trigger_class	( string _name, trigger_data_t* _data );
					Trigger_class	();
	virtual			~Trigger_class	() = default;

	void 			set_state		( bool _state ); // producer
	void 			set_active		( bool _active ); // controller
	bool 			get				(); // action
};

/***************************
 * Scanner_class
 **************************/
class Scanner_class
	: public virtual Logfacility_class
{
	string					className		= "";
public:
	// scanner_t scanner = scanner_struct( Mem.Data, min_frames, max_frames );

	buffer_t				inc; 			// read frame
	buffer_t 				rpos;			// currejt read  cursor
	buffer_t				wpos;			// current write cursor
	buffer_t				wrt; 			// write frames
	Data_t*					Data;

	range_T<buffer_t>		mem_range;		// 0<pos<max
	range_T<buffer_t>		fillrange;
	bool					trigger 		= false;

							Scanner_class	( Data_t* _ptr, buffer_t _inc, buffer_t _max );
	virtual 				~Scanner_class()= default;
	void 					Show			( bool debug, void* p = nullptr );

	Data_t* 				Next_read			();
	void 					Next_write			( buffer_t n );
	Data_t* 				Set_rpos			( buffer_t n );
	buffer_t 				Set_wpos			( buffer_t n );
	void 					Set_wrt_len			( buffer_t n );
	void 					Set_fillrange		( buffer_t n );

};


/**************************************************
 * StAstate_class
 *************************************************/
class StAstate_class :
	StA_state_struct
{
public:
	StAstate_class() :
		StA_state_struct()
	{};
	virtual ~StAstate_class() = default;

	void Forget( bool flag )
	{
		forget = flag;
	}
	bool Forget()
	{
		return forget;
	}
	void Store( bool flag )
	{
		store = flag;
	}
	bool Store()
	{
		return store;
	}
	void Play( bool flag )
	{
		play = flag;
	}
	bool Play()
	{
		return play;
	}
	void Filled( bool flag )
	{
		filled = flag;
	}
	bool Filled()
	{
		return filled;
	}
	StA_state_t Get()
	{
		StA_state_t 	state 	= StA_state_struct();
		state.filled 			= filled;
		state.forget 			= forget;
		state.play				= play;
		state.store 			= store;
		return 			state;
	}
	void Set( StA_state_t state )
	{
		filled	= state.filled;
		forget	= state.forget;
		play	= state.play;
		store	= state.store;
	}
private:
};

/***************************
 * Heap_Memory
 **************************/
class Heap_Memory :
	virtual 		Logfacility_class,
	virtual public 	Memory_base
{
	string className =  "";

public:
	Heap_Memory( buffer_t bytes );
	virtual ~Heap_Memory() = default;

};



/***************************
 * Stereo_Memory
 **************************/
template< typename stereo >
class Stereo_Memory :
	virtual public 	Logfacility_class,
	virtual public 	Memory_base
{
	string className = "";
public:

	static const buffer_t	stereobuffer_bytes 	= recduration*frames_per_sec * sizeof( stereo );

	stereo* 				stereo_data 		= nullptr;

	Stereo_Memory		(buffer_t bytes) :
		Logfacility_class( "Stereo_Memory" ),
		Memory_base( bytes )
	{
		className = Logfacility_class::className;
		Init_data( bytes );
	}

	virtual ~Stereo_Memory( )
	{
		DESTRUCTOR( className )
	}

	void Init_data( buffer_t bytes, buffer_t bs = min_frames )
	{
		stereo_data = ( stereo* ) Init_void( bytes );

		SetDs( sizeof( stereo ) );
		statistic.stereo += mem_ds.bytes;
		mem_ds.name	= Logfacility_class::className;
	}
	void Clear_data()
	{
		stereo zero = { 0,0 };
		for ( buffer_t n = 0 ; n < mem_ds.data_blocks ; n++ )
			stereo_data[n] = zero;

	}

private:

};

/***************************
 * Storage_class
 **************************/

class Storage_class :
		virtual public Logfacility_class,
		virtual public Memory_base
{
	string className = "";
public:
	// dynamic properties
	StA_param_t 	param			= Mem_param_struct("",0 );
	string 			Name			= "";
	uint8_t 		Id				= 0xFF;
	bool			is_RecId		= false;
	uint 			record_data		= 0;
	Dynamic_class	DynVolume		{ volidx_range };
	Scanner_class	scanner 		{ nullptr, min_frames, 0 };

	StAstate_class 	state 			{};//= StA_state_struct();

	void 			Store_block		( Data_t* ) ;
	void 			Write_data		( Data_t* src );//, const buffer_t& pos );
	void 			Record_mode		( bool );
	void 			Setup			( StA_param_t);
	void 			Set_store_counter( uint n);
	void 			Reset			();
	uint*			Get_storeCounter_p();

					Storage_class( StA_param_t param );
	virtual 		~Storage_class() = default;

private:

	uint 			read_counter 	= 0;
	uint 			record_counter	= 0;
};

/***************************************
 * Shared_Memory
 * Synthesizer/Audioserver data exchange
 **************************************/
class Shared_Memory :
		virtual public 		Logfacility_class,
		virtual public 		Shm_base
{
	string 					className 			= "";
public:
	shm_ds_t				ds					= shm_data_struct();

	static const buffer_t 	sharedbuffer_size 	= (audio_frames) * sizeof( Stereo_t );
	Stereo_t* 				addr 				= nullptr;
	range_T<buffer_t>		shm_range			;
							Shared_Memory( buffer_t size );
	virtual 				~Shared_Memory();

	void 					Stereo_buffer( key_t key );
	void 					Clear( const buffer_t& frames );


};




#endif /* SYNTHSHM_H_ */
