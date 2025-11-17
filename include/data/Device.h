/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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
