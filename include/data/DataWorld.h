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

class Dataworld_class :
		virtual public Logfacility_class
{
	string className = "";
public:

	typedef vector<Interface_class> SDS_vec_t;

	uint					AppId		= NOID;
	int						SDS_Id		= -1;

	Config_class			Cfg			{ "Config" };
	Config_class*			Cfg_p		= &Cfg;
	Semaphore_class			Sem			{ Cfg_p};
	Semaphore_class*		Sem_p		= &Sem;
	SDS_vec_t			 	SDS_vec 	{ };
	Shared_Memory			SHM_0		{ sharedbuffer_size };
	Shared_Memory			SHM_1		{ sharedbuffer_size };

	stereo_t* 				ShmAddr_0 	= nullptr;
	stereo_t* 				ShmAddr_1 	= nullptr;
	Interface_class*		Sds_p		= nullptr;
	Interface_class*		Sds_master= nullptr;
	interface_t*			sds_master	= nullptr;

	Appstate_class 			Appstate;
	Register_class			Reg			;

	interface_t* 		GetSdsAddr();
	interface_t* 		GetSdsAddr( int id );
	Interface_class*	GetSds(  );
	Interface_class*	GetSds( int id );

	stereo_t* 			SetShm_addr( ); 			// Audioserver
	stereo_t* 			GetShm_addr( ); 			// Synthesizer
	void 				ClearShm();

	void 				EmitEvent( const uint8_t flag, string comment = ""  );

	void				Test_Dataworld();

	Dataworld_class( uint id );
	virtual ~Dataworld_class();


private:

	void state_pMap();
	void init_Shm( Shared_Memory& SHM, key_t key, uint idx );
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
