/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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

/*
 * audio.cpp
 *
 *  Created on: Sep 15, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Audio.h>
#include <Utilities.h>

/**************************************************
 * Record_class
 *************************************************/
Record_class::Record_class( Dataworld_class* data ) :
	Logfacility_class	("Record_class"),
	ProgressBar_class	( &data->sds_master->RecCounter ),
	Note_start			( "Notes start", &data->sds_master->Note_start ),
	Note_end			( "Notes end  ", &data->sds_master->Note_end ),
	External			( data->Cfg_p, data->sds_master )
{
	this->sds				= data->sds_master;
	this->sds->RecCounter 	= 0;
	this->sds->Record_state	= INACTIVE;

	this->Sem				= data->Sem_p;
	this->Appstate			= &data->Appstate;
	this->record_sec		= data->Cfg_p->Config.record_sec;
	Stop( true ); // init
	Sem->Reset( SEMAPHORE_RECORD );

};
Record_class::~Record_class()
{
	DESTRUCTOR( className );
};

void Record_class::set_recording( StateId_t StateId, bool flag )
{
	sds->Record_state 						= StateId;
	sds->StA_state_arr[ STA_EXTERNAL ].store = flag; //public record state
	recording								= flag; // Audioser internal recording state

};
bool Record_class::Start( )
{
	set_recording( RECORDING, true );
	rcounter 							= 0;
	ProgressBar_class::Set				( &rcounter, record_sec * 1000/min_msec );

	RecTimer.Start						();
	Comment(INFO, "Audioserver starts recording" );
	return true;
}

void Record_class::Stop( bool init )
{
	if ( not init )
		Sem->Release( SEMAPHORE_RECORD ); // save recording to file

	ProgressBar_class::Unset();
	set_recording( INACTIVE , false );

	if ( not init )
	{
		uint sec_elapsed 					= RecTimer.Time_elapsed() /1000;
		Info( "Record duration: ", sec_elapsed, " sec");
	}
}
bool Record_class::State( StateId trigger )
{
	return ( (sds->Record_state == trigger)	or
			Note_start.Get() 				or
			Note_end.Get() 					);
}
void Record_class::Set_rcounter( )
{
	rcounter++;  // # of recorded audio_frames
	ProgressBar_class::Update();
	if( rcounter * audio_frames >= record_sec * frames_per_sec )
		Stop( false );
}
void Record_class::Store_audioframes( stereo_t* addr, buffer_t frames )
{
	External.Record_buffer( addr, frames );
}


/**************************************************
 * Capture_class
 *************************************************/

typedef Capture_class::InputData_struct	InputData_t;
int RtAudioIn( 	void*	/*outputBuffer*/,
				void* 	inputBuffer,
				uint 	nBufferFrames,
				double	streamTime,
				RtAudioStreamStatus status,
				void* 	classdata )
{
	InputData_t*	capture 	= ( InputData_t* ) classdata;
	frame_t* 		buffer 		= ( frame_t* ) inputBuffer ;

	for( buffer_t n = 0; n < nBufferFrames; n++ )
	{
		frame_t stereo 			= buffer[n];
		capture->staData[capture->frameCounter] = Data_t( stereo.left + stereo.right );
		capture->frameCounter 	+= 1;
	}

	if ( capture->frameCounter >= capture->maxFrames )
		return 2;	// abort
	if ( capture->streamDone )
		return 1;	// stop stream
	return 0;		// contiunue
}

Capture_class::Capture_class( External_class* ext ) :
	Logfacility_class	( "Capture_class" ),
	Audio_class			( ext->Cfg, IN ),
	InputData			( ext )
{
	this->Extern		= ext;

};
Capture_class::~Capture_class()
{
	DESTRUCTOR( className );
};

void Capture_class::Start( StAId_e staid )
{
	InputData.streamDone= false;
	InputData.frameCounter	= *Extern->StA_ext->Store_counter() * audio_frames;
	Start_stream		( &RtAudioIn, (void*) &InputData );
}

void Capture_class::Stop( StAId_e staid )
{
	InputData.streamDone= true;
	Shutdown_stream		();
	Extern->StA_ext->Set_records( InputData.frameCounter/audio_frames );
}


/**********************************************
 * Audio_class
 **********************************************/

extern Logfacility_class Log;
void errorCallback( RtAudioErrorType err, const std::string& txt )
{
	Log.Comment(ERROR, txt ) ;
}

Audio_class::Audio_class( Config_class* cfg, IO_DIRECTION_e dir )
	: Logfacility_class		( "Audio_class" )
	, Rt					(  )
{
	this->direction			= dir;
	this->sample_rate		= cfg->Config.rate;
	deviceIds 				= Rt.getDeviceIds();
	deviceNames 			= Rt.getDeviceNames();

	Rt.showWarnings			( true );
	streamParams.nChannels 	= cfg->Config.channel;
	streamParams.firstChannel= cfg->Config.ch_offs;
	streamParams.deviceId 	= getDeviceDescription( cfg->Config.device );
	show_characteristic		();
/*
	#define USE_INTERLEAVED
	#if !defined( USE_INTERLEAVED )
			options.flags |= RTAUDIO_NONINTERLEAVED,
	#endif
*/
};
Audio_class::~Audio_class()
{
	Shutdown_stream();
	Info( "frame buffer deleted" );

	DESTRUCTOR( className );
};

void Audio_class::show_characteristic()
{

// The parameter list is available after stream open
	Table_class	T{"Characteristic" };
	T.AddColumn( "Parameter", 20 );
	T.AddColumn( "Value"	, 10 );
	T.PrintHeader();


	T.AddRow( "Buffer size"			, streamFrames );
	T.AddRow( "Number of buffers"	, streamOptions.numberOfBuffers );
	T.AddRow( "Stream latency"		, Rt.getStreamLatency() );
	T.AddRow( "Stream sample rate"	, Rt.getStreamSampleRate() );
	T.AddRow( "Stream name"			, streamOptions.streamName );
	T.AddRow( "RtApi option flags"	, streamOptions.flags );
	T.AddRow( "Device Id"			, streamParams.deviceId );
	T.AddRow( "Device name"			, DeviceDescription.Name );

	RtAudio::DeviceInfo Info = Rt.getDeviceInfo( streamParams.deviceId );
    T.AddRow( "Max Output channels", Info.outputChannels );
    T.AddRow( "Max input channels", Info.inputChannels );
    T.AddRow( "Max duplex channels", Info.duplexChannels );
    T.AddRow( "Is default output", yesno( Info.isDefaultOutput ) );
    T.AddRow( "Is default input", yesno( Info.isDefaultInput ) );
    T.AddRow( "Sample rates", show_items( Info.sampleRates ) );

}


void Audio_class::Start_stream( RtAudioCallback Audio_fnc, void* userData )
{
	open_stream( Audio_fnc, userData );

	Rt.startStream();

	if ( not Rt.isStreamRunning() )
	{
		Exception( Rt.getErrorText() );
	}
	else
		Comment( INFO, "Audio Stream is started ") ;
}

void Audio_class::open_stream( RtAudioCallback AudioIO_fnc, void* userData )
{

	// An error in the openStream() function can be detected either by
	// checking for a non-zero return value OR by a subsequent call to
	// isStreamOpen().

	RtAudioErrorType err = RTAUDIO_UNKNOWN_ERROR;
	if ( direction == IN )
	{
		if( not Rt.isStreamOpen() )
			err = Rt.openStream(	NULL,
									&streamParams,
									RTAUDIO_SINT16,	// 16bit signed integer
									sample_rate,
									&streamFrames,
									AudioIO_fnc,
									userData
	//								, &streamOptions
									) ;
	}
	else
	{
		if( not Rt.isStreamOpen() )
			err = Rt.openStream(	&streamParams,
									NULL,
									RTAUDIO_SINT16,	// 16bit signed integer
									sample_rate,
									&streamFrames,
									AudioIO_fnc,
									NULL,//( void* )frame,
									&streamOptions ) ;
	}
	show_characteristic();

	if ( not Rt.isStreamOpen() )
	{
		Exception( to_string( err ) + Rt.getErrorText() );
	}
}

void Audio_class::Shutdown_stream()
{
	if ( Rt.isStreamRunning() )
	{
		Rt.stopStream();
	}

	if ( Rt.isStreamOpen() )
	{
		Rt.closeStream();
	}
}

uint Audio_class::selectDeviceIndex( vector<string> deviceNames )
{
	unsigned int 	device_index;
	string 			keyHit;
	cout << 		'\n';

	for ( device_index = 0; device_index < deviceNames.size(); device_index++ )
		cout << "  Device #" << device_index << ": " << deviceNames[device_index] << '\n';

	do
	{
		cout << "\nChoose a device #: ";
		cin >> device_index;
	} while ( device_index >= deviceNames.size() );
	getline( cin, keyHit );  // used to clear out stdin

	return device_index;
}

const uint Audio_class::getDeviceDescription( uint index )
{

	if ( deviceIds.size() < 1 )
	{
		Exception( "No audio devices found!" );
	}

	Comment( INFO,  "Available audio devices :");
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
		if( direction == IN )
			DeviceDescription = {0, Rt.getDefaultInputDevice(), deviceNames[ 0 ] };
		else
			DeviceDescription = {0, Rt.getDefaultOutputDevice(), deviceNames[ 0 ] };
	}
	else
	{
		DeviceDescription = {index, deviceIds[ index ], deviceNames[ index ] };
	}
	return DeviceDescription.Id;
}


/**********************************************
 * AudioVolume_class
 **********************************************/
AudioVolume_class::AudioVolume_class( interface_t* _sds)
	: Logfacility_class	( "AudioVolume_class" )
	, DynVolume 		( volidx_range )

{
	this->sds 			= _sds;
	this->Max			= numeric_limits< data_t >::max();
	DynVolume.SetupVol	( sds->Master_Amp,	FIXED );
	selfTest			();
};

void AudioVolume_class::selfTest()
{
	Info( "selfTest" );

	Stereo_t null = {0,0};
	array<Stereo_t, 5> src_arr { null };
	for( uint n = 0; n<5; n++ )
		src_arr[n] = {-32000,0};
	Stereo_t* src = &src_arr[0];
	float ratio = dynamic_limit( 5, src );
	Assert_equal( ratio, 1.0f , " " );

	src_arr[1] = { -2.0f*(float)Max, 0.0f };
	ratio = dynamic_limit( 5, src );
	float f = 32767.0/2/Max;
	Assert_equal( ratio, f );

	src_arr[2] = { -2.0f, 0.0f };
	ratio = dynamic_limit( 5, src );
	f = 32767.0/2/Max;
	Assert_equal( ratio, f );

	float 		balanceR	= 0.5 * ( 60 ) / 200.0;
	float 		balanceL 	= 0.5 *0.5 - balanceR;
	Assert_equal( fcomp( balanceR/balanceL, (float)6/4 ), true , "balance ratio" );
	sds->overmodulated = false;
}
const float AudioVolume_class::dynamic_limit( buffer_t frames, Stereo_t* src )
{
	float 	max = (float)Max;
	for( buffer_t n = 0; n < frames; n++ )
	{
		if ( abs( src[n].left  ) > max ) max = abs( src[n].left  );
		if ( abs( src[n].right ) > max ) max = abs( src[n].right );
	}
	float ratio = max / abs(Max) ;//ratio = 0.0;
	if ( ratio > 1.0 )
	{
		float Ratio = 1.0 / ratio;
		sds->overmodulated = true;
		Comment( WARN, "Output", max, "limited by ratio", Ratio );
		return Ratio;
	}
	else
	{
		return 1.0;
	}
}
void AudioVolume_class::Transform( buffer_t frames, Stereo_t* src, stereo_t* dst )
{	// transform Stereo data to stereo data using
	// dynamic volume, balance and dynamic limit calculation

	DynVolume.SetupVol( sds->Master_Amp, sds->vol_slidemode );

	float 		ratio 		= dynamic_limit( frames, src );
	float 		balanceR	= ratio * ( 100 + sds->mixer_balance ) / 200.0;
	float 		balanceL 	= ratio -  balanceR;

	DynVolume.SetDelta( sds->slide_duration );
	for( buffer_t n = 0; n < frames ; n++ )
	{
		float
		vol_percent 	= DynVolume.Get();
		dst[n].left 	= rint( src[n].left		* vol_percent * balanceL  ) ;
		dst[n].right 	= rint( src[n].right	* vol_percent * balanceR  ) ;
	}
	DynVolume.Update();

}

