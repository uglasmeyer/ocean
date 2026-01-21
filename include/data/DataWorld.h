/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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

#include <Ocean.h>
#include <data/Interface.h>
#include <data/SharedDataSegment.h>
#include <data/Appstate.h>
#include <data/Config.h>
#include <data/Memory.h>
#include <data/Semaphore.h>
#include <data/Sdsbase.h>

typedef vector<Interface_class> 	SDS_Vec_t;

/*********************
 * SDS_struct
 *********************/
typedef struct SDS_struct
{
public:
	SDS_Vec_t 				Vec				{};
	sds_vec_t				vec				{};
	interface_t*			master			;
	Interface_class*		Master			;

							SDS_struct		( APPID appid,
											Config_class* Cfg_p,
											Semaphore_class* Sem_p );
	virtual 				~SDS_struct		() = default;
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
public:

	APPID					AppId				;

	SDS_t			 		SDS 				;
	Appstate_class 			Appstate			;
	Id_t					SDS_Id				;

	Interface_class*		Sds_p				;
	Interface_class*		Sds_master			;
	interface_t*			sds_master			;
	Config_class*			Cfg_p				;
	Semaphore_class*		Sem_p				;
	Shm_base				SHM_l				;
	Shm_base				SHM_r				;
	Stereo_t* 				ShmAddr_l 			;
	Stereo_t* 				ShmAddr_r 			;

	interface_t* 			GetSdsAddr			();
	Interface_class*		GetSds				();
	Stereo_t* 				SetShm_addr			(); // Audioserver
	Stereo_t* 				GetShm_addr			();	// Synthesizer
	void 					ClearShm			();
	void 					EmitEvent			( const uint8_t flag, string comment = ""  );
	void					Test_Dataworld		();

							Dataworld_class		( Config_class* cfg,
												Semaphore_class* sem );
	virtual 				~Dataworld_class	();


private:
	void 					init_shared_data	();

	void 					init_Shm			( Shm_base& SHM,
												key_t key,
												uint idx );
};


/*******************
 * EventLog_class
 *******************/

class EventLog_class :
	virtual Logfacility_class
{
	string className = "";


	Dataworld_class* 	DaTA;
	//	string logfile_name = file_structure().reclog_file;
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
