


#include <keys.h>
#include <synthesizer.h>
#include <kbd.h>
#include <Spectrum.h>
#include <mixer.h>
#include <version.h>
#include <Instrument.h>
#include <Wavedisplay.h>
#include <Logfacility.h>
#include <notes.h>
#include <synthmem.h>
#include <Record.h>
#include <External.h>
#include <common.h>
#include <Interface.h>

using namespace std;

Logfacility_class		Log( "Synthesizer" );
Interface_class		GUI;
Mixer_class				Mixer;
Instrument_class 		Instrument(GUI.addr );
Note_class 				Notes( &Instrument );
External_class 			External( &Mixer.StA[ MbIdExternal] );
Shared_Memory			Shm_a, Shm_b;
Wavedisplay_class 		Wavedisplay( GUI.addr );
Memory 					Mono(monobuffer_size); // Wavedisplay output
Kbd_class 				Kbd;
Record_class			Record( GUI.addr );
Loop_class 				master_amp_loop;
Loop_class				record_amp_loop;

bool 					record_thread_done = false;

int ERROR 				= Log.ERROR;
int DEBUG				= Log.DEBUG;
int INFO  				= Log.INFO;
int D2 					= Log.DBG2;

string This_Application = Get_application_name( "Synthesizer" );
vector <string> dirs = {
		dir_struct().homedir,
		dir_struct().basedir,
		dir_struct().etcdir,
		dir_struct().bindir,
		dir_struct().libdir,
		dir_struct().tmpdir,
		dir_struct().vardir,
		dir_struct().musicdir,
		dir_struct().instrumentdir,
		dir_struct().logdir,
		dir_struct().notesdir,
		dir_struct().includedir,
		dir_struct().autodir
};

bool SaveRecordFlag = false;
stereo_t* set_addr( unsigned char tag )
{
	return ( tag == 0 ) ? Shm_a.addr : Shm_b.addr;
}

void creat_dir_structure()

{
	for( string dir : dirs )
	{
		if( filesystem::create_directories( dir ) )
			Log.Comment( INFO, "Synthesizer directory " + dir + " created");
	}
}

using std::istringstream;

config_map_t read_synthesizer_config( )
{
	config_map_t configmap;
	ifstream cFile( file_structure().config_file );

	if (cFile.is_open()) {
        String Line{""};
        while (getline(cFile, Line.Str)) {

        	Line.normalize();
        	string line=Line.Str;

        	istringstream iss(line);
            string strr;
            while (getline(iss, strr, ',')) {
                auto delimiterPos 	= strr.find("=");
                auto name         	= strr.substr(0, delimiterPos);
                auto value      	= strr.substr(delimiterPos + 1);
                configmap[name] 	= value;
            }
        }
    }
    else
    {
        Log.Comment(ERROR, "Couldn't open config file ");
    }
	return configmap;
// example:    shm_key_a=stoi( configmap.at("shm_key_a") );

//    std::cout << shm_key_a << endl;

}

void Setup_Wavedisplay()
{
	Log.Comment(INFO, "Setup Wave display data" );

	Wavedisplay.Add_data_ptr( Mixer.Mono_out.Data ); // TODO stereo out
	Wavedisplay.Add_data_ptr( Instrument.main.Mem.Data );
	Wavedisplay.Add_data_ptr( Instrument.vco.Mem.Data );
	Wavedisplay.Add_data_ptr( Instrument.fmo.Mem.Data );
	Wavedisplay.Add_data_ptr( Mixer.StA[ MbIdExternal].Data );

	Wavedisplay.Set_data_ptr( GUI.addr->Wavedisplay_Id );
}

void record_thead_fcn( )
{
	Value fileno {0};
	while ( not record_thread_done ) 			// run until exit_proc or empty_exit_proc is called
	{
		if ( ( SaveRecordFlag ) )				// triggered by RECORDWAVFILEKEY
		{
			fileno = (int) GUI.addr->FileNo;
			Log.Comment( INFO,
				"record thread received job " + fileno.str);

			External.save_record_data( fileno.i );

			// clean up
			SaveRecordFlag = false;
			Record.Unset();
			GUI.Update( RECORDWAVFILEFLAG ); 	// feedback to GUI
		}
		Wait( 1 * SECOND ); 					// for the next job
	}
	Log.Comment( INFO, "record thread terminated ");
}

thread record_thread(record_thead_fcn);

void empty_exit_proc( int signal )
{
	Log.Comment(INFO, "Entering test case exit procedure for application" + Application );
	record_thread_done = true;
	if( record_thread.joinable() )
		record_thread.join();
	exit( signal );
}
void exit_proc( int signal )
{
	Log.Comment(INFO, "Entering application exit procedure for " + Application );
	record_thread_done = true;
	Log.Comment(INFO, "Clearing output buffers");
	Shm_a.clear();
	Shm_b.clear();
//	Wavedisplay.Clear_data();
	GUI.addr->RecCounter = 0; // memory buffers are empty after restart
	GUI.Announce( "Synthesizer", false);
	GUI.Dump_ifd();
	Kbd.Reset();
	if( record_thread.joinable() )
		record_thread.join();
	exit( signal );
}

void Update_ifd_status_flags()
{
	GUI.addr->mi_status = Mixer.status;
	for ( uint id = 0; id < MbSize; id++ )
	{
		GUI.addr->ma_status[id] = Mixer.StA[id].status;
	}
}

void Volume_control()
{
	master_amp_loop.next( &GUI.addr->Master_Amp );
	Mixer.master_volume = GUI.addr->Master_Amp;

	char amp = Mixer.StA[MbIdExternal].Amp;
	record_amp_loop.next( &amp );
	Mixer.StA[MbIdExternal].Amp = amp;
}

void show_AudioServer_Status()
{
	GUI.addr->AudioServer = NOCONTROL;
	Wait( SECOND );

	if ( GUI.addr->AudioServer == NOCONTROL )
		Log.Comment(0, "Sound server not running with status "+to_string( GUI.ifd_data.AudioServer ) );
	else
		Log.Comment(INFO, "Sound server is up" );
}






void show_usage()
{
	printf("---------------------------------------------------------\n");

	cout << Application << " " << Version_str ;
	cout << endl;

	printf("---------------------------------------------------------\n");

	return;
}

void shutdown_other()
{
	GUI.addr->Synthesizer	= EXITSERVER; 	// kill a running synthesizer
	GUI.addr->UpdateFlag	= true;
	Wait( 2*SECOND );
	GUI.addr->KEY 			= NULLKEY;
}

void process( char key )
{
	ifd_t* ifd = GUI.addr;

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
			Instrument.main.set_frequency( ifd->Main_Freq );
			Notes.main_osc.set_frequency( ifd->Main_Freq );
			GUI.Commit();
			break;
		}
		case VCOFREQUENCYKEY : // modify the secondary oscillator
		{
			Instrument.vco.set_frequency( ifd->VCO_Freq );
			Notes.vco_osc.set_frequency( ifd->VCO_Freq );
			Instrument.main.Connect_vco_data( &Instrument.vco);

			GUI.Commit();
			break;
		}
		case FMOFREQUENCYKEY : // modify the fm_track data
		{
			Instrument.fmo.set_frequency( ifd->FMO_Freq );
			Notes.fmo_osc.set_frequency( ifd->FMO_Freq );

			Instrument.main.Connect_fmo_data( &Instrument.fmo);

			GUI.Commit();
			break;
		}
		case VCOAMPKEY : // modify the VCO volume
		{
			Value vol = ifd->VCO_Amp;
			Log.Comment( INFO, "Changing VCO volume to " + vol.str + " %" );
			Instrument.vco.set_volume( vol.ch );
			Notes.vco_osc.set_volume( vol.ch );
			Instrument.main.Connect_vco_data( &Instrument.vco);
			GUI.Commit();
			break;
		}
		case FMOAMPKEY : // modify the FMO volume
		{
			Instrument.fmo.set_volume(ifd->FMO_Amp );
			Notes.fmo_osc.set_volume( ifd->FMO_Amp );
			Instrument.main.Connect_fmo_data( &Instrument.fmo);
			GUI.Commit();
			break;
		}
		case MASTERAMP_KEY : // modify main volume
		{
			Mixer.master_volume = ifd->Master_Amp;
			GUI.Commit();
			break;

		}
		case MASTERAMP_LOOP_KEY :
		{
			uint16_t beg = Mixer.master_volume;
			uint16_t end = ifd->LOOP_end;
			uint8_t step = ifd->LOOP_step;
			master_amp_loop.start( beg, end, step );
			GUI.Commit();
			break;
		}
		case MASTERAMP_MUTE_KEY : // Mute Main Volume
		{
			Mixer.status.mute = ifd->mi_status.mute;
			if ( Mixer.status.mute )
				Log.Comment( INFO, "receiving command <Mute master volume>");
			else
				Log.Comment( INFO, "receiving command <UnMute master volume>");

			GUI.Commit();
			break;
		}
		case ADSRDURATIONKEY :
		{
			Instrument.main.adsr.bps_id = ifd->Main_adsr_bps_id;
			GUI.Commit();
			break;
		}
		case ADSRDECAYKEY :
		{
			Instrument.main.adsr.attack = ifd->Main_adsr_attack;
			GUI.Commit();
			break;
		}
		case ADSRHALLKEY : // adsr hall
		{
			Instrument.main.adsr.hall = ifd->Main_adsr_hall;
			GUI.Commit();
			break;
		}
		case PMWDIALKEY :
		{
			Instrument.main.wp.PMW_dial = ifd->PMW_dial;
			Instrument.vco.wp.PMW_dial = ifd->PMW_dial;
			Instrument.fmo.wp.PMW_dial = ifd->PMW_dial;
//			Instrument.vco.OSC( 0 );
//			Instrument.fmo.OSC( 0 );
			GUI.Commit();

			break;
		}
		case WAVEDISPLAYTYPEKEY :
		{
			Wavedisplay.Set_type( ifd->WD_type_ID );
			GUI.Commit();
			break;
		}
		case ADSRSUSTAINKEY :
		{
			Instrument.main.adsr.decay = ifd->Main_adsr_decay;
			GUI.Commit();
			break;
		}
		case SOFTFREQUENCYKEY :
		{
			Instrument.main.wp.glide_effect = ifd->Soft_freq;
			GUI.Commit();
			break;
		}
		case SETINSTRUMENTKEY : // Set instrument
		{
			Log.Comment(INFO, "receive command <set instrument>");
			string instrument = GUI.Read_str( INSTRUMENTSTR_KEY ); // other

			if( Instrument.Set(instrument) )
			{
				Log.Comment(INFO, "sucessfully loaded instrument " + instrument );
				Notes.Set_osc_track(  );
			}
			else
			{
				Log.Comment( ERROR, "cannot load instrument" + instrument );
			}

			GUI.Commit(); // reset flags on GUI side
			break;
		}
		case SETEXTERNALWAVEFILE :
		{
			Log.Comment(Log.INFO, "receive command <set external wave file>");
			string wavefile = GUI.Read_str( WAVEFILESTR_KEY );
			if ( External.read_file_header( wavefile ))
			{
				External.read_file_data();
				Mixer.StA[ MbIdExternal ].play_mode( true );
				Mixer.StA[ MbIdExternal ].Amp 	= ifd->MIX_Amp;
				Mixer.status.external				= true;
			}
			else
			{
				Log.Comment(ERROR , "Failed to setup header");
			}
			GUI.Commit();
			break;
		}
		case STOPRECORD_KEY : // stop record on data array id
		{
			Value id { (int)ifd->MIX_Id };
			Log.Comment(Log.INFO, "receive command <stop record on memory bank " + id.str + ">");
			Mixer.StA[id.i].record_mode( false ); // stop recording
			Record.Unset();
			GUI.Commit();
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
					string status = Mixer.StA[MbNr.i].record_mode(true); // start record-stop play
					Log.Comment( INFO, " Memory Bank Id " +  MbNr.str + " recording is " + status);
					Record.Set( &Mixer.StA[MbNr.i].store_counter  , Mixer.StA[MbNr.i].max_counter );
				}
				else // only one mb shall store data
				{
					Mixer.StA[id].record_mode( false );
				}
			}
			GUI.Commit();
			break;
		};

		case EXTERNAL_AMPLOOP_KEY :
		{
			uint16_t beg = Mixer.StA[MbIdExternal].Amp;
			uint16_t end = ifd->LOOP_end;
			uint8_t step = ifd->LOOP_step;
			record_amp_loop.start( beg, end, step );
			GUI.Commit();
			break;

		}
		case SETMBAMPPLAYKEY : // 109 change volume and play data array
		{
			Value arrnr { (int) ifd->MIX_Id };
			Value amp 	{ (int) ifd->MIX_Amp };
			string ONOFF = "on";
			Mixer.StA[arrnr.i].Amp 	= amp.i; // Amp % [0 ... 100]
			if ( Mixer.StA[arrnr.i].store_counter == 0 )
				Log.Comment( Log.WARN , "selected memory >" + arrnr.str + "< is empty ");
			Mixer.StA[arrnr.i].record_mode(false);
			if ( not Mixer.StA[arrnr.i].status.play )
				ONOFF = Mixer.StA[arrnr.i].play_mode( true );
			// start play by slider volume change if not already playing
									//only if array contains data
			Mixer.status.play 			= true; // TODO too unspecific
			Log.Comment(Log.INFO, "receive command <play memory bank " +
					arrnr.str + "> " + ONOFF +
					" vol: " + amp.str + " %");
			ifd->MODE = NOTES; // switch Audio Server into synchronize mode
			GUI.Commit();
			break;
		}
		case MUTEREC_KEY :
		{
			Value id = { (int) ifd->MIX_Id };
			Log.Comment(Log.INFO, "receive command <mute and stop record on id" + id.str + ">");
			Mixer.StA[id.i].mute(); // pause-play, pause-record
			GUI.Commit();
			break;
		}

		case MUTEMBKEY : // clear memory bank flag
		{
			Log.Comment(Log.INFO, "receive command <mute and stop record on all memory banks>");
			for ( uint_t arrnr = 0; arrnr < MbSize; arrnr++ )
			{
				Mixer.StA[arrnr].mute(); // pause-play, pause-record
			}
			Mixer.status.play = false;
			GUI.Commit();
			break;
		}
		case CLEAR_KEY :
		{
			uint8_t id 						= ifd->MIX_Id;
			Log.Comment( INFO, "Clear StA: " + to_string( id ));
			Mixer.StA[ id ].status.play 	= false;
			Mixer.StA[ id ].reset_counter();
			Record.Reset(); // RecCounter
			GUI.Commit();
			break;
		}
		case TOGGLEMBPLAYKEY: // toggle Memory bank status play
		{
			Value arrnr { (int) ifd->MIX_Id };
			Log.Comment(Log.INFO,
					"receive command <toggle play on memory bank" + arrnr.str +" >");
			Mixer.StA[arrnr.i].play_mode( not Mixer.StA[arrnr.i].status.play );
			if ( arrnr.i == MbIdExternal )
				Mixer.status.external 	= Mixer.StA[arrnr.i].status.play;
			if ( arrnr.i == MbIdNotes )
				Mixer.status.notes 	= Mixer.StA[arrnr.i].status.play;
			GUI.Commit();
			break;
		}

		case RESETMAINKEY : // reset main
		{
			Instrument.main.Mem_vco.clear_data(max_data_amp);
			Instrument.main.Mem_fmo.clear_data( 0 );
			Instrument.main.Reset_data( &Instrument.main );
//			Instrument.main.OSC(0);
			GUI.Commit();
			break;
		}
		case UPDATENOTESKEY : // update notes
		{
			Log.Comment(INFO, "receive command <update notes>");
			string notes_name = GUI.Read_str( NOTESSTR_KEY );
			Notes.Set_osc_track( );
			Notes.Read( notes_name );
			ifd->Noteline_sec = Notes.noteline_sec;
			Notes.Start_note_itr();
			GUI.Commit();
			break;
		}
		case NEWNOTESLINEKEY : // setup play or reset play notes
		{
			Log.Comment(Log.INFO, "receive command <setup play notes>");
			Value amp { (int) ifd->MIX_Amp };
			string notes_file = GUI.Read_str( NOTESSTR_KEY );
			Notes.Read( notes_file ); // notes have been written to file by the GUI already
			Mixer.status.notes = true;
			Mixer.StA[ MbIdNotes ].playnotes(true);
			Mixer.StA[ MbIdNotes ].Amp = amp.i;
			GUI.Update( NEWNOTESLINEFLAG );
			ifd->Noteline_sec = Notes.noteline_sec;
			Notes.Start_note_itr();
			break;
		}
		case PLAYNOTESREC_ON_KEY : // play modnt.composer = true;
		{
			Value sec { ifd->Noteline_sec };
			Value id  { ifd->MIX_Id };

			Mixer.composer = sec.i;
			Log.Comment(INFO, "generate composer notes");
			Log.Comment(INFO, "duration: " + sec.str + " sec.");
			Log.Comment(INFO, "store to array nr.: " + id.str );
			Mixer.StA[ id.i].record_mode( true );
			Notes.Set_osc_track(  );

			Mixer.add_noteline( id.i, &Notes );

			GUI.Commit();
			break;
		}
		case PLAYNOTESRECOFF_KEY : // play free
		{ // functionality is defined in PLAYNOTESREC_ON_KEY
			Mixer.composer = 0;
			GUI.Commit();
			break;
		}
		case NOTESONKEY :
		{
			Value amp { (int) ifd->MIX_Amp };
			Log.Comment(Log.INFO, "receive command < notes on " + amp.str + "%>");
			Mixer.status.notes = true;

			Mixer.StA[ MbIdNotes ].playnotes(true);
			Mixer.StA[ MbIdNotes ].Amp = amp.i;
			Notes.Start_note_itr();
			GUI.Commit();
			break;
		}
		case NOTESOFFKEY :
		{
			Log.Comment(Log.INFO, "receive command < notes off>");
			Mixer.status.notes = false;
			Mixer.StA[ MbIdNotes ].playnotes(false);
			GUI.Commit();
			break;
		}
		case UPDATENOTELINEKEY: // update Noteline during play
		{
			Log.Comment(Log.INFO, "receive command <update Noteline during play>");
			string notes_file = GUI.Read_str( NOTESSTR_KEY );
			Notes.Read( notes_file );
			GUI.Commit();
			break;
		}
		case SETBASEOCTAVE_KEY :
		{
			Value diff_oct { (int) ifd->FLAG };
			Notes.Set_base_octave( diff_oct.i ); // is positive, therefore identify 0 -> -1, 1 -> 1
			GUI.Commit();
			break;
		}

		case SETNOTESPERSEC_KEY:
		{
			Value nps = ifd->FLAG;
			Log.Comment(Log.INFO, "receive command <set notes per second>");
			if ( not Notes.Set_notes_per_second( nps.i ) )
			{
				Log.Comment( ERROR, nps.str + " notes per second not supported");
			}
			GUI.Commit();
			break;
		}

		case SETWAVEDISPLAYKEY :
		{
			Wavedisplay.Set_data_ptr( ifd->Wavedisplay_Id );
			GUI.Commit();
			break;
		}
		case CONNECTFMOVCOKEY : // connect FMO volume with vco data
		{

			Instrument.fmo.Connect_fmo_data( &Instrument.vco );
//			Instrument.fmo.OSC(0);
			Instrument.main.Connect_fmo_data( &Instrument.fmo );
			GUI.Commit();
			break;
		}
		case RESETFMOKEY : // reset FMO
		{
			Instrument.fmo.Reset_data( &Instrument.fmo );
//			Instrument.fmo.OSC(0);
			GUI.Commit();
			break;
		}
		case RECORDWAVFILEKEY : // record and save wav file
		{
			if ( SaveRecordFlag )
			{
				Log.Comment( Log.WARN, "Thread is saving data. ... Wait ");
				break;
			}
			Log.Comment(INFO,"Record Audio out to wav file format.");
			External.status.record = not External.status.record;
			if ( ifd->Composer == RECORD ) // composer overwrites default behaviour
				External.status.record = true;
			if ( ifd->Composer == STOPRECORD )
				External.status.record = false;
			string str = External.status.record ? "start" : "stop ";
			Log.Comment(INFO, str);

			if ( not External.status.record ) 	// STOPRECORD
			{
				Mixer.StA[MbIdExternal].record_mode( false );

				//no ifd-commit for this section
				ifd->KEY = NULLKEY;  //but do not start twice
				SaveRecordFlag = true; // trigger the record thread
			}
			else 								// RECORD
			{
				External.stereo.info.record_counter = 0;
				External.stereo.clear_data();
				Mixer.StA[MbIdExternal].record_mode( true );
				Record.Set( &External.stereo.info.record_counter,
							 External.stereo.info.max_records );
				GUI.Commit();
			}
			break;
		}
		case RESETVCOKEY : // reset VCO
		{
			Instrument.vco.Reset_data( &Instrument.vco );
//			Instrument.vco.OSC(0);
//			Instrument.main.connect_vco_data( *null_itr );
			GUI.Commit();
			break;
		}
		case CONNECTVCOFMOKEY ://connect VCO frequency with FMO data
		{
			Instrument.vco.Connect_vco_data( &Instrument.fmo );
//			Instrument.vco.OSC(0);
			Instrument.main.Connect_vco_data( &Instrument.vco );
			GUI.Commit();
			break;
		}
		case SETWAVEFORMFMOKEY :
		{
			set_waveform( &Instrument.fmo, ifd->FMO_spectrum.id );
//			uint8_t id = ifd->FMO_waveform_id;
//			Instrument.Set_waveform( &Instrument.fmo, id );
//			Instrument.fmo.wp.spectrum = Instrument.fmo.Get_spectrum(id);

//			Instrument.fmo.(0);
			GUI.Commit();
			break;
			}
		case SETWAVEFORMVCOKEY :
		{
			set_waveform( &Instrument.vco, ifd->VCO_spectrum.id );
//			Instrument.vco.OSC(0);
			GUI.Commit();
			break;
		}
		case SETWAVEFORMMAINKEY :
		{
			set_waveform( &Instrument.main, ifd->MAIN_spectrum.id );
			GUI.Commit();
			break;
		}

		case UPDATESPECTRUM_KEY :
		{
			Instrument.Update_spectrum();
			GUI.Commit();
			break;
		}
		case SAVEINSTRUMENTKEY :
		{
			Log.Comment(INFO, "saving current config to instrument " + Instrument.Name );
			Instrument.Save_Instrument( Instrument.Name );
			GUI.Commit();
			break;
		}

		case NEWINSTRUMENTKEY : // save instrument file
		{
			string instrument = GUI.Read_str( INSTRUMENTSTR_KEY );
			Log.Comment(INFO, "receiving instrument change to " + instrument );
			Instrument.Save_Instrument( instrument );
			GUI.Update( NEWINSTRUMENTFLAG );
			break;
		}

		case EXITKEY :
		{

			exit_proc( 1 );
			break;
		}

		default :
		{
			string str;
			str.assign(1, key);
			Log.Comment( Log.WARN,"Communication Key >" + to_string((int)key) + "< undefined");
			break;
		}

	} // switch char

}

void activate_ifd()
{
	for( char key : init_keys )
		process( key );
}

void play_keyboard( char key )
{
	Mixer.status.notes = true;
	Mixer.StA[MbIdNotes].status.play = false;

	freq_struc_t fs;
	fs = Instrument.vco.get_fstruct();
	Instrument.vco.wp.frequency = fs.base;
	Instrument.vco.set_delta_frequency(  (fs.base + fs.pitch)* key/12 );
//	Instrument.vco.OSC( 0 );

	fs = Instrument.fmo.get_fstruct();
	Instrument.fmo.wp.frequency = fs.base;
	Instrument.fmo.set_delta_frequency(  (fs.base + fs.pitch)* key/12 );
//	Instrument.fmo.OSC( 0 );

	fs = Instrument.main.get_fstruct();
	Instrument.main.wp.frequency = fs.base  ;
	Instrument.main.set_delta_frequency( (fs.base + fs.pitch)* key / 12  );
}

bool sync_mode()
{
	bool sync = ( 							// if true synchronize shm a/b with Audio Server
		( Instrument.fmo.wp.frequency < LFO_limit ) 	or
		( Mixer.status.notes 		)	or	// generate notes
		( Mixer.status.external 	)	or	// StA play external
		( Mixer.status.play 		)	or	// any StA triggers play if itself is in play mode
		( Record.active 			)		// StA record external
		);
	return sync ;
}

void ApplicationLoop()
{
	Log.Comment(INFO, "Entering Application loop\n");
	ifd_t* ifd = GUI.addr;
	stereo_t* 	shm_addr 	= set_addr( 0 );

	GUI.Commit(); // set flags to zero

	key_struct_t 	keystruct 	= Kbd.GetKey();
	Log.Comment(INFO, "Keyboard is ready");

	while ( ifd->Synthesizer != EXITSERVER )
	{
		ifd->Synthesizer = RUNNING;

		keystruct = Kbd.GetKey();
		size_t key = kbdnote( keystruct.key );
		if ( key != string::npos )
		{
			play_keyboard(key);
		}
		else
		{
			process( ifd->KEY );
		} // if kbd key

		Volume_control();



		if ( sync_mode() )
		{
			if ( Mixer.composer > 0  )
			{
				Mixer.composer 		= 0;
				ifd->Noteline_sec 	= 0; // done, do not reuse outdated info
			}
			else
			{
				if ( ifd->MODE == SENDDATA )				// Audio Server request 1 second data chunk
				{
					if ( not Mixer.status.mute )
						Instrument.Run_osc_group(); // generate the modified sound waves

					if ( Mixer.status.notes )
					{
						Notes.Set_osc_track(  );
						Notes.Generate_note_chunk( &Mixer.StA[MbIdNotes] ); // max_sec duration
					}

					ifd->UpdateFlag = true;
					shm_addr 		= set_addr( ifd->SHMID );
					Mixer.add( &Instrument, shm_addr, Record.active );
					Wavedisplay.Gen_cxwave_data(  );
				} 											// but handel further requests
			}
			ifd->MODE 		= NOTES;					// do not update Audio Server data

		}
		else
		{
			if ( not Mixer.status.mute )
				Instrument.Run_osc_group(); // generate the modified sound waves

			ifd->MODE 		= FREERUN;
			shm_addr 		= set_addr( 0 );
			Mixer.add( &Instrument, shm_addr, Record.active );
			Wavedisplay.Gen_cxwave_data(  );


		}

		if ( Record.active )
			External.add_record( &Mixer.Out_L, &Mixer.Out_R);//, Mixer.StA[ MbIdExternal ].Amp );
		Record.Progress_bar_update();

		Update_ifd_status_flags();
	} // while not EXITSERVER


	Log.Comment(INFO, "Exit Application loop");
	return;

} // Application loop


void test_classes()
{

	Log.init_log_file();

	Log.test();

	Loop_class Loop;
	Loop.test();

	String 			TestStr{""};
	TestStr.test();

	Log.test();

	Notes.Test();

	Oscillator 		TestOsc{"Test"};
	TestOsc.Test();

	Instrument.Test_Instrument();

	Mixer.test();

	External.test();

}



int main( int argc, char* argv[] )
{

	Log.Comment(INFO, "Evaluating startup arguments");

	prgarg_struct_t params = parse_argv( argc, argv );

	if ( params.test == 'y' )
	{
		Log.Comment(INFO, "entering test classes ");
		signal( SIGABRT, &empty_exit_proc );
		test_classes();
		exit(1);
	}

	Log.Comment(INFO, "Catching signals SIGINT and SIGABRT");
	signal( SIGINT, &exit_proc );
	signal( SIGABRT, &exit_proc );

	Log.init_log_file();
	Log.Show_loglevel();

	Log.Comment(INFO,"Checking directory structure");
	creat_dir_structure();

	Log.Comment(Log.INFO, "Reading config file " + file_structure().config_file );
    read_synthesizer_config( );

	Log.Comment(INFO, "Entering application init for " + This_Application );

	shutdown_other();

	Log.Comment(INFO,"Attaching data buffers");
	Shm_a.buffer( sharedbuffer_size, params.shm_key_a );
	Shm_b.buffer( sharedbuffer_size, params.shm_key_b );
	Shm_a.info();
	Shm_b.info();

	Setup_Wavedisplay();

	GUI.Restore_ifd();
	activate_ifd();

	show_usage();
	show_AudioServer_Status();

	Log.Comment(INFO, "Application initialized");

	GUI.Announce( "Synthesizer", true );

	ApplicationLoop( );

	exit_proc(0);
	return 0;
};
