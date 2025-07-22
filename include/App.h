/*
 * App.h
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */

#ifndef APP_H_
#define APP_H_

#include <data/DataWorld.h>
#include <data/Semaphore.h>
#include <data/Statistic.h>
#include <Version.h>



// https://en.cppreference.com/w/cpp/language/parameter_pack

class Application_class :
	virtual Logfacility_class,
	sdsstate_struct,
	Statistic_class
{
	string 				className 			= "";
public:

	char 				AppId				= NOID;
	process_t 			properties 			{};
	Time_class			Timer				{};
	Dataworld_class*	DaTA				= nullptr;
	Appstate_class*		Appstate			= nullptr;
	interface_t* 		sds					= nullptr;
	interface_t*		sds_master			= nullptr;
	Config_class*		Cfg 				= nullptr;
	Interface_class*	Sds					= nullptr;


	string 				This_Application 	= "";
	uint 				client_id			= NOID;
    const set<int> 		logowner 			=  { GUI_ID, COMPID, RTSPID };

    					Application_class	( Dataworld_class* );
	virtual 			~Application_class();

	void Start( int, char* [] );
	void Ready();


private:
	bool 				redirect_stderr 	= false;
	void 				app_properties		();
	void 				deRegister			();
	void 				versionTxt			();
	void 				init_Sds			();

	string 				ProgramName 		= "";
};



#endif /* APP_H_ */
