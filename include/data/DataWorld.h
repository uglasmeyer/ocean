/*
 * DataWorld.h
 *
 *  Created on: Sep 30, 2024
 *      Author: sirius
 */

#ifndef DATA_DATAWORLD_H_
#define DATA_DATAWORLD_H_


#include <data/Interface.h>

typedef function<void()> config_fcn_t;
extern void config_thread_fcn(	Semaphore_class* sem,
								config_fcn_t config_fcn,
								interface_t* sds,
								bool* config_thread_done );

class Dataworld_class : virtual Logfacility_class
{

public:

	typedef	array<shm_ds_t, MAXCONFIG + 1>	SharedDataSegment_arr_t;
	uint					Id			= NOID;
	Shm_base				Shm			{ 0 };//no data but functions

	Config_class			Cfg			{ "Config" };
	Config_class*			Cfg_p		= &Cfg;
	Semaphore_class			Sem			{ Cfg_p};
	Semaphore_class*		Sem_p		= &Sem;
	Interface_class			Sds			{ Cfg_p, Sem_p };
	Interface_class*		Sds_p		= &Sds;


	SharedDataSegment_arr_t	Sds_arr		{};

	Dataworld_class( uint id );
	~Dataworld_class();

	interface_t* SetSds( uint n );

private:


};

#endif /* DATA_DATAWORLD_H_ */
