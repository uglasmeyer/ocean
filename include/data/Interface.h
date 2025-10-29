/*
 * GUIinterface.h
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */


#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

#include <Logfacility.h>
#include <data/Config.h>
#include <data/Sdsbase.h>
#include <data/SharedDataSegment.h>
#include <data/Memorybase.h>
#include <EventKeys.h>

#include <Wavedisplay_base.h>
#include <data/Semaphore.h>
#include <data/EventQue.h>

class Interface_class
	: virtual public 		Logfacility_class
	, 						sdsstate_struct
{
	string					className	= "";
public:
	interface_t 			ifd_data 	= interface_struct();
	EventQue_class			Eventque	{};
	Shm_base				SHM			{ sizeof( ifd_data ) };
	interface_t* 			addr		= nullptr;
	shm_ds_t				ds			= shm_data_struct();
	Semaphore_class*		Sem_p		= nullptr;
	Config_class*			Cfg_p		= nullptr;
	file_structure*			fs			= nullptr;
	APPID					AppId		= NoAPPID;
	bool					capture_flag= false;
	string					dumpFile	= "";

			Interface_class( APPID appid, Id_t sdsid,  Config_class*, Semaphore_class* );
	virtual ~Interface_class();

	void	Setup_SDS( Id_t sdsid, key_t key );
	bool 	reject		( APPID id );
	void 	Write_arr( const wd_arr_t& arr );
	void 	Write_str( char, string );
	string 	Read_str( EVENTKEY_t );
	void 	Commit();
	void	Dump_ifd();
	bool 	Restore_ifd();
	void 	Reset_ifd( );

	template < typename V >
	void Set( V& ref, V value )
	{
		if ( reject( AppId ) )
			return;
		ref = value;
	};
	void Event( EVENTKEY_t event );
	void Test_interface();
	void					selfTest		( );


private:
	size_t					sds_size		= sizeof( ifd_data );

};



#endif /* GUIINTERFACE_H_ */
