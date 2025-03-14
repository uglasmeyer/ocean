
#include <Synthesizer.h>

using namespace std;

LogVector_t LogVector{ "Synthesizer" };

Exit_class				Exit{};
Logfacility_class		Log( "Synthesizer" );

DirStructure_class		Dir;
Dataworld_class			DaTA( SYNTHID );
interface_t*			sds = DaTA.GetSdsAddr();
interface_t*			sds_master = DaTA.GetSdsAddr( 0 );

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
	uint8_t state = App.GetAppState( AUDIOID);
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
	Notes.Set_Loglevel(DEBUG, false );
	DaTA.Set_Loglevel( INFO, true );
	LogMask[ DEBUG ] = false;
	LogMask[ INFO ] = true;
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
		Mixer.StA[ id ].Volume.Set( DaTA.Sds_p->addr->StA_amp_arr[id], FIXED );
	}
	Event.Set_Loglevel( DEBUG, false );
}

void SetSyncState()
{
	Mixer.status.sync = false;
	for( uint id : Mixer.SycIds )
		Mixer.status.sync |= ( 	Mixer.StA[id].state.play or
								Mixer.StA[id].state.store );

	sds->mixer_status.sync = Mixer.status.sync;
}


void add_sound( )
{

	SetSyncState();
	Instrument.Set_msec( sds_master->audioframes  );

	if ( Mixer.status.notes )
	{
		Notes.Set_instrument( &Instrument );
		Notes.Generate_note_chunk( );
	}

	if (( Mixer.status.instrument ) )
		Instrument.Oscgroup.Run_Oscgroup( 0 ); // generate the modified sound waves

	stereo_t* shm_addr = DaTA.GetShm_addr(  );

	Mixer.Add_Sound( 	Instrument.osc->Mem.Data,
						Keyboard.osc->Mem.Data,
						Notes.osc->Mem.Data,
						shm_addr );

	ProgressBar.Update();


}


void kbd_release( )
{
	if ( Keyboard.Decay() )
	{
		add_sound();
	}
	else
	{
		Keyboard.Release();
		Mixer.Set_mixer_state( MbIdKeyboard, false );
	}

}
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
		sds->time_elapsed = Timer.Performance();

		Event.Handler(  );

		if ( Mixer.status.kbd )
			kbd_release(  );
		int note_key = Keyboard.Kbdnote( );
		if ( Keyboard.Attack( 	note_key, sds->noteline_prefix.Octave ) )
		{
			Keyboard.Set_ch( 0 );
			Mixer.Set_mixer_state( MbIdKeyboard, true );
			kbd_release();
		}


		if ( sds->Synthesizer != EXITSERVER )
			sds->Synthesizer = RUNNING;


		if (( sds->WD_status.roleId != osc_struct::AUDIOID ) and
			( sds->Composer != RUNNING ))
			Wavedisplay.Write_wavedata();
	} ;


	Log.Comment(INFO, "Exit Application loop");
	Log.Comment( INFO, Log.Line);

	return;

} // Application loop

bool SyncAudioThread_done	= false;
string SyncAudioThread_name	= "audio sync thread";
void synchronize_fnc( )
{
	Log.Comment(INFO, SyncAudioThread_name + "started" );

	while( true )
	{
		Sem->Lock( Sync_Semaphore );
		if ( SyncAudioThread_done )
			break;
		add_sound( );
	}

	Log.Comment(INFO, SyncAudioThread_name + " terminated" );
}
thread* SyncAudio_thread_p = nullptr;

bool SyncNotesThread_done	= false;
string SyncNotesThread_name	= "notes sync thread";
void sync_notes_fnc( )
{
	Log.Comment(INFO, SyncNotesThread_name + " started" );

	while( true )
	{
		Sem->Lock( SEMAPHORE_SYNCNOTES );
		if ( SyncNotesThread_done )
			break;
		DaTA.Sem.Release( SEMAPHORE_SYNCNOTES ); //other
		Notes.Restart = true;//Notes.Start_note_itr();
		Log.Info( "reset note pointer");
	}

	Log.Comment(INFO, SyncNotesThread_name + " terminated" );
}
thread* SyncNotes_thread_p = nullptr;

bool ReadNotesThread_done	= false;
string ReadNotesThread_name	= "read notes thread";
void read_notes_fnc( )
{
	Log.Info( ReadNotesThread_name, " started" );
	Sem->Reset( SEMAPHORE_INITNOTES );
	while( true )
	{
		Sem->Lock( SEMAPHORE_INITNOTES );
		if ( SyncNotesThread_done )
			break;

		string name = DaTA.Sds_p->Read_str( NOTESSTR_KEY );
		string filename = file_structure().Dir.xmldir + name + file_structure().xml_type ;
		Log.Comment( INFO, "from filename: " + filename );
		Notes.musicxml = MusicXML.Xml2notelist( filename );
		if ( Notes.musicxml.scoreduration != 0 )
		{
			Notes.Set_notelist( Notes.musicxml.notelist );
			sds->Noteline_sec = Notes.musicxml.scoreduration / 1000;

			Mixer.status.notes = true;
			DaTA.Sds_p->Update(NEWNOTESLINEFLAG);

			DaTA.Sds_p->Write_str(INSTRUMENTSTR_KEY, Notes.musicxml.instrument_name ); // other
			DaTA.Sds_p->Event( SETINSTRUMENTKEY );

			Notes.Restart = true;//Notes.Start_note_itr();
			Log.Comment(INFO, "xml notes loaded");
		}
	}

	Log.Comment(INFO, ReadNotesThread_name + " terminated" );
}
thread* ReadNotes_thread_p = nullptr;

constexpr void activate_logging()
{
	LogMask.at( DEBUG ) 	= false;
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
		Sem->Release( SEMAPHORE_STARTED );
		SynthesizerTestCases();
		Log.Show_loglevel();

		exit_proc( 0 );
		return 0;
	}

	DaTA.Sds_p->Restore_ifd();
	activate_sds();

	SetLogLevels();

	show_usage();
	show_AudioServer_Status();

    DaTA.Sds_p->Announce( );

    thread SyncNotes_thread	( sync_notes_fnc );
    SyncNotes_thread_p = &SyncNotes_thread;

    thread ReadNotes_thread	( read_notes_fnc );
    ReadNotes_thread_p = &ReadNotes_thread;

    thread SyncAudio_thread	( synchronize_fnc );
    SyncAudio_thread_p = &SyncAudio_thread;

    Log.Comment(INFO, "Application initialized");
	ApplicationLoop( );
	exit_proc( 0 );
	return 0;
};

void stop_threads()
{
	if ( Cfg->Config.test == 'y' ) return;

	SyncAudioThread_done	= true;
	Sem->Release(Sync_Semaphore);
	Log.Comment(INFO, "attempting to join " + SyncAudioThread_name );
	if ( SyncAudio_thread_p->joinable() )
		SyncAudio_thread_p->join();

	SyncNotesThread_done	= true;
	Sem->Reset( SEMAPHORE_SYNCNOTES );
	Log.Comment(INFO, "attempting to join " + SyncNotesThread_name );
	if ( SyncNotes_thread_p->joinable() )
		SyncNotes_thread_p->join();

	ReadNotesThread_done	= true;
	Sem->Reset( SEMAPHORE_INITNOTES );
	Log.Comment(INFO, "attempting to join " + ReadNotesThread_name );
	if ( ReadNotes_thread_p->joinable() )
		ReadNotes_thread_p->join();

}

int sig_counter = 0;
void exit_proc( int signal )
{
	Log.Comment(INFO, "Entering exit procedure for " + App.ProgamName );
	if ( sig_counter > 0 )
	{
		Log.Comment( ERROR, "Exit procedure failed" );
		Log.Comment( WARN, "Synthesizer reached target exit " + to_string( signal ));
	}
	sig_counter++;

	if ( signal > 2 )
		Log.Comment( ERROR, "received signal: " + to_string( signal ) );
	else
		Log.Comment(INFO, "received signal: " + to_string( signal ) );

	stop_threads();

	Sem->Release( SEMAPHORE_STARTED ); // if start was not successful release waiting processes here
    Log.Comment(INFO, "Synthesizer reached target exit 0" );

    Log.WriteLogFile();

    if ( signal > 0 )
    	exit( signal );
}
