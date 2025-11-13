/*
 * mixer.cpp
 *
 *  Created on: Dec 31, 2023
 *      Author: sirius
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


	StA_param_t usr_conf = Mem_param_struct( "temp"		, data->Cfg_p->Config.temp_sec );
	StA_param_t ist_conf = Mem_param_struct( "Instrument",data->Cfg_p->Config.kbd_sec );
	StA_param_t ext_conf = Mem_param_struct( "External"	, data->Cfg_p->Config.record_sec );
	StA_param_t kbd_conf = Mem_param_struct( "Keyboard"	, data->Cfg_p->Config.kbd_sec );
	StA_param_t nte_conf = Mem_param_struct( "Notes"	, data->Cfg_p->Config.kbd_sec );

	int n = 0;
	for( const StA_param_t& param : { 	usr_conf, usr_conf, usr_conf, usr_conf,
										ist_conf, kbd_conf, nte_conf, ext_conf }  )
	{
		Storage_class sta { param };
		sta.Id = n;
		StA.push_back( sta );
		n++;
	}
	for( uint n : StAMemIds )
	{
		StA[n].scanner.Data 		= StA[n].Data;
		StA[n].scanner.mem_range.max= StA[n].param.size;
		StA[n].scanner.wrt 			= StA[n].param.block_size;
	}

	StA[STA_KEYBOARD].scanner.Set_wrt_len( max_frames );

	StA[STA_NOTES   ].scanner.Set_wrt_len( max_frames );
	StA[STA_NOTES   ].scanner.Set_fillrange( StA[STA_NOTES   ].param.size );

	wd->Add_role_ptr( NOTESROLE		, StA[ STA_NOTES   ].Data, &StA[ STA_NOTES   ].param.size );
	wd->Add_role_ptr( KBDROLE		, StA[ STA_KEYBOARD].Data, &StA[ STA_KEYBOARD].param.size );
	wd->Add_role_ptr( EXTERNALROLE	, StA[ STA_EXTERNAL].Data, &StA[ STA_EXTERNAL].param.size );

	SetStA();

	if( LogMask[ TEST ] )
	{
		for ( uint n : StAMemIds )
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
	Clear_StA_status( sds->StA_state_arr );
	sds->mixer_state.external	= false;
	DESTRUCTOR( className );
};

void Mixer_class::clear_memory()
{
	// clear temporary memories
	Out.Clear_data();
	Mono.Clear_data(0);
}

void Mixer_class::Clear_StA_status( StA_state_arr_t& state_arr )
{
	Comment( INFO, "Reset SDS state" );
	std::ranges::for_each( state_arr, 	[ ]( auto& state )
			{ state.store = false;});
	for ( Storage_class sta : StA )
		sta.Reset();
}

void Mixer_class::Set_play_mode( const uint& id, const bool& mode )
{
	StA[id].state.Play( mode );

	switch ( id )
	{
		case STA_INSTRUMENT :	{ state.instrument	= mode; break; }
		case STA_NOTES 		:	{ state.notes 		= mode;	break; }
		case STA_KEYBOARD	: 	{ state.kbd 		= mode; break; }
		case STA_EXTERNAL 	:	{ state.external 	= mode; break; }
		default				:	break;
	}

};

void Mixer_class::Update_sds_state( int Id, interface_t* sds )
{

	sds->mixer_state =  state;
	for ( uint id :  StAMemIds )
	{
		sds->StA_state_arr[id] 	=  StA[id].state.Get();
	}
}

void Mixer_class::SetStA( Id_t staId )
{	// synchronize between sds->StA_state and StA[].state

	Comment( INFO, "updating mixer id ", (int)staId );

	Set_play_mode( staId , sds->StA_state_arr[staId].play );
	StA[staId].state.Filled( sds->StA_state_arr[staId].filled );
	if ( not StA[staId].state.Filled() )
	{
		StA[staId].Reset();
		if( LowIds.contains( staId ) )
		{
			Set_play_mode( staId , false );
		}
	}

	StA[staId].Record_mode( sds->StA_state_arr[staId].store );

	StA[ staId ].DynVolume.SetupVol( sds->StA_amp_arr[ staId ] , SLIDE );

//	sds->StA_state[staId] = StA[staId].state.Get();

}
void Mixer_class::SetStA()
{
	for ( Id_t mixerId : AllIds )
	{
		SetStA( mixerId );
	}
}


void Mixer_class::add_mono( Data_t* Data, const uint& id )
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
		Mono.Data[n]  				+= rint( Data[n] );//*volpercent );	// collect mono data for store
	}
	StA[id].DynVolume.Update();
	if( StA[ id ].state.Forget() )
		delete_after_read( Data, sds_master->audioframes );

}

void Mixer_class::add_stereo( Stereo_t* Data  )
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

	auto set_sds_filled = [ this ]( uint8_t staid )
	{
		bool filled = ( StA[staid].scanner.fillrange.max > 0 );
		StA[staid].state.Filled( filled );
		sds->StA_state_arr[staid].filled = filled;
	};

	if( not shm_addr ) return;
	clear_memory();

	if ( state.mute )
	{
		add_stereo( shm_addr );
		return;
	}

	// add osc sound
	if ( StA[ STA_INSTRUMENT ].state.Play() )
	{
		add_mono( instrument_Data	, STA_INSTRUMENT );
	}
	if ( StA[ STA_NOTES 	].state.Play() )
	{
		add_mono( notes_Data		, STA_NOTES );
	}
	if ( StA[ STA_KEYBOARD  ].state.Play() )
	{
		add_mono( kbd_Data			, STA_KEYBOARD );
	}
	set_sds_filled( STA_INSTRUMENT );
	set_sds_filled( STA_NOTES );
	set_sds_filled( STA_KEYBOARD );

	// write/read StA sound
	for ( uint staId : RecIds )// scan rec_ids and exclude notes from being overwritten by store_block
	{
		Storage_class& sta = StA[staId];

		if( sta.state.Store() )
			sta.Store_block( Mono.Data );

		sds->StA_state_arr[staId].store		= sta.state.Store();
		set_sds_filled( staId );

		if ( sta.state.Play() )
		{
			Data_t* StAdata = sta.scanner.Next_read();
			if ( StAdata )
				add_mono( StAdata, staId );
		}
	}

	add_stereo( shm_addr ); 	// push sound to audio server

};

void Mixer_class::TestMixer()
{

	TEST_START( className);

	TEST_END( className );


}
