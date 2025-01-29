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
#include <Keys.h>
#include <Spectrum.h>
#include <Logfacility.h>
#include <Version.h>
#include <data/Semaphore.h>
#include <data/SharedDataSegment.h>
#include <data/Register.h>
#include <data/Memory.h>

static const uint STATE_MAP_SIZE = LASTNUM;


class Interface_class : virtual public Logfacility_class
{
public:

	interface_t 			ifd_data;
	Shm_base				SHM{ sizeof( ifd_data )};
	interface_t* 			addr		= nullptr;
	shm_ds_t				ds			= shm_data_struct();
	Semaphore_class*		Sem_p		= nullptr;
	Config_class*			Cfg_p		= nullptr;
	uint					Type_Id		= NOID;


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
	void 	Reset_ifd();
	void 	Announce(  );
	void 	Set( char& key, char value);
	void 	Set( bool& key, bool value);
	void 	Set( uint8_t& key, uint8_t value);
	void 	Set( uint16_t& key, uint16_t value);
	void 	Set( float& key, float value);
	string 	Decode( uint8_t idx );
	uint8_t* Getstate_ptr( uint TypeID );
	void 	State_pMap();

	template < typename K, typename V >
	void _Set( K& key, V value )
	{
		if ( reject( addr->Composer, Type_Id ) ) return;
		key = value;
	};

private:
	string			dumpFile		= "";
	size_t			sds_size		= sizeof( ifd_data );
	Spectrum_base	GUIspectrum 	{};
	vector<string>	Waveform_vec	{};
	char 			previous_status = OFFLINE;
	array<string, STATE_MAP_SIZE>
					state_map {""};
	array<uint8_t*,APP_SIZE>
							state_p_map	{};

	Spectrum_base			Spectrum 		{};
	bool 	reject(char status, int id );
	void	stateMap();


};



#endif /* GUIINTERFACE_H_ */
