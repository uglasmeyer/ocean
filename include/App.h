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
#include <Version.h>
#include <External.h>
#include <Record.h>

extern void record_thead_fcn( Interface_class*, External_class*,  ProgressBar_class*, bool*, bool* );
extern void SynthesizerTestCases();

class Application_class : Logfacility_class
{
public:

	string 		Name 				= "";
	string 		This_Application 	= "";
	uint8_t* 	status_p 				= nullptr;
	uint 		client_id			= NOID;

	Application_class( string name, uint id, uint8_t* status );
	~Application_class();

	void Shutdown_instance( );
	void DeRegister( ifd_t* );
	void Start();

private:
	bool redirect_stderr = false;
};



#endif /* APP_H_ */
