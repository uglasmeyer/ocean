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
#include <Ocean.h>
#include <Version.h>
#include <External.h>
#include <Progressbar.h>
#include <Time.h>


extern void thread_fnc( const string& a );


// https://en.cppreference.com/w/cpp/language/parameter_pack




class Application_class :
	virtual Logfacility_class,
	state_struct
{
	string 				className 			= "";
	char 				AppId				= NOID;
public:

	Statistic_class 	Statistic			;
	Dataworld_class*	DaTA				= nullptr;
	Appstate_class*		Appstate			= nullptr;
	interface_t* 		sds					= nullptr;
	interface_t*		sds_master			= nullptr;
	Config_class*		Cfg 				= nullptr;
	Interface_class*	Sds					= nullptr;


	string 				ProgramName 		= "";
	string 				This_Application 	= "";
	uint 				client_id			= NOID;
	bool				Server_init			= true;
    std::set<int> 		logowner 			=  { GUI_ID, COMPID, RTSPID };

	Application_class( Dataworld_class* );
	virtual ~Application_class();

	void Shutdown_instance( );
	void Start( int, char* [] );
	void Init_Sds( );
	void Ready();


private:
	bool 				redirect_stderr 	= false;
	void 				app_properties		();
	void 				deRegister			();
	void 				versionTxt			();

};



#endif /* APP_H_ */
