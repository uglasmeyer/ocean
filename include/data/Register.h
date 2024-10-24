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

class Register_class :  virtual public  Logfacility_class
{
	string 			className 	= "";
	interface_t*	sds 		= nullptr;
public:

	void	Setup( interface_t* sds, const uint& tid );
	int 	GetId();
	int 	GetStartId( uint id );
	bool 	Is_dataprocess();
	void 	Clear_procregister();
	void	Reset( uint sid, uint tid );
	void 	Show_proc_register( uint idx );
	void 	Test_Register();
	void 	Proc_deRegister();

	Register_class( ) ;
	virtual ~Register_class();


private:
	std::set<int> 	dataProc	{ AUDIOID, SYNTHID };
	uint 			Type_Id 	= NOID;
	uint 			Sds_Id		= -1;

	void 	proc_Register();
	int 	scan_proc_register(); // returns SDS_id
	void 	show_proc_register();

};



#endif /* DATA_REGISTER_H_ */
