/*
 * app.cpp
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */


/*
 * Synthesizer.cpp extensions
 */



#include <App.h>



Application_class::Application_class( Dataworld_class* _DaTA ) :
		Logfacility_class( "Application_class" ),
		Statistic_class( _DaTA->Appstate.Name )
{
	this->ProgramName			= _DaTA->Appstate.Name;
	this->className				= Logfacility_class::className ;
	this->DaTA					= _DaTA;
	this->Cfg 					= DaTA->Cfg_p;
	this->sds_master			= DaTA->sds_master;
	this->AppId					= DaTA->AppId;
	this->Appstate				= &DaTA->Appstate;
	this->This_Application 		= Application + ProgramName + " " + Version_str;
	Comment( INFO, This_Application + " initialized ");
	init_Sds( );

}

void Application_class::init_Sds( )
{
	this->Sds		= DaTA->GetSds( );
	assert( this->Sds != nullptr );
	this->sds		= Sds->addr;
}

Application_class::~Application_class()
{
	cerr.flush() << "~" << className << endl;
	if ( Cfg->Config.test 	== 'y' )
	{
		system_execute( " cat /tmp/log/Synthesizer.log 1>&2 " );
	}
	deRegister();

    if (( AppId == SYNTHID ) and ( not LogMask[TEST] ))
		Sds->Dump_ifd();
}
void Application_class::versionTxt()
{
	fstream File;
	File.open( file_structure().version_txt, fstream::out );
	File << "Version: " << Version_No << endl;
	File.close();
}

void Application_class::app_properties()
{
	properties.logowner			= logowner.contains( AppId );
	properties.start_once		= Appstate->startonceIds.contains( AppId );
	properties.data_process		= DaTA->Reg.is_dataproc ;
	properties.keyboard			= ( is_atty and
									( AppId 			== SYNTHID ) and
									( Cfg->Config.test 	== 'n') );
	properties.Show();
}
void Application_class::Start( int argc, char* argv[] )
{
	Cfg->Parse_argv(argc, argv );

	app_properties();
	if( properties.keyboard )
	{
		this->sds->Keyboard = true;
		for( interface_t* sds : DaTA->SDS.vec)
		{
			if ( sds != this->sds )
			{
				if( sds->Keyboard )
				{
					if( Appstate->IsRunning( sds, SYNTHID ))
					{
						Comment( ERROR, "This Ocean process should only start once" );
					    std::this_thread::sleep_for( chrono::seconds(2) );
						exit( 1 );
					}
				}
			}
		}
	}


	if ( not Appstate->StartOnceOk( this->sds ))
		exit( 1 );

	Appstate->Announce();

	if ( logowner.contains( AppId ) )
	{
		Init_log_file();
	}
	Info(Line );
	Info("Entering application init for ", This_Application );
	Info(Line );

	versionTxt();

	Cfg->Show_Config( );

	if ( DaTA->Cfg_p->Config.clear == 'y' )
	{
		DaTA->Reg.Clear_procregister();
		EXCEPTION( "Restart processes");
	}

}


void Application_class::deRegister( )
{
	if (properties.keyboard )
		sds->Keyboard = false;
	DaTA->Appstate.SetOffline( );//this->sds, this->AppId );

	if ( this->AppId != GUI_ID )
		if( DaTA->Appstate.IsRunning( sds_master, GUI_ID)  )
		{
			DaTA->EmitEvent( APPSTATE_FLAG, ProgramName );
		}
	DaTA->Sem_p->Release( SEMAPHORE_EXIT );
}

void Application_class::Ready(  )
{
	Show_Statistic( );

	Comment(INFO, ProgramName + " is ready");
	Info( "SDS ID: ", (int) DaTA->SDS_Id );
	cout << Line << endl;
	DaTA->EmitEvent( APPSTATE_FLAG, ProgramName );

}

