/*
 * apptest.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: sirius
 */

#include <Utilities.h>
#include "../Synth/Synthesizer.h"

auto test_cycle2_range = [  ]( range_T<int> range  )
{
	ASSERTION( range.len > 0, "range.len", range.len, ">0" );
	string
	range_str = to_string( range.min) + "..." + to_string(range.max) ;
	int value0 = range.min + range.len/2;
	int value1 = check_cycle2(range, value0 , "" );
	Assert_equal( value0 , value1, range_str );
	int value = range.max;
	ASSERTION( 	check_cycle2(range, value , "" ) == value, range_str,
				check_cycle2(range, value, "" ), value );
	 value = range.min;
	ASSERTION( 	check_cycle2(range, value , "" ) == value, range_str,
				check_cycle2(range, value, "" ), value );
	 value = range.min-1;
	ASSERTION( 	check_cycle2(range, value , "" ) == range.max -1, range_str,
				check_cycle2(range, value, "" ), range.max - 1 );
	 value = range.max + 1;
	ASSERTION( 	check_cycle2(range, value , "" ) == range.min +1, range_str,
				check_cycle2(range, value, "" ), range.min+1 );
	 value = range.max + 2*range.len +1;
	ASSERTION( 	check_cycle2(range, value , "" ) == range.min + 1, range_str,
				check_cycle2(range, value, "" ), range.min+1 );
	 value = range.min - 2*range.len - 1;
	ASSERTION( 	check_cycle2(range, value , "" ) == range.max - 1, range_str,
				check_cycle2(range, value, "" ), range.max-1 );
	ASSERTION( range.len == abs(range.max-range.min), "", range.len, range.max-range.min);
};

bool
isopen = false;
void SynthesizerTestCases()
{


	Logfacility_class		Log( "SynthesizerTest" );
	Log.Set_Loglevel( TEST, true );


	process_t				Process			{};
	file_structure			fs				{};
	Config_class			Cfg				{ Process.name, &fs };
	Semaphore_class			Sem				{ Cfg.Config.Sem_key };
	Dataworld_class 		DaTA			{ Process.AppId, &Cfg, &Sem };
	interface_t*			sds				= DaTA.GetSdsAddr();
	DaTA.Sds_p->Reset_ifd();

	Wavedisplay_class		Wavedisplay{ DaTA.Sds_p};
	Wavedisplay_class*		wd_p = &Wavedisplay;
	Instrument_class 		Instrument( sds, wd_p );

	Application_class		App( &DaTA );
	Mixer_class				Mixer{&DaTA, wd_p };

	Note_class 				Notes{ &Instrument, &Mixer.StA[ STA_NOTES ] };
	Musicxml_class			MusicXML{};

	Keyboard_class			Keyboard( 	&Instrument, &Mixer.StA[ STA_KEYBOARD], &Notes );
	External_class 			External( 	&Mixer.StA[ STA_EXTERNAL],
										DaTA.Cfg_p);
	ProgressBar_class		ProgressBar( &sds->RecCounter );
	Time_class				Timer( &DaTA.sds_master->time_elapsed );
	Statistic_class 		Statistic{ Log.className };
	coutf << "SynthesizerTestCases" << endl;

	Cfg.Test();
	DaTA.Sds_p->Reset_ifd();

	Shm_base Shm_test;
	Shm_test.Test_Memory();
	Frequency_class Frq {};
	Frq.TestFrequency();

	String 					TestStr{""};
	Oscillator 				TestOsc{ INSTRROLE, OSCID, monobuffer_bytes };


	Log.Init_log_file();
	Log.Test_Logging();


//	App.Init_Sds( );

	range_T<int> range_test { 1,4 };
	test_cycle2_range( range_test );
	int value = check_cycle( range_test, 1, "range_test" );
	Assert_equal( value, 1, "range test " );
	value = check_cycle( range_test, 4, "range_test" );
	assert( value == 4 );
	value = check_cycle( range_test, 5, "range_test" );
	Assert_equal( value, 1, "range test " );

	App.DaTA->Appstate.Announce();
    std::set<string> abc{"a","b","c"};
    assert( abc.contains("b"));
	TestStr.TestString();

	Spectrum_class Spectrum {};
	Spectrum.Test_Spectrum();

	Log.Comment(TEST, "entering test classes ");


	Notes.TestNoteBase();

	Notes.Test();
	MusicXML.Test();

	TestOsc.DynFrequency.TestFrq();

	Mixer.DynVolume.TestVol( );

	Mixer.TestMixer();

	Timer.Start();

	Timer.Stop();
	cout << "Run osc group in " << Timer.Time_elapsed() << " milli seconds" <<  endl;

	Mixer.Test_Logging();

	Keyboard.Kbd_pitch_Test();
	Keyboard.Test();

	External.Test_External();

	set<string> Ss { "ab", "cd", "ef", "gh" };
	assert( Ss.contains("cd"));
	assert( not Ss.contains("de"));

	Timer.Test();

	DaTA.Sem_p->Test();

	DaTA.Sds_p->Reset_ifd();
	Instrument.Test_Instrument();
	Instrument.Oscgroup.Run_OSCs( 0 );


	Log.Test_Logging();

	System_Test();

	DaTA.Test_Dataworld();
	DaTA.Sds_p->Test_interface();

	Event_class				Event{
								&Instrument,
								&Notes,
								&Keyboard,
								&Mixer,
								&Wavedisplay,
								&DaTA,
								&External,
								&ProgressBar,
								&MusicXML};

	Event.TestHandler();

	DaTA.Sds_p->Restore_ifd();
//	Log.Set_Loglevel( TEST, true );


}

