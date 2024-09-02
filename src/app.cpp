/*
 * app.cpp
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */

#include <App.h>

Application_class::Application_class( string name, uint id, uint8_t* status ) :
Logfacility_class("App")
{
	this->status 			= status;
	this->Name 				= name;
	this->This_Application 	= Application + Name + " " + Version_str;
	this->client_id 		= id;
	Comment( INFO, This_Application + " initialized ");
}

Application_class::~Application_class(){}

void Application_class::Decline( bool* update )
{
	Comment( INFO, "decline " + Name );
	*status = OFFLINE;
	*update = true;
}

void Application_class::Shutdown_instance( )
{
	if ( *status == RUNNING )
	{
		Comment( INFO, "Shutdown running instances of " + Name );
		*status	= EXITSERVER;
		long int 	max_wait 	= 2 * SECOND;
		long int 	amoment 	= 100 * MILLISECOND;
		int 		moments		= 0;
		while (( *status != OFFLINE ) and ( amoment*moments < max_wait ))
		{
			Wait( amoment );
			moments++;
		}
		if ( amoment * moments >= max_wait )
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
	if (( client_id == GUI_ID ) or ( client_id == COMPID ) )
	{
		init_log_file();
	}

}


