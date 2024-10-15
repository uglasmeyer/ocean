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
#include <Config.h>
#include <data/SharedDataSegment.h>

class Register_class :  virtual Logfacility_class
{
	string className = "";
public:

	void	Setup( process_arr_t*, const type_map_t&, const uint& tid );
	int 	GetId();
	bool 	Is_dataprocess();
	void 	Clear_procregister();
	void 	Show_proc_register();
	void 	Show_proc_register( uint idx );
	void 	Test_Register();

	Register_class() ;
	virtual ~Register_class();


private:
	std::set<int> 	dataProc	{ AUDIOID, SYNTHID, RTSPID };
	uint 			Type_Id 	= NOID;
	const uint		MAXINDEX 	= MAXPROCARRAY;
	uint 			Sds_Id		= -1;
	process_arr_t* 	process_arr = nullptr;
	type_map_t		type_map 	{};
	void 	proc_Register();
	void 	proc_deRegister();
	void 	show_proc_register();
	int 	scan_proc_register(); // returns SDS_id
};



#endif /* DATA_REGISTER_H_ */
