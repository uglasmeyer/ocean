/*
 * Rtaudio.h
 *
 *  Created on: Sep 15, 2025
 *      Author: sirius
 */

#ifndef INCLUDE_AUDIO_H_
#define INCLUDE_AUDIO_H_

#define FORMAT RTAUDIO_SINT16 // 16bit signed integer

#include <RtAudio.h>
#include <Logfacility.h>
#include <data/Config.h>


class Audio_class
	: virtual 	Logfacility_class
	, virtual public	RtAudio
{
	string 				className = "";
	typedef struct device_struct
	{
		uint 	Index;
		uint 	Id;
		string 	Name;
	} device_struct_t;

	device_struct_t DeviceDescription;

public:
	RtAudio::StreamParameters 	oParams 	{};
	RtAudio::StreamOptions 		options 	= {	.flags 				= RTAUDIO_HOG_DEVICE,
												.numberOfBuffers 	= 2,
												.streamName 		= Application };
	uint						bufferFrames= chunksize;

						Audio_class			( Config_class* cfg );
	virtual 			~Audio_class		();
	void 				Show_parameter		();
	void 				Start_audio_stream	();
	void 				Shutdown_stream		();

private:
	void 				get_device_description( uint index );
	unsigned int 		selectDeviceIndex	( std::vector<std::string> deviceNames );
};

#endif /* INCLUDE_AUDIO_H_ */
