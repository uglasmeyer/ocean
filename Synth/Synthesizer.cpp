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
Instrument_class 		Instrument		{ &DaTA, &Wavedisplay };
Mixer_class				Mixer			{ &DaTA, &Wavedisplay } ;
CutDesk_class			CutDesk			{ &Mixer };
Note_class 				Notes			{ &DaTA, &Instrument, &Mixer.StA[ STA_NOTES ] };
Keyboard_class			Keyboard		( &DaTA, &Instrument, &Mixer.StA[ STA_KEYBOARD], &Notes );
External_class 			External		( &Mixer.StA[ STA_EXTERNAL], &Cfg, sds_p );
ProgressBar_class		ProgressBar		( &sds_p->RecCounter );
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
										&CutDesk
										};

Data_t*					Kbd_Data		= Mixer.StA[STA_KEYBOARD ].Data;
Data_t*					Notes_Data		= Mixer.StA[STA_NOTES ].Data;
const uint 				Sync_Semaphore 	= SEMAPHORE_SENDDATA0 + DaTA.SDS_Id;
StateId_t 				Audioserver_state= StateId_t::INACTIVE;
extern void 			SynthesizerTest	();

void AudioServer_state()
{
	StateId_t 	state				=  Appstate->GetState( sds_master, AUDIOID );
	if( state == Audioserver_state )
		return ;

				Audioserver_state	= state;

	LOG_e		level 				= LOG_e::WARN;
	if ( state == StateId_t::RUNNING )
		level	= LOG_e::INFO;
	Log.Comment( level, "Audioserver state is: ", App.sdsstateName( state ) );

	return ;
}

void SetLogLevels()
{

	Log.Show_loglevel();

}


void activate_sds()
{
	Sds->Activate_sds();
	Keyboard.Enable( App.properties.keyboard ) ;
	Wavedisplay.Set_WdData();
}

void add_sound()
{

	if ( Mixer.Cutdesk_isActive() )
	{
		CutDesk.Loop();
		return; // cursor control by cutter
	};

	Mixer.state 		= sds_p->mixer_state;

	Mixer.BeatClock( sds_p->beatClock );

	if (( Mixer.state.instrument ) or ( sds_p->WD_state.roleId == INSTRROLE ) )
	{
		Instrument.Oscgroup.Data_Reset();
		Instrument.Oscgroup.Run_OSCs( 0 );
	}

	Mixer.state.kbd 	&= ( App.properties.keyboard | sds_p->StA_state_arr[ STA_KEYBOARD ].play );
	if ( Mixer.state.kbd )
	{
		kbdInt_t key = sds_p->Kbd_state.key;
		if ( key == NOKEY )
		{
			key = App.KeyboardKey( false );
		}
		bool sync = Mixer.state.sync;
		if ( sds_p->Kbd_state.bpsidx == 0 )
			sync = true;
		Keyboard.Dispatcher( key, sync ); //key from keyboard terminal
		DaTA.EmitEvent( UPDATE_KBDDIALOG_FLAG, "Keyboad_Dialog_class::Setup_Widget" );

		Kbd_Data = Keyboard.ScanData();
	}
	if ( Mixer.state.notes )
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

	if (( sds_p->WD_state.roleId != AUDIOROLE )	)
	{
		Mixer.StA_Wdcursor();
		Wavedisplay.WriteData();
	}


	Stereo_t* shm_addr = DaTA.GetShm_addr(  );
	Mixer.Add_Sound( 	Instrument.osc->MemData_p(),
						Kbd_Data,
						Notes_Data,
						shm_addr
						);
}
Thread_class 	SyncAudio( DaTA.Sem_p, Sync_Semaphore, add_sound, "add_sound" );
thread* 		SyncAudio_thread_p = nullptr;


void ApplicationLoop()
{
	Log.Comment(INFO, "Entering Application loop\n");

	DaTA.Sds_p->Commit(); // set flags to zero and update flag to true

	Event.Handler();
	App.Ready();
	Appstate->SetRunning();
	Keyboard.Show_help( App.properties.keyboard );

	while ( Appstate->IsRunning( sds_p, App.AppId ) )
	{
		Timer.Performance();
		Event.Handler();
		AudioServer_state();
		if( not Appstate->IsRunning( sds_master, APPID::AUDIOID ) or
			not Mixer.state.kbd )
		{
			App.KeyboardKey( false );
		}
	} ;

	Log.Comment(INFO, "Exit Application loop");
	Log.Comment( INFO, Line( 80 - 26 ) );

	return;

} // Application loop


void sync_notes_fnc( )
{
//	Notes.Start_note_itr();
	if( sds_p->NotesTypeId == XML_ID )
		Notes.Generate_volatile_data( true );
	else
		Notes.Generate_cyclic_data( );
}
Thread_class 	SyncNotes( DaTA.Sem_p, SEMAPHORE_SYNCNOTES, sync_notes_fnc, "sync_notes_fnc" );
thread* 		SyncNotes_thread_p = nullptr;

void read_notes_fnc( )
{
//	DaTA.Sds_p->Commit();
	if ( sds_p->NotesTypeId != XML_ID )
		return;
	string name 	= DaTA.Sds_p->Read_str( NOTESSTR_KEY );
	Notes.musicxml 	= Notes.Musicxml.XmlFile2notelist( name );
	if ( Notes.musicxml.scoreduration > 0 )
	{
		Notes.Set_xmlnotelist( Notes.musicxml.notelist );
		Notes.Generate_volatile_data( true );

		Mixer.state.notes = true;
		DaTA.EmitEvent( NEWNOTESLINEFLAG, Notes.musicxml.instrument_name );

//		Notes.Start_note_itr();

		Log.Comment(INFO, "xml notes ", name, " loaded");
	}

}
Thread_class 	ReadNotes( DaTA.Sem_p, SEMAPHORE_INITNOTES, read_notes_fnc, "read_notes_fnc" );
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


	if ( Cfg.Config.test == 'y' )
	{
		Log.Set_Loglevel( TEST, true );
		Sem.Release( SEMAPHORE_STARTED );
		SynthesizerTest();
		Log.Show_loglevel();

		exit_proc( 0 );
		return 0;
	}

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

    Log.Comment(INFO, "Application initialized");
	ApplicationLoop( );
	exit_proc( 0 );
	return 0;
};

void stop_threads()
{
	if ( Cfg.Config.test == 'y' ) return;

	SyncAudio.StopLoop();
	if ( SyncAudio_thread_p )
		SyncAudio_thread_p->join();

	SyncNotes.StopLoop();
	if ( SyncNotes_thread_p )
		SyncNotes_thread_p->join();

	ReadNotes.StopLoop();
	if ( ReadNotes_thread_p )
		ReadNotes_thread_p->join();
}

int sig_counter = 0;
void exit_proc( int signal )
{
	coutf;
	if ( signal == SIGILL )
		exit(0);

	Log.Comment(INFO, "Entering exit procedure for " + Appstate->Name );

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

	stop_threads();
    Sem.Release( SEMAPHORE_EXIT );

    Log.Comment( INFO, "Synthesizer reached target exit 0" );
//	Log.Set_Loglevel( DBG2, true );

    exit( 0 );
}
