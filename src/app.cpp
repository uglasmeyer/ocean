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
#include <Exit.h>
#include <Oscgroup.h>




void SynthesizerTestCases()
{
	Shm_base Shm_test{0};
	Shm_test.Test_Memory();

	Logfacility_class		Log( "Synthesizer test" );
	DirStructure_class		Dir;

	Dataworld_class			DaTA( SYNTHID );
	DaTA.Sds_p->Reset_ifd();

	Wavedisplay_class		Wavedisplay{ DaTA.Sds_p};
	Wavedisplay_class*		wd_p = &Wavedisplay;
	Application_class		App( &DaTA );
	interface_t*			sds = DaTA.GetSdsAddr();
	Mixer_class				Mixer{&DaTA, wd_p };
	Mixer.Set_Loglevel( TEST, true );
	Mixer.Test();

	Instrument_class 		Instrument( DaTA.sds_master, wd_p );
	Note_class 				Notes{ wd_p };
	Keyboard_class			Keyboard( 	&Instrument );
	External_class 			External( 	&Mixer.StA[ MbIdExternal],
										DaTA.Cfg_p);
	ProgressBar_class		ProgressBar( &sds->RecCounter );
	Time_class				Timer( &DaTA.sds_master->time_elapsed );
	Statistic_class 		Statistic{ Log.className };

	Semaphore_class*		Sem	= DaTA.Sem_p;
//	uint8_t ch;
//	Loop_class 				Loop{ &ch };
	String 					TestStr{""};
	Oscillator 				TestOsc{  };

	Log.TEST_START( "Application " );

	App.Init_Sds( );


	App.Sds->Announce();
    std::set<string> abc{"a","b","c"};
    assert( abc.contains("b"));
	TestStr.TestString();

	Spectrum_class Spectrum {};
	Spectrum.Test_Spectrum();

	Log.Set_Loglevel( TEST, true);
	Log.Comment(TEST, "entering test classes ");

	Dir.Test();

	Log.Init_log_file();
	Log.Test_Logging();

//	Loop.Test();


	Notes.Test();

	TestOsc.Test_wf();

	Timer.Start();

	Timer.Stop();
	cout << "Run osc group in " << Timer.Time_elapsed() << " milli seconds" <<  endl;

	Mixer.Test_Logging();

	Keyboard.Test();

	External.Test_External();


	set<string> Ss { "ab", "cd", "ef", "gh" };
	assert( Ss.contains("cd"));
	assert( not Ss.contains("de"));

	Timer.Test();

	Sem->Test();

	Instrument.Test_Instrument();
	Instrument.Oscgroup.Run_Oscgroup( 0 );

	Log.Test_Logging();
	Frequency_class Frq {};
	Frq.TestFrequency();

	DaTA.Reg.Test_Register();
	DaTA.Test_Dataworld();
	System_Test();
	DaTA.Sds_p->Test_interface();


	Log.TEST_END( "Application " );
}

Application_class::Application_class( 	Dataworld_class* _DaTA ) :
Logfacility_class( "Application" )
{
	this->ProgamName			= _DaTA->Cfg.prgname;
	this->className				= Logfacility_class::className ;
	this->DaTA					= _DaTA;
	this->Statistic.module		= ProgamName;
	this->This_Application 		= Application + DaTA->Cfg.prgname + " " + Version_str;
	Comment( INFO, This_Application + " initialized ");

}

uint8_t Application_class::GetAppState( uint appid )
{
	uint8_t state;
	if ( appid == DaTA->TypeId )
		state = *Sds->Getstate_ptr( appid );
	else
		state = *DaTA->SDS_vec[0].Getstate_ptr( appid );
	return state;
}

void Application_class::Init_Sds( )
{
	this->Sds		= DaTA->GetSds( );
	assert( this->Sds != nullptr );
	this->state_p	= this->Sds->Getstate_ptr( DaTA->TypeId );

	this->sds		= Sds->addr;
}

void Application_class::VersionTxt()
{
	fstream File;
	File.open( file_structure().version_txt, fstream::out );
	File << "Version: " << Version_No << endl;
	File.close();
}

void Application_class::Start( int argc, char* argv[] )
{
    std::set<int> logowner =  { GUI_ID, COMPID, RTSPID };
	if ( logowner.contains( DaTA->TypeId ) )
	{
		Init_log_file();
	}
	Info(Line );
	Info("Entering application init for ", This_Application );
	Info(Line );

	VersionTxt();

	redirect_stderr = false;//(bool) std::freopen( errFile.data(), "w", stderr);
	if ( redirect_stderr )
	{
		Info( "Redirecting stderr");
		fprintf( stderr, "%s\n", "error file content:\n");
	}

	this->Cfg = DaTA->Cfg_p;

	Cfg->Parse_argv(argc, argv );
	Cfg->Show_Config( );
	Init_Sds( );

	if ( DaTA->Cfg.Config.clear == 'y' )
	{
		DaTA->Reg.Clear_procregister();
		EXCEPTION( "Restart processes");
	}

}

Application_class::~Application_class()
{
	deRegister();

    if (( DaTA->TypeId == SYNTHID ) and ( not LogMask[TEST] ))
	{
		Sds->Dump_ifd();
	}
}



void Application_class::deRegister( )
{
/*	auto closeStderr = [ this ]( string errFile )
	{
		Info2(1, "Closing stderr");

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
	};
*/

	auto setState = [ this ](  )
	{
		cout << endl;
		Info( "De-register " ,Type_map( DaTA->TypeId ) );
		assert ( state_p != nullptr );
		*state_p 	= OFFLINE;
		if(( sds->UserInterface != OFFLINE ) and  ( DaTA->TypeId == SYNTHID ) )
		{
			sds->UserInterface = UPDATEGUI;
		}
		sds->UpdateFlag = true;
	};


	setState( );
//	closeStderr( errFile ); TODO causes valgrind error
}

void Application_class::Ready(  )
{
	Statistic.Show_Statistic( );

	Comment(INFO, DaTA->Cfg.prgname + " is ready");
	Info( "SDS ID: ", (int) DaTA->SDS_Id );
	cout << Line << endl;
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
		Info( "No other " , ProgamName , " is running"	);
	}
	Server_init = false;
}

