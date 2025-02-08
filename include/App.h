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

extern void SynthesizerTestCases();

extern void thread_fnc( const string& a );


// https://en.cppreference.com/w/cpp/language/parameter_pack




class Application_class :
	virtual Logfacility_class
{
	string className = "";
public:

	Statistic_class 	Statistic	{ className };
	Dataworld_class*	DaTA		= nullptr;
	interface_t* 		sds			= nullptr;
	Config_class*		Cfg 		= nullptr;
	Interface_class*	Sds			= nullptr;

	string 				ProgamName 				= "";
	string 				This_Application 	= "";
	uint 				client_id			= NOID;
	bool				Server_init			= true;

	Application_class( Dataworld_class* );
	virtual ~Application_class();

	void Shutdown_instance( );
	void VersionTxt();
	void Start( int, char* [] );
	void Init_Sds( );
	uint8_t GetAppState( uint appid );
	void Ready();


private:
	bool 				redirect_stderr 	= false;
	uint8_t*			state_p				= nullptr;

	void deRegister( );

};



#endif /* APP_H_ */
