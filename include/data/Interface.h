//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


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
