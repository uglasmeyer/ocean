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
#include <Record.h>
#include <Time.h>


extern void SynthesizerTestCases();

class Application_class : virtual public Logfacility_class
{
	string className = "Application_class";
public:
	Dataworld_class*	DaTA		= nullptr;
	Interface_class* 	SDS			= nullptr;
	interface_t* 		sds			= nullptr;
	Config_class*		Cfg 		= nullptr;

	string 				Name 				= "";
	string 				This_Application 	= "";
	uint 				client_id			= NOID;
	bool				Server_init			= true;

	Application_class( Dataworld_class* );
	virtual ~Application_class();

	void Shutdown_instance( );
	void Start( int, char* [] );

private:
	bool 			redirect_stderr = false;
	array<uint8_t*, APP_SIZE >
						state_arr 		{};
	uint8_t*		state_p			= nullptr;

	void deRegister( );

};



#endif /* APP_H_ */
