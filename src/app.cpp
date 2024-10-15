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
	Shm_base Shm_test{0};
	Shm_test.Test_Memory();

	Logfacility_class		Log( "Synthesizer test" );
	DirStructure_class		Dir;
	Dataworld_class			DaTA( SYNTHID );

	Application_class		App( &DaTA );
	Mixer_class				Mixer;
	Mixer.Set_Loglevel( TEST, true );
	Instrument_class 		Instrument( DaTA.Sds_master );
	Note_class 				Notes;
	Keyboard_class			Keyboard( 	&Instrument );
	External_class 			External( 	&Mixer.StA[ MbIdExternal],
										DaTA.Cfg_p);
	ProgressBar_class		ProgressBar( &DaTA.Sds_master->RecCounter );
	Time_class				Timer( &DaTA.Sds_master->time_elapsed );
	Statistic_class 		Statistic{ Log.module };

	Semaphore_class*		Sem	= DaTA.Sem_p;
	Loop_class 				Loop;
	String 					TestStr{""};
	Oscillator 				TestOsc{ TESTID };
	TestOsc.Set_Loglevel( TEST, true );

	Log.TEST_START( "Application " );

	App.Init_Sds( 0 );


	App.Sds->Announce();

    std::set<string> abc{"a","b","c"};
    assert( abc.contains("b"));

	Log.Set_Loglevel( TEST, true);
	Log.Comment(TEST, "entering test classes ");

	Dir.Test();

	Log.Init_log_file();
	Log.Test_Logging();

	Loop.Test();


	Notes.Test();

	TestOsc.Test();


	Timer.Start();
	Instrument.Run_osc_group();
	Timer.Stop();
	cout << "Run osc group in " << Timer.Time_elapsed() << " milli seconds" <<  endl;

	Mixer.Setup( DaTA.Sds_master, DaTA.SDS_Id);
	Mixer.Test_Logging();

	Keyboard.Test();

	External.Test_External();

	TestStr.TestString();
	Timer.Test();

	Sem->Test();
	Instrument.Test_Instrument();

	Log.Info("5, variatic, argument, 4, 5 ");
	Log.TEST_END( "Application " );

	Register_class	Reg{};
	Reg.Test_Register();
}




Application_class::Application_class( 	Dataworld_class* DaTA ) :
Logfacility_class( "App" )
{
	this->Name 					= Logfacility_class::module ;
	this->DaTA					= DaTA;


	this->This_Application 		= Application + Name + " " + Version_str;
	Info( This_Application + " initialized ");

}

void Application_class::Init_Sds( uint sds_id )
{
	this->Sds		= DaTA->GetSds( );
	assert( this->Sds != nullptr );
	this->sds		= Sds->addr;

	this->state_p	= Sds->state_p_map[ DaTA->TypeId ];
	assert( state_p != nullptr );

}
void Application_class::Start( int argc, char* argv[] )
{
    std::set<int> logowner{ GUI_ID, COMPID, RTSPID };
	if ( logowner.contains( DaTA->TypeId ) )
	{
		Init_log_file();
	}
	Info( Line );
	Info( "Entering application init for ");
	Info( This_Application );
	Info( Line );

	redirect_stderr = (bool) std::freopen( errFile.data(), "w", stderr);
	if ( redirect_stderr )
	{
		Info( "Redirecting stderr");
		fprintf( stderr, "%s\n", "error file content:\n");
	}
	this->Cfg					= DaTA->Cfg_p;

	Cfg->Parse_argv(argc, argv );
	Cfg->Show_Config( );
	Init_Sds( DaTA->SDS_Id );

	if ( DaTA->Cfg.Config.clear == 'y' )
	{
		DaTA->Reg.Clear_procregister();
		Exception( "Restart processes");
	}
	DaTA->Reg.Show_proc_register();

	if( DaTA->TypeId == SYNTHID )
	{
		if ( DaTA->Sds_master->process_arr[1].type == NOID )
		{
			for( uint n = 0; n<MAXCONFIG +1; n++)
				DaTA->Reg.Show_proc_register(n);
			Comment( ERROR, "Start master synthesizer process first");
			Comment( ERROR, "before secondary can run" );
			Exception("Cannot continue" );
		}
	}
	else
		cout << "not a data process"<< endl;
}

Application_class::~Application_class()
{
	deRegister();
	if ( DaTA->TypeId == SYNTHID )
	{
		Sds->Dump_ifd();
	}
}

void Application_class::deRegister( )
{
	cout << endl;
	Info( "De-register " + Name );
	assert ( state_p != nullptr );
	*state_p 	= OFFLINE;
	if(( sds->UserInterface != OFFLINE ) and  ( DaTA->TypeId == SYNTHID ) )
	{
		sds->UserInterface = UPDATEGUI;
	}
	sds->UpdateFlag = true;

	Info( "Closing stderr");

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
		DaTA->Sem.Lock(SEMAPHORE_EXIT, 1);
	}
	else
	{
		Info( "No other " + Name + " is running"	);
	}
	Server_init = false;
}

