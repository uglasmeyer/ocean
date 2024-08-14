/*
 * GUIinterface.cpp
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */


#include <Interface.h>


Interface_class::Interface_class()
: Logfacility_class("Gui")
{

	bool shm_exists;

	Comment(INFO, "allocating shared memory for IPC");

	// use shm of ifd_data as a persistent checkpoint
	shmget( shm_key,sizeof(ifd_t),S_IRUSR | S_IWUSR | IPC_CREAT|IPC_EXCL);
	shm_exists = ( EEXIST == errno );
	addr 		= (ifd_t*) buffer( 0, shm_key);
	if ( not shm_exists )
	{
		Comment(INFO, "initializing data interface using default values ");
		memcpy( addr	, &ifd_data		, sizeof( ifd_t ) );
	}
	Comment( INFO, "check shared memory version");
	if ( ifd_data.version == addr->version )
	{
		Comment( INFO, "OK");
	}
	else
	{
		Comment( ERROR, "Failed");
		Comment( INFO, 	"IPC version " + to_string( ifd_data.version ) +
						" differs from BIN version " + to_string( addr->version ) );
		exit(1);
	}
	Waveform_vec = GUIspectrum.Get_waveform_vec();
}

Interface_class::~Interface_class()
{
	Comment(INFO, "detach GUI interface from id: " + to_string( shm_info.id));
	shmdt( shm_info.addr );
}

void Interface_class::Show_interface()
{
	auto lline = []( string s, auto v )
			{ cout << setw(20) << dec  << setfill('.') 	<< left << s << setw(20) << v ; };
	auto rline = []( string s, auto v )
			{ cout << setw(20) << dec  << setfill('.') 	<< left <<s << setw(20) << v << endl;};
	auto decode = [this]( uint8_t value )
			{
				assert( uint8_code_str.size() == 15 );
				string str = "";
				if ( value > uint8_code_str.size() )
				{
					this->Comment( this->ERROR, "cannot decode value " + to_string(value));
					return str;
				}
				return uint8_code_str[value];
			};
	auto conv_bool_s = []( bool b )
		{
			return ( b ) ? string("yes") : string("no ");
		};

	string status1 {};
	status1 	= 	      conv_bool_s(addr->mi_status.external) +
					"," + conv_bool_s(addr->mi_status.notes) +
					"," + conv_bool_s(addr->mi_status.play) +
					"," + conv_bool_s(addr->mi_status.mute);
	string status2 {};
	uint arrno = 0;
	for( ma_status_t status : addr->ma_status )
	{
		status2.append( to_string(arrno) + ":");
		status2.append( to_string(status.play));
		status2.append( to_string(status.store));
		status2.append( ", ");
		arrno++;
	}


	Read_str('a');

	cout << setfill( '-') << setw(80) << left << "Shared Data Structure " + Version_str << endl;

	lline( "(M)ain (F)requency:" , addr->Main_Freq );
	rline( "(A)DSR (G)lide freq:" , (int)addr->Soft_freq);
	lline( "(M)ain (A)mplitude:" , (int)addr->Master_Amp );
	rline( "(A)DSR (D)ecay:    " , (int)addr->Main_adsr_attack );
	lline( "Main duration      " , (int)addr->Main_Duration);
	rline( "(A)DSR D(u)ration: " , bps_struct().getbps_str((int)addr->Main_adsr_bps_id) );
	lline( "(M)ain (W)aveform: " , Waveform_vec[ (int)addr->MAIN_spectrum.id ]);
	rline( "(A)DSR (S)ustain:  " , (int)addr->Main_adsr_decay );
	cout << endl;
	lline( "(F)MO  (F)requency:" , addr->FMO_Freq);
	rline( "(V)CO  (F)requency:" , addr->VCO_Freq);
	lline( "(F)MO  (A)mplitude:" , (int)addr->FMO_Amp);
	rline( "(V)CO  (A)mplitude:" , (int) addr->VCO_Amp);
	lline( "(F)MO  (W)aveform: " , Waveform_vec[ (int)addr->FMO_spectrum.id ]);
	rline( "(V)CO  (W)aveform: " , Waveform_vec[ (int)addr->VCO_spectrum.id ]);
//	rline( "." , '.');
	rline( "Spectrum:          " , Spectrum.Show_this_spectrum( addr->MAIN_spectrum ));
	rline( "Spectrum:          " , Spectrum.Show_this_spectrum( addr->VCO_spectrum ));
	rline( "Spectrum:          " , Spectrum.Show_this_spectrum( addr->FMO_spectrum ));
	rline( "VCO  PMW dial      " , (int)addr->PMW_dial) ;
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
	rline( "Instrument        :" , addr->Instrument);
	lline( "Notes             :" , addr->Notes );
	rline( "Noteline duration :" , (int) addr->Noteline_sec);
	lline( "Status Ex,No,Pl,Mu:" , status1);
	rline( "Waveform counter  :" , addr->WD_type_ID);
	rline( "Status StA #:ps,  :" , status2);

}

auto reject = [](char addr, int id )
		{
			if (( addr == RUNNING ) and ( id == 4 ))
			{
				cout << "composer rejected" << endl;
				return true;
			}
			else
			{
				return false;
			}
		};


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
		Comment(ERROR, "cannot get shared memory" + to_string( shm_id ) );
		exit ( 1 );
	}

	void* addr = shmat (shm_id, 0, 0);
	Comment(INFO,"GUI  shm_id " + to_string(shm_id) );

	shm_info.addr 	= addr;
	shm_info.id 	= shm_id;
	shm_info.key 	= shm_key;
	shm_info.size 	= shm_size;

	return ( addr );
};

void Interface_class::Announce( string client, bool flag )
{
	auto set = [flag, this](auto& a , auto id){
		if (flag)
			a = RUNNING;
		else
			a = NOCONTROL ;
		this->Commit();
		return id;
	};

	if ( client.compare("Composer") 	== 0 ) client_id = set( addr->Composer		, 1 );
	if ( client.compare("Synthesizer") 	== 0 ) client_id = set( addr->Synthesizer	, 2 );
	if ( client.compare("AudioServer") 	== 0 ) client_id = set( addr->AudioServer	, 3 );
	if ( client.compare("SndlabGUI") 	== 0 ) client_id = set( addr->UserInterface	, 4 );
	if ( client.compare("Comstack") 	== 0 ) client_id = set( addr->Comstack		, 5 );

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
}

void Interface_class::Set( bool& key, bool value )
{
	if ( reject( addr->Composer, client_id ) ) return;
	key = value;
}

void Interface_class::Set( char& key, char value )
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


