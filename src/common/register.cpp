/*
 * register.cpp
 *
 *  Created on: Oct 14, 2024
 *      Author: sirius
 */

#include  <data/Register.h>
#include <data/SharedDataSegment.h>

Register_class::Register_class( ) :
	Logfacility_class( "Process Reg")
{
	className 	= Logfacility_class::module;
};

Register_class::~Register_class()
{
};

void Register_class::Setup( interface_t* sds, const uint& tid  )
{
	this->Type_Id 	= tid;
	this->sds		= sds;
	this->Sds_Id 	= 0;

	switch( tid )
	{
		case AUDIOID :
		{
			if ( Is_running_process((int)this->sds->process_arr.at( AUDIOID ).pid ) )
			{
				Info( "Running Audioserver " + to_string( this->sds->process_arr.at( AUDIOID ).pid ) + "detected");
				if ( not Log[TEST] )
					EXCEPTION( "Cannot start second Audioserver" );
			}
			else
			{
				proc_Register();
			}
			break;
		}
		case SYNTHID :
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
			cout << "Type_Id: " << Type_Id << endl;
			break;
		}
	}
}

bool Register_class::Is_dataprocess()
{
	return 	dataProc.contains( this->Type_Id );
}

void Register_class::Reset( uint idx )
{
	if ( not Is_running_process( (int)sds->process_arr.at( idx ).pid ) )
		sds->process_arr.at( idx ) = noprocess;
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
	uint idx = Type_Id + Sds_Id;
	regComment( this, "", Type_map( Type_Id ), Sds_Id, idx );
	if( idx > REGISTER_SIZE )
	{
		Comment( ERROR, "register out of range ");
		return;
	}

	sds->process_arr.at(idx).idx 	= idx;
	sds->process_arr.at(idx).sdsId 	= Sds_Id;
	sds->process_arr.at(idx).type	= Type_Id;
	sds->process_arr.at(idx).pid	= getpid();

	show_proc_register( );

}
void Register_class::Proc_deRegister(  )
{
	if( not Is_dataprocess() )
		return;
	uint idx = Type_Id + Sds_Id ;
	regComment( this, "De-", Type_map( Type_Id ), Sds_Id, idx );
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
		strs << Type_map( proc.type ) << endl;
		strs << SETW << "Index   "	<< idx << endl;
		strs << SETW << "Sds  Id "	<< (int)proc.sdsId << endl;
		strs << SETW << "Type Id " 	<< Type_map(proc.type) << endl;
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

void Register_class::update_register()
{
	for( uint idx = 0; idx < REGISTER_SIZE; idx++ )
	{
		if ( not Is_running_process( (int) sds->process_arr.at(idx).pid)  )
			Reset( idx );
	}
}
int Register_class::scan_proc_register() // returns Sds_Id
{
	assert( Type_Id < NOID );
	update_register();
	for( uint idx = SYNTHID; idx < REGISTER_SIZE; idx++ )
	{
		if ( sds->process_arr.at(idx).type == NOID )
			return idx - SYNTHID;
	}
	return -1;
}


void Register_class::Test_Register()
{
	TEST_START( className );

	show_proc_register();
	Clear_procregister();
	show_proc_register();
	Setup( sds, SYNTHID );
	Setup( sds, AUDIOID );
	Clear_procregister();
	show_proc_register();

	TEST_END( className );


}


