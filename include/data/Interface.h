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

static const uint STATE_MAP_SIZE = LASTNUM;

class EventQue_class :
		virtual Logfacility_class,
		EventPtr_struct
{
public:
	interface_t*	addr		= nullptr;
	eventptr_t		eventptr;
	bool			repeat		= false;
	uint8_t			prev_event	= NULLKEY;

	EventQue_class(  )  : eventptr()
	{};
	virtual ~EventQue_class(){};

	void setup( interface_t* _addr );
	void reset();
	void add( uint8_t event );
	uint8_t get();
	string show();
};


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
	uint					Type_Id		= NOID;
	bool					capture_flag= false;
	string					dumpFile	= "";

	Interface_class( Config_class*, Semaphore_class* );
	virtual ~Interface_class();

	void	Setup_SDS( uint sdsid, key_t key );
	void 	Write_arr( const wd_arr_t& arr );
	void 	Write_str( char, string );
	string 	Read_str( char );
	void 	Commit();
	void 	Update( char );
	void 	Show_interface();
	void	Dump_ifd();
	bool 	Restore_ifd();
	void 	Reset_ifd( );
	void 	Announce(  );
	string 	Decode( uint8_t idx );
	uint8_t* Getstate_ptr( uint TypeID );
	void 	State_pMap();

	template < typename V >
	void Set( V& ref, V value )
	{
		if ( reject( addr->Composer, Type_Id ) ) return;
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
	array<string, STATE_MAP_SIZE>
					state_map {""};
	array<uint8_t*,APP_SIZE>
							state_p_map	{};

	Spectrum_class	Spectrum 		{};

	bool 	reject(char status, int id );
	void	stateMap();


};



#endif /* GUIINTERFACE_H_ */
