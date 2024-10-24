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



class External_class : virtual Logfacility_class//, public ProgressBar_class
{
	Storage_class* 	StA;
	interface_t*	sds		= nullptr;
	FILE*			File;
	Config_class*	Cfg 	= nullptr;

public:
	Stereo_Memory	stereo			{ stereobuffer_size };
	long 			Filedata_size 	= 0;

	// Synthesizer read only class
	External_class( Storage_class* sta,
					Config_class* cfg ) : //, Stereo_Memory* stereo ) :
					Logfacility_class("External")
	{
		this->StA 			= sta;
		this->File 			= NULL;
		this->Cfg			= cfg;
		stereo.Info			( "External Stereo data") ;
	};


	// Audioserver write only class
	External_class( Config_class* cfg,
					interface_t* sds ) : //, Stereo_Memory* stereo ) :
					Logfacility_class("External")
	{
		this->StA 			= nullptr;
		this->File 			= NULL;
		this->Cfg			= cfg;
		this->sds			= sds;
		stereo.Info			( "External Stereo data") ;
	};

	virtual ~External_class(){};

	struct status_struct
	{
		bool record 	= false;
	} status;


	bool Read_file_data(  );
	bool Read_file_header( string );
	void Save_record_data( uint sec, int filenr );
	void Mono2Stereo( Data_t* mono, uint size );
	void Record_buffer( stereo_t* src, stereo_t* dst, buffer_t offs );
	string GetName();
	void Test_External();

private:
	const uint 		MAXWAVFILES = 5; // max numbers of file names generated automatically
	long int 		read_position = 0;
	wav_struct_t 	header_struct;
	string 			Name 		= "";
	string 			Filename 	= "";
	string 			add_header 	= "cat " + file_structure().raw_file +
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
