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
#include <Synthmem.h>
#include <Version.h>
#include <Wav.h>
#include <Record.h>
#include <Interface.h>



class External_class : virtual Logfacility_class, virtual Config_class, public ProgressBar_class
{
	Storage::Storage_class* 	StA;
	FILE*						File;

public:
	Stereo_Memory				stereo{ stereobuffer_size };

	External_class( Storage::Storage_class* sta, uint8_t* counter_p) : //, Stereo_Memory* stereo ) :
		Logfacility_class("External"),
		Config_class( "External"),
		ProgressBar_class( counter_p )
	{
		this->StA 			= sta;
		this->File 			= NULL;
		stereo.Info			( "External Stereo data") ;

	};
	virtual ~External_class(){};

	struct status_struct
	{
		bool record 	= false;
	} status;


	bool Read_file_data(  );
	bool Read_file_header( string );
	void Save_record_data( int );
	void Add_record( Memory*, Memory* );
	string GetName();
	void Test();
	long Filedata_size = 0;

private:

	long int read_position = 0;
	wav_struct_t 	header_struct;
	string 			Name 		= "";
	string 			Filename 	= "";
	string add_header 			= "cat " + file_structure().raw_file +
								  " >> " + file_structure().wav_file;
	void 	write_audio_data( string );
	void 	write_music_file( string );
	void 	wav_define (  long  );
	long 	write_wav_header( string );
	long 	write_audio_data( string, buffer_t );
	void 	setName( string );
	bool 	read_stereo_data( long );
	string 	ffmpeg_cmd( string wav, string mp3 );
	string 	id3tool_cmd( string mp3 );

	void close( );

};



#endif /* EXTERNAL_H_ */
