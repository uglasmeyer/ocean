/*
 * Process.h
 *
 *  Created on: Oct 14, 2024
 *      Author: sirius
 */

#ifndef DATA_REGISTER_H_
#define DATA_REGISTER_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <Configbase.h>
#include <data/SharedDataSegment.h>

class Register_class :  virtual Logfacility_class
{
	string 			className 	= "";
	interface_t*	sds 		= nullptr;
public:

	void	Setup( interface_t* sds, const uint& tid );
	int 	GetId();
	bool 	Is_dataprocess();
	void 	Clear_procregister();
	void 	Show_proc_register( uint idx );
	void 	Test_Register();

	Register_class( type_map_t* map ) ;
	virtual ~Register_class();


private:
	std::set<int> 	dataProc	{ AUDIOID, SYNTHID, RTSPID };
	uint 			Type_Id 	= NOID;
	uint 			Sds_Id		= -1;
	type_map_t*		type_map 	= nullptr;

	void 	proc_Register();
	void 	proc_deRegister();
	int 	scan_proc_register(); // returns SDS_id
	void 	show_proc_register();

};



#endif /* DATA_REGISTER_H_ */
