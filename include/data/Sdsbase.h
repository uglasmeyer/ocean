/*
 * Sdsbase.h
 *
 *  Created on: Sep 1, 2025
 *      Author: sirius
 */

#ifndef DATA_SDSBASE_H_
#define DATA_SDSBASE_H_

#include <Ocean.h>
#include <Kbd.h>
#include <Mixerbase.h>
#include <Adsr.h>
#include <data/Configbase.h>


struct sdsstate_struct
{
	enum StateId {
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



const uint 			str_buffer_len 	= 32;
const uint  		MAXQUESIZE		= 100;
typedef 			array< uint8_t, MAXQUESIZE>					deque_t ;
typedef				array<appstate_t, NoAPPID>					appstate_arr_t;
typedef				StAarray_t									StA_amp_arr_t;
typedef				array<feature_t	, OSCIDSIZE >				feature_arr_t;
typedef				array<spectrum_t, OSCIDSIZE>				spectrum_arr_t;
typedef				array<adsr_t	, OSCIDSIZE>				adsr_arr_t;
typedef				array<connectId_t,OSCIDSIZE>				connect_arr_t;



#endif /* DATA_SDSBASE_H_ */
