/*
 * DataWorld.h
 *
 *  Created on: Sep 30, 2024
 *      Author: sirius
 */

#ifndef DATA_DATAWORLD_H_
#define DATA_DATAWORLD_H_


#include <data/Interface.h>
class Dataworld_class :
		virtual Logfacility_class
{
	string className = "";
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
	Register_class			Reg			{ };

	stereo_t* 				ShmAddr_0 	= nullptr;
	stereo_t* 				ShmAddr_1 	= nullptr;
	Interface_class*		Sds_p		= nullptr;
	Interface_class*		Master_Sds_p= nullptr;
	interface_t*			sds_master	= nullptr;


	interface_t* 		GetSdsAddr();
	interface_t* 		GetSdsAddr( int id );
	Interface_class*	GetSds(  );
	Interface_class*	GetSds( int id );

	stereo_t* 			SetShm_addr( ); 			// Audioserver
	stereo_t* 			GetShm_addr( ); 			// Synthesizer
	void 				ClearShm();

	void				Test_Dataworld();

	Dataworld_class( uint id );
	virtual ~Dataworld_class();


private:

	void state_pMap();
	void init_Shm( Shared_Memory& SHM, key_t key, uint idx );
};


class EventLog_class :
	virtual Logfacility_class
{
	string className = "";
	bool capture_flag = false;

public:
	Dataworld_class* DaTA;
	string logfile_name = file_structure().reclog_file;
	struct event_struct
	{
		uint8_t sdsid = 0;
		uint8_t event = 0;
	};
	typedef event_struct event_t;
	vector<event_t> rawlog_vec{};

	EventLog_class( Dataworld_class* _data ) :
		Logfacility_class("EventLog_class")
	{
		className 	= Logfacility_class::className;
		DaTA		= _data;
	};
	~EventLog_class()
	{
		write_log();
	};

	void add( uint8_t sdsid, uint8_t event )
	{
		DaTA->SDS_vec[ sdsid ].Event( event );
		if ( capture_flag)
			rawlog_vec.push_back( { sdsid, event } );
	}
	void write_log()
	{
		fstream File;
		File.open( logfile_name, fstream::out );
		for( event_t ev : rawlog_vec )
		{
			File << dec << (int)ev.sdsid << ":" << (int)ev.event << endl;
		}
	}
	void spool()
	{
		fstream File;
		String Str{""};
		vector_str_t arr;
		event_t	ev;

		File.open( logfile_name, fstream::in );
		while( getline( File, Str.Str))
		{
			arr = Str.to_array(':');
			ev.sdsid = (uint8_t)Str.secure_stoi( arr[0]);
			ev.event = (uint8_t)Str.secure_stoi( arr[1]);
			add( ev.sdsid, ev.event );
		}

	}
	bool capture( uint8_t sdsid, bool flag )
	{

		capture_flag = flag;
		Interface_class* Sds = DaTA->GetSds( sdsid );
		if ( capture_flag )
		{
			rawlog_vec.clear();
			filesystem::remove( file_structure().session_dump_file );
			filesystem::copy( Sds->dumpFile , file_structure().session_dump_file );
		}
		else
		{
			filesystem::remove( Sds->dumpFile );
			filesystem::copy( file_structure().session_dump_file, Sds->dumpFile );
			Sds->Restore_ifd();
			write_log();
			spool();
		}
		return flag;
	}

private:
};



#endif /* DATA_DATAWORLD_H_ */
