/*
 * mixer.cpp
 *
 *  Created on: Dec 31, 2023
 *      Author: sirius
 */

#include <Mixer.h>

//-------------------------------------------------------------------------------------------------


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
//	cout << max_counts << " " << (int)this->step << " " << abs(diff) << " " << abs(diff)/abs(step) << abs(10)/abs(10) << endl;
}


void Loop_class::Next( uint8_t& addr )
{
	auto inc = [this]()
	{
		counts++;
		counter = counter + step;
	};

	if ( counts == max_counts ) active = false;
	if ( active )
	{
		inc();
		addr = counter;
	}
};

void Loop_class::Next( uint16_t& addr )
{
	auto inc = [this]()
	{
		counts++;
		counter = counter + step;
	};

	if ( counts == max_counts ) active = false;
	if ( active )
		inc();
	addr = counter;
	//	cout << counter  << endl;
};

void Loop_class::Test()
{
	uint16_t l;
	uint8_t 	 ch;

	Loop_class Loop;

	Loop.Start(10, 20, 1);
	for( int i = 0; i<20; i++ ) Loop.Next( l);
	assert( l == 20 );

	Loop.Start(20, 10, 1);
	for( int i = 0; i<20; i++ ) Loop.Next( l);
	assert( l == 10 );

	Loop.Start(20, 10, 1);
	for( int i = 0; i<20; i++ ) Loop.Next( ch);
	assert( ch == 10 );


	Loop.Start(20, 10, -1);
	for( int i = 0; i<20; i++ ) Loop.Next(ch);
	assert( ch == 10 );

	Loop.Start(0, 0, 0);
	for( int i = 0; i<10; i++ ) Loop.Next(l);
	assert( l == 0 );

	Loop.Start(0, 0, -1);
	for( int i = 0; i<10; i++ ) Loop.Next( l );
	assert( l == 0 );

	Loop.Start(10, 20, -1);
	for( int i = 0; i<20; i++ ) Loop.Next( l );
	assert( l == 20 );



};

//-------------------------------------------------------------------------------------------------


Mixer_class::Mixer_class( ifd_t* sds )
: Logfacility_class("Mixer")
{
	cout << "Init Mixer_class" << endl;
	this->sds = sds;
	for( uint n : MemIds )
	{
		Storage_class DataMem;
		DataMem.Id = n;
		StA.push_back(DataMem);
	}

	StA_struct_t usr_conf = {"temp"		, max_frames*tmpduration };
	StA_struct_t ext_conf = {"External"	, max_frames*recduration };

	for( uint n : UsrIds )
		StA[n].Setup(usr_conf);
	StA[MbIdExternal].Setup(ext_conf);

	for ( uint n : MemIds )
		StA[n].Info();

	Mono.Info		( "Mono data");
	Mono_out.Info	( "Wave display data");
	Out_L.Info		( "Output Stereo Left");
	Out_R.Info		( "Output Stereo Right");

};

Mixer_class::~Mixer_class()
{
	Clear_StA_status( sds->StA_state );
	sds->mixer_status.external	= false;

};


void Mixer_class::clear_memory()
{
	// clear temporary memories
	Out_L.clear_data(0);
	Out_R.clear_data(0);
	Mono.clear_data(0); // Wavedisplay mono col data
}

void Mixer_class::Clear_StA_status( StA_state_arr_t& state_arr )
{
	Comment( INFO, "Reset SDS state" );
	std::ranges::for_each( state_arr, 	[ ]( auto& state ){ state.store = false;});
	for ( Storage_class sta : StA )
		sta.Reset_counter();
}

void Mixer_class::Volume_control( ifd_t* sds )
{
	master_amp_loop.Next( sds->Master_Amp );
	master_volume = sds->Master_Amp;

	uint8_t amp = StA[MbIdExternal].Amp;
	record_amp_loop.Next( amp );
	StA[MbIdExternal].Amp = amp;
	sds->StA_amp_arr[MbIdExternal] = amp;
}

void Mixer_class::Set_mixer_state( const uint& id, const bool& play )
{
	switch ( id )
	{
		case MbIdInstrument :	{ status.instrument = play; break; }
		case MbIdNotes 		:	{ status.notes = play; 		break; }
		case MbIdKeyboard	: 	{ status.kbd = play; 		break; }
		case MbIdExternal 	:	{ status.external = play; 	break; }
		default				:	break;
	}

	if (( play ) and ( id != MbIdInstrument ))
		 status.play = true;

	StA[id].Play_mode( play );

};
void Mixer_class::Update_ifd_status_flags( ifd_t* sds )
{

	sds->mixer_status =  status;
	for ( uint id :  MemIds )
	{
		sds->StA_state[id] 	=  StA[id].state;
		sds->StA_amp_arr[id] 	=  StA[id].Amp;
	}
}


void Mixer_class::add_mono(Data_t* Data, const uint8_t& sta_amp, const uint& id )
{								// 0  1  2  3  In Kb Nt Ex
	const array<int,8> phase_r = { 1, 1,-1,-1, 1, 1, 1,-1 };
	const array<int,8> phase_l = { 1, 1,-1,-1,-1, 1,-1,-1 };

	assert( phase_r.size() == StA.size() );
	assert( sta_amp <= 100 );

	float volpercent=sta_amp/100.0;
	float Data_r = (phase_r[id] * volpercent);
	float Data_l = (phase_l[id] * volpercent);

	for( buffer_t n = 0; n < max_frames; n++)
	{
		Out_L.Data[n] 		+= rint( Data[n]*Data_l );
		Out_R.Data[n] 		+= rint( Data[n]*Data_r );
		Mono.Data[n]  		+= rint( Data[n]*volpercent );	// collect mono data by using sta-amp
	}
}

void Mixer_class::stereo_out( stereo_t* data, const uint8_t& master_vol )
{
	float out_percent=master_vol/100.0;
	for( buffer_t n = 0; n < max_frames; n++ )
	{
		data[n].left 	= rint( Out_L.Data[n] * out_percent );
		data[n].right 	= rint( Out_R.Data[n] * out_percent );
		Mono_out.Data[n]= ( Mono.Data[n]  * out_percent ); // Wavedisplay mono data
	}
}

void Mixer_class::Store_noteline( uint8_t arr_id, Note_class* Notes )
{
	while ( composer > 0 )
	{
		cout << dec << composer << " " << arr_id << endl;
		Notes->Generate_note_chunk( );
		StA[ arr_id ].Store_block( Notes->main.Mem.Data );
		composer--;
	}
	StA[ arr_id ].Record_mode( false );
	StA[ arr_id ].Play_mode( true );

}


void Mixer_class::Add_Sound( Data_t* 	instrument,
							 Data_t* 	keyboard,
							 Data_t* 	notes,
							 stereo_t* 	shm_addr )
{
	auto get_store_id = [ this ]()
		{
			for ( int id : MemIds  )
				if ( StA[id].state.store )
					return id;
			return -1;
		};

	auto calc_amp_mod = [ this ]()
		{
			float sum 	= 0;
			uint count 	= 0;
			for( uint id : MemIds )
			{
				if (( StA[id].state.play ) )
				{
					if (( id == 4 ) or ( id == 5  ) or ( id == 6 ))
						{ sum += StA[id].Amp; count++; }
					else
						if ( StA[id].state.store )
							{ sum += StA[id].Amp; count++; }
				}
			}
			if ( count == 1 ) 	return (float) 1.0;
			if ( sum == 0 ) 	return (float)1.0;
			float 	mean = 100.0 / sum;
			return 	mean;
		};

	clear_memory();

	if ( status.mute )
	{
		stereo_out( shm_addr, master_volume );
		return;
	}

	float amp_mod 	= calc_amp_mod();

	// add osc sound
	if ( StA[ MbIdInstrument].state.play )
		add_mono( instrument, StA[ MbIdInstrument ].Amp * amp_mod, MbIdInstrument );
	if ( StA[ MbIdNotes 	].state.play )
		add_mono( notes, StA[ MbIdNotes ].Amp 			* amp_mod, MbIdNotes );
	if ( StA[ MbIdKeyboard	].state.play )
		add_mono( keyboard, StA[MbIdKeyboard].Amp 		* amp_mod, MbIdKeyboard );
	StA[ MbIdKeyboard	].state.play = false;
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
	stereo_out( shm_addr, master_volume );
	status.play = false;
};

void Mixer_class::Test()
{
	Set_Loglevel(TEST, true );
	Mono.Set_Loglevel( TEST, true );
	Mono_out.Set_Loglevel( TEST, true );
	Out_L.Set_Loglevel( TEST, true );
	Out_R.Set_Loglevel( TEST, true );
//	stereo->Set_Loglevel( TEST, true );
	for ( Memory& sta : StA )
		sta.Set_Loglevel( TEST, true );
	Mono.Info();

	Comment( TEST, "Testing Mixer_class" );


}
