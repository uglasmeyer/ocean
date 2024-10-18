
#include <Audioserver.h>

RtAudio::StreamParameters 	oParams;
RtAudio::StreamOptions 		options = {	.flags = RTAUDIO_HOG_DEVICE,
										.numberOfBuffers = 8,
										.streamName = Application };

void errorCallback( RtAudioErrorType /*type*/, const std::string &errorText )
{
	Log.Comment(ERROR, errorText ) ;
}



RtAudio rtapi( RtAudio::LINUX_PULSE, &errorCallback );

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

void shutdown_stream()
{
	if ( rtapi.isStreamRunning() )
	{
		Log.Comment(INFO,"stream will be stopped on exit");
		try
		{
			rtapi.stopStream();  // or could call dac.abortStream();
		}
		catch (bad_alloc const& ex) { cout << "bad alloc" << endl;}
	}
	if ( rtapi.isStreamOpen() )
	{
		Log.Comment(INFO, "stream will be closed on exit");
		rtapi.closeStream();
		if ( rtapi.isStreamOpen() )
			Log.Comment( ERROR, "stream is still open");
	}

}


bool 					SaveRecordFlag 		= false;
bool 					RecordThreadDone 	= false;
void record_thead_fcn()
{
	Log.Comment( INFO, "record thread started ");
//    DaTA.Sem.Reset(SEMAPHORE_RECORD);

	Value Fileno {0};
	while ( true )
	{
//		DaTA.Sem.Lock( SEMAPHORE_RECORD );

		if ( RecordThreadDone ) break;
	    Timer.Wait(1);
	    if ( SaveRecordFlag )
	    {
//		SaveRecordFlag = true;

		Fileno = (int) DaTA.Sds.addr->FileNo;
		Log.Comment( INFO, "record thread received job " + Fileno.str);

		External.Save_record_data( Fileno.i );
		Log.Comment( INFO, "preparing the next record session");
			// clean up
		DaTA.Sds.Update( RECORDWAVFILEFLAG );
//		DaTA.Sem.Release( SEMAPHORE_RECORD );	// if some process waits for completion
											// it will be released hereby
	    }
		SaveRecordFlag = false;
	}

	Log.Comment( INFO, "record thread terminated ");
}
thread Record_thread( record_thead_fcn );

void shutdown_thread( )
{
	RecordThreadDone 	= true;
//    DaTA.Sem.Release( SEMAPHORE_RECORD );

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
		Log.Comment( WARN, "Synthesizer reached target exit " + to_string( signal ));
		exit( signal );
	}
	sig_counter++;

	Log.Comment( INFO, Application + "received command <exit> " );
	Log.Comment( INFO, "Entering exit procedure for " + App.This_Application );
	Log.Comment( INFO, "suspend server" );
}


double* frame = nullptr;
void exit_proc( int signal )
{
	exit_intro( signal );

	done = true;
//	DaTA.Sem.Release( AUDIOSERVER_SEM );

	shutdown_stream();
	shutdown_thread();
	if ( frame )
		free( frame);

	Log.Comment(INFO, "Audioserver reached target exit 0" );

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

void call_for_update()
{
	DaTA.ClearShm();

	shm_addr = DaTA.SetShm_addr( );

	DaTA.Sem.Release( SEMAPHORE_SENDDATA0 );
	DaTA.Sem.Release( SEMAPHORE_SENDDATA1 );
	DaTA.Sem.Release( SEMAPHORE_SENDDATA2 );
	DaTA.Sem.Release( SEMAPHORE_SENDDATA3 );
}

buffer_t rcounter = 0;
void set_ncounter( buffer_t n )
{
	if( mode == KEYBOARD )
			ncounter	= 0;

	if ( n > max_frames - 1 )
	{
		ncounter = 0;
		if ( External.status.record )
		{
			External.Record_buffer( shm_addr, External.stereo.stereo_data, rcounter * max_frames);
			sds->RecCounter = rcounter;
			rcounter++;
			if( rcounter >= recduration )
				sds->AudioServer = RECORDSTOP;
		}

		call_for_update();
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
		for ( uint i{0}; i < chunksize; i++ )
		{
			buffer[i]	= shm_addr[ncounter];
			ncounter 	= (ncounter	+ 1 );
		}

		if (sds->AudioServer == EXITSERVER )
		{
			done = true;
//			exit_proc( 1 );
			DaTA.Sem.Release( AUDIOSERVER_SEM );

			return 1;
		}
		if (sds->AudioServer == RECORDSTART )
		{
			if ( SaveRecordFlag )
			{
				Log.Comment( WARN, "Audioserver is saving data. ... Wait ");
			}
			else
			{
				ncounter = 0;
				sds->Record = true; // display record status in ui
				rcounter = 0;
				Log.Comment(INFO, "Audioserver starts recording" );
				External.status.record = true;
			}
		}
		if (sds->AudioServer == RECORDSTOP )
		{
			External.status.record = false;
			sds->Record = false;
			SaveRecordFlag = true;
//			DaTA.Sem.Release( SEMAPHORE_RECORD );
		}

		sds->AudioServer = RUNNING;

		return 0;
} // callback function



int main( int argc, char *argv[] )
{
	catch_signals( &exit_proc, { SIGHUP, SIGINT, SIGABRT } );
	App.Start(argc, argv );

	Log.Set_Loglevel(DEBUG, false);
	Log.Show_loglevel();


	App.Shutdown_instance( );


	App.Sds->Announce(  );
	App.sds->Record = false;

    DaTA.Sem.Release( SEMAPHORE_STARTED );
    DaTA.Sem.Reset(AUDIOSERVER_SEM);
	sds->RecCounter 	= 0;


//	wav_header.srate 				= Cfg->Config.rate;
//	wav_header.num_chans 			= Cfg->Config.channel;
//	wav_header.bytes_per_sec 		= wav_header.num_chans * wav_header.bytes_per_samp * wav_header.srate;

	// dynamic rtapi output parameter
	// Tell RtAudio to output all messages, even warnings.
	rtapi.showWarnings( true );
	frame = (double *) calloc( Cfg->Config.channel, sizeof( double ) );
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
	if ( rtapi.openStream(&oParams,
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



	Log.Comment(INFO, "Application initialized");
	Log.Comment(INFO, "Entering Application loop\n");


	App.Ready();

//	DaTA.Sem.Lock( AUDIOSERVER_SEM );

	while ( not done )
	{
//		cout.flush() << "." ;
		Timer.Wait( 1 );// 1sec
	}
	exit_proc( 0 );
	return 0;
//	raise( SIGHUP);

}

