/*
 * apptest.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: sirius
 */

#include <Synthesizer.h>

bool
isopen = false;
void SynthesizerTestCases()
{
	coutf << "SynthesizerTestCases" << endl;
	Shm_base Shm_test;
	Shm_test.Test_Memory();

	Logfacility_class		Log( "SynthesizerTest" );
	Log.Set_Loglevel( TEST, true );


	process_t				Process{  };
	Config_class			Cfg				{ Process.name };
	Semaphore_class			Sem				{ Cfg.Config.Sem_key };
	Dataworld_class 		DaTA			{ Process.AppId, &Cfg, &Sem };	DaTA.Sds_p->Reset_ifd();

	Wavedisplay_class		Wavedisplay{ DaTA.Sds_p};
	Wavedisplay_class*		wd_p = &Wavedisplay;
	Application_class		App( &DaTA );
	interface_t*			sds = DaTA.GetSdsAddr();
	Mixer_class				Mixer{&DaTA, wd_p };


	Instrument_class 		Instrument( DaTA.sds_master, wd_p );
	Note_class 				Notes{ wd_p };
	Musicxml_class			MusicXML{};

	Keyboard_class			Keyboard( 	&Instrument, &Mixer.StA[ STA_KEYBOARD] );
	External_class 			External( 	&Mixer.StA[ STA_EXTERNAL],
										DaTA.Cfg_p);
	ProgressBar_class		ProgressBar( &sds->RecCounter );
	Time_class				Timer( &DaTA.sds_master->time_elapsed );
	Statistic_class 		Statistic{ Log.className };

	Frequency_class Frq {};
	Frq.TestFrequency();

	String 					TestStr{""};
	Oscillator 				TestOsc{ osc_struct::INSTRID, osc_struct::OSCID, monobuffer_bytes };


	Log.Init_log_file();
	Log.Test_Logging();


//	App.Init_Sds( );


	App.DaTA->Appstate.Announce();
    std::set<string> abc{"a","b","c"};
    assert( abc.contains("b"));
	TestStr.TestString();

	Spectrum_class Spectrum {};
	Spectrum.Test_Spectrum();

	Log.Comment(TEST, "entering test classes ");


	Notes.TestNoteBase();

	Notes.Test();

	TestOsc.DynFrequency.TestFrq();

	Mixer.DynVolume.TestVol( );

	Mixer.TestMixer();

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

	DaTA.Sem_p->Test();

	Instrument.Test_Instrument();
	Instrument.Oscgroup.Run_OSCs( 0 );


	Log.Test_Logging();

	System_Test();

	DaTA.Reg.Test_Register();
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
	Log.Set_Loglevel( TEST, true );
}

#include <Interpreter.h>
void ComposerTestCases()
{
	process_t				Process{  };

	Config_class			Cfg				{ Process.name };
	Semaphore_class			Sem				{ Cfg.Config.Sem_key };
	Dataworld_class 		DaTA			{ Process.AppId, &Cfg, &Sem };
	Application_class		App( &DaTA );
	Interpreter_class 		Compiler( &DaTA );
	Variation_class 		Variation {};
	vector<int>				pos_stack {};
	String 					Str{""};
	vector<line_struct_t> 	Program{};

	string arg = "-t";
	char* args = arg.data();
	App.Start(1, &args );
	Variation.Test();
	Charset_class A("abdefabdef");
	A.test();

	Compiler.Test( );


	Processor_class Processor{ DaTA.Sds_p, &DaTA.Appstate };
	Processor.Test_Processor();
	Processor.Set_Loglevel( TEST, true );

}

