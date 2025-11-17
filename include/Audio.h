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
 * Rtaudio.h
 *
 *  Created on: Sep 15, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_AUDIO_H_
#define INCLUDE_AUDIO_H_


#include <data/DataWorld.h>
#include <RtAudio.h>


typedef 				stereo_t 			frame_t;

extern void 			errorCallback		( RtAudioErrorType, const std::string& txt );



/**************************************************
 * Audio_class
 *************************************************/
class Audio_class
	: virtual public	Logfacility_class
{
	string 				className 			= "";
	typedef struct 		device_struct
	{
		uint 	Index;
		uint 	Id;
		string 	Name;
	} device_struct_t;

	device_struct_t 	DeviceDescription;

	RtAudio::
	StreamParameters	streamParams 		{};
	RtAudio::
	StreamOptions		streamOptions 		= {	.flags 				= RTAUDIO_HOG_DEVICE,
												.numberOfBuffers 	= 2,
												.streamName 		= Application,
												.priority			= 0
											};
	uint				bufferFrames		= chunksize;
	uint				sample_rate			;
	frame_t*			frame				;
	RtAudio 			Rt					{ RtAudio::LINUX_PULSE, &errorCallback };
	vector<uint>		deviceIds 			;//Rt.getDeviceIds();
	vector<string> 		deviceNames 		;//Rt.getDeviceNames();

public:

	Audio_class( Config_class* cfg )
		: Logfacility_class("Audio_class")
	{
		className 				= Logfacility_class::className;
		this->sample_rate		= cfg->Config.rate;
		this->frame 			= ( frame_t* )calloc( cfg->Config.channel, sizeof( frame_t ) );
		deviceIds 				= Rt.getDeviceIds();
		deviceNames 			= Rt.getDeviceNames();

		Rt.showWarnings			( true );
		streamParams.nChannels 		= cfg->Config.channel;
		streamParams.firstChannel 	= cfg->Config.ch_offs;
		streamParams.deviceId 		= getDeviceDescription( cfg->Config.device );
/*
		#define USE_INTERLEAVED
		#if !defined( USE_INTERLEAVED )
				options.flags |= RTAUDIO_NONINTERLEAVED,
		#endif
*/
	};
	~Audio_class()
	{
		DESTRUCTOR( className );
	};

	void 				Start_stream		( RtAudioCallback Audio_out );
	void 				Shutdown_stream		();

private:
	void				open_stream			( RtAudioCallback Audio_out );

	void 				show_parameter		();
	const uint 			getDeviceDescription( uint index );
	unsigned int 		selectDeviceIndex	( vector<string> deviceNames );

};

#include <External.h>
/**************************************************
 * Record_class
 *************************************************/
class Record_class :
	virtual public 		Logfacility_class,
	virtual 			ProgressBar_class,
	virtual public		sdsstate_struct,
	virtual				Trigger_class
{
	string 				className	 		= "";
	Time_class			RecTimer			{};
	Trigger_class		Note_start			;
	Trigger_class		Note_end			;
	interface_t*		sds					;
	Semaphore_class*	Sem					;
	Appstate_class*		Appstate			;
	uint				rcounter			= 0; // # of recorded audioframes
	uint				record_sec			= 0;

public:
	External_class		External			;
	bool				recording			= false;

						Record_class		( Dataworld_class* data );
	virtual 			~Record_class		();
	bool 				Start				();
	void 				Stop				( bool init = false );
	bool				State				( StateId trigger );
	void				Store_audioframes	( Stereo_t* addr, buffer_t frames );
	void 				Set_rcounter		();

};

#include <Dynamic.h>
#include <data/Interface.h>

/**************************************************
 * AudioVolume_class
 *************************************************/
class AudioVolume_class :
	Logfacility_class
{
	string 				className			= "";
	interface_t* 		sds					;
	Dynamic_class 		DynVolume 			{ volidx_range };

public:

						AudioVolume_class	( interface_t* _sds);
	virtual				~AudioVolume_class	() = default;

	void 				Transform			( buffer_t frames, Stereo_t* src, stereo_t* dst );

private:
	const float 		dynamic_limit		( buffer_t frames, Stereo_t* src );
	void 				selfTest			();
	data_t 				Max 				= numeric_limits<data_t>::max();
};


#endif /* INCLUDE_AUDIO_H_ */
