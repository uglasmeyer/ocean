/*
 * apptest.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: sirius
 */

#include <Synthesizer.h>

void SynthesizerTestCases()
{
	Shm_base Shm_test{0};
	Shm_test.Test_Memory();

	Logfacility_class		Log( "Synthesizer test" );
	DirStructure_class		Dir;

	Dataworld_class			DaTA( SYNTHID );
	DaTA.Sds_p->Reset_ifd();

	Wavedisplay_class		Wavedisplay{ DaTA.Sds_p};
	Wavedisplay_class*		wd_p = &Wavedisplay;
	Application_class		App( &DaTA );
	interface_t*			sds = DaTA.GetSdsAddr();
	Mixer_class				Mixer{&DaTA, wd_p };


	Instrument_class 		Instrument( DaTA.sds_master, wd_p );
	Note_class 				Notes{ wd_p };
	Musicxml_class			MusicXML{};

	Keyboard_class			Keyboard( 	&Instrument );
	External_class 			External( 	&Mixer.StA[ MbIdExternal],
										DaTA.Cfg_p);
	ProgressBar_class		ProgressBar( &sds->RecCounter );
	Time_class				Timer( &DaTA.sds_master->time_elapsed );
	Statistic_class 		Statistic{ Log.className };

	Semaphore_class*		Sem	= DaTA.Sem_p;
//	uint8_t ch;
//	Loop_class 				Loop{ &ch };
	String 					TestStr{""};
	Oscillator 				TestOsc{ osc_struct::INSTRID, osc_struct::OSCID };

	Log.TEST_START( "Application " );

	App.Init_Sds( );


	App.DaTA->Appstate.Announce();
    std::set<string> abc{"a","b","c"};
    assert( abc.contains("b"));
	TestStr.TestString();

	Spectrum_class Spectrum {};
	Spectrum.Test_Spectrum();

	Log.Set_Loglevel( TEST, true);
	Log.Comment(TEST, "entering test classes ");

	Dir.Test();

	Log.Init_log_file();
	Log.Test_Logging();

//	Loop.Test();


	Notes.Test();

	TestOsc.Test_wf();
	TestOsc.DynFrequency.TestFrq();

	Mixer.Test();

	Timer.Start();

	Timer.Stop();
	cout << "Run osc group in " << Timer.Time_elapsed() << " milli seconds" <<  endl;

	Mixer.Test_Logging();

	Keyboard.Test();

	External.Test_External();


	set<string> Ss { "ab", "cd", "ef", "gh" };
	assert( Ss.contains("cd"));
	assert( not Ss.contains("de"));

	Timer.Test();

	Sem->Test();

	Instrument.Test_Instrument();
	Instrument.Oscgroup.Run_OSCs( 0 );

	Log.Test_Logging();
	Frequency_class Frq {};
	Frq.TestFrequency();

	DaTA.Reg.Test_Register();
	DaTA.Test_Dataworld();
	System_Test();
	DaTA.Sds_p->Test_interface();

	Event_class				Event{
								&Instrument,
								&Notes,
								&Mixer,
								&Wavedisplay,
								&DaTA,
								&External,
								&ProgressBar,
								&MusicXML};

	Event.TestHandler();

	Log.TEST_END( "Application " );
}


