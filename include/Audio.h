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
 * Audio.h
 *
 *  Created on: Sep 15, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_AUDIO_H_
#define INCLUDE_AUDIO_H_


#include <data/DataWorld.h>
#include <External.h>
#include <RtAudio.h>


typedef 				stereo_t 			frame_t;




/**************************************************
 * Audio_class
 *************************************************/
class Audio_class
	: virtual public	Logfacility_class
{
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
	StreamOptions		streamOptions 		= {
											.flags 				= 0,//RTAUDIO_HOG_DEVICE,
											.numberOfBuffers 	= 2,
											.streamName 		= Application,
											.priority			= 0
											};
	uint				streamFrames		= chunksize;
	uint				sample_rate			;
	frame_t*			frame				;
	vector<uint>		deviceIds 			;
	vector<string> 		deviceNames 		;
	RtAudio				Rt					;

public:
	enum IO_DIRECTION_e : unsigned char
	{
		IN,
		OUT
	};
	IO_DIRECTION_e		direction			;

	Audio_class( Config_class* cfg, IO_DIRECTION_e dir );
	virtual ~Audio_class();

	void 				Start_stream		( RtAudioCallback Audio_out,
											void* userData = nullptr );
	void 				Shutdown_stream		();


private:
	void				open_stream			( RtAudioCallback AudioIO_fnc,
											void* userData = nullptr );

	void 				show_characteristic	();
	const uint 			getDeviceDescription( uint index );
	unsigned int 		selectDeviceIndex	( vector<string> deviceNames );

};

/**************************************************
 * Record_class
 *************************************************/
class Record_class :
	virtual 			ProgressBar_class,
	virtual public		sdsstate_struct,
	virtual				Trigger_class
{
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
	void				Store_audioframes	( stereo_t* addr, buffer_t frames );
	void 				Set_rcounter		();

private:
	void				set_recording		( StateId_t StateID, bool flag );
};

#include <Dynamic.h>
#include <data/Interface.h>

/**************************************************
 * AudioVolume_class
 *************************************************/
class AudioVolume_class :
	Logfacility_class
{
	interface_t* 		sds					;
	Dynamic_class 		DynVolume 			;
	data_t 				Max 				;

public:

						AudioVolume_class	( interface_t* _sds);
	virtual				~AudioVolume_class	() = default;

	void 				Transform			( buffer_t frames, Stereo_t* src, stereo_t* dst );

private:
	const float 		dynamic_limit		( buffer_t frames, Stereo_t* src );
	void 				selfTest			();
};



extern int	RtAudioIn		(	void*	outputBuffer,
								void* 	/*inputBuffer*/,
								uint 	nBufferFrames,
								double	streamTime,
								RtAudioStreamStatus status,
								void* /* userData */) ;
extern void	errorCallback	(	RtAudioErrorType,
								const string& txt );

/**************************************************
 * Capture_class
 *************************************************/
class Capture_class :
	virtual public Logfacility_class,
	virtual public Audio_class
{
	External_class* Extern	;

public:
	typedef struct InputData_struct
	{
		Data_t* 	staData			;
		buffer_t 	maxFrames		;
		buffer_t 	frameCounter	;
		bool		streamDone		;

		InputData_struct( External_class* ext )
		{
			staData 	= ext->StA_ext->Data;
			maxFrames	= ext->StA_ext->param.size;
			frameCounter= 0;
			streamDone	= false;
		}
		~InputData_struct() = default;
	} InputData_t;
	InputData_t		InputData		;

				Capture_class( External_class* ext );
	virtual 	~Capture_class();
	void 		Start		( StAId_e staid );
	void		Stop		( StAId_e staid );

private:
};

#endif /* INCLUDE_AUDIO_H_ */
