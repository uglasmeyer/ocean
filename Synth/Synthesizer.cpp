// License Notice
/******************************************************************************
-> Ocean Sound Lab Application Suite (short Ocean-SL)
Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
Linux based Operating Systems, that allows to generate, play and record sound.
It includes interfaces for musicxml-files and supports the sound drivers:
-> native ALSA and
-> Pulseaudio
The software includes a simple keyboard to play and record music as you type.
It comes with a limited set of sample files for instruments and notes to start
with. Funny play.

-> Ocean-SL GitHub site: https://github.com/uglasmeyer/ocean
-> Copyright (c) 2024-2025 Ulrich.Glasmeyer@web.de

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software. Any person wishing to
distribute modifications to the Software is asked to send the modifications to
the original developer so that they can be incorporated into the canonical
version. This is, however, not a binding provision of this license.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/


#include <Appsymbols.h>
#include <Synth/Synthesizer.h>


const uint 				Sync_Semaphore 	= SEMAPHORE_SENDDATA0 + DaTA.SDS_Id;
const int 				EXITTEST		= 15;

//EventQue_class*			EventQue 		= &Sds->Eventque;// &DaTA.Sds_p->Eventque;
Wavedisplay_class 		Wavedisplay		{ Sds };
Wavedisplay_class*		Wd_p 			= &Wavedisplay;
Appstate_class*			Appstate 		= &DaTA.Appstate;
Mixer_class				Mixer			{ &DaTA, Wd_p } ;// DaTA.Sds_master );
Instrument_class 		Instrument		{ sds, Wd_p, Cfg.fs };
Note_class 				Notes			{ &Instrument, &Mixer.StA[ STA_NOTES ] };
Keyboard_class			Keyboard		( &Instrument, &Mixer.StA[ STA_KEYBOARD], &Notes );
External_class 			External		( &Mixer.StA[ STA_EXTERNAL], &Cfg );
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

extern void 			SynthesizerTestCases();

char	 				Audioserver_state = -1;
void AudioServer_state()
{
	char state =  (char)Appstate->IsRunning( sds_master, AUDIOID );
	if ( state == Audioserver_state)
		return ;
	Audioserver_state = state;
	if ( state )
		Log.Info( "Audioserver state is: ", Appstate->GetStateStr(sds_master, AUDIOID ) );
	else
		Log.Comment( WARN, "Audioserver state is: ", Appstate->GetStateStr(sds_master, AUDIOID ) );

	return ;
}

void SetLogLevels()
{

	Log.Show_loglevel();

}


void activate_sds()
{

	// reset state of empty buffers
	for ( uint id : Mixer.RecIds )
		sds->StA_state_arr[id].play = false;

	std::ranges::for_each( init_keys, [  ]( EVENTKEY_e key )
			{	DaTA.Sds_p->Eventque.add( key );	} );

	if ( Mixer.state.instrument )
		sds->WD_status.roleId = INSTRROLE;
	if ( Mixer.state.kbd )
		sds->WD_status.roleId = KBDROLE;

	Keyboard.Enable( DaTA.Appstate.IsKeyboard( ));

	if( Mixer.state.notes )
	{
		sds->WD_status.roleId = NOTESROLE;
		if( sds->NotestypeId == XML_ID )
			DaTA.Sds_p->Eventque.add( XMLFILE_KEY );
		else
			DaTA.Sds_p->Eventque.add( NEWNOTESLINEKEY );
	}
	Wavedisplay.SetDataPtr( sds->WD_status );
}



void add_sound()
{
	Mixer.state 		= sds->mixer_state;
	Mixer.state.kbd 	&= ( App.properties.keyboard | sds->StA_state_arr[ STA_KEYBOARD ].play );

	if (( Mixer.state.instrument ) )
	{
		Instrument.Oscgroup.Set_Duration( min_msec );
		Instrument.Oscgroup.Data_Reset();
		Instrument.Oscgroup.Run_OSCs( 0 );
	}
	else
		Instrument.osc->kbd_trigger = true; // any time

	kbdInt_t key = App.KeyboardKey( false );
	if ( Mixer.state.kbd )
	{
		if( sds->Kbd_state.key == 0 )
			Keyboard.Dispatcher( key );
		else
			Keyboard.Dispatcher( sds->Kbd_state.key );
		sds->StA_state_arr[ STA_KEYBOARD ].play = true;
		Keyboard.ScanData();
	}
	if ( Mixer.state.notes )
	{

		if( sds->NotestypeId == XML_ID )
		{
			Notes.Generate_volatile_data();
		}
		else
		{
			Notes.Note_itr_start.set_active( false );
			Notes.Note_itr_end.set_active( false );
		}

		Notes.ScanData();
		if( sds->StA_amp_arr[STA_NOTES] == 0 )
			sds->StA_amp_arr[STA_NOTES] = osc_default_volume;
	}


	Stereo_t* shm_addr = DaTA.GetShm_addr(  );

	Mixer.Add_Sound( 	Instrument.osc->MemData_p(),
						Keyboard.Kbd_Data,
						Notes.NotesData,
						shm_addr
						);


	ProgressBar.Update();

	if (( sds->WD_status.roleId != AUDIOROLE )	)
		Wavedisplay.Write_wavedata();
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
	Keyboard.Show_help( Keyboard.Enabled );

	while ( Appstate->IsRunning( sds, App.AppId ) )
	{
		Timer.Performance();
		Event.Handler();
		AudioServer_state();
		if( not Appstate->IsRunning( sds_master, APPID::AUDIOID ))
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
	Notes.Start_note_itr();
	Notes.Generate_volatile_data( true );
}
Thread_class 	SyncNotes( DaTA.Sem_p, SEMAPHORE_SYNCNOTES, sync_notes_fnc, "sync_notes_fnc" );
thread* 		SyncNotes_thread_p = nullptr;

void read_notes_fnc( )
{
	DaTA.Sds_p->Commit();
	string name 	= DaTA.Sds_p->Read_str( NOTESSTR_KEY );
	Notes.musicxml 	= MusicXML.XmlFile2notelist( name );
	if ( Notes.musicxml.scoreduration > 0 )
	{
		Notes.Set_notelist( Notes.musicxml.notelist );
		sds->Noteline_sec = Notes.musicxml.scoreduration / 1000;

		Mixer.state.notes = true;
		DaTA.EmitEvent( NEWNOTESLINEFLAG, Notes.musicxml.instrument_name );

		Notes.Start_note_itr();
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
	App.Start( argc, argv );
	activate_logging();

	if ( Cfg.Config.test == 'y' )
	{
		Log.Set_Loglevel( TEST, true );
		Sem.Release( SEMAPHORE_STARTED );
		SynthesizerTestCases();
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
	Log.Set_Loglevel( DBG2, false );

    exit( 0 );
}
