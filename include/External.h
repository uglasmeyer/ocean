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
 * External.h
 *
 *  Created on: Mar 29, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef EXTERNAL_H_
#define EXTERNAL_H_

#include <data/DataWorld.h>
#include <Progressbar.h>
#include <Wav.h>


/**************************************************
 * External_class
 *************************************************/

class 							External_class :
	public virtual 				Logfacility_class,
	public virtual 				Stereo_Memory<stereo_t>

{
	string						className 			= "";
	Storage_class* 				StA_ext					= nullptr;
	FILE*						File				= nullptr;
	Config_class*				Cfg 				= nullptr;
	file_structure*				fs					= nullptr;

public:
	buffer_t					Filedata_size 		= 0;
	interface_t*				sds					= nullptr;
	string 						Filename 			= "";
	buffer_t					record_size			= 0;

								// Synthesizer read only class
								External_class		( Storage_class*,
													Config_class* cfg,
													interface_t* sds);
	const bool 					Read_file_data		();
	const bool 					Read_file_header	( string );
	void 						SetName				( string name );
	void 						Test_External		();


								// Audioserver write only class
								External_class		( Config_class* cfg,
													interface_t* sds ) ;
	const string 				Save_record_data	( int filenr );
	void 						Save_record_data	( string filename );
	void 						Record_buffer		( Stereo_t* src, buffer_t frames );

	virtual 					~External_class		() = default;

private:
	long int 					read_position 		= 0;

	wav_struct_t 				header_struct;
	string 						Name 				= "";
	string 						testcounter_file 	= "/tmp/counter";

	void 						write_audio_data	( string );
	void 						write_music_file	( string );
	void 						wav_define 			(  long  );
	long 						write_wav_header	( string );
	long 						write_audio_data	( string, buffer_t );
	bool 						read_stereo_data	( buffer_t );
	string						addheader_cmd		();
	string 						ffmpeg_cmd			( string wav, string mp3 );
	string 						id3tool_cmd			( string mp3 );
	int 						generate_file_no	();

	void 						close				();

};


/**************************************************
 * StAExternal_class
 *************************************************/
class StAExternal_class :
	virtual External_class
{
//	string 					className 	= "";
	StorageArray_t*			StA_arr 	= nullptr;
	Storage_class*			StA			= nullptr;
public:
	StAExternal_class( StorageArray_t* sta_arr, Config_class* cfg, interface_t* sds ) :
//		Logfacility_class("StAExternal_class"),
		Memory_base( sta_arr->at(STA_EXTERNAL).param.size * sizeof_Data ),
		Stereo_Memory<stereo_t>( sta_arr->at(STA_EXTERNAL).param.size * sizeof_stereo ),
		External_class( &sta_arr->at(STA_EXTERNAL), cfg, sds )
	{
		this->StA_arr			= sta_arr;
	};
	virtual ~StAExternal_class()
	{
//		DESTRUCTOR( className );
	};
	void Convert_StA2WAV( StAId_e staid );


private:
	void copy_sta2stereo( buffer_t frames );

};



#endif /* EXTERNAL_H_ */
