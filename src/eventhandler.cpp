/*
 * eventhandler.cpp
 *
 *  Created on: Oct 28, 2024
 *      Author: sirius
 */

#include <Synthesizer.h>

void Event_class::TestHandler()
{
	TEST_START( className );
	Sds->Event( CONNECTFMO_KEY );
	TEST_END( className );
}



void Event_class::Handler()
{

	auto EvInfo = [ this ]( uint key = 0, string str )
	{
		if( this->EventQue->repeat ) return;
		if ( LogMask[ DEBUG ] )
		{
			string eventstr = Comment(DEBUG, key, "<-", str );
			this->DaTA->EmitEvent( UPDATELOG_EVENT, eventstr );
		}
	};

	string 	str 	= EventQue->show();
	uint8_t event 	= EventQue->get();

	if ( event == NULLKEY ) return;

	EvInfo( event, str );

	switch ( event )
	{
	case XMLFILE_KEY :
	{
		if ( sds->NotestypeId == NTE_ID )
			break;
		Sem->Release( SEMAPHORE_INITNOTES ); //other
		EvInfo( event, "receive command <setup play xml notes>");
		Mixer->Set_mixer_state( STA_NOTES, true );
		Sds->Commit();
		break;
	}
	case UPDATESPECTRUM_KEY:
	{
		EvInfo( event, "Spectrum update");
		char 		oscid 	= sds->Spectrum_type;
		Instrument->Update_osc_spectrum( oscid );
		Sds->Commit();
		break;
	}
	case OSCFREQUENCYKEY:
	{
		EvInfo( event, "Slider Mode: " + slidermodes[sds->frq_slidermode] );
		if ( sds->frq_slidermode == COMBINE )
		{
			Instrument->Oscgroup.Set_Combine_Frequency( sds->spectrum_arr[OSCID].frqidx[0], sds->frq_slidermode );
			sds->spectrum_arr[VCOID] = Instrument->vco->spectrum;
			sds->spectrum_arr[FMOID] = Instrument->fmo->spectrum;
		}
		else
		{
			Instrument->osc->Set_frequency( sds->spectrum_arr[OSCID].frqidx[0], sds->frq_slidermode );
		}
		sds->spectrum_arr[OSCID] = Instrument->osc->spectrum;
		Sds->Commit();
		break;
	}
	case VCOFREQUENCYKEY: // modify the secondary oscillator
	{
		EvInfo( event, "VCO frequency change");

		Instrument->vco->Set_frequency( sds->spectrum_arr[VCOID].frqidx[0], sds->frq_slidermode );
		Instrument->Connect( Instrument->osc, Instrument->vco, CONV );
		Sds->Commit();
		break;
	}
	case FMOFREQUENCYKEY : // modify the fm_track data
	{
		EvInfo( event, "FMO frequency change");

		Instrument->fmo->Set_frequency( sds->spectrum_arr[FMOID].frqidx[0], sds->frq_slidermode );
		Instrument->Connect( Instrument->osc, Instrument->fmo, CONF );
		Sds->Commit();
		break;
	}
	case VCOAMPKEY : // modify the VCO volume
	{
		EvInfo( event, "VCO amplitude change");

		Value vol = sds->spectrum_arr[VCOID].volidx[0];
		Instrument->vco->Set_volume(vol.ch, sds->vol_slidemode);

		Instrument->Connect( Instrument->osc, Instrument->vco, CONV);
		sds->spectrum_arr[VCOID].vol[0] = vol.val * 0.01;
//		sds->VCO_spectrum.volidx[0] = vol.val;
		Sds->Commit();
		break;
	}
	case FMOAMPKEY : // modify the FMO volume
	{
		EvInfo( event, "FMO amplitude change");

		Value vol = sds->spectrum_arr[FMOID].volidx[0];
		Instrument->fmo->Set_volume(vol.ch, sds->vol_slidemode );
		Instrument->Connect( Instrument->osc, Instrument->fmo, CONF );
		sds->spectrum_arr[FMOID].vol[0] = vol.val * 0.01;
//		sds->FMO_spectrum.volidx[0] = vol.val;
		Sds->Commit();
		break;
	}
	case MASTERAMP_KEY : // modify main volume
	{
		EvInfo( event, "Audio volume change");

		Mixer->status.mute = false;
		Mixer->DynVolume.SetupVol( sds_master->Master_Amp,
								sds_master->vol_slidemode);
		Sds->Commit();
		break;
	}

	case ADJUST_KEY :
	{
		Instrument->vco->features.adjust = sds->features[VCOID].adjust;
		Instrument->fmo->features.adjust = sds->features[FMOID].adjust;
		Sds->Commit();
		break;
	}
	case MASTERAMP_LOOP_KEY :
	{
		Mixer->DynVolume.SetupVol(	sds_master->Master_Amp,
							SLIDE);
		Sds->Commit();
		break;
	}
	case MASTERAMP_MUTE_KEY: // Mute Main Volume
	{
		Mixer->status.mute = not sds->mixer_status.mute;
		string str = (Mixer->status.mute) ? "Mute" : "UnMute";
		EvInfo( event, "receiving command <" + str + "> master volume>");
		Sds->Commit();
		break;
	}
	case FEATURE_KEY	:
	{

		EvInfo( event, "Feature change");
		Instrument->Oscgroup.SetFeatures( sds );

		Sds->Commit();
		break;
	}
	case ADSRALL_KEY :
	{
		EvInfo( event, "all ADSR change");
		Instrument->Oscgroup.SetAdsr( sds );

		Sds->Commit();
		break;
	}
	case ADSR_KEY:
	{
		EvInfo( event, "ADSR change");
		char oscid = sds->Spectrum_type;
		Instrument->Set_adsr( oscid );

		Sds->Commit();
		break;
	}
	case PWMDIALKEY:
	{
		EvInfo( event, "PWM change");

		Instrument->osc->Set_pmw(sds->features[VCOID].PMW_dial);
		Instrument->vco->Set_pmw(sds->features[VCOID].PMW_dial);
		Instrument->fmo->Set_pmw(sds->features[VCOID].PMW_dial);
		Sds->Commit();
		break;
	}
	case SETWAVEDISPLAYKEY:
	{
		if( sds->WD_status.roleId == AUDIOOUTID )
			break; // audio data is handled by the Audio server
		Wavedisplay->SetDataPtr(sds->WD_status );
		Wavedisplay->Write_wavedata();

		Sds->Commit();
		break;
	}

	case SOFTFREQUENCYKEY:
	{
		EvInfo( event, "Frequency slide effect update ");
		Instrument->Oscgroup.SetSlide( sds->features[OSCID].glide_effect );
		Sds->Commit();
		break;
	}
	case SETINSTRUMENTKEY: // Set instrument
	{
		Comment(INFO, "receive command <set instrument>");
		string instrument = Sds->Read_str(INSTRUMENTSTR_KEY);

		Instrument->Set(instrument);

		Sds->Commit();
		break;
	}
	case SAVE_EXTERNALWAVFILEKEY: // record and save wav file
	{
		if ( sds_master->Record )
			// Composer - Interpreter
			sds_master->AudioServer = RECORDSTART; // start and  wait
		else
			sds_master->AudioServer = RECORDSTOP; // stop and save

		Sds->Commit();
		break;
	}
	case READ_EXTERNAL_WAVFILE: {
		Comment(INFO, "receive command <set external wave file>");
		string wav_file = Sds->Read_str(WAVFILESTR_KEY);
//		Sem->Lock(SEMAPHORE_TEST, 1); // assume record thread is working on that file
		if (External->Read_file_header(wav_file))
		{
			External->Read_file_data();
			Mixer->StA[STA_EXTERNAL].Play_mode(true);
			Mixer->StA[STA_EXTERNAL].DynVolume.SetupVol( 100, FIXED );
			sds->StA_amp_arr[ STA_EXTERNAL ] = 100;
			Mixer->status.external = true;
			ProgressBar->SetValue( 100 * External->Filedata_size / External->mem_ds.bytes);
		}
		else
		{
			Comment(ERROR, "Failed to setup header");
		}
		Sds->Commit();
//		DaTA->EmitEvent( READ_EXTERNALWAVEFILE );

		break;
	}
	case STOPRECORD_KEY: // stop record on data array id
	{
		Value id { (int) (sds->MIX_Id) };
		Comment(INFO,
				"receive command <stop record on storage area " + id.str
						+ ">");
		Mixer->StA[id.val].Record_mode(false); // stop recording
		ProgressBar->Unset();
		Sds->Commit();
		break;
	}
	case STORESOUNDKEY: //start record
	{
		Value MbNr { (int) (sds->MIX_Id) };
		Comment(INFO,
				"receiving command <store sound to memory bank " + MbNr.str
						+ " >");
		for (int StaId : Mixer_base::StAMemIds)
		{
			if (MbNr.val == StaId)
			{
				string status = 	Mixer->StA[StaId].Record_mode(true); // start record-stop play
				ProgressBar->Set(	Mixer->StA[StaId].Get_storeCounter_p(),
									Mixer->StA[StaId].mem_ds.max_records);
			}
			else // only one mb shall store data
			{
				Mixer->StA[StaId].Record_mode(false);
			}
		}
		Sds->Commit();
		break;
	}
	case EXTERNAL_AMPLOOP_KEY:
	{
		uint8_t 	Id 		= sds->MIX_Id;
		Mixer->StA[Id].DynVolume.SetupVol( sds->StA_amp_arr[Id], SLIDE);
		Sds->Commit();
		break;
	}
	case RESET_STA_SCANNER_KEY : // Audioserver start record
	{
		Info( "Reset StA scanner");
		for( char staid = 0; staid < STA_SIZE; staid++ )
		{
			Mixer->StA[staid].scanner.Set_rpos(0);
		}
		Sds->Commit();
		break;
	}
	case SETSTA_KEY:
	{
		Mixer->SetStA();
		Sds->Commit();
		break;
	}
	case MUTEREC_KEY:
	{
		Value id = { (int) (sds->MIX_Id) };
		Comment(INFO,
				"receive command <mute and stop record on id" + id.str
						+ ">");
		Mixer->StA[id.val].Play_mode(false); // pause-play, pause-record
		Sds->Commit();
		break;
	}
	case MUTEMBKEY: // clear all memory bank flag
	{
		Comment(INFO,
				"receive command <mute and stop record on all memory banks>");
		for (uint id : Mixer_base::StAMemIds) {
			Mixer->Set_mixer_state(id, false);
		}
		Sds->Commit();
		break;
	}
	case CLEAR_KEY:
	{
		for( uint id : Mixer_base::StAMemIds )
		{
			Mixer->StA[id].Reset();
		}
		ProgressBar->Reset(); // RecCounter
		Sds->Commit();
		break;
	}
	case SETSTAPLAY_KEY: // toggle Memory bank status play
	{
		Value Id { (int) (sds->MIX_Id) };
		bool play { (bool) sds->StA_state[Id.val].play };
		sds->StA_state[Id.val].play = play;
		Comment(INFO,
				"receive command <toggle play on memory bank" + Id.str
						+ " >" + to_string(play));
		Mixer->Set_mixer_state(Id.val, play);
		Sds->Commit();
		break;
	}
	case UPDATENOTESKEY: // update notes
	{
		Comment(INFO, "receive command <update notes>");
		if ( sds->NotestypeId == XML_ID ) break;
		string notes_name = Sds->Read_str(NOTESSTR_KEY);
		Notes->Read(notes_name);
		Sem->Release(SEMAPHORE_SYNCNOTES);
		Mixer->Set_mixer_state( STA_NOTES, true );
		if( Mixer->StA[ STA_NOTES ].DynVolume.GetCurrent().future == 0 )
		{
			Mixer->StA[ STA_NOTES ].DynVolume.SetupVol( 75, SLIDE );
			sds->StA_amp_arr[ STA_NOTES ] = 75;
		}
		DaTA->EmitEvent( NEWNOTESLINEFLAG );
		Notes->Generate_cyclic_data();
		Sds->Commit();
		break;
	}
	case NEWNOTESLINEKEY: // setup play or reset play notes
	{
		Comment(INFO, "receive command <setup play notes>");
		string notes_file = Sds->Read_str(NOTESSTR_KEY);
		Notes->Read(notes_file); // notes have been written to file by the GUI already
//		Mixer->status.notes = true;
		DaTA->EmitEvent( NEWNOTESLINEFLAG );
		Notes->Generate_cyclic_data();
		Sem->Release(SEMAPHORE_SYNCNOTES);

		Sds->Commit();
		break;
	}
	case PLAYNOTESREC_ON_KEY: // play modnt.composer = true;
	{
		Value sec { sds->Noteline_sec };
		Value id { sds->MIX_Id };
		if (sec.val > 0) {
			Comment(INFO, "generate composer notes");
			Comment(INFO, "duration: " + sec.str + " sec.");
			Comment(INFO, "store to StA id: " + id.str);
//			Mixer->composer = sec.val;
//			Mixer->StA[id.val].Record_mode(true);
//			Notes->Set_instrument(Instrument);
			Notes->Generate_cyclic_data();
//			Mixer->Store_noteline(id.val, Notes);
		} else {
			Comment(WARN, "nothing to do for " + sec.str + " Notes!");
		}
		Sds->Commit();
		break;
	}
	case PLAYNOTESRECOFF_KEY: // play free
	{
		// functionality is defined in PLAYNOTESREC_ON_KEY
		Mixer->composer = 0;
		Sds->Commit();
		break;
	}
	case NOTESONKEY:
	{
		Value amp { (int) (sds->StA_amp_arr[STA_NOTES]) };
		Comment(INFO, "receive command < notes on " + amp.str + "%>");
		Mixer->StA[STA_NOTES].DynVolume.SetupVol( amp.val, FIXED );
		Mixer->Set_mixer_state(STA_NOTES, true);
		Sem->Release(SEMAPHORE_SYNCNOTES);
		Sds->Commit();
		break;
	}
	case NOTESOFFKEY:
	{
		Comment(INFO, "receive command < notes off>");
		Mixer->Set_mixer_state(STA_NOTES, false);
		Sds->Commit();
		break;
	}

	case SETBASEOCTAVE_KEY:
	{
		Value diff_oct { (int) (sds->FLAG) };
		Notes->Set_base_octave(diff_oct.val); // is positive, therefore identify 0 -> -1, 1 -> 1
		Sds->Commit();
		break;
	}
	case UPDATE_NLP_KEY: // Noteline_prefix
	{
		noteline_prefix_t nlp = sds->noteline_prefix;
		Comment(INFO, "receive command <update notesline prefix");
		Notes->Set_noteline_prefix(nlp);
		Notes->Restart = true;
		Sds->Commit();
		break;
	}
	case SETNOTESPERSEC_KEY: {
		Value nps = sds->noteline_prefix.nps;
		Comment(INFO, "receive command <set notes per second>");
		if (!Notes->Set_notes_per_second(nps.val)) {
			Comment(ERROR, nps.str + " notes per second not supported");
		}
		Sds->Commit();
		break;
	}
	case CONNECTOSC_KEY : // reset main
	{
		EvInfo( event, "update modulation connections for the OSC");
		if( sds->ui_connect_status[CON::OSCFMOF] )
			Instrument->Connect( Instrument->osc, Instrument->fmo, CONF );
		else
			Instrument->Connect( Instrument->osc, Instrument->osc, CONF );

		if( sds->ui_connect_status[CON::OSCVCOV] )
			Instrument->Connect( Instrument->osc, Instrument->vco, CONV );
		else
			Instrument->Connect( Instrument->osc, Instrument->osc, CONV );
//		Instrument->Oscgroup.Show_sound_stack();


		Sds->Commit();
		break;
	}
	case CONNECTVCO_KEY: //connect VCO volume with FMO data
	{
		EvInfo( event, "modulate VCO volume by FMO output" );

		if( sds->ui_connect_status[CON::VCOFMOV] )
			Instrument->Connect( Instrument->vco, Instrument->fmo, CONV );
		else
			Instrument->Connect( Instrument->vco, Instrument->vco, CONV );
//		Instrument->Oscgroup.Show_sound_stack();


		Sds->Commit();
		break;
	}
	case CONNECTFMO_KEY: // connect FMO volume with vco data
	{
		EvInfo( event, "modulate FMO volume by VCO output" );

		if( sds->ui_connect_status[CON::FMOVCOV] )
			Instrument->Connect( Instrument->fmo, Instrument->vco, CONV );
		else
			Instrument->Connect( Instrument->fmo, Instrument->fmo, CONV );
//		Instrument->Oscgroup.Show_sound_stack();

		Sds->Commit();
		break;
	}
	case SETWAVEFORMFMOKEY:
	{
		EvInfo( event, "FMO waveform " + to_string((int) sds->spectrum_arr[OSCID].wfid[0] ) );
		Instrument->fmo->Set_waveform( sds->spectrum_arr[FMOID].wfid);

		Sds->Commit();
		break;
	}
	case SETWAVEFORMVCOKEY:
	{
		EvInfo( event, "VCO waveform " + to_string((int) sds->spectrum_arr[OSCID].wfid[0] ) );
		Instrument->vco->Set_waveform( sds->spectrum_arr[VCOID].wfid);

		Sds->Commit();
		break;
	}
	case SETWAVEFORMMAINKEY:
	{
		EvInfo( event, "OSC waveform " + to_string((int) sds->spectrum_arr[OSCID].wfid[0] ) );
		Instrument->osc->Set_waveform( sds->spectrum_arr[OSCID].wfid);

		Sds->Commit();
		break;
	}
	case SAVEINSTRUMENTKEY:
	{
		string Name = Sds->Read_str( INSTRUMENTSTR_KEY );
		Comment(INFO,
				"saving current config to instrument " + Name);
		Instrument->Save_Instrument( Name );
		DaTA->EmitEvent( NEWINSTRUMENTFLAG, "Save " + Name );
		Sds->Commit();
		break;
	}
	case NEWINSTRUMENTKEY: // save instrument file
	{
		string instrument = Sds->Read_str(INSTRUMENTSTR_KEY);
		Comment(INFO, "receiving instrument change to " + instrument);
		Instrument->Save_Instrument(instrument);
		DaTA->EmitEvent( NEWINSTRUMENTFLAG, instrument );
		Sds->Commit();
		break;
	}
	default:
	{
		EXCEPTION( "Communication Key Id >" + to_string((int) (event ))	+ "< undefined");
	}
	} // switch event

	Mixer->Update_sds_state( ALLITEMS, sds);
}
