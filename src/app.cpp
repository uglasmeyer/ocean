/*
 * app.cpp
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */

#include <App.h>

Application_class::Application_class( string name, char* status ) :
Logfacility_class("App")
{
	this->status 	= 	status;
	this->Name 		= 	name;
	this->This_Application = Application + Name + " " + Version_str;
}
Application_class::~Application_class(){}

void Application_class::Shutdown_instance( )
{
	if ( *status == RUNNING )
	{
		Comment( INFO, "Shutdown running instances of " + Name );
		*status	= EXITSERVER;
		long int 	max_wait 	= 2 * SECOND;
		long int 	wait 		= 100 * MILLISECOND;
		int 		wait_counter= 0;
		while (( *status != OFFLINE ) and ( wait*wait_counter < max_wait ))
		{
			Wait( wait );
			wait_counter++;
		}
		if ( wait * wait_counter >= max_wait )
			Comment( ERROR, "Giving up" );
	}
	else
	{
		Comment( INFO, "No other " + Name + " is running"	);
	}
}

void Application_class::Start()
{
	cout << Line << endl;
	Comment( INFO, "Entering application init for " + This_Application );
	cout << Line << endl;

}


