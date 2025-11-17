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

struct sdsstate_struct
{
	enum StateId : unsigned char {
		 OFFLINE	,
		 RUNNING	,
		 FREERUN	,
		 UPDATEGUI 	,
		 SYNC 		,
		 DEFAULT	,
		 EXITSERVER	,
 		 RECORDSTART,
		 RECORDSTOP ,
		 RECORDING	,
		 STOPPING	,
		 STARTING	,
		 INACTIVE	,
		 NOSTATE
		};

	map<int, string> sdsstate_map  // @suppress("Invalid arguments")
	{
		{ OFFLINE		, "OFFLINE" },
		{ RUNNING		, "RUNNING" },
		{ FREERUN		, "FREERUN" },
		{ UPDATEGUI		, "UPDATEGUI" },
		{ SYNC			, "SYNC" },
		{ DEFAULT		, "DEFAULT" },
		{ EXITSERVER	, "EXITSERVER" },
		{ RECORDSTART	, "RECORDSTART" },
		{ RECORDSTOP	, "RECORDSTOP" },
		{ RECORDING		, "RECORDING"},
		{ STOPPING		, "STOPPING"},
		{ STARTING		, "STARTING"},
		{ INACTIVE		, "Inactive"},
		{ NOSTATE		, "unknown"}
	};

	sdsstate_struct()
	{
		assert( sdsstate_map.size() >= StateId::NOSTATE );
	};
	~sdsstate_struct() = default;
	string sdsstateName( StateId id )
	{
		return sdsstate_map[ id ];
	}

};
typedef sdsstate_struct::StateId 			StateId_t;

enum CONNECT_t
{
	OSCFMOF,
	OSCVCOV,
	FMOVCOV,
	VCOFMOV,
	CONV = 'V',
	CONF = 'F'

};


typedef struct EventPtr_struct
{ // SDS related
	uint8_t 		first 	= 0;
	uint8_t 		last 	= 0;
	uint8_t			length	= 0;
} eventptr_t;

typedef struct appstate_struct
{ // SDS relevant
	APPID			type	= NoAPPID;
	StateId_t		state	= StateId_t::OFFLINE;
	int				pid		= NoPID;
} appstate_t;





const uint 			SDSSTR_SIZE 	= 32; // 31char + 0x0
const uint  		MAXQUESIZE		= 100;
typedef 			array<uint8_t	, MAXQUESIZE>				deque_t ;
typedef				array<appstate_t, NoAPPID>					appstate_arr_t;
typedef				array<StA_state_t,STAID_e::STA_SIZE> 		StA_state_arr_t;
typedef				array<uint8_t	, STAID_e::STA_SIZE>		StA_amp_arr_t;
typedef				array<feature_t	, OSCIDSIZE >				feature_arr_t;
typedef				array<spectrum_t, OSCIDSIZE>				spectrum_arr_t;
typedef				array<adsr_t	, OSCIDSIZE>				adsr_arr_t;
typedef				array<connectId_t,OSCIDSIZE>				connect_arr_t;
typedef				char 										cstring_t[SDSSTR_SIZE];

constexpr 	StA_state_arr_t default_StA_state_arr ()
{
	StA_state_arr_t arr {};
	for( size_t n = 0; n < STAID_e::STA_SIZE; n++ )
		 arr[n] = default_StA_state;
	return arr;
};


constexpr adsr_arr_t default_adsr_expr(  )
{
	adsr_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_adsr;
	}
	return arr;
};

constexpr spectrum_arr_t default_spectrum_expr(  )
{
	spectrum_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_spectrum;
	}
	return arr;
};

constexpr feature_arr_t default_feature_expr(  )
{
	feature_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_feature;
	}
	return arr;
};

constexpr connect_arr_t default_connect_expr(  )
{
	connect_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = Connect_struct();
	}
	return arr;
};


#endif /* DATA_SDSBASE_H_ */
