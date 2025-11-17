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
 * Device.cpp
 *
 *  Created on: Aug 27, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_DEVICE_H_
#define DATA_DEVICE_H_

#include <Logfacility.h>
#include <Adsr.h>
#include <data/SharedDataSegment.h>

constexpr connectId_t default_connect( OSCID_e oscid )
{
	return { oscid, oscid };
};

class Device_class
	: virtual public 	Logfacility_class
	, virtual public	osc_struct
{
	string 				className 				= "";

	interface_t* 		sds_p;

public:
	typedef struct connectName_struct
	{
		string frq="";
		string vol="";
	} connectName_t;

						Device_class			( interface_t* sds );
	virtual 			~Device_class			();
	connectId_t 		Get_connect_state		( char oscid );
	void 				Set_sds_connect_state	( char oscid, connectId_t connect );
	void 				Set_sds_adsr			( char oscid, adsr_t adsr );

	void 				Set_sds					( interface_t* sds );
	connectName_t		Get_connection_names	( char oscid );
	void 				Show_Connection_names	( char oscid );
	void 				SelfTest				();

private:
};

typedef Device_class::connectName_t		connectName_t;


#endif /* DATA_DEVICE_H_ */
