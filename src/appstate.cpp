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
 * appstate.cpp
 *
 *  Created on: Apr 26, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include <data/Appstate.h>
#define NoSDSID -1


/**************************************************
 * Appstate_class
 *************************************************/
Appstate_class::Appstate_class( APPID_e 		appid,
								sds_vec_t		_sds_vec ) :
	Logfacility_class("Appstate_class"),
	sdsstate_struct()
{
	AppId 		= appid;
	sds_vec 	= _sds_vec;
	sds_master	= sds_vec[ MASTER_SDSID ];

	all_sdsids	= Iota_T<Id_t>( 0, MAXCONFIG );
	Assert_equal( (int)sds_master->appstate_arr[SYNTHID].type, (int)SYNTHID, "appstate_arr" );

	AppType		= ( AppId == KEYBOARDID ) ? SYNTHID : AppId;
	SDSid		= assign_sdsid( AppType );
	if ( SDSid == NoSDSID  )
	{
		Comment( ERROR, "Cannot get valid SDS ID" ) ;
		raise( SIGILL );
		return;
	}
	sds								= sds_vec[ SDSid ];
	Name 							= AppIdName( AppId );
	sds->appstate_arr[AppId].type 	= AppType;
	SetRunning();
};
Appstate_class::~Appstate_class()
{
	DESTRUCTOR( className );
}

SdsId_vec_t Appstate_class::allowed_sdsid( APPID_e appid )
{
	if( UniqueRunApp.contains( appid) )
		return { MASTER_SDSID };
	else
		return all_sdsids;
}

Id_t Appstate_class::assign_sdsid( APPID_e appid ) // returns Sds_Id
{
	auto get_sdsid = [ this, appid ]( SdsId_vec_t valid_sids  )
	{
		for( Id_t sdsid : valid_sids ) // scan the range that has no pid
		{
			interface_t* sds = sds_vec[ sdsid ];
			CheckAppstate( sds, appid ); // clean up the state if needed
			if ( GetPid( sds, appid ) == NoPID )
				return sdsid;
		}
		string valid_str = show_items( valid_sids );
		Comment( INFO, "Application id", (int)AppId, "is already registered on SDS", valid_str );

		return Id_t( NoSDSID ); // -> SIGILL
	};

	// The composer specifies the sdsid for the synthesizer module
	// in case of composer::autostart (default = yes)

	SdsId_vec_t valid_sids = { MASTER_SDSID };
	if( MultiRunApp.contains ( appid ) )
	{
		if( sds_master->mixer_state.composer )
		{
			valid_sids = { sds_master->comId };
		}
		else
		{
			valid_sids = all_sdsids;
			Id_t sdsid = get_sdsid( valid_sids );
			sds_master->comId = sdsid;
			return sdsid;
		}
	}
	return get_sdsid( valid_sids );
}

APPID_e Appstate_class::Type( interface_t* sds, APPID_e appid )
{
	APPID_e _type = appid;
	return _type;
}

void Appstate_class::Setup( interface_t* _sds, interface_t* _sds_master )
{
	this->sds 			= _sds;
	this->sds_master 	= _sds_master;
}

void Appstate_class::setState( interface_t* sds, APPID_e appid, StateId_e state )
{
	sds->appstate_arr[ Type(sds, appid) ].state = state;
	sds_master->UpdateFlag = true;
}

void Appstate_class::SetPid( interface_t* sds, APPID_e appid, int pid )
{
	sds->appstate_arr[ Type(sds, appid) ].pid = pid;
}

StateId_e	Appstate_class::GetState( interface_t* sds, APPID_e appid )
{
	APPID_e _type = Type(sds, appid);
	Assert_lt( (size_t)_type , sds->appstate_arr.size(), "" );
	return sds->appstate_arr[ _type ].state;
}

string	Appstate_class::GetStateStr( interface_t* sds, APPID_e appid )
{
	StateId_e state = GetState( sds, appid );
	return sdsstateName( state );
}

int Appstate_class::GetPid( interface_t* sds, APPID_e appid )
{
	return sds->appstate_arr[ Type(sds, appid) ].pid;
}

string	Appstate_class::GetPidStr( interface_t* sds, APPID_e appid )
{
	return to_string( GetPid( sds, appid ) );
}

bool Appstate_class::IsRunning()
{
	StateId_e	state	= GetState( this->sds, this->AppId );
	bool 		flag	= ( ( state == RUNNING ) or ( state == SLEEPING ) );
	return		flag;
}

bool Appstate_class::IsRunning( interface_t* sds, APPID_e appid )
{
	StateId_e	state	= GetState( sds, appid );
	bool 		flag	= ( ( state == RUNNING ) or ( state == SLEEPING ) );
	return		flag;
}

Id_t Appstate_class::Find_running( APPID_e appid )
{
	for( interface_t* sds : sds_vec )
	{
		if( IsRunning( sds, appid ) )
		{
			return sds->SDS_Id;
		}
	}
	return MAIN_SYNTHID;
}

bool Appstate_class::IsOffline( interface_t* sds, APPID_e appid )
{
	bool flag = ( StateId_e::OFFLINE == GetState( sds, appid ));
	return flag;
}

bool Appstate_class::IsInactive()
{
	bool flag = ( StateId_e::INACTIVE == GetState( this->sds, this->AppId ));
	return flag;
}

bool Appstate_class::IsSleeping()
{
	bool flag = ( StateId_e::SLEEPING == GetState( this->sds, this->AppId ));
	return flag;
}
bool Appstate_class::IsExitserver( interface_t* sds, APPID_e appid )
{
	bool flag = ( StateId_e::EXITSERVER == GetState( sds, appid ));
	return flag;
}

void Appstate_class::SetRunning( interface_t* sds, APPID_e appid )
{
	setState( sds, appid, StateId_e::RUNNING );
}

void Appstate_class::SetSleeping()
{
	setState( this->sds, this->AppId, StateId_e::SLEEPING );
}

void Appstate_class::SetRunning()
{
	SetRunning( this->sds, this->AppId );
}

void Appstate_class::SetInactive()
{
	StateId_e	state = GetState( sds, this->AppId );
	if( set<StateId_e>{ OFFLINE, EXITSERVER }.contains( state ) )
		return;
	setState( this->sds, this->AppId, INACTIVE );
}

void Appstate_class::Announce()
{
	Info( "announcing application ", Name );
	SetRunning( );
	SetPid( sds, AppId, getpid() );
}

void Appstate_class::SetOffline()
{
	SetOffline( this->sds, this->AppId );
}

void Appstate_class::SetOffline( interface_t* sds, APPID_e appid )
{
	SetPid( sds, appid, NoPID );
	setState( sds, appid, OFFLINE );
}
void Appstate_class::SetExitserver( interface_t* sds, APPID_e appid )
{
	if ( IsExitserver( sds, appid ) )
		SetOffline( sds, appid );
	else
		setState( sds, appid, EXITSERVER );
}

void Appstate_class::CheckAppstates( interface_t* sds )
{
	for( APPID_e appid : AppIds )
		CheckAppstate( sds, appid );
}
bool Appstate_class::CheckAppstate( interface_t* sds, APPID_e appid )
{	// return true if appud is not running
	// vis versa

	// check for inconsistencies and try to correct them
	int pid = GetPid( sds, appid );
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

	pid = GetPid( sds, appid );
	if ( pid == NoPID )
	{
		if( not IsOffline( sds, appid ) )
			SetOffline( sds, appid );
		return true;
	}
	else
	{
		if ( IsExitserver( sds, appid ) )
		{
			Comment( WARN, "Application is already in exit state" );
			System_execute( " kill " + to_string( pid ), NOEXEPT );
			SetOffline( sds, appid );
			return true;
		}
	}
	return false;
}

void  Appstate_class::Shutdown( interface_t* sds, APPID_e appid )
{
	Info( "shutting down ", AppIdName( appid ) );
	SetExitserver( sds, appid );
}

void Appstate_class::Shutdown_all()
{
	Comment( DEBUG, "Shutting down all instances" );
	for( interface_t* sds : sds_vec )
	{
		for( APPID_e appid : AppIds )
		{
			if( IsRunning( sds, appid ) )
			{
				if( not ( appid == SDSVIEWID ) ) //  exclude sdsview
					Shutdown( sds, appid );
			}
		}
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

