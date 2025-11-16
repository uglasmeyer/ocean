/*
 * GUIinterface.h
 *
 *  Created on: Jan 14, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */


#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

#include <data/SharedDataSegment.h>
#include <EventKeys.h>
#include <data/Semaphore.h>
#include <data/EventQue.h>
#include <data/Config.h>

class Interface_class
	: virtual public 		Logfacility_class
	, 						sdsstate_struct
{
	string					className			= "";
	APPID					AppId				= NoAPPID;
	Semaphore_class*		Sem_p				= nullptr;
	Config_class*			Cfg_p				= nullptr;
	file_structure*			fs					= nullptr;
	interface_t 			ifd_data 			= interface_struct();
	shm_ds_t				ds					= shm_data_struct();
	Shm_base				SHM					{ sizeof( ifd_data ) };
	string					dumpFile			= "";


public:
	EventQue_class			Eventque			{}; // considered to be a named class extension
	interface_t* 			addr				= nullptr;
	bool					capture_flag		= false;

							Interface_class		( APPID appid,
												Id_t sdsid,
												Config_class*,
												Semaphore_class* );
	virtual 				~Interface_class	();


	void					Setup_SDS			( Id_t sdsid, key_t key );
	void 					Write_arr			( const wd_arr_t& arr );
	void 					Write_str			( char, string );
	string 					Read_str			( EVENTKEY_e );
	void 					Commit				();
	void					Dump_ifd			();
	bool 					Restore_ifd			();
	void 					Reset_ifd			();
	bool 					Datasegment_exists	();
	void 					Delete_Shm			();
	void					Remove_dumpFile		();
	void					Copy_dumpFileTo		( string dst );
	void					Copy_dumpFileFrom	( string src );
	void 					Test_interface		();

	template < typename V >
	void Set( V& ref, V value )
	{
		if ( reject( AppId ) )
			return;
		ref = value;
	};

private:
	size_t					sds_size			= sizeof( ifd_data );
	bool 					reject				( APPID id );
	void					selfTest			();

};



#endif /* GUIINTERFACE_H_ */
