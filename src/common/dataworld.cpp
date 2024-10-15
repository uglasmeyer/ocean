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

	// Shared Data Segment

	for( uint sdsid = 0; sdsid < MAXCONFIG; sdsid++ )
	{
		Interface_class SDS {Cfg_p, Sem_p };
		SDS.Setup_SDS( sdsid, Cfg.Config.sdskeys[sdsid] );
		SDS.Type_Id = type_id;

		SDS_vec.push_back( SDS );
	}
	//	test
	assert( SDS_vec[0].ds.addr != SDS_vec[1].ds.addr );

	Sds_master = (interface_t*) SDS_vec[0].ds.addr;
	assert( Sds_master->SDS_Id == 0 );
	Reg.Setup( &Sds_master->process_arr , Cfg.type_map, TypeId );
	SDS_Id = Reg.GetId();

	init_Sds( );


	if ( Reg.Is_dataprocess() )
	{
		Comment(INFO,"Attaching data buffers");
		uint idx = 0;
		for (key_t key : Cfg.Config.shmkeys_l )
		{
			Shared_Memory SHM { sharedbuffer_size };
			SHM.Stereo_buffer( key );
			SHM.ds.Id = idx;
			SHM_vecL.push_back( SHM );
			SHM.ShowDs( SHM.ds );
			idx++;
		}
		idx = 0;
		for (key_t key : Cfg.Config.shmkeys_r )
		{
			Shared_Memory SHM { sharedbuffer_size };
			SHM.Stereo_buffer( key );
			SHM.ds.Id = idx;
			SHM_vecR.push_back( SHM );
			SHM.ShowDs( SHM.ds );
			idx++;
		}
		init_Shm( );

	}
}

Dataworld_class::~Dataworld_class()
{
	if ( Reg.Is_dataprocess() )
	{
		for( uint n =0; n<MAXCONFIG; n++)
		{
			SHM_vecL[n].Detach( SHM_vecL[n].ds.addr );
			SHM_vecR[n].Detach( SHM_vecR[n].ds.addr );
		}
	}
}

void Dataworld_class::init_Sds( )
{
	Sds_p		= &SDS_vec[ SDS_Id ];
	Sds.addr 	= GetSdsAddr(  );
}

void Dataworld_class::init_Shm( )
{
	// Shared Memory
	if ( this->SDS_Id < 0 )
	{
		Exception( "SDS_ID undefined " );
	}
	if ( Reg.Is_dataprocess() )
	{
		ShmAddr_L = (stereo_t*) SHM_vecL[0].ds.addr;
		ShmAddr_R = (stereo_t*) SHM_vecR[0].ds.addr;

		// test
		assert( SHM_vecL[0].ds.addr != SHM_vecL[1].ds.addr );
		assert( SHM_vecR[0].ds.addr != SHM_vecR[1].ds.addr );
	}
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
	Comment( INFO, "SDS Id: " + to_string( id ) + " " + Cfg.type_map[TypeId] );
	if (( id<0) or ( id > (int)MAXCONFIG ))
	{
		Comment( ERROR, "no such Shared Data Segment ");
		return Sds.addr;
	}
	if( not SDS_vec[id].ds.eexist )
	{
		Comment( ERROR, "segment not available");
		return Sds.addr;
	}
//	SDS_vec[id].SHM.ShowDs( SDS_vec[id].ds );

	return ( interface_t*) SDS_vec[id].ds.addr;
}

stereo_t* Dataworld_class::GetShm_addr( ) // Synthesizer
{
//	interface_t* 	sds 	= SDS_vec[ SDS_Id ].addr;
	interface_t* 	sds 	= SDS_vec[ 0 ].addr;
	int 			shm_id 	= sds->SHMID;
	stereo_t* 		addr 	= ( shm_id == 0 ) ? ShmAddr_R : ShmAddr_L;
	return addr;

}

stereo_t* Dataworld_class::GetShm_addr( uint sdsid ) // rtsp shm mixer
{
	interface_t* 	sds 	= SDS_vec[ 0 ].addr;
	int 			shm_id 	= sds->SHMID;
	stereo_t* 		addr 	= ( shm_id == 0 ) ? ( stereo_t*) SHM_vecR[ sdsid ].ds.addr :
												( stereo_t*) SHM_vecL[ sdsid ].ds.addr;
	return addr;

}

stereo_t* Dataworld_class::SetShm_addr() // Audioserver
{
	stereo_t* 		addr;
	interface_t* 	sds 	= SDS_vec[0].addr;
	uint 			shm_id 	= sds->SHMID;
	uint 			mode 	= sds->MODE;
	if ( mode == SYNC )
	{
		shm_id 	= ( shm_id + 1 ) % 2;
		addr 	= (	shm_id == 0 ) ? ShmAddr_L : ShmAddr_R;
		sds->SHMID 	= shm_id;
		sds->MODE 	= FREERUN;

	}
	else
	{
		addr 			= ( shm_id == 0 ) ? ShmAddr_R : ShmAddr_L;
	}
	return addr;
}

