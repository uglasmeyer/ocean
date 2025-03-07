/*
 * mixer.cpp
 *
 *  Created on: Dec 31, 2023
 *      Author: sirius
 */

#include <Mixer.h>
#include <Wavedisplay_base.h>
//-------------------------------------------------------------------------------------------------
/*

void Loop_class::Start( uint16_t beg, uint16_t end, uint8_t step )
{
	active = true;
	this->beg = beg;
	this->end = end;
	int diff = end - beg;
	if ( step > abs(diff) ) step = abs(diff);
	if ( diff >= 0 )
		this->step = abs(step);
	else
		this->step = -abs(step);

	this->counts = 0;
	this->counter = beg ;
	if ( this->step == 0 )
		this->max_counts = 0;
	else
		this->max_counts = (abs( diff )/abs(step )  );

	stringstream strs{""};
	String Str{""};
	strs << "Loop starts on addr " << Str.to_hex( (long)ptr8 ) << endl;
	strs << SETW << "with parameter step:" << (int) step << " begin: " << (int)beg << " end: " << end ;
	Comment( INFO,  strs.str() );
}


void Loop_class::Next_amp(  ) // Amplify
{
	auto inc = [this]()
	{
		counts++;
		counter = counter + step;
	};


	if ( not active ) return;
	if ( counts == max_counts ) active = false;
	if ( active )
	{
		inc();
		*ptr8 = counter;
	}
};


void Loop_class::Test()
{

	uint8_t 	 ch = 20;

	Loop_class Loop{ &ch};


	Loop.Start(20, 10, 1);
	for( int i = 0; i<20; i++ ) Loop.Next_amp( );
	assert( ch == 10 );


	ch=20;Loop.Start(20, 10, -1);
	for( int i = 0; i<20; i++ ) Loop.Next_amp();
	assert( ch == 10 );

	ch =0; Loop.Start(0, 0, 0);
	for( int i = 0; i<10; i++ ) Loop.Next_amp();
	ASSERTION( ch == 0, "ch " , ch, "0" );

	ch = 0; Loop.Start(0, 0, -1);
	for( int i = 0; i<10; i++ ) Loop.Next_amp( );
	ASSERTION( ch == 0, "ch " , ch, "0" );

	ch = 10; Loop.Start(10, 20, -1);
	for( int i = 0; i<20; i++ ) Loop.Next_amp( );
	assert( ch == 20 );



};
*/
//-------------------------------------------------------------------------------------------------


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

	Set_master_volume( 	sds_master->Master_Amp,
						FIXED,
						sds_master->slide_duration ); //set start and master_volume
	present_vol 			= future_volume;
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
		sds->StA_amp_arr[id] 	=  StA[id].Amp;
	}
}


void Mixer_class::add_mono(Data_t* Data, const uint8_t& sta_amp, const uint& id )
// sample Data for different sound devices
// by applying mixer volume per device
{								// 0   1   2   3  In  Kb  Nt  Ex
	const array<int,8> phase_r = {10,  0,-10,  0,  5, -5,  5, -5 };
	const array<int,8> phase_l = { 0,-10,  0, 10,  5, -5,  5, -5 };

	assert( phase_r.size() == StA.size() );
	assert( sta_amp <= 100 );

	float volpercent= sta_amp / 100.0;
	float Data_r 	= (phase_r[id] * volpercent)/10;
	float Data_l 	= (phase_l[id] * volpercent)/10;

	for( buffer_t n = 0; n < max_frames; n++)
	{
		Out_L.Data[n] 		+= rint( Data[n]*Data_l );
		Out_R.Data[n] 		+= rint( Data[n]*Data_r );
		Mono.Data[n]  		+= rint( Data[n]*volpercent );	// collect mono data by using sta-amp
	}
}

void Mixer_class::Set_master_volume( uint8_t vol, int mode, uint8_t sl_duration )
{
	future_volume	= check_range( volume_range, 		vol );
	slide_duration 	= check_range( slide_duration_range,sl_duration );
	audio_frames	= sds_master->audioframes;

	switch ( mode )
	{
		case FIXED :
		{
			past_volume	= future_volume;
			present_vol = future_volume;
			break;
		}
		case SLIDE :
		{
			past_volume	= rint( present_vol );
			break;
		}
		default :
			assert( false );
			break;
	}
}

void Mixer_class::stereo_out( stereo_t* data )
// sample Data for different Synthesizer into audio memory
// by applying master volume per Synthesizer
{
	float		slide_percent 	= float( slide_duration ) * 0.01 ;
	buffer_t 	slide_frames	= 4 * max_frames * slide_percent;
	float 		delta_volume	= future_volume - past_volume;
	buffer_t 	frames 			= audio_frames;
	float		dvol			= delta_volume / slide_frames;
	float 		vol_percent 	= present_vol * 0.01;

	for( buffer_t n = 0; n < frames ; n++ )
	{
		if( not fcomp( present_vol , future_volume, 2*dvol ) )
		{
			present_vol		= present_vol + dvol;
			vol_percent 	= present_vol * 0.01;
		}
		data[n].left 	+= rint( Out_L.Data[n] * vol_percent );
		data[n].right 	+= rint( Out_R.Data[n] * vol_percent );
	}
	if( fcomp( present_vol, future_volume, 2*dvol ) )
	{
		past_volume = future_volume;
		present_vol = future_volume;
	}
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
		stereo_out( shm_addr );
		return;
	}
	float amp_mod 	= 1.0;//calc_amp_mod();

	// add osc sound
	if ( StA[ MbIdInstrument].state.play )
		add_mono( instrument_osc, StA[ MbIdInstrument ].Amp * amp_mod, MbIdInstrument );
	if ( StA[ MbIdNotes 	].state.play )
		add_mono( notes_osc, StA[ MbIdNotes ].Amp 			* amp_mod, MbIdNotes );
	if ( StA[ MbIdKeyboard	].state.play )
		add_mono( keyboard_osc, StA[MbIdKeyboard].Amp 		* amp_mod, MbIdKeyboard );

	// add StA sound
	for ( uint DAid : MemIds )// scan rec_ids and exclude notes from being overwritten by store_block
	{
		Data_t* read_data = StA[ DAid ].Get_next_block();
		if ( read_data )
		{
			add_mono( read_data, StA[ DAid ].Amp * amp_mod, DAid );
		}
	}

	// store sound to record StA
	int store_id = get_store_id();
	if( store_id >= 0 )
		StA[ store_id ].Store_block( Mono.Data );

	// push sound to audio server
	stereo_out( shm_addr );
};

void Mixer_class::Test()
{
	TEST_START( className );
	assert( check_range(volume_range, (uint8_t)   2 ) == 2 );
	assert( check_range(volume_range, (uint8_t) 112 ) == 100 );

	stereo_t* shm_addr = DaTA->ShmAddr_0;

	sds_master->audioframes = max_frames;

	Set_master_volume( 100, FIXED, 1 );
	Set_master_volume( 75 , SLIDE, 1 );
	stereo_out( shm_addr );
	ASSERTION( past_volume == future_volume, "start_volume", (int)past_volume, (int)future_volume);

	Set_master_volume( 50, FIXED, 1 );
	Set_master_volume( 75, SLIDE, 1 );
	stereo_out( shm_addr );
	ASSERTION( past_volume == future_volume, "start_volume", (int)past_volume, (int)future_volume);

	Set_master_volume( 50, FIXED, 10 );
	Set_master_volume( 75, SLIDE, 25 );
	stereo_out( shm_addr );

	ASSERTION( past_volume == future_volume, "start_volume", (int)past_volume, (int)future_volume);

	Set_master_volume( 50, FIXED, 20 );
	Set_master_volume( 0, SLIDE, 20 );
	stereo_out( shm_addr );
	stereo_out( shm_addr );
	stereo_out( shm_addr );
	stereo_out( shm_addr );
	ASSERTION( past_volume == future_volume, "start_volume", (int)past_volume, (int)future_volume);

	Mono.Set_Loglevel( TEST, true );
	Mono_out.Set_Loglevel( TEST, true );
	Out_L.Set_Loglevel( TEST, true );
	Out_R.Set_Loglevel( TEST, true );
	for ( Memory& sta : StA )
		sta.Set_Loglevel( TEST, true );
	Mono.Memory_base::Info();

	TEST_END( className );


}
