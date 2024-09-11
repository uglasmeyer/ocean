/*
 * app.cpp
 *
 *  Created on: Aug 27, 2024
 *      Author: sirius
 */

#include <App.h>

/*
 * Synthesizer.cpp extensions
 */

void record_thead_fcn( 	Interface_class* SDS,
						External_class* External,
						ProgressBar_class* 	Record,
						bool*			SaveRecordFlag,
						bool*			RecordThreadDone )
{
	Logfacility_class Log("RecordThread");
	Log.Comment( INFO, "record thread started ");

	Value fileno {0};
	while ( not *RecordThreadDone ) 			// run until exit_proc or empty_exit_proc is called
	{
		if ( ( *SaveRecordFlag ) )				// triggered by RECORDWAVFILEKEY
		{
			fileno = (int) SDS->addr->FileNo;
			Log.Comment( INFO, "record thread received job " + fileno.str);

			External->Save_record_data( fileno.i );

			// clean up
			*SaveRecordFlag = false;
			Record->Unset();
			SDS->Update( RECORDWAVFILEFLAG ); 	// feedback to GUI

		}
	    std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	Log.Comment( INFO, "record thread terminated ");

}



#include <Mixer.h>
#include <Keyboard.h>

void SynthesizerTestCases()
{
	Logfacility_class Log("Synthesizer test");
	Loop_class 				Loop;
	String 					TestStr{""};
	Note_class 				Notes;
	Oscillator 				TestOsc{ TESTID };
	Interface_class			SDS;
	Instrument_class 		Instrument(SDS.addr );
	Mixer_class 			Mixer( SDS.addr );
	Keyboard_class			Keyboard( &Instrument );
	External_class 			External( &Mixer.StA[ MbIdExternal] );
	Time_class				Timer( &SDS.addr->time_elapsed );
	Config_class* 			Cfg = new Config_class;

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

	Cfg->read_synthesizer_config( );
	External.Id3tool_cmd( Cfg->Get["Title"], Cfg->Get["Author"], Cfg->Get["Genre"], Cfg->Get["Album"]);
	string I = Cfg->Get["int"];
	cout << dec << atoi(I.data()) <<endl;
    Server_struct().TERM = Cfg->Get["TERM"];
    cout << Server_struct().cmd( Audio_Srv, "help") << endl;
    delete(Cfg);
	TestStr.test();
	Timer.Test();
}


/*
 *
 */

DirStructure_class	DirStructure{};


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

	std::ifstream cFile( file_structure().err_file );
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
		long int 	amoment 	= 100 * MILLISECOND;
		int 		moments		= 0;
		while (( *state_p == EXITSERVER ) and ( amoment*moments < max_wait ))
		{
			Wait( amoment );
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

void Application_class::Start()
{
	if (( client_id == GUI_ID ) or ( client_id == COMPID ) )
	{
		Init_log_file();
	}
	Comment( INFO, Line );
	Comment( INFO, "Entering application init for ");
	Comment( INFO, This_Application );
	Comment( INFO, Line );

	redirect_stderr = (bool) std::freopen( file_structure().err_file.data(), "w", stderr);
	if ( redirect_stderr )
	{
		Comment( INFO, "Redirecting stderr");
		fprintf( stderr, "%s\n", "error file is empty");
	}
	DirStructure.Create();


}

using namespace std::chrono;

Time_class::Time_class( uint8_t* t )
: Logfacility_class("Timer")
{
	this->time_elapsed = t;
	Start();
	Stop(); // duration is zero
}

Time_class::~Time_class( )
{
	*this->time_elapsed = 0;
}
long int Time_class::Time_elapsed()
{
	Stop();
	long long int start_count = duration_cast<milliseconds>(start_time.time_since_epoch()).count();
	long long int stop_count  = duration_cast<milliseconds>( stop_time.time_since_epoch()).count();
	return stop_count - start_count;
}
void Time_class::Start()
{
	start_time = steady_clock::now();
};

void Time_class::Stop()
{
	stop_time = steady_clock::now();
}

void Time_class::Block()
{
	duration = Time_elapsed();
	latency = duration*100/wait;
	if ( latency > 100 )
		Comment( WARN, "runtime latency exceeds 100% " + to_string( latency ));


	if ( wait > duration )
		ms_wait = wait - duration ;
	usleep( ms_wait * 1000 ); //milli seconds

}
uint Time_class::Performance( )
{
	Stop();
	uint perf 	= Time_elapsed() / 10; // time elapsed in percentage w.r.t. 1 second = 1000 msec
	Start();
	return perf;
}

void Time_class::Wait( const uint& d )
{
    std::this_thread::sleep_for( chrono::seconds(d) );
}

void Time_class::Test()
{
	Set_Loglevel( TEST, true );
  	Comment( TEST, "wait for 2 seconds ");
	Start();
  	Wait( 2 );
	Comment( TEST, "elapsed time " + to_string( Time_elapsed()) + "ms");
}


