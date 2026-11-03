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
 * mixer.cpp
 *
 *  Created on: Dec 31, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Mixer.h>
#include <Wavedisplay_base.h>

/**************************************************
 * StorageArea_class
 *************************************************/

StorageArea_class::StorageArea_class( interface_t* sds, Config_class* cfg, Wavedisplay_class* wd ) :
	Logfacility_class("StA_class")
{
	this->Role_map		= sta_role_map();
	this->sds_p			= sds;
	this->SDS_ID		= sds_p->SDS_Id;
	this->Cfg_p			= cfg;
	this->Wd_p			= wd;
	string 		sta_dir	= Cfg_p->fs->vardir + "SDS_"	+ to_string( SDS_ID ) + "/";
	StA_param_t usr_conf( "", Cfg_p->Config.temp_sec	, sta_dir );
	StA_param_t ist_conf( "", Cfg_p->Config.kbd_sec		, sta_dir );
	StA_param_t ext_conf( "", Cfg_p->Config.record_sec	, sta_dir );
	StA_param_t kbd_conf( "", Cfg_p->Config.kbd_sec		, sta_dir );
	StA_param_t nte_conf( "", Cfg_p->Config.kbd_sec		, sta_dir );

	StAId_e id	= StAId_e( 0 );
	for( StA_param_t param : { 	usr_conf, usr_conf, usr_conf, usr_conf,
								ist_conf, kbd_conf, nte_conf, ext_conf }  )
	{
		param.name		= Role_map.GetName( id );
		Storage_class	sta { id, param };
		sta.Set_filename( sds_p->Session );
		restoreStA		( sta ); // on startup overwrite
		Array.push_back	( sta );

		id++;
	}

	for( StAId_e staid : WdIds )
	{
		RoleId_e sta_role = Role_map.GetRoleid( staid );
		Wd_p->Add_role_ptr( sta_role , Array[ staid ].Data, &Array[ staid ].param.wdsize );
	}

};
StorageArea_class::~StorageArea_class()
{
	DESTRUCTOR( className );
};
void StorageArea_class::StA_Wdcursor( RoleId_e roleid )
{

	StAId_e 	staid = Role_map.GetStaid( roleid );
	if ( staid < STAID_ALL )
	{
		buffer_t 	rpos = Array[ staid ].scanner.rpos;
		Wd_p->Set_wdcursor( rpos, 1 );
		Comment( Logmask(DEBUG), "Set_Wdcursor", (int) staid, rpos );
	}
}

void StorageArea_class::DumpStA( Storage_class& sta )
{
	auto is_zero = [ sta ]( buffer_t bytes )
	{
		for( buffer_t n = 0; n < bytes; n++ )
		{
			if ( abs( sta.Data[n] ) > 0 )
				return false;
		}
		return true; // true if sta content is not zero
	};

	if( sta.Id == STA_NOTES )	return;

	// copy StA memory to file
	if ( not is_zero( sta.scanner.fillrange.max ) )
	{
		buffer_t 	bytes2write = sta.scanner.fillrange.max * sizeof_Data;
		bool success = dumpData( sta.file, sta.Data, bytes2write );
		if ( success )
			return;
		string txt = "writing " + to_string( bytes2write ) + " bytes to directory " + sta.filename ;
		Comment( ERROR, txt );
	}
	else
	{
		Info( "empty data not dumped to", sta.filename );
	}
}
void StorageArea_class::ResetStA( const StAId_e& staid )
{
	Comment( INFO, "Reset SDS state" );
	if ( staid == STAID_ALL )
	{
		for_each( Array,
				[ this ]( Storage_class& sta )
				{
					sta.Reset();
				} );
	}
	else
	{
		Array[staid].Reset();
	}
	set_sds_values( staid );
}
void StorageArea_class::set_sds_values( StAId_e staid )
{
	sds_p->StA_state_arr[ staid ] = Array[ staid ].state.Get();
}
void StorageArea_class::SetStA_Files( string subdir )
{

	for( StAId_e staid : StAIds)
	{

		if ( Array[ staid ].Set_filename( subdir ) ) 	// do not overwrite current content
													// if subdir is unchanged
			restoreStA( Array[ staid ] );
	}
}

void StorageArea_class::setStAs()
{
	for ( StAId_e staid : StAIds )
	{
		SetStAProperties( staid );
	}
}

void StorageArea_class::SetStAProperties( StAId_e staId )
{	// distribution of sds_p->StA_state_arr into StA[].state

	auto auto_volume = [ this ]( const StAId_e& id)
	{
		if ( ( sds_p->StA_amp_arr[id].amp == 0 ) and ( not sds_p->mixer_state.composer ) )
		{
			sds_p->StA_amp_arr[id].amp = osc_default_volume;
		}
		Array[id].Volume( sds_p->StA_amp_arr[id].amp, sds_p->StA_amp_arr[id].mode );
	};

	if( staId == STAID_ALL )
	{
		setStAs();
		return;
	}

	Storage_class* sta = &Array[staId];

	sta->state.Set( sds_p->StA_state_arr[staId] );
	if( sta->state.Start() )
	{
		auto_volume( staId );
		sta->scanner.Set_rpos( 0 );
	}
};


void StorageArea_class::RestoreStAs()
{
	for_each( Array,
			[ this ]( Storage_class sta ){ restoreStA( sta ) ;});
}
bool StorageArea_class::restoreStA( Storage_class& sta )
{

	auto set_sta_zeroflag = [ sta ]( buffer_t bytes )
	{
		for( buffer_t n = 0; n < bytes; n++ )
		{
			if ( abs(sta.Data[n]) > 0 )
				return false;
		}
		return true; // true if sta content is not zero
	};

	// copy dump file data into StA memory and set related info (update sds)

	sta.Clear_data					( 0 );
	if( sta.Id == STA_NOTES )	return true;// because the content of array notes is generated

	size_t		bytes_red			= loadData( sta.file, sta.Data, 0 );
	mem_ds_t	mem_ds				= mem_data_struct( sta.mem_ds.block_size, bytes_red );

	sta.Set_records					( mem_ds.max_records );
	sta.is_zero 					= set_sta_zeroflag( sta.mem_ds.data_blocks );
	Info( "Restore StA data from file", sta.file );
	return true;

}


/************************
 *  Mixer_class
 ***********************/

Mixer_class::Mixer_class( 	Dataworld_class* data,
							StorageArea_class* sta )
	: Logfacility_class	( "Mixer_class" )
	, Interface_base	( data )
	, RecMono			( audiobuffer_bytes )
	, Out				( sizeof_Stereo, Stereoaudio_bytes )

{
	Info( "Init", className );

	this->StA			= sta;
	this->beat_clock 	= 0;
	this->composer_cntr	= 0;		// note chunk counter
	this->state			= mixer_state_data();


	initStA_data();

	if( LogMask[ TEST ] )
	{
		for ( uint n : StAIds )
			StA->Array[n].Memory_base::DsInfo();

		RecMono.DsInfo	( "Mono data" );
		Out.DsInfo		( );
	}
};

Mixer_class::~Mixer_class()
{
	if( not LogMask[TEST ] )
	{
		Info( "Using StA dump dirdectory", StA->Array[0].dirname );

		for( Storage_class& sta : StA->Array )
			StA->DumpStA( sta );
	}
	DESTRUCTOR( className );
};

// begin initfunctions
void Mixer_class::initStA_data()
{
	for( StAId_e staid : StAIds )
	{
		// trigger sta to play as before the last shutdown
		sds_p->StA_state_arr[staid].start 	= sds_p->StA_state_arr[staid].play;
		sds_p->StA_state_arr[staid].store 	= false;
		StA->SetStAProperties				( staid );
	}
}

// end initfunctions



bool Mixer_class::BeatClock( const uint8_t& bps )
{
	bool sync_kbd = true;
	beat_clock 	= ( beat_clock + 1 ) % ( measure_parts );
	if ( ( bps > 1 ) and ( bps < measure_parts ))
	{
		sync_kbd = not (( beat_clock ) % ( bps ) );
	}

	// reset audio volume alarm
	if( beat_clock == 0 )
	{
		sds_master->overmodulated	= false;
		sds_master->overload		= false;
	}
//	coutf << boolalpha << sync_kbd  << endl;

	return ( sync_kbd );
}










void Mixer_class::Add_mono( Data_t* deviceData, StAId_e id )
// collect Data from different sound devices OSC or StAs
// by applying dynamic sta volume
// and distribute the data to both stereo channels
{								//U0  U1  U2  U3  In  Kb  Nt  Ex
	if( deviceData == nullptr )
		return;

	auto delete_after_read = [ ]( Data_t* Data, buffer_t frames )
	{
		for( buffer_t n = 0; n < frames; n++ )
		{
			Data[n] =0;
		}
	};

												//+r  -l  -r  +l  In  Ky  No  Ex
	const array<int,StAId_e::STAID_ALL> phase_r = {10,  0,-10,  0,  7, -7,  7, -7 };
	const array<int,StAId_e::STAID_ALL> phase_l = { 0,-10,  0, 10,  7, -7,  7, -7 };

	StA->Array[id].DynVolume.SetDelta( sds_p->StA_amp_arr[id].slide );
	for( buffer_t n = 0; n < audio_frames; n++)
	{
		float	volpercent		 	= StA->Array[id].DynVolume.Get() ;
		float	volpermill			= volpercent * 0.1;
		// collect mono data for AudioServer
		Out.stereo_data[n].left 	+= deviceData[n] * phase_l[id] * volpermill;
		Out.stereo_data[n].right 	+= deviceData[n] * phase_r[id] * volpermill;
		// collect mono data for sta store
		RecMono.Data[n]  			+= rint( deviceData[n] ) * volpercent;
	}
	StA->Array[id].DynVolume.Update();

	if( StA->Array[ id ].state.Forget() )
		delete_after_read( deviceData, audio_frames );

}

void Mixer_class::AddStereo()
// sample Data for different Synthesizer into audio memory
{
	Stereo_t* Data = DaTA_p->GetShm_addr();

	for( buffer_t n = 0; n < audio_frames ; n++ )
	{
		Data[n].left 		+= Out.stereo_data[n].left ;
		Data[n].right	 	+= Out.stereo_data[n].right;
		Out.stereo_data[n]	 = { 0, 0 }; // collected data is no longer needed
	}

}


bool Mixer_class::Cutdesk_isActive()
{
	return ( sds_p->WD_state.mode == WdModeID_t::RANGE_WD );
}

void Mixer_class::Add_Sound( Data_t* 	instrument_Data,
							 Data_t* 	kbd_Data,
							 Data_t* 	notes_Data )
{
	auto adsr_sync_start = [ this  ](  )
	{	// sync play with adsr

		for ( Storage_class& sta : StA->Array )
		{
			if( sta.state.Start() and state.sync_adsr )
			{
				sta.state.Play	( true );
				sta.state.Start	( false );
			}
		}

	};

	auto add_mono = [ this ]( Data_t* Data, StAId_e staid )
	{
		if ( StA->Array[ staid ].state.Play() )
		{
			Add_mono( Data, staid );
		}
	};

	if ( state.mute )
	{
		// stereo is empty at this point
		AddStereo();
		return;
	}

	// read sound from devices and store to RecMono
	adsr_sync_start	();
	add_mono		( instrument_Data	, STA_INSTRUMENT );
	add_mono		( notes_Data		, STA_NOTES );
	add_mono		( kbd_Data			, STA_KEYBOARD );

	// read sound from all RecStAs first, to collect audio data and sta record data
	for ( StAId_e staId : RecIds )
	{
		Storage_class& sta = StA->Array[staId];
		//don't play if record
		if ( sta.state.Play() and ( not sta.state.Store() ) )
		{
			Data_t* StAdata = sta.scanner.Next_read();
			Add_mono( StAdata, staId );
		}
	}

	// write sound to Rec-StAs
	for ( StAId_e staId : RecIds )
	{
		// switch storing according to the current sds value
		// at the beginning of the adsr curve
		if( state.sync_adsr )
			StA->Array[staId].state.Store( sds_p->StA_state_arr[staId].store );

		StA->Array[staId].Store_record( RecMono.Data ); // copy Recmono data to StA
	}
	RecMono.Clear_data( 0 );// no longer needed

	AddStereo(); 	// push sound to audio server

	// update sta state array for all sta's
	for( Storage_class& sta : StA->Array  )
	{
		sta.state.Filled( sta.scanner.Get_filled() );
		sds_p->StA_state_arr[sta.Id] = sta.state.Get();
	}
};

void Mixer_class::TestMixer()
{

	TEST_START( className);

	WfDisplay_que_struct	WfD_Que	{ sds_p, &StA->Role_map };
	WfD_Que.Test();

	uint8_t test_vol = 87;

	StA->Array[STA_NOTES].Volume( test_vol, SLIDE );
	float vol = StA->Array[STA_NOTES].Volume();
	Assert_equal( (int)test_vol, (int)(vol*100), "StA volume" );

	StA->Array[STA_NOTES].Volume( test_vol, FIXED );
	vol = StA->Array[STA_NOTES].Volume();
	Assert_equal( (int)test_vol, (int)(vol*100), "StA volume" );

	StAId_e staid = STA_USER00;
	sds_p->StA_amp_arr[staid].amp = 0;
	sds_p->StA_state_arr[staid].start = true;
	StA->SetStAProperties( staid );
	Assert_equal( sds_p->StA_amp_arr[staid].amp, osc_default_volume );
	// roleid is completely managed by the GUI
//	Assert_equal( sds_p->WD_state.roleId, sta_rolemap.GetRoleid( staid) );

	staid = STA_INSTRUMENT;
	sds_p->StA_amp_arr[staid].amp= 0;
	sds_p->StA_state_arr[staid].start = true;
	StA->SetStAProperties( staid );
	Assert_equal( sds_p->StA_amp_arr[staid].amp, osc_default_volume );
	Assert_equal( sds_p->StA_state_arr[staid].play, true );

	range_T<int> r{10,14};
	Info( range_str( r ) );
	r = { 1 , 12 };
	Info( range_str( r ) );
	Assert_equal( (int)r.len, 11 );
	cursor_T<int> c{0,1,3};
	Assert_equal( c.len, size_t(3) );
	Assert_equal( c.cur, 1 );
	TEST_END( className );
}


/**************************************************
 * CutDesk_class
 *************************************************/

inline int16_t to_cursor( int value, int max )
{
	int	ret 	= 0;
	if (max != 0 )
		ret 	= rint( ( value * wavedisplay_len ) / max );
	return int16_t( ret );
};

auto Update_Sds = [  ]( interface_t* sds_p,
						record_range_t range,
						Scanner_class& scanner  )
{
	//consistently update sds_p->WDstate and StA->scanner
	int	max_records				= sds_p->WD_state.frames / min_frames;

	sds_p->WD_state.direction 	= Direction_e::NO_direction;
	sds_p->WD_state.cursor.min	= to_cursor( range.min, max_records ),
	sds_p->WD_state.cursor.max	= to_cursor( range.max, max_records ) ;

	scanner.fillrange.min		= range.min * min_frames ;
	scanner.fillrange.max 		= ( range.max * min_frames );
	scanner.Set_rpos			( scanner.rpos );

};


CutDesk_class::CutDesk_class( Mixer_class* mixer )
	: Logfacility_class			("Cutter_class")
	, Interface_base			( mixer->DaTA_p )
	, record_range				{ 0, 0 }
	, record_limits				{ 0, 0 }
	, restore_range				{ 0, 0 }
{
	this->Mixer					= mixer;
	this->StA					= nullptr;
	this->StAId					= STAID_ALL;
	this->StAName 				= "";
//	this->record_range			= { 0, 0 };
//	this->record_limits			= { 0, 0 };
//	this->restore_range			= { 0, 0 };
	this->restore_wddata		= sds_p->WD_state;
	Set_record_steps			( sds_p->WD_state.record_steps );

};

CutDesk_class::~CutDesk_class()
{
	sds_p->WD_state				= restore_wddata;
	DESTRUCTOR( className );
};

bool CutDesk_class::setStAId()
{
	StAId_e	staid 		= GetStaid( sds_p->WD_state.roleId );
	if ( CutIds.contains( staid ) )
	{
		if( StA )
			Restore();
		StAId 			= staid;
		StAName 		= GetName( staid );
		StA				= &Mixer->StA->Array[staid];
		return true;
	}
	else
	{
		Info( "StA", (int) staid, "is not a record Id" );
		return false;
	}
}

void CutDesk_class::Setup()
{
	setStAId();
	if ( not StA )
		return;

	// init record range, record_limits, sds_p->WDstate
	restore_range				= StA->scanner.fillrange; 	// backup fillrange
	restore_wddata				= sds_p->WD_state;
	int16_t	min					= restore_range.min / min_frames;
	int16_t	max 				= restore_range.max / min_frames;
	int16_t	len			 		= ( max - min );
	Set_record_steps			( sds_p->WD_state.record_steps );
	if( len < record_steps )
		return; 				// nothing to do

	record_range 				= { min, int16_t( min + measure_parts ) };
	record_limits				= { min, max		};

	sds_p->WD_state.mode		= RANGE_WD;
	sds_p->WD_state.frames		= StA->param.wdsize;
	Update_Sds					( sds_p, record_range, StA->scanner );
//	Wd->Set_WdData				();
}

void CutDesk_class::Set_record_steps( uint8_t width )
{
	// one record has min_frames
	record_steps = ( width == 0 ) ? 1 : width;
}
void CutDesk_class::Restore()
{
	if ( not StA )
		return;					// setup misssing or failed
	StAId 						= STAID_ALL;
	sds_p->WD_state				= restore_wddata;
	StA->scanner.fillrange 		= restore_range;
//	Wd->Set_WdData();
	StA							= nullptr;
}

void CutDesk_class::CursorUpdate()
{
	// provides data for wavedisplay update
	if( not StA )
		return;					// setup misssing or failed
	if ( sds_p->WD_state.direction == wavedisplay_struct::NO_direction )
		return;

	Direction_e	dir				= sds_p->WD_state.direction;
	int16_t		min				= record_range.min;
	int16_t		max				= record_range.max;
	int16_t		rmin			= min;
	int16_t		rmax			= max;
	switch ( dir )
	{
		case BACK_LEFT :
		{
			max	= record_range.max - record_steps;
			if( max > record_range.min )
				rmax	= check_range( record_limits, int16_t(max) );
			break;
		}
		case BACK_RIGHT :
		{
			max	= record_range.max + record_steps;
			rmax = check_range( record_limits, int16_t(max) );
			break;
		}
		case FRONT_LEFT :
		{
			min = record_range.min - record_steps;
			if( min >= 0 )
				rmin 		= check_range( record_limits, int16_t(min) );
			break;
		}
		case FRONT_RIGHT :
		{
			min = record_range.min + record_steps;
			if( min < record_range.max )
				rmin 	= check_range( record_limits, int16_t(min) );
			break;
		}
		case GOTO_END :
		{
			rmax = record_limits.max;
			break;
		}
		case ROTATE_RIGHT :
		{
			Rotate( ROTATE_RIGHT );
			break;
		}
		case ROTATE_LEFT :
		{
			Rotate( ROTATE_LEFT );
			break;
		}
		default :
		{
			break;
		}
	} // switch  dir
	record_range 	= { rmin, rmax };
	Update_Sds		( sds_p, record_range, StA->scanner );
	Info( "Record cursor pos update", 	record_limits.min, "<", record_range.min, "<",
										record_range.max, "<", record_limits.max );
}

void CutDesk_class::Rotate( Direction_e direction)
{
	DataVec_t 	Data_vec{};

	auto rotate = [ this, &Data_vec ]( range_T<buffer_t> range, buffer_t dn )
	{
		for( buffer_t n = range.min; n < range.max; n++ )
			Data_vec.push_back( StA->Data[ (n+dn) % range.len ] );
	};

	buffer_t frames = record_steps * min_frames;
	buffer_t dn 	= 0;
	switch ( direction ) // define start point
	{
		case ROTATE_LEFT : 	{ dn = frames						; break; }
		case ROTATE_RIGHT:	{ dn = restore_range.max - frames	; break; }
		default : { break; }
	} // switch  direction

	rotate( restore_range, dn );

	for( buffer_t n = 0; n < restore_range.len; n++ )
	{
		StA->Data[n] = Data_vec[n];
	}
}

void CutDesk_class::Cut()
{
	if( not StA )
		return;
	buffer_t	start		= record_range.min * min_frames;
	string		filename 	= StA->file;
	Data_t*		cut_data	= &StA->Data[start];
	uint16_t	records		= ( record_range.max - record_range.min );
	buffer_t	cut_bytes	= records * min_frames * sizeof_Data;

	Info( "changing", filename );
	if ( dumpData( filename, cut_data, cut_bytes ) )
	{
		Info( "Cutting", StAName, 	"from", (int) record_range.min,
									"to",	(int) record_range.max,
									"bytes;", cut_bytes);
		StA->Reset();
		loadData( filename, StA->Data, cut_bytes );
		StA->Set_records( records );
		restore_range = StA->scanner.fillrange;
	}
	else
	{
		Comment( ERROR, "writing", cut_bytes, " bytes to file", filename );
	}
}

bool CutDesk_class::LoopFnc()
{
	if( sds_p->WD_state.mode != WDMODE_t::RANGE_WD )
		return false;

	Data_t*		StAdata				= StA->scanner.Next_read();
				if( not Mixer->state.mute )
					Mixer->Add_mono	( StAdata, StAId );	// sum audio data
				Mixer->AddStereo	();		// send to AudioServer
				return 				true;				// announce success
}




