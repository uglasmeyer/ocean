/*
 * dataworld.cpp
 *
 *  Created on: Sep 30, 2024
 *      Author: sirius
 */

#include <data/DataWorld.h>

Logfacility_class Log_data( "DataWorld");

void config_thread_fcn(	Semaphore_class* sem,
						config_fcn_t config_fcn,
						interface_t* sds,
						bool* config_thread_done )
{
	Log_data.Comment( INFO, "Config thread started");
	while( true )
	{
		sem->Lock( SEMAPHORE_CONFIG );
		if ( *config_thread_done )
			break;
		config_fcn();

	}
	Log_data.Comment( INFO, "Config thread terminated");
}


Dataworld_class::Dataworld_class( uint id ) :
Logfacility_class( "DaTA")
{
	this->Id	= id;

	Sds.Setup_SDS( Cfg.Config.SDS_key );
	Sds.ds.Id = 0;
	Sds_arr[0] = Sds.ds;
	if (id != GUI_ID) Shm.Detach( Sds.ds.addr );
	if ( true )//this->Id == GUI_ID )
	{
		for( uint n = 0; n < MAXCONFIG; n++ )
		{
			Sds.Setup_SDS( Cfg.Config.sds_arr[ n ] );
			Sds.ds.Id = n+1;
			Sds_arr[n + 1 ] = Sds.ds;
			if (id != GUI_ID) Shm.Detach( Sds.ds.addr );
		}
	}
	SetSds( 0 );

}
Dataworld_class::~Dataworld_class()
{

}

interface_t* Dataworld_class::SetSds( uint n )
{
	if (( n<0) or ( n > MAXCONFIG ))
	{
		Comment( ERROR, "no such Shared Data Segment ");
		return Sds.addr;
	}
	if( not Sds_arr[n].eexist )
	{
		Comment( ERROR, "segment not available");
		return Sds.addr;
	}
	if ( Id != GUI_ID) Shm.Detach( Sds.ds.addr );
	Sds.ds	= Sds_arr[n];
	assert ( n == Sds.ds.Id );
	if ( Id != GUI_ID)
	{
		Sds.addr = (interface_t*) Shm.Attach( Sds.ds.shmid ) ;
	}
	else
		Sds.addr = (interface_t*) Sds.ds.addr;

	Comment( INFO, "New shm data structure, Id: " + to_string( n ));
	Shm.ShowDs( Sds.ds );

	Sem.Release( SEMAPHORE_CONFIG );
	return Sds.addr;
}

