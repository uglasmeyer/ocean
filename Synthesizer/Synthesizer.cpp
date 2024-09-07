


#include <External.h>
#include <Instrument.h>
#include <Keyboard.h>
#include <Logfacility.h>
#include <Record.h>
#include <Spectrum.h>
#include <Wavedisplay.h>
#include <common.h>
#include <keys.h>
#include <mixer.h>
#include <notes.h>
#include <App.h>
#include <Synthesizer.h>
#include <Synthmem.h>

using namespace std;

string					Module = "Synthesizer";
Logfacility_class		Log( Module );
Interface_class			SDS;
Application_class		App( Module, SYNTHID, &SDS.addr->Synthesizer);
Mixer_class				Mixer;
Instrument_class 		Instrument(SDS.addr );
Note_class 				Notes;
Keyboard_class			Keyboard( &Instrument, &Mixer.StA[MbIdKeyboard] );
External_class 			External( &Mixer.StA[ MbIdExternal] );
Shared_Memory			Shm_a, Shm_b;
Wavedisplay_class 		Wavedisplay( SDS.addr );
Memory 					Mono(monobuffer_size); // Wavedisplay output
Record_class			Record( SDS.addr );

bool 					record_thread_done = false;


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

bool SaveRecordFlag = false;

void record_thead_fcn( )
{
	Value fileno {0};
	while ( not record_thread_done ) 			// run until exit_proc or empty_exit_proc is called
	{
		if ( ( SaveRecordFlag ) )				// triggered by RECORDWAVFILEKEY
		{
			fileno = (int) SDS.addr->FileNo;
			Log.Comment( INFO,
				"record thread received job " + fileno.str);

			External.Save_record_data( fileno.i );

			// clean up
			SaveRecordFlag = false;
			Record.Unset();
			SDS.Update( RECORDWAVFILEFLAG ); 	// feedback to GUI
		}
		Wait( 1 * SECOND ); 					// for the next job
	}
	Log.Comment( INFO, "record thread terminated ");
}

thread record_thread(record_thead_fcn);

void empty_exit_proc( int signal )
{
	Log.Comment(INFO, "Entering test case exit procedure for application" + Application );
	SDS.addr->RecCounter = 0; // memory buffers are empty after restart
	App.Decline( SDS.addr );
	SDS.Dump_ifd();
	Keyboard.Reset();

	record_thread_done = true;
	while( not record_thread.joinable() )
	record_thread.join();
	exit( 0 );
}
void exit_proc( int signal )
{

	Log.Comment(INFO, "Entering application exit procedure for " + Application );
	Log.Comment(INFO, "Clearing output buffers");
	Shm_a.clear();
	Shm_b.clear();
//	Wavedisplay.Clear_data();
	SDS.addr->RecCounter 	= 0; // memory buffers are empty after restart
	SDS.addr->time_elapsed 	= 0;
	App.Decline( SDS.addr );
	Mixer.Clear_StA_status( SDS.addr->StA_status );
	SDS.Dump_ifd();
	Keyboard.Reset();

	record_thread_done = true;
	while( not ( record_thread.joinable() ) )
	{
		Wait( 1*MILLISECOND );
		cout << " join wait " << endl;
	}
	record_thread.join();
	exit( 0 );
}


void show_AudioServer_Status()
{
	if ( SDS.addr->AudioServer == RUNNING )
		Log.Comment(INFO, "Sound server is up" );
	else
		Log.Comment( ERROR, "Sound server not running with status " +
							uint8_code_str[ SDS.ifd_data.AudioServer ] );
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
		case SETEXTERNALWAVEFILE :
		{
			Log.Comment(INFO, "receive command <set external wave file>");
			string wavefile = SDS.Read_str( WAVEFILESTR_KEY );
			if ( External.Read_file_header( wavefile ))
			{
				External.Read_file_data();
				Mixer.StA[ MbIdExternal ].Play_mode( true );
				Mixer.StA[ MbIdExternal ].Amp 	= 100;//ifd->StA_amp_arr[MbIdExternal];
				Mixer.status.external			= true;

				Record.Set_progress_bar( 100*External.Filedata_size / External.stereo.info.mem_bytes );
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
			Record.Unset();
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
					Record.Set( &Mixer.StA[MbNr.i].store_counter  , Mixer.StA[MbNr.i].max_counter );
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
			Value amp	{ ifd->StA_amp_arr[mixid.i]} ;
			Value play 	{ ifd->StA_status[ mixid.i ].play };

			Mixer.StA[mixid.i].Amp = amp.i;
			Mixer.StA[mixid.i].status.play = (bool) play.i;
			Mixer.Set_mixer_state( mixid.i, (bool)play.i );
			Log.Comment( INFO, 	"Mixer state ID " + mixid.str +
								" Amp: " + amp.str +
								" Logic: " + play.boolstr);

			SDS.Commit();
			break;
		}
		case MUTEREC_KEY :
		{
			Value id = { (int) ifd->MIX_Id };
			Log.Comment(INFO, "receive command <mute and stop record on id" + id.str + ">");
			Mixer.StA[id.i].Mute(); // pause-play, pause-record
			SDS.Commit();
			break;
		}
		case MUTEMBKEY : // clear memory bank flag
		{
			Log.Comment(INFO, "receive command <mute and stop record on all memory banks>");
			std::ranges::for_each( Mixer.StA, []( auto& sta ){ sta.Mute(); } );

		    for( uint id = 0; id < MbSize; id++ )
		    {
		    	Mixer.Set_mixer_state( id, false );
		    }

			SDS.Commit();
			break;
		}
		case CLEAR_KEY :
		{
			uint8_t id 						= ifd->MIX_Id;
			Log.Comment( INFO, "Clear StA: " + to_string( id ));
			Mixer.StA[ id ].Reset_counter();
			Record.Reset(); // RecCounter
			SDS.Commit();
			ifd->UserInterface = UPDATEGUI;
			break;
		}
		case TOGGLEMBPLAYKEY: // toggle Memory bank status play
		{
			Value arrnr { (int) ifd->MIX_Id };
			Log.Comment(INFO,
					"receive command <toggle play on memory bank" + arrnr.str +" >");
			Mixer.StA[arrnr.i].Play_mode( not Mixer.StA[arrnr.i].status.play );
			if ( arrnr.i == MbIdExternal )
				Mixer.status.external 	= Mixer.StA[arrnr.i].status.play;
			if ( arrnr.i == MbIdNotes )
				Mixer.status.notes 	= Mixer.StA[arrnr.i].status.play;
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
				break;
			}
			External.status.record = not External.status.record;
			if ( ifd->Composer == RECORD ) // composer overwrites default behaviour
				External.status.record = true;
			if ( ifd->Composer == STOPRECORD )
				External.status.record = false;
			string str = External.status.record ? "Start" : "Stop ";

			Log.Comment(INFO, str + "record Audio out to wav file format.");

			if ( not External.status.record ) 	// STOPRECORD, also means: save to file
			{
				Mixer.StA[MbIdExternal].Record_mode( false );

				//no ifd-commit for this section
				ifd->KEY = NULLKEY;  //but do not start twice
				SaveRecordFlag = true; // trigger the record thread
			}
			else 								// RECORD
			{
				External.stereo.info.record_counter = 0;
				External.stereo.clear_data();
				Mixer.StA[MbIdExternal].Record_mode( true );
				Record.Set( &External.stereo.info.record_counter,
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

}
// TODO process end location

void activate_ifd()
{
	for( char key : init_keys )
		process( key );
	Mixer.status = SDS.addr->mixer_status;
	for ( uint id : Mixer.MemIds )
	{
		Mixer.StA[ id ].Amp = SDS.addr->StA_amp_arr[id];
		Mixer.StA[ id ].status.play = SDS.addr->StA_status[id].play;
	}
	SDS.addr->UserInterface = UPDATEGUI;
}

bool sync_mode()
{
	int play = 0;
	for( uint id : Mixer.SycIds)
		play += (int) Mixer.StA[id].status.play;

	Mixer.status.play = (bool) play;
	bool sync =
		( 	// if true synchronize shm a/b with Audio Server
			( Instrument.fmo.wp.frequency < LFO_limit 	)	or
			( Instrument.vco.wp.frequency < LFO_limit 	) 	or
			( Mixer.StA[MbIdExternal].status.store 		)	or
			( Mixer.status.play 		)	or	// any StA triggers play if itself is in play mode
			( Record.active 			)		// StA record external
		);
	return sync ;
}

Time_class Timer{};

void Performanc()
{
	Timer.Stop();
	uint8_t time_elapsed 	= Timer.Time_elapsed() / 10; // time elapsed in percentage w.r.t. 1 second
	SDS.addr->time_elapsed 	= time_elapsed;
	Timer.Start();
//	cout << (int)time_elapsed << endl;
}

void add_sound( stereo_t* shm_addr )
{
	if ( Mixer.status.instrument )
		Instrument.Run_osc_group(); // generate the modified sound waves
	Mixer.Add_Sound( Instrument.main.Mem.Data, Keyboard.main.Mem.Data,  Notes.main.Mem.Data, shm_addr );
	Wavedisplay.Gen_cxwave_data(  );

}
void ApplicationLoop()
{
	Log.Comment(INFO, "Entering Application loop\n");

	ifd_t* 			ifd 		= SDS.addr;
	stereo_t* 		shm_addr 	= set_addr( 0 );

	Log.Comment(INFO, App.Name + " is ready");

	SDS.Commit(); // set flags to zero and update flag to true

	while ( ifd->Synthesizer != EXITSERVER )
	{
		Performanc();

		char Key = ifd->KEY;
		process( Key );
		Mixer.Update_ifd_status_flags( ifd );


		int note_key = Keyboard.Kbdnote( );
		if ( Keyboard.Attack( note_key, 100 ) )
		{
			cout << "KEY: " << note_key << endl;
			Mixer.StA[ MbIdKeyboard].status.play = true;
//			Mixer.status.kbd = true;
//			add_sound( shm_addr );
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

				if ( Record.active )
					External.Add_record( &Mixer.Out_L, &Mixer.Out_R);
				Record.Progress_bar_update();

				ifd->UpdateFlag = true;
				int shm_id 	= ifd->SHMID;
				shm_addr 	= set_addr( shm_id );
			} 	// but handel further requests

		}
		else
		{
			add_sound( shm_addr );
			shm_addr 		= set_addr( 0 );
			ifd->MODE = FREERUN;
		}
	} ;


	Log.Comment(INFO, "Exit Application loop");
	return;

} // Application loop


void test_classes()
{

	Log.init_log_file();

    config_map_t Synthesizer_cfg = read_synthesizer_config( );

	Log.test();

	Loop_class Loop;
	Loop.Test();

	String 			TestStr{""};
	TestStr.test();

	Log.test();

	Keyboard.Test();

	Notes.Test();

	Oscillator 		TestOsc{ TESTID };
	TestOsc.Test();

	Instrument.Test_Instrument();

	Mixer.test();

	External.test();
    config_map_t cfg = read_synthesizer_config( );
	External.Id3tool_cmd( cfg["Title"], cfg["Author"], cfg["Genre"], cfg["Album"]);
	string I = cfg["int"];
	cout << dec << atoi(I.data()) <<endl;
    Server_struct().TERM = cfg["TERM"];
    cout << Server_struct().cmd( Audio_Srv, "help") << endl;
}



int main( int argc, char* argv[] )
{
	App.Start();

	Log.Comment(INFO, "Evaluating startup arguments");
	prgarg_struct_t params = parse_argv( argc, argv );

	if ( params.test == 'y' )
	{
		Log.init_log_file();
		Log.Comment(INFO, "entering test classes ");
		signal( SIGABRT, &empty_exit_proc );
		test_classes();
		empty_exit_proc(0);
	}

	Log.Comment(INFO, "Catching signals SIGINT and SIGABRT");
	signal( SIGINT	, &exit_proc );
	signal( SIGABRT	, &exit_proc );
	signal(	SIGHUP	, &exit_proc );


	Log.Show_loglevel();

	Log.Comment(INFO,"Checking directory structure");
	creat_dir_structure();

	Log.Comment(INFO, "Reading config file " + file_structure().config_file );
    config_map_t cfg = read_synthesizer_config( );

    External.Id3tool_cmd( cfg["Title"], cfg["Author"], cfg["Genre"], cfg["Album"]);
    Server_struct().TERM = cfg["TERM"];

	App.Shutdown_instance( );

	Log.Comment(INFO,"Attaching data buffers");
	Shm_a.buffer( sharedbuffer_size, params.shm_key_a );
	Shm_b.buffer( sharedbuffer_size, params.shm_key_b );
	Shm_a.info();
	Shm_b.info();

	Setup_Wavedisplay();

	SDS.Restore_ifd();
	activate_ifd();

	show_usage();
	show_AudioServer_Status();
    SDS.Announce( App.client_id, App.status );

	Log.Comment(INFO, "Application initialized");

	ApplicationLoop( );

	exit_proc(0);
	return 0;
};
