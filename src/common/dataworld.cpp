/*
 * dataworld.cpp
 *
 *  Created on: Sep 30, 2024
 *      Author: sirius
 */

#include <data/DataWorld.h>

/****************
 * Dataworld_class
 ***************/


Dataworld_class::Dataworld_class( char appId, Config_class* cfg, Semaphore_class* sem ) :
Logfacility_class( "Dataworld_class"),
SDS( appId, cfg, sem ),
Reg( appId, SDS.master ),
Appstate( appId, SDS.vec[Reg.Sds_Id], SDS.master, &Reg )
{

	className = Logfacility_class::className;
	this->AppId	= appId;
	this->Cfg_p = cfg;
	this->Sem_p = sem;

	Sds_master	= SDS.Master;
	sds_master 	= SDS.master;//(interface_t*) SdsVec.vec[0].ds.addr;

	SDS_Id 		= Reg.GetId(  );
	Sds_p 		= GetSds();

	if ( Reg.is_dataproc )
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
	if ( Reg.is_dataproc )
	{
		SHM_0.Detach( SHM_0.ds.addr );
		SHM_1.Detach( SHM_1.ds.addr );
		Reg.Proc_deRegister( );
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
	ClearShm( max_frames );
	cout << "read/write on SHM " << 0 << " ok: " << endl;

	sds_master->SHMID = 1;
	ClearShm( max_frames );
	cout << "read/write on SHM " << 1 << " ok: " << endl;
	TEST_END( className );

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

void EventLog_class::add( uint8_t sdsid, uint8_t event )
{
	add( { sdsid, event } );
}
void EventLog_class::add( event_t ev )
{
	DaTA->SDS.Vec[ ev.sdsid ].Event( ev.event );
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
			ev.event = (uint8_t)Str.secure_stoi( arr[1]);
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
		Sds->Dump_ifd();
		filesystem::copy( 	Sds->dumpFile ,
							file_structure().session_dump_file,
							filesystem::copy_options::overwrite_existing);
		Sds->Eventque.reset();
		DaTA->EmitEvent( INSTRUMENTSTR_KEY );
	}
	else
	{
		capture_state = SPOOLING;
		filesystem::copy( 	file_structure().session_dump_file,
							Sds->dumpFile,
							filesystem::copy_options::overwrite_existing );
		Sds->Restore_ifd();
		write_log();
		spool();
		DaTA->EmitEvent( INSTRUMENTSTR_KEY );
		capture_state = CAPTURE;
	}
	return flag;
}


