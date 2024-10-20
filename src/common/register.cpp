/*
 * register.cpp
 *
 *  Created on: Oct 14, 2024
 *      Author: sirius
 */

#include  <data/Register.h>
#include <data/SharedDataSegment.h>

Register_class::Register_class( type_map_t* map ) :
	Logfacility_class( "Process Reg")
{
	className 	= Logfacility_class::module;
	this->type_map 	= map;
};

Register_class::~Register_class()
{
	proc_deRegister( );
};

void Register_class::Setup( interface_t* sds, const uint& tid  )
{
	this->Type_Id 	= tid;
	this->sds		= sds;

	int id	= GetStartId( sds->SDS_Id );//scan_proc_register();
	if ( id  < 0 )
	{
		Comment( ERROR, "Nr of started synthesizer processes exeeds limitation" );
		exit( 1 );
	}
	this->Sds_Id = id;
	proc_Register();
}

bool Register_class::Is_dataprocess()
{
	return 	dataProc.contains( this->Type_Id );
}

void Register_class::Reset( uint sid, uint tid )
{
	uint idx = sid + tid;
	sds->process_arr.at( idx ) = process_struct();
}
void Register_class::Clear_procregister()
{
	Comment( WARN, "apply maintenence option -X" );
	Comment(WARN, "Clearing process register" );

	for( uint n = 0; n < REGISTER_SIZE; n++ )
	{
		sds->process_arr.at(n) = process_struct();
	}
	show_proc_register();
}

auto regComment = []( Register_class* C, string pref, string tstr, uint s, uint idx )
{
	C->Info( pref + "Register " + tstr + " " + to_string( s ) + " idx " + to_string( idx ));
;
};
void Register_class::proc_Register()
{
	if( not Is_dataprocess() )
		return;
	uint idx = Type_Id + Sds_Id;
	regComment( this, "", type_map->at( Type_Id ), Sds_Id, idx );
	if( idx > REGISTER_SIZE )
	{
		Comment( ERROR, "register out of range ");
		return;
	}

	sds->process_arr.at(idx).idx 	= idx;
	sds->process_arr.at(idx).sdsId 	= Sds_Id;
	sds->process_arr.at(idx).type	= Type_Id;

	show_proc_register( );

}
void Register_class::proc_deRegister(  )
{
	if( not Is_dataprocess() )
		return;
	uint idx = Type_Id + Sds_Id ;
	regComment( this, "De-", type_map->at( Type_Id ), Sds_Id, idx );
	if( idx > REGISTER_SIZE )
	{
		Comment( ERROR, "de-register out of range ");
		return;
	}

	sds->process_arr.at(idx) = process_struct();

}

void Register_class::show_proc_register()
{
	for( uint idx = 0; idx < REGISTER_SIZE - 1 ; idx++)
	{
		cout << (int)sds->process_arr.at(idx).type << ", " ;
	}
	cout << (int)sds->process_arr.at(REGISTER_SIZE-1).type << endl ;
}

void Register_class::Show_proc_register( uint idx )
{
	process_t proc { sds->process_arr.at(idx) };
	stringstream strs;
	strs << type_map->at( proc.type ) << endl;
	strs << SETW << "Index   "	<< to_string(idx) << endl;
	strs << SETW << "Sds  Id "	<< proc.sdsId << endl;
	strs << SETW << "Type Id " 	<< type_map->at(proc.type) << endl;
	Comment( TEST, strs.str() );
}

int Register_class::GetStartId( uint id ) // used by Ocean
{
	int ID = id;
	if ( sds->process_arr.at( id ).type == NOID )
	{
		sds->SDS_Id = ID;
		return ID;
	}
	else
	{
		ID = scan_proc_register();
		if ( ID < 0 )
			return ID;
		else
		{
			sds->SDS_Id = ID;
			return ID;
		}
	}
}

int Register_class::GetId()
{
	return Sds_Id;
}
int Register_class::scan_proc_register() // returns Sds_Id
{
	assert( Type_Id < NOID );
	if ( Type_Id == SYNTHID )
	{
		for( uint idx = 1; idx < REGISTER_SIZE; idx++ )
		{
			if ( sds->process_arr.at(idx).type == NOID )
				return idx - Type_Id;
		}
		return -1;
	}
	else
		return 0;
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


