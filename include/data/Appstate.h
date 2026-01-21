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

class Appstate_class :
	virtual public Logfacility_class,
	virtual public sdsstate_struct,
	virtual	public AppMap_struct

{
public:
	string			Name 				= "";
	APPID 			AppId				;
	APPID 			AppType				;
	int8_t			SDSid				;
	interface_t* 	sds 				= nullptr;
	interface_t* 	sds_master			= nullptr;
	sds_vec_t		sds_vec				{};

	const set<APPID>assignMasterSds 	{ SDSVIEWID,
											AUDIOID,
											GUI_ID,
											RTSPID,
											COMPID,
											KEYBOARDID,
											TESTPRGID } ;
	SdsId_vec_t		all_sdsids			= Iota_T<Id_t>( 0, MAXCONFIG );

	appstate_arr_t	backup_state		{ };

					Appstate_class		( 	APPID appid,
											sds_vec_t sds_vec
										);

	virtual			~Appstate_class		()
						{ DESTRUCTOR( className );};

	void 			Setup				( interface_t* _sds, interface_t* _sds_master );
	APPID 			Type				( interface_t* sds, APPID appid );
	void 			Announce			( );
	bool 			StartOnceOk			( interface_t* sds );

	void			SetState			( interface_t* sds, APPID appid, StateId_t state );
	void			SetPid				( interface_t* sds, APPID appid, int pid );
	string			GetStateStr				( interface_t* sds, APPID appid );
	string			GetPidStr			( interface_t* sds, APPID appid );
	int				getPid				( interface_t* sds, APPID appid );

	bool			IsRunning  			( interface_t* sds, APPID appid );
	bool			IsOffline  			( interface_t* sds, APPID appid );
	bool 			IsExitserver		( interface_t* sds, APPID appid );

	void 			SetRunning			( );
	void 			SetOffline			( );
	void 			SetOffline			( interface_t* sds, APPID appid );
	void 			SetExitserver		( interface_t* sds, APPID appid );

	void			SaveStateArr			( interface_t* sds );
	void			RestoreStateArr		( interface_t* sds );
	void 			CheckAppstates		( interface_t* sds );
	bool 			CheckAppstate		( interface_t* sds, APPID appid );

	void			Shutdown_all		( sds_vec_t sds_vec );
	void  			Shutdown			( interface_t* sds, APPID appid );
	StateId_t		GetState			( interface_t* sds, APPID appid );

	int 			GetNextSdsId		(  ); //find next synthesizer SDS

private:
	Id_t 			assign_sdsid		( APPID appid ); // returns Sds_Id
	SdsId_vec_t	 	allowed_sdsid		( APPID appid );

};

#endif /* DATA_APPSTATE_H_ */
