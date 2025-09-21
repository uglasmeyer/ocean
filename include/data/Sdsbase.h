/*
 * Sdsbase.h
 *
 *  Created on: Sep 1, 2025
 *      Author: sirius
 */

#ifndef DATA_SDSBASE_H_
#define DATA_SDSBASE_H_

#include <Ocean.h>
#include <Mixerbase.h>
#include <Adsr.h>
#include <Configbase.h>


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
		 LASTNUM
		};

	const vector<string> state_map
	{
		{"OFFLINE" },
		{"RUNNING" },
		{"FREERUN" },
		{"UPDATEGUI" },
		{"SYNC" },
		{"DEFAULT" },
		{"EXITSERVER" },
		{"RECORDSTART" },
		{"RECORDSTOP" },
		{"RECORDING"},
		{"STOPPING"},
		{"STARTING"},
		{"Inactive"},
		{"unknown"}
	};
	sdsstate_struct()
	{
		assert( state_map.size() >= StateId::LASTNUM );
	};
	~sdsstate_struct() = default;
};
enum CON
{
	OSCFMOF,
	OSCVCOV,
	FMOVCOV,
	VCOFMOV,
	CONV = 'V',
	CONF = 'F'

};


typedef struct EventPtr_struct
{
	uint8_t 		first 	= 0;
	uint8_t 		last 	= 0;
	uint8_t			length	= 0;
} eventptr_t;

typedef struct register_process_struct
{
	int8_t			idx		= -1;
	uint8_t			sdsId	= 0;
	uint8_t			type	= NOID;
	pid_t			pid		= -1;
} register_process_t;


const uint			REGISTER_SIZE 	= MAXCONFIG+1;
const uint 			str_buffer_len 	= 32;
const uint  		MAXQUESIZE		= 100;
typedef 			array< uint8_t, MAXQUESIZE>					deque_t ;
typedef				array<register_process_t, REGISTER_SIZE>	process_arr_t;
typedef				StAarray_t									StA_amp_arr_t;
typedef				array<feature_t	, OSCIDSIZE >				feature_arr_t;
typedef				array<spectrum_t, OSCIDSIZE>				spectrum_arr_t;
typedef				array<adsr_t	, OSCIDSIZE>				adsr_arr_t;
typedef				array<connectId_t,OSCIDSIZE>				connect_arr_t;

#endif /* DATA_SDSBASE_H_ */
