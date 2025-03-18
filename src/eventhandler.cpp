/*
 * synthesizercore.cpp
 *
 *  Created on: Oct 28, 2024
 *      Author: sirius
 */


#include <Synthesizer.h>



void Event_class::Handler()
{
	string str = Eventque.show();
	uint8_t event = Eventque.get();

	if ( event == NULLKEY ) return;
	Comment( DEBUG, str );
	switch ( event )
	{


	case XMLFILE_KEY :
	{
		if ( sds->NotestypeId == NTE_ID ) break;
		Sem->Release( SEMAPHORE_INITNOTES ); //other
		Comment(INFO, "receive command <setup play xml notes>");
		Sds->Commit();
		break;
	}
	case UPDATESPECTRUM_KEY:
	{
		Instrument->Update_spectrum();
		Sds->Commit();
		break;
	}
	case OSCFREQUENCYKEY:
	{
		Info( "Slider Mode:" , slidermodes[sds->slidermode] );
		if ( sds->slidermode == COMBINE )
		{
			Instrument->Oscgroup.Set_Frequency( sds->OSC_wp.frqidx, sds->slidermode );
			sds->VCO_spectrum = Instrument->vco->spectrum;
			sds->VCO_wp.frqidx= Instrument->vco->wp.frqidx;
			sds->FMO_spectrum = Instrument->fmo->spectrum;
			sds->FMO_wp.frqidx= Instrument->fmo->wp.frqidx;
		}
		else
		{
			Instrument->osc->Set_frequency( sds->OSC_wp.frqidx, sds->slidermode );
		}
		sds->OSC_spectrum = Instrument->osc->spectrum;

		Sds->Commit();
		break;
	}
	case VCOFREQUENCYKEY: // modify the secondary oscillator
	{
		uint8_t frqidx = Instrument->vco->Set_frequency( sds->VCO_wp.frqidx, sds->slidermode );
//		sds->VCO_spectrum.base = Instrument->osc->spectrum.base;//Calc( frqidx );
		sds->VCO_spectrum.frqidx[0] = frqidx;

		Instrument->osc->Connect_vco_data(Instrument->vco);

		Sds->Commit();
		break;
	}
	case FMOFREQUENCYKEY: // modify the fm_track data
	{
		uint8_t frqidx = Instrument->fmo->Set_frequency( sds->FMO_wp.frqidx, sds->slidermode );
//		sds->FMO_spectrum.base = Instrument->osc->spectrum.base;//Calc( frqidx );
		sds->FMO_spectrum.frqidx[0] = frqidx;


		Instrument->osc->Connect_fmo_data(Instrument->fmo);

		Sds->Commit();
		break;
	}
	case VCOAMPKEY: // modify the VCO volume
	{
		Value vol = sds->VCO_wp.volume;
		Instrument->vco->Set_volume(vol.ch, sds->slidermode);
		Instrument->osc->Connect_vco_data(Instrument->vco);
		sds->VCO_spectrum.vol[0] = vol.val * 0.01;
		sds->VCO_spectrum.volidx[0] = vol.val;

		Sds->Commit();
		break;
	}
	case FMOAMPKEY: // modify the FMO volume
	{
		Value vol = sds->FMO_wp.volume;
		Instrument->fmo->Set_volume(vol.ch, sds->slidermode);
		Instrument->osc->Connect_fmo_data(Instrument->fmo);
		sds->FMO_spectrum.vol[0] = vol.val * 0.01;
		sds->FMO_spectrum.volidx[0] = vol.val;

		Sds->Commit();
		break;
	}
	case MASTERAMP_KEY: // modify main volume
	{
		Mixer->status.mute = false;
		Mixer->DynVolume.Set( 	sds_master->Master_Amp,
								sds_master->vol_slidemode);
		Sds->Commit();
		break;
	}
	case ADJUST_KEY:
	{
		Instrument->vco->wp.adjust = sds->VCO_wp.adjust;
		Instrument->fmo->wp.adjust = sds->FMO_wp.adjust;
		Sds->Commit();
		break;
	}
	case MASTERAMP_LOOP_KEY:
	{
		Mixer->DynVolume.Set(	sds_master->Master_Amp,
							SLIDE);
		Sds->Commit();
		break;
	}
	case MASTERAMP_MUTE_KEY: // Mute Main Volume
	{
		Mixer->status.mute = not sds->mixer_status.mute;
		string str = (Mixer->status.mute) ? "Mute" : "UnMute";
		Comment(INFO, "receiving command <" + str + "> master volume>");
		Sds->Commit();
		break;
	}
	case ADSR_KEY:
	{
		Instrument->osc->Set_adsr(sds->OSC_adsr);
		Sds->Commit();
		break;
	}
	case PMWDIALKEY:
	{
		Instrument->osc->Set_pmw(sds->VCO_wp.PMW_dial);
		Instrument->vco->Set_pmw(sds->VCO_wp.PMW_dial);
		Instrument->fmo->Set_pmw(sds->VCO_wp.PMW_dial);
		Sds->Commit();
		break;
	}
	case SETWAVEDISPLAYKEY:
	{
		if( sds->WD_status.roleId == osc_struct::AUDIOID )
			break; // audio data is handled by the Audio server
		Wavedisplay->SetDataPtr(sds->WD_status );
		Sds->Commit();
		break;
	}

	case SOFTFREQUENCYKEY:
	{
		Instrument->osc->Set_glide(sds->OSC_wp.glide_effect);
		Sds->Commit();
		break;
	}
	case SETINSTRUMENTKEY: // Set instrument
	{
		Comment(INFO, "receive command <set instrument>");
		string instrument = Sds->Read_str(INSTRUMENTSTR_KEY); // other

		if (Instrument->Set(instrument))
		{
			Comment(INFO, "sucessfully loaded instrument " + instrument);
			Notes->Set_instrument(Instrument);
		} else
		{
			Comment(ERROR, "cannot load instrument" + instrument);
		}
		Sds->Commit(); // reset flags on GUI side
		break;
	}
	case SAVE_EXTERNALWAVFILEKEY: // record and save wav file
	{
		if (sds_master->Record)
			// Composer - Interpreter
			sds_master->AudioServer = RECORDSTART; // start and  wait
		else
			sds_master->AudioServer = RECORDSTOP; // stop and save

		Sds->Commit();
		break;
	}
	case READ_EXTERNALWAVEFILE: {
		Comment(INFO, "receive command <set external wave file>");
		string wavefile = Sds->Read_str(WAVEFILESTR_KEY);
		Sem->Lock(SEMAPHORE_TEST, 1); // assume record thread is working on that file
		if (External->Read_file_header(wavefile)) {
			External->Read_file_data();
			Mixer->StA[MbIdExternal].Play_mode(true);
			Mixer->StA[MbIdExternal].Volume.Set( 100, FIXED );
			Mixer->status.external = true;
			ProgressBar->SetValue(
					100 * External->Filedata_size / External->ds.mem_bytes);
		} else {
			Comment(ERROR, "Failed to setup header");
		}
		Sds->Commit();
		Sds->addr->UserInterface = UPDATEGUI; //set cb_sta play flag for external
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
		for (int StaId : Mixer->MemIds)
		{
			if (MbNr.val == StaId)
			{
				string status = 	Mixer->StA[StaId].Record_mode(true); // start record-stop play
				ProgressBar->Set(	Mixer->StA[StaId].Get_storeCounter_p(),
									Mixer->StA[StaId].ds.max_records);
			}
			else // only one mb shall store data
			{
				Mixer->StA[StaId].Record_mode(false);
			}
		}
		Sds->Commit();
		break;
	}
	case EXTERNAL_AMPLOOP_KEY: // TODO not working
	{
		uint8_t 	Id 		= sds->MIX_Id;
		Mixer->StA[Id].Volume.Set( sds->StA_amp_arr[Id], SLIDE);
		Sds->Commit();
		break;
	}
	case SETMBAMPPLAYKEY: // 109 change volume and play data array
	{
		Value mixid { sds->MIX_Id };
		Value amp { sds->StA_amp_arr[mixid.val] };
		Value play { sds->StA_state[mixid.val].play };
		Mixer->StA[mixid.val].Volume.Set( amp.val, SLIDE);
		Mixer->Set_mixer_state(mixid.val, (bool) (play.val));
		Comment(INFO,
				"Mixer ID " + mixid.str + " Amp: " + amp.str + " State: "
						+ play.boolstr);
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
		for (uint id : Mixer->RecIds) {
			Mixer->Set_mixer_state(id, false);
		}
		Sds->Commit();
		break;
	}
	case CLEAR_KEY: {
		uint8_t id = sds->MIX_Id;
		Comment(INFO, "Clear StA: " + to_string(id));
		Mixer->StA[id].Reset_counter();
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
	case RESETMAINKEY: // reset main
	{
		Instrument->osc->Mem_vco.Clear_data(max_data_amp);
		Instrument->osc->Mem_fmo.Clear_data(0);
		Instrument->osc->Reset_data();
		Sds->Commit();
		break;
	}
	case UPDATENOTESKEY: // update notes
	{
		Comment(INFO, "receive command <update notes>");
		if ( sds->NotestypeId == XML_ID ) break;
		string notes_name = Sds->Read_str(NOTESSTR_KEY);
		Notes->Set_instrument(Instrument);
		Notes->Read(notes_name);
		sds->Noteline_sec = Notes->noteline_sec;
		Sem->Release(SEMAPHORE_SYNCNOTES);
		Sds->Commit();
		break;
	}
	case NEWNOTESLINEKEY: // setup play or reset play notes
	{
		Comment(INFO, "receive command <setup play notes>");
		string notes_file = Sds->Read_str(NOTESSTR_KEY);
		Notes->Read(notes_file); // notes have been written to file by the GUI already
		Mixer->status.notes = true;
		Sds->Update(NEWNOTESLINEFLAG);
		sds->Noteline_sec = Notes->noteline_sec;
		Sem->Release(SEMAPHORE_SYNCNOTES);
		//			Notes->Start_note_itr();
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
			Mixer->composer = sec.val;
			Mixer->StA[id.val].Record_mode(true);
			Notes->Set_instrument(Instrument);
			Mixer->Store_noteline(id.val, Notes);
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
	case NOTESONKEY: {
		Value amp { (int) (sds->StA_amp_arr[MbIdNotes]) };
		Comment(INFO, "receive command < notes on " + amp.str + "%>");
		Mixer->StA[MbIdNotes].Volume.Set( amp.val, FIXED );
		Mixer->Set_mixer_state(MbIdNotes, true);
		Sem->Release(SEMAPHORE_SYNCNOTES);
		//			Notes->Start_note_itr();
		Sds->Commit();
		break;
	}
	case NOTESOFFKEY: {
		Comment(INFO, "receive command < notes off>");
		Mixer->Set_mixer_state(MbIdNotes, false);
		Sds->Commit();
		break;
	}
	case UPDATENOTELINEKEY: // update Noteline during play
	{
		Comment(INFO, "receive command <update Noteline during play>");
		string notes_file = Sds->Read_str(NOTESSTR_KEY);
		Notes->Read(notes_file);
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
	case CONNECTFMOVCOKEY: // connect FMO volume with vco data
	{
		Instrument->fmo->Connect_fmo_data(Instrument->vco);
		Instrument->osc->Connect_fmo_data(Instrument->fmo);
		Sds->Commit();
		break;
	}
	case RESETVCOKEY: // reset VCO
	{
		Instrument->vco->Reset_data();
		Sds->Commit();
		break;
	}
	case RESETFMOKEY: // reset FMO
	{
		Instrument->fmo->Reset_data();
		Sds->Commit();
		break;
	}
	case CONNECTVCOFMOKEY: //connect VCO frequency with FMO data
	{
		Instrument->vco->Connect_vco_data(Instrument->fmo);
		Instrument->osc->Connect_vco_data(Instrument->vco);
		Sds->Commit();
		break;
	}
	case SETWAVEFORMFMOKEY: {
		Instrument->fmo->Set_waveform( sds->FMO_spectrum.wfid);
		Sds->Commit();
		break;
	}
	case SETWAVEFORMVCOKEY: {
		Instrument->vco->Set_waveform( sds->VCO_spectrum.wfid);
		Sds->Commit();
		break;
	}
	case SETWAVEFORMMAINKEY:
	{
		Instrument->osc->Set_waveform( sds->OSC_spectrum.wfid);
		Sds->Commit();
		break;
	}
	case SAVEINSTRUMENTKEY:
	{
		Comment(INFO,
				"saving current config to instrument " + Instrument->Name);
		Instrument->Save_Instrument(Instrument->Name);
		Sds->Update(NEWINSTRUMENTFLAG);
		break;
	}
	case NEWINSTRUMENTKEY: // save instrument file
	{
		string instrument = Sds->Read_str(INSTRUMENTSTR_KEY);
		Comment(INFO, "receiving instrument change to " + instrument);
		Instrument->Save_Instrument(instrument);
		Sds->Update(NEWINSTRUMENTFLAG);
		break;
	}
	default: {
		EXCEPTION(
				"Communication Key Id >" + to_string((int) (event ))
						+ "< undefined");
	}
	} // switch char
	Mixer->Update_ifd_status_flags(sds);
}
