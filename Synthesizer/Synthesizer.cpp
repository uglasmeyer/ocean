
#include <Synthesizer.h>

using namespace std;
string					Module = "Synthesizer";
Logfacility_class		Log( Module );
DirStructure_class		Dir;
Interface_class			SDS;
Application_class		App( Module, SYNTHID, &SDS );
Semaphore_class			Sem;
Mixer_class				Mixer ( SDS.addr );
Instrument_class 		Instrument(SDS.addr );
Note_class 				Notes;
Keyboard_class			Keyboard( &Instrument );
External_class 			External( &Mixer.StA[ MbIdExternal], &SDS.addr->RecCounter );
Shared_Memory			Shm_a, Shm_b;
Wavedisplay_class 		Wavedisplay;
ProgressBar_class		ProgressBar( &SDS.addr->RecCounter );
Time_class				Timer( &SDS.addr->time_elapsed );
bool 					SaveRecordFlag 		= false;
bool 					RecordThreadDone 	= false;


extern void record_thead_fcn( 	Interface_class*,
								External_class* ,
								bool*,
								bool* );


const int 				EXITTEST			= 15;;

stereo_t* set_addr( char id )
{
	return ( id == 0 ) ? Shm_a.addr : Shm_b.addr;
}



void Setup_Wavedisplay()
{
	Log.Comment(INFO, "Setup Wave display data" );

	Wavedisplay.Add_data_ptr( Mixer.Mono_out.Data ); // TODO stereo out
	Wavedisplay.Add_data_ptr( Instrument.main.Mem.Data );
	Wavedisplay.Add_data_ptr( Instrument.vco.Mem.Data );
	Wavedisplay.Add_data_ptr( Instrument.fmo.Mem.Data );
	Wavedisplay.Add_data_ptr( Mixer.StA[ MbIdExternal].Data );

	Wavedisplay.Set_data_ptr( SDS.addr->Wavedisplay_Id );
}

thread record_thread( record_thead_fcn, &SDS,
										&External,
										&SaveRecordFlag,
										&RecordThreadDone );


void exit_proc( int signal )
{
	Log.Comment(INFO, "received signal: " + to_string( signal ) );
	if ( Log.Log[ TEST ] )
	{
		Log.Comment(TEST, "Entering test cases exit procedure for application" + Application );
	}
	else
	{
		Log.Comment(INFO, "Entering exit procedure for " + Application );
	}

	RecordThreadDone = true;
	Sem.Release( SEMAPHORE_RECORD );

	if ( record_thread.joinable() )
		record_thread.join();
	exit( 0 );
}

void show_AudioServer_Status()
{
	if ( SDS.addr->AudioServer == RUNNING )
		Log.Comment(INFO, "Sound server is up" );
	else
		Log.Comment( ERROR, "Sound server not running with status " +
							SDS.Decode( SDS.addr->AudioServer ));
}

void show_usage()
{
	printf("---------------------------------------------------------\n");

	cout << Application << " " << Version_str ;
	cout << endl;

	printf("---------------------------------------------------------\n");

	return;
}

// TODO navigate to process start location

void process( char key )
{
	ifd_t* ifd = SDS.addr;

	auto set_waveform = [ ifd ]( Oscillator* osc, char id )
		{
			osc->Set_waveform( id);
			string wf = osc->Get_waveform_str( id );
			Log.Comment(INFO, "set waveform >" + wf + "< for " + osc->osc_type );
		};

	switch ( key )
	{
		case NULLKEY :
		{
			break;
		}
		case MAINFREQUENCYKEY :
		{
			Instrument.main.Set_frequency( ifd->Main_Freq );
			Notes.main.Set_frequency( ifd->Main_Freq );
			SDS.Commit();
			break;
		}
		case VCOFREQUENCYKEY : // modify the secondary oscillator
		{
			Instrument.vco.Set_frequency( ifd->VCO_Freq );
			Notes.vco.Set_frequency( ifd->VCO_Freq );
			Instrument.main.Connect_vco_data( &Instrument.vco);

			SDS.Commit();
			break;
		}
		case FMOFREQUENCYKEY : // modify the fm_track data
		{
			Instrument.fmo.Set_frequency( ifd->FMO_Freq );
			Notes.fmo.Set_frequency( ifd->FMO_Freq );

			Instrument.main.Connect_fmo_data( &Instrument.fmo);

			SDS.Commit();
			break;
		}
		case VCOAMPKEY : // modify the VCO volume
		{
			Value vol = ifd->VCO_Amp;
			Log.Comment( INFO, "Changing VCO volume to " + vol.str + " %" );
			Instrument.vco.Set_volume( vol.ch );
			Notes.vco.Set_volume( vol.ch );
			Instrument.main.Connect_vco_data( &Instrument.vco);
			SDS.Commit();
			break;
		}
		case FMOAMPKEY : // modify the FMO volume
		{
			Value vol = ifd->FMO_Amp;
			Instrument.fmo.Set_volume( vol.ch );
			Notes.fmo.Set_volume( vol.ch );
			Instrument.main.Connect_fmo_data( &Instrument.fmo);
			Notes.fmo.Set_volume( vol.ch );
			SDS.Commit();
			break;
		}
		case MASTERAMP_KEY : // modify main volume
		{
			Mixer.master_volume = ifd->Master_Amp;
			Mixer.status.mute = false;
			SDS.Commit();
			break;

		}
		case MASTERAMP_LOOP_KEY :
		{
			uint16_t beg = Mixer.master_volume;
			uint16_t end = ifd->LOOP_end;
			uint8_t step = ifd->LOOP_step;
			Mixer.master_amp_loop.Start( beg, end, step );
			SDS.Commit();
			break;
		}
		case MASTERAMP_MUTE_KEY : // Mute Main Volume
		{
			Mixer.status.mute = not ifd->mixer_status.mute;
			string str = ( Mixer.status.mute ) ? "Mute" : "UnMute";
			Log.Comment( INFO, "receiving command <"+str+"> master volume>");
			SDS.Commit();
			break;
		}
		case ADSR_KEY :
		{
			Instrument.main.Set_adsr( ifd->Main_adsr );
			SDS.Commit();
			break;
		}
		case PMWDIALKEY :
		{
			Instrument.main.Set_pmw(ifd->PMW_dial );
			Instrument.vco.Set_pmw( ifd->PMW_dial );
			Instrument.fmo.Set_pmw( ifd->PMW_dial );

			SDS.Commit();

			break;
		}
		case WAVEDISPLAYTYPEKEY :
		{
			Wavedisplay.Set_type( ifd->WD_type_ID );
			SDS.Commit();
			break;
		}
		case SOFTFREQUENCYKEY :
		{
			Instrument.main.Set_glide( ifd->Soft_freq );
			SDS.Commit();
			break;
		}
		case SETINSTRUMENTKEY : // Set instrument
		{
			Log.Comment(INFO, "receive command <set instrument>");
			string instrument = SDS.Read_str( INSTRUMENTSTR_KEY ); // other

			if( Instrument.Set(instrument) )
			{
				Log.Comment(INFO, "sucessfully loaded instrument " + instrument );
				Notes.Set_osc_track( &Instrument );
			}
			else
			{
				Log.Comment( ERROR, "cannot load instrument" + instrument );
			}

			SDS.Commit(); // reset flags on GUI side
			break;
		}
		case READ_EXTERNALWAVEFILE :
		{
			Log.Comment(INFO, "receive command <set external wave file>");
			string wavefile = SDS.Read_str( WAVEFILESTR_KEY );
			if( cmpstr( External.GetName(), wavefile ) )
			{
				Sem.Lock( SEMAPHORE_RECORD ); // assume record thread is working on that file
			}

			   // wait until the worker thread is done doing the work
			    // by attempting to decrement the semaphore's count

			if ( External.Read_file_header( wavefile ))
			{
				External.Read_file_data();
				Mixer.StA[ MbIdExternal ].Play_mode( true );
				Mixer.StA[ MbIdExternal ].Amp 	= 100;//ifd->StA_amp_arr[MbIdExternal];
				Mixer.status.external			= true;

				ProgressBar.Setup( 100*External.Filedata_size / External.stereo.info.mem_bytes );
			}
			else
			{
				Log.Comment(ERROR , "Failed to setup header");
			}
			SDS.Commit();
			ifd->UserInterface = UPDATEGUI; //set cb_sta play flag for external
			break;
		}
		case STOPRECORD_KEY : // stop record on data array id
		{
			Value id { (int)ifd->MIX_Id };
			Log.Comment(INFO, "receive command <stop record on storage area " + id.str + ">");
			Mixer.StA[id.i].Record_mode( false ); // stop recording
			ProgressBar.Unset();
			SDS.Commit();
			break;
		}
		case STORESOUNDKEY : //start record
		{
			Value MbNr { (int) ifd->MIX_Id };
			Log.Comment( INFO, "receiving command <store sound to memory bank " + MbNr.str + " >");
			for ( int id : Mixer.RecIds )
			{
				if ( MbNr.i == id )
				{
					string status = Mixer.StA[MbNr.i].Record_mode(true); // start record-stop play
					Log.Comment( INFO, " Storage Id " +  MbNr.str + " recording is " + status);
					ProgressBar.Set( Mixer.StA[MbNr.i].Get_storeCounter_p()  ,
								Mixer.StA[MbNr.i].max_counter );
				}
				else // only one mb shall store data
				{
					Mixer.StA[id].Record_mode( false );
				}
			}
			SDS.Commit();
			break;
		};

		case EXTERNAL_AMPLOOP_KEY :
		{
			uint16_t beg = Mixer.StA[MbIdExternal].Amp;
			uint16_t end = ifd->LOOP_end;
			uint8_t step = ifd->LOOP_step;
			Mixer.record_amp_loop.Start( beg, end, step );
			SDS.Commit();
			break;

		}
		case SETMBAMPPLAYKEY : // 109 change volume and play data array
		{

			Value mixid { ifd->MIX_Id };
			Value amp	{ ifd->StA_amp_arr[mixid.i] } ;
			Value play 	{ ifd->StA_state[ mixid.i ].play };

			Mixer.StA[mixid.i].Amp = amp.i;
			Mixer.Set_mixer_state( mixid.i, (bool)play.i );
			Log.Comment( INFO, 	"Mixer ID " + mixid.str +
								" Amp: " 	+ amp.str +
								" State: " 	+ play.boolstr);

			SDS.Commit();
			break;
		}
		case MUTEREC_KEY :
		{
			Value id = { (int) ifd->MIX_Id };
			Log.Comment(INFO, "receive command <mute and stop record on id" + id.str + ">");
			Mixer.StA[id.i].Play_mode( false ); // pause-play, pause-record
			SDS.Commit();
			break;
		}
		case MUTEMBKEY : // clear all memory bank flag
		{
			Log.Comment(INFO, "receive command <mute and stop record on all memory banks>");
		    for( uint id : Mixer.RecIds )
		    	{ Mixer.Set_mixer_state( id, false ); }

			SDS.Commit();
			break;
		}
		case CLEAR_KEY :
		{
			uint8_t id 						= ifd->MIX_Id;
			Log.Comment( INFO, "Clear StA: " + to_string( id ));
			Mixer.StA[ id ].Reset_counter();
//			Mixer.Set_mixer_state(id, false );
			ProgressBar.Reset(); // RecCounter
			SDS.Commit();
//			ifd->UserInterface = UPDATEGUI;
			break;
		}
		case TOGGLEMBPLAYKEY: // toggle Memory bank status play
		{
			Value Id 	{ (int)ifd->MIX_Id };
			bool play 	{ not (ifd->StA_state[ Id.i ].play) };
			ifd->StA_state[ Id.i].play = play ;
			Log.Comment(INFO,
					"receive command <toggle play on memory bank" + Id.str +" >" + to_string( play ) );
			Mixer.Set_mixer_state( Id.i, play );
			SDS.Commit();
			break;
		}

		case RESETMAINKEY : // reset main
		{
			Instrument.main.Mem_vco.clear_data(max_data_amp);
			Instrument.main.Mem_fmo.clear_data( 0 );
			Instrument.main.Reset_data( &Instrument.main );

			SDS.Commit();
			break;
		}
		case UPDATENOTESKEY : // update notes
		{
			Log.Comment(INFO, "receive command <update notes>");
			string notes_name = SDS.Read_str( NOTESSTR_KEY );
			Notes.Set_osc_track( &Instrument);
			Notes.Read( notes_name );
			ifd->Noteline_sec = Notes.noteline_sec;
			Notes.Start_note_itr();
			SDS.Commit();
			break;
		}
		case NEWNOTESLINEKEY : // setup play or reset play notes
		{
			Log.Comment(INFO, "receive command <setup play notes>");
			string notes_file = SDS.Read_str( NOTESSTR_KEY );
			Notes.Read( notes_file ); // notes have been written to file by the GUI already
			Mixer.status.notes = true;
			SDS.Update( NEWNOTESLINEFLAG );
			ifd->Noteline_sec = Notes.noteline_sec;
			Notes.Start_note_itr();
			break;
		}
		case PLAYNOTESREC_ON_KEY : // play modnt.composer = true;
		{
			Value sec { ifd->Noteline_sec };
			Value id  { ifd->MIX_Id };

			if ( sec.i > 0 )
			{
				Log.Comment(INFO, "generate composer notes");
				Log.Comment(INFO, "duration: " + sec.str + " sec.");
				Log.Comment(INFO, "store to StA id: " + id.str );

				Mixer.composer = sec.i;
				Mixer.StA[ id.i].Record_mode( true );
				Notes.Set_osc_track( &Instrument );
				Mixer.Store_noteline( id.i, &Notes );

			}
			else
			{
				Log.Comment( WARN, "nothing to do for " + sec.str + " Notes!" );
			}
			SDS.Commit();
			break;
		}
		case PLAYNOTESRECOFF_KEY : // play free
		{ // functionality is defined in PLAYNOTESREC_ON_KEY
			Mixer.composer = 0;
			SDS.Commit();
			break;
		}
		case NOTESONKEY :
		{
			Value amp { (int) ifd->StA_amp_arr[ MbIdNotes] };
			Log.Comment(INFO, "receive command < notes on " + amp.str + "%>");
			Mixer.status.notes = true;
			Mixer.StA[ MbIdNotes ].Amp = amp.i;
			Notes.Start_note_itr();
			SDS.Commit();
			break;
		}
		case NOTESOFFKEY :
		{
			Log.Comment(INFO, "receive command < notes off>");
			Mixer.status.notes = false;
			SDS.Commit();
			break;
		}
		case UPDATENOTELINEKEY: // update Noteline during play
		{
			Log.Comment(INFO, "receive command <update Noteline during play>");
			string notes_file = SDS.Read_str( NOTESSTR_KEY );
			Notes.Read( notes_file );
			SDS.Commit();
			break;
		}
		case SETBASEOCTAVE_KEY :
		{
			Value diff_oct { (int) ifd->FLAG };
			Notes.Set_base_octave( diff_oct.i ); // is positive, therefore identify 0 -> -1, 1 -> 1
			SDS.Commit();
			break;
		}
		case UPDATE_NLP_KEY : // Noteline_prefix
		{
			Note_base::noteline_prefix_t nlp = ifd->noteline_prefix;
			Log.Comment(INFO, "receive command <update notesline prefix");
			Notes.show_noteline_prefix( nlp );
			Notes.Noteline_prefix = nlp;
			Notes.Verify_noteline( nlp, Notes.Get_note_line() );
			SDS.Commit();
			break;
		}

		case SETNOTESPERSEC_KEY:
		{
			Value nps = ifd->noteline_prefix.nps;
			Log.Comment(INFO, "receive command <set notes per second>");
			if ( not Notes.Set_notes_per_second( nps.i ) )
			{
				Log.Comment( ERROR, nps.str + " notes per second not supported");
			}
			SDS.Commit();
			break;
		}

		case SETWAVEDISPLAYKEY :
		{
			Wavedisplay.Set_data_ptr( ifd->Wavedisplay_Id );
			SDS.Commit();
			break;
		}
		case CONNECTFMOVCOKEY : // connect FMO volume with vco data
		{

			Instrument.fmo.Connect_fmo_data( &Instrument.vco );
			Instrument.main.Connect_fmo_data( &Instrument.fmo );
			SDS.Commit();
			break;
		}
		case RESETVCOKEY : // reset VCO
		{
			Instrument.vco.Reset_data( &Instrument.vco );
			SDS.Commit();
			break;
		}
		case RESETFMOKEY : // reset FMO
		{
			Instrument.fmo.Reset_data( &Instrument.fmo );
			SDS.Commit();
			break;
		}
		case RECORDWAVFILEKEY : // record and save wav file
		{
		    if ( SaveRecordFlag )
			{
				Log.Comment( WARN, "Thread is saving data. ... Wait ");
				ifd->KEY = NULLKEY;
				break;
			}
			External.status.record = not ifd->mixer_status.external;

			if ( ifd->Composer == RECORD ) // composer overwrites default behaviour
				External.status.record = true;
			if ( ifd->Composer == STOPRECORD )
				External.status.record = false;
			Mixer.status.external	= External.status.record;
			string str = External.status.record ? "Start" : "Stop ";

			if ( not External.status.record ) 	// STOPRECORD, also means: save to file
			{
				Log.Comment(INFO, str + " record Audio out to wav file format.");
				Mixer.StA[MbIdExternal].Record_mode( false );
				ProgressBar.Unset();
				//no ifd-commit for this section
				ifd->KEY = NULLKEY;  //but do not start twice

				Sem.Release( SEMAPHORE_RECORD );
			}
			else 								// RECORD
			{
				Log.Comment(INFO, str + " record Audio out to wav file format.");
				External.stereo.info.record_counter = 0;
				External.stereo.clear_data();
				Mixer.StA[MbIdExternal].Record_mode( true );
				ProgressBar.Set( &External.stereo.info.record_counter,
							 	 External.stereo.info.max_records );
				SDS.Commit();
			}
			break;
		}
		case CONNECTVCOFMOKEY ://connect VCO frequency with FMO data
		{
			Instrument.vco.Connect_vco_data( &Instrument.fmo );
			Instrument.main.Connect_vco_data( &Instrument.vco );
			SDS.Commit();
			break;
		}
		case SETWAVEFORMFMOKEY :
		{
			set_waveform( &Instrument.fmo, ifd->FMO_spectrum.id );

			SDS.Commit();
			break;
			}
		case SETWAVEFORMVCOKEY :
		{
			set_waveform( &Instrument.vco, ifd->VCO_spectrum.id );
			SDS.Commit();
			break;
		}
		case SETWAVEFORMMAINKEY :
		{
			set_waveform( &Instrument.main, ifd->MAIN_spectrum.id );
			SDS.Commit();
			break;
		}

		case UPDATESPECTRUM_KEY :
		{
			Instrument.Update_spectrum();
			SDS.Commit();
			break;
		}
		case SAVEINSTRUMENTKEY :
		{
			Log.Comment(INFO, "saving current config to instrument " + Instrument.Name );
			Instrument.Save_Instrument( Instrument.Name );
			SDS.Commit();
			break;
		}

		case NEWINSTRUMENTKEY : // save instrument file
		{
			string instrument = SDS.Read_str( INSTRUMENTSTR_KEY );
			Log.Comment(INFO, "receiving instrument change to " + instrument );
			Instrument.Save_Instrument( instrument );
			SDS.Update( NEWINSTRUMENTFLAG );
			break;
		}

		case EXITKEY :
		{
			exit_proc( 1 );
			break;
		}

		default :
		{
			Log.Comment( ERROR, "Communication Key Id >" +
					to_string((int)key) +
					"< undefined");
			exit_proc( 1);
		 }

	} // switch char
	Mixer.Update_ifd_status_flags( ifd );

}
// TODO process end location

void activate_ifd()
{

	for ( uint id : Mixer.MemIds )
	{
		Mixer.StA[ id ].Amp 	= SDS.addr->StA_amp_arr[id];
		Mixer.StA[ id ].state 	= SDS.addr->StA_state[id];
	}

	for ( uint id : Mixer.HghIds )
		Mixer.Set_mixer_state(id, SDS.addr->StA_state[id].play );

	for( char key : init_keys )
		process( key );
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



void add_sound( stereo_t* shm_addr )
{
	if ( Mixer.status.instrument )
		Instrument.Run_osc_group(); // generate the modified sound waves
	Mixer.Add_Sound( Instrument.main.Mem.Data, Keyboard.osc.Mem.Data,  Notes.main.Mem.Data, shm_addr );
/*	if (	Mixer.StA[ MbIdKeyboard	].state.play )
	{
		Keyboard.Set_ch( 0 );
		Mixer.StA[ MbIdKeyboard	].state.play = false;
	}
	*/
	wd_arr_t display_data = Wavedisplay.Gen_cxwave_data(  );
	SDS.Write_arr( display_data );
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
		Mixer.StA[ MbIdKeyboard ].state.play = false;
		Mixer.status.kbd = false;
		SDS.addr->UpdateFlag 	= true;
	}

}
void ApplicationLoop()
{
	Log.Comment(INFO, "Entering Application loop\n");
	ifd_t* 			ifd 		= SDS.addr;
	stereo_t* 		shm_addr 	= set_addr( 0 );

	Log.Comment(INFO, App.Name + " is ready");

	SDS.Commit(); // set flags to zero and update flag to true

	if( Sem.Getval( SEMAPHORE_STARTED, GETVAL ) > 0 )
		Sem.Release( SEMAPHORE_STARTED );

	while ( ifd->Synthesizer != EXITSERVER )
	{
		ifd->time_elapsed = Timer.Performance();

		char Key = ifd->KEY;
		process( Key );
		assert( ifd->StA_amp_arr[0 ]== Mixer.StA[0].Amp );

		int note_key = Keyboard.Kbdnote( );
		if ( Keyboard.Attack( note_key, ifd->noteline_prefix.Octave, 100 ) )
		{
			cout << "KEY: " << note_key << endl;
			Keyboard.Set_ch( 0 );
			Mixer.StA[ MbIdKeyboard ].state.play = true;
			add_sound( shm_addr );
			Mixer.status.kbd	= true;
			ifd->UpdateFlag 	= true;
			ifd->MODE 			= KEYBOARD;
			while ( ifd->MODE 	== KEYBOARD )
			    this_thread::sleep_for(chrono::nanoseconds(10));
//				Wait( 10 );
		}

		Mixer.Volume_control( ifd );

		if ( ifd->Synthesizer != EXITSERVER )
			ifd->Synthesizer = RUNNING;

		if( sync_mode()  )
		{
			int mode 	= (int)ifd->MODE;
			ifd->MODE 	= SYNC;
			if ( mode == SENDDATA )		// Audio Server request 1-second data chunk
			{

				if ( Mixer.status.notes )
				{
					Notes.Set_osc_track( &Instrument );
					Notes.Generate_note_chunk( );
				}

				add_sound( shm_addr );

				if ( ProgressBar.active )
					External.Add_record( &Mixer.Out_L, &Mixer.Out_R);
				ProgressBar.Update();

				ifd->UpdateFlag = true;
				int shm_id 	= ifd->SHMID;
				shm_addr 	= set_addr( shm_id );
			} 	// but handel further requests

		}
		else
		{
			// Keyboard decay mode
			if ( Mixer.status.kbd )
			{
				kbd_release( shm_addr );
			}
			else
				add_sound( shm_addr );


			shm_addr	= set_addr( 0 );
			ifd->MODE	= FREERUN;
		}
	} ;


	Log.Comment(INFO, "Exit Application loop");
	return;

} // Application loop


int main( int argc, char* argv[] )
{
	catch_signals( &exit_proc, { SIGINT, SIGHUP, SIGABRT } );
	App.Start( argc, argv );

	Log.Comment(INFO, "Evaluating startup arguments");

	Log.Comment( INFO, "using base directory: " + Dir.basedir );
	Dir.Create();

	if ( App.Cfg.Config.test == 'y' )
	{
		SynthesizerTestCases();
		exit_proc( EXITTEST );
	}

	Log.Comment(INFO,"Attaching data buffers");
	Shm_a.buffer( sharedbuffer_size, App.Cfg.Config.shm_key_a );
	Shm_b.buffer( sharedbuffer_size, App.Cfg.Config.shm_key_b );

//	App.Shutdown_instance( );

//	Log.Show_loglevel();


	SDS.Restore_ifd();
	activate_ifd();

	Setup_Wavedisplay();

	show_usage();
	show_AudioServer_Status();


	SDS.Announce( App.client_id, &SDS.addr->Synthesizer );

	Log.Comment(INFO, "Application initialized");

	ApplicationLoop( );
	exit_proc( 0 );
	return 0;
};
