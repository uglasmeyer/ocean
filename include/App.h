/*
 * App.h
 *
 *  Created on: Aug 27, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef APP_H_
#define APP_H_

#include <Ocean.h>
#include <data/Semaphore.h>
#include <data/Statistic.h>
#include <data/DataWorld.h>
#include <Kbd.h>



// https://en.cppreference.com/w/cpp/language/parameter_pack

class Application_class :
	virtual Logfacility_class,
	sdsstate_struct,
	Statistic_class
{
	string 				className 			= "";
public:

	APPID 				AppId				= NoAPPID;
	process_t 			properties 			{};
	Time_class			Timer				{};
	Kbd_base			Kbd					{};
	Dataworld_class*	DaTA				= nullptr;
	Appstate_class*		Appstate			= nullptr;
	interface_t* 		sds					= nullptr;
	interface_t*		sds_master			= nullptr;
	Config_class*		Cfg 				= nullptr;
	Interface_class*	Sds					= nullptr;

	string 				This_Application 	= "";
    const set<APPID>	logowner 			=  { GUI_ID, COMPID, RTSPID };

    					Application_class	( Dataworld_class* );
	virtual 			~Application_class	();

	void 				Start				( int, char* [] );
	void 				Ready				();
	kbdInt_t			KeyboardKey			( bool debug);


private:
	bool 				redirect_stderr 	= false;
	void 				app_properties		();
	void 				deRegister			();
	void 				versionTxt			();

	string 				ProgramName 		= "";
};



#endif /* APP_H_ */
