/*
 * GUIinterface.cpp
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */

#include <Interface.h>
#include <Oscbase.h>
#include <Config.h>
#include <System.h>


Interface_class::Interface_class()
: Logfacility_class("Shared Data" ),
  Config_class( "Shared Data" )
{

	bool shm_exists;

	Comment(INFO, "allocating shared memory for IPC");

	// use shm of ifd_data as a persistent checkpoint
	shmget( Config.SDS_key, sizeof(ifd_t), S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL);
	shm_exists = ( EEXIST == errno );
	addr 		= (ifd_t*) buffer( 0, Config.SDS_key );
	if ( not shm_exists )
	{
		Comment(INFO, "initializing data interface using default values ");
		memcpy( addr	, &ifd_data		, sizeof( ifd_t ) );
	}
	Comment( INFO, "check shared memory version");
	filesystem::path sds_dump = file_structure().ifd_file;
	if (( filesystem::exists( sds_dump )))
	{
		size_t fsize = filesystem::file_size( sds_dump );
		if ( fsize != sizeof( ifd_data ))
		{
			Exception( "IPC version " + to_string( ifd_data.version ) + " differs in size" );
		}
	}
	if (( ifd_data.version == addr->version ))
	{
		Comment( INFO, "OK");
	}
	else
	{
		Comment( ERROR, "Failed");
		Exception( "IPC version " + to_string( ifd_data.version ) +
				" differs from BIN version " + to_string( addr->version )  +
				" or lib/ifd_data.bin size ");
	}
	Waveform_vec = GUIspectrum.Get_waveform_vec();
}

Interface_class::~Interface_class()
{
	Commit();
	Comment(INFO, "detach GUI interface from id: " + to_string( shm_info.id));
	shmdt( shm_info.addr );
}


void Interface_class::Show_interface()
{
	auto Lline = []( string s, auto v )
		{ cout << setw(40) << dec  << setfill('.') 	<< left << s << setw(40) << v << endl;};
	auto lline = []( string s, auto v )
		{ cout << setw(20) << dec  << setfill('.') 	<< left << s << setw(20) << v ; };
	auto rline = []( string s, auto v )
		{ cout << setw(20) << dec  << setfill('.') 	<< left <<s << setw(20) << v << endl;};
	auto decode = [this]( uint8_t value )
		{ return uint8_code_str[value]; };
	auto conv_bool_s = []( bool b )
		{ return ( b ) ? string("yes") : string("no "); };

	assert( uint8_code_str.size() == LASTNUM );
	string status1 {};
	status1 	= 	      conv_bool_s(addr->mixer_status.external) +
					"," + conv_bool_s(addr->mixer_status.notes) +
					"," + conv_bool_s(addr->mixer_status.play) +
					"," + conv_bool_s(addr->mixer_status.mute) +
					"," + conv_bool_s(addr->mixer_status.kbd) +
					"," + conv_bool_s(addr->mixer_status.instrument );
	string status2 {};
	uint arrno = 0;
	for( StA_status_t status : addr->StA_state )
	{
		status2.append( to_string(arrno) + ":");
		status2.append( to_string(status.play));
		status2.append( to_string(status.store));
		status2.append( ", ");
		arrno++;
	}
	string status3 {};
	for( uint8_t amp : addr->StA_amp_arr )
		status3.append( to_string( amp) + ", " );


//	Read_str('a');

	cout << setfill( '-') << setw(80) << left <<
			"\nShared Data Structure " + Version_str <<"-"<< addr->version << endl;

	lline( "(M)ain (F)requency:" , addr->Main_Freq );
	rline( "(A)DSR (G)lide freq:" , (int)addr->Soft_freq);

	lline( "(M)ain (A)mplitude:" , (int)addr->Master_Amp );
	rline( "(A)DSR (D)ecay:    " , (int)addr->Main_adsr.attack );

	lline( "Main duration      " , (int)addr->Main_Duration);
//	rline( "(A)DSR D(u)ration: " , bps_struct().getbps_str((int)addr->Main_adsr.bps_id) );
	rline( "(A)DSR Beat p. sec:" , (int)addr->Main_adsr.bps_id) ;

	lline( "(M)ain (W)aveform: " , Waveform_vec[ (int)addr->MAIN_spectrum.id ]);
	rline( "(A)DSR (S)ustain:  " , (int)addr->Main_adsr.decay );

	lline( "(F)MO  (F)requency:" , addr->FMO_Freq);
	rline( "(V)CO  (F)requency:" , addr->VCO_Freq);

	lline( "(F)MO  (A)mplitude:" , (int)addr->FMO_Amp);
	rline( "(V)CO  (A)mplitude:" , (int) addr->VCO_Amp);

	lline( "(F)MO  (W)aveform: " , Waveform_vec[ (int)addr->FMO_spectrum.id ]);
	rline( "(V)CO  (W)aveform: " , Waveform_vec[ (int)addr->VCO_spectrum.id ]);

	lline( "", "" );
	rline( "VCO  PMW dial      " , (int)addr->PMW_dial) ;

	rline( "Spectrum:          " , Spectrum.Show_this_spectrum( addr->MAIN_spectrum ));
	rline( "Spectrum:          " , Spectrum.Show_this_spectrum( addr->VCO_spectrum ));
	rline( "Spectrum:          " , Spectrum.Show_this_spectrum( addr->FMO_spectrum ));


	lline( "Mixer Volume:      " , (int)addr->MIX_Amp );
	rline( "Mixer Id           " , (int)addr->MIX_Id );

	lline( "Sync data id       " , (int) addr->SHMID);
	rline( "Communication Key  " , (int) addr->KEY );

	lline( "Record Progress   :" , (int)addr->RecCounter);
	rline( "File No.          :" , (int)addr->FileNo );

	lline( "AudioServer status:" , decode(addr->AudioServer));
	rline( "Composer status   :" , decode(addr->Composer));

	lline( "Synthesizer status:" , decode(addr->Synthesizer));
	rline( "Userinterface stat:" , decode(addr->UserInterface));

	rline( "Data Mode         :" , decode(addr->MODE));

	lline( "Instrument        :" , addr->Instrument);
	rline( "Wav filename      :" , addr->Other );

	lline( "Notes             :" , addr->Notes );
	rline( "Noteline duration :" , (int) addr->Noteline_sec);

	Lline( "Status Extr,Note,Play,Mute,Kbd,Inst:" , status1 );
	rline( "StA Amp;", status3 );
	rline( "Status StA #:ps,  :" , status2);


}

bool Interface_class::reject(char status, int id )
{
	if (( status == RUNNING ) and ( id != COMPID ))
	{
		if( previous_status != status )
			cout << "Observer mode ON" << endl;
		previous_status = status;
		return true;
	}
	else
	{
		return false;
	}
};

void Interface_class::Write_arr( const wd_arr_t& arr )
{
	for( buffer_t n = 0 ; n < wavedisplay_len; n++ )
		addr->wavedata[n] = arr[n] ;
}

void Interface_class::Write_str(const char selector, const string str )
{
	if (reject( addr->Composer, client_id )) return;

	if ( addr->Comstack != RUNNING )
		addr->UpdateFlag = true;


	switch ( selector )
	{
		case INSTRUMENTSTR_KEY :
		{
			strcpy( addr->Instrument, str.data());
			break;
		}
		case NOTESSTR_KEY :
		{
			strcpy( addr->Notes, str.data() );
			break;
		}
		case OTHERSTR_KEY :
		{
			strcpy( addr->Other, str.data() );
			break;
		}
		default:
		{
			strcpy( addr->Other, str.data() );
			break;
		}
	}

}

string Interface_class::Read_str( char selector )
{
	string str;
	switch ( selector )
	{
		case INSTRUMENTSTR_KEY :
		{
			str.assign( addr->Instrument );
			break;
		}
		case NOTESSTR_KEY :
		{
			str.assign( addr->Notes );
			break;
		}
		default :
		{
			str.assign( addr->Other );
			break;
		}

	}

	return str;
}


void* Interface_class::buffer( buffer_t shm_size, key_t shm_key )
{
	int shmflg, shm_id;

	shmflg = S_IRUSR | S_IWUSR | IPC_CREAT;
	shm_id = shmget ( shm_key, shm_size, shmflg );//shm_size+1, shmflg );// obtain a shm identifier
	if ( shm_id < 0 ){
		Exception("cannot get shared memory" + to_string( shm_id ) );
	}

	void* addr = shmat (shm_id, 0, 0);
	Comment(INFO,"GUI  shm_id " + to_string(shm_id) );

	shm_info.addr 	= addr;
	shm_info.id 	= shm_id;
	shm_info.key 	= shm_key;
	shm_info.size 	= shm_size;

	return ( addr );
};

void Interface_class::Announce( uint id, uint8_t* status)
{
	client_id = id;
	*status = RUNNING;
	addr->UpdateFlag = true;

}
void Interface_class::Reset_ifd()
{
	Comment(INFO, "Reset shared data");
	memcpy( addr	, &ifd_data		, sizeof( ifd_t ) );
	Dump_ifd();

}

bool Interface_class::Restore_ifd()
{

	Comment(INFO,"Restore shared data from file");

	FILE* fd = fopen( file_structure().ifd_file.data() , "r");
	if ( not fd )
		return false;
	uint size = fread( addr, sizeof( ifd_data ), 1, fd);
	fclose( fd );

	return ( size == sizeof( ifd_data ));
}

void Interface_class::Dump_ifd()
{
	Comment(INFO,"Dump shared data to file");
	FILE* fd = fopen( file_structure().ifd_file.data() , "w");
	fwrite( addr, sizeof( ifd_data ), 1, fd);
	fclose( fd );
}

void Interface_class::Update( char ch )
{
	Commit();

	addr->FLAG = ch;
	addr->UserInterface = UPDATEGUI;
}
void Interface_class::Commit()
{
	addr->KEY 	= NULLKEY;
	addr->FLAG 	= NULLKEY;
	addr->UpdateFlag = true;
	if ( SEM.getval( PROCESSOR_WAIT ) > 0 )
		SEM.release( PROCESSOR_WAIT );
}

void Interface_class::Set( bool& key, bool value )
{
	if ( reject( addr->Composer, client_id ) ) return;
	key = value;
}

void Interface_class::Set( uint8_t& key, uint8_t value )
{
	if ( reject( addr->Composer, client_id ) ) return;
	key = value;
}

void Interface_class::Set( uint16_t& key, uint16_t value )
{
	if ( reject( addr->Composer, client_id ) ) return;
	key = value;
}

void Interface_class::Set( float& key, float value )
{
	if ( reject( addr->Composer, client_id ) ) return;
	key = value;
}


