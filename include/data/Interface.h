/*
 * GUIinterface.h
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */


#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

#include <Ocean.h>
#include <Config.h>
#include <Spectrum.h>
#include <Logfacility.h>
#include <Version.h>
#include <data/Semaphore.h>
#include <data/SharedDataSegment.h>
#include <data/Register.h>
#include <data/Memory.h>
#include <EventKeys.h>
#include <data/EventQue.h>

static const uint STATE_MAP_SIZE = LASTNUM;



class Interface_class :
		virtual public Logfacility_class
{
	Frequency_class 		Frequency 		{};

public:

	interface_t 			ifd_data 	= interface_struct();
	Shm_base				SHM{ sizeof( ifd_data )};
	interface_t* 			addr		= nullptr;
	shm_ds_t				ds			= shm_data_struct();
	Semaphore_class*		Sem_p		= nullptr;
	Config_class*			Cfg_p		= nullptr;
	EventQue_class			Eventque	{};
	uint					AppId		= NOID;
	bool					capture_flag= false;
	string					dumpFile	= "";

	Interface_class( Config_class*, Semaphore_class* );
	virtual ~Interface_class();

	void	Setup_SDS( uint sdsid, key_t key );
	void 	Write_arr( const wd_arr_t& arr );
	void 	Write_str( char, string );
	string 	Read_str( char );
	void 	Commit();
	void 	Show_interface();
	void	Dump_ifd();
	bool 	Restore_ifd();
	void 	Reset_ifd( );
	string 	Decode( uint8_t idx );

	template < typename V >
	void Set( V& ref, V value )
	{
		if ( reject( addr->Composer, AppId ) ) return;
		ref = value;
	};
	void Event( uint8_t event )
	{
		Eventque.add( event );
	}
	void Test_interface();


private:
	size_t			sds_size		= sizeof( ifd_data );
	char 			previous_status = OFFLINE;
	static const uint		map_size = LASTNUM ;
	typedef 	array< string, LASTNUM>		state_map_t ;

	state_map_t state_map {""};
	Spectrum_class	Spectrum 		{};

	bool 	reject(char status, int id );
	void	initStateMap();


};



#endif /* GUIINTERFACE_H_ */
