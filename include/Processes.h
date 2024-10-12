/*
 * Processes.h
 *
 *  Created on: Oct 6, 2024
 *      Author: sirius
 */

#ifndef PROCESSES_H_
#define PROCESSES_H_

#include <Ocean.h>
#include <data/DataWorld.h>

class Process_class : virtual Logfacility_class
{
	string className = "Process_class";
public:

	Dataworld_class* DaTA;
	void Register( uint type, uint id )
	{
		Comment(INFO,"Register: " + DaTA->id_map[type] + " " + to_string(id));
		process_t proc = process_struct();

		proc.Id = id;
		proc.type= type;
		proc.Sds = DaTA->Sds_arr[id].addr;
		proc.ShmL= ( stereo_t* ) DaTA->SHM_vecL[id].ds.addr;
		proc.ShmR= ( stereo_t* ) DaTA->SHM_vecR[id].ds.addr;
		proc.size= DaTA->Shm_ds_R[id].size;

		DaTA->Sds_master->process_arr[type+id] = proc;

	}

	void deRegister( uint type, uint id )
	{
		DaTA->Sds_master->process_arr[ type+id ] = process_struct();
	}

	Process_class( Dataworld_class* data ) :
		Logfacility_class( className )
	{
		DaTA = data;
	};
	virtual ~Process_class()
	{};
private:

};


#endif /* PROCESSES_H_ */
