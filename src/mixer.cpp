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
 * mixer.cpp
 *
 *  Created on: Dec 31, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Mixer.h>
#include <Wavedisplay_base.h>

/************************
 *  Mixer_class
 ***********************/

Mixer_class::Mixer_class( Dataworld_class* data, Wavedisplay_class* wd )
	: Logfacility_class	("Mixer_class")
	, RecMono_mem		( audiobuffer_bytes )
	, Out				( sizeof_Stereo, Stereoaudio_bytes )
	, DynVolume			( volidx_range )

{
	Info( "Init Mixer_class" );
	this->className 	= Logfacility_class::className;
	this->sds 			= data->GetSdsAddr( );
	this->sds_master 	= data->sds_master;
	this->DaTA			= data;
	this->Wd_p			= wd;

	StA_param_t usr_conf = Mem_param_struct( "temp"		, data->Cfg_p->Config.temp_sec );
	StA_param_t ist_conf = Mem_param_struct( "Instrument",data->Cfg_p->Config.kbd_sec );
	StA_param_t ext_conf = Mem_param_struct( "External"	, data->Cfg_p->Config.record_sec );
	StA_param_t kbd_conf = Mem_param_struct( "Keyboard"	, data->Cfg_p->Config.kbd_sec );
	StA_param_t nte_conf = Mem_param_struct( "Notes"	, data->Cfg_p->Config.kbd_sec );

	StAId_e staid = STA_USER00;
	for( const StA_param_t& param : { 	usr_conf, usr_conf, usr_conf, usr_conf,
										ist_conf, kbd_conf, nte_conf, ext_conf }  )
	{
		Storage_class sta { staid, param };
		sta.Volume( sds->StA_amp_arr[staid], FIXED );
		sta.Set_filename( DaTA->Cfg_p->fs->vardir, sds->SDS_Id );
		restoreStA( sta );
		StA.push_back( sta );
		staid++;
	}

	StA[STA_NOTES   ].scanner.Set_fillrange( StA[STA_NOTES   ].param.size );

	Wd_p->Add_role_ptr( NOTESROLE	, StA[ STA_NOTES   ].Data, &StA[ STA_NOTES   ].param.wdsize );
	Wd_p->Add_role_ptr( KBDROLE		, StA[ STA_KEYBOARD].Data, &StA[ STA_KEYBOARD].param.wdsize );
	Wd_p->Add_role_ptr( EXTERNALROLE, StA[ STA_EXTERNAL].Data, &StA[ STA_EXTERNAL].param.wdsize );
	for( StAId_e staid : LowIds )
	{
		RoleId_e sta_role = sta_rolemap.GetRoleid( staid );
		Wd_p->Add_role_ptr( sta_role , StA[ staid ].Data, &StA[ staid ].param.wdsize );
	}

	SetStAs();

	if( LogMask[ TEST ] )
	{
		for ( uint n : StAIds )
			StA[n].Memory_base::DsInfo();

		RecMono_mem.DsInfo		( "Mono data");
		Out.DsInfo		( );
	}
};

Mixer_class::~Mixer_class()
{
	if ( not sds )
		return;
	if( not LogMask[TEST ])
		for( Storage_class& sta : StA )
			dumpStA( sta );

	DESTRUCTOR( className );
};

void Mixer_class::BeatClock( const uint8_t& bps )
{
	if ( ( bps > 0 ) and ( bps < measure_parts ) )
	{
		beat_clock = ( beat_clock + 1 ) % ( measure_parts / bps );
		state.sync = ( beat_clock == 0 ); //follow instrument rythm if there is any
	}
	else
		state.sync = true; // no rythm

	// switch the record mode only if state.sync and trigger is active
	if( state.sync )
	{
		for( StAId_e staid : StAIds )
		{
			if ( StA[staid].beattrigger.local_data.active )
			{
				StA[staid].Record_mode( sds->StA_state_arr[staid].store );
				StA[staid].beattrigger.local_data.active = false; // trigger work is done
			}
		}
		Comment( Logmask(DEBUG), state.sync, StA[0].beattrigger.local_data.active,
								sds->StA_state_arr[0].store, StA[0].state.Store() );
	}
	sds->mixer_state.sync = state.sync;
	if( beat_clock == 0 )
		sds_master->overmodulated = false;

}

void Mixer_class::StA_Wdcursor()
{

	StAId_e 	staid = sta_rolemap.GetStaid( sds->WD_state.roleId );
	if ( staid < STA_SIZE )
	{
		buffer_t rpos = StA[ staid ].scanner.rpos;
		Wd_p->Set_wdcursor( rpos, 1 );
		Comment( Logmask(DEBUG), "Set_Wdcursor", (int) staid, rpos );
	}
}

void Mixer_class::clear_temporary_memory()
{
	// clear temporary memories
	Out.Clear_data();
	RecMono_mem.Clear_data(0);
}


void Mixer_class::ResetStA( const StAId_e& staid )
{
	Comment( INFO, "Reset SDS state" );
	if ( staid == STA_SIZE )
	{
		std::ranges::for_each( StAIds, 	[ this ]( StAId_e id )
			{ StA[id].Reset(); } );
	}
	else
	{
		StA[staid].Reset();
	}
	SetStAProperties( staid );
}

bool Mixer_class::restoreStA( Storage_class& sta )
{
	// copy dump file data into StA

			sta.Clear_data(0);

	size_t	bytes_red 			= loadData( sta.file, sta.Data, 0 );
	uint	records				= bytes_red / sta.mem_ds.record_size / sta.mem_ds.sizeof_type;
			sta.Store_counter	( records );

	Info( "Restore StA data from file", sta.filename );

	return true;

}

void Mixer_class::dumpStA( Storage_class& sta )
{
	// copy StA memory file

	buffer_t 	bytes2write = sta.scanner.fillrange.max * sizeof(Data_t);//mem_ds.bytes;

	Info( "Dump StA memory to file", sta.filename ) ;
	dumpData( sta.file, sta.Data, bytes2write );

}


void Mixer_class::auto_volume( const StAId_e& id)
{
	if( sds->StA_amp_arr[id] == 0 )
	{
		sds->StA_amp_arr[id] = osc_default_volume;
		StA[id].Volume( osc_default_volume, sds->vol_slidemode );
	}
}

void Mixer_class::set_play_mode( const StAId_e& id, const bool& mode )
{
	Comment( INFO, "updating StA id", (int)id, " play:", (int) mode );
	state = sds->mixer_state;
	switch ( id )
	{
		case STA_INSTRUMENT :	{ state.instrument	= mode; break; }
		case STA_NOTES 		:	{ state.notes 		= mode;	break; }
		case STA_KEYBOARD	: 	{ state.kbd 		= mode; break; }
		case STA_EXTERNAL 	:	{ state.external 	= mode; break; }
		default				:	break;
	}
	sds->mixer_state = state;
};

bool Mixer_class::setFillState( StAId_e id )
{
	// achieve consistency between involved parties
	bool	filled							= ( StA[id].scanner.Get_filled() );
			sds->StA_state_arr[id].filled 	= filled;
			StA[id].state.Filled			( filled );
	return	filled;
}

void Mixer_class::SetStAProperties( StAId_e staId )
{	// distribution of sds->StA_state into StA[].state

	auto setStA = [ this ]( StAId_e id  )
	{
		StA[id].state.Set( sds->StA_state_arr[id] );
		set_play_mode( id, StA[id].state.Play() );

		if ( not setFillState( id ) )
		{
			if( LowIds.contains( id ) ) // only LowIds are automatically switched off
			{							// HighIds are filled because of play
				StA[id].Reset();
				set_play_mode( id , false );
			}
		}
		if( StA[id].state.Play() )
		{
			auto_volume( id );
			RoleId_e 	roleid = sta_rolemap.GetRoleid( id );
			Wd_p->Set_WdRole( roleid );
		}
		sds->StA_state_arr[id] = StA[id].state.Get();
	};

	if( staId == STA_SIZE )
	{
		std::ranges::for_each( StAIds, [ this, setStA ](StAId_e id)
			{ setStA(id) ;} );
	}
	else
	{
		setStA( staId );
	}

};

void Mixer_class::SetStAs()
{
	for ( StAId_e mixerId : StAIds )
	{
		SetStAProperties( mixerId );
	}
}


void Mixer_class::Add_mono( Data_t* Data, const uint& id )
// sample Data for different sound devices
// by applying mixer volume per device
{								//U0  U1  U2  U3  In  Kb  Nt  Ex
	if( Data == nullptr ) return;

	auto delete_after_read = [ ]( Data_t* Data, buffer_t frames )
	{
		for( buffer_t n = 0; n < frames; n++ )
		{
			Data[n] =0;
		}
	};

//												  +r  -l  -r  +l  In  Ky  No  Ex
	const array<int,StAId_e::STA_SIZE> phase_r = {10,  0,-10,  0,  7, -7,  7, -7 };
	const array<int,StAId_e::STA_SIZE> phase_l = { 0,-10,  0, 10,  7, -7,  7, -7 };

	StA[id].DynVolume.SetDelta( sds_master->slide_duration );
	for( buffer_t n = 0; n < sds_master->audioframes; n++)
	{
		float
		volpermill 					= StA[id].DynVolume.Get() * 0.1;
		Out.stereo_data[n].left 	+= rint( Data[n] * phase_l[id] * volpermill );
		Out.stereo_data[n].right 	+= rint( Data[n] * phase_r[id] * volpermill );
		RecMono_mem.Data[n]  				+= rint( Data[n] );	// collect mono data for store
	}
	StA[id].DynVolume.Update();
	if( StA[ id ].state.Forget() )
		delete_after_read( Data, sds_master->audioframes );

}

void Mixer_class::Add_stereo( Stereo_t* Data  )
// sample Data for different Synthesizer into audio memory
// by applying master volume per Synthesizer
{
	buffer_t	audioframes	= sds_master->audioframes;

	for( buffer_t n = 0; n < audioframes ; n++ )
	{

		Data[n].left 	+= Out.stereo_data[n].left ;//	* vol_percent * balanceL );
		Data[n].right 	+= Out.stereo_data[n].right;// 	* vol_percent * balanceR );
		Out.stereo_data[n] = {0,0};
	}

}

/*
void Mixer_class::Store_noteline( uint8_t arr_id, Note_class* Notes )
{
	while ( composer > 0 )
	{
		cout << dec << composer << " " << arr_id << endl;
		Notes->Generate_data( );
		StA[ arr_id ].Store_block( Notes->Oscgroup.osc.MemData_p() );
		composer--;
	}
	StA[ arr_id ].Record_mode( false );
	StA[ arr_id ].Play_mode( true );

}
*/

void Mixer_class::Add_Sound( Data_t* 	instrument_Data,
							 Data_t* 	kbd_Data,
							 Data_t* 	notes_Data,
							 Stereo_t* 	shm_addr )
{

	if( not shm_addr ) return;
	clear_temporary_memory();

	if ( state.mute )
	{
		Add_stereo( shm_addr );
		return;
	}

	// add osc sound
	if ( state.instrument )
	{
		Add_mono( instrument_Data, STA_INSTRUMENT );
	}
	if ( StA[ STA_NOTES 	].state.Play() )
	{
		Add_mono( notes_Data	, STA_NOTES );
	}
	if ( StA[ STA_KEYBOARD  ].state.Play() )
	{
		Add_mono( kbd_Data		, STA_KEYBOARD );
	}
	setFillState( STA_INSTRUMENT );
	setFillState( STA_NOTES );
	setFillState( STA_KEYBOARD );

	// read sound from StA
	for ( uint staId : RecIds )
	{
		Storage_class& sta = StA[staId];
		if ( sta.state.Play() and ( not sta.state.Store()) ) //don't play if record
		{
			Data_t* StAdata = sta.scanner.Next_read();
			if ( StAdata )
			{
				Add_mono( StAdata, staId );
			}
		}
	}
	// write sound to StA
	for ( StAId_e staId : RecIds )
	{
		Storage_class& sta = StA[staId];
		if( sta.state.Store() )
			sta.Store_record( RecMono_mem.Data );
		if( state.sync ) // sync from beatclock
			sds->StA_state_arr[staId].store = StA[staId].state.Store();

		setFillState( staId );
	}

	Add_stereo( shm_addr ); 	// push sound to audio server

};

void Mixer_class::TestMixer()
{

	TEST_START( className);

	uint8_t test_vol = 87;

	StA[STA_NOTES].Volume( test_vol, SLIDE );
	float vol = StA[STA_NOTES].Volume();
	Assert_equal( (int)test_vol, (int)(vol*100), "StA volume" );

	StA[STA_NOTES].Volume( test_vol, FIXED );
	vol = StA[STA_NOTES].Volume();
	Assert_equal( (int)test_vol, (int)(vol*100), "StA volume" );

	StAId_e staid = STA_USER00;
	sds->StA_amp_arr[staid]= 0;
	sds->StA_state_arr[staid].play = true;
	SetStAProperties( staid );
	Assert_equal( sds->StA_amp_arr[staid], osc_default_volume );
	Assert_equal( sds->WD_state.roleId, sta_rolemap.GetRoleid( staid) );

	staid = STA_INSTRUMENT;
	sds->StA_amp_arr[staid]= 0;
	sds->mixer_state.instrument = false;
	sds->StA_state_arr[staid].play = true;
	SetStAProperties( staid );
	Assert_equal( sds->StA_amp_arr[staid], osc_default_volume );
	Assert_equal( sds->mixer_state.instrument, true );
	Assert_equal( sds->WD_state.roleId, INSTRROLE );

	range_T<int> r{10,14};
	Info( show_range( r ) );
	r = { 1 , 12 };
	Info( show_range( r ) );
	Assert_equal( (int)r.len, 11 );
	cursor_T<int> c{0,1,3};
	Assert_equal( c.len, size_t(3) );
	Assert_equal( c.cur, 1 );
	TEST_END( className );
}

/**************************************************
 * CutDesk_class
 *************************************************/

inline int16_t set_record_steps( uint8_t bps )
{
	return ( bps == 0 ) ? 1 : measure_parts / bps;
};

inline int16_t conv_int16( int value, int max )
{
	int	ret 	= 0;
	if (max != 0 )
		ret 	= rint( ( value * wavedisplay_len ) / max );
	return int16_t( ret );
};

auto Update_Sds = [  ]( interface_t* sds,
						record_range_t range,
						Scanner_class& scanner  )
{
	//consistently update sds->WDstate and StA->scanner
	int	max_records				= sds->WD_state.frames / min_frames;

	sds->WD_state.direction 	= wavedisplay_t::NO_direction;
	sds->WD_state.cursor.min	= conv_int16( range.min, max_records ),
	sds->WD_state.cursor.max	= conv_int16( range.max, max_records ) ;

	scanner.fillrange.min		= range.min * min_frames ;
	scanner.Set_fillrange		( range.max * min_frames );
	if( not in_range( scanner.fillrange, scanner.rpos ) )
		scanner.rpos 			= range.min * min_frames;

};

CutDesk_class::CutDesk_class( Mixer_class* mixer ) :
	Logfacility_class("Cutter_class")
{
	className 					= Logfacility_class::className;
	this->Mixer					= mixer;
	this->StA					= nullptr;
	this->sds					= Mixer->sds;
	this->Wd					= Mixer->Wd_p;
	this->StAId					= STA_SIZE;
	this->StAName 				= "";
	this->step_records			= set_record_steps( sds->beatClock );
	this->record_range			= { 0, 0 };
	this->record_limits			= { 0, 0 };
	this->restore_range			= { 0, 0 };
};

CutDesk_class::~CutDesk_class()
{
	DESTRUCTOR( className );
};

void CutDesk_class::Setup()
{
	setStAId();
	if ( not StA )
		return;

	// init record range, record_limits, sds->WDstate
			restore_range		= StA->scanner.fillrange; 	// backup fillrange
	int16_t	min					= restore_range.min / min_frames;
	int16_t	max 				= restore_range.max / min_frames;
	int16_t	len			 		= ( max - min );
				step_records	= set_record_steps( sds->beatClock );
	if( len < step_records )
		return; 				// nothing to do

			record_range 		= { min, int16_t( min + measure_parts ) };
			record_limits		= { min, max		};

			Wd->Set_wdmode		( CURSORID );
			sds->WD_state.wd_mode=CURSORID;
			sds->WD_state.frames= restore_range.max;
			Update_Sds			( sds, record_range, StA->scanner );
}

void CutDesk_class::reset()
{
	StAId 						= STA_SIZE;
	StA							= nullptr;
	Wd->Set_wdmode				( FULLID );
	sds->WD_state.wd_mode 		= FULLID;
	sds->WD_state.cursor.min	= 0;
	sds->WD_state.cursor.max	= wavedisplay_len;
}

void CutDesk_class::Restore()
{
	if ( not StA )
		return;					// setup misssing or failed
	StA->scanner.fillrange 		= restore_range;
	reset();
}

void CutDesk_class::CursorUpdate()
{
	// provides data for wavedisplay update
	if( not StA )
		return;					// setup misssing or failed
	if ( record_range.len < step_records )
		return; 				// nothing to do
	if ( sds->WD_state.direction == wavedisplay_struct::NO_direction )
		return;

				step_records	= set_record_steps( sds->beatClock );
	Direction_e	dir				= sds->WD_state.direction;
	int16_t		min				= record_range.min;
	int16_t		max				= record_range.max;
	int16_t		rmin			= min;
	int16_t		rmax			= max;
	switch ( dir )
	{
		case BACK_LEFT :
		{
			max	= record_range.max - step_records;
			if( max > record_range.min )
				rmax	= check_range( record_limits, int16_t(max) );
			break;
		}
		case BACK_RIGHT :
		{
			max	= record_range.max + step_records;
			rmax = check_range( record_limits, int16_t(max) );
			break;
		}
		case FRONT_LEFT :
		{
			min = record_range.min - step_records;
			if( min >= 0 )
				rmin 		= check_range( record_limits, int16_t(min) );
			break;
		}
		case FRONT_RIGHT :
		{
			min = record_range.min + step_records;
			if( min < record_range.max )
				rmin 	= check_range( record_limits, int16_t(min) );
			break;
		}
		case GOTO_END :
		{
			rmax = record_limits.max;
			break;
		}
		default :
		{
			break;
		}
	} // switch  dir
	record_range 	= { rmin, rmax };
	Update_Sds		( sds, record_range, StA->scanner );

	Info( "Record cursor pos update", 	record_limits.min, "<", record_range.min, "<",
										record_range.max, "<", record_limits.max );
}

bool CutDesk_class::setStAId()
{
	StAId_e	staid 		= GetStaid( sds->WD_state.roleId );
	set<StAId_e> cutdeskIds = RecIds;
	cutdeskIds.insert( STA_KEYBOARD );
	if ( cutdeskIds.contains( staid ) )
	{
		StAId 			= staid;
		StAName 		= StAIdName ( staid );
		StA				= &Mixer->StA[staid];
		return true;
	}
	else
	{
		Info( "StA", (int) staid, "is not a record Id" );
		return false;
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
		StA->Store_counter( records );
		restore_range = StA->scanner.fillrange;
	}
	else
	{
		Comment( ERROR, "failed" );
	}
}

void CutDesk_class::Display()
{
	Wd->Set_wdcursor( StA->scanner.rpos, 1 );
	Wd->Write_wavedata();
}

