
#include <Audioserver.h>

RtAudio::StreamParameters 	oParams;
RtAudio::StreamOptions 		options = {	.flags = RTAUDIO_HOG_DEVICE,
										.numberOfBuffers = 8,
										.streamName = Application };


void errorCallback( RtAudioErrorType /*type*/, const string& errorText )
{
	Log.Comment(ERROR, errorText ) ;
}
RtAudio rtapi( RtAudio::LINUX_PULSE, &errorCallback );


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
		Exception( "Failed to open Audio Stream" );
	else
		Log.Comment( INFO, "Audio Stream is open ") ;

	if ( rtapi.startStream() )
		Exception( rtapi.getErrorText() );
	else
		Log.Comment( INFO, "Audio Stream is started ") ;
}

typedef stereo_t frame_t;
//double* frame = nullptr;
frame_t* frame = nullptr;
void shutdown_stream()
{
	if ( rtapi.isStreamRunning() )
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
	}
	if ( rtapi.isStreamOpen() )
	{
		Log.Comment(INFO, "stream will be closed");
		rtapi.closeStream();
		if ( rtapi.isStreamOpen() )
			Log.Comment( ERROR, "stream is still open");
	}
	string txt = rtapi.getErrorText();
	if ( txt.length() == 0 )  txt = "None";
	Log.Comment( ERROR, txt );

}


bool 					SaveRecordFlag 		= false;
bool 					RecordThreadDone 	= false;
void save_record_fcn()
{
	Log.Comment( INFO, "record thread started ");

	Value Fileno {0};
	while ( true )
	{
		DaTA.Sem.Lock( SEMAPHORE_RECORD );
		if ( RecordThreadDone )
			break;
		SaveRecordFlag = true;
		Fileno = (int) DaTA.sds_master->FileNo;
		Log.Comment( INFO, "record thread received job " + Fileno.str);

		External.Save_record_data( Fileno.val );

		Log.Comment( INFO, "recording done");
		DaTA.Sds_p->Update( RECORDWAVFILEFLAG );
		SaveRecordFlag = false;
	}

	Log.Comment( INFO, "record thread terminated ");
}

thread Record_thread( save_record_fcn );

void shutdown_thread( )
{
	RecordThreadDone 	= true;
	DaTA.Sem.Release( SEMAPHORE_RECORD );

	if ( Record_thread.joinable() )
		Record_thread.join();
	Log.Comment( INFO, "Record thread joined" );
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


void exit_proc( int signal )
{
	exit_intro( signal );


	shutdown_thread();

	if ( signal )
	{
		shutdown_stream();
		if ( frame )
			free( frame);
		exit(0);
	}
	if ( frame )
		free( frame);
	done = true;


}

void show_usage( void )
{
	string str;
	str = "Usage: " + App.Name + " -c #1 -r #2 -d #3 -o #4 \n";
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
	Log.Comment(INFO, App.Name + " is ready");

	while ( not done and rtapi.isStreamRunning() )
	{
	    this_thread::sleep_for(chrono::seconds(1));
	}
	Log.Comment(INFO, "Application loop exit");

}


void write_waveaudio()
{
	for( buffer_t n = 0; n < max_frames; n++ )
	{
		mono_out.Data[n] = shm_addr[n].left + shm_addr[n].right;
	}
	Wavedisplay.Write_wavedata();
}

void call_for_update()
{
	DaTA.ClearShm();
	shm_addr = DaTA.SetShm_addr( );

	for ( int n = 0; n < 4; ++n)
		DaTA.Sem.Release( SEMAPHORE_SENDDATA0 + n );
}

void record_start( )
{
	if ( SaveRecordFlag )
	{
		Log.Comment( WARN, "Audioserver is still saving data. ... Wait ");
		return;
	}
	External.status.record = true;
	rcounter = 0;
	ProgressBar.Set( &rcounter, recduration );
	RecTimer.Start();
	sds->StA_state[ MbIdExternal ].store = true;
	Log.Comment(INFO, "Audioserver starts recording" );
}

void record_stop()
{
	DaTA.Sem.Release( SEMAPHORE_RECORD );

	External.status.record = false;
	ProgressBar.Unset();
	uint t_el = RecTimer.Time_elapsed();
	sds->StA_state[ MbIdExternal ].store = false;
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
	if( mode == KEYBOARD )
		ncounter	= 0;

	buffer_t audioframes = sds->audioframes;
	if ( n > audioframes - 1 )
//	if ( n > frames_per_sec - 1 )
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

		if ( sds->Wavedisplay_Id == AUDIOOUT )
			write_waveaudio();
	}
}

key_struct_t AppKey;

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
			buffer[i]	= shm_addr[ncounter];
			ncounter 	= (ncounter	+ 1 );
		}

		if (sds->AudioServer == EXITSERVER )
		{
			exit_proc( 0 );
			return 1;
		}

		if (sds->AudioServer == RECORDSTART )
			record_start();

		if (sds->AudioServer == RECORDSTOP )
			record_stop();

		sds->AudioServer = RUNNING;

		return 0;
} // callback function



int main( int argc, char *argv[] )
{
	App.Start(argc, argv );

	Log.Set_Loglevel(DEBUG, false);
	Log.Show_loglevel();


	App.Shutdown_instance( );


	App.Sds->Announce(  );

	for ( int n = 0; n < 4; ++n)
	{
		DaTA.Sem.Reset( SEMAPHORE_SENDDATA0 + n );
	}
    DaTA.Sem.Release( SEMAPHORE_STARTED );
    DaTA.Sem.Reset( SEMAPHORE_RECORD );
	sds->RecCounter 	= 0;

	string wd_name = wavedisplay_struct().names[ AUDIOOUT ];
	Wavedisplay.Add_data_ptr( wd_name, mono_out.Data );
	Wavedisplay.SetDataPtr( AUDIOOUT , 0 );

//	wav_header.srate 				= Cfg->Config.rate;
//	wav_header.num_chans 			= Cfg->Config.channel;
//	wav_header.bytes_per_sec 		= wav_header.num_chans * wav_header.bytes_per_samp * wav_header.srate;

	// dynamic rtapi output parameter
	// Tell RtAudio to output all messages, even warnings.
	rtapi.showWarnings( true );
	//	frame = (double *) calloc( Cfg->Config.channel, sizeof( double ) );
	frame = (frame_t* ) calloc( Cfg->Config.channel, sizeof( frame_t ) );
	oParams.nChannels 		= Cfg->Config.channel;
	oParams.firstChannel 	= Cfg->Config.ch_offs;
	get_device_description( Cfg->Config.device );
	oParams.deviceId 		= DeviceDescription.Id;
	#define USE_INTERLEAVED
	#if !defined( USE_INTERLEAVED )
			options.flags |= RTAUDIO_NONINTERLEAVED,
	#endif




// end parametrization
// -------------------------------------------------------------------------------

	call_for_update();

	show_usage();

	// An error in the openStream() function can be detected either by
	// checking for a non-zero return value OR by a subsequent call to
	// isStreamOpen().

	Log.Comment(INFO, "Opening Audio Stream");
	if ( rtapi.openStream(
						&oParams,
						NULL,
						FORMAT,
						Cfg->Config.rate,
						&bufferFrames,
						&RtAudioOut,
						( void* )frame,
						&options ) )
	{
		Log.Comment( ERROR, rtapi.getErrorText() );
		exit_proc(6);
	}


	start_audio_stream();

	show_parameter();

	Log.Comment(INFO, "Application initialized");
	Log.Comment(INFO, "Entering Application loop\n");


	App.Ready();

	while ( not done  && rtapi.isStreamRunning() )
	{
		Timer.Wait( 1 );// 1sec
	}
	shutdown_stream();

	Log.Comment( INFO, "AudioServer reached target exit main()" );


}

