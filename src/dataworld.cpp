/*
 * dataworld.cpp
 *
 *  Created on: Sep 30, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include <data/DataWorld.h>

/**************************************************
 * Dataworld_class
 *************************************************/
Dataworld_class::Dataworld_class( 	Config_class* cfg,
									Semaphore_class* sem )
	:Logfacility_class( "Dataworld_class")
	,SDS( cfg->Process.AppId, cfg, sem )
	,Appstate( cfg->Process.AppId, SDS.vec )
{

	className = Logfacility_class::className;
	this->AppId	= cfg->Process.AppId;
	this->Cfg_p = cfg;
	this->Sem_p = sem;

	SDS_Id 		= Appstate.SDSid;
	if( SDS_Id < 0 )
	{
		Exception( "cannot initialize data world ", SIGILL );
	}
	Sds_master	= SDS.Master;
	sds_master 	= SDS.master;//(interface_t*) SdsVec.vec[0].ds.addr;
	Sds_p 		= SDS.GetSds( SDS_Id );

	if ( Appstate.Is_dataproc( AppId ) )
	{
		Comment(INFO,"Attaching stereo buffers");

		init_Shm( SHM_0, Cfg_p->Config.SHM_keyl, 0 );
		ShmAddr_0 = (Stereo_t*) SHM_0.ds.addr;

		init_Shm( SHM_1, Cfg_p->Config.SHM_keyr, 1 );
		ShmAddr_1 = (Stereo_t*) SHM_1.ds.addr;
	}
}

Interface_class* Dataworld_class::GetSds(  )
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
	if ( Appstate.Is_dataproc( AppId) )
	{
		SHM_0.Detach( SHM_0.ds.addr );
		SHM_1.Detach( SHM_1.ds.addr );
	}
	DESTRUCTOR( className );
}


void Dataworld_class::init_Shm( Shared_Memory& SHM, key_t key, uint idx )
{
	// Shared Memory
	SHM.Stereo_buffer( key );
	SHM.ds.Id = idx;
	SHM.ShowDs( SHM.ds );
}


void Dataworld_class::ClearShm( const buffer_t& frames )
{
	int 			shm_id 	= sds_master->SHMID;
	( shm_id == 0 ) ? SHM_0.Clear( frames ) : SHM_1.Clear( frames );
}

Stereo_t* Dataworld_class::GetShm_addr( ) // Synthesizer
{
	uint8_t			shm_id 	= sds_master->SHMID;
	Stereo_t* 		addr 	= ( shm_id == 0 ) ? ShmAddr_1 : ShmAddr_0;
	return addr;

}

Stereo_t* Dataworld_class::SetShm_addr() // Audioserver
{
	Stereo_t* 		addr;
	uint8_t			shm_id 	= sds_master->SHMID;

	shm_id 	= ( shm_id + 1 ) % 2;
	addr 	= (	shm_id == 0 ) ? ShmAddr_0 : ShmAddr_1;
	sds_master->SHMID 	= shm_id;

	return addr;
}

void Dataworld_class::EmitEvent( const uint8_t flag, string comment )
{
	if ( not Appstate.IsRunning( sds_master, GUI_ID ) )
		return;
	sds_master->FLAG = flag;
	Sds_master->Write_str( UPDATELOG_EVENT, comment );
	Sem_p->Release( SEMAPHORE_EVENT );
};


void Dataworld_class::Test_Dataworld()
{
	TEST_START( className );
	Interface_class* Sds;
	for ( uint sdsid = 0; sdsid < 4; sdsid++ )
	{
		Sds = SDS.GetSds( sdsid );
		string str = Sds->Read_str( OTHERSTR_KEY );
		Sds->Write_str( OTHERSTR_KEY, str );
		cout << "read/write on Sds " << sdsid << " ok: " << str << endl;
	}
	sds_master->SHMID = 0;
	ClearShm( min_frames );
	cout << "read/write on SHM " << 0 << " ok: " << endl;

	sds_master->SHMID = 1;
	ClearShm( min_frames );
	cout << "read/write on SHM " << 1 << " ok: " << endl;
	TEST_END( className );

}

/***********************
 * SDS_struct
 **********************/

SDS_struct::SDS_struct( APPID appid, Config_class* Cfg_p, Semaphore_class* Sem_p )
{
	for ( Id_t sdsid = 0; sdsid < MAXCONFIG; sdsid++ )
	{
		Interface_class	Sds 		{ appid, sdsid, Cfg_p, Sem_p };
//		interface_t* 	sds 		= (interface_t*) Sds.ds.addr;
		interface_t* 	sds 		= Sds.addr;
						sds->SDS_Id = sdsid;
		vec  .push_back( sds );
		Vec  .push_back( Sds );
	};

	assert( Vec[0].addr != Vec[1].addr );
	master		= GetSdsAddr( 0 );
	Master		= GetSds( 0 );
}

SDS_struct::~SDS_struct()
{ DESTRUCTOR( className ); }

Interface_class* SDS_struct::GetSds( int id )
{
	return &Vec[ id ];
}
interface_t* SDS_struct::GetSdsAddr( int id )
{
	if (( id < 0) or ( id > (int)MAXCONFIG ))
	{
		Exception( "no such Shared Data Segment ");
	}
	if( not Vec[id].Datasegment_exists() )
	{
		Exception( "segment not available");
	}

	return vec[ id ];
}
void SDS_struct::Delete()
{
	for( Interface_class& Sds : Vec )
	{
		Sds.Delete_Shm();
	}
}

/****************
 * EventLog_class
 ***************/

EventLog_class::EventLog_class( Dataworld_class* _data ) :
	Logfacility_class("EventLog_class")
{
	className 	= Logfacility_class::className;
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
	DaTA->SDS.Vec[ ev.sdsid ].Eventque.add( ev.event );
	if ( capture_flag)
		rawlog_vec.push_back( ev );
}
void EventLog_class::write_log()
{
	fstream File;
	File.open( logfile_name, fstream::out );
	for( event_t ev : rawlog_vec )
	{
		File << dec << (int)ev.sdsid << ":" << (int)ev.event << endl;
	}
}
void EventLog_class::spool()
{
	fstream 		File;
	String Str		{""};
	vector_str_t 	arr;
	event_t			ev;
	File.open( logfile_name, fstream::in );
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
	Interface_class* Sds = DaTA->SDS.GetSds( sdsid );
	if ( capture_flag )
	{
		capture_state = CAPTURING;
		rawlog_vec.clear();
		Sds->Dump_ifd( );
		Sds->Copy_dumpFileTo( DaTA->Cfg_p->fs->session_dump_file );
		Sds->Eventque.reset();
		DaTA->EmitEvent( INSTRUMENTSTR_KEY );
	}
	else
	{
		capture_state = SPOOLING;
		Sds->Copy_dumpFileFrom( DaTA->Cfg_p->fs->session_dump_file );
		Sds->Restore_ifd();
		write_log();
		spool();
		DaTA->EmitEvent( INSTRUMENTSTR_KEY );
		capture_state = CAPTURE;
	}
	return flag;
}


