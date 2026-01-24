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
	string			name 			= "";

public:
	trigger_data_t	local_data		;		// dummy
					Trigger_class	( string _name, trigger_data_t* _data );
					Trigger_class	();
	virtual			~Trigger_class	() = default;

	void 			SetState		( bool _state ); // producer
	void 			SetActive		( bool _active ); // controller
	bool 			Get				(); // action
};

/***************************
 * Scanner_class
 **************************/
class Scanner_class
	: public virtual Logfacility_class
{
	bool					readlock		= false;
public:
	// scanner_t scanner = scanner_struct( Mem.Data, min_frames, max_frames );

	const buffer_t			inc 			= audio_frames;// read frame
	buffer_t 				rpos;			// currejt read  cursor
	buffer_t				wpos;			// current write cursor
	Data_t*					Data;

	range_T<buffer_t>		mem_range;		// 0<pos<max
	range_T<buffer_t>		fillrange;
	bool					trigger 		= false;

							Scanner_class	( Data_t* _ptr, buffer_t _max );
	virtual 				~Scanner_class()= default;
	void 					Show			( bool debug, void* p = nullptr );

	Data_t* 				Next_read			();
	buffer_t				Next_wpos			( buffer_t n );
	void					Reset				();
	void 					Lock_read			( bool flag );

	Data_t* 				Set_rpos			( buffer_t n );
	buffer_t 				Set_wpos			( buffer_t n );
	void 					Set_fillrange		( buffer_t n );
	bool 					Get_filled			();
};


/**************************************************
 * StAstate_class
 *************************************************/
class StAstate_class :
	StA_state_struct
{
public:
				StAstate_class	();
	virtual 	~StAstate_class	() = default;

	void 		Forget			( bool flag );
	bool 		Forget			();
	void 		Store			( bool flag );
	bool 		Store			();
	void 		Play			( bool flag );
	bool 		Play			();
	void 		Filled			( bool flag );
	bool 		Filled			();
	StA_state_t Get				();
	void 		Set				( StA_state_t state );
};

/***************************
 * Heap_Memory
 **************************/
class Heap_Memory :
	virtual public	Logfacility_class,
	virtual public 	Memory_base
{
public:
	Heap_Memory( buffer_t bytes );
	virtual ~Heap_Memory() = default;

};



/***************************
 * Stereo_Memory
 **************************/
template< typename stereoT >
class Stereo_Memory :
	virtual public 	Logfacility_class,
	virtual public 	Memory_base
{
	string className = "";
public:

	static const buffer_t	stereobuffer_bytes 	= recduration*frames_per_sec * sizeof(stereoT) ;

	stereoT* 				stereo_data 		= nullptr;

	Stereo_Memory		(uint8_t type_bytes, buffer_t bytes) :
		Logfacility_class( "Stereo_Memory" ),
		Memory_base( type_bytes, bytes )
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
		stereo_data = ( stereoT* ) Init_void( sizeof(stereoT), bytes );

		Assert_equal( mem_ds.bytes, mem_ds.sizeof_type * mem_ds.max_records * mem_ds.record_size );
		statistic.stereo += mem_ds.bytes;
		mem_ds.name	= Logfacility_class::className;
	}
	void Clear_data()
	{
		stereoT zero = { 0,0 };
		for ( buffer_t n = 0 ; n < mem_ds.data_blocks ; n++ )
			stereo_data[n] = zero;

	}

};

/**************************************************
 * Mem_param_struct
 *************************************************/
struct 				Mem_param_struct
{
	string 			name 			;
	buffer_t		size 			;	// number of buffer frames
	buffer_t		wdsize			;	// current frames of the wave display
	uint8_t			storage_time	;	// storage time in seconds
	const static
	buffer_t		block_size		= min_frames;  	// numer of read frames
	Mem_param_struct( string _name, int _sec )
	{
		name 			= _name;
		storage_time	= _sec;
		size 			= frames_per_sec * _sec;
		wdsize			= size;
	};
	~Mem_param_struct(){};
} 	;
typedef				Mem_param_struct StA_param_t;

/***************************
 * Storage_class
 **************************/
#include <Mixerbase.h>
class Storage_class :
		virtual public Logfacility_class,
		virtual public Memory_base
{
	string className = "";
public:
	// dynamic properties
	StA_param_t 	param			;
	StAId_e 		Id				;
	Dynamic_class	DynVolume		;
	Scanner_class	scanner 		;
	Trigger_class	beattrigger		{};
	StAstate_class 	state 			{};
	string			file			= "";
	string			filename		= "";
	bool			touched			= false;

	void 			Store_record		( Data_t* ) ;
	void 			Write_data		( Data_t* src, buffer_t frames, const float volume );//, const buffer_t& pos );
	void 			Record_mode		( bool );
	void 			Set_filename	( string dir, uint8_t sdsid );
	void 			Store_counter	( uint records );
	uint*			Store_counter	();
	void 			Reset			();
	void			Volume			( uint8_t vol, DYNAMIC mode );
	float			Volume			();

					Storage_class	( StAId_e id, StA_param_t param );
	virtual 		~Storage_class	() = default;

private:
	uint 			records	= 0;
};
typedef vector<Storage_class>		StorageArray_t;



#endif /* SYNTHSHM_H_ */
