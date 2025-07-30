
#include <Audioserver.h>




void errorCallback( RtAudioErrorType /*type*/, const std::string& errorText )
{
	//Log.Comment(ERROR, errorText ) ;
	std::cout.flush() << errorText << endl;
}
RtAudio::StreamParameters 	oParams;
RtAudio::StreamOptions 		options = {	.flags = RTAUDIO_HOG_DEVICE,
										.numberOfBuffers = 2,
										.streamName = Application };

void 	show_parameter()
{
// The parameter list is available after stream open
Log.Comment(INFO , 	"Parameters in use:\n       buffer size        " +
												to_string( bufferFrames ) + "\n" +
					"       number of buffers  " + to_string( options.numberOfBuffers ) + "\n" +
					"       Stream latency     " + to_string( rtapi.getStreamLatency()) + "\n" +
					"       Stream sample rate " + to_string( rtapi.getStreamSampleRate()) + "\n" +
					"       Stream name        " + options.streamName + "\n" +
					"       RtApi option flags " + to_string( options.flags )+ "\n" +
					"       Device Id          " + to_string( oParams.deviceId ) + "\n" +
					"       Device name        " + DeviceDescription.Name  );
}

void start_audio_stream()
{
	if ( not rtapi.isStreamOpen() )
	{
		EXCEPTION( "Failed to open Audio Stream" );
	}
	else
		Log.Comment( INFO, "Audio Stream is open ") ;

	if ( rtapi.startStream() )
	{
		EXCEPTION( rtapi.getErrorText() );
	}
	else
		Log.Comment( INFO, "Audio Stream is started ") ;
}


/*
 * Exit
 */

typedef stereo_t frame_t;
frame_t* frame = nullptr;

string txt {""};
void shutdown_stream()
{
	if ( rtapi.isStreamRunning() )
		rtapi.stopStream();
	Log.Info( rtapi.getErrorText() );
	if ( rtapi.isStreamOpen() )
		rtapi.closeStream();
	Log.Info( rtapi.getErrorText() );
/*	if ( rtapi.isStreamRunning() )
	{
		Log.Comment(INFO,"stream will be stopped");

		try
		{
			rtapi.stopStream();  // or could call dac.abortStream();
		}
		catch (bad_alloc const& ex)
		{
			cout << "bad alloc" << endl;
		}

		// TODO  - causes valgrind error
	}
	else
		return;

	if ( rtapi.isStreamOpen() )
	{
		Log.Comment(INFO, "stream will be closed");
		rtapi.closeStream();
		if ( rtapi.isStreamOpen() )
			Log.Comment( ERROR, "stream is still open");
	}
	txt = rtapi.getErrorText();
	if ( txt.length() == 0 )  txt = "None";
	Log.Comment( ERROR, txt );
*/
}



bool 					SaveRecordFlag 		= false;
bool 					RecordThreadDone 	= false;
Value Fileno {0};

void save_record_fcn()
{
	SaveRecordFlag = true;
	Fileno = 0;
	sds->FileNo = Fileno.val;// 0=generate file no - (int) DaTA.sds_master->FileNo;
	Log.Comment( INFO, "record thread received job " + Fileno.str);

	Fileno = External.Save_record_data( Fileno.val );
	sds->FileNo = Fileno.val;
	string filename = file_structure().get_rec_filename( Fileno.val);
	Sds->Write_str( OTHERSTR_KEY, filename );
	Log.Info( "recording to file " + filename + " done.");
	DaTA.EmitEvent( RECORDWAVFILEFLAG );
	SaveRecordFlag = false;
}
Thread_class			SaveRecord			{ &Sem,
											  SEMAPHORE_RECORD,
											  save_record_fcn,
											  "save record" };
thread* SaveRecord_thread_p = nullptr;

thread Record_thread( save_record_fcn );

void shutdown_thread( )
{
	SaveRecord.StopLoop();
	if (SaveRecord_thread_p )
	{
		SaveRecord_thread_p->join();
		Log.Comment( INFO, "Record thread joined" );
	}
}

int sig_counter = 0;
void exit_intro( int signal )
{
	if ( sig_counter > 0 )
	{
		Log.Comment( ERROR, "Exit procedure failed" );
		Log.Comment( WARN, "Audioserver reached target exit " + to_string( signal ));
		exit( signal );
	}
	sig_counter++;

	Log.Comment( INFO,  Application + "received command exit signal " + to_string( signal ) );
	Log.Comment( INFO, "Entering exit procedure for " + App.This_Application );
	Log.Comment( INFO, "suspend server" );
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


unsigned int selectDeviceIndex( std::vector<std::string> deviceNames )
{
	unsigned int 	device_index;
	std::string 	keyHit;
	std::cout << '\n';

	for ( device_index = 0; device_index < deviceNames.size(); device_index++ )
		std::cout << "  Device #" << device_index << ": " << deviceNames[device_index] << '\n';

	do
	{
		std::cout << "\nChoose a device #: ";
		std::cin >> device_index;
	} while ( device_index >= deviceNames.size() );
	std::getline( std::cin, keyHit );  // used to clear out stdin

	return device_index;
}


void get_device_description( uint index )
{
	std::vector<unsigned int> 	deviceIds 	= rtapi.getDeviceIds();
	std::vector<string> 		deviceNames = rtapi.getDeviceNames();

	if ( deviceIds.size() < 1 )
	{
		Log.Comment( ERROR, "No audio devices found!" );
		exit_proc( 5 );
	}

	Log.Comment( INFO,  "Available  audio devices :");
	int ind = 0;
	for ( uint dev : deviceIds )
	{
		Log.Comment( INFO,  "device Id: " + to_string( dev ) + " " + deviceNames[ ind ] );
		ind++;
	}

	if ( index >= deviceIds.size() )
	{
		index = selectDeviceIndex( deviceNames );
	}

	if ( index == 0 )
	{
		DeviceDescription = {0, rtapi.getDefaultOutputDevice(), "default output device" };
	}
	else
	{
		DeviceDescription = {index, deviceIds[ index ], deviceNames[ index ] };
	}
}


void Application_loop()
{
	Log.Comment(INFO, "Entering Application loop \n");
	Log.Comment(INFO, DaTA.Cfg_p->prgname + " is ready");

	while ( not done and rtapi.isStreamRunning() )
	{
	    this_thread::sleep_for( std::chrono::seconds(1));
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
	//audioframes = min_frames;// SetAudioFrames();
	shm_addr = DaTA.SetShm_addr( );

	Volume.DynVolume.SetupVol( sds_master->Master_Amp, sds_master->vol_slidemode );
	Volume.Transform( audio_frames, shm_addr, stereo.stereo_data );

	Request_data();
}

void record_start( )
{
	if ( SaveRecordFlag )
	{
		Log.Comment( WARN, "Audioserver is still saving data. ... Wait ");
		return;
	}
	Sds->addr->Record 					= true;
	External.status.record 				= true;
	rcounter 							= 0;
	ProgressBar.Set						( &rcounter, recduration );
	RecTimer.Start						();
	sds->StA_state[ STA_EXTERNAL].store = true;

	Log.Comment(INFO, "Audioserver starts recording" );
}

void record_stop()
{
	DaTA.Sem_p->Release( SEMAPHORE_RECORD );

	External.status.record = false;
	ProgressBar.Unset();
	uint t_el = RecTimer.Time_elapsed();
	sds->StA_state[ STA_EXTERNAL ].store = false;
	Sds->addr->Record = false;

	Log.Comment(INFO, "Record duration: " + to_string( t_el/1000 ) + " sec");

}
void set_rcounter( )
{
	rcounter++;
	ProgressBar.Update();
	if( rcounter >= recduration )
		record_stop();
}

//Time_class Measure{};
void set_ncounter( buffer_t n )
{

	if ( n > audioframes - 1 )
	{
//		cout << Measure.Time_elapsed() << "[msec]" << endl;
//		Measure.Start();
		ncounter = 0;
		if ( External.status.record )
		{
			External.Record_buffer( shm_addr, audioframes );
			set_rcounter();
		}

		call_for_update();

		write_waveaudio();

	}
}


int RtAudioOut(	void *outputBuffer,
				void * /*inputBuffer*/,
				uint nBufferFrames,
				double streamTime,
				RtAudioStreamStatus status,
				void *data )
{
		stereo_t* buffer { (stereo_t* ) outputBuffer };

		// keep all variable data initializations outside of RtAudioOut
		if ( status != 0 ) // buffer overflow
			Log.Comment( WARN, "Buffer status differs: " + to_string( status ));

		set_ncounter( ncounter );

		// the output loop is implicitly protect against status changes by the fact
		// that the buffer boundaries are aligned to 44100 respectively one second

		for ( uint i = 0; i < chunksize; i++ )
		{
			buffer[i]	= stereo.stereo_data[ncounter]; // shm_addr[ncounter];
			ncounter 	= (ncounter	+ 1 );
		}

		if ( DaTA.Appstate.IsExitserver( sds, AUDIOID ) )
		{
			done = true;
			return 1;
		}

		if (sds->AudioServer == sdsstate_struct::RECORDSTART )
			record_start();

		if (sds->AudioServer == sdsstate_struct::RECORDSTOP )
			record_stop();

		DaTA.Appstate.SetRunning(  );

		return 0;
} // callback function



int main( int argc, char *argv[] )
{

	App.Start(argc, argv );

	// init logging
	Log.Set_Loglevel(DEBUG, false);
	Log.Show_loglevel();

	// init communication
	for ( uint n = 0; n < MAXCONFIG; ++n)
	{
		uint8_t id = SEMAPHORE_SENDDATA0 + n;
		DaTA.Sem_p->Reset( id );
	}

	// init SDS
	sds->audioframes	= audioframes;
	sds->RecCounter 	= 0;
	sds->Record			= false;

	// init record thread
	thread 	SaveRecord_thread( &Thread_class::Loop, &SaveRecord ); // run class method: Loop in a thread
	SaveRecord_thread_p = &SaveRecord_thread;
	//    DaTA.Sem.Reset( SEMAPHORE_RECORD );


	// init wavedisplay
	wd_status_t wd_status 	= WD_status_struct();
	wd_status.roleId 		= osc_struct::AUDIOOUTID;
	wd_status.oscId 		= osc_struct::OSCID;
	Wavedisplay.Add_role_ptr( wd_status.roleId, mono_out.Data, &audioframes);
	Wavedisplay.SetDataPtr( wd_status );

	// init rtape parameter
//	wav_header.srate 				= Cfg->Config.rate;
//	wav_header.num_chans 			= Cfg->Config.channel;
//	wav_header.bytes_per_sec 		= wav_header.num_chans * wav_header.bytes_per_samp * wav_header.srate;

	// dynamic rtapi output parameter
	// Tell RtAudio to output all messages, even warnings.
	rtapi.showWarnings( true );
	//	frame = (double *) calloc( Cfg->Config.channel, sizeof( double ) );
	frame = (frame_t* ) calloc( DaTA.Cfg_p->Config.channel, sizeof( frame_t ) );
	oParams.nChannels 		= DaTA.Cfg_p->Config.channel;
	oParams.firstChannel 	= DaTA.Cfg_p->Config.ch_offs;
	get_device_description( DaTA.Cfg_p->Config.device );
	oParams.deviceId 		= DeviceDescription.Id;
	#define USE_INTERLEAVED
	#if !defined( USE_INTERLEAVED )
			options.flags |= RTAUDIO_NONINTERLEAVED,
	#endif




// end parametrization
// -------------------------------------------------------------------------------


	show_usage();
	show_parameter();

	// An error in the openStream() function can be detected either by
	// checking for a non-zero return value OR by a subsequent call to
	// isStreamOpen().

	// start rtapi stream
	Log.Comment(INFO, "Opening Audio Stream");
	if ( rtapi.openStream(
						&oParams,
						NULL,
						FORMAT,
						DaTA.Cfg_p->Config.rate,
						&bufferFrames,
						&RtAudioOut,
						( void* )frame,
						&options ) )
	{
		Log.Comment( ERROR, rtapi.getErrorText() );
		exit_proc(6);
	}

	start_audio_stream();

    DaTA.Sem_p->Reset( SEMAPHORE_STARTED );

	call_for_update();
	App.Ready();

	Log.Comment(INFO, "Application initialized");
	Log.Comment(INFO, "Entering Application loop\n");

	// audio server loop
	while ( not done  && rtapi.isStreamRunning() )
	{
	    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
	}
	Log.Comment( INFO, "AudioServer reached target exit main()" );
	exit_proc(0);

//	return 0;

}

void exit_proc( int signal )
{

	done = true;
	shutdown_stream();
	cout.flush();
	//	exit_intro( signal );

	shutdown_thread();
	if ( frame )
	{
		free( frame);
		Log.Info( "frame buffer freed" );
	}
	if( signal > 0 )
		exit( signal );
}
