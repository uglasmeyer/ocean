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

	for( uint n = 0; n < MAXCONFIG; n++ )
	{
		Interface_class SDS {Cfg_p, Sem_p };
		SDS.Setup_SDS( Cfg.Config.sdskeys[n]);
		SDS.ds.Id = n;
		Sds_arr[n] = SDS.ds;
		SDS.Type_Id = type_id;
		SDS.State_pMap();

		SDS_vec.push_back( SDS );

	}
	//	test
	assert( SDS_vec[0].SHM.ds.addr != SDS_vec[1].SHM.ds.addr );
	Sds_p		= &SDS_vec[0];
	Sds_master 	= (interface_t*) SDS_vec[0].ds.addr;


	if ( dataProc.contains( this->TypeId ))
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

	}
}

Dataworld_class::~Dataworld_class()
{
	proc_deRegister( );
}

void Dataworld_class::Init_Shm( )
{
	// Shared Memory
	if ( this->SDS_Id < 0 )
	{
		Exception( "SDS_ID undefined " );
	}
	if ( dataProc.contains( this->TypeId ))
	{
		proc_Register( );
		ShmAddr_L = (stereo_t*) SHM_vecL[SDS_Id].ds.addr;
		ShmAddr_R = (stereo_t*) SHM_vecR[SDS_Id].ds.addr;

		// test
		assert( SHM_vecL[0].ds.addr != SHM_vecL[1].ds.addr );
		assert( SHM_vecR[0].ds.addr != SHM_vecR[1].ds.addr );
	}
}

Interface_class* Dataworld_class::GetSds( uint id )
{
	return &SDS_vec[id];
}

interface_t* Dataworld_class::GetSdsAddr( uint id )
{
	Comment( INFO, "SDS Id: " + to_string( id ) + " " + Sds.type_map[TypeId] );
	if (( id<0) or ( id > MAXCONFIG ))
	{
		Comment( ERROR, "no such Shared Data Segment ");
		return Sds.addr;
	}
	if( not SDS_vec[id].ds.eexist )
	{
		Comment( ERROR, "segment not available");
		return Sds.addr;
	}
	SDS_vec[id].SHM.ShowDs( SDS_vec[id].ds );

	return ( interface_t*) SDS_vec[id].ds.addr;
}

stereo_t* Dataworld_class::GetShm_addr( ) // Synthesizer
{
	interface_t* 	sds 	= SDS_vec[ SDS_Id ].addr;
	int 			shm_id 	= sds->SHMID;
	stereo_t* 		addr 	= ( shm_id == 0 ) ? ShmAddr_R : ShmAddr_L;
	return addr;

}

stereo_t* Dataworld_class::GetShm_addr( uint sdsid ) // rtsp shm mixer
{
	interface_t* 	sds 	= SDS_vec[ 0 ].addr;
	int 			shm_id 	= sds->SHMID;
	stereo_t* 		addr 	= ( shm_id == 0 ) ? SHM_vecR[ sdsid ].addr : SHM_vecL[ sdsid ].addr;
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

void Dataworld_class::proc_Register()
{
	if( not dataProc.contains( TypeId ))
		return;
	uint idx = TypeId + SDS_Id;
	Comment(INFO,"Register: " + Sds.type_map[ TypeId ] + " " + to_string(SDS_Id) + " idx " + to_string( idx ));

	Sds_master->process_arr[idx].Id = SDS_Id;
	Sds_master->process_arr[idx].type= TypeId;
	Sds_master->process_arr[idx].size= SHM_vecL[SDS_Id].ds.size;

	show_proc_register( );

}
void Dataworld_class::proc_deRegister(  )
{
	if( not dataProc.contains( TypeId ))
		return;
	uint idx = TypeId + SDS_Id ;
	if( idx > MAXCONFIG + 1 )
	{
		Comment( ERROR, "de-register out of range ");
		return;
	}
	Comment( INFO, "de-register process " + Sds.type_map[ TypeId ] + " " + to_string( SDS_Id ) );
	Sds_master->process_arr[ idx ] = process_struct();
}

void Dataworld_class::show_proc_register(  )
{
	uint idx = TypeId + SDS_Id;
	process_t proc { Sds_master->process_arr[ idx ] };

	stringstream strs;
	strs << Line << endl;
	strs << SETW << "Id "		<< proc.Id << endl;
	strs << SETW << "type id " 	<< proc.type << endl;
	strs << SETW << "size    " 	<< proc.size << endl;
	Comment( INFO, strs.str() );
}

void Dataworld_class::Test_dw()
{
	TEST_START( className );

	TEST_END( className );


}

