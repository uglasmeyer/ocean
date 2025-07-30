/*
 * DataWorld.h
 *
 *  Created on: Sep 30, 2024
 *      Author: sirius
 */

#ifndef DATA_DATAWORLD_H_
#define DATA_DATAWORLD_H_


#include <data/Interface.h>
#include <data/Appstate.h>

typedef struct SDS_struct
{
	vector<Interface_class> 	Vec			{};
	vector<interface_t*>		vec			{};

	interface_t*			master	= nullptr;
	Interface_class*		Master	= nullptr;

	SDS_struct( char appid, Config_class* Cfg_p, Semaphore_class* Sem_p )
	{
		for ( uint8_t sdsid = 0; sdsid < MAXCONFIG; sdsid++ )
		{
			Interface_class
			Sds 		{ appid, sdsid, Cfg_p, Sem_p };
			interface_t* sds = (interface_t*) Sds.ds.addr;
			sds->SDS_Id = sdsid;
			vec.push_back( sds );
			Vec.push_back( Sds );
		};
		assert( Vec[0].ds.addr != Vec[1].ds.addr );
		master 		= vec[0];
		Master		= &Vec[0];
	}
	virtual ~SDS_struct() //= default;
			{ if( LogMask[ DEBUG ] ) coutf << "~SDS_struct" << endl; }

	Interface_class* GetSds( int id )
	{
		return &Vec[ id ];
	}
	interface_t* GetSdsAddr( int id )
	{
		if (( id<0) or ( id > (int)MAXCONFIG ))
		{
			EXCEPTION( "no such Shared Data Segment ");
		}
		if( not Vec[id].ds.eexist )
		{
			EXCEPTION( "segment not available");
		}

		return vec[ id ];
	}
	void Delete()
	{
		for( Interface_class& Sds : Vec )
		{
			Sds.SHM.Delete();
		}
	}


} SDS_t;


class Dataworld_class :
		virtual public Logfacility_class
{
	string className = "";
public:


	char					AppId		= NOID;
	int						SDS_Id		= -1;

	Shared_Memory			SHM_0		{ Shared_Memory::sharedbuffer_size };
	Shared_Memory			SHM_1		{ Shared_Memory::sharedbuffer_size };

	SDS_t			 		SDS ;
	Register_class			Reg	;
	Appstate_class 			Appstate;


	Stereo_t* 				ShmAddr_0 	= nullptr;
	Stereo_t* 				ShmAddr_1 	= nullptr;
	Interface_class*		Sds_p		= nullptr;
	Interface_class*		Sds_master	= nullptr;
	interface_t*			sds_master	= nullptr;
	Config_class*			Cfg_p		= nullptr;
	Semaphore_class*		Sem_p		= nullptr;

	interface_t* 			GetSdsAddr();
	Interface_class*		GetSds( );

	Stereo_t* 				SetShm_addr( ); 			// Audioserver
	Stereo_t* 				GetShm_addr( ); 			// Synthesizer
	void 					ClearShm( const buffer_t& frames );

	void 					EmitEvent( const uint8_t flag, string comment = ""  );

	void					Test_Dataworld();

							Dataworld_class( char id, Config_class* cfg, Semaphore_class* sem );
	virtual 				~Dataworld_class();


private:

	void 					state_pMap();
	void 					init_Shm( Shared_Memory& SHM, key_t key, uint idx );
};


/*
 * EventLog_class
 */

class EventLog_class :
	virtual Logfacility_class
{
	string className = "";


	Dataworld_class* 	DaTA;
	string logfile_name = file_structure().reclog_file;
	struct event_struct
	{
		uint8_t sdsid = 0;
		uint8_t event = 0;
	};
	typedef event_struct event_t;
	vector<event_t> 	rawlog_vec		{};

public:

	uint 				capture_state 	= CAPTURE;
	bool 				capture_flag 	= false;
	enum { CAPTURE, CAPTURING, SPOOL, SPOOLING };

	EventLog_class( Dataworld_class* _data );
	virtual ~EventLog_class();

	void add( uint8_t sdsid, uint8_t event );
	void write_log();
	void spool();
	bool capture( uint8_t sdsid, bool flag );

private:
	void add( event_t ev );
};



#endif /* DATA_DATAWORLD_H_ */
