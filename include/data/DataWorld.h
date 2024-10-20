/*
 * DataWorld.h
 *
 *  Created on: Sep 30, 2024
 *      Author: sirius
 */

#ifndef DATA_DATAWORLD_H_
#define DATA_DATAWORLD_H_


#include <data/Interface.h>


class Dataworld_class : virtual Logfacility_class
{
	string className = "Dataworld_class";
public:

	typedef vector<Interface_class> SDS_vec_t;

	uint					TypeId		= NOID;
	int						SDS_Id		= -1;

	Config_class			Cfg			{ "Config" };
	Config_class*			Cfg_p		= &Cfg;
	Semaphore_class			Sem			{ Cfg_p};
	Semaphore_class*		Sem_p		= &Sem;
	SDS_vec_t			 	SDS_vec 	{ };
	Shared_Memory			SHM_0		{ sharedbuffer_size };
	Shared_Memory			SHM_1		{ sharedbuffer_size };
	Register_class			Reg			{ &Cfg.type_map };

	stereo_t* 				ShmAddr_0 	= nullptr;
	stereo_t* 				ShmAddr_1 	= nullptr;
	Interface_class*		Sds_p		= nullptr;
	interface_t*			Sds_master	= nullptr;


	interface_t* GetSdsAddr();
	interface_t* GetSdsAddr( int id );
	Interface_class* GetSds(  );
	Interface_class* GetSds( int id );

	stereo_t* 	SetShm_addr( ); 			// Audioserver
	stereo_t* 	GetShm_addr( ); 			// Synthesizer
	void 		ClearShm();

	Dataworld_class( uint id );
	~Dataworld_class();


private:

	void state_pMap();
	void init_Shm( Shared_Memory& SHM, key_t key, uint idx );
};

#endif /* DATA_DATAWORLD_H_ */
