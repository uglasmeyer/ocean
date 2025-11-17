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



