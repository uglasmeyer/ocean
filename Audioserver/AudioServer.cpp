

#include <RtAudio.h>
// Platform-dependent sleep routines.
#if defined( WIN32 )
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif
#define RTAUDIO


#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>

#include <synthesizer.h>
#include <synthmem.h>
#include <Interface.h>
#include <common.h>
#include <Logfacility.h>
#include <version.h>

// classes and structures

Logfacility_class 		Log("AudioServer");
Shared_Memory 			Shm_a;
Shared_Memory			Shm_b;
GUI_interface_class 	GUI ;


string This_Application 	= "";

// runtime parameter
buffer_t 		ncounter 		= 0;
int 			shm_id 			= 0; // out_data = Shm_a
stereo_t*		shm_addr 		= Shm_a.addr;
uint 			bufferFrames 	= chunksize;
int 			record			= 0;
int 			ERROR 			= Log.ERROR;
int 			DEBUG 			= Log.DEBUG;
int 			INFO  			= Log.INFO;
bool 			done 			= false;
bool*			Done			= &done;
// ----------------------------------------------------------------
// RT Audio constant declarations

typedef struct device_struct
{
	uint 	Index;
	uint 	Id;
	string 	Name;
} device_struct_t;
device_struct_t DeviceDescription;

RtAudio::StreamParameters 	oParams;
RtAudio::StreamOptions 		options = {	.flags = RTAUDIO_HOG_DEVICE,
										.numberOfBuffers = 8,
										.streamName = Application };



#define FORMAT RTAUDIO_SINT16 // 16bit signed integer


// end declaration section
// --------------------------------------------------------------------------------------------

void errorCallback( RtAudioErrorType /*type*/, const std::string &errorText )
{
	Log.Comment(ERROR, errorText ) ;
}

RtAudio rtapi( RtAudio::LINUX_PULSE, &errorCallback );

void exit_proc( int exit_code )
{
	Log.Comment( Log.DEBUG, "Entering exit procedure for \n" + This_Application );

	Log.Comment( Log.INFO, "suspend server" );
	GUI.announce( "AudioServer", false );

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
	}

	exit( 1 );
}

void show_usage( void )
{
	string str;
	str = "Usage: AudioServer -c #1 -r #2 -d #3 -o #4 \n";
	str.append("       where \n");
	str.append("       #1 = number of channels (default=2),\n");
	str.append("       #2 = the sample rate (default = 44100),\n");
	str.append("       #3 = device index (default = 0, default device),\n");
	str.append("       #4 = channelOffset on the device (default = 0) ");
	Log.Comment( Log.INFO , str );
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
	for ( auto dev : deviceIds )
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
	Log.Comment(INFO, "Entering Application loop");

	while ( not done and rtapi.isStreamRunning() )
	{
		SLEEP( SECOND );
	}
	Log.Comment(INFO, "Application loop exit");

}
buffer_t set_ncounter( buffer_t n )
{
	if ( ncounter > max_frames - 1 ) //TODO check max_frames
	{
		if ( GUI.addr->MODE == FREERUN )
			return 0;
		// request new data for data buffers
		if ( shm_id == 0 )
		{
			shm_id = 1; 			// get b
			shm_addr = Shm_a.addr; 	// out a
		}
		else
		{
			shm_id = 0;				// get a
			shm_addr = Shm_b.addr; 	// out b
		}
		GUI.addr->SHMID = shm_id;
		GUI.addr->MODE = SENDDATA;
		return 0;
	}
	else
	{
		return n;
	}
}

int RtAudioOut(	void *outputBuffer,
				void * /*inputBuffer*/,
				unsigned int nBufferFrames,
				double streamTime,
				RtAudioStreamStatus status,
				void *data )
{
	  stereo_t* buffer { (stereo_t* ) outputBuffer };

	  // keep all variable data initializations outside of RtAudioOut
	  if ( status != 0 ) // buffer overflow
		  Log.Comment( Log.WARN, "Buffer status differs: " + to_string( status ));

	  ncounter = set_ncounter( ncounter );
	  if( GUI.addr->MODE == FREERUN )
		  shm_addr = Shm_a.addr; // set the default

	  // the output loop is implicitly protect against status changes by the fact
	  // that the buffer boundaries are aligned to 44100 respectively one second
	  for ( uint i{0}; i < chunksize; i++ )
	  {
	  	  buffer[i]	= shm_addr[ncounter];
	  	  ncounter 	= (ncounter	+ 1 );
	  }

	  if (GUI.addr->AudioServer == STOPSNDSRV )
	  {
		  Log.Comment(Log.INFO, Application + "received command <exit> " );
		  GUI.addr->UpdateFlag = true;
		  exit_proc(1); // exit on used request
	  }
	  else
	  {
		  GUI.addr->AudioServer = RUNSNDSRV;
	  }
	  return 0;
} // callback function



int main( int argc, char *argv[] )
{

	This_Application = Get_application_name ( "Audio Server" );
	Log.Comment( INFO, "Entering application init for " );
	Log.Comment( INFO, This_Application );

	Log.Set_Loglevel(Log.DEBUG, false);
	Log.Show_loglevel();

	Log.Comment(INFO, "Catching signals SIGINT and SIGABRT");
	signal(SIGINT , &exit_proc );
	signal(SIGABRT, &exit_proc );

	GUI.announce( "AudioServer", true );
	Log.Comment(Log.INFO, This_Application );
	GUI.addr->AudioServer	= STOPSNDSRV;
	Wait(1*SECOND);
	GUI.addr->RecCounter 			= 0;
	GUI.addr->MODE 			= SENDDATA;
	GUI.addr->UpdateFlag 	= true;
	GUI.addr->AudioServer 	= RUNSNDSRV;


	Log.Comment(INFO, "Evaluating startup arguments");
	prgarg_struct_t params = parse_argv( argc, argv );
//	wav_header.srate 				= params.rate;
//	wav_header.num_chans 			= params.channel;
//	wav_header.bytes_per_sec 		= wav_header.num_chans * wav_header.bytes_per_samp * wav_header.srate;

	// dynamic rtapi output parameter
	// Tell RtAudio to output all messages, even warnings.
	rtapi.showWarnings( true );
	double *data = (double *) calloc( params.channel, sizeof( double ) );
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
	shm_addr = Shm_a.addr; // default output address

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
						(void *)data,
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
	Log.Comment(INFO, "Entering Application loop");
	while ( not *Done )
//	while ( ( not done ) and rtapi.isStreamRunning() )
	{
		cout.flush() << "." << endl;
		SLEEP( SECOND );
	}
	Log.Comment(INFO, "Application loop exit");
	exit(0);
//	Application_loop();
}

