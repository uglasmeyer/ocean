/*
 * DataWorld.h
 *
 *  Created on: Sep 30, 2024
 *      Author: sirius
 */

#ifndef DATA_DATAWORLD_H_
#define DATA_DATAWORLD_H_

#include <Ocean.h>
#include <data/Interface.h>
#include <data/SharedDataSegment.h>
#include <data/Appstate.h>
#include <data/Config.h>
#include <data/Semaphore.h>
#include <data/Memory.h>
#include <data/Sdsbase.h>

typedef vector<Interface_class> SDS_vec_t;

/*********************
 * SDS_struct
 *********************/
typedef struct SDS_struct
{
private:
	string					className 		= "SDS_struct";
public:
	SDS_vec_t 				Vec				{};
	sds_vec_t				vec				{};
	interface_t*			master			= nullptr;
	Interface_class*		Master			= nullptr;

							SDS_struct		( APPID appid,
											Config_class* Cfg_p,
											Semaphore_class* Sem_p );
	virtual 				~SDS_struct		();
	Interface_class* 		GetSds			( int id );
	interface_t* 			GetSdsAddr		( int id );
	void 					Delete			();

} SDS_t;

/*********************
 * Dataworld_class
 *********************/

class 	Dataworld_class :
		virtual public Logfacility_class
{
	string 					className 	= "";
public:

	APPID					AppId				= NoAPPID;
	Id_t					SDS_Id				= -1;

	Shared_Memory			SHM_0				{ Shared_Memory::sharedbuffer_size };
	Shared_Memory			SHM_1				{ Shared_Memory::sharedbuffer_size };

	SDS_t			 		SDS ;
	Appstate_class 			Appstate;

	Stereo_t* 				ShmAddr_0 			= nullptr;
	Stereo_t* 				ShmAddr_1 			= nullptr;
	Interface_class*		Sds_p				= nullptr;
	Interface_class*		Sds_master			= nullptr;
	interface_t*			sds_master			= nullptr;
	Config_class*			Cfg_p				= nullptr;
	Semaphore_class*		Sem_p				= nullptr;

	interface_t* 			GetSdsAddr			();
	Interface_class*		GetSds				( );
	Stereo_t* 				SetShm_addr			( ); 			// Audioserver
	Stereo_t* 				GetShm_addr			( ); 			// Synthesizer
	void 					ClearShm			( const buffer_t& frames );
	void 					EmitEvent			( const uint8_t flag, string comment = ""  );
	void					Test_Dataworld		();

							Dataworld_class		( APPID appid,
												Config_class* cfg,
												Semaphore_class* sem );
	virtual 				~Dataworld_class	();


private:

	void 					init_Shm( Shared_Memory& SHM, key_t key, uint idx );
};


/*******************
 * EventLog_class
 *******************/

class EventLog_class :
	virtual Logfacility_class
{
	string className = "";


	Dataworld_class* 	DaTA;
	string logfile_name = file_structure().reclog_file;
	struct event_struct
	{
		uint8_t 	sdsid = 0;
		EVENTKEY_t	event = NULLKEY;
	};
	typedef event_struct event_t;
	vector<event_t> 	rawlog_vec		{};

public:

	uint 				capture_state 	= CAPTURE;
	bool 				capture_flag 	= false;
	enum { CAPTURE, CAPTURING, SPOOL, SPOOLING };

	EventLog_class( Dataworld_class* _data );
	virtual ~EventLog_class();

	void add( uint8_t sdsid, EVENTKEY_t event );
	void write_log();
	void spool();
	bool capture( uint8_t sdsid, bool flag );

private:
	void add( event_t ev );
};



#endif /* DATA_DATAWORLD_H_ */
