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

Mixer_class::Mixer_class( Dataworld_class* data, Wavedisplay_class* wd ) :
	Logfacility_class("Mixer_class")
{
	this->className = Logfacility_class::className;

	Info( "Init Mixer_class" );
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
		StA.push_back( sta );
		staid++;
	}

	StA[STA_KEYBOARD].scanner.Set_wrt_len( max_frames );
	StA[STA_NOTES   ].scanner.Set_wrt_len( max_frames );
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
	for( Storage_class& sta : StA )
	{
		sta.Set_filename( DaTA->Cfg_p->fs->vardir );
		restoreStA( sta );
	}

	if( LogMask[ TEST ] )
	{
		for ( uint n : StAIds )
			StA[n].Memory_base::DsInfo();

		Mono.DsInfo		( "Mono data");
		Mono_out.DsInfo	( "Wave display data");
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
	}
	if( state.sync )
		Comment( DEBUG, state.sync, StA[0].beattrigger.local_data.active,
						sds->StA_state_arr[0].store, StA[0].state.Store() );

}

void Mixer_class::StA_Wdcursor()
{

	StAId_e 	staid = sta_rolemap.GetStaid( sds->WD_status.roleId );
	if ( staid < STA_SIZE )
	{
		buffer_t rpos = StA[ staid ].scanner.rpos;
		Wd_p->Set_wdcursor( rpos );
		Comment( DEBUG, "Set_Wdcursor", (int) staid, rpos );
	}
}

void Mixer_class::clear_memory()
{
	// clear temporary memories
	Out.Clear_data();
	Mono.Clear_data(0);
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

	Info( "Restore StA data from file", sta.filename );
	size_t bytes_red = loadData( sta.file, sta.Data, 0 );
	uint		records = bytes_red / sta.mem_ds.record_size / sizeof(Data_t);
	sta.Set_store_counter( records );


	return true;

}

void Mixer_class::dumpStA( Storage_class& sta )
{
	// copy StA memory file

	buffer_t 	bytes2write = sta.scanner.fillrange.max * sizeof(Data_t);//mem_ds.bytes;

	Info( "Dump StA memory to file", sta.filename ) ;
	dumpData( sta.file, sta.Data, bytes2write );

}

void Mixer_class::Set_staVolume( const StAId_e& id, uint8_t vol )
{
	StA[id].DynVolume.SetupVol( vol, sds->vol_slidemode );
}
void Mixer_class::auto_volume( const StAId_e& id)
{
	if( sds->StA_amp_arr[id] == 0 )
	{
		sds->StA_amp_arr[id] = osc_default_volume;
		Set_staVolume( id, osc_default_volume );
	}
}

void Mixer_class::Set_play_mode( const StAId_e& id, const bool& mode )
{
	Comment( INFO, "updating mixer id ", (int)id, " play:", (int) mode );
	switch ( id )
	{
		case STA_INSTRUMENT :	{ state.instrument	= mode; break; }
		case STA_NOTES 		:	{ state.notes 		= mode;	break; }
		case STA_KEYBOARD	: 	{ state.kbd 		= mode; break; }
		case STA_EXTERNAL 	:	{ state.external 	= mode; break; }
		default				:	break;
	}
	sds->mixer_state = state;
	StA[id].state.Play( mode );

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
		bool play = sds->StA_state_arr[id].play;
		Set_play_mode( id , play );

		if ( not setFillState( id ) )
		{
			StA[id].Reset();
			if( LowIds.contains( id ) )
			{
				Set_play_mode( id , false );
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
	auto delete_after_read = [ ]( Data_t* Data, buffer_t frames )
	{
		if( not Data ) return;
		for( buffer_t n = 0; n < frames; n++ )
		{
			Data[n] =0;
		}
	};
//								   +r -l  -r  +l
	const array<int,8> phase_r = {10,  0,-10,  0,  7, -7,  7, -7 };
	const array<int,8> phase_l = { 0,-10,  0, 10,  7, -7,  7, -7 };

	if( Data == nullptr ) return;
	assert( phase_r.size() == StA.size() );


	StA[id].DynVolume.SetDelta( sds_master->slide_duration );
	for( buffer_t n = 0; n < sds_master->audioframes/*max_frames*/; n++)
	{
		float
		volpermill 		= StA[id].DynVolume.Get() * 0.1;
		Out.stereo_data[n].left 	+= rint( Data[n] * phase_l[id] * volpermill );
		Out.stereo_data[n].right 	+= rint( Data[n] * phase_r[id] * volpermill );
		Mono.Data[n]  				+= rint( Data[n] );	// collect mono data for store
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
	clear_memory();

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
			sta.Store_block( Mono.Data );

		if (state.sync )
			sds->StA_state_arr[staId].store = StA[staId].state.Store();
		setFillState( staId );
	}

	Add_stereo( shm_addr ); 	// push sound to audio server

};

void Mixer_class::TestMixer()
{

	TEST_START( className);

	TEST_END( className );


}

/**************************************************
 * Cutter_class
 *************************************************/
Cutter_class::Cutter_class( Mixer_class* Mixer ) :
	Logfacility_class("Cutter_class")
{
	className 	= Logfacility_class::className;
	this->StA	= &Mixer->StA;
	this->sds	= Mixer->sds;
	this->Wd	= Mixer->Wd_p;
	this->StAId	= STA_SIZE;
};

Cutter_class::~Cutter_class()
{
	DESTRUCTOR( className );
};


void Cutter_class::Setup()
{
	if ( not setStAId() )
		return;

	buffer_t min	= StA->at(StAId).scanner.fillrange.min;
	buffer_t max 	= StA->at(StAId).scanner.fillrange.max;
	buffer_t len 	= max - min;
	if( len < step_records )
		return;
	size_t record_end = min + 2*step_records;

	fillrange					= StA->at(StAId).scanner.fillrange; // backup
	record_range 				= { min, max, (size_t)len };
	StA->at(StAId).scanner.Set_fillrange( record_end ) ;
	StA->at(StAId).scanner.rpos = 0;
	sds->WD_status.wd_mode 		= CURSORID;
    sds->WD_status.direction 	= NO_direction;
	sds->WD_status.max 			= record_end;
	sds->WD_status.min 			= min;
	Wd->Set_wdmode( CURSORID );
	setup_done					= true;
}
void Cutter_class::Restore()
{
	if ( not setup_done )
		return;
	setup_done 							= false;
	sds->WD_status.wd_mode 				= FULLID;
	StA->at(StAId).scanner.fillrange 	= fillrange;
	StAId 								= STA_SIZE;
	Wd->Set_wdmode( FULLID );
}
void Cutter_class::CursorUpdate()
{
	// providesdata for wavedisplay update
	if( StAId == STA_SIZE )
		return;
	if ( record_range.len < step_records )
		return; // no change of boundaries

	sds->WD_status.min			= check_range( record_range, sds->WD_status.min );
	sds->WD_status.max			= check_range( record_range, sds->WD_status.max );

	buffer_t wd_min				= sds->WD_status.min;
	buffer_t wd_max				= sds->WD_status.max;

	Direction_e	dir				= sds->WD_status.direction;
	switch ( dir )
	{
		case BACK_LEFT :
		{
			buffer_t max	= wd_max - step_records;
			wd_max			= check_range( record_range, max );
			break;
		}
		case BACK_RIGHT :
		{
			buffer_t max	= wd_max + step_records;
			wd_max			= check_range( record_range, max );
			break;
		}
		case FRONT_LEFT :
		{
			buffer_t min 	= wd_min - step_records;
			wd_min 			= check_range( record_range, min );
			break;
		}
		case FRONT_RIGHT :
		{
			buffer_t min 	= wd_min - step_records;
			wd_min 			= check_range( record_range, min );
			break;
		}
		case GOTO_END :
		{
			buffer_t max 	= record_range.max;
			wd_max			= check_range( record_range, max );
			break;
		}

		default : { break; }
	} // switch  dir

	StA->at(StAId).scanner.fillrange.min = wd_min;
	StA->at(StAId).scanner.fillrange.max = wd_max;
	StA->at(StAId).scanner.rpos = StA->at(StAId).scanner.fillrange.min;

	sds->WD_status.min		= wd_min;
	sds->WD_status.max		= wd_max;
	sds->WD_status.cursor	= wd_min;

	Info( "Record cursor pos update", 	record_range.min, "<", wd_min, "<",
										wd_max, "<", record_range.max );

}

bool Cutter_class::setStAId()
{
	StAId_e staid 	= GetStaid( sds->WD_status.roleId );
	if ( RecIds.contains( staid ) )
	{
		StAId 			= staid;
		StAName 		= StAIdName ( staid );
		return true;
	}
	Info( "StA", (int) staid, "is not a record Id" );
	return false;
}

void Cutter_class::Cut()
{

	buffer_t	start		= sds->WD_status.min;

	string		filename 	= StA->at(StAId).file;
				cut_data	= &StA->at(StAId).Data[start];
				cut_bytes	= record_range.len * sizeof(Data_t);
				StA->at( StAId).param.wdsize = record_range.len;

	Info( "changing", filename );
	if ( dumpData( filename, cut_data, cut_bytes ) )
	{
		Info( "Cutting", StAName, 	"from", (int) sds->WD_status.min,
									"to",	(int) sds->WD_status.max );
	}
	else
	{
		Comment( ERROR, "failed" );
	}
}



void Cutter_class::Display()
{
	Wd->Write_wavedata();
}

