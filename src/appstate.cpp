/*
 * appstate.cpp
 *
 *  Created on: Apr 26, 2025
 *      Author: sirius
 */


#include <data/Appstate.h>


Appstate_class::Appstate_class( char 			appid,
								interface_t* 	_sds,
								interface_t* 	_sds_master,
								Register_class* reg ) :
	Logfacility_class("Appstate_class"),
	sdsstate_struct()
{
	className 	= Logfacility_class::className;
	Reg_p		= reg;
	AppId 		= appid;
	Name 		= AppIdName( appid );
	if ( ( AppId == APPID::KBDID ) or ( AppId == APPID::SYNTHID ) )
		Setup( _sds			, _sds_master ) ;
	else
		Setup( _sds_master	, _sds_master ) ;

};
void Appstate_class::Setup( interface_t* _sds, interface_t* _sds_master )
{
	this->sds 			= _sds;
	this->sds_master 	= _sds_master;
	SaveState();
}
uint8_t* Appstate_class::appAddr( interface_t* sds, uint appId  )
{
	assert( sds != nullptr);
	uint8_t*	ptr = nullptr;
	switch ( appId )
	{
		case APPID::SYNTHID		: { ptr = &sds->Synthesizer; break; }
		case APPID::AUDIOID		: { ptr = &sds->AudioServer; break; }
		case APPID::COMPID		: { ptr = &sds->Composer; break; }
		case APPID::GUI_ID		: { ptr = &sds->UserInterface; break; }
		case APPID::COMSTACKID	: { ptr = &sds->Comstack; break; }
		case APPID::KBDID		: { ptr = &sds->Keyboard; break; }
		case APPID::SETUPID		: { ptr = &sds->Setup; break; }
		case APPID::RTSPID		: { ptr = &sds->Rtsp; break; }
		case APPID::TESTID		: { ptr = &sds->Rtsp; break; }
		case APPID::NOID		: { ptr = nullptr; break; }
		default : {
					cout << "WARN: unknown application id: " << (int)AppId << endl;
					ptr = nullptr;
					};
	}
	assert( ptr != nullptr);
	return ptr;
}


void Appstate_class::Set( interface_t* sds, uint appid, int state )
{
	*appAddr( sds, appid ) 		= state;
}
int	Appstate_class::Get( interface_t* sds, uint appid )
{
	return *appAddr( sds, appid);
}
string	Appstate_class::GetStr( interface_t* sds, uint appid )
{
	uint state = Get( sds, appid );
	return state_map[state];
}
bool Appstate_class::IsRunning( interface_t* sds, uint appid )
{
//	if(( appid == this->AppId ) and ( this->sds == sds ))
//		return true;
/*	if ( dataProc.contains( appid ) )
	{
		return Is_running_process( (int) sds->process_arr.at( appid + sds->SDS_Id ).pid);
	}*/
	bool flag = ( RUNNING == Get( sds, appid ));
	return flag;
}
bool Appstate_class::IsOffline( interface_t* sds, uint appid )
{
	bool flag = ( OFFLINE == Get( sds, appid ));
	return flag;
}
bool Appstate_class::IsExitserver( interface_t* sds, uint appid )
{
	bool flag = ( EXITSERVER == Get( sds, appid ));
	return flag;
}
bool Appstate_class::IsKeyboard	()
{
	bool flag = ( is_atty and ( this->AppId	== APPID::KBDID ));
	return flag;
}

void Appstate_class::SetRunning(  )
{
	Set( this->sds, this->AppId, RUNNING );
}
void Appstate_class::SetOffline()
{
	Set( sds, AppId, OFFLINE );
	sds_master->UpdateFlag 			= true;
}
void Appstate_class::SetExitserver( interface_t* sds, uint appid )
{
	if ( IsInconsistent( sds, appid ))
		return;
	if ( IsExitserver( sds, appid ) )
		Set( sds, appid, OFFLINE );
	else
		Set( sds, appid, EXITSERVER );
}

bool Appstate_class::IsInconsistent( interface_t* sds, char appid )
{
	int pid = Reg_p->Get_regpid( sds->SDS_Id, appid );
	if ( ( pid < 0 ) and ( IsRunning( sds, appid ) ) )
	{
		Comment( ERROR, "Inconsistent process state detected" );
		Set( sds, appid, OFFLINE );
		return true;
	}
	if ( ( pid > 0 ) and ( IsExitserver( sds, appid ) ) )
	{
		Comment( WARN, "Application is already in exit state" );
		system_execute( " kill " + to_string( pid ) );
		Set( sds, appid, OFFLINE );
		return true;
	}
	return false;
}
void  Appstate_class::shutdown( interface_t* sds, char appid )
{
	Info( "shutting down ", AppIdName( appid ) );
	SetExitserver( sds, appid );
}
void Appstate_class::Shutdown_all( vector< interface_t* >  _sds_vec )
{
	Comment( DEBUG, "Shutting down all instances" );
	for( interface_t* sds : _sds_vec )
	{
		for( char appid = 0; appid < APPID::NOID; appid++ )
		{
			if( IsRunning( sds, appid ) )
			{
				if( not ( appid == APPID::COMSTACKID ) ) //  exclude comstack
					shutdown( sds, appid );
			}
		}
	}
}

bool Appstate_class::StartOnceOk( interface_t* sds )
{

	assert( sds != nullptr );
	bool is_running = ( Get( sds, this->AppId) == RUNNING );

	if ( startonceIds.contains( this->AppId ) and ( is_running ))
	{
		Comment( ERROR, "This Ocean process should only start once" );
		return false;
	}
	return true;
}

void Appstate_class::Announce( )
{
	Info( "announcing application ", Name );
	SetRunning( );
	sds_master->UpdateFlag = true;
}

void Appstate_class::SaveState			( )
{
	for( uint n = appId_range.min; n < appId_range.max; n++ )
	{
		backup_state[n] = Get( this->sds, n ) ;
	}
}
void Appstate_class::RestoreState		( )
{
	for( uint n = appId_range.min; n < appId_range.max; n++ )
	{
		Set( this->sds, n,  backup_state[n] ) ;
	}
}


