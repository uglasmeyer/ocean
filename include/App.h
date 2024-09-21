/*
 * App.h
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */

#ifndef APP_H_
#define APP_H_

#include <Interface.h>
#include <Ocean.h>
#include <Version.h>
#include <External.h>
#include <Record.h>
#include <Time.h>


/*
extern void record_thead_fcn( 	Interface_class*,
								External_class*,
								ProgressBar_class*,
								std::binary_semaphore*,
								std::binary_semaphore*,
								bool*,
								bool* );
*/
extern void SynthesizerTestCases();

class Application_class : Logfacility_class
{
public:

	string 				Name 				= "";
	string 				This_Application 	= "";
	uint 				client_id			= NOID;
	Config_class		Cfg 				{ "App" };

	Application_class( string name, uint , Interface_class*);
	~Application_class();

	void Shutdown_instance( );
	void Start( int, char* [] );

private:
	bool 			redirect_stderr = false;
	Interface_class* 	SDS			= nullptr;
	ifd_t* 				sds			= nullptr;
	vector<uint8_t*>state_vec 		{};
	uint8_t*		state_p			= nullptr;

	void deRegister( );

};



#endif /* APP_H_ */
