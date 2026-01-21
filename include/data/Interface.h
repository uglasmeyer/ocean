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
 * GUIinterface.h
 *
 *  Created on: Jan 14, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */


#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <data/SharedDataSegment.h>
#include <EventKeys.h>
#include <data/Semaphore.h>
#include <data/EventQue.h>
#include <data/Config.h>

class Interface_class
	: virtual public 		Logfacility_class
	, 						sdsstate_struct
{
	string					className			;
	APPID					AppId				;
	key_t					sds_key				;
	Semaphore_class*		Sem_p				;
	Config_class*			Cfg_p				;
	file_structure*			fs					;
	interface_t 			ifd_data 			;
	size_t					sds_size			;
	Shm_base				SHM					;
	shm_ds_t				shm_ds				;
	string					dumpFile			;
	string					filename			;

public:
	EventQue_class			Eventque			;
	interface_t* 			addr				;

							Interface_class		( APPID appid,
												Id_t sdsid,
												Config_class*,
												Semaphore_class* );
	virtual 				~Interface_class	();


	void					Setup_SDS			( Id_t sdsid, key_t key );
	void 					Set_filename		( string dir, uint8_t sdsid );
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
	template < typename V >
	void Set( volatile V& ref, V value )
	{
		if ( reject( AppId ) )
			return;
		ref = value;
	};

private:
	bool 					reject				( APPID id );
	void					selfTest			();

};



#endif /* INTERFACE_H_ */
