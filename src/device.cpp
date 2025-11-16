/*
* device.cpp
*
*  Created on: Aug 28, 2025
*      Author: Ulrich.Glasmeyer@web.de
*/

#include <data/Device.h>

Device_class::Device_class( interface_t* sds ) :
	Logfacility_class("Device_class")
{
	className = Logfacility_class::className;
	this->sds_p = sds;
	Set_sds( sds_p );
	SelfTest();
};

Device_class::~Device_class()
{
	DESTRUCTOR( className );
};
void Device_class::Set_sds( interface_t* sds )
{
	this->sds_p = sds;

}
void Device_class::SelfTest()
{
	connectId_t Connect;
	Connect.frq = VCOID;
	Connect.vol = VCOID;
	Set_sds_connect_state( FMOID, Connect );

	assert( sds_p->connect_arr[FMOID] == Connect );
	Set_sds_connect_state( FMOID, default_connect( FMOID ) );

}

connectId_t Device_class::Get_connect_state( char oscid )
{
	connectId_t Connect = sds_p->connect_arr[oscid];
	return Connect;
}
void Device_class::Set_sds_adsr( char oscid, adsr_t adsr )
{
	sds_p->adsr_arr[oscid] = adsr;
}

void Device_class::Set_sds_connect_state( char oscid, connectId_t connect )
{
	sds_p->connect_arr[ oscid ] = connect;
}

connectName_t Device_class::Get_connection_names( char oscid )
{
	connectName_t 	ConnectName;
	connectId_t 	ConnectId 		= Get_connect_state( oscid );
					ConnectName.frq = typeNames[ ConnectId.frq ];
					ConnectName.vol = typeNames[ ConnectId.vol ];

	return ConnectName;

}
void Device_class::Show_Connection_names( char oscid )
{
	connectName_t 	names = Get_connection_names( oscid );
	coutf << typeNames[ oscid] << ": ";
	coutf << names.frq << " " << names.vol << endl;
}



