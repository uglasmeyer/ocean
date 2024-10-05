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


string					Module			= "comstack";
Logfacility_class 		Log( Module );
Keyboard_class			Keyboard;
Dataworld_class 		DaTA( COMSTACKID );
Application_class		App( &DaTA );
interface_t* 			sds 			= App.sds;

Keyboard_base::key_struct_t
						keys			= Keyboard_base::key_struct();
string 					waveform_string = "0 ... 10";
int 					update_counter 	= 1;




#endif /* COMSTACK_H_ */
