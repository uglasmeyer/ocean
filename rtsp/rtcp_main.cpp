/*
 * rtcp_main.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: sirius
 */

#include <App.h>
#include <System.h>
#include <Semaphore.h>

string				Module				= "rtsp";
Logfacility_class	Log( Module );
Interface_class		SDS;
Application_class	App( Module, RTSPID, &SDS );
Config_class 		Cfg( Module );
Semaphore_class		Sem;


void Start_audioserver()
{
    string Start_Audio_Srv = Cfg.Server_cmd( Cfg.Config.Term, file_structure().audio_bin, "" );
	system_execute( Start_Audio_Srv.data() );
	Log.Comment( INFO, Start_Audio_Srv );
}


void Start_synthesizer()
{
    string Start_Synthesizer = Cfg.Server_cmd( 	Cfg.Config.Term,
    											file_structure().synth_bin, "" );
    system_execute( Start_Synthesizer.data() );
    Log.Comment( INFO, Start_Synthesizer );

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
	SDS.addr->AudioServer = EXITSERVER;

}
void Stop_synthesizer()
{
	Log.Comment( INFO, "Receiver signal to stop");
	SDS.addr->Synthesizer = EXITSERVER;
}

void exit_proc( int signal )
{
	exit( signal );
}
int main(  int argc, char* argv[] )
{
	catch_signals( &exit_proc, { SIGHUP, SIGINT, SIGABRT } );
	Sem.Init();
	App.Start( argc, argv );

	if( App.Cfg.Config.oceangui == 'y' )
		Start_gui();
	if( App.Cfg.Config.composer )
	{
		Start_synthesizer();

		Log.Comment( INFO, "waiting for release of SEMAPHORE_STARTED");
		Sem.Lock( SEMAPHORE_STARTED );

		Start_audioserver();

		Log.Comment( INFO, "waiting for release of SEMAPHORE_EXIT");
		Sem.Lock( SEMAPHORE_EXIT );
		Stop_synthesizer();
		Stop_audioserver();

	}
	exit_proc( 0 );
	return 0;
}

