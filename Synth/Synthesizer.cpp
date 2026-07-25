/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/


#include <Synth/Synthesizer.h>
#include <Appsymbols.h>


Trigger_class			BeatTrigger		{};
Wavedisplay_class 		Wavedisplay		{ Sds };
Wavedisplay_class*		Wd_p			= &Wavedisplay;
Instrument_class 		Instrument		{ &DaTA, &Wavedisplay };
Mixer_class				Mixer			{ &DaTA, &Wavedisplay } ;
External_class 			External		( &Mixer.StA[ STA_EXTERNAL], Cfg_p, sds_master );
Capture_class			Capture			{ &External };
CutDesk_class			CutDesk			{ &Mixer };
Musicxml_class			Musicxml		{ DaTA.Cfg_p->fs };
Note_class 				Notes			{ &DaTA, &Instrument, &Mixer.StA[ STA_NOTES ], &Musicxml };
Keyboard_class			Keyboard		( &DaTA, &Instrument, &Mixer.StA[ STA_KEYBOARD], &Notes );
ProgressBar_class		ProgressBar		( &sds_master->RecCounter );
Time_class				Timer			( &sds_p->time_elapsed );
Event_class				Event			{
										&Instrument,
										&Notes,
										&Keyboard,
										&Mixer,
										&Wavedisplay,
										&DaTA,
										&External,
										&ProgressBar,
										&CutDesk,
										&Capture
										};

Data_t*					Kbd_Data		= Mixer.StA[STA_KEYBOARD ].Data;
Data_t*					Notes_Data		= Mixer.StA[STA_NOTES ].Data;
Data_t*					Instr_Data		= Instrument.Oscgroup.member[OSCID]->Mem.Data;
const SEMID_e 			Sync_Semaphore 	= SemIds[ DaTA.SDS_Id ];
extern void 			SynthesizerTest	();

StateId_e 				Audio_state		= DEFAULT;

void AppState_update()
{
	StateId_e audio_state 	= Appstate->GetState( sds_master, AUDIOID );
	bool changed			= not ( Audio_state == audio_state );
	Audio_state				= audio_state;
	if ( changed )
	{
		Log.Comment( LOG_e::INFO, "Audioserver state is: ", App.sdsstateName( audio_state ) );
		cout.flush();
	}
	bool	audio_is_running	= ( Audio_state == RUNNING );

	if( audio_is_running )
	{
		if( Appstate->IsSleeping() )
			Appstate->SetRunning();
	}
	else
	{
		if( Appstate->IsRunning() )
			Appstate->SetSleeping();
	}
}

void SetLogLevels()
{
	Log.Show_loglevel();
}

void activate_sds()
{
	eventkey_vec_t init_keys =
	{
		// avoid toggle commands here
		// all events are executed on the prepared set of data in the SDS
		SETINSTRUMENTKEY,
		SETWAVEDISPLAYKEY,
		BEATCLOCK_KEY,
		SETSTA_KEY
	};
	// the order of initialization matters
	Keyboard.Enable				( App.properties.keyboard ) ;
	Sds->Prepare_sds			();
	sds_p->eventDebug			= false;
	sds_p->StA_Id				= STAID_ALL;
	Wavedisplay.Set_WdData		();

	Event.eventbuffer.add		( init_keys );
	Event.eventbuffer.release	();
}

void update_WD_data()
{
	if( sds_p->WD_state.roleId == AUDIOROLE )
		return;
	Wavedisplay.Set_WdData	();
	Mixer.StA_Wdcursor		();
	Wavedisplay.WriteData	();
	DaTA.EmitEvent			( READWAVEDATAFLAG, "update display data" );
}

Time_class Sound			{};
long int time_elapsed		= 0;

#define ADDSOUND_RETURN \
	Event.State( StateId_e::ENABLED ); \
	update_WD_data(); \
	Sound.Set_Threshold( 125, &sds_p->overload ); \
	time_elapsed = Sound.Time_elapsed(); \
	return;

void add_sound()
{
	Sound.Start();
	Event.State( StateId_e::DISABLED );

	if(	CutDesk.LoopFnc() )
	{
		ADDSOUND_RETURN
	}

	if ( sds_p->WD_state.adsrmode ) // update adsr data
	{
		OSCID_e	oscid 			= sds_p->SpectrumTypeId;

		Wavedisplay.Set_WdData	();
		Instrument.Oscgroup.member[oscid]->Set_adsr( sds_p->adsr_arr[oscid] );
		Instrument.Oscgroup.member[oscid]->Adsr_OSC();
	}

	// advance instrument clock
	Instrument.Oscgroup.DataReset();
	Instrument.Oscgroup.RunOSCs( 0 );

	// set kbd beat clock
	Mixer.state.sync_kbd	=  Mixer.BeatClock( sds_p->beatClock );

	if ( Mixer.StA[STA_KEYBOARD].state.Play() )
	{
		Keyboard.Dispatcher	( Mixer.state.sync_kbd );
		Kbd_Data 			= Keyboard.ScanData();

		DaTA.EmitEvent( UPDATE_KBDDIALOG_FLAG, "Keyboad_Dialog_class::Setup_Widget" );
	}

	if ( Mixer.StA[STA_NOTES].state.Play() )
	{
		if( sds_p->NotesTypeId == XML_ID )
		{
			Notes.Generate_volatile_data( false );
		}
		else
		{
			Notes.Note_itr_start.SetActive( false );
			Notes.Note_itr_end.SetActive( false );
		}
		Notes_Data = Notes.ScanData();
	}

	ProgressBar.Update();

	Mixer.state.sync_adsr 	= Instrument.Sync( Mixer.state.composer );

	Mixer.Add_Sound			( Instr_Data, Kbd_Data,	Notes_Data );

	ADDSOUND_RETURN

}
Thread_class 	SyncAudio( DaTA.Sem_p, Sync_Semaphore, add_sound );
thread* 		SyncAudio_thread_p = nullptr;

void ApplicationLoop()
{
	Log.Comment(INFO, "Entering Application loop\n");

	DaTA.Sds_p->Commit(); // set flags to zero and update flag to true

	App.Ready();
	Keyboard.Show_help();

	while ( Appstate->IsRunning( sds_p, App.AppId ) )
	{
		Timer.Performance	( &sds_p->overload );
		Mixer.state 		= sds_p->mixer_state;
		Mixer.state.composer= sds_master->mixer_state.composer;
		AppState_update		();
		Event.Handler		();
		App.KeyboardKey		( true );
	} ;

	Log.Comment(INFO, "Exit Application loop");

	return; // Application loop
}

void sync_notes_fnc( )
{
//	Notes.Start_note_itr();
	NOTETYPE_e TypeID = sds_p->NotesTypeId;
	switch ( TypeID )
	{
		case XML_ID : { Notes.Generate_volatile_data( true ); break; }
		case NTE_ID : { Notes.Generate_cyclic_data();		; break; }
		default : { break; }
	} // switch  TypeID
	sds_p->NotesTypeId = TypeID;
}
Thread_class 	SyncNotes( DaTA.Sem_p, SEMAPHORE_SYNCNOTES, sync_notes_fnc );
thread* 		SyncNotes_thread_p = nullptr;

void read_notes_fnc( )
{
//	DaTA.Sds_p->Commit();
	if ( sds_p->NotesTypeId != XML_ID )
		return;
	string name 	= DaTA.Sds_p->Read_str( NOTENAMESTR );
	Notes.musicxml 	= Musicxml.XmlFile2notelist( name );
	if ( Notes.musicxml.scoreduration > 0 )
	{
		Notes.Set_xmlnotelist( Notes.musicxml.notelist );
		Notes.Generate_volatile_data( true );

		sds_p->StA_state_arr[STA_NOTES].play = true;
		DaTA.EmitEvent( NEWNOTESLINEFLAG, Notes.musicxml.instrument_name );

//		Notes.Start_note_itr();

		Log.Comment(INFO, "xml notes ", name, " loaded");
	}

}
Thread_class 	ReadNotes( DaTA.Sem_p, SEMAPHORE_INITNOTES, read_notes_fnc );
thread* 		ReadNotes_thread_p = nullptr;

void activate_logging()
{
	Notes.logmask[DEBUG] 	= true;
	Mixer.logmask[DEBUG]	= false;
}

int main( int argc, char* argv[] )

{
	App.Start( argc, argv );
	activate_logging();


	if ( Cfg_p->Config.test == 'y' )
	{
		Log.Set_Loglevel( TEST, true );
		Sem.Release( SEMAPHORE_STARTED );
		SynthesizerTest();
		Log.Show_loglevel();

		exit_proc( 0 );
		return 0;
	}

	Sds->Restore_ifd();
	activate_sds();



	SetLogLevels();

    thread
	SyncNotes_thread( &Thread_class::Loop, &SyncNotes ); // run class method: Loop in a thread
    SyncNotes_thread_p = &SyncNotes_thread;

    thread
	ReadNotes_thread( &Thread_class::Loop, &ReadNotes);
    ReadNotes_thread_p = &ReadNotes_thread;

    thread
	SyncAudio_thread( &Thread_class::Loop, &SyncAudio );
    SyncAudio_thread_p = &SyncAudio_thread;

//    thread
//	ReadAudio_thread( &Thread_class::Loop, &ReadAudio );
//    ReadAudio_thread_p = &ReadAudio_thread;

    Log.Comment(INFO, "Application initialized");
	ApplicationLoop( );
	exit_proc( 0 );
};

void stop_apploop( int signal )
{

	if( signal == 0 )
		return; // exit by appstate
	// exit by catched signal

	Timer.State = DISABLED;
	Event.State( DISABLED );

	Appstate->SetExitserver( sds_p, App.AppId );
	usleep( 200 );
}
void stop_threads( int signal )
{
	if ( Cfg_p->Config.test == 'y' )
		return;

	stop_apploop( signal );

	SyncAudio.StopLoop();
	if ( SyncAudio_thread_p )
		SyncAudio_thread_p->join();

	ReadNotes.StopLoop();
	if ( ReadNotes_thread_p )
		ReadNotes_thread_p->join();

	SyncNotes.StopLoop();
	if ( SyncNotes_thread_p )
		SyncNotes_thread_p->join();
}

int sig_counter = 0;
void exit_proc( int signal )
{
	coutf  << Line( term_columns(), 'x' );

	if ( LogMask[TEST] )
		DaTA.Sds_p->Load_ifd();

	if ( signal == SIGILL ) // exit by test process
		exit(0);

	Log.Comment(INFO, "Entering exit procedure for " + Appstate->Name );
//	Notes.scanner->Lock_read( false );
//	Notes.Readlock_wait( true );	// join read lock

	if ( sig_counter > 0 )
	{
		coutf << "ERROR:   Exit procedure failed " << endl;
		coutf << "WARNING: Synthesizer reached target exit " << to_string( signal ) << endl;
	}
	sig_counter++;

	if ( signal > 2 )
		Log.Comment( ERROR, "received signal: " + to_string( signal ) );
	else
		Log.Comment( INFO, "received signal: " + to_string( signal ) );


	stop_threads( signal );
    Sem.Release( SEMAPHORE_EXIT );


    Log.Comment( INFO, "Synthesizer reached target exit 0" );
//	Log.Set_Loglevel( DBG2, true );

    exit( 0 );
}
