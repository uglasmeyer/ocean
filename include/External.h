/*
 * External.h
 *
 *  Created on: Mar 29, 2024
 *      Author: sirius
 */

#ifndef EXTERNAL_H_
#define EXTERNAL_H_

#include <Ocean.h>
#include <Config.h>
#include <data/Interface.h>
#include <data/Memory.h>
#include <Progressbar.h>
#include <Version.h>
#include <Wav.h>



class 							External_class :
	public virtual 				Logfacility_class,
	public virtual 				Stereo_Memory<stereo_t>

{
	string						className 			= "";
	Storage_class* 				StA					= nullptr;
	interface_t*				sds					= nullptr;
	FILE*						File				= nullptr;
	Config_class*				Cfg 				= nullptr;

public:
	long 						Filedata_size 		= 0;

								// Synthesizer read only class
								External_class		( Storage_class* sta,
													Config_class* cfg );


								// Audioserver write only class
								External_class		( Config_class* cfg,
													interface_t* sds ) ;

	virtual 					~External_class		() = default;

	struct 						status_struct
	{
		bool record = false;
	};
	status_struct				status				= status_struct();

	bool 						Read_file_data		();
	bool 						Read_file_header	( string );
	int 						Save_record_data	( int filenr );
	void 						Mono2Stereo			( Data_t* mono, uint size );
	void 						Record_buffer		( Stereo_t* src, buffer_t frames );
	string 						GetName				();
	void 						Test_External		();

private:
	const uint 					MAXWAVFILES 		= 5; // max numbers of file names generated automatically
	long int 					read_position 		= 0;
	buffer_t					record_size			= 0;

	wav_struct_t 				header_struct;
	string 						Name 				= "";
	string 						Filename 			= "";
	string 						add_header 			= "cat " + file_structure().raw_file +
								  	  	  	  	  	  " >> " + file_structure().wav_file;
	void 						write_audio_data	( string );
	void 						write_music_file	( string );
	void 						wav_define 			(  long  );
	long 						write_wav_header	( string );
	long 						write_audio_data	( string, buffer_t );
	void 						setName				( string name );
	bool 						read_stereo_data	( long );
	string 						ffmpeg_cmd			( string wav, string mp3 );
	string 						id3tool_cmd			( string mp3 );

	void 						close				();

};



#endif /* EXTERNAL_H_ */
