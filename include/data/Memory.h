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

struct scanner_struct
{
	// scanner_t scanner = scanner_struct( Mem.Data, min_frames, max_frames );
	buffer_t 				pos;
	Data_t*					Data;
	buffer_t				inc;
	buffer_t				max_pos;
	bool					trigger = false;

	scanner_struct( Data_t* _ptr, buffer_t _inc, buffer_t _max )
	{
		pos 	= 0;
		Data 	= &_ptr[ 0 ];
		inc		= _inc;
		max_pos	= _max;//0;
	}
	virtual ~scanner_struct() = default;

	Data_t* next()
	{
		if ( inc > max_pos )
			return nullptr; // data is empty
		Data_t*
		data 			= &Data[ pos ];
		pos 			= ( pos + inc ) % ( max_pos );
		trigger 		= ( pos == 0 );
//		cout.flush()<< pos << " " << endl;
		return data;
	}
	Data_t* Set_pos( buffer_t n )
	{
		if( n > max_pos )
			pos = 0;
		else
			pos	= n;
		return &Data[n];
	}
	void Set_max( buffer_t n )
	{
		if (pos > n )
			pos = 0;
		max_pos = n;
	}
};
typedef scanner_struct scanner_t;

class Memory :
		virtual Logfacility_class,
		virtual public Memory_base
{
	string className = "";
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
	void Info( string );
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

	stereo* 				stereo_data = nullptr;
	Logfacility_class Log{"Memory_base"};
	Stereo_Memory(buffer_t size) :
		Logfacility_class( "Stereo_Memory" ),
		Memory_base( size )
	{
		className = Logfacility_class::className;
		Init_data( size );
	}
	Stereo_Memory() :
		Logfacility_class( "Memory_base"),
		Memory_base( )
	{};

	virtual ~Stereo_Memory( ) = default;

	void Init_data( buffer_t bytes, buffer_t bs = min_frames )
	{
		Memory_base::mem_ds.bytes = bytes;
		stereo_data = ( stereo* ) Init_void();

		SetDs( sizeof( stereo ), bs );
		statistic.stereo += mem_ds.bytes;
		mem_ds.name	= Logfacility_class::className;
	}
	void Clear_data()
	{
		stereo zero = { 0,0 };
		for ( buffer_t n = 0 ; n < mem_ds.data_blocks ; n++ )
			stereo_data[n] = zero;

	}
	void Info( string name )
	{
		mem_ds.name = name;
		Memory_base::Info();
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
	uint 			record_data		= 0;
	Dynamic_class	DynVolume		{ volidx_range };
	scanner_t 		scanner 		= scanner_struct( nullptr, min_frames, 0 );

	StA_status_t 	state 			= StA_state_struct();

	void 			Store_block		( Data_t* ) ;
	string 			Record_mode		( bool );
	string 			Play_mode		( bool );
	void			Playnotes		( bool );
	void 			Setup			( StA_param_t);
	void 			Set_store_counter( uint n);
	void 			Reset_counter	();
	uint*			Get_storeCounter_p();

	Storage_class( ) :
		Logfacility_class( "Storage_class" ),
		Memory()
	{} ;
	virtual ~Storage_class()
	{};

private:

	uint 			read_counter 	= 0;
	uint 			record_counter	= 0;
};


class Shared_Memory :
		virtual public Logfacility_class,
		virtual public Shm_base
{

public:
	static const buffer_t 	sharedbuffer_size 	= max_frames * sizeof( stereo_t );
	stereo_t* 	addr 		= nullptr;
	shm_ds_t	ds 			= shm_data_struct();

	Shared_Memory( buffer_t size );
	virtual ~Shared_Memory();

	void Stereo_buffer( key_t key );
	void Clear( const buffer_t& frames );


};




#endif /* SYNTHSHM_H_ */
