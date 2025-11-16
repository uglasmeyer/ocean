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
