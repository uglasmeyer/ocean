/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * DataWorld.h
 *
 *  Created on: Sep 30, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_DATAWORLD_H_
#define DATA_DATAWORLD_H_

#include <data/Sdsbase.h>
#include <data/SharedDataSegment.h>
#include <data/Interface.h>
#include <data/Appstate.h>
#include <data/Config.h>
#include <data/Memory.h>
#include <data/Semaphore.h>

typedef vector<SharedData_class> 	SDS_Vec_t;

/**************************************************
 * SDS_struct
 * provide access to the physical Shared Datasegment in terms of Vec and vec
 *************************************************/
typedef struct SDS_struct
{
public:
	SDS_Vec_t 				Vec				{};
	sds_vec_t				vec				{};
	interface_t*			master			;
	SharedData_class*		Master			;

							SDS_struct		( APPID_e appid,
											Config_class* Cfg_p,
											Semaphore_class* Sem_p );
	virtual 				~SDS_struct		() = default;
	SharedData_class* 		GetSds			( int id );
	interface_t* 			GetSdsAddr		( int id );
	void 					Delete			();

} SDS_t;

/**************************************************
 * Dataworld_class
 * Connect the physical data structures
 *************************************************/
class 	Dataworld_class :
		virtual public Logfacility_class
{
public:

	APPID_e					AppId				;
	SDS_t			 		SDS 				;
	Appstate_class 			Appstate			;
	SDSId_t					SDS_Id				;

	SharedData_class*		Sds_p				;
	Config_class*			Cfg_p				;
	Semaphore_class*		Sem_p				;
	Shm_base				SHM_l				;
	Shm_base				SHM_r				;
	Stereo_t* 				ShmAddr_l 			;
	Stereo_t* 				ShmAddr_r 			;
	string					Test_result			;

							Dataworld_class		( Config_class* cfg,
												Semaphore_class* sem );
	virtual 				~Dataworld_class	();
	interface_t* 			GetSdsAddr			();
	SharedData_class*		GetSds				();
	Stereo_t* 				SetShm_addr			(); // Audioserver receive next data point
	Stereo_t* 				GetShm_addr			();	// Synthesizer send current data point
	void 					ClearShm			();
	void 					EmitEvent			( const EVENTFLAG_e flag, string comment = ""  );
	void					Test_Dataworld		();

private:
	void 					init_shared_sound	();
	void 					init_Shm			( Shm_base& SHM,
												key_t key,
												uint idx );
};


/**************************************************
 * Interface_base
 * provide uniform data access point names as well as
 * the uniform change of the access point
 *************************************************/
class Interface_base
	: public virtual Logfacility_class
{
public:
	Dataworld_class*		DaTA_p			;
	fs_t*					fs				;
	SharedData_class*		Sds				;
	SharedData_class*		Sds_master		;
	interface_t*			sds_master		;
	interface_t*			sds_p			;
	SDSId_t					SDS_ID			;

							Interface_base	( Dataworld_class* data );
	virtual 				~Interface_base	() = default;

	interface_t*			Get_sdsCom		();
	void 					Set_Sds			( SDSId_t sdsid );
	void 					Set_Sds			();
	void 					Set_masterComId	( SDSId_t sdsid );
	void 					Set_comid		( SDSId_t sdsid );
};

/**************************************************
 * Sdsmaster_class
 * Interface to the MASTER_SDSID
 *************************************************/
// TODO to be implemented
class Sdsmaster_class :
	virtual public Logfacility_class
{
public:
	Sdsmaster_class()
	{
	};
	virtual ~Sdsmaster_class()
	{
	};



private:
};
/*******************
 * EventLog_class
 *******************/

class EventLog_class :
	virtual Logfacility_class
{

	Dataworld_class* 	DaTA;
	struct event_struct
	{
		uint8_t 	sdsid = 0;
		EVENTKEY_e	event = NULLKEY;
	};
	typedef event_struct event_t;
	vector<event_t> 	rawlog_vec		{};

public:

	uint 				capture_state 	= CAPTURE;
	bool 				capture_flag 	= false;
	enum { CAPTURE, CAPTURING, SPOOL, SPOOLING };

	EventLog_class( Dataworld_class* _data );
	virtual ~EventLog_class();

	void add( uint8_t sdsid, EVENTKEY_e event );
	void write_log();
	void spool();
	bool capture( uint8_t sdsid, bool flag );

private:
	void add( event_t ev );
};



#endif /* DATA_DATAWORLD_H_ */
