/*
 * External.cpp
 *
 *  Created on: Mar 29, 2024
 *      Author: sirius
 */

#include "External.h"

void External_class::SetName( string name )
{
	Name = name;
	Filename = dir_struct().musicdir + Name + ".wav";
}

bool External_class::read_file_header( string name )
{
	SetName( name );
	read_position = 0;
	File = fopen( Filename.data(), "rb" );
	if ( File )
	{
		buffer_t nrecords = fread( &header_struct, sizeof(header_struct), 1, File );
		StA->mbparam.name = Filename;

		Comment(INFO, "Using external wav-file: " + Filename);
		Comment(INFO, "Sample rate:             " + to_string( header_struct.srate ));
		Comment(INFO, "Data size  :             " + to_string(header_struct.dlength ));
		Comment(DEBUG, error_text( errno ));
		return ( nrecords == 1 );
	}
	else
	{
		Comment(ERROR, error_text( errno ));
		Comment(ERROR, "Cannot open file " + Filename );
		return false;
	}
}

bool External_class::read_file_data(  )
{
	if ( feof( File )    )
	{
		Comment( INFO, error_text( errno ) );
		return false;
	}

	buffer_t 	bytes 	= header_struct.dlength;
	uint 		blocks 	= bytes/sizeof(stereo_t)/StA->block_size;
	uint 		structs	= bytes/sizeof(stereo_t);

	if (Log[DBG2])
		StA->Info("Memory Array External");
	if ( structs >= StA->info.data_blocks )
	{
		Comment(WARN, "Partly read file data into memory.");
		Comment(WARN, "Taking limits from memory info" );
		structs 	= StA->info.data_blocks;
		blocks 		= StA->info.max_records;
		bytes 		= StA->info.mem_bytes;
	}
	if( read_stereo_data( bytes ) )
	{
		for ( buffer_t n = 0; n < structs; n++)
		{	// provide mean value of stereo data into mono loop
			// but
			// use the opposite sign to restore mono data from stereo synthesizer data file
			// convert fermion to boson
			Data_t L = stereo->stereo_data[n].left;
			Data_t R = stereo->stereo_data[n].right;

			StA->Data[n]	= L + R;
		}
		StA->set_store_counter( blocks );
		Comment(INFO,"Converted stereo to mono data");
		Comment(INFO,"Bytes   " + to_string(bytes));
		Comment(INFO,"Blocks  " + to_string(blocks));
		Comment(INFO,"Structs " + to_string(structs));

		return true;
	}
	else
		return false;
}


bool External_class::read_stereo_data( long data_bytes  )
{
	read_position += fread( stereo->stereo_data ,
							sizeof(stereo_t),
							data_bytes/sizeof(stereo_t),
							File  );
	Comment( DEBUG, to_string( read_position ));
	long read_bytes = read_position * sizeof(stereo_t);
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
	fclose( File );
}

//------------------------------------------------------------------------------------------------
// common
Logfacility_class Log_system("System log");

void system_execute( string cmd )
{
	std::cout.flush();
	int ret = system( cmd.data() );
	if ( ret != 0 )
	{
		Log_system.Comment( Log_system.ERROR, cmd);
		Log_system.Comment( Log_system.ERROR, "check out system error message ");
		exit( ret );
	}
}

int generate_file_no( )
{
	uint16_t filenumber ;

	string counter_file = file_structure().counter_file;
	size_t count = 0;
	FILE* fd;
	if ( filesystem::exists( counter_file ))
	{
		fd = fopen( counter_file.data(), "rb" );
		count = fread( &filenumber, sizeof(filenumber), 1, fd );
		fclose( fd );
	}

	if ( count != 1 )
	{
		filenumber = 0;
	}

	filenumber = filenumber + 1;

	fd = fopen( counter_file.data(), "wb" );
    count = fwrite( &filenumber, sizeof(filenumber),  1, fd );
    fclose( fd );

    return filenumber;


}


// --------------------------------------------------------------------------------------------

long External_class::write_audio_data( string filename, buffer_t rcounter )
{

	FILE* fd 	= fopen( filename.data(), "ab" );
    long count 	= fwrite( this->stereo->stereo_data, sizeof(stereo_t), rcounter , fd );
    fclose( fd );
    return count;
}



long  External_class::write_wav_header( string dest)
{
	if ( filesystem::exists( dest ))
		filesystem::remove( dest );

	FILE* 			fd 			= fopen( dest.data(), "wb");
	wav_struct_t* 	addr 		= &header_struct;
	long count = fwrite( addr, 1, sizeof(header_struct), fd );
	fclose( fd );
	return count;
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

void External_class::save_record_data( int fileno)
{
	Comment( INFO, "Prepare record file ");
	long rcounter = stereo->info.record_counter * stereo->info.block_size;
	Comment( INFO, "Record counter: " + to_string (rcounter));
	stereo->Info( "External Stereo data");

	if ( rcounter == 0 )
	{
		Comment( WARN, "Nothing to do");
		return;
	}

	if ( filesystem::exists( file_structure().raw_file ))
	{
		filesystem::remove( file_structure().raw_file );
	}

	long count		= write_audio_data( file_structure().raw_file, rcounter );
	bool success = ( rcounter == count );
	if ( success )
		Comment(INFO,"All " + to_string( rcounter*stereo->info.sizeof_data) + " bytes written to file");
	else
	{
		Comment(WARN,to_string(count) + " of " + to_string(rcounter) + " written");
		return ;
	};

	if ( filesystem::exists( file_structure().wav_file ))
		filesystem::remove( file_structure().wav_file );

	assert( not filesystem::exists( file_structure().wav_file ) );

	write_music_file( file_structure().wav_file );

	if ( filesystem::exists( file_structure().mp3_file ))
		filesystem::remove( file_structure().mp3_file );

	system_execute( convert_wav2mp3 );
	system_execute( insert_mp3_tags );

	if ( fileno == 0 ) // generate a file name
		fileno = generate_file_no( );

    string newfilename = 	dir_struct().musicdir +
    						file_structure().filename + to_string( fileno) + ".wav";
    filesystem::remove( newfilename );
    filesystem::rename( file_structure().wav_file, newfilename);

}

void External_class::test()
{
	Set_Loglevel(TEST, true);
 	Comment( TEST, "Testing External_class");
	assert( StA->info.max_records - stereo->info.max_records == 0 );


}



