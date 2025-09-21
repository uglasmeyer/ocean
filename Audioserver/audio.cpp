/*
 * audio.cpp
 *
 *  Created on: Sep 15, 2025
 *      Author: sirius
 */

#include <Audio.h>

void errorCallback( RtAudioErrorType, const std::string& txt )
{
	//Log.Comment(ERROR, errorText ) ;
	coutf << txt << endl;
	errno =  0;
}

Audio_class::Audio_class( Config_class* cfg ) :
	Logfacility_class("Audio_class"),
	RtAudio(RtAudio::LINUX_PULSE, &errorCallback)
{
	className 				= Logfacility_class::className;
	// dynamic rtapi output parameter
	// Tell RtAudio to output all messages, even warnings.
	showWarnings			( true );
	oParams.nChannels 		= cfg->Config.channel;
	oParams.firstChannel 	= cfg->Config.ch_offs;
	get_device_description	( cfg->Config.device );
	oParams.deviceId 		= DeviceDescription.Id;
	#define USE_INTERLEAVED
	#if !defined( USE_INTERLEAVED )
			options.flags |= RTAUDIO_NONINTERLEAVED,
	#endif

};
Audio_class::~Audio_class()
{
	DESTRUCTOR( className );
};

void 	Audio_class::Show_parameter()
{
// The parameter list is available after stream open
Comment(INFO , 	"Parameters in use:\n       buffer size        " +
												to_string( bufferFrames ) + "\n" +
					"       number of buffers  " + to_string( options.numberOfBuffers ) + "\n" +
					"       Stream latency     " + to_string( getStreamLatency()) + "\n" +
					"       Stream sample rate " + to_string( getStreamSampleRate()) + "\n" +
					"       Stream name        " + options.streamName + "\n" +
					"       RtApi option flags " + to_string( options.flags )+ "\n" +
					"       Device Id          " + to_string( oParams.deviceId ) + "\n" +
					"       Device name        " + DeviceDescription.Name  );
}

void Audio_class::Start_audio_stream()
{
	if ( not isStreamOpen() )
	{
		EXCEPTION( "Failed to open Audio Stream" );
	}
	else
		Comment( INFO, "Audio Stream is open ") ;

	if ( startStream() )
	{
		EXCEPTION( getErrorText() );
	}
	else
		Comment( INFO, "Audio Stream is started ") ;
}

void Audio_class::Shutdown_stream()
{
	if ( not isStreamRunning() )
	{
		stopStream();
		Info( "audio stream was running", getErrorText() );
	}
	if ( not isStreamOpen() )
	{
		closeStream();
		Info( "audio stream was open ", getErrorText() );
	}
}

unsigned int Audio_class::selectDeviceIndex( std::vector<std::string> deviceNames )
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


void Audio_class::get_device_description( uint index )
{
	std::vector<unsigned int> 	deviceIds 	= getDeviceIds();
	std::vector<string> 		deviceNames = getDeviceNames();

	if ( deviceIds.size() < 1 )
	{
		Comment( ERROR, "No audio devices found!" );
		exit_proc( 5 );
	}

	Comment( INFO,  "Available  audio devices :");
	int ind = 0;
	for ( uint dev : deviceIds )
	{
		Comment( INFO,  "device Id: " + to_string( dev ) + " " + deviceNames[ ind ] );
		ind++;
	}

	if ( index >= deviceIds.size() )
	{
		index = selectDeviceIndex( deviceNames );
	}

	if ( index == 0 )
	{
		DeviceDescription = {0, getDefaultOutputDevice(), "default output device" };
	}
	else
	{
		DeviceDescription = {index, deviceIds[ index ], deviceNames[ index ] };
	}
}



