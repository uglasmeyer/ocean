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
#include <Keys.h>
#include <Exit.h>

Exit_class				Exit{};
string					Module			= "comstack";
Logfacility_class 		Log( Module );
Keyboard_class			Keyboard{ };
Dataworld_class 		DaTA( COMSTACKID );
Application_class		App( &DaTA );
interface_t* 			sds 			= nullptr;

string 					waveform_string = "0 ... 10";
int 					update_counter 	= 1;
int 					sdsid			= 0;
Interface_class*		Sds				= DaTA.GetSds( sdsid );





#endif /* COMSTACK_H_ */
