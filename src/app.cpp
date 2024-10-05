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
#include <Config.h>
#include <Mixer.h>
#include <Keyboard.h>

void SynthesizerTestCases()
{
	Logfacility_class 		Log("Synthesizer test");
	Dataworld_class			DaTA( SYNTHID  );
	Loop_class 				Loop;
	Config_class 			Cfg( "TestCfg");
	String 					TestStr{""};
	Note_class 				Notes;
	Oscillator 				TestOsc{ TESTID };
	Instrument_class 		Instrument(DaTA.Sds.addr );
	Mixer_class 			Mixer( DaTA.Sds.addr );
	Keyboard_class			Keyboard( &Instrument );
	External_class 			External( 	&Mixer.StA[ MbIdExternal],
										&DaTA.Sds.addr->RecCounter,
										&DaTA.Cfg );
	Time_class				Timer( &DaTA.Sds.addr->time_elapsed );
	DirStructure_class		Dir;
	Semaphore_class			Sem( DaTA.Cfg_p );

	Log.TEST_START( "Application " );

	Log.Set_Loglevel( TEST, true);
	Log.Comment(TEST, "entering test classes ");

	Dir.Test();

	Log.Init_log_file();
	Log.Test_Logging();

	Loop.Test();


	Notes.Test();

	TestOsc.Test();


	Instrument.Test_Instrument();
	Timer.Start();
	Instrument.Run_osc_group();
	Timer.Stop();
	cout << "Run osc group in " << Timer.Time_elapsed() << " milli seconds" <<  endl;


	Mixer.Test_Logging();

	Keyboard.Test();

	External.Test_External();

	TestStr.TestString();
	Timer.Test();

	Sem.Test();


	Log.TEST_END( "Application " );
}


/*
 *
 */



Application_class::Application_class( 	Dataworld_class* DaTA ) :
Logfacility_class( "App" )
{
	this->Name 					= Logfacility_class::module ;
	this->SDS					= &DaTA->Sds;
	this->SDS->addr				= (interface_t*) DaTA->Sds_arr[0].addr;
	this->sds 					= SDS->addr;
	this->Cfg					= DaTA->Cfg_p;
	this->client_id 			= DaTA->Id;
	this->DaTA					= DaTA;


	this->state_arr[NOID]		= nullptr;
	this->state_arr[SYNTHID]	= &sds->Synthesizer,
	this->state_arr[COMPID]		= &sds->Composer;
	this->state_arr[GUI_ID]		= &sds->UserInterface;
	this->state_arr[COMSTACKID]	= &sds->Comstack;
	this->state_arr[AUDIOID]	= &sds->AudioServer;
	this->state_arr[RTSPID]		= &sds->Rtsp;

	this->This_Application 		= Application + Name + " " + Version_str;

	this->state_p				= state_arr[ client_id ];
	Comment( INFO, This_Application + " initialized ");

}

Application_class::~Application_class()
{
	deRegister();
	if ( client_id == SYNTHID )
	{
		SDS->Dump_ifd();
	}
}


void Application_class::deRegister( )
{
	cout << endl;
	Comment( INFO, "De-register " + Name );
	*state_p 	= OFFLINE;
	if(( sds->UserInterface != OFFLINE ) and  ( client_id == SYNTHID ) )
	{
		sds->UserInterface = UPDATEGUI;
	}
	sds->UpdateFlag = true;

	Comment( INFO, "Closing stderr");

	if ( redirect_stderr )
		std::fclose(stderr);

	std::ifstream cFile( errFile );
    string out = "";
    do
    {
    	cout.flush() << out << endl;

    } while( getline ( cFile, out ));
    cout << endl;
    cFile.close( );

}

void Application_class::Shutdown_instance( )
{

	if ( *state_p == RUNNING )
	{
		*state_p	= EXITSERVER;
	}
	else
	{
		Comment( INFO, "No other " + Name + " is running"	);
	}
	Server_init = false;
}

void Application_class::Start( int argc, char* argv[] )
{
	if (( client_id == GUI_ID ) or
		( client_id == COMPID ) or
		( client_id == RTSPID ))
	{
		Init_log_file();
	}
	Comment( INFO, Line );
	Comment( INFO, "Entering application init for ");
	Comment( INFO, This_Application );
	Comment( INFO, Line );

	redirect_stderr = (bool) std::freopen( errFile.data(), "w", stderr);
	if ( redirect_stderr )
	{
		Comment( INFO, "Redirecting stderr");
		fprintf( stderr, "%s\n", "error file content:\n");
	}

	Cfg->Parse_argv(argc, argv);
	Cfg->Show_prgarg_struct( );
	DaTA->Sds.addr = DaTA->SetSds( DaTA->Cfg.Config.SDS_id );

}
