/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * eventhandler.cpp
 *
 *  Created on: Oct 28, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Synth/Synthesizer.h>


void Event_class::TestHandler()
{
	TEST_START( className );
	Sds->Eventque.reset();

	Sds->Eventque.add( CONNECTOSC_KEY );
	Handler();

	StAId_e staid = STA_INSTRUMENT;
	Sds->Set( Sds->addr->StA_Id , staid );

	sds->StA_state_arr[staid].play = false;
	Sds->Eventque.add( SETSTA_KEY );
	Handler();
	Assert_equal( Mixer->state.instrument, false );
	Assert_equal( Mixer->StA[STA_INSTRUMENT].state.Play(), false );

	sds->StA_state_arr[staid].play = true;

	Sds->Eventque.add( SETSTA_KEY );
	Handler();
	Assert_equal( Mixer->state.instrument, true );
	Assert_equal( Mixer->StA[staid].state.Play(), true );

	TEST_END( className );
}



void Event_class::Handler()
{

	auto EvInfo = [ this ]( EVENTKEY_e key = NULLKEY, string str )
	{
		if( this->EventQue->repeat ) return;
		if ( LogMask[ DEBUG ] )
		{
			string eventstr = Comment(DEBUG, key, "<-", str );
			this->DaTA->EmitEvent( UPDATELOG_EVENT, eventstr );
		}
	};

	string		str 	= EventQue->show();
	EVENTKEY_e 	event 	= EventQue->get();

	if ( event == NULLKEY ) return;

	EvInfo( event, str );

	switch ( event )
	{
	case UPDATESPECTRUM_KEY:
	{
		EvInfo( event, "Spectrum update");
		char 		oscid 	= sds->SpectrumTypeId;
		Instrument->Update_osc_spectrum( oscid );
		Sds->Commit();
		break;
	}
	case OSCFREQUENCYKEY:
	{
		EvInfo( event, "Slider Mode: " + slidermodes[sds->frq_slidermode] );
		if ( sds->frq_slidermode == COMBINE )
		{
			Instrument->Oscgroup.Set_Combine_Frequency( sds,
														sds->spectrum_arr[OSCID].frqidx[0],
														COMBINE );
			sds->spectrum_arr[VCOID].frqidx[0] = Instrument->vco->spectrum.frqidx[0];
			sds->spectrum_arr[FMOID].frqidx[0] = Instrument->fmo->spectrum.frqidx[0];
		}
		else
		{
			Instrument->osc->Set_frequency( sds->spectrum_arr[OSCID].frqidx[0], sds->frq_slidermode );
		}
		sds->spectrum_arr[OSCID].frqidx[0] = Instrument->osc->spectrum.frqidx[0];
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
		Instrument->vco->Set_spectrum_volume(vol.ch );

		Instrument->Connect( Instrument->osc, Instrument->vco, CONV);
		Sds->Commit();
		break;
	}
	case FMOAMPKEY : // modify the FMO volume
	{
		EvInfo( event, "FMO amplitude change");

		Value vol = sds->spectrum_arr[FMOID].volidx[0];
		Instrument->fmo->Set_spectrum_volume(vol.ch );
		Instrument->Connect( Instrument->osc, Instrument->fmo, CONF );
		Sds->Commit();
		break;
	}
	case MASTERAMP_KEY : // modify main volume
	{
		EvInfo( event, "Audio volume change");

		Mixer->state.mute = false;
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
		Mixer->state.mute = not sds->mixer_state.mute;
		string str = (Mixer->state.mute) ? "Mute" : "UnMute";
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
		if( not Mixer->state.instrument )
			Instrument->Oscgroup.Adsr_OSC();
		Sds->Commit();
		break;
	}
	case ADSR_KEY:
	{

		EvInfo( event, "ADSR change");
		OSCID_e oscid = sds->SpectrumTypeId;
		Instrument->Set_adsr( oscid );
		if( not Mixer->state.instrument )
			Instrument->Oscgroup.member[oscid]->Adsr_OSC();
		Sds->Commit();
		break;
	}
	case PWMDIALKEY:
	{
		EvInfo( event, "PWM change");

		Instrument->osc->Set_pmw(sds->features[VCOID].PWM);
		Instrument->vco->Set_pmw(sds->features[VCOID].PWM);
		Instrument->fmo->Set_pmw(sds->features[VCOID].PWM);
		Sds->Commit();
		break;
	}
	case SETWAVEDISPLAYKEY:
	{
		WD_data_t	wd_state 	= sds->WD_state;
		if( wd_state.roleId == AUDIOROLE )
		{
			Sds->Commit();
			break; // audio data is provided by the Audio server
		}
		if( wd_state.roleId == ADSRROLE )
		{
			Instrument->Oscgroup.member[wd_state.oscId]->Adsr_OSC();
		}
		Wavedisplay->Set_WdRole( wd_state.roleId );
		Sds->Commit();
		break;
	}
	case SOFTFREQUENCYKEY:
	{
		EvInfo( event, "Frequency slide effect update ");
		uint8_t	slide = sds->features[OSCID].slide_frq;
		Instrument->Oscgroup.SetSlideFrq( slide );
		sds->features[FMOID].slide_frq = slide;
		sds->features[VCOID].slide_frq = slide;
		Sds->Commit();
		break;
	}
	case STARTRECORD_KEY : // TODO complete functionality
	{
    	if( sds->StA_state_arr[STA_NOTES].play )
    	{
    		sds->Record_state = sdsstate_struct::STARTING;
    		Notes->Note_itr_start.SetActive( true );
    		Notes->Note_itr_end.SetActive( true );
    	}
    	else // no synchronization
    	{
//    		Notes->Note_itr_start.set_active( false );
//    		Notes->Note_itr_end.set_active( false );
    		sds->Record_state = sdsstate_struct::RECORDSTART;
    	}
		Sds->Commit();
		break;
	}
	case STOPRECORD_KEY :
	{
    	if( sds->StA_state_arr[STA_NOTES].play )
    	{
    		sds->Record_state = sdsstate_struct::STOPPING;
    		Notes->Note_itr_start.SetActive( false );
    		Notes->Note_itr_end.SetActive( true );
    	}
    	else // no synchronization
    	{
    		Notes->Note_itr_start.SetActive( false );
    		Notes->Note_itr_end.SetActive( false );
    		sds->Record_state = sdsstate_struct::RECORDSTOP;
    	}
		Sds->Commit();
		break;
	}
	case KBD_SAVE_KEY :
	{
		Keyboard->Save_notes();
		Sds->Commit();
		break;
	}
	case KBD_EVENT_KEY :
	{
		Keyboard->Set_kbdstate( );

		Sds->Commit();
		break;
	}
	case SAVE_EXTERNALWAVFILEKEY: // record and save wav file
	{
		if ( sds_master->Record_state == sdsstate_struct::RECORDING )
			// Composer - Interpreter
    		sds->Record_state = sdsstate_struct::RECORDSTOP;
		else
    		sds->Record_state = sdsstate_struct::RECORDSTART;

		Sds->Commit();
		break;
	}
	case READ_EXTERNAL_WAVFILE:
	{
		Comment(INFO, "receive command <set external wave file>");
		string wav_file = Sds->Read_str(WAVFILESTR_KEY);
		if (External->Read_file_header(wav_file))
		{
			External->Read_file_data();
			sds->StA_state_arr[STA_EXTERNAL ].play	= true;
			Mixer->SetStAProperties( STA_EXTERNAL );
			ProgressBar->SetValue( 100 * External->Filedata_size / External->mem_ds.bytes);
		}
		else
		{
			Comment(ERROR, "Failed to setup header");
		}
		Sds->Commit();
		break;
	}
	case STARECORD_STOP_KEY: // stop record on data array id
	{
		int id = sds->StA_Id ;
		Info( "receive command <stop record on storage area", id, ">" );
		sds->StA_state_arr[id].store = false;
		Mixer->StA[id].beattrigger.local_data.active = true;
		ProgressBar->Unset();
		Sds->Commit();
		break;
	}
	case STARECORD_START_KEY: //start record
	{
		StAId_e id = sds->StA_Id;
		Info( "receive command <start record on storage area", (int)id, ">" );
		for (StAId_e StaId : StAIds)
		{
			if ( id == StaId)
			{
				sds->StA_state_arr[StaId].store = true;
				Mixer->StA[id].beattrigger.local_data.active = true;
				ProgressBar->Set(	Mixer->StA[StaId].Store_counter(),
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
	case RESET_STA_SCANNER_KEY : // Audioserver start record
	{
		Info( "Reset StA scanner");
		for( StAId_e staid : StAIds )
		{
			Mixer->StA[staid].scanner.Set_rpos(0);
		}
		Sds->Commit();
		break;
	}
	case STA_VOLUME_KEY :
	{
		StAId_e staid = sds->StA_Id;
		Mixer->StA[staid].Volume( sds->StA_amp_arr[staid], sds->vol_slidemode );
		Sds->Commit();
		break;
	}
	case SETSTA_KEY:
	{
		StAId_e staid = sds->StA_Id;
		Mixer->SetStAProperties( staid );

		Sds->Commit();
		break;
	}
	case MUTEREC_KEY:
	{
		Value id = { (int) (sds->StA_Id) };
		Comment(INFO,
				"receive command <mute and stop record on id" + id.str
						+ ">");
		Mixer->StA[id.val].state.Play(false); // pause-play, pause-record
		Sds->Commit();
		break;
	}
	case MUTEMBKEY: // clear all memory bank flag
	{
		Comment(INFO,
				"receive command <mute and stop record on all memory banks>");
		for (StAId_e id : StAIds)
		{
			sds->StA_state_arr[id].play = false;
			//			Mixer->Set_play_mode(id, false);
		}
		Mixer->SetStAs();
		Sds->Commit();
		break;
	}
	case CLEAR_KEY: // all or mix id
	{
		StAId_e	staid = sds->StA_Id;
		Mixer->ResetStA( staid );
		ProgressBar->Reset(); // RecCounter
		Sds->Commit();
		break;
	}
	case UPDATENOTESKEY: // update notes
	{
		string name = Sds->Read_str(NOTESSTR_KEY);
		Comment(INFO, "receive command <update notes>", name );
		if ( sds->NotesTypeId == XML_ID )
		{
			EvInfo( event, "receive command <play xml notes> " + name);
			sds->StA_state_arr[ STA_NOTES ].play = true;
			Mixer->SetStAProperties( STA_NOTES );
			Sem->Release( SEMAPHORE_INITNOTES ); //other
		}
		else
		{
			Notes->Read(name);
			sds->StA_state_arr[STA_NOTES].play = true;
			Mixer->SetStAProperties( STA_NOTES );
			Notes->Generate_cyclic_data();
			DaTA->EmitEvent( NEWNOTESLINEFLAG ); // ->UpdateFileDialog
		}
		Sds->Commit();
		break;
	}
	case PLAYNOTESREC_ON_KEY: // play modnt.composer = true;
	{
		int sec { sds->Noteline_sec };
		int id { sds->StA_Id };
		if (sec > 0) {
			Comment(INFO, "generate composer notes");
			Comment(INFO, "duration:", sec, "sec.");
			Comment(INFO, "store to StA id:", id );
			Notes->Generate_cyclic_data();
		} else {
			Comment(WARN, "nothing to do for", sec, "Notes!");
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
		uint8_t	amp 						= sds->StA_amp_arr[STA_NOTES] ;
		sds->StA_state_arr[STA_NOTES].play	= true;
		sds->vol_slidemode					= FIXED;
		Info( "receive command < notes on", (int) amp, "%>" );
		Mixer->StA[STA_NOTES].Volume( amp, FIXED );
		Mixer->SetStAProperties( STA_NOTES );
		Sem->Release(SEMAPHORE_SYNCNOTES);
		Sds->Commit();
		break;
	}
	case NOTESOFFKEY:
	{
		Comment(INFO, "receive command < notes off>");
		sds->StA_state_arr[STA_NOTES].play = false;
		Mixer->SetStAProperties( STA_NOTES );
//		Mixer->Set_play_mode(STA_NOTES, false);
		Sds->Commit();
		break;
	}

	case SETBASEOCTAVE_KEY:
	{
		noteline_prefix_t nlp 	= Notes->Set_base_octave((uint) (sds->FLAG));
		// sds->FLAG is positive, therefore identify 0 -> -1, 1 -> 1
		sds->noteline_prefix	= nlp;

		Sds->Commit();
		break;
	}
	case UPDATE_NLP_KEY: // Noteline_prefix
	{
		noteline_prefix_t nlp = sds->noteline_prefix;
		Comment(INFO, "receive command <update notesline prefix");
		Notes->Set_noteline_prefix(nlp);
		Sds->Commit();
		break;
	}
	case SETNOTESPERSEC_KEY:
	{
		Value nps = sds->noteline_prefix.nps;
		Comment(INFO, "receive command <set notes per second>");
		if ( not Notes->Set_notes_per_second(nps.val))
		{
			Comment(ERROR, nps.str + " notes per second not supported");
		}
		Sds->Commit();
		break;
	}
	case CONNECTOSC_KEY :
	{
		EvInfo( event, "update modulation connections for the OSC");
		Instrument->Oscgroup.Set_Connections( sds );
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
	case SETINSTRUMENTKEY: // Set instrument
	{
		string Name = Sds->Read_str(INSTRUMENTSTR_KEY);
		Comment(INFO, "receive command <set instrument>", Name );
		Instrument->Set(Name);
		DaTA->EmitEvent( NEWINSTRUMENTFLAG, Name );
		Sds->Commit();
		break;
	}
	case SAVEINSTRUMENTKEY:
	{
		string Name = Sds->Read_str( INSTRUMENTSTR_KEY );
		Comment(INFO, "receive command <save instrument>", Name);
		Instrument->Save( Name );
		DaTA->EmitEvent( NEWINSTRUMENTFLAG, Name );
		Sds->Commit();
		break;
	}
	case CUT_SETUP_KEY	:
	{
		CutDesk->Setup();
		DaTA->EmitEvent( CUT_UPDATE_DISPLAY_FLAG, "update cutdesk" );
		Sds->Commit();
		break;
	}
	case CUT_UPDATE_KEY :
	{
		CutDesk->CursorUpdate();
		DaTA->EmitEvent( CUT_UPDATE_DISPLAY_FLAG, "update cutdesk" );
		Sds->Commit();
		break;
	}
	case CUT_KEY :
	{
		CutDesk->Cut();
		Sds->Commit();
		break;
	}
	case CUT_SAVE :
	{
		StAExternal.Convert_StA2WAV( CutDesk->StAId, CutDesk->restore_range );
		DaTA->EmitEvent( RECORDWAVFILEFLAG, "update wav filelist" );

		Sds->Commit();
		break;
	}
	case CUT_RESTORE_KEY	:
	{
		CutDesk->Restore();
		Sds->Commit();
		break;
	}
	default:
	{
		Exception( "Communication Key Id >" + to_string((int) (event ))	+ "< undefined");
	}
	} // switch event

}
