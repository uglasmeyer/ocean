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
	buffer_t 		bytes			;			// in bytes
	int				sizeof_type		;//= sizeof( Stereo_t );
	key_t			key				;		// shm key
	int 			shmid			= -1;		// shmid
	void* 			addr			= nullptr;
	string			hex				= "0x0";
	shm_data_struct( int type_size, buffer_t _bytes, key_t _key )
	{
		this->bytes			= _bytes;
		this->sizeof_type	= type_size;
		this->key			= _key;
	}
	shm_data_struct( )
	{
		this->bytes			= 0;
		this->sizeof_type	= 0;
		this->key			= 0;
	}
	~shm_data_struct() = default;
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
	shm_ds_t		shm_ds			;

	shm_ds_t* 		Get				( key_t key );
	void 			ShowDs			( shm_ds_t );
	void* 			Attach			( int id );
	void 			Clear			();
	void 			Detach			( void* );
	void 			Delete			(); // SDSview only

	void			Test_Memory		();

					Shm_base		( int type_size, buffer_t size, key_t key ); //interface SDS
	virtual 		~Shm_base		();

private:

};

/**************************************************
 * mem_data_struct
 *************************************************/
struct 				mem_data_struct
{
	const static buffer_t
					record_size 	{ min_frames }; // define a block as a substructue on the memory data
	string			name 			= "memory";
	void*			addr			= nullptr;
	string			hex				= "0x0";
	int				sizeof_type 	;
	buffer_t		bytes			;
	buffer_t 		data_blocks		;
	uint 			max_records		;
					mem_data_struct	( int type_bytes, buffer_t data_bytes )
					{
						sizeof_type = type_bytes;
						bytes		= data_bytes;
						data_blocks = bytes / type_bytes;
						max_records	= data_blocks / record_size;
					}
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
	mem_ds_t		mem_ds			;//= mem_data_struct();

	void 			DsInfo			( string name = "Memory_base");
	void* 			Init_void		( uint8_t type_size, buffer_t bytes );
	void 			Init_data		( buffer_t bytes );
	void 			SetDs			( size_t type_bytes );
	mem_ds_t* 		GetDs			();
	void 			Clear_data		( Data_t value );

					Memory_base		( uint8_t type_bytes, buffer_t bytes );
	virtual 		~Memory_base	();

};

#endif /* DATA_MEMORYBASE_H_ */
