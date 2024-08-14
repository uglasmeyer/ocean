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


void Loop_class::start( uint16_t beg, uint16_t end, uint8_t step )
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


void Loop_class::next( char* addr )
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

void Loop_class::next( uint16_t* addr )
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

void Loop_class::test()
{
	uint16_t l;
	char 	 ch;

	Loop_class Loop;

	Loop.start(10, 20, 1);
	for( int i = 0; i<20; i++ ) Loop.next( &l);
	assert( l == 20 );

	Loop.start(20, 10, 1);
	for( int i = 0; i<20; i++ ) Loop.next( &l);
	assert( l == 10 );

	Loop.start(20, 10, 1);
	for( int i = 0; i<20; i++ ) Loop.next( &ch);
	assert( ch == 10 );


	Loop.start(20, 10, -1);
	for( int i = 0; i<20; i++ ) Loop.next(&ch);
	assert( ch == 10 );

	Loop.start(0, 0, 0);
	for( int i = 0; i<10; i++ ) Loop.next(&l);
	assert( l == 0 );

	Loop.start(0, 0, -1);
	for( int i = 0; i<10; i++ ) Loop.next( &l );
	assert( l == 0 );

	Loop.start(10, 20, -1);
	for( int i = 0; i<20; i++ ) Loop.next( &l );
	assert( l == 20 );




//	exit(1);
};

//-------------------------------------------------------------------------------------------------


Mixer_class::Mixer_class( )
: Logfacility_class("Mixer")
{
	cout << "Init Mixer_class" << endl;
//	this->stereo 			 = stereo;

	StA_struct_t tmp_conf = {"temp", 	max_frames*tmpduration	, 'a'};
	StA_struct_t ext_conf = {"External", max_frames*recduration	, 's'};
	StA_struct_t nte_conf = {"Notes", 	max_frames				, 'a'};


	for( uint n = 0; n<MbSize; n++)
	{
		Storage_class DataMem;
		DataMem.Id = n;
		StA.push_back(DataMem);
	}
	for( uint n = 0; n < (MbSize - 2) ; n++ )
		StA[n].setup(tmp_conf);
	StA[MbIdExternal].setup(ext_conf);
	StA[MbIdNotes].setup(nte_conf);

	for ( uint n = 0; n < MbSize; n++ )
		StA[n].Info();


	Mono_tmp.Info	( "Temporary Mono data");
	Mono_out.Info	( "Output Mono data");
	Out_L.Info		( "Output Stereo Left");
	Out_R.Info		( "Output Stereo Right");
//	stereo->Info	( "Record Memory Stereo" );

};




void Mixer_class::clear_memory()
{
	// clear temporary memories
	Out_L.clear_data(0);
	Out_R.clear_data(0);
	Mono_tmp.clear_data(0); // Wavedisplay mono col data
}

void Mixer_class::add_mono(Data_t* Data, uint8_t volume, uint id )
{
	const array<int,8> phase_r = {10, 7,  0, -7, -10, -7,   0,  7 };
	const array<int,8> phase_l = { 0, 7, 10,  7,   0, -7, -10, -7 };

	id = 3;
	assert( volume <= 100 );
	float volpercent=volume/100.0;
	Data_t Data_r = (phase_r[id]*volpercent)/10.0;
	Data_t Data_l = (phase_l[id]*volpercent)/10.0;

	for( buffer_t n = 0; n < max_frames; n++)
	{
		Out_L.Data[n] += Data[n]*Data_l;
		Out_R.Data[n] += Data[n]*Data_r;
		Mono_tmp.Data[n]  += Data[n]*volpercent; 		// collect mono data - wavedisplay
	}
}

void Mixer_class::stereo_out( stereo_t* data, uint8_t master_vol )
{
	float out_percent=master_vol/100.0;
	for( buffer_t n = 0; n < max_frames; n++ )
	{
		data[n].left 	= rint( Out_L.Data[n] * out_percent );
		data[n].right 	= rint( Out_R.Data[n] * out_percent );
		Mono_out.Data[n]= ( Mono_tmp.Data[n]*out_percent ); // Wavedisplay mono data
	}
}

void Mixer_class::add_noteline( uint8_t arr_id, Note_class* Notes )
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

void Mixer_class::add(  Instrument_class* instrument, stereo_t* shm_addr, bool rec )
{
	// store the result into a local buffer, before making it available to the audio server

	clear_memory();

	if ( not status.mute )
	{
		add_mono( instrument->main.Mem.Data, master_volume, 0 );
		for( int store_id : UsrIds )
		{
			if ( StA[ store_id ].status.store )
				StA[ store_id ].store_block( instrument->main.Mem.Data ); // store data with amp=100
		}
	}

	if ( StA[MbIdNotes].status.play )
	{
		add_mono( StA[MbIdNotes].Data, StA[MbIdNotes].Amp, MbIdNotes );
	}
	// store data potentially and add read_data to output
	status.play = false; // set loop default
	for ( uint DAid : RecIds )// scan rec_ids and exclude notes from being overwritten by store_block
	{
		Storage_class*  DA = &StA[ DAid ];

		if( StA[MbIdNotes].status.play ) // ignore flat instrument if notes are available
			DA->store_block( StA[MbIdNotes].Data ); // only one of 7 is available for data

		Data_t* read_data = DA->get_next_block();
		if ( read_data )
		{
			status.play = true; // any active mem array triggers total synchronized play mode
			uint8_t Amp = (int)DA->Amp;
			add_mono( read_data, Amp, DAid );
		}
	}

	status.external 	= (	StA[MbIdExternal].status.play or
							StA[MbIdExternal].status.store );
	status.notes 		= 	StA[MbIdNotes].status.play;
	stereo_out( shm_addr, master_volume );
};

void Mixer_class::test()
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
