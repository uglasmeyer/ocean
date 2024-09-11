
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

void exit_proc( int exit_code )
{
	Log.Comment(INFO, Application + "received command <exit> " );
	Log.Comment( INFO, "Entering exit procedure for \n" + App.This_Application );
	Log.Comment( INFO, "suspend server" );

	done = true;

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
			Log.Comment(INFO, "stream is still open");
	}

	exit( 0 );
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

void get_mode()
{
	mode = sds->MODE;
}
void set_ncounter( buffer_t n )
{
	if ( n > max_frames - 1 )
	{
		ncounter = 0;

		if ( mode == SYNC )
		{
			mode 		= SENDDATA;
			shm_id 		= ( shm_id + 1 ) % 2;
			shm_addr 	= ( shm_id == 0 ) ? Shm_a.addr : Shm_b.addr;
			sds->MODE 	= mode;		// request new data for data buffers
			sds->SHMID 	= shm_id;
		}
	}
	// else unchanged
}
void set_shm_addr(  )
{
	// required mode to setup shm communication
	if (( mode == FREERUN ) or ( mode == KEYBOARD ))
	{
		shm_id 		= 0;
		sds->SHMID 	= shm_id;
		shm_addr 	= Shm_a.addr;
	}
	// else unchanged
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

	  get_mode();
	  set_ncounter( ncounter );
	  set_shm_addr();
	  if( mode == KEYBOARD )
	  	  ncounter	= 0;

	  // the output loop is implicitly protect against status changes by the fact
	  // that the buffer boundaries are aligned to 44100 respectively one second
	  for ( uint i{0}; i < chunksize; i++ )
	  {
	  	  buffer[i]	= shm_addr[ncounter];
	  	  ncounter 	= (ncounter	+ 1 );
	  }

	  if (sds->AudioServer == EXITSERVER )
	  {
		  *Done = true;
		  return 1;
	  }

	  sds->AudioServer = RUNNING;

	  return 0;
} // callback function



int main( int argc, char *argv[] )
{
	App.Start();

	Log.Set_Loglevel(DEBUG, false);
	Log.Show_loglevel();

	Log.Comment(INFO, "Catching signals SIGINT and SIGABRT");
	signal(SIGINT , &exit_proc );
	signal(SIGABRT, &exit_proc );
	signal(SIGHUP, &exit_proc );

	App.Shutdown_instance( );

    SDS.Announce( App.client_id, &sds->AudioServer );
	sds->RecCounter 	= 0;


	Log.Comment(INFO, "Evaluating startup arguments");
	prgarg_struct_t params = parse_argv( argc, argv );
//	wav_header.srate 				= params.rate;
//	wav_header.num_chans 			= params.channel;
//	wav_header.bytes_per_sec 		= wav_header.num_chans * wav_header.bytes_per_samp * wav_header.srate;

	// dynamic rtapi output parameter
	// Tell RtAudio to output all messages, even warnings.
	rtapi.showWarnings( true );
	double *frame = (double *) calloc( params.channel, sizeof( double ) );
	oParams.nChannels 		= params.channel;
	oParams.firstChannel 	= params.ch_offs;
	get_device_description( params.device );
	oParams.deviceId 		= DeviceDescription.Id;
	#define USE_INTERLEAVED
	#if !defined( USE_INTERLEAVED )
			options.flags |= RTAUDIO_NONINTERLEAVED,
	#endif




// end parametrization
// -------------------------------------------------------------------------------




	Log.Comment(INFO,"Attaching data buffers");
	Shm_a.buffer( sharedbuffer_size, params.shm_key_a );
	Shm_b.buffer( sharedbuffer_size, params.shm_key_b );
	shm_id		= 0;
	sds->SHMID 	= shm_id;
	shm_addr 	= Shm_a.addr;

//	shm_addr = (shm_id == 0 ) ? Shm_a.addr : Shm_b.addr;

	show_usage();



	// An error in the openStream() function can be detected either by
	// checking for a non-zero return value OR by a subsequent call to
	// isStreamOpen().


	Log.Comment(INFO, "Opening Audio Stream");
	if ( rtapi.openStream(&oParams,
						NULL,
						FORMAT,
						params.rate,
						&bufferFrames,
						&RtAudioOut,
						( void* )frame,
						&options ) )
	{
		Log.Comment( ERROR, rtapi.getErrorText() );
		exit_proc(6);
	}

	if ( not rtapi.isStreamOpen() )
	{
		Log.Comment( ERROR ,"Failed to open Audio Stream" );
		exit_proc(3);
	}
	else
		Log.Comment( INFO, "Audio Stream is open ") ;

	if ( rtapi.startStream() )
	{
		Log.Comment( ERROR , rtapi.getErrorText() );
		exit_proc(4);
	}
	else
		Log.Comment( INFO, "Audio Stream is started ") ;

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


	Log.Comment(INFO, "announcing Application");

	Log.Comment(INFO, "Application initialized");
	Log.Comment(INFO, "Entering Application loop\n");
	Log.Comment(INFO, App.Name + " is ready");

	while ( not *Done )
	{
		cout.flush() << "." ;
		Timer.Wait( 1 );// 1sec
	}
	free( frame );
	Log.Comment(INFO, "Application loop exit");
	exit_proc(0);

}

