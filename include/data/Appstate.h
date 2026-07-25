/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * Appstate.h
 *
 *  Created on: Mar 30, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_APPSTATE_H_
#define DATA_APPSTATE_H_


#include <data/Configbase.h>
#include <data/SharedDataSegment.h>
#include <Logfacility.h>
#include <data/Sdsbase.h>

/**************************************************
 * Appstate_class
 * manage content of appstate_arr in all Shared Date Segments
 *************************************************/
class Appstate_class :
	virtual public	Logfacility_class,
	virtual public	sdsstate_struct,
	virtual	public	AppMap_struct
{
	interface_t* 	sds 				;
	interface_t* 	sds_master			;
	APPID_e 		AppId				;
	APPID_e 		AppType				;
	SdsId_vec_t		all_sdsids			;
	sds_vec_t		sds_vec				{};
	appstate_arr_t	backup_state		{};

public:
	string			Name 				;
	int8_t			SDSid				;

	const set<APPID_e>
					UniqueRunApp	=
					{
						SDSVIEWID,
						AUDIOID,
						GUI_ID,
						RTSPID,
						COMPOSERID,
						SETUPID,
						TESTPRGID,
						XMLEDITORID,
						TESTPRGID
					} ;

	const set<APPID_e>
					MultiRunApp		=
					{
						SYNTHID,
						KEYBOARDID
					};

					Appstate_class		( APPID_e appid, sds_vec_t sds_vec );
	virtual			~Appstate_class		();
	void 			Announce			();
	void 			CheckAppstates		( interface_t* sds );
	bool 			CheckAppstate		( interface_t* sds, APPID_e appid );
	Id_t			Find_running		( APPID_e appid );
	int				GetPid				( interface_t* sds, APPID_e appid );
	string			GetPidStr			( interface_t* sds, APPID_e appid );
	StateId_e		GetState			( interface_t* sds, APPID_e appid );
	string			GetStateStr			( interface_t* sds, APPID_e appid );
	bool 			IsInactive			();
	bool			IsOffline  			( interface_t* sds, APPID_e appid );
	bool 			IsExitserver		( interface_t* sds, APPID_e appid );
	bool			IsRunning  			( interface_t* sds, APPID_e appid );
	bool			IsRunning  			();
	bool			IsSleeping 			();
	void			RestoreStateArr		( interface_t* sds );
	void			SaveStateArr		( interface_t* sds );
	void 			SetExitserver		( interface_t* sds, APPID_e appid );
	void			SetInactive			();
	void 			SetOffline			();
	void 			SetOffline			( interface_t* sds, APPID_e appid );
	void			SetPid				( interface_t* sds, APPID_e appid, int pid );
	void 			SetRunning			( interface_t* sds, APPID_e appid );
	void 			SetRunning			();
	void 			SetSleeping			();
	void 			Setup				( interface_t* _sds, interface_t* _sds_master );
	void			Shutdown_all		();
	void  			Shutdown			( interface_t* sds, APPID_e appid );
	APPID_e 		Type				( interface_t* sds, APPID_e appid );

private:
	Id_t 			assign_sdsid		( APPID_e appid ); // returns Sds_Id
	SdsId_vec_t	 	allowed_sdsid		( APPID_e appid );
	void			setState			( interface_t* sds, APPID_e appid, StateId_e state );
};

#endif /* DATA_APPSTATE_H_ */
