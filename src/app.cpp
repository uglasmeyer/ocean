/*
 * app.cpp
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */

#include <App.h>
#include <Config.h>

/*
 * Synthesizer.cpp extensions
 */




#include <Mixer.h>
#include <Keyboard.h>

void SynthesizerTestCases()
{
	Logfacility_class Log("Synthesizer test");
	Loop_class 				Loop;
	Config_class 			Cfg( "TestCfg");
	String 					TestStr{""};
	Note_class 				Notes;
	Oscillator 				TestOsc{ TESTID };
	Interface_class			SDS;
	Instrument_class 		Instrument(SDS.addr );
	Mixer_class 			Mixer( SDS.addr );
	Keyboard_class			Keyboard( &Instrument );
	External_class 			External( &Mixer.StA[ MbIdExternal], &SDS.addr->RecCounter );
	Time_class				Timer( &SDS.addr->time_elapsed );
	DirStructure_class		Dir;


	Dir.Test();

	Log.Set_Loglevel( TEST, true);
	Log.Comment(TEST, "entering test classes ");

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

	External.test();

	External.Id3tool_cmd( ); // @suppress("Invalid arguments")
	TestStr.test();
	Timer.Test();
}


/*
 *
 */



Application_class::Application_class( string name, uint id, Interface_class* SDS ) :
Logfacility_class("App")
{
	this->SDS				= SDS;
	this->sds 				= SDS->addr;
	this->Name 				= name;
	this->client_id 		= id;

	this->state_vec			= { nullptr,
								&sds->Synthesizer,
								&sds->Composer,
								&sds->UserInterface,
								&sds->Comstack,
								&sds->AudioServer
								};
	this->This_Application 	= Application + Name + " " + Version_str;

	this->state_p			= state_vec[ id ];
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
    	cout.flush() << out;

    } while( getline ( cFile, out ));
    cout << endl;
    cFile.close( );

}

void Application_class::Shutdown_instance( )
{
	if ( *state_p == RUNNING )
	{
		*state_p	= EXITSERVER;
		Comment( INFO, "Shutdown running instances of " + Name );
		long int 	max_wait 	= 2 * SECOND;
		long int 	amoment 	= 100 ;
//		long int 	amoment 	= 100 * MILLISECOND;
		int 		moments		= 0;
		while (( *state_p == EXITSERVER ) and ( amoment*moments < max_wait ))
		{
			this_thread::sleep_for(chrono::milliseconds(amoment));
//			Wait( amoment );
			Comment( WARN, "-" ) ;
			moments++;
		}

		if ( amoment * moments >= max_wait )
		{
			Comment( ERROR, "Giving up" );
			*state_p = RUNNING;
		}
	}
	else
	{
		Comment( INFO, "No other " + Name + " is running"	);
	}
}

void Application_class::Start( int argc, char* argv[] )
{
	if (( client_id == GUI_ID ) or ( client_id == COMPID ) )
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
		fprintf( stderr, "%s\n", "error file is empty");
	}

	Cfg.Parse_argv(argc, argv);
	Cfg.Show_prgarg_struct( );



}
