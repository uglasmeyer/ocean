/*
 * rtcp_main.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: sirius
 */

#include <rtsp/Rtsp.h>


#include <Time.h>
#include <Logfacility.h>

Time_class Timer{};

void Start_audioserver()
{
    string Start_Audio_Srv = Cfg_p->Server_cmd( 	Cfg_p->Config.Nohup,
    											file_structure().audio_bin,
												"-S 0 2>&1 >> " +
												file_structure().nohup_file );
	system_execute( Start_Audio_Srv );
}

void Start_synthesizer( uint cfgid )
{
    string Start_Synthesizer = Cfg_p->Server_cmd( Cfg_p->Config.Nohup,
    											file_structure().synth_bin,
												" -S " + to_string( cfgid ) +
												" 2>&1 > "  + file_structure().nohup_file);
    system_execute( Start_Synthesizer );

}

void Start_gui()
{
    string Start_GUI = file_structure().ocean_bin + " &" ;
    system_execute( Start_GUI );
    Log.Comment( INFO, Start_GUI );
}

void Stop_audioserver()
{
	Log.Comment( INFO, "Receiver signal to stop");
	Appstate->SetExitserver( sds_master, AUDIOID );

}
void Stop_synthesizer()
{
	Log.Comment( INFO, "Receiver signal to stop");
	Appstate->SetExitserver( sds, APPID::SYNTHID ); // TODO wrong sds
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
	cout << endl;

	Log.Comment( INFO, "Reaching RTSP exit ");
	exit( 0 );
}
#include <Kbd.h>

void Test_rtsp()
{
	Kbd_base Kbd;
	key3struct_t key = Kbd_base::key3_struct( 0,0,0 );
	while( key.key != '#')
	{
		cout << "." ;
		Timer.Wait(1);
	}
}

int main(  int argc, char* argv[] )
{

	App.Start( argc, argv );
	sds = App.sds;

	if ( Cfg_p->Config.test == 'y' )
	{
		Test_rtsp();
		exit_proc( 0 );
	}


	Sem_p->Release( SYNTHESIZER_START);

	Sem_p->Release( RTSP_STARTED );

	Log.Comment( INFO, "RTSP is " + sdsstate_struct().state_map[ sds->Rtsp ] );

	if( Cfg_p->Config.oceangui == 'y' )
	{
		Start_gui();
		return 0;
	}
//	assert( sds->UserInterface == OFFLINE );
	string cfg = "";
	if( Cfg_p->Config.composer == 'y' )
	{
		Start_synthesizer( 0 );

		Log.Comment( INFO, "waiting for release of SEMAPHORE_STARTED");
		Sem_p->Lock( SEMAPHORE_STARTED );

		Start_audioserver();

		Log.Comment( INFO, "waiting for release of SEMAPHORE_EXIT");
		Sem_p->Lock( SEMAPHORE_EXIT );
		Stop_synthesizer();
		Stop_audioserver();
		return 0;
	}

	uint cfgid = 0;
	while(true)
	{
		Log.Comment( INFO, "waiting for release of SYNTHESIZER_START");
		Log.Comment( INFO, "use OceanGUI to start additional synthesizer");
		Sem_p->Lock( SYNTHESIZER_START );
		if ( Appstate->IsExitserver( sds_master, RTSPID ) )
		{
			exit_proc( 0 );
		}

		Start_synthesizer( cfgid );
		Log.Comment( INFO, "waiting for release of SYNTHESIZER_STARTED");
		Sem_p->Lock( SEMAPHORE_STARTED, 2 );
		cfgid = ( cfgid + 1 ) % MAXCONFIG;
	}
	exit_proc( 0 );
	return 0;
}

