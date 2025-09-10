
#include <Synthesizer.h>
#include <Appsymbols.h>


const uint 				Sync_Semaphore 	= SEMAPHORE_SENDDATA0 + DaTA.SDS_Id;
const int 				EXITTEST		= 15;

EventQue_class*			EventQue 		= &Sds->Eventque;// &DaTA.Sds_p->Eventque;
Wavedisplay_class 		Wavedisplay		{ Sds };
Wavedisplay_class*		Wd_p 			= &Wavedisplay;
Appstate_class*			Appstate 		= &DaTA.Appstate;
Mixer_class				Mixer			{ &DaTA, Wd_p } ;// DaTA.Sds_master );
Instrument_class 		Instrument		{ sds, Wd_p };
Note_class 				Notes			{ &Instrument, &Mixer.StA[ STA_NOTES ] };
Keyboard_class			Keyboard		( &Instrument, &Mixer.StA[ STA_KEYBOARD], &Notes );
External_class 			External		( &Mixer.StA[ STA_EXTERNAL], &Cfg);
ProgressBar_class		ProgressBar		( &sds->RecCounter );
Time_class				Timer			( &sds->time_elapsed );
Musicxml_class			MusicXML		{};
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

extern void 			ComposerTestCases();
extern void 			SynthesizerTestCases();


void show_AudioServer_Status()
{
	if ( Appstate->IsRunning( sds_master, AUDIOID ) )
	{
		Log.Comment(INFO, "Sound server is up" );
	}
	else
		Log.Comment( WARN," Sound server not running with status " +
							Appstate->GetStr( sds_master, AUDIOID ) );
}

void SetLogLevels()
{

	Log.Show_loglevel();

}

void activate_sds()
{
	// reset state of empty buffers
	for ( uint id : Mixer.RecIds )
		sds->StA_state[id].play = false;

//	Event.Set_Loglevel( DEBUG, true );
	std::ranges::for_each( init_keys, [  ]( char key )
			{	DaTA.Sds_p->Event( key );	} );

	Keyboard.Enable( DaTA.Appstate.IsKeyboard( ));

	for ( uint id : Mixer.HghIds )
		Mixer.Set_mixer_state(id, sds->StA_state[id].play );

	for ( uint id : Mixer.StAMemIds )
	{
		Mixer.StA[ id ].state 	= sds->StA_state[id];
		Mixer.StA[ id ].DynVolume.SetupVol( sds->StA_amp_arr[id], FIXED );
	}
	if( Mixer.status.notes )
	{
		if( sds->NotestypeId == XML_ID )
			DaTA.Sds_p->Event( XMLFILE_KEY );
		else
			DaTA.Sds_p->Event( NEWNOTESLINEKEY );
	}

//	Event.Set_Loglevel( DEBUG, false );
}


key3struct_t key 	= Kbd_base::key3_struct( 0,0,0 );

void add_sound()
{
	Mixer.status 		= sds->mixer_status;
	Mixer.status.kbd 	&= ( App.properties.keyboard | sds->StA_state[ STA_KEYBOARD ].play );

	if (( Mixer.status.instrument ) )
	{
		Instrument.Oscgroup.Set_Duration( min_msec );
		Instrument.Oscgroup.Data_Reset();
		Instrument.Oscgroup.Run_OSCs( 0 );
	}
	else
		Instrument.osc->kbd_trigger = true;

	key = Keyboard.GetKeystruct( false );
	if ( ( key == Keyboard.ESCkey ))
	{
		Appstate->SetOffline();
		Log.Info( "Exit Key <ESC>");
	}

	if ( Mixer.status.kbd )
	{
		Keyboard.Set_Kbdnote( key );
		sds->StA_state[ STA_KEYBOARD ].play = true;
		Keyboard.ScanData();
	}
	if ( Mixer.status.notes )
	{
		if( sds->NotestypeId == XML_ID )
			Notes.Generate_volatile_data( false ); // no init
		Notes.ScanData();
	}


	Stereo_t* shm_addr = DaTA.GetShm_addr(  );

	Mixer.Add_Sound( 	Instrument.osc->MemData_p(),
						Keyboard.Kbd_Data,
						Notes.NotesData,
						shm_addr );

	ProgressBar.Update();

	if (( sds->WD_status.roleId != osc_struct::AUDIOOUTID )	)
		Wavedisplay.Write_wavedata();
}
Thread_class 	SyncAudio( DaTA.Sem_p, Sync_Semaphore, add_sound, "add_sound" );
thread* 		SyncAudio_thread_p = nullptr;


void ApplicationLoop()
{
	Log.Comment(INFO, "Entering Application loop\n");

	DaTA.Sds_p->Commit(); // set flags to zero and update flag to true

	Sem.Release( SEMAPHORE_STARTED );

	Event.Handler();
	App.Ready();
	show_AudioServer_Status();
	Keyboard.Show_help( Keyboard.Enabled );

	while ( Appstate->IsRunning( sds, App.AppId ) )
	{
		Timer.Performance();
		Event.Handler();

		if( not Appstate->IsRunning( sds_master, APPID::AUDIOID ))
		{
			key3struct_t key = Keyboard.GetKeystruct( false );
			if( key == Keyboard.ESCkey )
				Appstate->SetOffline( );
		}
	} ;

	Log.Comment(INFO, "Exit Application loop");
	Log.Comment( INFO, Line() );

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
	string name 	= DaTA.Sds_p->Read_str( NOTESSTR_KEY );
	string filename = fs.xmldir + name + fs.xml_type ;
	Notes.musicxml 	= MusicXML.Xml2notelist( filename );
	if ( Notes.musicxml.scoreduration > 0 )
	{
		Notes.Set_notelist( Notes.musicxml.notelist );
		sds->Noteline_sec = Notes.musicxml.scoreduration / 1000;

		Mixer.status.notes = true;
		DaTA.EmitEvent( NEWNOTESLINEFLAG, Notes.musicxml.instrument_name );

		Notes.Start_note_itr();
		Notes.Generate_volatile_data( true ); // initialize
		Log.Comment(INFO, "xml notes ", name, " loaded");
	}

}
Thread_class 	ReadNotes( DaTA.Sem_p, SEMAPHORE_INITNOTES, read_notes_fnc, "read_notes_fnc" );
thread* 		ReadNotes_thread_p = nullptr;

void activate_logging()
{
}

int main( int argc, char* argv[] )
{
	activate_logging();

	App.Start( argc, argv );

	if ( Cfg.Config.test == 'y' )
	{
		Log.Set_Loglevel( TEST, true );
		Sem.Release( SEMAPHORE_STARTED );
		SynthesizerTestCases();
		ComposerTestCases();
		Log.Show_loglevel();

		exit_proc( 0 );
		return 0;
	}
//	DaTA.Appstate.SaveState();
//	DaTA.Sds_p->Restore_ifd();
//	DaTA.Appstate.RestoreState();

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
	Log.Set_Loglevel( DBG2, false );

    exit( 0 );
}
