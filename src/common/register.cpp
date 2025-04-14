/*
 * register.cpp
 *
 *  Created on: Oct 14, 2024
 *      Author: sirius
 */

#include <data/Appstate.h>

void Appstate_class::Setup( interface_t* _sds )
{
	sds = _sds;
	ptr = State_pMap( _sds );
}

uint8_t* Appstate_class::State_pMap( interface_t* sds )
{
	switch ( AppId )
	{
		case APPID::AUDIOID		: return &sds->AudioServer;
		case APPID::SYNTHID		: return &sds->Synthesizer;
		case APPID::COMPID		: return &sds->Composer;
		case APPID::GUI_ID		: return &sds->UserInterface;
		case APPID::COMSTACKID	: return &sds->Comstack;
		case APPID::RTSPID		: return &sds->Rtsp;
		case APPID::TESTID		: return &sds->Rtsp;
		case APPID::NOID		: return nullptr;
		default 		: 	{
							cout << "WARN: unknown application id: " << AppId << endl;
							return nullptr;
							};
	}
}

void Appstate_class::Announce( )
{
	Comment(INFO, "announcing application " + Name );
	*ptr = RUNNING;
	sds->UpdateFlag = true;
}



#include  <data/Register.h>

Register_class::Register_class( uint id, string name ) :
	Logfacility_class( "Process Reg")
{
	className 	= Logfacility_class::className;
	AppName 	= name;
	AppId 		= id;
};

Register_class::~Register_class()
{
};

void Register_class::Setup( interface_t* sds )
{
	this->sds		= sds;
	this->Sds_Id 	= 0;

	switch( AppId )
	{
		case APPID::AUDIOID :
		{
			if ( Is_running_process((int)this->sds->process_arr.at( APPID::AUDIOID ).pid ) )
			{
				Info( 	"Running Audioserver " +
						to_string( this->sds->process_arr.at( APPID::AUDIOID ).pid ) + "detected");
				if ( not LogMask[TEST] )
					EXCEPTION( "Cannot start second Audioserver" );
			}
			else
			{
				proc_Register();
			}
			break;
		}
		case APPID::SYNTHID :
		{
			int id = scan_proc_register();
			if ( id < 0 )
			{
				Comment( ERROR, "Nr of started synthesizer processes exeeds limitation" );
				exit(1);
			}
			else
			{
				this->Sds_Id = id;
				this->sds->config = this->Sds_Id;
				proc_Register();
			}
			break;
		}
		default :
		{
			cout << "App Id:  " << AppId << endl;
			break;
		}
	}
}

bool Register_class::Is_dataprocess()
{
	return 	dataProc.contains( this->AppId );
}

void Register_class::Reset( uint idx )
{
	if ( (idx<0) or (idx>REGISTER_SIZE-1)) return;
	int pid = (int)sds->process_arr.at( idx ).pid ;
	sds->process_arr.at( idx ) = noprocess;
	if ( LogMask[DEBUG] )
		Info( "Process idx: ", idx , "reset process ", dec, pid );
}

void Register_class::Clear_procregister()
{
	Comment( WARN, "apply maintenence option -X" );
	Comment(WARN, "Clearing process register" );

	for( uint idx = 0; idx < REGISTER_SIZE; idx++ )
		Reset( idx );

	show_proc_register();
}

auto regComment = []( Register_class* C, string pref, string tstr, uint s, uint idx )
{
	if ( not C ) return;
	stringstream strs {""};
	strs << pref << "Register " << tstr<< s<< " idx "<< idx ;
	C->Comment( INFO, strs.str() );
;
};
void Register_class::proc_Register()
{
	if( not Is_dataprocess() )
		return;
	uint idx = AppId + Sds_Id;
	regComment( this, "", AppName, Sds_Id, idx );
	if( idx > REGISTER_SIZE )
	{
		Comment( ERROR, "register out of range ");
		return;
	}

	sds->process_arr.at(idx).idx 	= idx;
	sds->process_arr.at(idx).sdsId 	= Sds_Id;
	sds->process_arr.at(idx).type	= AppId;
	sds->process_arr.at(idx).pid	= getpid();

	if( LogMask[DEBUG] )
	{
		Info( "Register process pid ",dec, (int)sds->process_arr.at(idx).pid );
		show_proc_register( );
	}

}
void Register_class::Proc_deRegister(  )
{
	uint idx = AppId + Sds_Id ;
	regComment( this, "De-",AppName, Sds_Id, idx );
	if( idx > REGISTER_SIZE )
	{
		Comment( ERROR, "de-register out of range ");
		return;
	}
	Reset( idx );
}

void Register_class::show_proc_register()
{
	for( uint idx = 0; idx < REGISTER_SIZE - 1 ; idx++)
	{
		Show_proc_register(idx);
	}
}

void Register_class::Show_proc_register( uint idx )
{
	process_t proc { sds->process_arr.at(idx) };
	stringstream strs;

	if ( Is_running_process( (int) proc.pid ))
	{
		strs << AppIdName( proc.type ) << endl;
		strs << SETW << "Index   "	<< idx << endl;
		strs << SETW << "Sds  Id "	<< (int)proc.sdsId << endl;
		strs << SETW << "App  Id " 	<< AppIdName(proc.type) << endl;
		strs << SETW << "Pid     " 	<< proc.pid << endl;
		Info( strs.str() );
	}
}

int Register_class::GetStartId(  ) // external use by GUI
{

	int ID = scan_proc_register();
	if ( ID < 0 )
		return ID;
	else
	{
		sds->config = ID;  	// master_sds refers to Synthesizer ID
		return ID;			// ID is NOID
	}
}

int Register_class::GetId()
{
	return Sds_Id;
}

void Register_class::Update_register()
{
	for( uint idx = 0; idx < REGISTER_SIZE; idx++ )
	{
		if ( not Is_running_process( (int) sds->process_arr.at(idx).pid)  )
			Reset( idx );
	}
}
int Register_class::scan_proc_register() // returns Sds_Id
{
	assert( AppId < NOID );
	Update_register();
	for( uint idx = APPID::SYNTHID; idx < REGISTER_SIZE; idx++ )
	{
		if ( sds->process_arr.at(idx).type == APPID::NOID )
			return idx - APPID::SYNTHID;
	}
	return -1;
}


void Register_class::Test_Register()
{
	TEST_START( className );

	show_proc_register();
	Clear_procregister();
	show_proc_register();
	Setup( sds );
	Setup( sds );
	Clear_procregister();
	show_proc_register();

	TEST_END( className );


}


