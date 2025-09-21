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


	for( uint n : StAMemIds )
	{
		Storage_class sta;
		sta.Id = n;
		StA.push_back( sta );
	}

	prgarg_struct  	conf = data->Cfg_p->Config;
	StA_param_t usr_conf = {"temp"		, frames_per_sec * conf.temp_sec	, min_frames };
	StA_param_t ext_conf = {"External"	, frames_per_sec * conf.record_sec	, min_frames };
	StA_param_t kbd_conf = {"Keyboard"	, frames_per_sec * conf.kbd_sec		, min_frames };
	StA_param_t nte_conf = {"Notes"		, frames_per_sec * conf.kbd_sec		, min_frames };

	for( uint n : UsrIds )
		StA[n].Setup(usr_conf);
	StA[STA_EXTERNAL].Setup(ext_conf);
	StA[STA_KEYBOARD].Setup(kbd_conf);
	StA[STA_NOTES].Setup(nte_conf);

	for( uint n : StAMemIds )
	{
		StA[n].scanner.Data 		= StA[n].Data;
		StA[n].scanner.mem_range.max= StA[n].StAparam.size;
		StA[n].scanner.wrt 			= StA[n].StAparam.block_size;
	}

	StA[STA_KEYBOARD].scanner.Set_wrt_len( max_frames );
	StA[STA_KEYBOARD].scanner.Set_fillrange( StA[STA_KEYBOARD].scanner.mem_range.max );

	StA[STA_NOTES].scanner.Set_wrt_len( max_frames );
	StA[STA_NOTES].Reset();//Set_fillrange( StA[STA_NOTES].scanner.mem_range.max );

	DynVolume.SetupVol( sds_master->Master_Amp,	FIXED ); //set start and master_volume

	if( LogMask[ TEST ] )
	{
		for ( uint n : StAMemIds )
			StA[n].Memory_base::DsInfo();

		Mono.DsInfo		( "Mono data");
		Mono_out.DsInfo	( "Wave display data");
		Out.DsInfo		( );
	}

	wd->Add_role_ptr( 	osc_struct::EXTID,
						StA [ STA_EXTERNAL].Data,
						&StA[ STA_EXTERNAL].StAparam.size );
};

Mixer_class::~Mixer_class()
{
	if ( not sds )
		return;
	Clear_StA_status( sds->StA_state );
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

void Mixer_class::Set_mixer_state( const uint& id, const bool& play )
{
	StA[id].Play_mode( play );

	switch ( id )
	{
		case STA_INSTRUMENT :	{ state.instrument = play; break; }
		case STA_NOTES 		:	{ state.notes 		= play;	break; }
		case STA_KEYBOARD	: 	{ state.kbd 		= play; break; }
		case STA_EXTERNAL 	:	{ state.external 	= play; break; }
		default				:	break;
	}
};

void Mixer_class::Update_sds_state( int Id, interface_t* sds )
{

	sds->mixer_state =  state;
	for ( uint id :  StAMemIds )
	{
		sds->StA_state[id] 	=  StA[id].state;
	}
}

void Mixer_class::SetStA()
{
	for ( uint n = 0; n < StA.size() ; n++ )
	{
		bool store = (bool) sds->StA_state[n].store;
		StA[n].Record_mode( store );

		bool play = (bool) sds->StA_state[n].play;
		StA[n].Play_mode( play );

		sds->StA_state[n] = StA[n].state;

		Set_mixer_state( n , play );

		uint8_t amp = sds->StA_amp_arr[ n ];
		StA[ n ].DynVolume.SetupVol( amp , SLIDE );
	}
}

void Mixer_class::add_mono( Data_t* Data, const uint& id )
// sample Data for different sound devices
// by applying mixer volume per device
{								//U0  U1  U2  U3  In  Kb  Nt  Ex
	const array<int,8> phase_r = {10,  0,-10,  0,  5, -5,  5, -5 };
	const array<int,8> phase_l = { 0,-10,  0, 10,  5, -5,  5, -5 };

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

	auto delete_after_read = [ ]( Data_t* Data, buffer_t frames )
	{
		if( not Data ) return;
		for( buffer_t n = 0; n < frames; n++ )
		{
			Data[n] =0;
		}
	};
	auto set_sds_filled = [ this ]( uint8_t staid )
	{
		bool filled = ( StA[staid].scanner.fillrange.max > 0 );
		sds->StA_state[staid].filled = filled;
	};

	if( not shm_addr ) return;
	clear_memory();

	if ( state.mute )
	{
		add_stereo( shm_addr );
		return;
	}

	// add osc sound
	if ( StA[ STA_INSTRUMENT].state.play )
		add_mono( instrument_Data	, STA_INSTRUMENT );
	if ( StA[ STA_NOTES 	].state.play )
	{
		add_mono( notes_Data		, STA_NOTES );
		if( false )//StA[ STA_NOTES ].state.forget )
			delete_after_read( notes_Data, sds->audioframes );
	}
	if ( StA[ STA_KEYBOARD  ].state.play )
	{
		add_mono( kbd_Data			, STA_KEYBOARD );
		if( StA[ STA_KEYBOARD ].state.forget )
			delete_after_read( kbd_Data, sds->audioframes );
	}
	set_sds_filled( STA_INSTRUMENT );
	set_sds_filled( STA_NOTES );
	set_sds_filled( STA_KEYBOARD );

	// write/read StA sound
	for ( uint staid : RecIds )// scan rec_ids and exclude notes from being overwritten by store_block
	{

		if( StA[ staid ].state.store )
			StA[ staid ].Store_block( Mono.Data );
		sds->StA_state[staid].store		= StA[staid].state.store;
		set_sds_filled( staid );
		if ( StA[ staid ].state.play )
		{
			Data_t* StAdata = StA[staid].scanner.Next_read();
			if ( StAdata )
				add_mono( StAdata, staid );
		}
	}

	add_stereo( shm_addr ); 	// push sound to audio server

};

void Mixer_class::TestMixer()
{

	DaTA->Sds_p->Reset_ifd();


	TEST_START( className);

	sds_master->audioframes = min_frames;
	sds_master->slide_duration = 1;

	// TODO	moved to AudioServre
//	DynVolume.SetupVol( 100, FIXED );
//	DynVolume.SetupVol( 75 , SLIDE );
//	add_stereo( DaTA->ShmAddr_0 );

//	ASSERTION(   DynVolume.current.past ==   DynVolume.current.future, "start_volume",
//			(int)DynVolume.current.past,(int)DynVolume.current.future);
/*
	DynVolume.SetupVol( 50, FIXED );
	DynVolume.SetupVol( 75, SLIDE );
	add_stereo( DaTA->ShmAddr_0 );

	ASSERTION(   DynVolume.current.past ==   DynVolume.current.future, "start_volume",
			(int)DynVolume.current.past,(int)DynVolume.current.future);

	DynVolume.SetupVol( 50, FIXED );
	DynVolume.SetupVol( 75, SLIDE );
	add_stereo( DaTA->ShmAddr_0 );

	ASSERTION(   DynVolume.current.past ==   DynVolume.current.future, "start_volume",
			(int)DynVolume.current.past,(int)DynVolume.current.future);

	sds_master->slide_duration = 100;
	sds_master->audioframes = max_frames;
	DynVolume.SetupVol( 50, FIXED );
	DynVolume.SetupVol( 0, SLIDE );
	for( uint n = 0; n < 10; n++ )
	{
		add_stereo( DaTA->ShmAddr_0 );

	}
	ASSERTION(   DynVolume.current.past ==   DynVolume.current.future, "start_volume",
			(int)DynVolume.current.past,(int)DynVolume.current.future);
*/
	Mono.Memory_base::DsInfo();

	TEST_END( className );


}
