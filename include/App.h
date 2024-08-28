/*
 * App.h
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */

#ifndef APP_H_
#define APP_H_

#include <Interface.h>
#include <Synthesizer.h>
#include <version.h>

class Application_class : Logfacility_class
{
public:

	string 	Name 				= "";
	string 	This_Application 	= "";
	uint8_t* 	status 				= nullptr;

	Application_class( string name, uint8_t* status );
	~Application_class();

	void Shutdown_instance( );
	void Start();

};



#endif /* APP_H_ */
