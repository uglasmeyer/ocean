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
 * External.cpp
 *
 *  Created on: Mar 29, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <External.h>
#include <System.h>

// Synthesizer read data from file into StA_External
External_class::External_class	( 	Storage_class* sta,
									Config_class* cfg,
									interface_t* sds ) :
	Logfacility_class("External"),
	Memory_base( sizeof_Data, sta->param.size * sizeof_Data ),
	Stereo_Memory<stereo_t>( sizeof_stereo, sta->param.size * sizeof_stereo )
{
	className			= Logfacility_class::className;
	this->StA_ext 			= sta;
	this->File 			= NULL;
	this->Cfg			= cfg;
	this->sds			= sds;
	this->fs			= cfg->fs;

	Filedata_size		= StA_ext->param.size; // wavedisplay external with dynamic size

	Stereo_Memory::DsInfo	( ) ;
};

void External_class::SetName( string  _name )
{
	Name 			= _name;
	Filename 		= fs->musicdir + Name + fs->wav_type;
}

const bool External_class::Read_file_header( string name )
{
	SetName( name );
	read_position = 0;
	File = fopen( Filename.data(), "rb" );
	if ( File )
	{
		buffer_t nrecords = fread( &header_struct, sizeof(header_struct), 1, File );
		this->StA_ext->param.name = Filename;

		Comment(INFO, "Using external wav-file: " + Filename);
		Comment(INFO, "Sample rate:             " + to_string( header_struct.srate ));
		Comment(INFO, "Data size  :             " + to_string(header_struct.dlength ));
		Comment(DEBUG, Error_text( errno ));
		return ( nrecords == 1 );
	}
	else
	{
		Comment(ERROR, Error_text( errno ));
		Comment(ERROR, "Cannot open file " + Filename );
		Filename = "";

		return false;
	}
}

const bool External_class::Read_file_data(  )
{
	if ( feof( File ) )
	{
		Comment( INFO, Error_text( errno ) );
		return false;
	}

	buffer_t 	bytes 	= header_struct.dlength;
	uint 		blocks 	= bytes/sizeof_stereo/StA_ext->mem_ds.record_size;
	uint 		frames	= bytes/sizeof_stereo;

	if (LogMask[DBG2])
		StA_ext->DsInfo("Memory Array External");
	if ( frames > StA_ext->mem_ds.data_blocks )
	{
		Comment(WARN, "Partly read file data into memory.");
		Comment(WARN, "Taking limits from memory info" );
		frames 	= StA_ext->mem_ds.data_blocks;
		blocks	= StA_ext->mem_ds.max_records;
		bytes 	= StA_ext->mem_ds.bytes;
	}

	StA_ext->Clear_data( 0 );

	if( read_stereo_data( bytes ) )
	{
		for ( buffer_t n = 0; n < frames; n++)
		{
			Data_t L = stereo_data[n].left;
			Data_t R = stereo_data[n].right;
			StA_ext->Data[n]	= L + R;
		}
		Comment(INFO,"Converted stereo to mono data");
		Comment(INFO,"Bytes   " + to_string(bytes));
		Comment(INFO,"Blocks  " + to_string(blocks));
		Comment(INFO,"Structs " + to_string(frames));
		StA_ext->Store_counter( blocks );

		return true;
	}
	else
	{
		Filename = "";
		return false;
	}
}

bool External_class::read_stereo_data( buffer_t data_bytes  )
{
	read_position += fread( stereo_data ,
							sizeof_stereo,
							data_bytes/sizeof_stereo,
							File  );
	Comment( DEBUG, to_string( read_position ));
	buffer_t
	read_bytes		= read_position * sizeof_stereo;
	Filedata_size	= read_position;

	if ( read_bytes >= data_bytes )
	{
		close( );
		Comment(DEBUG,"read bytes: " + to_string(read_bytes) );
		Comment(DEBUG,"data bytes: " + to_string(header_struct.dlength) );
		Comment(DEBUG,"diff bytes: " + to_string(header_struct.dlength - read_bytes ));
		return true;
	}
	return false;
}

void External_class::close(  )
{
	Filename = "";
	fclose( File );
}


/* public Audioserver functions */
// Audioserver write audio data continuesly into stereo_buffer and write stereo_buffer to file
External_class::External_class( Config_class* cfg,
								interface_t* sds ) : //, Stereo_Memory* stereo ) :
	Logfacility_class("External"),
	Memory_base		( sizeof_stereo, cfg->Config.record_sec * frames_per_sec * sizeof_stereo ),
	Stereo_Memory	( sizeof_stereo, cfg->Config.record_sec * frames_per_sec * sizeof_stereo )
{
	this->className		= Logfacility_class::className;
	this->StA_ext 			= nullptr;
	this->File 			= NULL;
	this->Cfg			= cfg;
	this->sds			= sds;
	this->fs			= cfg->fs;

	Stereo_Memory::DsInfo( ) ;
};

void External_class::Record_buffer( Stereo_t* src, buffer_t frames )
{
	for ( buffer_t n = 0; n < frames; n++ )
	{
		Stereo_Memory::stereo_data[n + record_size ].left  = rint( src[ n ].left );
		Stereo_Memory::stereo_data[n + record_size ].right = rint( src[ n ].right);
	}
	record_size += frames; // variable frame size (audioframes);
}

int External_class::generate_file_no()
{
	fstream		File;
	string		counterfile	= fs->counter_file;
	if ( LogMask[ TEST ])
	{
		counterfile = testcounter_file;
	}

	if ( not filesystem::exists( counterfile ))
	{
		Comment( INFO, "creating file number file");
		File.open( counterfile, fstream::out );
		File << "1" << endl;
		File.close();
		return 1;
	}

	File.open( counterfile, fstream::in );
	String S {""};
	getline( File, S.Str );
	File.close();

	filesystem::remove( counterfile );
	int filenr = S.to_int( S.Str );
	range_T<int> wavfileno_range{ 1, (int)Cfg->Config.MAXWAVFILES };

	filenr = check_range( wavfileno_range, filenr + 1, "filenr" );

	File.open( counterfile, fstream::out );
	File << filenr << endl;
	File.close();

	return filenr;
}

long External_class::write_audio_data( string filename, buffer_t rcounter )
{
	FILE* fd 	= fopen( filename.data(), "ab" );
    long count 	= fwrite( this->stereo_data, sizeof_stereo, rcounter , fd );
    fclose( fd );
    return count;
}


long  External_class::write_wav_header( string dest)
{
	if ( filesystem::exists( dest ))
		filesystem::remove( dest );

	FILE* 			fd 			= fopen( dest.data(), "wb");
	if( fd )
	{
		wav_struct_t* 	addr 		= &header_struct;
		long count = fwrite( addr, 1, sizeof(header_struct), fd );
		fclose( fd );
		return count;
	}
	else
	{
		Comment( ERROR, Error_text( errno ) );
		return 0;
	}
}

void External_class::wav_define (  long raw_filesize ) // add filesize to wav structure
{
	header_struct.flength = raw_filesize + sizeof(header_struct);
	header_struct.dlength = raw_filesize;
}

void External_class::write_music_file( string musicfile )
{
	uintmax_t raw_data_size = filesystem::file_size( fs->raw_file );
	if ( raw_data_size <= 0 )
	{
		Comment( ERROR, "empty raw file, nothing to do" );
		return ;
	}
	wav_define( raw_data_size);
	long header_size = write_wav_header( musicfile );
	if ( header_size != sizeof(header_struct) )
	{
		Comment(ERROR, "error generating wav_header header ") ;
		return ;
	}
	System_execute( addheader_cmd() );
}

string External_class::id3tool_cmd( string mp3 )
{
	string genre = Cfg->Config.Genre;
	string author= Cfg->Config.author;
	string album = Cfg->Config.album;
	string title = Cfg->Config.title;

	string cmd = "id3tool -t '" + title + "'" 	+
			" -r " + "'" + author	+ "'"+
			" -a " + "'" + album	+ "'"+
			" -G " +  genre + " " +
			mp3;
	Comment( INFO, cmd );
	return cmd;
}

string External_class::addheader_cmd()
{
	string add_header 	= 	"cat " + fs->raw_file + " >> " + fs->wav_file;
	return add_header;
}

string External_class::ffmpeg_cmd( string wav, string mp3 )
{
	string cmd = Cfg->Config.ffmpeg + " -y -i " 	+ wav + " -f mp3 " + mp3 + " " ;
	Comment( INFO, cmd );
	return cmd;
}

void External_class::Save_record_data( string filename )
{
	if ( record_size == 0 )
	{
		Comment( WARN, "Nothing to do");
		return ;
	}
				Remove_file		( fs->raw_file );
	buffer_t 	count			= write_audio_data( fs->raw_file, record_size );
	bool 		success 		= ( record_size == count );
	if ( success )
		Info( "All", record_size * mem_ds.sizeof_type, "bytes written to file");
	else
	{
		Comment(WARN,to_string(count) + " of " + to_string(record_size) + " written");
		return;
	};

				Remove_file		( fs->wav_file );
				write_music_file( fs->wav_file );

				Remove_file		( fs->mp3_file );
	string 		convert_wav2mp3 = ffmpeg_cmd(fs->wav_file, fs->mp3_file ) ;
				System_execute	( convert_wav2mp3 );

	string 		insert_mp3_tags = id3tool_cmd( fs->mp3_file );
				System_execute( insert_mp3_tags );

				Rename_file		( fs->wav_file, Filename);
				Filename 		= "";
				record_size 	= 0; // reset size
}

const string External_class::Save_record_data( int filenr)
{
	string 		filename 	= "";
	if ( filenr > 0 ) // generate a file name
	{
		filenr 		= generate_file_no();
		filename 	= fs->get_rec_filename( filenr);
		sds->FileNo = filenr;
	}
	else 			// use filename from sds
	{
		filename.assign( sds->Other );
	}
	SetName		( filename );

	Comment		( INFO, "Prepare record file:", Name );
	Comment		( INFO, "Record frames:", record_size );
	Stereo_Memory::DsInfo( );

	Save_record_data( Filename );
    return filename;

}

void External_class::Test_External()
{
	TEST_START( className );
	StA_ext->DsInfo( "External StA Info");
	Stereo_Memory::DsInfo( );

	if ( filesystem::exists( testcounter_file ))
		filesystem::remove( testcounter_file );

	for( uint n = 1; n <= Cfg->Config.MAXWAVFILES; n++ )
	{
		cout << n << endl;
		uint	fnr = generate_file_no( );
		Assert_equal( fnr , n );
	}

	ffmpeg_cmd(fs->wav_file, fs->mp3_file ) ;
	id3tool_cmd( fs->mp3_file );

	TEST_END( className );
}

/**************************************************
 * StAExternal_class
 *************************************************/
void StAExternal_class::copy_sta2stereo( Data_t* src_p, buffer_t frames )
{

	for ( buffer_t n = 0; n < frames; n++ )
	{
		Stereo_Memory::stereo_data[n ].left  = rint( src_p[ n ] * 0.7 );
		Stereo_Memory::stereo_data[n ].right = rint( src_p[ n ] * 0.7 );
	}
	record_size = frames; // variable frame size (audioframes);
}
void StAExternal_class::Convert_StA2WAV( StAId_e staid, range_T<buffer_t> range  )
{
	// define staid
	if( staid == STA_SIZE ) return;
	this->StA	= &StA_arr->at( staid );

	// copy
	copy_sta2stereo( &this->StA->Data[range.min], range.len );

	// dump
	SetName( string( sds->Other ) + "StA" );
	Save_record_data( Filename );

}



