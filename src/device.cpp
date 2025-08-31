/*
* device.cpp
*
*  Created on: Aug 28, 2025
*      Author: sirius
*/

#include <data/Device.h>

Device_class::Device_class( interface_t* sds ) :
	Logfacility_class("Device_class")
{
	className = Logfacility_class::className;
	this->sds_p = sds;
	Set_sds_vec( sds_p );
	selfTest();
};
Device_class::~Device_class()
{
	DESTRUCTOR( className );
};
void Device_class::Set_sds_vec( interface_t* sds )
{
	this->sds_p = sds;
	sds_connect_vec	=
	{
		&sds->VCO_connect,
		&sds->FMO_connect,
		&sds->OSC_connect
	};
}
void Device_class::selfTest()
{
	connectId_t Connect;
	Connect.frq = VCOID;
	Connect.vol = VCOID;
	Set_connect_state( FMOID, Connect );

	assert( sds_p->FMO_connect == Connect );
	Set_connect_state( FMOID, default_connect( FMOID ) );

}


connectId_t Device_class::Get_connect_state( char oscid )
{
	connectId_t Connect = *sds_connect_vec[oscid];
	return Connect;
}
void Device_class::Set_connect_state( char oscid, connectId_t connect )
{
	*sds_connect_vec[ oscid ] = connect;
}

connectName_t Device_class::Get_connection_names( char oscid )
{
	connectName_t 	ConnectName;
	connectId_t 	ConnectId 		= Get_connect_state( oscid );
					ConnectName.frq = oscNames[ ConnectId.frq ];
					ConnectName.vol = oscNames[ ConnectId.vol ];

	return ConnectName;

}
void Device_class::Show_Connection_names( char oscid )
{
	connectName_t 	names = Get_connection_names( oscid );
	coutf << oscNames[ oscid] << ": ";
	coutf << names.frq << " " << names.vol << endl;
}



