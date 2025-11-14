/*
 * External.h
 *
 *  Created on: Mar 29, 2024
 *      Author: sirius
 */

#ifndef EXTERNAL_H_
#define EXTERNAL_H_

#include <data/Config.h>
#include <Ocean.h>
#include <data/Interface.h>
#include <data/Memory.h>
#include <Progressbar.h>
#include <Wavedisplay.h>
//#include <Version.h>
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
	file_structure*				fs					= nullptr;
	Wavedisplay_class*			Wd					= nullptr;

public:
	buffer_t					Filedata_size 		= 0;
	struct 						status_struct
	{
		bool record = false;
	};
	status_struct				status				= status_struct();

								// Synthesizer read only class
								External_class		( Storage_class* sta,
													Config_class* cfg,
													Wavedisplay_class* wd );
	const bool 					Read_file_data		();
	const bool 					Read_file_header	( string );
	void 						Test_External		();


								// Audioserver write only class
								External_class		( Config_class* cfg,
													interface_t* sds ) ;
	const string 				Save_record_data	( int filenr );
	void 						Record_buffer		( Stereo_t* src, buffer_t frames );

	virtual 					~External_class		() = default;

private:
	long int 					read_position 		= 0;
	buffer_t					record_size			= 0;

	wav_struct_t 				header_struct;
	string 						Name 				= "";
	string 						Filename 			= "";
	string 						testcounter_file 	= "/tmp/counter";

	void 						write_audio_data	( string );
	void 						write_music_file	( string );
	void 						wav_define 			(  long  );
	long 						write_wav_header	( string );
	long 						write_audio_data	( string, buffer_t );
	void 						setName				( string name );
	bool 						read_stereo_data	( buffer_t );
	string						addheader_cmd		();
	string 						ffmpeg_cmd			( string wav, string mp3 );
	string 						id3tool_cmd			( string mp3 );
	int 						generate_file_no	();

	void 						close				();

};



#endif /* EXTERNAL_H_ */
