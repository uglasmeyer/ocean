/*
 * GUIinterface.cpp
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */


#include <GUIinterface.h>


GUI_interface_class::GUI_interface_class()
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
		exit(1);
	}
	Waveform_vec = GUIspectrum.Get_waveform_vec();
}

GUI_interface_class::~GUI_interface_class()
{
	Comment(INFO, "detach GUI interface from id: " + to_string( shm_info.id));
	shmdt( shm_info.addr );
}

void GUI_interface_class::show_GUI_interface()
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


	read_str('a');

	cout << setfill( '-') << setw(80) << left << "Shared Data Structure " + Version_str << endl;

	lline( "(M)ain (F)requency:" , addr->Main_Freq );
	rline( "(A)DSR (G)lide freq:" , (int)addr->Soft_freq);
	lline( "(M)ain (A)mplitude:" , (int)addr->Master_Amp );
	rline( "(A)DSR (D)ecay:    " , (int)addr->Main_adsr_attack );
	lline( "Main duration      " , (int)addr->Main_Duration);
	rline( "(A)DSR D(u)ration: " , Bps_string[(int)addr->Main_adsr_bps_id]);
	lline( "(M)ain (W)aveform: " , Waveform_vec[ (int)addr->Main_waveform_id ]);
	rline( "(A)DSR (S)ustain:  " , (int)addr->Main_adsr_decay );
	cout << endl;
	lline( "(F)MO  (F)requency:" , addr->FMO_Freq);
	rline( "(V)CO  (F)requency:" , addr->VCO_Freq);
	lline( "(F)MO  (A)mplitude:" , (int)addr->FMO_Amp);
	rline( "(V)CO  (A)mplitude:" , (int) addr->VCO_Amp);
	lline( "(F)MO  (W)aveform: " , Waveform_vec[ (int)addr->FMO_waveform_id ]);
	rline( "(V)CO  (W)aveform: " , Waveform_vec[ (int)addr->VCO_waveform_id ]);
	lline( "." , '.');
	rline( "VCO  PMW dial      " , (int)addr->PMW_dial) ;
	lline( "Mixer Volume:      " , (int)addr->MIX_Amp );
	rline( "Mixer Id           " , (int)addr->MIX_Id );
	lline( "Spectrum value:    " , (int)addr->Spectrum_value );
	rline( "Spectrum channel   " , (int)addr->Spectrum_channel );
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

	if ( addr->Comstack == RUNNING )
	{
		addr->UpdateFlag = false;
	}

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


void GUI_interface_class::write_str(const char selector, const string str )
{
	if (reject( addr->Composer, client_id )) return;

	if ( addr->Comstack != RUNNING )
		addr->UpdateFlag = true;

	switch ( selector )
	{
		case 'i' :
		{
			strcpy( addr->Instrument, str.data());
			break;
		}
		case 'n' :
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

string GUI_interface_class::read_str( char selector )
{
	string str;
	switch ( selector )
	{
		case 'i' :
		{
			str.assign( addr->Instrument );
			break;
		}
		case 'n' :
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


void* GUI_interface_class::buffer( buffer_t shm_size, key_t shm_key )
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

void GUI_interface_class::announce( string client, bool flag )
{
	auto set = [flag, this](auto& a , auto id){
		if (flag)
			a = RUNNING;
		else
			a = NOCONTROL ;
		this->commit();
		return id;
	};

	if ( client.compare("Composer") 	== 0 ) client_id = set( addr->Composer		, 1 );
	if ( client.compare("Synthesizer") 	== 0 ) client_id = set( addr->Synthesizer	, 2 );
	if ( client.compare("AudioServer") 	== 0 ) client_id = set( addr->AudioServer	, 3 );
	if ( client.compare("SndlabGUI") 	== 0 ) client_id = set( addr->UserInterface	, 4 );
	if ( client.compare("Comstack") 	== 0 ) client_id = set( addr->Comstack		, 5 );

}
void GUI_interface_class::reset_ifd()
{
	Comment(INFO, "Reset shared data");
	memcpy( addr	, &ifd_data		, sizeof( ifd_t ) );
	dump_ifd();

}

bool GUI_interface_class::restore_ifd()
{

	Comment(INFO,"Restore shared data from file");

	FILE* fd = fopen( file_structure().ifd_file.data() , "r");
	if ( not fd )
		return false;
	uint size = fread( addr, sizeof( ifd_data ), 1, fd);
	fclose( fd );

	return ( size == sizeof( ifd_data ));
}

void GUI_interface_class::dump_ifd()
{
	Comment(INFO,"Dump shared data to file");
	FILE* fd = fopen( file_structure().ifd_file.data() , "w");
	fwrite( addr, sizeof( ifd_data ), 1, fd);
	fclose( fd );
}

void GUI_interface_class::update( char ch )
{
	commit();

	addr->FLAG = ch;
	addr->UserInterface = UPDATEGUI;
}
void GUI_interface_class::commit()
{
	addr->KEY 	= 0;
	addr->FLAG 	= 0;
	addr->UpdateFlag = true;
}

void GUI_interface_class::Set( bool& key, bool value )
{
	if ( reject( addr->Composer, client_id ) ) return;
	key = value;
}

void GUI_interface_class::Set( char& key, char value )
{
	if ( reject( addr->Composer, client_id ) ) return;
	key = value;
}

void GUI_interface_class::Set( uint16_t& key, uint16_t value )
{
	if ( reject( addr->Composer, client_id ) ) return;
	key = value;
}

void GUI_interface_class::Counter_class::setup( uint16_t base, uint8_t bits, uint8_t pos )
{

	this->shift		= bits*pos;
	this->modulo 	= 1 << bits;
	uint16_t arr 	= ( modulo - 1) << shift;
	this->mask 		= 0xFFFF - arr;
	this->value 	= ( base & ~mask ) >> shift ;
}

void GUI_interface_class::Counter_class::setup( uint16_t base, uint8_t bits, uint8_t pos, uint8_t modulo )
{
	setup( base, bits, pos );
	this->modulo	= modulo;
}

uint16_t GUI_interface_class::Counter_class::get_counter( uint16_t base )
{
	value = ( base & ~mask )  >> shift;
	return value % modulo;
}

void GUI_interface_class::Counter_class::set_counter( uint16_t val )
{
	value = ( val ) % modulo;
}

uint16_t GUI_interface_class::Counter_class::inc_counter( uint16_t base )
{
	uint16_t clear = base & mask;
	value = ( value + 1 ) % modulo;
	return clear | (value  << shift );
}

#include <assert.h>
#include <bitset>
void GUI_interface_class::Counter_class::test(  )
{
	uint16_t BASE = 0xCBA1;
	setup( BASE, 4, 1);

	uint16_t a = get_counter( BASE );
	cout << a << endl;
	assert( 0xF % 16 == 0xF );
	cout << "modulo:" << to_string( this->modulo) << endl;

	assert( this->modulo == 16 );
	assert( a  == 0xA );

	BASE = inc_counter( BASE );
	bitset<16> y{ BASE };
	cout << y << endl;
	assert( BASE == 0xCBA1 + 16);

	uint16_t compare = 1;
	setup( BASE, 2, 0);
	for ( int n{0} ; n<6 ; n++ )
	{
		compare = (compare+1) % 4 + 0xCBB0;
		BASE = inc_counter( BASE );
		bitset<16> y{ BASE } , z{compare};
		cout << "y: " << y << " z: " << z << endl;
		assert( BASE == compare );
	}

	set_counter(2);
	Comment(INFO, "Test String OK");
}

