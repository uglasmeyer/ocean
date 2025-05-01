/*
 * Comstack.h
 *
 *  Created on: Sep 29, 2024
 *      Author: sirius
 */

#ifndef COMSTACK_H_
#define COMSTACK_H_

#include <Ocean.h>
#include <Keyboard.h>
#include <Logfacility.h>
#include <App.h>
#include <data/Interface.h>
#include <EventKeys.h>
#include <Exit.h>


/*
Exit_class				Exit{};
Logfacility_class 		Log( "Comstack" );
const string			Module			= Log.className;
Frequency_class 		Frequency 		{};

Keyboard_class			Keyboard		{ };
Config_class			Cfg				{ "Config" };
Dataworld_class 		DaTA			( COMSTACKID, &Cfg. & );
Application_class		App				( &DaTA );
//ViewInterface_class		ViewSds			{ DaTA.Cfg_p, DaTA.Sem_p };
interface_t*			sds_master		= DaTA.sds_master;

string 					waveform_string = "0 ... 10";
int 					update_counter 	= 1;
int 					sdsid			= sds_master->config;
Interface_class*		Sds				= DaTA.GetSds( sdsid );
interface_t* 			sds 			= Sds->addr;
*/



#endif /* COMSTACK_H_ */
