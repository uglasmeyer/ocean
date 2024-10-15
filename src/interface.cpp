/*
 * GUIinterface.cpp
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */

#include <Oscbase.h>
#include <Config.h>
#include <data/Interface.h>
#include <System.h>


Interface_class::Interface_class( Config_class* cfg, Semaphore_class* sem )
: Logfacility_class("Shared Data" )
{
	stateMap();

	this->Sem_p	= sem;
	this->Cfg_p = cfg;
	Waveform_vec = GUIspectrum.Get_waveform_vec();
}

Interface_class::~Interface_class()
{}

void Interface_class::State_pMap(  )
{
	assert( addr != nullptr );
	state_p_map[NOID]		= nullptr;
	state_p_map[SYNTHID]	= &addr->Synthesizer,
	state_p_map[COMPID]	= &addr->Composer;
	state_p_map[GUI_ID]	= &addr->UserInterface;
	state_p_map[COMSTACKID]=&addr->Comstack;
	state_p_map[AUDIOID]	= &addr->AudioServer;
	state_p_map[RTSPID]	= &addr->Rtsp;

}

void Interface_class::Setup_SDS( uint sdsid, key_t key)
{
	Comment(INFO, "allocating shared memory for IPC " + to_string( sdsid ));

	ds 		= *SHM.Get( key );
	ds.Id	= sdsid;
	this->addr = ( interface_t* ) ds.addr;
	SHM.ShowDs(ds);

	if ( not ds.eexist )
	{
		Comment(INFO, "initializing data interface using default values ");
		memcpy( addr	, &ifd_data		, sds_size );
	}
	Comment( INFO, "check shared memory version");
	dumpFile = file_structure().ifd_file + to_string( sdsid) ;
	filesystem::path sds_dump = dumpFile;
	if (( filesystem::exists( sds_dump )))
	{
		size_t fsize = filesystem::file_size( sds_dump );
		if ( fsize != sds_size)
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
		Comment( ERROR, "Setup SDS failed");
		Exception( "IPC version " + to_string( ifd_data.version ) +
				" differs from BIN version " + to_string( addr->version )  +
				" or lib/ifd_data.bin size ");
	}
	ds.eexist = true;
	addr->SDS_Id = sdsid;
	State_pMap();
}

void Interface_class::stateMap()
{
	state_map[OFFLINE]		= "Offline";
	state_map[RUNNING] 		= "Running";
	state_map[FREERUN] 		= "free running";
	state_map[UPDATEGUI]	= "Update GUI";
	state_map[SYNC] 		= "sync mode";
	state_map[DEFAULT] 		= "default mode";
	state_map[EXITSERVER] 	= "Exit server";
	state_map[KEYBOARD] 	= "Keyboard";
	assert( state_map.size() == LASTNUM );
}

string Interface_class::Decode( uint8_t idx)
{
	return state_map[ idx ];
}

void Interface_class::Show_interface()
{
	auto Lline = []( string s, auto v )
		{ cout << setw(40) << dec  << setfill('.') 	<< left << s << setw(40) << v << endl;};
	auto lline = []( string s, auto v )
		{ cout << setw(20) << dec  << setfill('.') 	<< left << s << setw(20) << v ; };
	auto rline = []( string s, auto v )
		{ cout << setw(20) << dec  << setfill('.') 	<< left <<s << setw(20) << v << endl;};
	auto conv_bool_s = []( bool b )
		{ return ( b ) ? string("yes") : string("no "); };

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



	cout << setfill( '-') << setw(80) << left <<
			"\nShared Data Structure Id " + to_string((int) ds.Id ) +  " " + Version_str <<"-"<< addr->version
			<< endl;

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

	lline( "AudioServer status:" , Decode(addr->AudioServer));
	rline( "Composer status   :" , Decode(addr->Composer));

	lline( "Synthesizer status:" , Decode(addr->Synthesizer));
	rline( "Userinterface stat:" , Decode(addr->UserInterface));

	lline( "Rtsp status       :" , Decode(addr->Rtsp));
	rline( "Data Mode         :" , Decode(addr->MODE));

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
	if (reject( addr->Composer, Type_Id )) return;

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


void Interface_class::Announce( )
{
	Comment(INFO, "announcing application " + Cfg_p->type_map[ Type_Id ] );
	uint8_t* state = state_p_map[ Type_Id];
	assert ( state != nullptr );
	*state = RUNNING;
	addr->UpdateFlag = true;
}
void Interface_class::Reset_ifd()
{
	Comment(INFO, "Reset shared data");
	memcpy( addr	, &ifd_data		, sizeof( interface_t ) );
	Dump_ifd();

}

bool Interface_class::Restore_ifd()
{

	Comment(INFO,"Restore shared data from file");

	FILE* fd = fopen( dumpFile.data() , "r");
	if ( not fd )
		return false;
	uint size = fread( addr, sizeof( ifd_data ), 1, fd);
	fclose( fd );

	return ( size == sizeof( ifd_data ));
}

void Interface_class::Dump_ifd()
{
	Comment(INFO,"Dump shared data to file \n" + dumpFile) ;
	assert( dumpFile.size() > 0 );
	FILE* fd = fopen( dumpFile.data() , "w");
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
	if ( Sem_p->Getval( PROCESSOR_WAIT, GETVAL ) > 0 )
		Sem_p->Release( PROCESSOR_WAIT );
}

void Interface_class::Set( bool& key, bool value )
{
	if ( reject( addr->Composer, Type_Id ) ) return;
	key = value;
}

void Interface_class::Set( uint8_t& key, uint8_t value )
{
	if ( reject( addr->Composer, Type_Id ) ) return;
	key = value;
}

void Interface_class::Set( uint16_t& key, uint16_t value )
{
	if ( reject( addr->Composer, Type_Id ) ) return;
	key = value;
}

void Interface_class::Set( float& key, float value )
{
	if ( reject( addr->Composer, Type_Id ) ) return;
	key = value;
}


