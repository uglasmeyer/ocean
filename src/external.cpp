/*
 * External.cpp
 *
 *  Created on: Mar 29, 2024
 *      Author: sirius
 */

#include <data/DataWorld.h>
#include <External.h>
#include <System.h>



string External_class::GetName()
{
	return Filename;
}
void External_class::setName( string name )
{
	Name 		= name;
	Filename 	= file_structure().Dir.musicdir + Name + ".wav";
}

bool External_class::Read_file_header( string name )
{
	setName( name );
	read_position = 0;
	File = fopen( Filename.data(), "rb" );
	if ( File )
	{
		buffer_t nrecords = fread( &header_struct, sizeof(header_struct), 1, File );
		this->StA->StAparam.name = Filename;

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

bool External_class::Read_file_data(  )
{
	if ( feof( File )    )
	{
		Comment( INFO, Error_text( errno ) );
		return false;
	}

	buffer_t 	bytes 	= header_struct.dlength;
	uint 		blocks 	= bytes/sizeof(stereo_t)/StA->ds.block_size;
	uint 		structs	= bytes/sizeof(stereo_t);

	if (Log[DBG2])
		StA->Info("Memory Array External");
	if ( structs > StA->ds.data_blocks )
	{
		Comment(WARN, "Partly read file data into memory.");
		Comment(WARN, "Taking limits from memory info" );
		structs 	= StA->ds.data_blocks;
		blocks 		= StA->ds.max_records;
		bytes 		= StA->ds.mem_bytes;
	}
	if( read_stereo_data( bytes ) )
	{
		for ( buffer_t n = 0; n < structs; n++)
		{
			Data_t L = stereo_data[n].left;
			Data_t R = stereo_data[n].right;
			StA->Data[n]	= L + R;
		}
		Comment(INFO,"Converted stereo to mono data");
		Comment(INFO,"Bytes   " + to_string(bytes));
		Comment(INFO,"Blocks  " + to_string(blocks));
		Comment(INFO,"Structs " + to_string(structs));
		StA->Set_store_counter( blocks - 1 );

		return true;
	}
	else
	{
		Filename = "";
		return false;
	}
}


bool External_class::read_stereo_data( long data_bytes  )
{
	read_position += fread( stereo_data ,
							sizeof(stereo_t),
							data_bytes/sizeof(stereo_t),
							File  );
	Comment( DEBUG, to_string( read_position ));
	long read_bytes = read_position * sizeof(stereo_t);
	Filedata_size 			= read_bytes;

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

void External_class::Record_buffer( stereo_t* src, buffer_t frames, buffer_t offs )
{
	for ( buffer_t n = 0; n < frames; n++ )
	{
		Stereo_Memory::stereo_data[n + offs ] = src[ n ];
//		dst[n + offs ] = src[ n ];
	}
}

//------------------------------------------------------------------------------------------------
// common
Logfacility_class Log_system("System log");

string testcounter = "/tmp/counter";
int generate_file_no( uint maxfileno )
{
	fstream File;
	string counterfile = file_structure().counter_file;
	if ( Log_system.Log[ TEST ])
	{
		counterfile = testcounter;
	}

	if ( not filesystem::exists( counterfile ))
	{
		Log_system.Comment( INFO, "creating file number file");
		File.open( counterfile, fstream::out );
		File << "0" << endl;
		File.close();
		return 0;
	}

	File.open( counterfile, fstream::in );
	String S {""};
	getline( File, S.Str );
	File.close();

	filesystem::remove( counterfile );
	int filenr = S.to_int( S.Str );
	filenr++;
	filenr = filenr % maxfileno;

	File.open( counterfile, fstream::out );
	File << filenr << endl;
	File.close();

	return filenr;

}

void remove_file( string file )
{
	if ( filesystem::exists( file ) )
		filesystem::remove( file);
	if ( filesystem::exists( file ))
		Exception( "cannot remove file: " + file );

}
void rename_file( string old_name, string new_name )
{
	remove_file( new_name );
    filesystem::rename( old_name, new_name);

	if ( filesystem::exists( old_name ))
		Exception( "cannot remove file: " + old_name );
}

// --------------------------------------------------------------------------------------------

long External_class::write_audio_data( string filename, buffer_t rcounter )
{

	FILE* fd 	= fopen( filename.data(), "ab" );
    long count 	= fwrite( this->stereo_data, sizeof(stereo_t), rcounter , fd );
    fclose( fd );
    return count;
}

void External_class::Mono2Stereo( Data_t* Data, uint size )
{

	for( buffer_t n = 0; n < size; n++)
	{
		stereo_data[n].left  		= rint( Data[n] );
		stereo_data[n].right 		= rint( Data[n] );
	}

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
	uintmax_t raw_data_size = filesystem::file_size( file_structure().raw_file );
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
	system_execute( add_header );
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

string External_class::ffmpeg_cmd( string wav, string mp3 )
{
	string cmd = Cfg->Config.ffmpeg + " -y -i " 	+ wav + " -f mp3 " + mp3 + " " ;
	Comment( INFO, cmd );
	return cmd;
}

void External_class::Save_record_data( uint sec, int filenr)
{
	if ( filenr == 0 ) // generate a file name
		filenr = generate_file_no( MAXWAVFILES );

	setName( file_structure().filename + to_string( filenr) );

	Comment( INFO, "Prepare record file " + Name );
//	long rcounter = StA->record_data;// * 2; //mono to stereo factor
	long rcounter = sec * frames_per_sec;
	Comment( INFO, "Record frames: " + to_string (rcounter));
	Stereo_Memory::Info( "External Stereo data");

	if ( rcounter == 0 )
	{
		Comment( WARN, "Nothing to do");
		return;
	}

	remove_file( file_structure().raw_file );

	long count		= write_audio_data( file_structure().raw_file, rcounter );
	bool success = ( rcounter == count );
	if ( success )
		Comment(INFO,"All " + to_string( rcounter * ds.sizeof_data ) + " bytes written to file");
	else
	{
		Comment(WARN,to_string(count) + " of " + to_string(rcounter) + " written");
		return ;
	};

	remove_file( file_structure().wav_file );

	write_music_file( file_structure().wav_file );

	remove_file( file_structure().mp3_file );

	string convert_wav2mp3 = ffmpeg_cmd(file_structure().wav_file, file_structure().mp3_file ) ;
	system_execute( convert_wav2mp3 );

	string insert_mp3_tags = id3tool_cmd( file_structure().mp3_file );
	system_execute( insert_mp3_tags );

    rename_file( file_structure().wav_file, Filename);
    Filename = "";

}

void External_class::Test_External()
{
	TEST_START( className );
	Assert( StA->ds.max_records - Stereo_Memory::ds.max_records == 0 ,
			to_string ( StA->ds.max_records ) + "=" + to_string( Stereo_Memory::ds.max_records));

	Log_system.Set_Loglevel( TEST, true);
	if ( filesystem::exists( testcounter ))
		filesystem::remove( testcounter );

	int
	fnr = generate_file_no( MAXWAVFILES );
	assert( fnr == 0 );
	fnr = generate_file_no( MAXWAVFILES );
	assert( fnr == 1 );
	fnr = generate_file_no( MAXWAVFILES );
	assert( fnr == 2 );
	fnr = generate_file_no( MAXWAVFILES );
	assert( fnr == 3 );


	ffmpeg_cmd(file_structure().wav_file, file_structure().mp3_file ) ;
	id3tool_cmd( file_structure().mp3_file );

	TEST_END( "External");
}



