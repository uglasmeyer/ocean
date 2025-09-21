/*
 * sharedmem.h
 *
 *  Created on: Dec 15, 2023
 *      Author: sirius
 */

#ifndef SYNTHSHM_H_
#define SYNTHSHM_H_


#include <Ocean.h>
#include <Logfacility.h>
#include <data/Memorybase.h>
#include <Dynamic.h>

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

/***************************
 * Memory
 **************************/
class Memory :
	virtual 		Logfacility_class,
	virtual public 	Memory_base
{
	string className =  "";

public:
	Data_t* 	Data = nullptr;
	Memory( buffer_t bytes ) :
		Logfacility_class( "Memory" ),
		Memory_base( bytes )
	{
		className = Logfacility_class::className;
		Init_data();
	};

	Memory( ) :
		Logfacility_class( "Memory" )
	{
		className = Logfacility_class::className;
		Comment( DEBUG, "pre-init " + className );
	};
	virtual ~Memory() = default;

	void Init_data(  );
	void Clear_data( Data_t );
	void DsInfo( string );
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

	Stereo_Memory		(buffer_t size) :
		Logfacility_class( "Stereo_Memory" ),
		Memory_base( size )
	{
		className = Logfacility_class::className;
		Init_data( size );
	}
	Stereo_Memory() :
		Logfacility_class( "Memory_base")
//	,	Memory_base( )
	{
		className = Logfacility_class::className;
	};

	virtual ~Stereo_Memory( )
	{
		DESTRUCTOR( className )
	}

	void Init_data( buffer_t bytes, buffer_t bs = min_frames )
	{
		Memory_base::mem_ds.bytes = bytes;
		stereo_data = ( stereo* ) Init_void();

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
		virtual public Memory
{
	string className = "";
public:
	// dynamic properties
	StA_param_t 	StAparam		= StA_param_struct();
	string 			Name			= "";
	uint8_t 		Id				= 0xFF;
	bool			is_RecId		= false;
	uint 			record_data		= 0;
	Dynamic_class	DynVolume		{ volidx_range };
	Scanner_class	scanner 		{ nullptr, min_frames, 0 };

	StA_state_t 	state 			= StA_state_struct();

	void 			Store_block		( Data_t* ) ;
	void 			Write_data		( Data_t* src );//, const buffer_t& pos );
	string 			Record_mode		( bool );
	string 			Play_mode		( bool );
	void			Playnotes		( bool );
	void 			Setup			( StA_param_t);
	void 			Set_store_counter( uint n);
	void 			Reset			();
	uint*			Get_storeCounter_p();

	Storage_class( ) :
		Logfacility_class( "Storage_class" ),
		Memory()
	{} ;
	virtual ~Storage_class() = default;

private:

	uint 			read_counter 	= 0;
	uint 			record_counter	= 0;
};

/***************************
 * Shared_Memory
 **************************/
class Shared_Memory :
		virtual public Logfacility_class,
		virtual public Shm_base
{
	string 					className 			= "";
public:
	shm_ds_t				ds					= shm_data_struct();

	static const buffer_t 	sharedbuffer_size 	= (audio_frames) * sizeof( Stereo_t );
	Stereo_t* 				addr 				= nullptr;
	range_T<buffer_t>		shm_range			;
	Shared_Memory( buffer_t size );
	virtual ~Shared_Memory();

	void Stereo_buffer( key_t key );
	void Clear( const buffer_t& frames );


};




#endif /* SYNTHSHM_H_ */
