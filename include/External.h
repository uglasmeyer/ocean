/*
 * External.h
 *
 *  Created on: Mar 29, 2024
 *      Author: sirius
 */

#ifndef EXTERNAL_H_
#define EXTERNAL_H_

#include <synthmem.h>
#include <Synthesizer.h>
#include <wav.h>
#include <version.h>


class External_class : virtual public Logfacility_class
{
	Storage::Storage_class* 	StA;
	FILE*						File;
	string 						insert_mp3_tags;
public:
	Stereo_Memory				stereo{ stereobuffer_size };

	External_class( Storage::Storage_class* StA ) : //, Stereo_Memory* stereo ) :
		Logfacility_class("External")
	{
		this->StA 			= StA;
		this->File 			= NULL;
		stereo.Info			( "External Stereo data") ;
		id3tool_cmd(	"Experimental " + Version_str,
						"U.G.",
						"Alternative",
						Application );

	};
	~External_class(){};

	struct status_struct
	{
		bool record 	= false;
	} status;

	void id3tool_cmd( string t, string r, string G, string a);
	bool read_file_data(  );
	bool read_file_header( string );
	void save_record_data( int );
	void add_record( Memory*, Memory* );
	void test();

private:
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
	long int read_position = 0;

};


#endif /* EXTERNAL_H_ */
