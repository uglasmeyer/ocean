/*
 * rtcp_main.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: sirius
 */

#include <Rtsp.h>

void Start_audioserver()
{
    string Start_Audio_Srv = Cfg->Server_cmd( Cfg->Config.Term, file_structure().audio_bin, "" );
	system_execute( Start_Audio_Srv.data() );
}


void Start_synthesizer( uint cfgid )
{
    string Start_Synthesizer = Cfg->Server_cmd( Cfg->Config.Term,
    											file_structure().synth_bin,
												" -K " + to_string( cfgid ) );
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

string define_config(  )
{
	uint cfgid = sds->config;
	cfgid = ( cfgid + 1 ) % MAX_CONFIG;
	sds->config = cfgid;
	string config = Dir.rtspdir + "S" + to_string( cfgid ) + ".cfg";
	return config;
}

void exit_proc( int signal )
{
	sds->Rtsp = OFFLINE;
	cout << endl;
	Log.Comment( INFO, "RTSP is " + DaTA.Sds.Decode( sds->Rtsp ) );
	exit( 0 );
}

int main(  int argc, char* argv[] )
{
	catch_signals( &exit_proc, { SIGHUP, SIGINT, SIGABRT } );
	Sem->Init();
	App.Start( argc, argv );
//	App.Shutdown_instance( );
	sds->Rtsp = EXITSERVER;
	Sem->Release( SYNTHESIZER_START);

	sds->Rtsp = RUNNING;
	Sem->Release( RTSP_STARTED );
	Log.Comment( INFO, "RTSP is " + DaTA.Sds.Decode( sds->Rtsp ) );

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
	Start_synthesizer( cfgid );
	while(true)
	{

		Log.Comment( INFO, "waiting for release of SYNTHESIZER_START");
		Log.Comment( INFO, "use OceanGUI to start additional synthesizer");
		Sem->Lock( SYNTHESIZER_START );
		if ( sds->Rtsp == EXITSERVER )
			exit_proc( 0 );

		Start_synthesizer( cfgid );
		Sem->Lock( SEMAPHORE_STARTED, 2 );
		cfgid = ( cfgid + 1 ) % MAXCONFIG;
	}
	return 0;
}

