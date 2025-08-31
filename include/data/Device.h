/*
 * Device.cpp
 *
 *  Created on: Aug 27, 2025
 *      Author: sirius
 */

#ifndef DATA_DEVICE_H_
#define DATA_DEVICE_H_


#include <data/Interface.h>

class Device_class
	: virtual public 	Logfacility_class
	, virtual			osc_struct
{
	string 				className 				= "";
	vector<connectId_t*>sds_connect_vec			{ 0,0,0 } ;

public:
	interface_t* 		sds_p;
	typedef struct connectName_struct
	{
		string frq="";
		string vol="";
	} connectName_t;

						Device_class			( interface_t* sds );
	virtual 			~Device_class			();
	connectId_t 		Get_connect_state		( char oscid );
	void 				Set_connect_state		( char oscid, connectId_t connect );
	void 				Set_sds_vec				( interface_t* sds );
	connectName_t		Get_connection_names	( char oscid );
	void 				Show_Connection_names	( char oscid );

private:
	void 				selfTest				();
};

typedef Device_class::connectName_t		connectName_t;


#endif /* DATA_DEVICE_H_ */
