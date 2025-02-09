
#include <Synthesizer.h>

using namespace std;

Exit_class				Exit{};
Logfacility_class		Log( "Synthesizer" );

DirStructure_class		Dir;
Dataworld_class			DaTA( SYNTHID );
interface_t*			sds = DaTA.GetSdsAddr();
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
Statistic_class 		Statistic{ Log.module };
Time_class				Timer		( &sds->time_elapsed );
Musicxml_class			MusicXML{};


Semaphore_class*		Sem	= DaTA.Sem_p;
Config_class*			Cfg = DaTA.Cfg_p;
const uint 				Sync_Semaphore 	= SEMAPHORE_SENDDATA0 + DaTA.SDS_Id;
const int 				EXITTEST			= 15;;

Core_class				Synthesizer{
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
		Log.Comment( ERROR,"Sound server not running with status " +
							DaTA.Sds_p->Decode( state ));
}

void SetLogLevels()
{
	Notes.Set_Loglevel(DEBUG, true );
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

	for( char key : init_keys )
		Synthesizer.Controller( key );

	if ( sds->NotestypeId == 0 ) // music xml file
		Synthesizer.Controller( XMLFILE_KEY );
	else
		Synthesizer.Controller( UPDATENOTESKEY );

	for ( uint id : Mixer.HghIds )
		Mixer.Set_mixer_state(id, DaTA.Sds_p->addr->StA_state[id].play );
}

void SetAudioframes()
{
	Mixer.status.sync = false;
	for( uint id : Mixer.SycIds)
		Mixer.status.sync |= ( Mixer.StA[id].state.play or Mixer.StA[id].state.store );

	if ( Mixer.status.sync )
		sds->audioframes = Instrument.Set_msec( max_milli_sec );
	else
		sds->audioframes = Instrument.Set_msec( min_milli_sec );
}


void add_sound( )
{

	SetAudioframes();

	if ( Mixer.status.notes )
	{
		Notes.Set_instrument( &Instrument );
		Notes.Generate_note_chunk( );
	}

	if ( Mixer.status.instrument )
		Instrument.Run_osc_group(); // generate the modified sound waves

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
//	interface_t* 	sds 		= DaTA.GetSdsAddr( );

	DaTA.Sds_p->Commit(); // set flags to zero and update flag to true

	if( Sem->Getval( SEMAPHORE_STARTED, GETVAL ) > 0 )
		Sem->Release( SEMAPHORE_STARTED );
	sds->Synthesizer = RUNNING;

	App.Ready();

	while ( sds->Synthesizer != EXITSERVER )
	{
		sds->time_elapsed = Timer.Performance();

		char Key = sds->KEY;
		Synthesizer.Controller( Key );

		assert( sds->StA_amp_arr[0 ]== Mixer.StA[0].Amp );

		if ( Mixer.status.kbd )
			kbd_release(  );
		int note_key = Keyboard.Kbdnote( );
		if ( Keyboard.Attack( 	note_key, sds->noteline_prefix.Octave ) )
		{
			Keyboard.Set_ch( 0 );
			Mixer.Set_mixer_state( MbIdKeyboard, true );
			kbd_release();
		}

		Mixer.Volume_control(  );

		if ( sds->Synthesizer != EXITSERVER )
			sds->Synthesizer = RUNNING;

//		if ( sds->WD_status.wd_mode == wavedisplay_struct::FLOWID )
//			Wavedisplay.Write_wavedata();
		if ( sds->WD_status.roleId != osc_struct::AUDIOID )
			Wavedisplay.Write_wavedata();
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
		add_sound( );
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
	Sem->Reset( SEMAPHORE_NOTES );
	Log.Comment(INFO, "attempting to join notes thread ");
		if ( Notes_thread.joinable() )
			Notes_thread.join();

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
	exit( 0 );
}


int main( int argc, char* argv[] )
{

	App.Start( argc, argv );
	SetLogLevels();
	Dir.Create();

	if ( Cfg->Config.test == 'y' )
	{
		Sem->Release( SEMAPHORE_STARTED );
		SynthesizerTestCases();
		exit( 0 );
	}

	DaTA.Sds_p->Restore_ifd();
	activate_sds();

	show_usage();
	show_AudioServer_Status();

    DaTA.Sds_p->Announce( );

	Log.Comment(INFO, "Application initialized");

	ApplicationLoop( );
	exit_proc( 0 );
//	return 0;
};

