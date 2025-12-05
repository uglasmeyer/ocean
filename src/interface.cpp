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
 * GUIinterface.cpp
 *
 *  Created on: Jan 14, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <data/Interface.h>

appstate_arr_t initAppstate_arr()
{
	appstate_arr_t arr {};
	for( APPID appid : AppIds )
	{
		arr[appid] = appstate_struct();
		arr[appid].type = appid; // Indexer complains
	}
	arr[SYNTHID].state = StateId_t::DEFAULT;
	return arr;
};

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

void Interface_class::selfTest( )
{
	OSCID_e oscid = OSCID_e::OSCID;
	Assert_lt( addr->connect_arr[oscid].frq, OSCID_e::NOOSCID, "connect_arr" );
	APPID appid = APPID::AUDIOID;
	APPID value = addr->appstate_arr[appid].type; // @suppress("Field cannot be resolved")
	Assert_equal( value ,  appid );
	uint16_t record = addr->WD_status.direction;
	Assert_equal( record, (uint16_t)0, "Wd_status.record" );
}

Interface_class::Interface_class( 	APPID appid,
									Id_t sdsid,
									Config_class* cfg,
									Semaphore_class* sem ):
Logfacility_class("SharedData_class" )
{

	this->AppId = appid;
	this->className = Logfacility_class::className;
	this->Sem_p		= sem;
	this->Cfg_p 	= cfg;
	this->fs		= cfg->fs;
	this->dumpFile 	= fs->ifd_file + to_string( sdsid) + to_string( ifd_data.version ) ;

	Setup_SDS		( sdsid, cfg->Config.sdskeys[ sdsid ] );
}



Interface_class::~Interface_class()
{
	DESTRUCTOR( className );
}


void Interface_class::Setup_SDS( Id_t sdsid, key_t key)
{
	Comment(INFO, "allocating shared memory for IPC " + to_string( sdsid ));

	ds 					= *SHM.Get( key );
	ds.Id				= sdsid;
	this->addr			= ( interface_t* ) ds.addr;
	this->addr->SDS_Id	= sdsid;
	Eventque.setup( addr );

	SHM.ShowDs(ds);

	if ( not ds.eexist )
	{
		Comment(WARN, "initializing data interface using default values ");
		Reset_ifd( );
	}
	Comment( DEBUG, "check shared memory version");

	filesystem::path sds_dump = dumpFile;
	if (( filesystem::exists( sds_dump )))
	{
		size_t fsize = filesystem::file_size( sds_dump );
		if ( fsize != sds_size)
		{
			Exception( 	"sds dump size " + to_string( fsize ) +
						" differs in sizeof sds" +
						to_string( sds_size ));

		}
	}
	if (( ifd_data.version == addr->version ))
	{
		Comment( DEBUG, "OK");
	}
	else
	{
		Comment( ERROR, "Setup SDS failed");
		Exception( "IPC version " + to_string( ifd_data.version ) +
				" differs from BIN version " + to_string( addr->version )  +
				" or lib/ifd_data.bin size ");
	}
	ds.eexist = true;
}

void Interface_class::Remove_dumpFile()
{
	Remove_file( dumpFile );
}
void Interface_class::Copy_dumpFileTo( string dst )
{
	filesystem::copy( 	dumpFile ,
						dst,
						filesystem::copy_options::overwrite_existing);
}
void Interface_class::Copy_dumpFileFrom( string src )
{
	filesystem::copy( 	src ,
						dumpFile,
						filesystem::copy_options::overwrite_existing);
}
void Interface_class::Delete_Shm()
{
	SHM.Delete();
}
bool Interface_class::Datasegment_exists()
{
	return ds.eexist;
}

void Interface_class::Write_arr( const wd_arr_t& arr )
{
	for( buffer_t n = 0 ; n < wavedisplay_len; n++ )
		addr->wavedata[n] = arr[n] ;
}
#include <cstring>
void Interface_class::Write_str(const char selector, const string str )
{
	if (reject( AppId )) return;

	if ( addr->appstate_arr[ SDSVIEWID ].state == RUNNING )
		addr->UpdateFlag = true;
	const string wrt = str.substr(0, SDSSTR_SIZE-1 );


	switch ( selector )
	{
		case INSTRUMENTSTR_KEY :
		{
			strcpy( addr->Instrument, wrt.data());
			break;
		}
		case NOTESSTR_KEY :
		{
			strcpy( addr->Notes, wrt.data() );
			break;
		}
		case OTHERSTR_KEY :
		{
			strcpy( addr->Other, wrt.data() );
			break;
		}
		case UPDATELOG_EVENT :
		{
			strcpy( addr->eventstr, str.data() );
			break;
		}
		default:
		{
			strcpy( addr->Other, wrt.data() );
			break;
		}
	}

}


string Interface_class::Read_str( EVENTKEY_e selector )
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
		case UPDATELOG_EVENT :
		{
			str.assign( addr->eventstr );
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


void Interface_class::Reset_ifd()
{
	// copy default values into sds memory

	Comment(INFO, "Reset shared data");
	memcpy( addr	, &ifd_data		, sizeof( interface_t ) );
	addr->appstate_arr	= initAppstate_arr();
	addr->connect_arr	= initConnect_arr();
	addr->StA_state_arr[ STA_INSTRUMENT ].play = true;
	Interface_class::selfTest();

	Dump_ifd( );
}

bool Interface_class::Restore_ifd()
{
	// copy dump file data into memory

	Comment(INFO,"Restore shared data from file");
	assert( dumpFile.size() > 0 );

	int 			sdsid 		= addr->config;
	appstate_arr_t 	appstate_arr= addr->appstate_arr;
	Assert_lt( appstate_arr[0].state, NOSTATE );

	bool ret = loadData( dumpFile, addr, sizeof( ifd_data ) );

	addr->config				= sdsid;
	addr->appstate_arr			= appstate_arr;
	Eventque.reset();
	return ret;//( size == sizeof( ifd_data ));
}

void Interface_class::Dump_ifd()
{
	// copy shared memory data to dumpfile

	Comment(INFO,"Dump SDS  data to file", dumpFile) ;

	Eventque.reset();
	if ( not dumpData( dumpFile, addr, sizeof( ifd_data ) ) )
		Comment( ERROR, "failed" );
}


void Interface_class::Commit()
{
	addr->UpdateFlag = true;
	if ( Sem_p->Getval( PROCESSOR_WAIT, GETVAL ) > 0 )
		Sem_p->Release( PROCESSOR_WAIT );
}

bool Interface_class::reject( APPID appid )
{
	StateId_t composer_state = addr->appstate_arr[COMPID].state;
	if (( composer_state == RUNNING ) and ( appid != COMPID ))
	{
		return true;
	}
	else
	{
		return false;
	}
};




void Interface_class::Test_interface()
{
	TEST_START( className );
	Eventque.reset();
	for( uint n = 0; n<5; n++ )
	{
		Eventque.add( (EVENTKEY_e)n );
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
	Write_str( INSTRUMENTSTR_KEY, "1234567890123456789012345678901234567890" );
	Assert_equal( SDSSTR_SIZE-1, (uint)Read_str( INSTRUMENTSTR_KEY ).length() );

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
	eventptr 		= EventPtr_struct();
	addr->eventptr 	= eventptr;
	std::ranges::for_each( addr->deque, []( uint8_t& element ){ element = 0  ;});
}
void EventQue_class::add( EVENTKEY_e event )
{
	if ( event == NULLKEY )
		return;
	eventptr = addr->eventptr;

	if ( eventptr.length >= MAXQUESIZE )
	{
		Comment( ERROR, "Event que overflow");
		return;
	}

	addr->deque[eventptr.last] 	= (uint8_t) event;
	eventptr.last 				= ( eventptr.last + 1 ) % MAXQUESIZE;
	eventptr.length 			+= 1;
	addr->eventptr 				= eventptr;
}

EVENTKEY_e EventQue_class::get()
{
	eventptr 					= addr->eventptr;
	if ( eventptr.length == 0 )
		return NULLKEY;
	EVENTKEY_e value 			= (EVENTKEY_e) addr->deque[eventptr.first];
	repeat 						= ( value == prev_event );
	prev_event 					= value;
	eventptr.first 				= ( eventptr.first + 1 ) % MAXQUESIZE;
	eventptr.length 			-= 1;
	addr->eventptr 				= eventptr;
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



