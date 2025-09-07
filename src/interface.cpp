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


Interface_class::Interface_class( 	char appid,
									uint8_t sdsid,
									Config_class* cfg,
									Semaphore_class* sem ):
Logfacility_class("SharedData_class" )
{
	this->AppId = appid;
	this->className = Logfacility_class::className;
	this->Sem_p	= sem;
	this->Cfg_p = cfg;
	Setup_SDS( sdsid, cfg->Config.sdskeys[ sdsid ] );


}

Interface_class::~Interface_class()
{
	DESTRUCTOR( className );
}


void Interface_class::Setup_SDS( uint sdsid, key_t key)
{
	Comment(INFO, "allocating shared memory for IPC " + to_string( sdsid ));

	ds 			= *SHM.Get( key );
	ds.Id		= sdsid;
	this->addr	= ( interface_t* ) ds.addr;
	Eventque.setup( addr );

	SHM.ShowDs(ds);

	if ( not ds.eexist )
	{
		Comment(WARN, "initializing data interface using default values ");
		Reset_ifd(  );
	}
	Comment( INFO, "check shared memory version");

	dumpFile = Cfg_p->fs.ifd_file + to_string( sdsid) + to_string( ifd_data.version ) ;
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
}





void Interface_class::Write_arr( const wd_arr_t& arr )
{
	for( buffer_t n = 0 ; n < wavedisplay_len; n++ )
		addr->wavedata[n] = arr[n] ;
}
void Interface_class::Write_str(const char selector, const string str )
{
	if (reject( addr->Composer, AppId )) return;

	if ( addr->Comstack == RUNNING )
		addr->UpdateFlag = true;
	string wrt = str.substr(0, str_buffer_len );


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




void Interface_class::Reset_ifd(  )
{
	// copy default values into sds memory

	Comment(INFO, "Reset shared data");
	memcpy( addr	, &ifd_data		, sizeof( interface_t ) );
//	Dump_ifd();
}

bool Interface_class::Restore_ifd()
{
	// copy dump file data into memory

	Comment(INFO,"Restore shared data from file");
	assert( dumpFile.size() > 0 );

	process_arr_t procarr 	= addr->process_arr; 	// let proc register untouched
	int sdsid 				= addr->config;
	bool keyboard			= addr->Keyboard;

	FILE* fd = fopen( dumpFile.data() , "r");
	if ( not fd )
		return false;
	uint size = fread( addr, sizeof( ifd_data ), 1, fd);
	fclose( fd );

	addr->process_arr 	= procarr;
	addr->config		= sdsid;
	addr->Keyboard		= keyboard;
	Eventque.reset();
	return ( size == sizeof( ifd_data ));
}

void Interface_class::Dump_ifd()
{
	// copy shared memory data to dumpfile

	Comment(INFO,"Dump shared data to file \n" + dumpFile) ;
	assert( dumpFile.size() > 0 );
	Eventque.reset();
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


void Interface_class::Commit()
{
	addr->UpdateFlag = true;
	if ( Sem_p->Getval( PROCESSOR_WAIT, GETVAL ) > 0 )
		Sem_p->Release( PROCESSOR_WAIT );
}

bool Interface_class::reject(char status, int id )
{
	if (( status == RUNNING ) and ( id != COMPID ))
	{

		return true;
	}
	else
	{
		return false;
	}
};
void Interface_class::Event( uint8_t event )
{
	Eventque.add( event );
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
	eventptr 		= EventPtr_struct();
	addr->eventptr 	= eventptr;
	std::ranges::for_each( addr->deque, [](uint8_t& element ){ element = NULLKEY  ;});
}
void EventQue_class::add( uint8_t event )
{
	if ( event == NULLKEY )
		return;
	eventptr = addr->eventptr;

	if ( eventptr.length >= MAXQUESIZE )
	{
		Comment( ERROR, "Event que overflow");
		return;
	}

	addr->deque[eventptr.last] 	= event;
	eventptr.last 				= ( eventptr.last + 1 ) % MAXQUESIZE;
	eventptr.length 			+= 1;
	addr->eventptr 				= eventptr;
}

uint8_t EventQue_class::get()
{
	eventptr 					= addr->eventptr;
	if ( eventptr.length == 0 )
		return NULLKEY;
	uint8_t value 				= addr->deque[eventptr.first];
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



