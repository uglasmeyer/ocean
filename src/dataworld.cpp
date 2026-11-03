/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * dataworld.cpp
 *
 *  Created on: Sep 30, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include <data/DataWorld.h>
#include <fstream>

/**************************************************
 * Dataworld_class
 *************************************************/
Dataworld_class::Dataworld_class( 	Config_class* 		cfg,
									Semaphore_class* 	sem )
	: Logfacility_class	( "Dataworld_class" )
	, AppId				( cfg->Process.AppId )
	, SDS				( AppId, cfg, sem )
	, Appstate			( AppId, SDS.vec )
	, SDS_Id			( Appstate.SDSid )
	, SHM_l				{ sizeof_Stereo, audio_frames * sizeof_Stereo, cfg->Config.SHM_keyl }
	, SHM_r				{ sizeof_Stereo, audio_frames * sizeof_Stereo, cfg->Config.SHM_keyr }
{

	this->Cfg_p 		= cfg;
	this->Sem_p 		= sem;

	this->Sds_p 		= SDS.GetSds( SDS_Id );
	this->Test_result	= "";

	init_shared_sound	();
}

void Dataworld_class::init_shared_sound()
{
	if ( Cfg_p->Process.data_process  )
	{
		Comment(INFO,"Attaching stereo buffers");

		init_Shm	( SHM_l, Cfg_p->Config.SHM_keyl, 0 );
		ShmAddr_l 	= (Stereo_t*) SHM_l.shm_ds.addr;

		init_Shm	( SHM_r, Cfg_p->Config.SHM_keyr, 1 );
		ShmAddr_r 	= (Stereo_t*) SHM_r.shm_ds.addr;
	}
}
void Dataworld_class::init_Shm( Shm_base& SHM, key_t key, uint idx )
{
	// Shared Memory
	SHM.shm_ds.Id 	= idx;
	SHM.ShowDs		( SHM.shm_ds );
}

SharedData_class* Dataworld_class::GetSds(  )
{
	return SDS.GetSds( SDS_Id );
}

interface_t* Dataworld_class::GetSdsAddr( )
{
	Comment( DEBUG, "SDS Id: " + to_string( SDS_Id ) + " " + Appstate.Name );
	return SDS.GetSdsAddr( SDS_Id );
}

Dataworld_class::~Dataworld_class()
{
	if ( Cfg_p->Process.data_process )
	{
		SHM_l.Detach( SHM_l.shm_ds.addr );
		SHM_r.Detach( SHM_r.shm_ds.addr );
	}
	if( Test_result.length() > 0 )
	{
		coutf << "Test Synthesizer " << Test_result << endl;
	}

	DESTRUCTOR( className );
}

void Dataworld_class::ClearShm()
{
	int 			shm_id 	= SDS.master->SHMID;
	( shm_id == 0 ) ? SHM_l.Clear() : SHM_r.Clear();
}

Stereo_t* Dataworld_class::GetShm_addr( ) // Synthesizer
{
	uint8_t			shm_id 	= SDS.master->SHMID;
	Stereo_t* 		addr 	= ( shm_id == 0 ) ? ShmAddr_r : ShmAddr_l;
	return addr;

}

Stereo_t* Dataworld_class::SetShm_addr() // Audioserver
{
	Stereo_t* 		addr;
	uint8_t			shm_id 	= SDS.master->SHMID;

	shm_id 	= ( shm_id + 1 ) % 2;
	addr 	= (	shm_id == 0 ) ? ShmAddr_l : ShmAddr_r;
	SDS.master->SHMID 	= shm_id;

	return addr;
}

void Dataworld_class::EmitEvent( const EVENTFLAG_e flag, string comment )
{
	if ( not Appstate.IsRunning( SDS.master, GUI_ID ) )
		return;
	SDS.master->FLAG = flag;
	SDS.Master->Write_str( UPDATELOG_STR, to_string( (uint8_t)flag ) + ">" + comment );
	Sem_p->Release( SEMAPHORE_EVENT );
};


void Dataworld_class::Test_Dataworld()
{
	TEST_START( className );
	SharedData_class* Sds;
	for ( uint sdsid = 0; sdsid < 4; sdsid++ )
	{
		Sds = SDS.GetSds( sdsid );
		string str = Sds->Read_str( NOTENAMESTR );
		Sds->Write_str( NOTENAMESTR, str );
		cout << "read/write on Sds " << sdsid << " ok: " << str << endl;
	}
	SDS.master->SHMID = 0;
	ClearShm( );
	cout << "read/write on SHM " << 0 << " ok: " << endl;

	SDS.master->SHMID = 1;
	ClearShm( );
	cout << "read/write on SHM " << 1 << " ok: " << endl;
	TEST_END( className );

}

/***********************
 * SDS_struct
 **********************/

SDS_struct::SDS_struct( APPID_e appid, Config_class* Cfg_p, Semaphore_class* Sem_p )
{
	for ( SDSId_t sdsid = 0; sdsid < MAXCONFIG; sdsid++ )
	{
		SharedData_class Sds 		{ appid, sdsid, Cfg_p, Sem_p };
		interface_t* 	sds 		= Sds.addr;
						sds->SDS_Id = sdsid;
		vec.push_back				( sds );
		Vec.push_back				( Sds );
	};

	assert( Vec[0].addr != Vec[1].addr );
	master		= GetSdsAddr( MASTER_SDSID );
	Master		= GetSds	( MASTER_SDSID );
}

SharedData_class* SDS_struct::GetSds( int id )
{
	return &Vec[ id ];
}
interface_t* SDS_struct::GetSdsAddr( int id )
{
	if ( not in_range( sds_range, id ) )
	{
		Exception( "no such Shared Data Segment " + to_string( id ) );
	}
	if( not Vec[id].Datasegment_exists() )
	{
		Exception( "segment not available");
	}

	return vec[ id ];
}
void SDS_struct::Delete()
{
	for( SharedData_class& Sds : Vec )
	{
		Sds.Delete_Shm();
	}
}

/**************************************************
 * EventLog_class
 *************************************************/
EventLog_class::EventLog_class( Dataworld_class* _data ) :
	Logfacility_class("EventLog_class")
{
	DaTA		= _data;
};
EventLog_class::~EventLog_class()
{
	write_log();
};

void EventLog_class::add( uint8_t sdsid, EVENTKEY_e event )
{
	add( { sdsid, event } );
}
void EventLog_class::add( event_t ev )
{
	DaTA->SDS.Vec[ ev.sdsid ].Eventque.Add( ev.event );
	if ( capture_flag)
		rawlog_vec.push_back( ev );
}
void EventLog_class::write_log()
{
	fstream File;
	File.open( DaTA->Cfg_p->fs->reclog_file, fstream::out );
	for( event_t ev : rawlog_vec )
	{
		File << dec << (int)ev.sdsid << ":" << (int)ev.event << endl;
	}
}
void EventLog_class::spool()
{
	fstream 		File;
	String Str		{""};
	string_vec_t 	arr;
	event_t			ev;
	File.open( DaTA->Cfg_p->fs->reclog_file, fstream::in );
	if ( File.is_open() )
	{
		while( getline( File, Str.Str))
		{
			arr = Str.to_array(':');
			ev.sdsid = (uint8_t)Str.secure_stoi( arr[0]);
			ev.event = (EVENTKEY_e)Str.secure_stoi( arr[1]);
			add( ev );
		}
	}

}
bool EventLog_class::capture( uint8_t sdsid, bool flag )
{

	capture_flag = flag;
	SharedData_class* Sds = DaTA->SDS.GetSds( sdsid );
	if ( capture_flag )
	{
		capture_state = CAPTURING;
		rawlog_vec.clear();
		Sds->Dump_ifd( );
		Sds->Copy_dumpFileTo( DaTA->Cfg_p->fs->session_dump_file );
		Sds->Eventque.Reset();
		DaTA->EmitEvent( NEWINSTRUMENTFLAG );
	}
	else
	{
		capture_state = SPOOLING;
		Sds->Copy_dumpFileFrom( DaTA->Cfg_p->fs->session_dump_file );
		Sds->Restore_ifd();
		write_log();
		spool();
		DaTA->EmitEvent( NEWINSTRUMENTFLAG );
		capture_state = CAPTURE;
	}
	return flag;
}


/**************************************************
 * Interface_base
 * provide uniform data access point names as well as
 * the uniform change of the access point
 *************************************************/

Interface_base::Interface_base( Dataworld_class* data )
{
	this->DaTA_p		= data;
	this->sds_master 	= data->SDS.master;
	this->Sds_master	= data->SDS.Master;
	this->fs			= data->Cfg_p->fs;
	this->SDS_ID		= data->SDS_Id;
	Set_Sds				( SDS_ID );
};

void Interface_base::Set_Sds( SDSId_t sdsid )
{	// assign sds local
	this->SDS_ID		= sdsid;
	this->Sds 			= DaTA_p->SDS.GetSds( sdsid );
	this->sds_p 		= Sds->addr;
}
void Interface_base::Set_Sds()
{	// set sds from ifd
	this->Sds 			= DaTA_p->SDS.GetSds( sds_master->comId );
	this->sds_p 		= Sds->addr;
	this->SDS_ID		= sds_p->SDS_Id;
}
void Interface_base::Set_masterComId( SDSId_t sdsid )
{
	sds_master->comId	= sdsid;
}
interface_t* Interface_base::Get_sdsCom()
{
	return DaTA_p->SDS.GetSdsAddr( sds_master->comId );
}
void Interface_base::Set_comid( SDSId_t sdsid )
{	// assign sds global
	this->SDS_ID		= sdsid;
	this->Sds 			= DaTA_p->SDS.GetSds( sdsid );
	this->sds_p 		= Sds->addr;
	Set_masterComId		( sdsid );
}



