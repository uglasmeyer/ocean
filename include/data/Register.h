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

class Register_class :
		virtual public  Logfacility_class
{
	string 			className 			= "";
	interface_t*	sds 				= nullptr;
	const set<int> 	dataProc			{ APPID::AUDIOID, APPID::SYNTHID, APPID::KBDID };

public:
	bool			is_dataproc			= false;
	uint 			Sds_Id				= -1;

	void			Setup				( interface_t* sds );
	int 			GetId				();
	void 			Clear_procregister	();
	void			Reset				( uint idx );
	void 			Show_proc_register	( uint idx );
	void 			Test_Register		();
	void 			Proc_deRegister		();
	int 			GetStartId			( );
	void			Update_register		();
	int 			Get_regpid			( uint8_t sdsid, char appid );

					Register_class		( char appid, interface_t* _sds ) ;
	virtual 		~Register_class();


private:
	uint 			AppId 				= NOID;
	string			AppName				= "";
	register_process_t
					noprocess 			{};

	void 			proc_Register		();
	int 			scan_proc_register	(); // returns SDS_id
	void 			show_proc_register	();
	int 			regpid				( uint8_t sdsid, char appid );
	bool 			Is_dataproc			( char appid );

};



#endif /* DATA_REGISTER_H_ */
