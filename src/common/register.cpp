/*
 * register.cpp
 *
 *  Created on: Oct 14, 2024
 *      Author: sirius
 */

#include  <data/Register.h>
#include <data/SharedDataSegment.h>

Register_class::Register_class() :
	Logfacility_class( "Process Reg")
{
	className = Logfacility_class::module;
};

Register_class::~Register_class()
{
	proc_deRegister( );
};

void Register_class::Setup( process_arr_t* addr, const type_map_t& type_map, const uint& tid  )
{
	this->Type_Id 	= tid;
	process_arr		= addr;

	this->Sds_Id	= scan_proc_register();
	if ( this->Sds_Id  < 0 )
	{
		Comment( ERROR, "Nr of started synthesizer processes exeeds limitation" );
		exit( 1 );
	}
	this->type_map = type_map;
	proc_Register();
}

bool Register_class::Is_dataprocess()
{
	return 	dataProc.contains( this->Type_Id );
}

void Register_class::Clear_procregister()
{
	Comment( WARN, "apply maintenence option -X" );
	Comment(WARN, "Clearing process register" );

	for( uint n = 0; n < MAXINDEX; n++ )
	{
		process_arr->at(n) = process_struct();
	}
}


void Register_class::proc_Register()
{
	if( not Is_dataprocess() )
		return;
	uint idx = Type_Id + Sds_Id;
	Comment(INFO, type_map[ Type_Id ] + " " + to_string(Sds_Id) +
				" idx " + to_string( idx ));

	process_arr->at(idx).Id 	= Sds_Id;
	process_arr->at(idx).type	= Type_Id;

	show_proc_register( );

}
void Register_class::proc_deRegister(  )
{
	if( not Is_dataprocess() )
		return;
	uint idx = Type_Id + Sds_Id ;
	if( idx > MAXINDEX )
	{
		Comment( ERROR, "de-register out of range ");
		return;
	}
	Comment( INFO, "de-register process " + type_map[ Type_Id ] + " " + to_string( Sds_Id ) );
	process_arr->at(idx) = process_struct();
}

void Register_class::Show_proc_register()
{
	for( uint idx = 0; idx < MAXINDEX ; idx++)
	{
		Show_proc_register(idx);
	}
}

void Register_class::Show_proc_register( uint idx )
{
	process_t proc { process_arr->at(idx) };
	stringstream strs;
	strs << type_map[ proc.type ] << endl;
	strs << SETW << "Index   "	<< to_string(idx) << endl;
	strs << SETW << "Sds  Id "	<< proc.Id << endl;
	strs << SETW << "Type Id " 	<< proc.type << endl;
	Comment( INFO, strs.str() );
}

void Register_class::show_proc_register(  )
{
	uint idx = Type_Id + Sds_Id;
	Show_proc_register( idx );
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
		for( uint idx = 1; idx < MAXINDEX; idx++ )
		{
			if ( process_arr->at(idx).type == NOID )
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
	Clear_procregister();
	Sds_Id = 0;
	proc_Register();
	proc_Register();
	proc_Register();
	Show_proc_register();
	Clear_procregister();

	TEST_END( className );


}


