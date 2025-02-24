/*
 * Comstack.h
 *
 *  Created on: Sep 29, 2024
 *      Author: sirius
 */

#ifndef COMSTACK_H_
#define COMSTACK_H_


#include <Keyboard.h>
#include <Logfacility.h>
#include <App.h>
#include <data/Interface.h>
#include <EventKeys.h>
#include <Exit.h>

Exit_class				Exit{};
string					Module			= "Comstack";
Logfacility_class 		Log( Module );
Frequency_class 		Frequency 		{};

Keyboard_class			Keyboard{ };
Dataworld_class 		DaTA( COMSTACKID );
Application_class		App( &DaTA );
interface_t*			sds_master		= DaTA.sds_master;

string 					waveform_string = "0 ... 10";
int 					update_counter 	= 1;
int 					sdsid			= sds_master->config;
Interface_class*		Sds				= DaTA.GetSds( sdsid );
interface_t* 			sds 			= Sds->addr;





#endif /* COMSTACK_H_ */
