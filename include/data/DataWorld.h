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


	typedef	array<shm_ds_t, MAXCONFIG>	SharedDataSegment_arr_t;
	typedef vector<Interface_class> SDS_vec_t;
	typedef vector<Shared_Memory> 	SHM_vec_t;

	uint					TypeId		= NOID;
	int						SDS_Id		= -1;
	Shared_Memory			Shm			{ sharedbuffer_size };//no data but functions

	Config_class			Cfg			{ "Config" };
	Config_class*			Cfg_p		= &Cfg;
	Semaphore_class			Sem			{ Cfg_p};
	Semaphore_class*		Sem_p		= &Sem;
	Interface_class			Sds			{ Cfg_p, Sem_p };
	SDS_vec_t			 	SDS_vec 	{ };
	Interface_class*		Sds_p		= nullptr;
	SHM_vec_t				SHM_vecL	{ };
	SHM_vec_t				SHM_vecR	{ };
	std::set<int> 			dataProc	{ AUDIOID, SYNTHID, RTSPID };
	stereo_t* 				ShmAddr_L 	= nullptr;
	stereo_t* 				ShmAddr_R 	= nullptr;
	interface_t*			Sds_master	;

	SharedDataSegment_arr_t	Sds_arr		{};
	vector<process_t>	SynthesizerProcesses	{};
	process_t			AudioserverProcess		{};

	interface_t* GetSdsAddr( uint id );
	Interface_class* GetSds( uint id );

	stereo_t* SetShm_addr( ); 			// Audioserver
	stereo_t* GetShm_addr( ); 			// Synthesizer
	stereo_t* GetShm_addr( uint sdsid );// rtsp
	void Init_Shm( );
	void Test_dw();

	Dataworld_class( uint id );
	~Dataworld_class();


private:


	void show_proc_register( );
	void proc_Register( );
	void proc_deRegister();

};

#endif /* DATA_DATAWORLD_H_ */
