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
****************************************************************************/

#include <Audioserver.h>


void errorCallback( RtAudioErrorType, const std::string& txt )
{
	Log.Comment(ERROR, txt ) ;
	errno =  0;
}

//bool 					RecordThreadDone 	= false;
void save_record_fcn()
{
	string filename		= Record.External.Save_record_data( sds_master->FileNo );
	Sds->Write_str( OTHERSTR_KEY, filename );

	Log.Comment( INFO, "record thread received job ", filename );
	Log.Info( "recording to file " + filename + " done.");

	DaTA.EmitEvent( RECORDWAVFILEFLAG );
};
Thread_class			SaveRecord			{ &Sem,
											  SEMAPHORE_RECORD,
											  save_record_fcn,
											  "save record file" };
thread*					SaveRecord_thread_p = nullptr;

void shutdown_thread( )
{
	SaveRecord.StopLoop();
	if (SaveRecord_thread_p )
	{
		SaveRecord_thread_p->join();
		Log.Comment( INFO, "Record thread joined" );
	}
}

bool exit_flag = false;
void exit_intro( int signal )
{
	exit_flag = true;
	coutf << Line(80, 'v' ) << endl;
	Log.Comment( INFO,  Cfg.prgName, " received command exit signal ",  signal );
	Log.Comment( INFO, "Entering exit procedure" );
}

void show_usage( void )
{
	string str;
	str = "Usage: " + Cfg.prgName + " -c #1 -r #2 -d #3 -o #4 \n";
	str.append("       where \n");
	str.append("       #1 = number of channels (default=2),\n");
	str.append("       #2 = the sample rate (default = 48000),\n");
	str.append("       #3 = device index (default = 0, default device),\n");
	str.append("       #4 = channelOffset on the device (default = 0) ");
	Log.Comment( INFO , str );
}

void Application_loop()
{
	Log.Comment(INFO, "Entering Application loop \n");
	App.Ready();


	while ( not audio_out_done )//and Audio.isStreamRunning() )
	{
		App.KeyboardKey( false );
	    this_thread::sleep_for( std::chrono::milliseconds(100));
	}

	Log.Comment(INFO, "Application loop exit");
}

void write_waveaudio()
{
	if ( sds->WD_state.roleId != AUDIOROLE )
		return;


	for( buffer_t n = 0; n < audio_frames; n++ )
	{
		wd_mono_mem.Data[n] = stereo.stereo_data[n].left + stereo.stereo_data[n].right;//shm_addr[n].left + shm_addr[n].right;
	}

	Wavedisplay.Set_WdData();
	Wavedisplay.WriteData();
}

void Request_data()
{
	for ( int n = 0; n < 4; ++n)
	{
		uint8_t id = SEMAPHORE_SENDDATA0 + n;
		DaTA.Sem_p->Release( id );
	}
}

void call_for_update()
{
	DaTA.ClearShm();
	shm_addr = DaTA.SetShm_addr();

	Volume.Transform( audio_frames, shm_addr, stereo.stereo_data );

	Request_data();
}



//Time_class Measure{};
void set_ncounter( buffer_t n )
{
	if(n < audio_frames )
		return;

	ncounter = 0;

	if (Record.State( Record.RECORDSTART) )
		Record.Start();
	if (Record.State( Record.RECORDSTOP ) )
		Record.Stop ();

	if ( Record.recording )
	{
		Record.Store_audioframes( shm_addr, audio_frames );
		Record.Set_rcounter();
	}

	call_for_update();

	write_waveaudio();

	//		cout << Measure.Time_elapsed() << "[msec]" << endl;
	//		Measure.Start();
}



int RtAudioOut(	void* outputBuffer,
				void* /*inputBuffer*/,
				uint nBufferFrames,
				double streamTime,
				RtAudioStreamStatus status,
				void* /* userData */)
{
	frame_t* buffer { (frame_t* ) outputBuffer };

	if ( status != 0 ) // buffer overflow
		Log.Comment( WARN, "Buffer status differs: ",(int) status );

	set_ncounter( ncounter );

	for ( uint i = 0; i < chunksize; i++ )
	{
		buffer[i]	= stereo.stereo_data[ncounter];
		ncounter 	= (ncounter	+ 1 );
	}

	if ( Appstate->IsExitserver( sds, AUDIOID ) ) 		// set by the oceangui process
		audio_out_done = true;							// trigger Application_loop() to terminate
	if ( audio_out_done ) 								// set by exit_proc signal
		return 1;										// terminate stream and drain
//		return 2;										// return abort

	Appstate->SetRunning(  );

	return 0; 											// return value to continue thread
} // RtAudio callback function

int main( int argc, char *argv[] )
{

	App.Start(argc, argv );

//	init logging
	Log.Show_loglevel();

// 	init communication
	for ( uint n = 0; n < MAXCONFIG; ++n)
	{
		uint8_t id = SEMAPHORE_SENDDATA0 + n;
		DaTA.Sem_p->Reset( id );
	}


//	init record thread
	thread
	SaveRecord_thread( &Thread_class::Loop, &SaveRecord ); // run class method: Loop in a thread
	SaveRecord_thread_p = &SaveRecord_thread;


// 	init wavedisplay
	buffer_t audioframes = audio_frames; // not constant
	Wavedisplay.Add_role_ptr( AUDIOROLE, wd_mono_mem.Data, &audioframes);

// end parametrization
// -------------------------------------------------------------------------------

	show_usage();

	Audio.Start_stream( &RtAudioOut );

	call_for_update();
	Application_loop();

	Log.Comment( INFO, Cfg.prgName, " reached target exit main()" );
	exit_proc( 0 );

	return 0;

}

void exit_proc( int signal )
{
	if ( signal == SIGILL )
		exit(0);
	audio_out_done = true;
	if ( exit_flag )
		exit(0);
	exit_intro( signal );
	shutdown_thread();

	Audio.Shutdown_stream();
	cout.flush()  << " ";


//	Log.Set_Loglevel( DBG2, true );
	exit ( 0 );
}
