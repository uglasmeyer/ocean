/*
 * External.h
 *
 *  Created on: Mar 29, 2024
 *      Author: sirius
 */

#ifndef EXTERNAL_H_
#define EXTERNAL_H_

#include <synthesizer.h>
#include <synthmem.h>
#include <wav.h>
#include <version.h>

extern void system_execute( const string );

class External_class : virtual public Logfacility_class
{
	Storage::Storage_class* 	StA;
	Stereo_Memory* 				stereo;
	FILE*						File;

public:

	External_class( Storage::Storage_class* Storge_Area, Stereo_Memory* stereo ) :
		Logfacility_class("External")
	{
		this->StA 		= Storge_Area;
		this->stereo	= stereo;
		this->File 		= NULL;
		this->stereo->Info	( "External Stereo data") ;
		this->StA->Info		( "Memory Array External");

	};
	~External_class(){};

	struct status_struct
	{
		bool record 	= false;
	} status;


	bool read_file_data(  );
	bool read_file_header( string );
	void save_record_data( int );
	void test();

private:
	wav_struct_t 	header_struct;
	string 			Name 		= ".wav";
	string 			Filename 	= "";
	string convert_wav2mp3 		= "ffmpeg -y -i " 	+ file_structure().wav_file +
								  " -f mp3 " 		+ file_structure().mp3_file + " >/dev/null" ;
	string add_header 			= "cat " + file_structure().raw_file +
								  " >> " + file_structure().wav_file;
	string Title				= " -t 'Experimental "+ Version_str +"' ";
	string Author				= " -r U.G.";
	string Genre				= " -G Alternative ";
	string Album				= " -a '" + Application +"' ";
	string insert_mp3_tags		= "id3tool " 	+
										Title 	+
										Author 	+
										Album 	+
										Genre 	+
										file_structure().mp3_file;

	void write_audio_data( string );
	void write_music_file( string );
	void wav_define (  long  );
	long write_wav_header( string );
	long write_audio_data( string, buffer_t );
	void SetName( string );
	long int read_position = 0;
	bool read_stereo_data( long );
	void close( );

};


#endif /* EXTERNAL_H_ */
