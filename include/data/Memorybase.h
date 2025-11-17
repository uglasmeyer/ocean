//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * Shmbase.h
 *
 *  Created on: Oct 2, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_MEMORYBASE_H_
#define DATA_MEMORYBASE_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <data/Statistic.h>

/**************************************************
 * Mem_param_struct
 *************************************************/
struct 				Mem_param_struct
{
	string 			name 			;
	buffer_t		size 			;	// number of buffer frames
	uint8_t			storage_time	;	// storage time in seconds
	buffer_t		block_size		= min_frames;  	// numer of read frames
	Mem_param_struct( string _name, int _sec )
	{
		name 			= _name;
		storage_time	= _sec;
		size 			= frames_per_sec * _sec;
	};
	~Mem_param_struct(){};
} 	;
typedef				Mem_param_struct StA_param_t;

/**************************************************
 * StA_state_struct
 *************************************************/
struct 				StA_state_struct // memory array status
{	// SDS related
	bool 			play			= false; // play this memory array
	bool 			store			= false; // record into this memory array
	bool			filled			= false; // there record counter is > 0
	bool			forget			= false; // delete after read
};
typedef				StA_state_struct StA_state_t;
const				StA_state_t 	default_StA_state 	= StA_state_struct();

/**************************************************
 * shm_data_struct
 *************************************************/
struct 				shm_data_struct
{
	uint			Id				= 0;		// interface id in the set of shm_data_structures
	bool			eexist			= false;	// becomes true if attached
	buffer_t 		bytes			= 0;		// in bytes
	int				sizeof_type		= sizeof( Stereo_t );
	key_t			key				= 0;		// shm key
	int 			shmid			= -1;		// shmid
	void* 			addr			= nullptr;
	string			hex				= "0x0";
};
typedef				shm_data_struct shm_ds_t;

/**************************************************
 * Shm_base
 * used by Shared_Memory and SDS interface
 *************************************************/

class 				Shm_base :
	virtual public 	Logfacility_class
{
	string 			className 		= "";
public:
	shm_ds_t		shm_ds			= shm_data_struct();

	shm_ds_t* 		Get				( key_t key );
	void 			ShowDs			( shm_ds_t );
	void* 			Attach			( int id );
	void 			Detach			( void* );
	void 			Delete			( ); // SDSview only

	void			Test_Memory		();

					Shm_base		( buffer_t size );
	virtual 		~Shm_base		();

private:

};

/**************************************************
 * mem_data_struct
 *************************************************/
struct 				mem_data_struct
{
	string			name 			= "memory";
	void*			addr			= nullptr;
	string			hex				= "0x0";
	uint 			sizeof_type 	= sizeof( Data_t );
	buffer_t 		size 			= max_frames; // define a block as a substructue on the memory data
	buffer_t		bytes			= size * sizeof_type;
	buffer_t		block_size		= min_frames;
//	buffer_t		block_bytes		= block_size * sizeof_type;
	buffer_t 		data_blocks		= size / block_size;
	uint 			max_records		= data_blocks; // data_block == max-records // TODO verify
					mem_data_struct	() = default;
					~mem_data_struct() = default;
};
typedef				mem_data_struct	mem_ds_t;


/**************************************************
 * Memory_base
 *************************************************/
class 				Memory_base :
	public virtual 	Logfacility_class
{
	string 			className 		= "";
public:
	Data_t* 		Data 			= nullptr;
	mem_ds_t		mem_ds			= mem_data_struct();

	void 			DsInfo			( string name = "Memory_base");
	void* 			Init_void		( buffer_t bytes );
	void 			Init_data		( buffer_t bytes );
	void 			SetDs			( size_t type_bytes );
	mem_ds_t* 		GetDs			();
	void 			Clear_data		( Data_t value );

					Memory_base		( buffer_t bytes );
	virtual 		~Memory_base	();

};

#endif /* DATA_MEMORYBASE_H_ */
