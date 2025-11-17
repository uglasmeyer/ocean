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
 * appstate.cpp
 *
 *  Created on: Apr 26, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include <data/Appstate.h>
#define NoSDSID -1

Appstate_class::Appstate_class( APPID 			appid,
								sds_vec_t		_sds_vec ) :
	Logfacility_class("Appstate_class"),
	sdsstate_struct()
{
	className 	= Logfacility_class::className;
	AppId 		= appid;
	AppType		= ( appid == KEYBOARDID ) ? SYNTHID : appid;
	sds_vec 	= _sds_vec ;
	Assert_equal( sds_vec[0]->appstate_arr[SYNTHID].type, SYNTHID );
	SDSid		= assign_sdsid( AppType );
	if ( SDSid == NoSDSID  )
	{
		Comment( INFO, "Application id ", AppId, " is already registered \n"
		"on SDS ", 0L );
		Comment( ERROR, "Cannot get valid SDS ID" ) ;

		sds_vec.clear();
		return;
	}
	sds_master	= sds_vec[0];
	sds			= sds_vec[SDSid];
	Name 		= AppIdName( appid );
	sds->appstate_arr[appid].type = AppType;
};

SdsId_vec_t Appstate_class::allowed_sdsid( APPID appid )
{
	if( assignMasterSds.contains( appid) )
		return { 0 };
	else
		return all_sdsids;
}

Id_t Appstate_class::assign_sdsid( APPID appid ) // returns Sds_Id
{
	SdsId_vec_t	validsdsids = allowed_sdsid( appid );
	for( Id_t sdsid : validsdsids )
	{
		CheckAppstate( sds_vec[sdsid], appid );
		if ( getPid( sds_vec[sdsid], appid ) == NoPID )
			return sdsid;
	}
	return NoSDSID;
}

APPID Appstate_class::Type( interface_t* sds, APPID appid )
{
	APPID _type = appid;//sds->appstate_arr[appid].type;
	return _type;
}

void Appstate_class::Setup( interface_t* _sds, interface_t* _sds_master )
{
	this->sds 			= _sds;
	this->sds_master 	= _sds_master;
}

void Appstate_class::SetState( interface_t* sds, APPID appid, StateId_t state )
{
	sds->appstate_arr[ Type(sds, appid) ].state = state;
}

void Appstate_class::SetPid( interface_t* sds, APPID appid, int pid )
{
	sds->appstate_arr[ Type(sds, appid) ].pid = pid;
}

StateId_t	Appstate_class::GetState( interface_t* sds, APPID appid )
{
	APPID _type = Type(sds, appid);
	Assert_lt( (size_t)_type , sds->appstate_arr.size(), "" );
	return sds->appstate_arr[ _type ].state;
}

string	Appstate_class::GetStateStr( interface_t* sds, APPID appid )
{
	StateId_t state = GetState( sds, appid );
	return sdsstateName( state );
}

int Appstate_class::getPid( interface_t* sds, APPID appid )
{
	return sds->appstate_arr[ Type(sds, appid) ].pid;
}

string	Appstate_class::GetPidStr( interface_t* sds, APPID appid )
{
	return to_string( getPid( sds, appid ) );
}

bool Appstate_class::IsRunning( interface_t* sds, APPID appid )
{
	bool flag = ( RUNNING == GetState( sds, appid ));
	return flag;
}

bool Appstate_class::IsOffline( interface_t* sds, APPID appid )
{
	bool flag = ( OFFLINE == GetState( sds, appid ));
	return flag;
}

bool Appstate_class::IsExitserver( interface_t* sds, APPID appid )
{
	bool flag = ( EXITSERVER == GetState( sds, appid ));
	return flag;
}

bool Appstate_class::IsKeyboard	()
{
	bool flag = ( is_atty and ( this->AppId	== KEYBOARDID ));
	return flag;
}

void Appstate_class::SetRunning(  )
{
	SetState( this->sds, this->AppId, RUNNING );
}

void Appstate_class::Announce( )
{
	Info( "announcing application ", Name );
	SetRunning( );
	SetPid( sds, AppId, getpid() );
	sds_master->UpdateFlag = true;
}

void Appstate_class::SetOffline()
{
	SetOffline( this->sds, this->AppId );
}

void Appstate_class::SetOffline( interface_t* sds, APPID appid )
{
	SetState( sds, appid, OFFLINE );
	SetPid( sds, appid, NoPID );

	sds_master->UpdateFlag 			= true;
}
void Appstate_class::SetExitserver( interface_t* sds, APPID appid )
{
	if ( IsExitserver( sds, appid ) )
		SetState( sds, appid, OFFLINE );
	else
		SetState( sds, appid, EXITSERVER );
}

void Appstate_class::CheckAppstates( interface_t* sds )
{
	for( APPID appid : AppIds )
		CheckAppstate( sds, appid );
}
bool Appstate_class::CheckAppstate( interface_t* sds, APPID appid )
{
	int pid = getPid( sds, appid );
	Comment( DEBUG, "pid; ", pid );

	// check if a pid for this application is registered
	if( pid > NoPID ) // a pid for this appid was found
	{
		if( not Is_running_process( pid ) ) // but this pid is not a running instance
		{
			Comment( WARN, "inconsistent process state corrected ", pid );
			SetPid( sds, appid, NoPID );
		}
		else
			Comment( DEBUG, "running " );
	}

	pid = getPid( sds, appid );
	if ( ( pid == NoPID ) )
	{
		SetState( sds, appid, OFFLINE );
		return true;
	}
	if ( ( pid > NoPID ) and ( IsExitserver( sds, appid ) ) )
	{
		Comment( WARN, "Application is already in exit state" );
		System_execute( " kill " + to_string( pid ), NOEXEPT );
		SetOffline( sds, appid );
		return true;
	}
	return false;
}

void  Appstate_class::Shutdown( interface_t* sds, APPID appid )
{
	Info( "shutting down ", AppIdName( appid ) );
	SetExitserver( sds, appid );
}

void Appstate_class::Shutdown_all( vector< interface_t* >  _sds_vec )
{
	Comment( DEBUG, "Shutting down all instances" );
	for( interface_t* sds : _sds_vec )
	{
		for( APPID appid : AppIds )
		{
			if( IsRunning( sds, appid ) )
			{
				if( not ( appid == SDSVIEWID ) ) //  exclude sdsview
					Shutdown( sds, appid );
			}
		}
	}
}

bool Appstate_class::StartOnceOk( interface_t* sds )
{
	// allow running only one time per sds
	CheckAppstate( sds, AppId );
	pid_t pid = getPid( sds, AppId );
	if ( pid < 0 )
		return true;
	else
	{
		Comment( ERROR, "This Ocean process should only start once per sds" );
		return false;
	}
}

void Appstate_class::SaveStateArr( interface_t* sds )
{
	CheckAppstates( sds );
	backup_state = sds->appstate_arr;
}

void Appstate_class::RestoreStateArr( interface_t* sds )
{
	sds->appstate_arr = backup_state;
	CheckAppstates( sds );
}

int Appstate_class::GetNextSdsId(  ) // external use by GUI
{
	int ID = assign_sdsid( SYNTHID);
	if ( ID > 0 )
	{
		sds->config = ID;  	// master_sds refers to Synthesizer ID
	}
	return ID;
}

bool Appstate_class::Is_dataproc( APPID appid )
{
	return dataProc.contains( appid );
}
