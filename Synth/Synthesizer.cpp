
#include <Synthesizer.h>
extern void ComposerTestCases();


LogVector_t LogVector{ "Synthesizer" };

Exit_class				Exit{};
Logfacility_class		Log( "Synthesizer" );

DirStructure_class		Dir;
Dataworld_class			DaTA( SYNTHID );
interface_t*			sds 			= DaTA.GetSdsAddr();
interface_t*			sds_master 		= DaTA.GetSdsAddr( 0 );
EventQue_class*			EventQue 		= &DaTA.Sds_p->Eventque;
Interface_class*		Sds_p = DaTA.GetSds();
Wavedisplay_class 		Wavedisplay{ Sds_p };//DaTA.Master_Sds_p };
Wavedisplay_class*		wd_p = &Wavedisplay;

Application_class		App( &DaTA );

Dataworld_class*		DaTA_p = &DaTA;
Mixer_class				Mixer{ DaTA_p, wd_p } ;// DaTA.Sds_master );
Instrument_class 		Instrument{ sds, wd_p };
Note_class 				Notes{ wd_p };
Keyboard_class			Keyboard( 	&Instrument );
External_class 			External( 	&Mixer.StA[ MbIdExternal],
									DaTA.Cfg_p);
ProgressBar_class		ProgressBar( &sds->RecCounter );
Statistic_class 		Statistic{ Log.className };
Time_class				Timer		( &sds->time_elapsed );
Musicxml_class			MusicXML{};


Semaphore_class*		Sem	= DaTA.Sem_p;
Config_class*			Cfg = DaTA.Cfg_p;
const uint 				Sync_Semaphore 	= SEMAPHORE_SENDDATA0 + DaTA.SDS_Id;
const int 				EXITTEST			= 15;;

Event_class				Event{
							&Instrument,
							&Notes,
							&Mixer,
							&Wavedisplay,
							&DaTA,
							&External,
							&ProgressBar,
							&MusicXML};


void show_AudioServer_Status()
{
	uint8_t state = sds_master->AudioServer;
	if ( state  == RUNNING )
	{
		Log.Comment(INFO, "Sound server is up" );
	}
	else
		Log.Comment( WARN,"Sound server not running with status " +
							DaTA.Sds_p->Decode( state ));
}

void SetLogLevels()
{
	Log.ResetLogMask();

	Log.Show_loglevel();

}
void show_usage()
{
	printf("---------------------------------------------------------\n");

	cout << Application << " " << Version_str ;
	cout << endl;

	printf("---------------------------------------------------------\n");

	return;
}

void activate_sds()
{
	Event.Set_Loglevel( DEBUG, true );
	std::ranges::for_each( init_keys, [  ]( char key )
			{	DaTA.Sds_p->Event( key );	} );

	for ( uint id : Mixer.HghIds ) // after a restart low id buffers are empty
		Mixer.Set_mixer_state(id, DaTA.Sds_p->addr->StA_state[id].play );
	for ( uint id : Mixer.MemIds )
	{
		Mixer.StA[ id ].state 	= DaTA.Sds_p->addr->StA_state[id];
		Mixer.StA[ id ].DynVolume.SetupVol( DaTA.Sds_p->addr->StA_amp_arr[id], FIXED );
	}
	Event.Set_Loglevel( DEBUG, false );
}



void add_sound( )
{
	Mixer.status = sds->mixer_status;
	sds->mixer_status.sync = Mixer.GetSyncState();

	Instrument.Set_msec( sds_master->audioframes  );

	if ( Mixer.status.notes )
	{
		Notes.Set_instrument( &Instrument );
		Notes.Generate_note_chunk( );
	}

	if (( Mixer.status.instrument ) )
	{
		Instrument.Oscgroup.Data_Reset();
		Instrument.Oscgroup.Run_OSCs( 0 );
	}
	if ( Mixer.status.kbd )
	{
		Keyboard.KbdEvent();
	}
	stereo_t* shm_addr = DaTA.GetShm_addr(  );

	Mixer.Add_Sound( 	Instrument.osc->MemData(),
						Keyboard.Kbd_Data,
						Notes.osc->MemData(),
						shm_addr );

	ProgressBar.Update();

	if (
		( sds->WD_status.roleId != osc_struct::AUDIOID )
//		and	( sds->Composer != RUNNING )
		)
		Wavedisplay.Write_wavedata();
}
Thread_class 	SyncAudio( DaTA.Sem_p, Sync_Semaphore, add_sound, "add_sound" );
thread* 		SyncAudio_thread_p = nullptr;


void ApplicationLoop()
{
	Log.Comment(INFO, "Entering Application loop\n");

	DaTA.Sds_p->Commit(); // set flags to zero and update flag to true

	if( Sem->Getval( SEMAPHORE_STARTED, GETVAL ) > 0 )
		Sem->Release( SEMAPHORE_STARTED );

	sds->Synthesizer = RUNNING;

	App.Ready();

	while ( sds->Synthesizer != EXITSERVER )
	{
		Timer.Performance();

		Event.Handler(  );

		if ( sds->Synthesizer != EXITSERVER )
			sds->Synthesizer = RUNNING;
	} ;


	Log.Comment(INFO, "Exit Application loop");
	Log.Comment( INFO, Log.Line);

	return;

} // Application loop


void sync_notes_fnc( )
{
//	DaTA.Sem.Release( SEMAPHORE_SYNCNOTES ); //other
	Notes.Restart = true;//Notes.Start_note_itr();
}
Thread_class 	SyncNotes( DaTA.Sem_p, SEMAPHORE_SYNCNOTES, sync_notes_fnc, "sync_notes_fnc" );
thread* 		SyncNotes_thread_p = nullptr;

void read_notes_fnc( )
{
	DaTA.Sds_p->Commit();
	string name = DaTA.Sds_p->Read_str( NOTESSTR_KEY );
	string filename = file_structure().Dir.xmldir + name + file_structure().xml_type ;
	Log.Comment( INFO, "from filename: " + filename );
	Notes.musicxml = MusicXML.Xml2notelist( filename );
	if ( Notes.musicxml.scoreduration != 0 )
	{
		Notes.Set_notelist( Notes.musicxml.notelist );
		sds->Noteline_sec = Notes.musicxml.scoreduration / 1000;

		Mixer.status.notes = true;
		DaTA.EmitEvent( NEWNOTESLINEFLAG, Notes.musicxml.instrument_name );
		DaTA.Sds_p->Write_str(INSTRUMENTSTR_KEY, Notes.musicxml.instrument_name ); // other
		DaTA.Sds_p->Event( SETINSTRUMENTKEY );

		Notes.Restart = true;//Notes.Start_note_itr();
		Log.Comment(INFO, "xml notes loaded");
	}

}
Thread_class 	ReadNotes( DaTA.Sem_p, SEMAPHORE_INITNOTES, read_notes_fnc, "read_notes_fnc" );
thread* 		ReadNotes_thread_p = nullptr;

void activate_logging()
{
	LogMask.set( DEBUG );
//	Log.Set_Loglevel( INFO, false );
//	Log.StartFileLogging( &LogVector );
//	Instrument.StartFileLogging( &LogVector );
//	Notes.StartFileLogging( &LogVector );
}

int main( int argc, char* argv[] )
{
	activate_logging();

	App.Start( argc, argv );
	Dir.Create();

	if ( Cfg->Config.test == 'y' )
	{
		Log.Set_Loglevel( TEST, true );
		Sem->Release( SEMAPHORE_STARTED );
		SynthesizerTestCases();
		ComposerTestCases();
		Log.Show_loglevel();

		exit_proc( 0 );
		return 0;
	}

	DaTA.Sds_p->Restore_ifd();
	activate_sds();

	SetLogLevels();

	show_usage();
	show_AudioServer_Status();

    DaTA.Appstate.Announce( );

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
	if ( Cfg->Config.test == 'y' ) return;

	SyncAudio.StopLoop();
	SyncAudio_thread_p->join();

	SyncNotes.StopLoop();
	SyncNotes_thread_p->join();

	ReadNotes.StopLoop();
	ReadNotes_thread_p->join();
}

int sig_counter = 0;
void exit_proc( int signal )
{
	Log.Comment(INFO, "Entering exit procedure for " + App.ProgramName );
	if ( sig_counter > 0 )
	{
		Log.Comment( ERROR, "Exit procedure failed" );
		Log.Comment( WARN, "Synthesizer reached target exit " + to_string( signal ));
	}
	sig_counter++;

	if ( signal > 2 )
		Log.Comment( ERROR, "received signal: " + to_string( signal ) );
	else
		Log.Comment( INFO, "received signal: " + to_string( signal ) );

	stop_threads();
    Log.Comment( INFO, "Synthesizer reached target exit 0" );

//    Log.WriteLogFile();

    exit( 0 );
}
