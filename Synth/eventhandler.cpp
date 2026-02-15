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

Event_class::Event_class(	Instrument_class* 	instrument,
							Note_class*			notes,
							Keyboard_class*		keyboard,
							Mixer_class*		mixer,
							Wavedisplay_class*	wavedisplay,
							Dataworld_class*	data,
							External_class*		external,
							ProgressBar_class*	progressbar,
							CutDesk_class*		cutter )
	: Logfacility_class	("Event_class")
	, sdsstate_struct	()
	, StAExternal		( &mixer->StA, data->Cfg_p, data->Sds_p->addr )
{
	this->DaTA			= data;
	this->Sds			= DaTA->Sds_p;
	this->Sds_master	= DaTA->Sds_master;
	this->sds_master	= DaTA->sds_master;
	this->sds_p 		= Sds->addr;
	this->Sem			= DaTA->Sem_p;
	this->EventQue		= &DaTA->Sds_p->Eventque;
	this->Instrument 	= instrument;
	this->Notes			= notes;
	this->Keyboard		= keyboard;
	this->Mixer			= mixer;
	this->Wavedisplay	= wavedisplay;
	this->External		= external;
	this->ProgressBar	= progressbar;
	this->Appstate		= &DaTA->Appstate;
	this->CutDesk		= cutter;
};


void Event_class::TestHandler()
{
	TEST_START( className );
	Sds->Eventque.reset();

	Sds->Eventque.add( CONNECTOSC_KEY );
	Handler();

	Assert_equal( Sds->addr, sds_p );
	StAId_e staid = STA_INSTRUMENT;
	Sds->Set( Sds->addr->StA_Id , staid );


	sds_p->StA_state_arr[staid].play = false;
	Sds->Eventque.add( SETSTA_KEY );
	Handler();
	Assert_equal( sds_p->StA_state_arr[staid].play,  false );
	Assert_equal( Mixer->state.instrument, false );
	Assert_equal( Mixer->StA[STA_INSTRUMENT].state.Play(), false );


	sds_p->StA_state_arr[staid].play = true;
	Sds->Eventque.add( SETSTA_KEY );
	Handler();
	Assert_equal( Mixer->state.instrument, true );
	Assert_equal( Mixer->StA[staid].state.Play(), true );

	TEST_END( className );
}


#define BREAK { Sds->Commit(); break; }
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
		char 		oscid 	= sds_p->SpectrumTypeId;
		Instrument->Update_osc_spectrum( oscid );
		BREAK
	}
	case OSCFREQUENCYKEY:
	{
		EvInfo( event, "Slider Mode: " + slidermodes[sds_p->frq_slidermode] );
		if ( sds_p->frq_slidermode == COMBINE )
		{
			Instrument->Oscgroup.Set_Combine_Frequency( sds_p,
														sds_p->spectrum_arr[OSCID].frqidx[0],
														COMBINE );
			sds_p->spectrum_arr[VCOID].frqidx[0] = Instrument->vco->spectrum.frqidx[0];
			sds_p->spectrum_arr[FMOID].frqidx[0] = Instrument->fmo->spectrum.frqidx[0];
		}
		else
		{
			Instrument->osc->Set_frequency( sds_p->spectrum_arr[OSCID].frqidx[0],
											sds_p->frq_slidermode );
		}
		sds_p->spectrum_arr[OSCID].frqidx[0] 	= Instrument->osc->spectrum.frqidx[0];
		sds_p->WD_state.oscId					= OSCID;
		BREAK
	}
	case VCOFREQUENCYKEY: // modify the secondary oscillator
	{
		EvInfo( event, "VCO frequency change");

		Instrument->vco->Set_frequency( sds_p->spectrum_arr[VCOID].frqidx[0], sds_p->frq_slidermode );
		Instrument->Connect( Instrument->osc, Instrument->vco, CONV );
		sds_p->WD_state.oscId=VCOID;
		BREAK
	}
	case FMOFREQUENCYKEY : // modify the fm_track data
	{
		EvInfo( event, "FMO frequency change");

		Instrument->fmo->Set_frequency( sds_p->spectrum_arr[FMOID].frqidx[0], sds_p->frq_slidermode );
		Instrument->Connect( Instrument->osc, Instrument->fmo, CONF );
		sds_p->WD_state.oscId=FMOID;

		BREAK
	}
	case VCOAMPKEY : // modify the VCO volume
	{
		EvInfo( event, "VCO amplitude change");

		Value vol = sds_p->spectrum_arr[VCOID].volidx[0];
		Instrument->vco->Set_spectrum_volume(vol.ch );

		Instrument->Connect( Instrument->osc, Instrument->vco, CONV);
		BREAK
	}
	case FMOAMPKEY : // modify the FMO volume
	{
		EvInfo( event, "FMO amplitude change");

		Value vol = sds_p->spectrum_arr[FMOID].volidx[0];
		Instrument->fmo->Set_spectrum_volume(vol.ch );
		Instrument->Connect( Instrument->osc, Instrument->fmo, CONF );
		BREAK
	}
	case MASTERAMP_KEY : // modify main volume
	{
		EvInfo( event, "Audio volume change");

		Mixer->state.mute = false;
		Mixer->DynVolume.SetupVol( sds_master->Master_Amp,
								sds_master->vol_slidemode);
		BREAK
	}

	case ADJUST_KEY :
	{
		Instrument->vco->features.adjust = sds_p->features[VCOID].adjust;
		Instrument->fmo->features.adjust = sds_p->features[FMOID].adjust;
		BREAK
	}
	case MASTERAMP_LOOP_KEY :
	{
		Mixer->DynVolume.SetupVol(	sds_master->Master_Amp,
							SLIDE);
		BREAK

	}
	case MASTERAMP_MUTE_KEY: // Mute Main Volume
	{
		bool mute = sds_p->mixer_state.mute;
		Mixer->state.mute = not mute;
		sds_p->mixer_state.mute = not mute;
		string str = ( Mixer->state.mute ) ? "Mute" : "UnMute";
		EvInfo( event, "receiving command <" + str + "> master volume>");
		BREAK
	}
	case FEATURE_KEY	:
	{
		EvInfo( event, "Feature change");
		Instrument->Oscgroup.SetFeatures( sds_p );
		BREAK
	}
	case BEATCLOCK_KEY	:
	{
		EvInfo( event, "Beatclock change");
		Instrument->osc->Set_bps( sds_p->instrumentClock );
		Instrument->vco->Set_bps( sds_p->instrumentClock );
		Instrument->fmo->Set_bps( sds_p->instrumentClock );
		BREAK
	}
	case ADSRALL_KEY :
	{
		EvInfo( event, "all ADSR change");
		Instrument->Oscgroup.SetAdsr( sds_p );
		if( not Mixer->state.instrument )
			Instrument->Oscgroup.Adsr_OSC();
		BREAK
	}
	case ADSR_KEY:
	{
		EvInfo( event, "ADSR change");
		OSCID_e oscid = sds_p->SpectrumTypeId;
		Instrument->Set_adsr( oscid );
		if( not Mixer->state.instrument )
			Instrument->Oscgroup.member[oscid]->Adsr_OSC();
		BREAK
	}
	case PWMDIALKEY:
	{
		EvInfo( event, "PWM change");

		Instrument->osc->Set_pmw(sds_p->features[VCOID].PWM);
		Instrument->vco->Set_pmw(sds_p->features[VCOID].PWM);
		Instrument->fmo->Set_pmw(sds_p->features[VCOID].PWM);
		BREAK
	}
	case SETWAVEDISPLAYKEY:
	{
		WD_data_t	wd_state 	= sds_p->WD_state;
		if( wd_state.roleId == AUDIOROLE )
		{
			BREAK
			 // audio data is provided by the Audio server
		}
		if( wd_state.roleId == ADSRROLE )
		{
			Instrument->Oscgroup.member[wd_state.oscId]->Adsr_OSC();
		}
		Wavedisplay->Set_WdData();
		BREAK
	}
	case SOFTFREQUENCYKEY:
	{
		EvInfo( event, "Frequency slide effect update ");
		uint8_t	slide = sds_p->features[OSCID].slide_frq;
		Instrument->Oscgroup.SetSlider_Frq( slide );
		sds_p->features[FMOID].slide_frq = slide;
		sds_p->features[VCOID].slide_frq = slide;
		BREAK
	}
	case STARTRECORD_KEY : // TODO complete functionality
	{
    	if( sds_p->StA_state_arr[STA_NOTES].play )
    	{
    		sds_p->Record_state = sdsstate_struct::STARTING;
    		Notes->Note_itr_start.SetActive( true );
    		Notes->Note_itr_end.SetActive( true );
    	}
    	else // no synchronization
    	{
//    		Notes->Note_itr_start.set_active( false );
//    		Notes->Note_itr_end.set_active( false );
    		sds_p->Record_state = sdsstate_struct::RECORDSTART;
    	}
		BREAK
	}
	case STOPRECORD_KEY :
	{
    	if( sds_p->StA_state_arr[STA_NOTES].play )
    	{
    		sds_p->Record_state = sdsstate_struct::STOPPING;
    		Notes->Note_itr_start.SetActive( false );
    		Notes->Note_itr_end.SetActive( true );
    	}
    	else // no synchronization
    	{
    		Notes->Note_itr_start.SetActive( false );
    		Notes->Note_itr_end.SetActive( false );
    		sds_p->Record_state = sdsstate_struct::RECORDSTOP;
    	}
		BREAK
	}
	case KBD_SAVE_KEY :
	{
		Keyboard->Save_notes();
		BREAK
	}
	case KBD_EVENT_KEY :
	{
		Keyboard->SetKbdKey( );

		BREAK
	}
	case SAVE_EXTERNALWAVFILEKEY: // record and save wav file
	{
		if ( sds_master->Record_state == sdsstate_struct::RECORDING )
			// Composer - Interpreter
    		sds_p->Record_state = sdsstate_struct::RECORDSTOP;
		else
    		sds_p->Record_state = sdsstate_struct::RECORDSTART;

		BREAK
	}
	case READ_EXTERNAL_WAVFILE:
	{
		Comment(INFO, "receive command <set external wave file>");
		string wav_file = Sds->Read_str(WAVFILESTR_KEY);
		if (External->Read_file_header(wav_file))
		{
			External->Read_file_data();
			sds_p->StA_state_arr[STA_EXTERNAL ].play	= true;
			Mixer->SetStAProperties( STA_EXTERNAL );
			ProgressBar->SetValue( 100 * External->Filedata_size / External->mem_ds.bytes);
			Wavedisplay->Set_WdData();
		}
		else
		{
			Comment(ERROR, "Failed to setup header");
		}
		BREAK
	}
	case STARECORD_STOP_KEY: // stop record on data array id
	{
		int id = sds_p->StA_Id ;
		if( not in_range( staid_range, (StAId_e)id ) )
			BREAK
		Info( "receive command <stop record on storage area", id, ">" );
		sds_p->StA_state_arr[id].store = false;
		ProgressBar->Unset();
		BREAK
	}
	case STARECORD_START_KEY: //start record
	{
		StAId_e id = sds_p->StA_Id;
		if( not in_range( staid_range, (StAId_e)id ) )
			BREAK
		Info( "receive command <start record on storage area", (int)id, ">" );
		for (StAId_e StaId : StAIds)
		{
			if ( id == StaId)
			{
				sds_p->StA_state_arr[id].store = true;
				ProgressBar->Set(	Mixer->StA[StaId].Store_counter(),
									Mixer->StA[StaId].mem_ds.max_records);
			}
			else // only one mb shall store data
			{
				Mixer->StA[StaId].Record_mode(false);
			}
		}
		BREAK
	}
	case RESET_STA_SCANNER_KEY : // Audioserver start record
	{
		Info( "Reset StA scanner");
		for( StAId_e staid : StAIds )
		{
			Mixer->StA[staid].scanner.Set_rpos(0);
		}
		BREAK
	}
	case STA_VOLUME_KEY :
	{
		StAId_e staid = sds_p->StA_Id;
		Mixer->StA[staid].Volume( sds_p->StA_amp_arr[staid], sds_p->vol_slidemode );
		BREAK
	}
	case SETSTA_KEY:
	{
		StAId_e staid = sds_p->StA_Id;
		Mixer->SetStAProperties( staid );

		BREAK
	}
	case MUTESTA_KEY:
	{
		StAId_e staid = sds_p->StA_Id;
		sds_p->StA_state_arr[staid].play = false;
		Info( "receive command <mute and stop record on id", (int)staid, ">");
		Mixer->SetStAProperties( staid );
		BREAK
	}
	case STAS_CLEARPLAY_KEY: // toggle StA mute flag and pause StA output
	{
		Info( "receive command <mute StAs>");
		for( StAId_e staid : StAIds )
		{
			sds_p->StA_state_arr[staid].play = false;
		}
		Mixer->SetStAs();
		BREAK
	}
	case CLEAR_KEY: // all or mix id
	{
		StAId_e	staid = sds_p->StA_Id;
		Mixer->ResetStA( staid );
		ProgressBar->Reset(); // RecCounter
		BREAK
	}
	case UPDATENOTESKEY: // update notes
	{
		string name = Sds->Read_str(NOTESSTR_KEY);
		Comment(INFO, "receive command <update notes>", name );
		if ( sds_p->NotesTypeId == XML_ID )
		{
			EvInfo( event, "receive command <play xml notes> " + name);
			sds_p->StA_state_arr[ STA_NOTES ].play = true;
			Mixer->SetStAProperties( STA_NOTES );
			Sem->Release( SEMAPHORE_INITNOTES ); //other
		}
		else
		{
			Notes->Read(name);
			sds_p->StA_state_arr[STA_NOTES].play = true;
			Mixer->SetStAProperties( STA_NOTES );
			Notes->Generate_cyclic_data();
			DaTA->EmitEvent( NEWNOTESLINEFLAG ); // ->UpdateFileDialog
		}
		BREAK
	}
	case PLAYNOTESREC_ON_KEY: // play modnt.composer = true;
	{
		int sec { sds_p->Noteline_sec };
		int id { sds_p->StA_Id };
		if (sec > 0) {
			Comment(INFO, "generate composer notes");
			Comment(INFO, "duration:", sec, "sec.");
			Comment(INFO, "store to StA id:", id );
			Notes->Generate_cyclic_data();
		} else {
			Comment(WARN, "nothing to do for", sec, "Notes!");
		}
		BREAK
	}
	case PLAYNOTESRECOFF_KEY: // play free
	{
		// functionality is defined in PLAYNOTESREC_ON_KEY
		Mixer->composer = 0;
		BREAK
	}
	case NOTESONKEY:
	{
		uint8_t	amp 							= sds_p->StA_amp_arr[STA_NOTES] ;
		sds_p->StA_state_arr[STA_NOTES].play	= true;
		sds_p->vol_slidemode					= FIXED;
		Mixer->StA[STA_NOTES].Volume			( amp, FIXED );
		Mixer->SetStAProperties					( STA_NOTES );
		Sem->Release							(SEMAPHORE_SYNCNOTES);
		Info( "receive command < notes on", (int) amp, "%>" );
		BREAK
	}
	case NOTESOFFKEY:
	{
		Comment(INFO, "receive command < notes off>");
		sds_p->StA_state_arr[STA_NOTES].play = false;
		Mixer->SetStAProperties( STA_NOTES );
		BREAK
	}

	case SETBASEOCTAVE_KEY:
	{
		noteline_prefix_t nlp 	= Notes->Set_base_octave((uint) (sds_p->FLAG));
		// sds->FLAG is positive, therefore identify 0 -> -1, 1 -> 1
		sds_p->noteline_prefix	= nlp;
		BREAK
	}
	case UPDATE_NLP_KEY: // Noteline_prefix
	{
		noteline_prefix_t nlp = sds_p->noteline_prefix;
		Comment(INFO, "receive command <update notesline prefix");
		Notes->Set_noteline_prefix(nlp);
		BREAK
	}
	case SETNOTESPERSEC_KEY:
	{
		Value nps = sds_p->noteline_prefix.nps;
		Comment(INFO, "receive command <set notes per second>");
		if ( not Notes->Set_notes_per_second(nps.val))
		{
			Comment(ERROR, nps.str + " notes per second not supported");
		}
		BREAK
	}
	case CONNECTOSC_KEY :
	{
		EvInfo( event, "update modulation connections for the OSC");
		Instrument->Oscgroup.Set_Connections( sds_p );
		BREAK
	}
	case SETWAVEFORMFMOKEY:
	{
		EvInfo( event, "FMO waveform " + to_string((int) sds_p->spectrum_arr[OSCID].wfid[0] ) );
		Instrument->fmo->Set_waveform( sds_p->spectrum_arr[FMOID].wfid);

		BREAK
	}
	case SETWAVEFORMVCOKEY:
	{
		EvInfo( event, "VCO waveform " + to_string((int) sds_p->spectrum_arr[OSCID].wfid[0] ) );
		Instrument->vco->Set_waveform( sds_p->spectrum_arr[VCOID].wfid);

		BREAK
	}
	case SETWAVEFORMMAINKEY:
	{
		EvInfo( event, "OSC waveform " + to_string((int) sds_p->spectrum_arr[OSCID].wfid[0] ) );
		Instrument->osc->Set_waveform( sds_p->spectrum_arr[OSCID].wfid);

		BREAK
	}
	case SETINSTRUMENTKEY: // Set instrument
	{
		string Name = Sds->Read_str(INSTRUMENTSTR_KEY);
		Comment(INFO, "receive command <set instrument>", Name );
		Instrument->Set(Name);
		DaTA->EmitEvent( NEWINSTRUMENTFLAG, Name );
		BREAK
	}
	case SAVEINSTRUMENTKEY:
	{
		string Name = Sds->Read_str( INSTRUMENTSTR_KEY );
		Comment(INFO, "receive command <save instrument>", Name);
		Instrument->Save( Name );
		DaTA->EmitEvent( NEWINSTRUMENTFLAG, Name );
		BREAK
	}
	case CUT_SETUP_KEY	:
	{
		CutDesk->Setup();
		DaTA->EmitEvent( CUT_UPDATE_DISPLAY_FLAG, "update cutdesk" );
		BREAK
	}
	case CUT_UPDATE_KEY :
	{
		CutDesk->CursorUpdate();
		DaTA->EmitEvent( CUT_UPDATE_DISPLAY_FLAG, "update cutdesk" );
		BREAK
	}
	case CUT_KEY :
	{
		CutDesk->Cut();
		BREAK
	}
	case CUT_SAVE :
	{
		StAExternal.Convert_StA2WAV( CutDesk->StAId, CutDesk->restore_range );
		DaTA->EmitEvent( RECORDWAVFILEFLAG, "update wav filelist" );

		BREAK
	}
	case CUT_RESTORE_KEY	:
	{
		CutDesk->Restore();
		BREAK
	}
	case RESETSDS_KEY		:
	{
		sds_p->StA_state_arr 		= default_StA_state_arr();
		sds_p->StA_amp_arr			= { 0, 0, 0, 0, 75, 0, 0, 0 };
		sds_p->mixer_state 			= mixer_state_struct();
		sds_p->vol_slidemode		= SLIDE;
		sds_p->frq_slidermode		= SLIDE;
		Instrument->Set				( "default" );

		BREAK
	}
	default:
	{
		Exception( "Communication Key Id >" + to_string((int) (event ))	+ "< undefined");
	}
	} // switch event

}
