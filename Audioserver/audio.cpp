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

/*
 * audio.cpp
 *
 *  Created on: Sep 15, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Audio.h>


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
	this->className			= Logfacility_class::className;
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
bool Record_class::Start( )
{
	sds->Record_state 					= RECORDING;
	recording							= true;
	sds->StA_state_arr[ STA_EXTERNAL].store = true;
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
	recording							= false; // Audioser internal recording state
	sds->Record_state 					= INACTIVE; //public record state
	sds->StA_state_arr[ STA_EXTERNAL ].store= false;

	if ( not init )
	{
		uint sec_elapsed 					= RecTimer.Time_elapsed() /1000;
		Info( "Record duration: ", sec_elapsed, " sec");
	}
}
bool Record_class::State( StateId trigger )
{
	bool 		recordstate = ((sds->Record_state == trigger) or
								Note_start.get() or
								Note_end.get() );
	return 		recordstate;
}
void Record_class::Set_rcounter( )
{
	rcounter++;  // # of recorded audio_frames
	ProgressBar_class::Update();
	if( rcounter * audio_frames >= record_sec * frames_per_sec )
		Stop( false );
}
void Record_class::Store_audioframes( Stereo_t* addr, buffer_t frames )
{
	External.Record_buffer( addr, frames );
}



/**********************************************
 * Audio_class
 **********************************************/



void 	Audio_class::show_parameter()
{

// The parameter list is available after stream open
	Table_class	T{"Parameter in use" };
	T.AddColumn( "Parameter", 20 );
	T.AddColumn( "Value"	, 10 );
	T.PrintHeader();

	T.AddRow( "Buffer size"			, bufferFrames );
	T.AddRow( "Number of buffers"	, streamOptions.numberOfBuffers );
	T.AddRow( "Stream latency"		, Rt.getStreamLatency() );
	T.AddRow( "Stream sample rate"	, Rt.getStreamSampleRate() );
	T.AddRow( "Stream name"			, streamOptions.streamName );
	T.AddRow( "RtApi option flags"	, streamOptions.flags );
	T.AddRow( "Device Id"			, streamParams.deviceId );
	T.AddRow( "Device name"			, DeviceDescription.Name );
}

void Audio_class::Start_stream( RtAudioCallback Audio_out  )
{

	open_stream(  Audio_out );
	if ( not Rt.isStreamOpen() )
	{
		Exception( Rt.getErrorText() );
	}
	else
		Comment( INFO, "Audio Stream is open ") ;

	if ( Rt.startStream() )
	{
		Exception( Rt.getErrorText() );
	}
	else
		Comment( INFO, "Audio Stream is started ") ;
}

//#define FORMAT RTAUDIO_SINT16 // 16bit signed integer

void Audio_class::open_stream( RtAudioCallback Audio_out )
{

	// An error in the openStream() function can be detected either by
	// checking for a non-zero return value OR by a subsequent call to
	// isStreamOpen().

	Comment(INFO, "Opening Audio Stream");

	if ( Rt.openStream(	&streamParams,
						NULL,
						RTAUDIO_SINT16,	// 16bit signed integer
						sample_rate,
						&bufferFrames,
						Audio_out,
						( void* )frame,
						&streamOptions ) )
	{
		Exception( Rt.getErrorText() );
	}

	show_parameter();
}

void Audio_class::Shutdown_stream()
{
	if ( not Rt.isStreamRunning() )
	{
		Rt.stopStream();
	}

	if ( not Rt.isStreamOpen() )
	{
		Rt.closeStream();
	}

	if ( frame )
	{
		free( frame);
		Info( "frame buffer freed" );
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
		Comment( INFO,  "device Id: " + to_string( dev ) + " " + deviceNames[ ind++ ] );
//		ind++;
	}

	if ( index >= deviceIds.size() )
	{
		index = selectDeviceIndex( deviceNames );
	}

	if ( index == 0 )
	{
		DeviceDescription = {0, Rt.getDefaultOutputDevice(), "default output device" };
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
	: Logfacility_class( "AudioVolume_class" )
{
	className = Logfacility_class::className;
	this->sds = _sds;
	DynVolume.SetupVol( sds->Master_Amp,	FIXED ); //set start and master_volume
	selfTest();
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
		coutf << "Output " << max << " limited by ratio " << Ratio << endl;
		return Ratio;
	}
		return 1.0;
}

void AudioVolume_class::Transform( buffer_t frames, Stereo_t* src, stereo_t* dst )
{	// transform Stereo data to stereo data using
	// dynamic volume, balance and dynamic limitat calculation

	DynVolume.SetupVol( sds->Master_Amp, sds->vol_slidemode );

	float 		ratio 		= dynamic_limit( frames, src );
	float 		balanceL 	= ratio * ( 100.0 - sds->mixer_balance ) / 200.0;
	float 		balanceR	= ratio - balanceL;

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
