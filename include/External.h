/*
 * External.h
 *
 *  Created on: Mar 29, 2024
 *      Author: sirius
 */

#ifndef EXTERNAL_H_
#define EXTERNAL_H_

#include <Synthesizer.h>
#include <Config.h>
#include <Synthmem.h>
#include <Version.h>
#include <Wav.h>


class External_class : virtual Logfacility_class, virtual Config_class
{
	Storage::Storage_class* 	StA;
	FILE*						File;

public:
	Stereo_Memory				stereo{ stereobuffer_size };

	External_class( Storage::Storage_class* StA ) : //, Stereo_Memory* stereo ) :
		Logfacility_class("External"),
		Config_class( "External")
	{
		this->StA 			= StA;
		this->File 			= NULL;
		stereo.Info			( "External Stereo data") ;

	};
	virtual ~External_class(){};

	struct status_struct
	{
		bool record 	= false;
	} status;

	string Id3tool_cmd( );
	bool Read_file_data(  );
	bool Read_file_header( string );
	void Save_record_data( int );
	void Add_record( Memory*, Memory* );
	void test();
	long Filedata_size = 0;

private:

	long int read_position = 0;
	wav_struct_t 	header_struct;
	string 			Name 		= "";
	string 			Filename 	= "";
	string convert_wav2mp3 		= "ffmpeg -y -i " 	+ file_structure().wav_file +
								  " -f mp3 " 		+ file_structure().mp3_file +
								  " >/dev/null" ;
	string add_header 			= "cat " + file_structure().raw_file +
								  " >> " + file_structure().wav_file;
	void write_audio_data( string );
	void write_music_file( string );
	void wav_define (  long  );
	long write_wav_header( string );
	long write_audio_data( string, buffer_t );
	void setName( string );
	bool read_stereo_data( long );
	void close( );

};


#endif /* EXTERNAL_H_ */
