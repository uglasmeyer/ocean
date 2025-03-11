/*
 * mixer.cpp
 *
 *  Created on: Dec 31, 2023
 *      Author: sirius
 */

#include <Mixer.h>
#include <Wavedisplay_base.h>

/**********************
 * Volume_class
 *********************/

Volume_class::Volume_class() : Logfacility_class("Volume_class")
{
	className = Logfacility_class::className ;
};
Volume_class::~Volume_class()
{
};

void Volume_class::Set(	uint8_t future_vol,
						int _mode)
{
	future			= check_range( volume_range, future_vol );
	this->mode		= _mode;
	switch ( this->mode )
	{
		case FIXED :
		{
			past		= future;
			present 	= future;
			present_pc 	= present * 0.01;
			break;
		}
		case SLIDE :
		{
			past	= rint( present );
			break;
		}
		default :
			assert( false );
			break;
	}
}

float Volume_class::Delta_vol ( uint8_t sl_duration )
{
	slideduration 	= check_range( slide_duration_range,sl_duration );
	float		slide_percent 	= float( slideduration) * 0.01 ;
	buffer_t 	slide_frames	= 4 * max_frames * slide_percent;
	float 		delta_volume	= this->future - this->past;
				dvol			= delta_volume / slide_frames;
	return dvol;
};

float Volume_class::Get( )
{
	if( this->mode == FIXED )
		return present_pc;
	if( not fcomp( present , (float) future, 1 ) )
	{
		present		= present + dvol;
		present_pc	= present * 0.01;
	}
	return present_pc;
}

void Volume_class::Update()
{
	if( fcomp( present, (float) future, 1 ) )
	{
		past 	= future;
		present = future;
	}
}
void Volume_class::Show( bool on )
{
	if ( on )
		Set_Loglevel( DBG2, true);
	Comment( DBG2,	" mode   " + to_string( mode) +
 					" past   " + to_string( past ) +
					" present" + to_string( present ) +
					" future " + to_string( future ) +
					" dvol   " + to_string( dvol ));
	Set_Loglevel( DBG2, false );
}
void Volume_class::Test()
{
	TEST_START( className );
	assert( check_range( volume_range, (uint8_t)   2 ) == 2 );
	assert( check_range( volume_range, (uint8_t) 112 ) == 100 );



	TEST_END( className );


}

/************************
 *  Mixer_class
 ***********************/

Mixer_class::Mixer_class( Dataworld_class* data, Wavedisplay_class* wd )
: Logfacility_class("Mixer")
{
	this->className = Logfacility_class::className;

	cout << "Init Mixer_class" << endl;
	this->sds 			= data->GetSdsAddr( );
	this->sds_master 	= data->sds_master;
	this->DaTA			= data;

	for( uint n : MemIds )
	{
		Storage_class Sta;
		Sta.Id = n;
		StA.push_back(Sta);
	}

	StA_struct_t usr_conf = {"temp"		, frames_per_sec * data->Cfg.Config.temp_sec };
	StA_struct_t ext_conf = {"External"	, frames_per_sec * data->Cfg.Config.record_sec };

	for( uint n : UsrIds )
		StA[n].Setup(usr_conf);
	StA[MbIdExternal].Setup(ext_conf);

	Volume.Set( sds_master->Master_Amp,	FIXED ); //set start and master_volume
	if( LogMask[ TEST ] )
	{
		for ( uint n : MemIds )
			StA[n].Memory_base::Info();

		Mono.Info		( "Mono data");
		Mono_out.Info	( "Wave display data");
		Out_L.Info		( "Output Stereo Left");
		Out_R.Info		( "Output Stereo Right");
	}

	wd->Add_role_ptr( osc_struct::EXTID, StA[ MbIdExternal].Data );
};

Mixer_class::~Mixer_class()
{
	if ( not sds ) return;
	Clear_StA_status( sds->StA_state );
	sds->mixer_status.external	= false;

};

void Mixer_class::clear_memory()
{
	// clear temporary memories
	Out_L.Clear_data(0);
	Out_R.Clear_data(0);
	Mono.Clear_data(0); // Wavedisplay mono col data
}

void Mixer_class::Clear_StA_status( StA_state_arr_t& state_arr )
{
	Comment( INFO, "Reset SDS state" );
	std::ranges::for_each( state_arr, 	[ ]( auto& state ){ state.store = false;});
	for ( Storage_class sta : StA )
		sta.Reset_counter();
}


void Mixer_class::Set_mixer_state( const uint& id, const bool& play )
{
	switch ( id )
	{
		case MbIdInstrument :	{ status.instrument = play; break; }
		case MbIdNotes 		:	{ status.notes 		= play; break; }
		case MbIdKeyboard	: 	{ status.kbd 		= play; break; }
		case MbIdExternal 	:	{ status.external 	= play; break; }
		default				:	break;
	}


	StA[id].Play_mode( play );

};
void Mixer_class::Update_ifd_status_flags( interface_t* sds )
{

	sds->mixer_status =  status;
	for ( uint id :  MemIds )
	{
		sds->StA_state[id] 	=  StA[id].state;
	}
}


void Mixer_class::add_mono(Data_t* Data, const uint& id )
// sample Data for different sound devices
// by applying mixer volume per device
{								// 0   1   2   3  In  Kb  Nt  Ex
	const array<int,8> phase_r = {10,  0,-10,  0,  5, -5,  5, -5 };
	const array<int,8> phase_l = { 0,-10,  0, 10,  5, -5,  5, -5 };

	assert( phase_r.size() == StA.size() );

	StA[id].Volume.Delta_vol( sds_master->slide_duration );
	for( buffer_t n = 0; n < sds_master->audioframes/*max_frames*/; n++)
	{
		float
		volpermill 		= StA[id].Volume.Get() * 0.1;
		Out_L.Data[n] 	+= rint( Data[n] * phase_l[id] * volpermill );
		Out_R.Data[n] 	+= rint( Data[n] * phase_r[id] * volpermill );
		Mono.Data[n]  	+= rint( Data[n] );//*volpercent );	// collect mono data for store
	}
	StA[id].Volume.Update();
}

void Mixer_class::add_stereo( stereo_t* data  )
// sample Data for different Synthesizer into audio memory
// by applying master volume per Synthesizer
{
	buffer_t	audioframes		= sds_master->audioframes;

	Volume.Delta_vol( sds_master->slide_duration);
	for( buffer_t n = 0; n < audioframes ; n++ )
	{
		float
		vol_percent 	= Volume.Get();
		data[n].left 	+= rint( Out_L.Data[n]	* vol_percent );
		data[n].right 	+= rint( Out_R.Data[n] 	* vol_percent );
	}
	Volume.Update();

}


void Mixer_class::Store_noteline( uint8_t arr_id, Note_class* Notes )
{
	while ( composer > 0 )
	{
		cout << dec << composer << " " << arr_id << endl;
		Notes->Generate_note_chunk( );
		StA[ arr_id ].Store_block( Notes->Oscgroup.osc.Mem.Data );
		composer--;
	}
	StA[ arr_id ].Record_mode( false );
	StA[ arr_id ].Play_mode( true );

}


void Mixer_class::Add_Sound( Data_t* 	instrument_osc,
							 Data_t* 	keyboard_osc,
							 Data_t* 	notes_osc,
							 stereo_t* 	shm_addr )
{
	auto get_store_id = [ this ]()
		{
			for ( int id : MemIds  )
				if ( StA[id].state.store )
					return id;
			return -1;
		};


	clear_memory();

	if ( status.mute )
	{
		add_stereo( shm_addr );
		return;
	}

	// add osc sound
	if ( StA[ MbIdInstrument].state.play )
	{
		add_mono( instrument_osc, MbIdInstrument );
//		StA[MbIdInstrument].Volume.Show( true );
	}
	if ( StA[ MbIdNotes 	].state.play )
		add_mono( notes_osc		, MbIdNotes );
	if ( StA[ MbIdKeyboard	].state.play )
		add_mono( keyboard_osc	, MbIdKeyboard );

	// add StA sound
	for ( uint DAid : MemIds )// scan rec_ids and exclude notes from being overwritten by store_block
	{
		Data_t* read_data = StA[ DAid ].Get_next_block();
		if ( read_data )
		{
			add_mono( read_data, DAid );
		}
	}

	// store sound to record StA
	int store_id = get_store_id();
	if( store_id >= 0 )
		StA[ store_id ].Store_block( Mono.Data );

	// push sound to audio server
	add_stereo( shm_addr );
};

void Mixer_class::Test()
{
	DaTA->Sds_p->Set_Loglevel( INFO, true );
	DaTA->Sds_p->Reset_ifd();

	Volume.Test( );

	sds_master->audioframes = min_frames;
	sds_master->slide_duration = 1;

	Volume.Set( 100, FIXED );
	Volume.Set( 75 , SLIDE );
	add_stereo( DaTA->ShmAddr_0 );

	ASSERTION(   Volume.past ==   Volume.future, "start_volume",
			(int)Volume.past,(int)Volume.future);

	Volume.Set( 50, FIXED );
	Volume.Set( 75, SLIDE );
	add_stereo( DaTA->ShmAddr_0 );

	ASSERTION(   Volume.past ==   Volume.future, "start_volume",
			(int)Volume.past,(int)Volume.future);

	Volume.Set( 50, FIXED );
	Volume.Set( 75, SLIDE );
	add_stereo( DaTA->ShmAddr_0 );

	ASSERTION(   Volume.past ==   Volume.future, "start_volume",
			(int)Volume.past,(int)Volume.future);

	sds_master->slide_duration = 100;
	sds_master->audioframes = max_frames;
	Volume.Set( 50, FIXED );
	Volume.Set( 0, SLIDE );
	for( uint n = 0; n < 10; n++ )
	{
		add_stereo( DaTA->ShmAddr_0 );

	}
	ASSERTION(   Volume.past ==   Volume.future, "start_volume",
			(int)Volume.past,(int)Volume.future);

	TEST_START( className );
	Mono.Set_Loglevel( TEST, true );
	Mono_out.Set_Loglevel( TEST, true );
	Out_L.Set_Loglevel( TEST, true );
	Out_R.Set_Loglevel( TEST, true );
	for ( Memory& sta : StA )
		sta.Set_Loglevel( TEST, true );
	Mono.Memory_base::Info();

	TEST_END( className );


}
