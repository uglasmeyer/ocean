/*
 * mixer.cpp
 *
 *  Created on: Dec 31, 2023
 *      Author: sirius
 */

#include <mixer.h>
#include <synthmem.h>
#include <array>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>

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


void Loop_class::Next( uint8_t* addr )
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
		*addr = counter;
	}
};

void Loop_class::Next( uint16_t* addr )
{
	auto inc = [this]()
	{
		counts++;
		counter = counter + step;
	};

	if ( counts == max_counts ) active = false;
	if ( active )
		inc();
	*addr = counter;
	//	cout << counter  << endl;
};

void Loop_class::Test()
{
	uint16_t l;
	uint8_t 	 ch;

	Loop_class Loop;

	Loop.Start(10, 20, 1);
	for( int i = 0; i<20; i++ ) Loop.Next( &l);
	assert( l == 20 );

	Loop.Start(20, 10, 1);
	for( int i = 0; i<20; i++ ) Loop.Next( &l);
	assert( l == 10 );

	Loop.Start(20, 10, 1);
	for( int i = 0; i<20; i++ ) Loop.Next( &ch);
	assert( ch == 10 );


	Loop.Start(20, 10, -1);
	for( int i = 0; i<20; i++ ) Loop.Next(&ch);
	assert( ch == 10 );

	Loop.Start(0, 0, 0);
	for( int i = 0; i<10; i++ ) Loop.Next(&l);
	assert( l == 0 );

	Loop.Start(0, 0, -1);
	for( int i = 0; i<10; i++ ) Loop.Next( &l );
	assert( l == 0 );

	Loop.Start(10, 20, -1);
	for( int i = 0; i<20; i++ ) Loop.Next( &l );
	assert( l == 20 );



};

//-------------------------------------------------------------------------------------------------


Mixer_class::Mixer_class( )
: Logfacility_class("Mixer")
{
	cout << "Init Mixer_class" << endl;

	for( uint n : MemIds )
	{
		Storage_class DataMem;
		DataMem.Id = n;
		StA.push_back(DataMem);
	}

	StA_struct_t tmp_conf = {"temp"		, max_frames*tmpduration	, 'a'};
	StA_struct_t ext_conf = {"External"	, max_frames*recduration	, 's'};
	StA_struct_t nte_conf = {"Notes"	, max_frames				, 'a'};

	for( uint n : UsrIds )
		StA[n].setup(tmp_conf);
	StA[MbIdExternal].setup(ext_conf);
	StA[MbIdNotes].setup(nte_conf);

	for ( uint n : MemIds )
		StA[n].Info();


	Mono_tmp.Info	( "Temporary Mono data");
	Mono_out.Info	( "Output Mono data");
	Out_L.Info		( "Output Stereo Left");
	Out_R.Info		( "Output Stereo Right");

};




void Mixer_class::clear_memory()
{
	// clear temporary memories
	Out_L.clear_data(0);
	Out_R.clear_data(0);
	Mono_tmp.clear_data(0); // Wavedisplay mono col data
}

void Mixer_class::add_mono(Data_t* Data, const uint8_t& volume, const uint& id )
{
	const array<int,8> phase_r = {10, 7,  0, 7, 10, 7,  0, 7 };
	const array<int,8> phase_l = { 0, 7, 10, 7,  0, 7, 10, 7 };

	assert( phase_r.size() == StA.size() );
	assert( volume <= 100 );

	float volpercent=volume/100.0;
	Data_t Data_r = (phase_r[id]*volpercent)/10.0;
	Data_t Data_l = (phase_l[id]*volpercent)/10.0;

	for( buffer_t n = 0; n < max_frames; n++)
	{
		Out_L.Data[n] 		+= Data[n]*Data_l;
		Out_R.Data[n] 		+= Data[n]*Data_r;
		Mono_tmp.Data[n]  	+= Data[n]*volpercent; 		// collect mono data - wavedisplay
	}
}

void Mixer_class::stereo_out( stereo_t* data, const uint8_t& master_vol )
{
	float out_percent=master_vol/100.0;
	for( buffer_t n = 0; n < max_frames; n++ )
	{
		data[n].left 	= rint( Out_L.Data[n] * out_percent );
		data[n].right 	= rint( Out_R.Data[n] * out_percent );
		Mono_out.Data[n]= ( Mono_tmp.Data[n]  * out_percent ); // Wavedisplay mono data
	}
}

void Mixer_class::Store_noteline( uint8_t arr_id, Note_class* Notes )
{
	while ( composer > 0 )
	{
		cout << dec << composer << " " << arr_id << endl;
		Notes->Generate_note_chunk( &StA[ MbIdNotes] ); // max_sec duration
		StA[ arr_id ].store_block( StA[ MbIdNotes ].Data);
		composer--;
	}
	assert( StA[ arr_id ].store_counter > 0 );
	StA[ arr_id ].record_mode( false );

}


void Mixer_class::Add_Sound(Instrument_class* 	instrument,
							stereo_t* 			shm_addr )
{
	auto StA_id = [ this ]()
		{
			for ( int id : UsrIds )
				if ( StA[id].status.store )
					return id;
			return -1;
		};


	// store the result into a local buffer, before making it available to the audio server
	clear_memory();

	if ( not status.mute )
		add_mono( instrument->main.Mem.Data, master_volume, 0 );
	if (( status.notes ))
		add_mono( StA[ MbIdNotes ].Data, master_volume, MbIdNotes );

	int store_id = StA_id();
	if( store_id >= 0 )
		StA[ store_id ].store_block( Mono_tmp.Data );

	status.play 	= false; // set loop default
	for ( uint DAid : RecIds )// scan rec_ids and exclude notes from being overwritten by store_block
	{
		Storage_class*  DA = &StA[ DAid ];

		Data_t* read_data = DA->get_next_block();
		if ( read_data )
		{
			status.play = true; // any active mem array triggers total synchronized play mode
			add_mono( read_data, DA->Amp, DAid );
		}
	}

	stereo_out( shm_addr, master_volume );
};

void Mixer_class::Test()
{
	Set_Loglevel(TEST, true );
	Mono_tmp.Set_Loglevel( TEST, true );
	Mono_out.Set_Loglevel( TEST, true );
	Out_L.Set_Loglevel( TEST, true );
	Out_R.Set_Loglevel( TEST, true );
//	stereo->Set_Loglevel( TEST, true );
	for ( Memory& sta : StA )
		sta.Set_Loglevel( TEST, true );
	Mono_tmp.Info();

	Comment( TEST, "Testing Mixer_class" );


}
