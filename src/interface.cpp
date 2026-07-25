/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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
 * interface.cpp
 *
 *  Created on: Jan 14, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <data/Interface.h>


connect_arr_t initConnect_arr(  )
{
	connect_arr_t arr{};
	for ( OSCID_e oscid : oscIds ) // Indexer complains
	{
		arr[oscid].frq = oscid;
		arr[oscid].vol = oscid;
	}
	return arr;
};

void SharedData_class::selfTest( )
{
	OSCID_e oscid = OSCID_e::OSCID;
	Assert_lt( (int)addr->connect_arr[oscid].frq, (int)OSCID_e::NOOSCID, "connect_arr" );

	APPID_e appid = APPID_e::AUDIOID;
	APPID_e value = addr->appstate_arr[appid].type;
	Assert_equal( value ,  appid );

	Direction_e record = addr->WD_state.direction;
	Assert_equal( record, wavedisplay_struct::NO_direction, "Wd_status.record" );
}

/**************************************************
 * SharedData_class
 *************************************************/
SharedData_class::SharedData_class( APPID_e			appid,
									Id_t			sdsid,
									Config_class*	cfg,
									Semaphore_class* sem ):
	Logfacility_class	("SharedData_class" ),
	sds_key				( cfg->Config.sdskeys[ sdsid ] ),
	sds_size			( sizeof(interface_struct) ),
	SHM					( sds_size, sds_size, sds_key ),
	Eventque			( SHM.shm_ds.addr )
{

	this->AppId 		= appid;
	this->Sem_p			= sem;
	this->Cfg_p 		= cfg;
	this->fs			= cfg->fs;
	this->ifd_data		= interface_struct();
	this->ifd_data.SDS_Id = sdsid;
	this->shm_ds		= SHM.shm_ds;
	this->addr			= ( interface_t* ) SHM.shm_ds.addr;
	this->addr->SDS_Id	= sdsid;
	this->shm_ds.Id		= sdsid;

	set_dumpFile		( fs->vardir, sdsid );
	init_SDS			();
}

SharedData_class::~SharedData_class()
{
	DESTRUCTOR( className );
}

void SharedData_class::set_dumpFile( string dir, uint8_t sdsid )
{
	string 	subdir	= dir + "SDS_"	+ to_string( sdsid ) + "/";
	if( not filesystem::exists( subdir ) )
	{
		filesystem::create_directories( subdir );
	}
	this->dumpFile	= subdir + filename ;
}


void SharedData_class::init_SDS()
{
	if ( not shm_ds.eexist )
	{
		Comment(WARN, "initializing data interface using default values ");
		Reset_ifd( );
	}

	Comment( DEBUG, "check shared memory version");
	if (( ifd_data.version == addr->version ) )
	{
		Comment( DEBUG, "OK");
	}
	else
	{
		Comment( ERROR, "SDS content differs");
		Exception( 	"IPC version " + to_string( ifd_data.version ) +
					" is different to SDS version " + to_string( addr->version ) );
	}

	filesystem::path sds_dump_path = dumpFile;
	if (( filesystem::exists( sds_dump_path )))
	{
		size_t fsize = filesystem::file_size( sds_dump_path );
		if ( fsize == sds_size)
		{
			;
		}
		else
		{
			Comment	( INFO, "sds dump size ", fsize, "differs in sizeof sds", sds_size );
			Remove_dumpFile();
		}
	}
	shm_ds.eexist	= true;
	SHM.ShowDs		( shm_ds );
	Assert_equal( (int)this->shm_ds.Id, (int)this->addr->SDS_Id );
}

void SharedData_class::Remove_dumpFile()
{
	Info( "removing dump file", dumpFile );
	Remove_file( dumpFile );
}
void SharedData_class::Copy_dumpFileTo( string dst )
{
	filesystem::copy( 	dumpFile ,
						dst,
						filesystem::copy_options::overwrite_existing);
}
void SharedData_class::Copy_dumpFileFrom( string src )
{
	filesystem::copy( 	src ,
						dumpFile,
						filesystem::copy_options::overwrite_existing);
}
void SharedData_class::Delete_Shm()
{
	SHM.Delete();
}
bool SharedData_class::Datasegment_exists()
{
	return shm_ds.eexist;
}

#include <cstring>
char* SharedData_class::Write_str( CSTRKEY_e selector, const string str )
{
	if (reject( AppId )) return nullptr;

//	cannot access sds_master
//	if ( addr->appstate_arr[ SDSVIEWID ].state == StateId_e::RUNNING )
//		addr->UpdateFlag = true;

	char* char_p = nullptr;
	size_t size = 0;
	switch ( selector )
	{
		case INSTRUMENTSTR :
		{
			size = SDSNAME_SIZE;
			char_p = addr->Instrument;
			break;
		}
		case NOTENAMESTR :
		{
			size = SDSNAME_SIZE;
			char_p =addr->Notes;
			break;
		}
		case SESSIONSTR :
		{
			size = SDSNAME_SIZE;
			char_p = addr->Session;
			break;
		}
		case WAVFILE_STR :
		{
			size = SDSNAME_SIZE;
			char_p = addr->WAV_filename;
			break;
		}
		case UPDATELOG_STR :
		{
			size = SDSLONG_SIZE;
			char_p = addr->eventstr;
			break;
		}
		case NOTELINESTR :
		{
			size = SDSLONG_SIZE;
			char_p = addr->Noteline;
			break;
		}
		case RHYTHMLINESTR :
		{
			size = SDSLONG_SIZE;
			char_p = addr->Rythmline;
			break;
		}
		default:
		{
			Exception( "unhandled str id: " + to_string( int( selector )) );
			break;
		}

	} // switch
	const string wrt = str.substr(0, size-1 );
	strcpy( char_p, wrt.data() );

	return char_p;
}


string SharedData_class::Read_str( CSTRKEY_e selector )
{
	string str;
	switch ( selector )
	{
		case INSTRUMENTSTR :
		{
			str.assign( addr->Instrument );
			break;
		}
		case NOTENAMESTR :
		{
			str.assign( addr->Notes );
			break;
		}
		case UPDATELOG_STR :
		{
			str.assign( addr->eventstr );
			break;
		}
		case SESSIONSTR :
		{
			str.assign( addr->Session );
			break;
		}
		case WAVFILE_STR :
		{
			str.assign( addr->WAV_filename );
			break;
		}
		default :
		{
			Exception( "unknown key: " + to_string( int( selector ) ) );
			break;
		}

	}

	return str;
}

constexpr appstate_arr_t initAppstate_arr()
{
	appstate_arr_t arr { appstate_data() };
	for( APPID_e appid : AppMap_struct().AppIds )
	{
		arr[appid].type = appid;
	}
	return arr;
};

void SharedData_class::Reset_ifd()
{	// copy default values into sds memory

	Comment(INFO, "Reset shared data");
	*addr = ifd_data;
	addr->connect_arr = initConnect_arr();
	addr->StA_state_arr[ STA_INSTRUMENT ].play = true;
	addr->appstate_arr = initAppstate_arr();
}

bool SharedData_class::Load_ifd()
{
	Comment(INFO,"load SDS data from file");

	bool	ret = loadData( dumpFile, addr, sizeof( ifd_data ) );
	return	ret;
}
bool SharedData_class::Restore_ifd()
{
	// copy dump file data into share data segment

	Comment(INFO,"Restore shared data from file");
	assert( dumpFile.size() > 0 );

	Id_t 			config 		= addr->comId;
	uint8_t 		SHMID		= addr->SHMID;
	appstate_arr_t 	appstate_arr= addr->appstate_arr;
	KEYCODE			sdsview_page= addr->sdsview_page;
	Id_t			sdsview_sdsid=addr->sdsview_sdsid;

	Assert_lt( appstate_arr[0].state, StateId_e::STATEID_SIZE );

	bool ret = Load_ifd();

	addr->comId					= config;
	addr->SHMID					= SHMID;
	addr->appstate_arr			= appstate_arr;
	addr->sdsview_page			= sdsview_page;
	addr->sdsview_sdsid			= sdsview_sdsid;
	Eventque.Reset();
	return ret;
}

void SharedData_class::Prepare_sds()
{
	// switch addr->NotesTypeId because of incomplet commit
	switch (addr->NotesTypeId)
	{
		case BUSY_XML : { addr->NotesTypeId = XML_ID; break; }
		case BUSY_NTE : { addr->NotesTypeId = NTE_ID; break; }
		default : { break; }
	}

	addr->Capture_state	= StateId_e::INACTIVE;
	if( addr->WD_state.mode == WdModeID_t::RANGE_WD ) // ensure consistency
		addr->WD_state.mode = WdModeID_t::FULL_WD;

	if( ifd_data.SDS_Id == MASTER_SDSID )
	{
		if ( addr->Master_Amp == 0 )
			addr->Master_Amp = osc_default_volume;
	}
}
void SharedData_class::Dump_ifd()
{
	// copy shared memory data to dumpfile

	Comment(INFO,"Dump SDS data   to file", filename) ;

	Eventque.Reset();
	bool success = dumpData( dumpFile, addr, sizeof( ifd_data ) );
	if ( success ) return;
	Comment( ERROR, "writing", sizeof( ifd_data ), " bytes to directory", dumpFile );
}


void SharedData_class::Commit()
{
	addr->UpdateFlag = true;
//	if ( Sem_p->Getval( PROCESSOR_WAIT, GETVAL ) > 0 )
	Sem_p->Release( PROCESSOR_WAIT );
}

bool SharedData_class::reject( APPID_e appid )
{
	StateId_e composer_state = addr->appstate_arr[COMPOSERID].state;
	if (( composer_state == StateId_e::RUNNING ) and ( appid != COMPOSERID ))
	{
		return true;
	}
	else
	{
		return false;
	}
};




void SharedData_class::Test_interface()
{
	TEST_START( className );
	Eventque.Reset();
	for( uint n = 0; n<5; n++ )
	{
		Eventque.Add( (EVENTKEY_e)n );
	}
	cout << Eventque.Show() << endl;
	uint8_t value = Eventque.Get();
	ASSERTION( value == 1, "eventque.get()", value, 1 );
	cout << Eventque.Show()<< endl;
	cout << dec << (int)Eventque.Get() << endl;
	cout << Eventque.Show()<< endl;
	cout 	<< " length: " << dec << (int)addr->eventptr.length << ":"
			<< " first : " << (int) addr->eventptr.first
			<< " last  : " << (int) addr->eventptr.last << endl;

	string istr = "1234567890123456789012345678901234567890";
	Write_str( INSTRUMENTSTR, istr );
	string rstr = Read_str( INSTRUMENTSTR );
	Assert_equal( SDSNAME_SIZE-1, (uint)rstr.length() );
	cstring_T<cstring_name>	cstr{ &addr->Instrument, SDSNAME_SIZE };
	string str = cstr.read();
	Assert_equal( rstr, str );

	TEST_END( className );
}

/**************************************************
 * EventQue_class
 *************************************************/
EventQue_class::EventQue_class( void* _sds )
	: Logfacility_class	( "EventQue_class" )
//	, eventptr			()
{
	this->sds		= ( interface_t* )_sds;
	this->Repeat	= false;
	this->prev_event= NULLKEY;
	this->eventptr	= sds->eventptr;
}
void EventQue_class::Reset()
{
	eventptr		= EventPtr_data();
	sds->eventptr 	= eventptr;
	std::ranges::for_each( sds->deque, []( EVENTKEY_e& key )
		{ key = NULLKEY  ;});
}
void EventQue_class::Add( EVENTKEY_e event )
{
	if ( event == NULLKEY )
		return;
	eventptr = sds->eventptr;

	if ( eventptr.length >= MAXQUESIZE )
	{
		Comment( ERROR, "Event que overflow");
		return;
	}

	sds->deque[eventptr.last] 	= event;
	eventptr.last 				= ( eventptr.last + 1 ) % MAXQUESIZE;
	eventptr.length 			+= 1;
	sds->eventptr 				= eventptr;
}

EVENTKEY_e EventQue_class::Get()
{
	eventptr 					= sds->eventptr;
	if ( eventptr.length == 0 )
		return NULLKEY;
	EVENTKEY_e value 			= (EVENTKEY_e) sds->deque[eventptr.first];
//	Repeat 						= ( value == prev_event );
	prev_event 					= value;
	eventptr.first 				= ( eventptr.first + 1 ) % MAXQUESIZE;
	eventptr.length 			-= 1;
	sds->eventptr 				= eventptr;
	return value;
}

string EventQue_class::Show()
{
	return show_items( sds->deque );
}



