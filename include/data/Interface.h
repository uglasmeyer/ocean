/*
 * GUIinterface.h
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */


#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

#include <Ocean.h>

enum {
	 OFFLINE,
	 RUNNING	,
	 RECORD		,
	 STOPRECORD ,
	 SENDDATA 	,
	 FREERUN	,
	 BLOCKDATA	,
	 RELEASEDATA,
	 UPDATEGUI 	,
	 STORESOUND	,   // obsolete
	 SYNC 		,
	 DEFAULT	,
	 EXITSERVER	,
	 KEYBOARD	,
	 UPDATE		,
	 LASTNUM
};
static const uint CODE_MAP_SIZE = LASTNUM;

#include <Keys.h>
#include <Synthmem.h>
#include <Spectrum.h>
#include <Logfacility.h>
#include <Version.h>
#include <data/Semaphore.h>
#include <data/SharedDataSegment.h>




class Interface_class : virtual public Logfacility_class, Shm_base
{
public:

	typedef struct shm_info_struct
	{
		buffer_t 	size;
		key_t		key;
		int 		id;
		void* 		addr;
	} shm_info_t;
	shm_info_t				shm_info;

	interface_t 			ifd_data;
	interface_t* 			addr		= nullptr;
	shm_ds_t				ds			= shm_data_struct();
	Semaphore_class*		Sem_p		= nullptr;
	uint8_t					client_id 	= NOID;

	Interface_class( Config_class*, Semaphore_class* );
	virtual ~Interface_class();

	void	Setup_SDS( key_t key );
	void 	Write_arr( const wd_arr_t& arr );
	void 	Write_str( char, string );
	string 	Read_str( char );
	void 	Commit();
	void 	Update( char );
	void 	Show_interface();
	void	Dump_ifd();
	bool 	Restore_ifd();
	void 	Reset_ifd();
	void 	Announce( uint, uint8_t* );
	void 	Set( bool& key, bool value);
	void 	Set( uint8_t& key, uint8_t value);
	void 	Set( uint16_t& key, uint16_t value);
	void 	Set( float& key, float value);
	string 	Decode( uint8_t idx );

private:

	size_t			sds_size		= sizeof( ifd_data );
	Spectrum_base	GUIspectrum 	{};
	vector<string>	Waveform_vec	{};
	char 			previous_status = OFFLINE;
	array<string, CODE_MAP_SIZE>
					code_str_arr;

	Spectrum_base			Spectrum 		{};
//	void*	buffer( buffer_t, key_t );
	bool 	reject(char status, int id );
	void 	setup_code_arr();
};



#endif /* GUIINTERFACE_H_ */
