/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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
 * Sdsbase.h
 *
 *  Created on: Sep 1, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_SDSBASE_H_
#define DATA_SDSBASE_H_

#include <Kbd.h>
#include <Keyboard_base.h>
#include <data/Configbase.h>
#include <data/Memorybase.h>
#include <Adsr.h>
#include <Mixerbase.h>
#include <EventKeys.h>
#include <Wavedisplay_base.h>
#include <notes/Notesbase.h>
#include <cstring>

#define MASTER_SDSID 3 	// specification of the communication SDS sds_master and Sds_master.
						// id of unique processes
#define MAIN_SYNTHID 0	// fall back synthesizer id  if no synth or keyboard process is running

struct sdsstate_struct
{

	map< const StateId_e , string > sdsstate_map = // @suppress("Invalid arguments")
	{
		{ OFFLINE		, "OFFLINE" 	},
		{ RUNNING		, "RUNNING" 	},
		{ DEFAULT		, "DEFAULT" 	},
		{ EXITSERVER	, "EXITSERVER"	},
		{ RECORDSTART	, "RECORDSTART"	},
		{ RECORDSTOP	, "RECORDSTOP"	},
		{ RECORDING		, "RECORDING"	},
		{ STOPPING		, "STOPPING"	},
		{ STARTING		, "STARTING"	},
		{ INACTIVE		, "Inactive"	},
		{ SLEEPING		, "Sleeping"	},
		{ ENABLED		, "Enabled"		},
		{ DISABLED		, "Disabled"	},
		{ STATEID_SIZE	, "unknown"		}
	};

	sdsstate_struct()
	{
		assert( ( sdsstate_map.size() - 1 ) == StateId_e::STATEID_SIZE );
	};
	~sdsstate_struct() = default;
	string sdsstateName( StateId_e id )
	{
		return sdsstate_map[ id ];
	}

};

enum CONNECT_e : uint8_t
{
	OSCFMOF,
	OSCVCOV,
	FMOVCOV,
	VCOFMOV,
	CONV = 'V',
	CONF = 'F'
};


typedef struct EventPtr_data
{ // SDS related
	uint8_t 		first 	= 0;
	uint8_t 		last 	= 0;
	uint8_t			length	= 0;
} eventptr_t;

typedef struct appstate_data
{ // SDS relevant
	APPID_e			type	= APPID_SIZE;
	StateId_e		state	= StateId_e::DEFAULT;
	int				pid		= NoPID;
} appstate_t;

typedef struct StA_amp_data
{	// SDS related
	uint8_t			amp				= 0;
	uint8_t 		slide			= 0;
	DYNAMIC_e 		mode			= INIT;
} StA_amp_t;

const uint 			SDSNAME_SIZE 	= 32; // 31char + 0x0
const uint			SDSLONG_SIZE	= 128;
const uint  		MAXQUESIZE		= 100;
typedef 			array<EVENTKEY_e, MAXQUESIZE>				deque_t ;
typedef				array<appstate_t, APPID_SIZE>				appstate_arr_t;
typedef				array<StA_state_t,StAId_e::STAID_ALL> 		StA_state_arr_t;
typedef				array<StA_amp_t	, StAId_e::STAID_ALL>		StA_amp_arr_t;
typedef				array<feature_t	, OSCIDSIZE >				feature_arr_t;
typedef				array<spectrum_t, OSCIDSIZE>				spectrum_arr_t;
typedef				array<adsr_data_t	, OSCIDSIZE>				adsr_arr_t;
typedef				array<connectId_t,OSCIDSIZE>				connect_arr_t;
typedef				char										cstring_name[SDSNAME_SIZE];
typedef				char										cstring_long[SDSLONG_SIZE];

constexpr StA_state_arr_t default_StA_state_arr ()
{
	StA_state_arr_t arr {};
	for( size_t n = 0; n < StAId_e::STAID_ALL; n++ )
		 arr[n] = default_StA_state;
	return arr;
};

template< typename cstr >
struct cstring_T
{
private:
	string				str;
	size_t				maxlen;
public:
	cstr*				cstring_p;
	size_t				len;

	cstring_T()
	{
		str			= "";
		cstring_p 	= nullptr;
		maxlen		= 0;
		len			= 0;
	}
	cstring_T( cstr* p, size_t size )
	{
		cstring_p	= p;
		maxlen		= size - 2;
		len			= 0;
		read		();
	}
	void cstr_p( cstr* p )
	{
		cstring_p = p;
	}
	void save( string str )
	{
		set( str );
		save();
	}
	void save()
	{
		str 		= str.substr(0, maxlen );//SDSLONG_SIZE-2 );
		len 		= str.length();
		strcpy		( *cstring_p, str.data() );
	}
	string read()
	{
		str.assign	( *cstring_p );
		len 		= str.length();
		return str;
	}
	void set( string s ) // set local only
	{
		str = s;
		len = str.length();
	}
	string get()	// get local only
	{
		return str;
	}
	void append( string s )
	{
		str.append( s );
		len = str.length();
	}
};

typedef struct slide_duration_struct
{
	uint8_t* 	steps				;
	int 		max_steps			;
	uint8_t		default_slide_value	;
	int			page_steps			;

				slide_duration_struct	( uint8_t* sds_value );
				~slide_duration_struct	() = default;
	void 		msec2value				( int msec );
	void 		percent2value			( int pc );
	void		Set						( uint8_t val );
	int			value2msec				();
	string		value_str				( bool flag = true );


} slide_duration_t;

#endif /* DATA_SDSBASE_H_ */
