/*
 * rtcp_main.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: sirius
 */

#include <rtsp/Rtsp.h>


bool shmmixer_done = false;
#include <Time.h>

Time_class Timer{};

void shmmixer_fnc()
{
	Sem->Lock( RTSP_STARTED );
	Log.Comment( INFO, "Shared Memory Mixer Thread started");
	while ( true )
	{
//		Timer.Wait( 1 );
		Sem->Lock( SEMAPHORE_SENDDATA );
		if ( shmmixer_done )
			break;
		ShmMixer.AddShm();
		if( ShmMixer.error )
		{
			Log.Comment( ERROR, "Shm mixer returned with error code: " + to_string( ShmMixer.error ));
		}
		else
		{
			if ( not Sem->Log[ DEBUG ] )
				if (  Log.Log[ DEBUG ] ) cout.flush() << "#" ;
		}

	}
	Log.Comment( INFO, "Shared Memory Mixer Thread terminated");
}
thread shmmixer_thread( shmmixer_fnc );


void Start_audioserver()
{
    string Start_Audio_Srv = Cfg->Server_cmd( Cfg->Config.Term, file_structure().audio_bin, "-S 0" );
	system_execute( Start_Audio_Srv.data() );
}

void Start_synthesizer( uint cfgid )
{
    string Start_Synthesizer = Cfg->Server_cmd( Cfg->Config.Term,
    											file_structure().synth_bin,
												" -S " + to_string( cfgid ) );
    system_execute( Start_Synthesizer.data() );

}

void Start_gui()
{
    string Start_GUI = file_structure().ocean_bin + " &" ;
    system_execute( Start_GUI.data() );
    Log.Comment( INFO, Start_GUI );
}

void Stop_audioserver()
{
	Log.Comment( INFO, "Receiver signal to stop");
	sds->AudioServer = EXITSERVER;

}
void Stop_synthesizer()
{
	Log.Comment( INFO, "Receiver signal to stop");
	sds->Synthesizer = EXITSERVER;
}

int sig_counter = 0;
void exit_proc( int signal )
{
	if ( sig_counter > 0 )
	{
		Log.Comment( ERROR, "Exit procedure failed" );
		exit( signal );
	}
	sig_counter++;

	Log.Comment(INFO, "Entering exit procedure by signal " + to_string( signal ) );
	sds->Rtsp = OFFLINE;
	cout << endl;

	shmmixer_done = true;
	Sem->Reset( SEMAPHORE_SENDDATA );
	Log.Info( "Waiting for shm mixer thread to join");
	if ( shmmixer_thread.joinable() )
		shmmixer_thread.join();

	Log.Comment( INFO, "Reaching RTSP exit ");
	exit( 0 );
}
#include <Kbd.h>

void Test_rtsp()
{
	Keyboard_base Kbd;
	Keyboard_base::key_struct_t key = Keyboard_base::key_struct();
	while( key.key != '#')
	{
		cout << "." ;
		Timer.Wait(1);
	}
}

int main(  int argc, char* argv[] )
{
	catch_signals( &exit_proc, { SIGHUP, SIGINT, SIGABRT } );
	App.Start( argc, argv );
	sds = App.sds;
	ShmMixer.InitShm();

	if ( Cfg->Config.test == 'y' )
	{
		Test_rtsp();
		exit_proc( 0 );
	}
//	App.Shutdown_instance( );
	sds->Rtsp = EXITSERVER;
	App.Sds->Announce();

	Sem->Release( SYNTHESIZER_START);

	Sem->Release( RTSP_STARTED );
	sds->Rtsp = RUNNING;

	Log.Comment( INFO, "RTSP is " + DaTA.Sds.Decode( sds->Rtsp ) );

	Sem->Release( SEMAPHORE_SENDDATA );

	if( Cfg->Config.oceangui == 'y' )
	{
		Start_gui();
		return 0;
	}

	string cfg = "";
	if( Cfg->Config.composer == 'y' )
	{
		Start_synthesizer( DaTA.Cfg.Config.SDS_key );

		Log.Comment( INFO, "waiting for release of SEMAPHORE_STARTED");
		Sem->Lock( SEMAPHORE_STARTED );

		Start_audioserver();

		Log.Comment( INFO, "waiting for release of SEMAPHORE_EXIT");
		Sem->Lock( SEMAPHORE_EXIT );
		Stop_synthesizer();
		Stop_audioserver();
		return 0;
	}

	uint cfgid = 0;
	while(true)
	{
		Log.Comment( INFO, "waiting for release of SYNTHESIZER_START");
		Log.Comment( INFO, "use OceanGUI to start additional synthesizer");
		Sem->Lock( SYNTHESIZER_START );
		if ( sds->Rtsp == EXITSERVER )
		{
			sds->Rtsp = OFFLINE;
			exit_proc( 0 );
		}

		Start_synthesizer( cfgid );
		Log.Comment( INFO, "waiting for release of SYNTHESIZER_STARTED");
		Sem->Lock( SEMAPHORE_STARTED, 2 );
		cfgid = ( cfgid + 1 ) % MAXCONFIG;
	}
	exit_proc( 0 );
	return 0;
}

