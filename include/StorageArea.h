/**************************************************************************
MIT License

Copyright (c) 2026 Ulrich Glasmeyer

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
 * StorageArea.h
 *
 *  Created on: Sep 29, 2026
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef STORAGEAREA_H_
#define STORAGEAREA_H_


#include <Logfacility.h>
#include <Mixerbase.h>
#include <data/Memory.h>
#include <Wavedisplay.h>

/**************************************************
 * StorageArea_class
 * controls the data structures_
 * StorageArray_t
 * SDS StA_amp_arr, Sta_array, StA_state_arr
 *************************************************/
typedef vector<Storage_class>		StorageArray_t;

class StorageArea_class :
	virtual public 		Logfacility_class
{
	Config_class*		Cfg_p			;
	SDSId_t				SDS_ID			;
	interface_t*		sds_p			;

public:
	sta_role_map		Role_map		;
	StorageArray_t		Array			;
	Wavedisplay_class*	Wd_p			;

						StorageArea_class( interface_t* sds, Config_class* cfg, Wavedisplay_class* wd );
	virtual 			~StorageArea_class();
	void 				StA_Wdcursor( RoleId_e roleid );
	void 				DumpStA( Storage_class& sta );
	void 				ResetStA( const StAId_e& staid );
	void 				RestoreStAs();
	void 				SetStA_Files( string subdir );
	void 				SetStAProperties( StAId_e staId );

private:

	bool 				restoreStA( Storage_class& sta );
	void 				setStAs();
	void 				set_sds_values( StAId_e staid );

};


#endif /* STORAGEAREA_H_ */
