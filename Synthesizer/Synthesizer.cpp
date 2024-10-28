
#include <Synthesizer.h>

using namespace std;

Logfacility_class		Log( Module );
DirStructure_class		Dir;
Dataworld_class			DaTA( SYNTHID );

Wavedisplay_class 		Wavedisplay{ DaTA.Sds_p };
Wavedisplay_class*		wd_p = &Wavedisplay;

Application_class		App( &DaTA );

interface_t*			sds = DaTA.GetSdsAddr();
Dataworld_class*		DaTA_p = &DaTA;
Mixer_class				Mixer{ DaTA_p, wd_p } ;// DaTA.Sds_master );
Instrument_class 		Instrument{ sds, wd_p };
Note_class 				Notes;
Keyboard_class			Keyboard( 	&Instrument );
External_class 			External( 	&Mixer.StA[ MbIdExternal],
									DaTA.Cfg_p);
ProgressBar_class		ProgressBar( &sds->RecCounter );
Statistic_class 		Statistic{ Log.module };

Semaphore_class*		Sem	= DaTA.Sem_p;
Config_class*			Cfg = DaTA.Cfg_p;
const uint 				Sync_Semaphore 	= SEMAPHORE_SENDDATA0 + DaTA.SDS_Id;

Core_class				Synthesizer{
							&Instrument,
							&Notes,
							&Mixer,
							&Wavedisplay,
							&DaTA,
							&External,
							&ProgressBar };

extern void exit_proc( int signal );

void show_AudioServer_Status()
{
	uint8_t state = App.GetAppState( AUDIOID);
	if ( state  == RUNNING )
	{
		Log.Comment(INFO, "Sound server is up" );
	}
	else
		Log.Comment( ERROR,"Sound server not running with status " +
							DaTA.Sds_p->Decode( state ));
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

	for ( uint id : Mixer.MemIds )
	{
		Mixer.StA[ id ].Amp 	= DaTA.Sds_p->addr->StA_amp_arr[id];
		Mixer.StA[ id ].state 	= DaTA.Sds_p->addr->StA_state[id];
	}

	for ( uint id : Mixer.HghIds )
		Mixer.Set_mixer_state(id, DaTA.Sds_p->addr->StA_state[id].play );

	for( char key : init_keys )
		Synthesizer.processKey( key );
}

bool sync_mode()
{
	int play = 0;
	for( uint id : Mixer.SycIds)
		play += (int) Mixer.StA[id].state.play;

	Mixer.status.play = (bool) play;
	bool sync =
		( 	// if true synchronize shm a/b with Audio Server
			( Instrument.fmo.wp.frequency < LFO_limit 	)	or
			( Instrument.vco.wp.frequency < LFO_limit 	) 	or
			( Mixer.StA[MbIdExternal].state.store 		)	or
			( Mixer.status.play 		)	or	// any StA triggers play if itself is in play mode
			( ProgressBar.active 			)		// StA record external
		);
	if ( Mixer.status.kbd )
		sync = false;
	return sync ;
}


/*------------------------------Sync block
 *
 */

void add_sound( stereo_t* shm_addr )
{
	if ( Mixer.status.notes )
	{
		Notes.Set_instrument( &Instrument );
		Notes.Generate_note_chunk( );
	}
	if ( Mixer.status.instrument )
		Instrument.Run_osc_group(); // generate the modified sound waves
	Mixer.Add_Sound( 	Instrument.main.Mem.Data,
						Keyboard.osc.Mem.Data,
						Notes.main.Mem.Data,
						shm_addr );

	ProgressBar.Update();

	if ( sds->Wavedisplay_Id != AUDIOOUT )
		Wavedisplay.Write_wavedata();
}


void kbd_release( stereo_t* shm_addr )
{
	if ( Keyboard.Decay() )
	{
		cout << "DECAY" << endl;
	}
	else
	{
		cout << "RELEASE" << endl;
		Keyboard.Release();

		add_sound(shm_addr);
		Mixer.Set_mixer_state( MbIdKeyboard, false );
		DaTA.Sds_p->addr->UpdateFlag 	= true;
	}

}
void ApplicationLoop()
{
	Log.Comment(INFO, "Entering Application loop\n");
	interface_t* 	ifd 		= DaTA.GetSdsAddr( );
	stereo_t* 		shm_addr 	= DaTA.GetShm_addr( );
	Time_class		Timer		( &ifd->time_elapsed );

	DaTA.Sds_p->Commit(); // set flags to zero and update flag to true

	if( Sem->Getval( SEMAPHORE_STARTED, GETVAL ) > 0 )
		Sem->Release( SEMAPHORE_STARTED );
	ifd->Synthesizer = RUNNING;

	App.Ready();

	while ( ifd->Synthesizer != EXITSERVER )
	{
		ifd->time_elapsed = Timer.Performance();

		char Key = ifd->KEY;
		Synthesizer.processKey( Key );
		assert( ifd->StA_amp_arr[0 ]== Mixer.StA[0].Amp );

		int note_key = Keyboard.Kbdnote( );
		if ( Keyboard.Attack( note_key, ifd->noteline_prefix.Octave, 100 ) )
		{
			cout << "KEY: " << note_key << endl;
			Keyboard.Set_ch( 0 );
			Mixer.StA[ MbIdKeyboard ].state.play = true;
			add_sound( DaTA.GetShm_addr(  ) );
			Mixer.status.kbd	= true;
			ifd->UpdateFlag 	= true;
			ifd->MODE 			= KEYBOARD;
			while ( ifd->MODE 	== KEYBOARD )
			    this_thread::sleep_for(chrono::nanoseconds(10));
//				Wait( 10 );
		}

		Mixer.Volume_control(  );

		if ( ifd->Synthesizer != EXITSERVER )
			ifd->Synthesizer = RUNNING;

		if( sync_mode()  )
		{
			ifd->MODE 	= SYNC; // sync thread
		}
		else
		{
//			ifd->SHMID = 0;
			shm_addr	= DaTA.GetShm_addr( );

			if ( Mixer.status.kbd )
				kbd_release( shm_addr );
//			else
//				add_sound( shm_addr );

			ifd->MODE	= FREERUN;
		}
	} ;


	Log.Comment(INFO, "Exit Application loop");
	Log.Comment( INFO, Log.Line);

	return;

} // Application loop

bool SyncThread_done	= false;
void synchronize_fnc( )
{
	Log.Comment(INFO, "Sync thread started" );

	while( true )
	{
		Sem->Lock( Sync_Semaphore );
		if ( SyncThread_done )
			break;
		add_sound( DaTA.GetShm_addr(  ) );
		DaTA.Sds_p->addr->UpdateFlag = true;
	}

	Log.Comment(INFO, "Sync thread terminated" );
}
thread Sync_thread	( synchronize_fnc );

bool NotesThread_done	= false;
void notes_fnc( )
{
	Log.Comment(INFO, "Notes thread started" );

	while( true )
	{
		Sem->Lock( SEMAPHORE_NOTES );
		if ( NotesThread_done )
			break;
		DaTA.Sem.Release( SEMAPHORE_NOTES ); //other
		Notes.Restart = true;//Notes.Start_note_itr();
		Log.Comment(INFO, "reset note pointer");
	}

	Log.Comment(INFO, "Notes thread terminated" );
}
thread Notes_thread	( notes_fnc );

void stop_threads()
{
	SyncThread_done	= true;
	Sem->Release(Sync_Semaphore);
	Log.Comment(INFO, "attempting to join sync thread ");
	if ( Sync_thread.joinable() )
		Sync_thread.join();

	NotesThread_done	= true;
	Sem->Release(SEMAPHORE_NOTES );
	Log.Comment(INFO, "attempting to join notes thread ");
	if ( Notes_thread.joinable() )
		Notes_thread.join();

}
int sig_counter = 0;
void exit_proc( int signal )
{
	if ( sig_counter > 0 )
	{
		Log.Comment( ERROR, "Exit procedure failed" );
		Log.Comment( WARN, "Synthesizer reached target exit " + to_string( signal ));
		exit( signal );
	}
	sig_counter++;

	string text = "received signal: " + to_string( signal );
	if ( signal > 2 )
		Log.Comment( ERROR, text );
	else
		Log.Comment(INFO, text );
	if ( signal == EXITTEST )
	{
		Log.Comment(TEST, "Entering test cases exit procedure for application" + Application );
	}
	else
	{
		Log.Comment(INFO, "Entering exit procedure for " + Application );
	}

	stop_threads();

	Sem->Release( SEMAPHORE_STARTED ); // if start was not successful release waiting processes here
    Log.Comment(INFO, "Synthesizer reached target exit 0" );
	exit( 0 );
}


int main( int argc, char* argv[] )
{
	catch_signals( &exit_proc, { SIGINT, SIGHUP, SIGABRT } );

	App.Start( argc, argv );


	Dir.Create();

	if ( Cfg->Config.test == 'y' )
	{


		Sem->Release( SEMAPHORE_STARTED );
		SynthesizerTestCases();
		exit_proc( EXITTEST );
	}

	DaTA.Sds_p->Restore_ifd();
	activate_sds();

	Wavedisplay.SetId( DaTA.Sds_p->addr->Wavedisplay_Id );

	show_usage();
	show_AudioServer_Status();

    DaTA.Sds_p->Announce( );

	Log.Comment(INFO, "Application initialized");

	ApplicationLoop( );
	exit_proc( 0 );
//	return 0;
};

