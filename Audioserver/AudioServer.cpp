
#include <Audioserver.h>


bool 					RecordThreadDone 	= false;
Value 					Fileno 				{0};

void save_record_fcn()
{
	Fileno 				= External.Save_record_data( sds_master->FileNo );// 0=generate file no - (int) DaTA.sds_master->FileNo;
	sds->FileNo 		= 0;

	Log.Comment( INFO, "record thread received job " + Fileno.str);

	string filename = fs.get_rec_filename( Fileno.val);
	Sds->Write_str( OTHERSTR_KEY, filename );
	Log.Info( "recording to file " + filename + " done.");

	DaTA.EmitEvent( RECORDWAVFILEFLAG );
}
Thread_class			SaveRecord			{ &Sem,
											  SEMAPHORE_RECORD,
											  save_record_fcn,
											  "save record file" };
thread* SaveRecord_thread_p = nullptr;

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
	coutf << Line(80, 'v' );
	Log.Comment( INFO,  Application + "received command exit signal " + to_string( signal ) );
	Log.Comment( INFO, "Entering exit procedure for " + App.This_Application );
}

void show_usage( void )
{
	string str;
	str = "Usage: " + DaTA.Cfg_p->prgname + " -c #1 -r #2 -d #3 -o #4 \n";
	str.append("       where \n");
	str.append("       #1 = number of channels (default=2),\n");
	str.append("       #2 = the sample rate (default = 44100),\n");
	str.append("       #3 = device index (default = 0, default device),\n");
	str.append("       #4 = channelOffset on the device (default = 0) ");
	Log.Comment( INFO , str );
}

void Application_loop()
{
	Log.Comment(INFO, "Entering Application loop \n");
	Log.Comment(INFO, DaTA.Cfg_p->prgname + " is ready");

	while ( not audio_out_done )//and Audio.isStreamRunning() )
	{
	    this_thread::sleep_for( std::chrono::milliseconds(100));
		App.KeyboardKey( false );
	}

	Log.Comment(INFO, "Application loop exit");
}

void write_waveaudio()
{
	if ( sds->WD_status.roleId != osc_struct::AUDIOOUTID )
		return;


	for( buffer_t n = 0; n < audio_frames; n++ )
	{
		mono_out.Data[n] = stereo.stereo_data[n].left + stereo.stereo_data[n].right;//shm_addr[n].left + shm_addr[n].right;
	}

	Wavedisplay.SetDataPtr( sds->WD_status );
	Wavedisplay.Write_wavedata();
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
	DaTA.ClearShm( audio_frames );
	shm_addr = DaTA.SetShm_addr( );

	Volume.DynVolume.SetupVol( sds_master->Master_Amp, sds_master->vol_slidemode );
	Volume.Transform( audio_frames, shm_addr, stereo.stereo_data );

	Request_data();
}

bool record_start( )
{
	Sds->addr->Record_state 			= sdsstate_struct::RECORDING;
	External.status.record 				= true;
	sds->StA_state[ STA_EXTERNAL].store = true;
	rcounter 							= 0;
	ProgressBar.Set						( &rcounter, recduration * 1000/min_msec );

	RecTimer.Start						();
	Log.Comment(INFO, "Audioserver starts recording" );
	return true;
}

void record_stop( bool init )
{
	if ( not init )
		DaTA.Sem_p->Release( SEMAPHORE_RECORD ); // save recording to file

	ProgressBar.Unset();
	External.status.record 				= false; // Audioser internal recording state
	sds->Record_state 					= sdsstate_struct::INACTIVE; //public record state
	sds->StA_state[ STA_EXTERNAL ].store= false;

	uint sec_elapsed 					= RecTimer.Time_elapsed() /1000;
	Log.Info( "Record duration: ", sec_elapsed, " sec");
}
void set_rcounter( )
{
	rcounter++;  // # of recorded audio_frames
	ProgressBar.Update();
	if( rcounter * audio_frames >= recduration * frames_per_sec )
		record_stop( false );
}

//Time_class Measure{};
void set_ncounter( buffer_t n )
{
	if(n < audioframes )
		return;

	ncounter = 0;
	if ( External.status.record )
	{
		External.Record_buffer( shm_addr, audioframes );
		set_rcounter();
	}

	call_for_update();

	write_waveaudio();

	//		cout << Measure.Time_elapsed() << "[msec]" << endl;
	//		Measure.Start();
}

typedef stereo_t frame_t;
frame_t* frame = nullptr;

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
		buffer[i]	= stereo.stereo_data[ncounter]; // shm_addr[ncounter];
		ncounter 	= (ncounter	+ 1 );
	}

	if ( DaTA.Appstate.IsExitserver( sds, AUDIOID ) ) 	// set by ocean process
		audio_out_done = true;
	if ( audio_out_done ) 								// called by exit_proc
		return 1;										// terminate and drain
//		return 2;										// abort
	if( (sds->AudioServer == sdsstate_struct::RECORDSTART ) or	( Note_start.get() ))
		record_start();
	if( (sds->AudioServer == sdsstate_struct::RECORDSTOP ) or	( Note_end.get() ))
		record_stop( false );

	DaTA.Appstate.SetRunning(  );

	return 0; 											// continue thread
} // RtAudio callback function

int main( int argc, char *argv[] )
{

	App.Start(argc, argv );

	// init logging
	Log.Show_loglevel();

	// init communication
	for ( uint n = 0; n < MAXCONFIG; ++n)
	{
		uint8_t id = SEMAPHORE_SENDDATA0 + n;
		DaTA.Sem_p->Reset( id );
	}
	record_stop( true ); // init
	DaTA.Sem_p->Reset( SEMAPHORE_RECORD );

	// init SDS
	sds->audioframes	= audioframes;
	sds->RecCounter 	= 0;
	sds->Record_state	= sdsstate_struct::INACTIVE;

	// init record thread
	thread
	SaveRecord_thread( &Thread_class::Loop, &SaveRecord ); // run class method: Loop in a thread
	SaveRecord_thread_p = &SaveRecord_thread;


	// init wavedisplay
	wd_status_t wd_status 	= WD_status_struct();
	wd_status.roleId 		= osc_struct::AUDIOOUTID;
	wd_status.oscId 		= osc_struct::OSCID;
	Wavedisplay.Add_role_ptr( wd_status.roleId, mono_out.Data, &audioframes);
	Wavedisplay.SetDataPtr( wd_status );

	Audio.Show_parameter();

// end parametrization
// -------------------------------------------------------------------------------


	show_usage();

	// An error in the openStream() function can be detected either by
	// checking for a non-zero return value OR by a subsequent call to
	// isStreamOpen().


	// start rtapi stream
	Log.Comment(INFO, "Opening Audio Stream");
	frame = (frame_t* ) calloc( Cfg.Config.channel, sizeof( frame_t ) );

	if ( Audio.openStream(
						&Audio.oParams,
						NULL,
						FORMAT,
						Cfg.Config.rate,
						&Audio.bufferFrames,
						&RtAudioOut,
						( void* )frame,
						&Audio.options ) )
	{
		Log.Comment( ERROR, Audio.getErrorText() );
		exit_proc( SIGHUP);
	}

	Audio.Start_audio_stream();

    DaTA.Sem_p->Reset( SEMAPHORE_STARTED );

	call_for_update();
	App.Ready();

	Log.Comment(INFO, "Application initialized");

	// audio server loop
	Application_loop();


	Log.Comment( INFO, "AudioServer reached target exit main()" );
	exit_proc( 0 );

	return 0;

}

void exit_proc( int signal )
{
	audio_out_done = true;
	if ( exit_flag )
		exit(0);
	exit_intro( signal );
	shutdown_thread();

	Audio.Shutdown_stream();
	if ( frame )
	{
		free( frame);
		Log.Info( "frame buffer freed" );
	}
	cout.flush()  << " ";


//	Log.Set_Loglevel( DBG2, true );
	exit ( 0 );
}
