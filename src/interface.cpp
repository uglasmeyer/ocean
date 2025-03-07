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
}

Interface_class::~Interface_class()
{
	//SHM.Detach( ds.addr );
}


void Interface_class::Setup_SDS( uint sdsid, key_t key)
{
	Comment(INFO, "allocating shared memory for IPC " + to_string( sdsid ));

	ds 		= *SHM.Get( key );
	ds.Id	= sdsid;
	this->addr = ( interface_t* ) ds.addr;
	Eventque.setup(addr);
	SHM.ShowDs(ds);
	dumpFile = file_structure().ifd_file + to_string( sdsid) ;


	if ( not ds.eexist )
	{
		Comment(WARN, "initializing data interface using default values ");
		Reset_ifd(  );
	}
	Comment( INFO, "check shared memory version");

	filesystem::path sds_dump = dumpFile;
	if (( filesystem::exists( sds_dump )))
	{
		size_t fsize = filesystem::file_size( sds_dump );
		if ( fsize != sds_size)
		{
			EXCEPTION( 	"sds dump size " + to_string( fsize ) +
						" differs in sizeof sds" +
						to_string( sds_size ));

		}
	}
	if (( ifd_data.version == addr->version ))
	{
		Comment( INFO, "OK");
	}
	else
	{
		Comment( ERROR, "Setup SDS failed");
		EXCEPTION( "IPC version " + to_string( ifd_data.version ) +
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
	state_map[RECORDSTART] 	= "start recording";
	state_map[RECORDSTOP] 	= "stop recording";

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
					"," + conv_bool_s(addr->mixer_status.sync) +
					"," + conv_bool_s(addr->mixer_status.mute) +
					"," + conv_bool_s(addr->mixer_status.kbd) +
					"," + conv_bool_s(addr->mixer_status.instrument );

	stringstream status2 {""};
	uint arrno = 0;
	for( StA_status_t status : addr->StA_state )
	{
		status2 << to_string(arrno) << ":" << to_string(status.play) << to_string(status.store) << "  ";
		arrno++;
	}

	stringstream status3{""};
	arrno = 0;
	for( uint8_t amp : addr->StA_amp_arr )
	{
		status3 << to_string(arrno) << ":" << setw(4) << left << to_string( amp) ;
		arrno++;
	}


	lline( "\nShared Data Str. ID ", to_string((int) ds.Id ));
	rline( Version_str 			, addr->version);

	lline( "(M)ain (F)requency:" , Frequency.Calc( addr->OSC_wp.frqidx ));
	rline( "(A)DSR (G)lide freq:", (int)addr->OSC_wp.glide_effect);

	lline( "(M)aster(A)mplitude:", (int)addr->Master_Amp );
	rline( "(A)DSR (A)ttack:   " , (int)addr->OSC_adsr.attack );

	lline( "                   " , 0 );
	rline( "(A)DSR (B)eats Id  " , (int)addr->OSC_adsr.bps) ;

	lline( "(M)ain (W)aveform: " , waveform_str_vec[ (int)addr->OSC_spectrum.wfid[0] ]);
	rline( "(A)DSR (D)ecay:    " , (int)addr->OSC_adsr.decay );

	lline( "(F)MO  (F)requency:" , Frequency.Calc( addr->FMO_wp.frqidx ) );
	rline( "(V)CO  (F)requency:" , Frequency.Calc( addr->VCO_wp.frqidx ) );

	lline( "(F)MO  (A)mplitude:" , (int)addr->FMO_wp.volume);
	rline( "(V)CO  (A)mplitude:" , (int)addr->VCO_wp.volume);

	lline( "(F)MO  (W)aveform: " , waveform_str_vec[ (int)addr->FMO_spectrum.wfid[0] ]);
	rline( "(V)CO  (W)aveform: " , waveform_str_vec[ (int)addr->VCO_spectrum.wfid[0] ]);

	lline( "", "" );
	rline( "VCO  PMW dial      " , (int)addr->VCO_wp.PMW_dial) ;

	rline( "Spectrum volume    " , Spectrum.Show_spectrum_type( "SPEV", addr->OSC_spectrum ));
	rline( "Spectrum frequency " , Spectrum.Show_spectrum_type( "SPEF", addr->OSC_spectrum ));
	rline( "Spectrum wafeform  " , Spectrum.Show_spectrum_type( "SPEW", addr->OSC_spectrum ));

	rline( "Spectrum volume    " , Spectrum.Show_spectrum_type( "SPEV", addr->VCO_spectrum ));
	rline( "Spectrum frequency " , Spectrum.Show_spectrum_type( "SPEF", addr->VCO_spectrum ));
	rline( "Spectrum wafeform  " , Spectrum.Show_spectrum_type( "SPEW", addr->VCO_spectrum ));

	rline( "Spectrum volume    " , Spectrum.Show_spectrum_type( "SPEV", addr->FMO_spectrum ));
	rline( "Spectrum frequency " , Spectrum.Show_spectrum_type( "SPEF", addr->FMO_spectrum ));
	rline( "Spectrum wafeform  " , Spectrum.Show_spectrum_type( "SPEW", addr->FMO_spectrum ));


	lline( "Mixer Volume:      " , (int)addr->MIX_Amp );
	rline( "Mixer Id           " , (int)addr->MIX_Id );

	lline( "Sync data id       " , (int) addr->SHMID);
	rline( "Event ID           " , Eventque.show());

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

	lline( "Notes             :" , addr->Notes + NotesExtension[ addr->NotestypeId ] );
	rline( "Noteline duration :" , (int) addr->Noteline_sec);

	Lline( "Status Extr,Note,Sync,Mute,Kbd,Inst:" , status1 );
	rline( "StA #:Amp         :" , status3.str() );
	rline( "Status StA #:ps,  :" , status2.str() );


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
	Comment(INFO, "announcing application " + Type_map( this->Type_Id ) );
	uint8_t* state = Getstate_ptr( Type_Id );
	*state = RUNNING;
	addr->UpdateFlag = true;
}

void Interface_class::Reset_ifd(  )
{
	Comment(INFO, "Reset shared data");
	// copy ifd data into shared memory
	memcpy( addr	, &ifd_data		, sizeof( interface_t ) );
	Dump_ifd();
}

bool Interface_class::Restore_ifd()
{

	Comment(INFO,"Restore shared data from file");
	assert( dumpFile.size() > 0 );

	process_arr_t procarr 	= addr->process_arr; 	// let proc register untouched
	int sdsid 				= addr->config;

	FILE* fd = fopen( dumpFile.data() , "r");
	if ( not fd )
		return false;
	uint size = fread( addr, sizeof( ifd_data ), 1, fd);
	fclose( fd );

	addr->process_arr 	= procarr;
	addr->config		= sdsid;
	return ( size == sizeof( ifd_data ));
}

void Interface_class::Dump_ifd()
{
	Comment(INFO,"Dump shared data to file \n" + dumpFile) ;
	assert( dumpFile.size() > 0 );

	size_t count = 0;
	FILE* fd = fopen( dumpFile.data() , "w");
	if ( fd )
	{
		count = fwrite( addr, sizeof( ifd_data ), 1, fd);
		fclose( fd );
	}
	if( count != 1 )
		EXCEPTION( "incomplete dump" + Error_text( errno ) );
}

void Interface_class::Update( char ch )
{
	Commit();

	addr->FLAG = ch;
	addr->UserInterface = UPDATEGUI;
}
void Interface_class::Commit()
{
	addr->FLAG 	= NULLKEY;
	addr->UpdateFlag = true;
	if ( Sem_p->Getval( PROCESSOR_WAIT, GETVAL ) > 0 )
		Sem_p->Release( PROCESSOR_WAIT );
}

void Interface_class::State_pMap( )
{
	assert( addr != nullptr );
	state_p_map[NOID]		= nullptr;
	state_p_map[SYNTHID]	= &addr->Synthesizer,
	state_p_map[COMPID]		= &addr->Composer;
	state_p_map[GUI_ID]		= &addr->UserInterface;
	state_p_map[COMSTACKID]	= &addr->Comstack;
	state_p_map[AUDIOID]	= &addr->AudioServer;
	state_p_map[RTSPID]		= &addr->Rtsp;

}


uint8_t* Interface_class::Getstate_ptr( uint TypeId )
{
	uint8_t* state_p = state_p_map[ TypeId ];
	assert( state_p != nullptr );
	return state_p;
}

void Interface_class::Test_interface()
{
	TEST_START( className );
	Eventque.reset();
	for( uint n = 0; n<10; n++ )
	{
		Eventque.add( n*n );
	}
	cout << Eventque.show() << endl;
	uint8_t value = Eventque.get();
	ASSERTION( value == 1, "eventque.get()", value, 1 );
	cout << Eventque.show()<< endl;
	cout << dec << (int)Eventque.get() << endl;
	cout << Eventque.show()<< endl;
	cout 	<< " length: " << dec << (int)addr->eventptr.length << ":"
			<< " first : " << (int) addr->eventptr.first
			<< " last  : " << (int) addr->eventptr.last << endl;
	TEST_END( className );
}


/****************
 * EventQue_class
 ***************/

void EventQue_class::setup( interface_t* _addr )
{
	addr = _addr;
	reset();
}
void EventQue_class::reset()
{
	addr->eventptr = EventPtr_struct();
	std::ranges::for_each( addr->deque, [](uint element ){ element = NULLKEY  ;});
}
void EventQue_class::add( uint8_t event )
{
	if ( event == NULLKEY ) return;
	eventptr = addr->eventptr;

	if ( eventptr.length >= MAXQUESIZE ) return;
	addr->deque[eventptr.last] = event;
	eventptr.last = ( eventptr.last + 1 ) % MAXQUESIZE;
	eventptr.length += 1;
	addr->eventptr = eventptr;
}

uint8_t EventQue_class::get()
{
	eventptr = addr->eventptr;
	if ( eventptr.length == 0 ) return NULLKEY;
	uint8_t value = addr->deque[eventptr.first];
	eventptr.first = ( eventptr.first + 1 ) % MAXQUESIZE;
	eventptr.length -= 1;
	addr->eventptr = eventptr;
	return value;
}

string EventQue_class::show()
{
	stringstream strs{};
	eventptr = addr->eventptr;
	uint index = eventptr.first;
	for( uint n = 0 ; n < eventptr.length; n++ )
	{
		strs << dec << (int) addr->deque[ index ] << ":";
		index = ( index + 1 ) % MAXQUESIZE;
	}
	return strs.str();
}



