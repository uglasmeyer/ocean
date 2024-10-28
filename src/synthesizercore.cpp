/*
 * synthesizercore.cpp
 *
 *  Created on: Oct 28, 2024
 *      Author: sirius
 */


#include <Synthesizer.h>


void Core_class::processKey( char key )
{
//	interface_t* sds = DaTA.GetSdsAddr();

	auto set_waveform = [ this ]( Oscillator* osc, char id )
		{
			osc->Set_waveform( id);
			string wf = osc->Get_waveform_str( id );
			this->Comment(INFO, "set waveform >" + wf + "< for " + osc->osc_type );
		};

	switch ( key )
	{
		case NULLKEY :
		{
			break;
		}
		case MAINFREQUENCYKEY :
		{
			Info( "Frequency set to " + to_string( sds->Main_Freq ));
			Instrument->main.Set_frequency( sds->Main_Freq );
			Notes->main.Set_frequency( sds->Main_Freq );
			Sds->Commit();
			break;
		}
		case VCOFREQUENCYKEY : // modify the secondary oscillator
		{
			Instrument->vco.Set_frequency( sds->VCO_Freq );
			Notes->vco.Set_frequency( sds->VCO_Freq );
			Instrument->main.Connect_vco_data( &Instrument->vco);

			Sds->Commit();
			break;
		}
		case FMOFREQUENCYKEY : // modify the fm_track data
		{
			Instrument->fmo.Set_frequency( sds->FMO_Freq );
			Notes->fmo.Set_frequency( sds->FMO_Freq );

			Instrument->main.Connect_fmo_data( &Instrument->fmo);

			Sds->Commit();
			break;
		}
		case VCOAMPKEY : // modify the VCO volume
		{
			Value vol = sds->VCO_Amp;
			Comment( INFO, "Changing VCO volume to " + vol.str + " %" );
			Instrument->vco.Set_volume( vol.ch );
			Notes->vco.Set_volume( vol.ch );
			Instrument->main.Connect_vco_data( &Instrument->vco);
			Sds->Commit();
			break;
		}
		case FMOAMPKEY : // modify the FMO volume
		{
			Value vol = sds->FMO_Amp;
			Instrument->fmo.Set_volume( vol.ch );
			Notes->fmo.Set_volume( vol.ch );
			Instrument->main.Connect_fmo_data( &Instrument->fmo);
			Notes->fmo.Set_volume( vol.ch );
			Sds->Commit();
			break;
		}
		case MASTERAMP_KEY : // modify main volume
		{
			Mixer->master_volume = sds->Master_Amp;
			Mixer->status.mute = false;
			Sds->Commit();
			break;

		}
		case MASTERAMP_LOOP_KEY :
		{
			uint16_t beg = Mixer->master_volume;
			uint16_t end = sds->LOOP_end;
			uint8_t step = sds->LOOP_step;
			Mixer->amp_loop_vec[ MbSize ].Start( beg, end, step );
			Sds->Commit();
			break;
		}
		case MASTERAMP_MUTE_KEY : // Mute Main Volume
		{
			Mixer->status.mute = not sds->mixer_status.mute;
			string str = ( Mixer->status.mute ) ? "Mute" : "UnMute";
			Comment( INFO, "receiving command <"+str+"> master volume>");
			Sds->Commit();
			break;
		}
		case ADSR_KEY :
		{
			Instrument->main.Set_adsr( sds->Main_adsr );
			Sds->Commit();
			break;
		}
		case PMWDIALKEY :
		{
			Instrument->main.Set_pmw(sds->PMW_dial );
			Instrument->vco.Set_pmw( sds->PMW_dial );
			Instrument->fmo.Set_pmw( sds->PMW_dial );

			Sds->Commit();

			break;
		}
		case WAVEDISPLAYTYPEKEY :
		{
			Wavedisplay->Set_type( sds->WD_type_ID );
			Sds->Commit();
			break;
		}
		case SOFTFREQUENCYKEY :
		{
			Instrument->main.Set_glide( sds->Soft_freq );
			Sds->Commit();
			break;
		}
		case SETINSTRUMENTKEY : // Set instrument
		{
			Comment(INFO, "receive command <set instrument>");
			string instrument = Sds->Read_str( INSTRUMENTSTR_KEY ); // other

			if( Instrument->Set(instrument) )
			{
				Comment(INFO, "sucessfully loaded instrument " + instrument );
				Notes->Set_instrument( Instrument );
			}
			else
			{
				Comment( ERROR, "cannot load instrument" + instrument );
			}

			Sds->Commit(); // reset flags on GUI side
			break;
		}
		case SAVE_EXTERNALWAVFILEKEY : // record and save wav file
		{
			if( sds_master->Record ) // Composer - Interpreter
				sds_master->AudioServer = RECORDSTART; // start and  wait
			else
				sds_master->AudioServer = RECORDSTOP; // stop and save
			Sds->Commit();
			break;
		}
		case READ_EXTERNALWAVEFILE :
		{
			Comment(INFO, "receive command <set external wave file>");
			string wavefile = Sds->Read_str( WAVEFILESTR_KEY );
			Sem->Lock( SEMAPHORE_TEST, 1 ); // assume record thread is working on that file
			if ( External->Read_file_header( wavefile ))
			{
				External->Read_file_data();
				Mixer->StA[ MbIdExternal ].Play_mode( true );
				Mixer->StA[ MbIdExternal ].Amp 	= 100;//ifd->StA_amp_arr[MbIdExternal];
				Mixer->status.external			= true;

				ProgressBar->SetValue( 100*External->Filedata_size / External->stereo.ds.mem_bytes );
			}
			else
			{
				Comment(ERROR , "Failed to setup header");
			}
			Sds->Commit();
			Sds->addr->UserInterface = UPDATEGUI; //set cb_sta play flag for external
			break;
		}
		case STOPRECORD_KEY : // stop record on data array id
		{
			Value id { (int)sds->MIX_Id };
			Comment(INFO, "receive command <stop record on storage area " + id.str + ">");
			Mixer->StA[id.val].Record_mode( false ); // stop recording
			ProgressBar->Unset();
			Sds->Commit();
			break;
		}
		case STORESOUNDKEY : //start record
		{
			Value MbNr { (int) sds->MIX_Id };
			Comment( INFO, "receiving command <store sound to memory bank " + MbNr.str + " >");
			for ( int id : Mixer->MemIds )
			{
				if ( MbNr.val == id )
				{
					string status = Mixer->StA[MbNr.val].Record_mode(true); // start record-stop play
					Comment( INFO, " Storage Id " +  MbNr.str + " recording is " + status);
					ProgressBar->Set( Mixer->StA[MbNr.val].Get_storeCounter_p()  ,
								Mixer->StA[MbNr.val].ds.max_records );
				}
				else // only one mb shall store data
				{
					Mixer->StA[id].Record_mode( false );
				}
			}
			Sds->Commit();
			break;
		};

		case EXTERNAL_AMPLOOP_KEY :
		{
			uint8_t 	Id	= sds->MIX_Id;
			uint16_t 	beg	= Mixer->StA[ Id ].Amp;
			uint16_t 	end = sds->LOOP_end;
			uint8_t step 	= sds->LOOP_step;
			Mixer->amp_loop_vec[ Id ].Start( beg, end, step );
			Sds->Commit();
			break;

		}
		case SETMBAMPPLAYKEY : // 109 change volume and play data array
		{

			Value mixid { sds->MIX_Id };
			Value amp	{ sds->StA_amp_arr[mixid.val] } ;
			Value play 	{ sds->StA_state[ mixid.val ].play };

			Mixer->StA[mixid.val].Amp = amp.val;
			Mixer->Set_mixer_state( mixid.val, (bool)play.val );
			Comment( INFO, 	"Mixer ID " + mixid.str +
								" Amp: " 	+ amp.str +
								" State: " 	+ play.boolstr);

			Sds->Commit();
			break;
		}
		case MUTEREC_KEY :
		{
			Value id = { (int) sds->MIX_Id };
			Comment(INFO, "receive command <mute and stop record on id" + id.str + ">");
			Mixer->StA[id.val].Play_mode( false ); // pause-play, pause-record
			Sds->Commit();
			break;
		}
		case MUTEMBKEY : // clear all memory bank flag
		{
			Comment(INFO, "receive command <mute and stop record on all memory banks>");
		    for( uint id : Mixer->RecIds )
		    	{ Mixer->Set_mixer_state( id, false ); }

			Sds->Commit();
			break;
		}
		case CLEAR_KEY :
		{
			uint8_t id 						= sds->MIX_Id;
			Comment( INFO, "Clear StA: " + to_string( id ));
			Mixer->StA[ id ].Reset_counter();
			ProgressBar->Reset(); // RecCounter
			Sds->Commit();
			break;
		}
		case TOGGLEMBPLAYKEY: // toggle Memory bank status play
		{
			Value Id 	{ (int)sds->MIX_Id };
			bool play 	{ not (sds->StA_state[ Id.val ].play) };
			sds->StA_state[ Id.val].play = play ;
			Comment(INFO,
					"receive command <toggle play on memory bank" + Id.str +" >" + to_string( play ) );
			Mixer->Set_mixer_state( Id.val, play );

			Sds->Commit();
			break;
		}

		case RESETMAINKEY : // reset main
		{
			Instrument->main.Mem_vco.Clear_data(max_data_amp);
			Instrument->main.Mem_fmo.Clear_data( 0 );
			Instrument->main.Reset_data( &Instrument->main );

			Sds->Commit();
			break;
		}
		case UPDATENOTESKEY : // update notes
		{
			Comment(INFO, "receive command <update notes>");
			string notes_name = Sds->Read_str( NOTESSTR_KEY );
			Notes->Set_instrument( Instrument);
			Notes->Read( notes_name );
			sds->Noteline_sec = Notes->noteline_sec;
			Sem->Release( SEMAPHORE_NOTES );
			Sds->Commit();
			break;
		}
		case NEWNOTESLINEKEY : // setup play or reset play notes
		{
			Comment(INFO, "receive command <setup play notes>");
			string notes_file = Sds->Read_str( NOTESSTR_KEY );
			Notes->Read( notes_file ); // notes have been written to file by the GUI already
			Mixer->status.notes = true;
			Sds->Update( NEWNOTESLINEFLAG );
			sds->Noteline_sec = Notes->noteline_sec;
			Sem->Release( SEMAPHORE_NOTES );
//			Notes->Start_note_itr();
			break;
		}
		case PLAYNOTESREC_ON_KEY : // play modnt.composer = true;
		{
			Value sec { sds->Noteline_sec };
			Value id  { sds->MIX_Id };

			if ( sec.val > 0 )
			{
				Comment(INFO, "generate composer notes");
				Comment(INFO, "duration: " + sec.str + " sec.");
				Comment(INFO, "store to StA id: " + id.str );

				Mixer->composer = sec.val;
				Mixer->StA[ id.val].Record_mode( true );
				Notes->Set_instrument( Instrument );
				Mixer->Store_noteline( id.val, Notes );

			}
			else
			{
				Comment( WARN, "nothing to do for " + sec.str + " Notes!" );
			}
			Sds->Commit();
			break;
		}
		case PLAYNOTESRECOFF_KEY : // play free
		{ // functionality is defined in PLAYNOTESREC_ON_KEY
			Mixer->composer = 0;
			Sds->Commit();
			break;
		}
		case NOTESONKEY :
		{
			Value amp { (int) sds->StA_amp_arr[ MbIdNotes] };
			Comment(INFO, "receive command < notes on " + amp.str + "%>");
			Mixer->StA[ MbIdNotes ].Amp = amp.val;
			Mixer->Set_mixer_state( MbIdNotes, true );
			Sem->Release( SEMAPHORE_NOTES );
//			Notes->Start_note_itr();
			Sds->Commit();
			break;
		}
		case NOTESOFFKEY :
		{
			Comment(INFO, "receive command < notes off>");
			Mixer->Set_mixer_state( MbIdNotes, false );
			Sds->Commit();
			break;
		}
		case UPDATENOTELINEKEY: // update Noteline during play
		{
			Comment(INFO, "receive command <update Noteline during play>");
			string notes_file = Sds->Read_str( NOTESSTR_KEY );
			Notes->Read( notes_file );
			Sds->Commit();
			break;
		}
		case SETBASEOCTAVE_KEY :
		{
			Value diff_oct { (int) sds->FLAG };
			Notes->Set_base_octave( diff_oct.val ); // is positive, therefore identify 0 -> -1, 1 -> 1
			Sds->Commit();
			break;
		}
		case UPDATE_NLP_KEY : // Noteline_prefix
		{
			noteline_prefix_t nlp = sds->noteline_prefix;
			Comment(INFO, "receive command <update notesline prefix");
			Notes->Show_noteline_prefix( nlp );
			Notes->Noteline_prefix = nlp;
			Notes->Verify_noteline( nlp, Notes->Get_note_line() );
			Notes->Restart = true;
			Sds->Commit();
			break;
		}

		case SETNOTESPERSEC_KEY:
		{
			Value nps = sds->noteline_prefix.nps;
			Comment(INFO, "receive command <set notes per second>");
			if ( not Notes->Set_notes_per_second( nps.val ) )
			{
				Comment( ERROR, nps.str + " notes per second not supported");
			}
			Sds->Commit();
			break;
		}

		case SETWAVEDISPLAYKEY :
		{
			Wavedisplay->SetId( sds->Wavedisplay_Id );
			Sds->Commit();
			break;
		}
		case CONNECTFMOVCOKEY : // connect FMO volume with vco data
		{

			Instrument->fmo.Connect_fmo_data( &Instrument->vco );
			Instrument->main.Connect_fmo_data( &Instrument->fmo );
			Sds->Commit();
			break;
		}
		case RESETVCOKEY : // reset VCO
		{
			Instrument->vco.Reset_data( &Instrument->vco );
			Sds->Commit();
			break;
		}
		case RESETFMOKEY : // reset FMO
		{
			Instrument->fmo.Reset_data( &Instrument->fmo );
			Sds->Commit();
			break;
		}
		case CONNECTVCOFMOKEY ://connect VCO frequency with FMO data
		{
			Instrument->vco.Connect_vco_data( &Instrument->fmo );
			Instrument->main.Connect_vco_data( &Instrument->vco );
			Sds->Commit();
			break;
		}
		case SETWAVEFORMFMOKEY :
		{
			set_waveform( &Instrument->fmo, sds->FMO_spectrum.id );

			Sds->Commit();
			break;
			}
		case SETWAVEFORMVCOKEY :
		{
			set_waveform( &Instrument->vco, sds->VCO_spectrum.id );
			Sds->Commit();
			break;
		}
		case SETWAVEFORMMAINKEY :
		{
			set_waveform( &Instrument->main, sds->MAIN_spectrum.id );
			Sds->Commit();
			break;
		}

		case UPDATESPECTRUM_KEY :
		{
			Instrument->Update_spectrum();
			Sds->Commit();
			break;
		}
		case SAVEINSTRUMENTKEY :
		{
			Comment(INFO, "saving current config to instrument " + Instrument->Name );
			Instrument->Save_Instrument( Instrument->Name );
			Sds->Commit();
			break;
		}

		case NEWINSTRUMENTKEY : // save instrument file
		{
			string instrument = Sds->Read_str( INSTRUMENTSTR_KEY );
			Comment(INFO, "receiving instrument change to " + instrument );
			Instrument->Save_Instrument( instrument );
			Sds->Update( NEWINSTRUMENTFLAG );
			break;
		}

		default :
		{
			Exception( "Communication Key Id >" + to_string((int)key) +
					"< undefined" );
		 }

	} // switch char
	Mixer->Update_ifd_status_flags( sds );

}


