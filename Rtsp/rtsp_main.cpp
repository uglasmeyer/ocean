/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************//*
 * rtcp_main.cpp
 *
 *  Created on: Sep 24, 2024
 */


#include <rtsp/Rtsp.h>

void Start_audioserver()
{
    string Start_Audio_Srv = Cfg.Server_cmd( 	Cfg.Config.Nohup,
    											Cfg.fs->audio_bin,
												"-S 0 2>&1 >> " +
												Cfg.fs->nohup_file );
	System_execute( Start_Audio_Srv );
}

void Start_synthesizer( uint cfgid )
{
    string Start_Synthesizer = Cfg.Server_cmd( Cfg.Config.Nohup,
    											Cfg.fs->synth_bin,
												" -S " + to_string( cfgid ) +
												" 2>&1 > "  + Cfg.fs->nohup_file);
    System_execute( Start_Synthesizer );

}

void Start_gui()
{
    string Start_GUI = Cfg.fs->ocean_bin + " &" ;
    System_execute( Start_GUI );
    Log.Comment( INFO, Start_GUI );
}

void Stop_audioserver()
{
	Log.Comment( INFO, "Receiver signal to stop");
	DaTA.Appstate.SetExitserver( sds_master, AUDIOID );

}
void Stop_synthesizer()
{
	Log.Comment( INFO, "Receiver signal to stop");
	DaTA.Appstate.SetExitserver( sds, APPID::SYNTHID ); // TODO wrong sds
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
	int key = 0;
	while( key != '#')
	{
		cout << "." ;
		Timer.Wait(1);
	}
}

int main(  int argc, char* argv[] )
{

	App.Start( argc, argv );
	sds = App.sds;

	if ( Cfg.Config.test == 'y' )
	{
		Test_rtsp();
		exit_proc( 0 );
	}


	Sem.Release( SYNTHESIZER_START);

	Sem.Release( RTSP_STARTED );

	StateId_t rtsp_state = Appstate->GetState( sds, App.AppId );
	Log.Comment( INFO, "RTSP is ", Sdsstate.sdsstateName( rtsp_state ));

	if( Cfg.Config.oceangui == 'y' )
	{
		Start_gui();
		return 0;
	}
	string cfg = "";
	if( Cfg.Config.composer == 'y' )
	{
		Start_synthesizer( 0 );

		Log.Comment( INFO, "waiting for release of SEMAPHORE_STARTED");
		Sem.Lock( SEMAPHORE_STARTED );

		Start_audioserver();

		// Synthesizer will send SEMAPHORE_EXIT on exit
		Log.Comment( INFO, "waiting for release of SEMAPHORE_EXIT");
		Sem.Lock( SEMAPHORE_EXIT );
		Stop_synthesizer();
		Stop_audioserver();
		return 0;
	}

	uint cfgid = 0;
	while(true)
	{
		Log.Comment( INFO, "waiting for release of SYNTHESIZER_START");
		Log.Comment( INFO, "use OceanGUI to start additional synthesizer");
		Sem.Lock( SYNTHESIZER_START );
		if ( DaTA.Appstate.IsExitserver( sds_master, RTSPID ) )
		{
			exit_proc( 0 );
		}

		Start_synthesizer( cfgid );
		Log.Comment( INFO, "waiting for release of SYNTHESIZER_STARTED");
		Sem.Lock( SEMAPHORE_STARTED, 2 );
		cfgid = ( cfgid + 1 ) % MAXCONFIG;
	}
	exit_proc( 0 );
	return 0;
}

