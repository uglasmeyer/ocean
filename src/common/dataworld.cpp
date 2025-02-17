/*
 * dataworld.cpp
 *
 *  Created on: Sep 30, 2024
 *      Author: sirius
 */

#include <data/DataWorld.h>


Dataworld_class::Dataworld_class( uint type_id ) :
	Logfacility_class( "DaTA")
{
	this->TypeId	= type_id;
	auto sds_setup = [ this ]( uint sdsid )
	{
		Interface_class SDS {Cfg_p, Sem_p };
		SDS.Type_Id = this->TypeId;
		SDS.Setup_SDS( sdsid, Cfg.Config.sdskeys[sdsid] );

		SDS_vec.push_back( SDS );
	};


	// Shared Data Segment

	for( uint sdsid = 0; sdsid < MAXCONFIG; sdsid++ )
		sds_setup( sdsid );

	//	test
	assert( SDS_vec[0].ds.addr != SDS_vec[1].ds.addr );

	Master_Sds_p	= &SDS_vec[0];
	sds_master = (interface_t*) SDS_vec[0].ds.addr;

	Reg.Setup( sds_master, TypeId );
	SDS_Id = Reg.GetId(  );
	sds_master->SDS_Id = 0;

	Sds_p = GetSds();


	if ( Reg.Is_dataprocess() )
	{
		Comment(INFO,"Attaching stereo buffers");

		init_Shm( SHM_0, Cfg.Config.SHM_keyl, 0 );
		ShmAddr_0 = (stereo_t*) SHM_0.ds.addr;

		init_Shm( SHM_1, Cfg.Config.SHM_keyr, 1 );
		ShmAddr_1 = (stereo_t*) SHM_1.ds.addr;
	}
}

Dataworld_class::~Dataworld_class()
{
	Reg.Proc_deRegister( );

	if ( Reg.Is_dataprocess() )
	{
		Sem.Release(SEMAPHORE_EXIT);
		SHM_0.Detach( SHM_0.ds.addr );
		SHM_1.Detach( SHM_1.ds.addr );
	}

	cout << "visited ~" <<  className << endl;
}

void Dataworld_class::init_Shm( Shared_Memory& SHM, key_t key, uint idx )
{
	// Shared Memory
	SHM.Stereo_buffer( key );
	SHM.ds.Id = idx;
	SHM.ShowDs( SHM.ds );
}

Interface_class* Dataworld_class::GetSds(  )
{
	return &SDS_vec[ SDS_Id ];
}
Interface_class* Dataworld_class::GetSds( int id )
{
	return &SDS_vec[ id ];
}

interface_t* Dataworld_class::GetSdsAddr( )
{
	return GetSdsAddr( SDS_Id );
}
interface_t* Dataworld_class::GetSdsAddr( int id )
{
	Comment( DEBUG, "SDS Id: " + to_string( id ) + " " + Type_map( TypeId ) );
	if (( id<0) or ( id > (int)MAXCONFIG ))
	{
		EXCEPTION( "no such Shared Data Segment ");
	}
	if( not SDS_vec[id].ds.eexist )
	{
		EXCEPTION( "segment not available");
	}

	return ( interface_t*) SDS_vec[id].ds.addr;
}

void Dataworld_class::ClearShm()
{
	int 			shm_id 	= sds_master->SHMID;
	( shm_id == 0 ) ? SHM_0.Clear() : SHM_1.Clear();
}

stereo_t* Dataworld_class::GetShm_addr( ) // Synthesizer
{
	uint8_t			shm_id 	= sds_master->SHMID;
	stereo_t* 		addr 	= ( shm_id == 0 ) ? ShmAddr_1 : ShmAddr_0;
	return addr;

}

stereo_t* Dataworld_class::SetShm_addr() // Audioserver
{
	stereo_t* 		addr;
	uint8_t			shm_id 	= sds_master->SHMID;

	shm_id 	= ( shm_id + 1 ) % 2;
	addr 	= (	shm_id == 0 ) ? ShmAddr_0 : ShmAddr_1;
	sds_master->SHMID 	= shm_id;
//	sds_master->MODE 	= FREERUN;

	return addr;
}


