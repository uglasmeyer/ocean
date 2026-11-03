/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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
							StorageArea_class*	sta,
							Mixer_class*		mixer,
							Wavedisplay_class*	wavedisplay,
							Dataworld_class*	data,
							External_class*		external,
							ProgressBar_class*	progressbar,
							CutDesk_class*		cutter,
							Capture_class*		capture )
	: Logfacility_class	("Event_class")
	, sdsstate_struct	()
	, StAExternal		( &sta->Array, data->Cfg_p, data->SDS.master )
	, eventbuffer		( &data->Sds_p->Eventque )
{
	this->StA			= sta;
	this->DaTA			= data;
	this->Sds			= DaTA->Sds_p;
	this->Sds_master	= DaTA->SDS.Master;
	this->sds_master	= DaTA->SDS.master;
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
	this->Capture		= capture;
	this->state			= ENABLED;
};


void Event_class::TestHandler()
{
	TEST_START( className );
	Sds->Eventque.Reset();

	Sds->Eventque.Add( CONNECTOSC_KEY );
	Handler();

	Assert_equal( Sds->addr, sds_p );
	StAId_e staid = STA_INSTRUMENT;
	Sds->Set( Sds->addr->StA_Id , staid );


	sds_p->StA_state_arr[staid].play = false;
	Sds->Eventque.Add( SETSTA_KEY );
	Handler();
	Assert_equal( sds_p->StA_state_arr[staid].play,  false );
	Assert_equal( StA->Array[staid].state.Play(), false );


	sds_p->StA_state_arr[staid].play = true;
	Sds->Eventque.Add( SETSTA_KEY );
	Handler();
	Assert_equal( StA->Array[staid].state.Play(), true );

	sds_p->StA_state_arr[staid].store = true;
	StA->Array[staid].state.Store(false);
	Sds->Eventque.Add( SETSTA_KEY );
	Handler();
	Assert_equal( StA->Array[staid].state.Store(), true );

	Instrument->Oscgroup.member[OSCID]->Set_frequency( A3, FIXED );
	sds_p->frq_slidermode = FIXED;
	sds_p->spectrum_arr[OSCID].frqidx[0] = 72;
	sds_p->spectrum_arr[VCOID].frqidx[0] = 71;
	sds_p->features[VCOID].combine = true;
	Sds->Eventque.Add( FREQUENCYKEY );
	Handler();
	Assert_equal( (int)sds_p->spectrum_arr[VCOID].frqidx[0], 72 );

	TEST_END( className );
}


#define BREAK { Sds->Commit(); break; }




void Event_class::State( StateId_e stateid )
{
	state = stateid;
}

void Event_class::Handler()
{	// object classes orchestration

	if( state == DISABLED )
		return;

	string		str 	= EventQue->Show();
	EVENTKEY_e 	event 	= EventQue->Get();

	if ( event == NULLKEY )
		return;

	EvInfo( event, str );

	switch ( event )
	{
	case COMMIT_KEY :
	{
		BREAK
	}
//	case UPDATESPECTRUM_KEY:
//	{
//		EvInfo( event, "Spectrum update");
//		OSCID_e oscid 	= sds_p->SpectrumOscId;
//		Instrument->Update_osc_spectrum( oscid );
//		BREAK
//	}
	case SHIFTFRQ_KEY :
	{
		Instrument->Oscgroup.ShiftSliderFrequency( sds_p );
		BREAK
	}
	case FREQUENCYKEY:
	{
		OSCID_e oscid = sds_p->SpectrumOscId;
		EvInfo( event, typeNames[oscid] + " frequency change");
		if( oscid == OSCID )
		{
			Instrument->Oscgroup.SetSliderFrequency( sds_p );
		}
		else
		{
			Oscillator* osc = Instrument->Oscgroup.member[oscid];
			osc->Set_frequency( sds_p->spectrum_arr[oscid].frqidx[0], sds_p->frq_slidermode );
			Instrument->Connect( &Instrument->Oscgroup.osc, osc, CONV );
		}
		sds_p->WD_state.oscId	= oscid;
		BREAK
	}
	case VCOAMPKEY : // modify the VCO volume
	{
		EvInfo( event, "VCO amplitude change");

		Value vol = sds_p->spectrum_arr[VCOID].volidx[0];
		Instrument->Oscgroup.vco.Set_volume(vol.ch );
		Instrument->Connect( &Instrument->Oscgroup.osc, &Instrument->Oscgroup.vco, CONV);
		BREAK
	}
	case FMOAMPKEY : // modify the FMO volume
	{
		EvInfo( event, "FMO amplitude change");

		Value vol = sds_p->spectrum_arr[FMOID].volidx[0];
		Instrument->Oscgroup.fmo.Set_volume(vol.ch );
		Instrument->Connect( &Instrument->Oscgroup.osc, &Instrument->Oscgroup.fmo, CONF );
		BREAK
	}
	case MASTERAMP_KEY : // modify main volume
	{
		EvInfo( event, "Audio volume change");

		Mixer->state.mute		= false;
		BREAK
	}
	case MASTERAMP_MUTE_KEY: // Mute Main Volume
	{
		bool	mute 			= sds_p->mixer_state.mute;
		Mixer->state.mute		= not mute;
		sds_p->mixer_state.mute = not mute;
		string	str 			= ( Mixer->state.mute ) ? "Mute" : "UnMute";
		EvInfo( event, "receiving command <" + str + "> master volume>");
		BREAK
	}
	case ADJUST_KEY :
	{
		OSCID_e		oscid	= sds_p->SpectrumOscId;
		Oscillator* osc		= Instrument->Oscgroup.member[oscid];
		osc->features.adjust= sds_p->features[oscid].adjust;
		BREAK
	}
	case FEATURE_KEY	:
	{
		EvInfo( event, "Feature change");
		Instrument->Oscgroup.SetFeatures( sds_p->features );
		BREAK
	}
	case BEATCLOCK_KEY	:
	{
		EvInfo( event, "Beatclock change");
		for_each( Instrument->Oscgroup.member, [ this ]( Oscillator* osc )
			{ osc->Set_bps(); } );
		BREAK
	}
	case ADSR_KEY: // touch adsr data by setting new values
	{
		EvInfo			( event, "ADSR change");
		OSCID_e	oscid	= sds_p->SpectrumOscId;
		Instrument->Oscgroup.member[oscid]->Set_adsr( sds_p->adsr_arr[oscid] );

		BREAK
	}
	case PWMDIALKEY:
	{
		EvInfo( event, "PWM change");
		for_each( oscIds, [ this ]( OSCID_e oscid )
			{ Instrument->Oscgroup.member[oscid]->Set_pmw( sds_p->features[oscid].PWM ); } );
		BREAK
	}
	case FREQDYNAMIC_KEY:
	{
		EvInfo( event, "Frequency slide effect update ");
		uint8_t	slidevalue = sds_p->features[OSCID].frq_slide_value;
		sds_p->features[FMOID].frq_slide_value	= slidevalue;
		sds_p->features[VCOID].frq_slide_value	= slidevalue;
		Instrument->Oscgroup.SetFeatures( sds_p->features );

		BREAK
	}
	case EXTERNAL_WAVFILEKEY: // record and save wav file
	{
		if ( sds_master->Record_state == StateId_e::RECORDING )
			// Composer - Interpreter
			sds_master->Record_state = StateId_e::RECORDSTOP;
		else
			sds_master->Record_state = StateId_e::RECORDSTART; // e.g. if state is inactive

		BREAK
	}

 	 case STARTRECORD_KEY : // TODO complete functionality
	{
    	if( sds_p->StA_state_arr[STA_NOTES].play )
    	{
//    		sds_master->Record_state = StateId_e::RECORDSTART;
    		sds_master->Record_state = StateId_e::STARTING;
    		Notes->Note_itr_start.SetActive( true );
    		Notes->Note_itr_end.SetActive( false );
    	}
    	else // no synchronization
    	{
    		Notes->Note_itr_start.SetActive( false );
    		Notes->Note_itr_end.SetActive( false );
    		sds_master->Record_state = StateId_e::RECORDSTART;
    	}
		BREAK
	}

	case STOPRECORD_KEY :
	{
    	if( sds_p->StA_state_arr[STA_NOTES].play )
    	{
//    		sds_master->Record_state = StateId_e::RECORDSTOP;
    		sds_master->Record_state = StateId_e::STOPPING;
    		Notes->Note_itr_start.SetActive( false );
    		Notes->Note_itr_end.SetActive( true );
    	}
    	else // no synchronization
    	{
    		Notes->Note_itr_start.SetActive( false );
    		Notes->Note_itr_end.SetActive( false );
    		sds_master->Record_state = StateId_e::RECORDSTOP;
    	}

		BREAK
	}
	case CAPTURE_KEY : // TODO complete functionality
	{
		StateId_e state = sds_master->Capture_state;
		if ( state == StateId_e::RECORDSTART )				// 7
		{
			state = StateId_e::RECORDING;					// 9
			Capture->Start( STA_EXTERNAL );
			Capture->ReadAudio( true );
		}
		else
		{
			Capture->ReadAudio( false );
			Capture->Stop( STA_EXTERNAL );
			state = StateId_e::INACTIVE; 					// 12
		}
		sds_master->Capture_state = state;
		DaTA->EmitEvent( APPSTATE_FLAG, "update capture state " + sdsstateName(state) );

		BREAK
	}
	case KBD_SAVE_KEY :
	{
		Keyboard->Save_notes();
		DaTA->EmitEvent( NEWNOTESLINEFLAG, "keyboard notes update" );
		BREAK
	}
	case KBD_EVENT_KEY :
	{
		Keyboard->SetKbdKey( );

		BREAK
	}
	case READ_WAVFILE_KEY:
	{
		Comment(INFO, "receive command <set external wave file>");
		string wav_file = Sds_master->Read_str( WAVFILESTR );
		if ( External->Read_file_header( wav_file ) )
		{
			External->Read_file_data();
			sds_p->StA_state_arr[STA_EXTERNAL ].start	= true;
			StA->SetStAProperties( STA_EXTERNAL );
			ProgressBar->SetValue( 100 * External->Filedata_size / External->mem_ds.bytes);
			Wavedisplay->Set_WdData();
		}
		else
		{
			Comment(ERROR, "Failed to setup header");
		}
		BREAK
	}
	case STA_RECORD_KEY: // stop record on data array id
	{
		StAId_e id = sds_p->StA_Id ;
		if( not in_range( staid_range, id ) )
			BREAK

		bool store = sds_p->StA_state_arr[id].store;
		Info( "receive command <switch record" , store, "on storage area", (int)id, ">" );
		StA->SetStAProperties( id );
		if ( store )
			ProgressBar->Set(	StA->Array[id].Store_counter(),
								StA->Array[id].mem_ds.max_records);
		else
			ProgressBar->Unset();

		BREAK
	}
	case RESET_STA_SCANNER_KEY : // Audioserver start record
	{
		Info( "Reset StA scanner");
		for( StAId_e staid : StAIds )
		{
			StA->Array[staid].scanner.Set_rpos(0);
		}
		BREAK
	}
	case STA_VOLUME_KEY :
	{
		StAId_e		staid		= sds_p->StA_Id;
		uint8_t		volume 		= sds_p->StA_amp_arr[staid].amp;
		DYNAMIC_e	mode		= sds_p->StA_amp_arr[staid].mode;
		StA->Array[staid].Volume( volume, mode );
		Info( "STA_VOLUME", (int)staid, (int)volume, (int)mode );
		BREAK
	}
	case SETSTA_KEY:
	{
		StAId_e staid = sds_p->StA_Id;
		StA->SetStAProperties( staid );
		BREAK
	}
	case MUTESTA_KEY:
	{
		StAId_e staid = sds_p->StA_Id;
		if ( staid == STAID_ALL )
		{
			for( StAId_e id : StAIds )
				sds_p->StA_state_arr[id].play = false;
		}
		else
		{
			sds_p->StA_state_arr[staid].play = false;
		}
		StA->SetStAProperties( staid );
		Info( "receive command <mute and stop record on id", (int)staid, ">");
		BREAK
	}
	case CLEAR_KEY: // all or sta id
	{
		StAId_e	staid = sds_p->StA_Id;
		StA->ResetStA( staid );
		StA->SetStAProperties( staid );
		ProgressBar->Reset(); // RecCounter
		BREAK
	}
	case UPDATE_NLP_KEY: // Noteline_prefix
	{
		Comment(INFO, "receive command <update notesline prefix");
		Notes->Verify_noteline( sds_p->Noteline_prefix.origin );
		BREAK
	}
	case GENERATENOTES_KEY :
	{
		Notes->Set_new_name( sds_p->Notes );
		bool verified = Notes->Verify_noteline( sds_p->Noteline_prefix.origin );
		sds_p->NotesVerified 	= verified;
		if( verified )
			Notes->Generate_cyclic_data();// wait for commit
		else
			Comment( ERROR, "Notes not verified" );
		BREAK
	}
	case UPDATENOTESKEY: // generad notelist from sds and start playing
	{
		if ( sds_p->NotesTypeId == XML_ID )
		{
			string name = Sds->Read_str(NOTENAMESTR);
			Comment(INFO, "receive command <update notes>", name );
			EvInfo( event, "receive command <play xml notes> " + name);
			sds_p->StA_state_arr[ STA_NOTES ].play = true;
			StA->SetStAProperties( STA_NOTES );
			Sem->Release( SEMAPHORE_INITNOTES ); //other
			Sds->Commit();
		}
		if ( sds_p->NotesTypeId == NTE_ID )
		{
			sds_p->NotesVerified = Notes->Verify_noteline( sds_p->Noteline_prefix.origin ); // read from sds
			sds_p->StA_state_arr[STA_NOTES].start = false;
			sds_p->StA_state_arr[STA_NOTES].play = true; //immediate start
			StA->SetStAProperties( STA_NOTES );
			if( sds_p->mixer_state.composer )
			{
				DaTA->EmitEvent( NEWNOTESLINEFLAG ); // ->UpdateFileDialog
				Notes->Generate_cyclic_data();
			}
			else
				Sem->Release( SEMAPHORE_SYNCNOTES );
			Sds->Commit();
		}
		break;
	}
	case READNOTESFILE_KEY : // read notes file and update file dialog
	{
		Notes->Read( sds_p->Notes );
		DaTA->EmitEvent( NEWNOTESLINEFLAG ); // ->File_Dialog_p->updateWidgets()
		BREAK
	}
	case SAVENOTES_KEY :
	{
		Notes->Save( sds_p->Notes );
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
			sds_p->NotesTypeId = NTE_ID;

		} else {
			Comment(WARN, "nothing to do for", sec, "Notes!");
		}
		BREAK
	}
	case PLAYNOTESRECOFF_KEY: // play free
	{
		// functionality is defined in PLAYNOTESREC_ON_KEY
		Mixer->composer_cntr = 0;
		BREAK
	}
	case NOTESONKEY:
	{
		sds_p->StA_state_arr[STA_NOTES].play	= true;
		sds_p->StA_state_arr[STA_NOTES].start	= true;

		StA->SetStAProperties					( STA_NOTES );
		if( sds_p->mixer_state.composer )
			Notes->Generate_cyclic_data();
		else
			Sem->Release( SEMAPHORE_SYNCNOTES );
		Info( "receive command < notes on" );
		BREAK
	}
	case NOTESOFFKEY:
	{
		Comment(INFO, "receive command < notes off>");
		sds_p->StA_state_arr[STA_NOTES].play = false;
		StA->SetStAProperties( STA_NOTES );
		BREAK
	}

	case SETNOTESPERSEC_KEY:
	{
		Value nps = sds_p->Noteline_prefix.nps;
		Comment(INFO, "receive command <set notes per second>");
		Notes->Set_notes_per_second( nps.val );
		Notes->Generate_cyclic_data();

		BREAK
	}
	case CONNECTOSC_KEY :
	{
		EvInfo( event, "update modulation connections for the OSC");
		Instrument->Oscgroup.OscConnectState( sds_p->connect_arr );
		BREAK
	}
	case WAVEFORMKEY:
	{
		OSCID_e		oscid		= sds_p->SpectrumOscId;
		spec_arr_wt	wfids		= sds_p->spectrum_arr[oscid].wfid;
		WFID_e		wfid		= wfids[0];
		string		wf_str		= waveform->Get_str( wfid );
		EvInfo( event, typeNames[oscid] + " waveform is " + wf_str );
		Instrument->Oscgroup.member[oscid]->Set_waveform( wfids );
		BREAK
	}

	case SETINSTRUMENTKEY: // Set instrument
	{
		Instrument->Set	();

		DaTA->EmitEvent	( NEWINSTRUMENTFLAG, Instrument->Instrument_name.get() );
		BREAK
	}
	case SAVEINSTRUMENTKEY:
	{
		string Name 	= Sds->Read_str( INSTRUMENTSTR );
		Info			( "receive command <save instrument>", Name );
		Instrument->Save( Name );
		DaTA->EmitEvent	( NEWINSTRUMENTFLAG, Name );
		BREAK
	}
	case SAVESESSION_KEY :
	{
		Instrument->Save( Sds->Read_str( INSTRUMENTSTR ) );
		Notes->Save( Sds->Read_str( NOTENAMESTR ) );
		for_each( StA->Array, [ this ]( Storage_class& sta )
			{ StA->DumpStA( sta ) ;} );
		BREAK

	}
	case SAVEDEFAULT_KEY :
	{
		Instrument->Save( default_name );

		for_each( StA->Array, [ this ]( Storage_class& sta )
			{ StA->DumpStA( sta ) ;} );
		BREAK
	}
	case CUT_SETUP_KEY	:
	{
		CutDesk->Setup();
		DaTA->EmitEvent( UPDATE_CUTDESK_FLAG, "update cutdesk" );
		BREAK
	}
	case CUT_UPDATE_KEY :
	{
		CutDesk->CursorUpdate();
		DaTA->EmitEvent( UPDATE_CUTDESK_FLAG, "update cutdesk" );
		BREAK
	}
	case CUT_KEY :
	{
		CutDesk->Cut();
		BREAK
	}
	case CUT_SAVE_KEY :
	{
		sds_p->logState = StAExternal.Convert_StA2WAV( CutDesk->StAId, CutDesk->restore_range );
		DaTA->EmitEvent( UPDATE_WAV_FILELIST_FLAG, "update wav filelist" );
		BREAK
	}
	case CUT_RESTORE_KEY	:
	{
		CutDesk->Restore();
		BREAK
	}
	case CUT_STEPWIDTH_KEY	:
	{
		CutDesk->Set_record_steps( sds_p->WD_state.record_steps );
		BREAK
	}
	case RESETSDS_KEY		:
	{	// define and activate the initial state of the StAs
		sds_p->frq_slidermode				= FIXED;
		StA->RestoreStAs					();

		for_each( StA->Array, [this](Storage_class& StA )
		{
			StA.scanner.Set_rpos			( 0 ) ; // set start
			sds_p->StA_amp_arr[StA.Id].amp	= 0;
			sds_p->StA_state_arr[StA.Id] 	= StA_state_data();

		});
		// remark: the dependency of the wdstate to param.wdsize is covered by reference
		// and synthesizer add sound
		StA->Array[ STA_NOTES ].Reset	();
		StA->Array[ STA_EXTERNAL ].Reset();
		StA->SetStAProperties			( STAID_ALL );//all StAs
		BREAK
	}
	case SETSESSIONDIR_KEY : // distribute sessiondir to all invelved parties
	{
		string subdir 		= Sds_master->Read_str( SESSIONSTR );
		StA->SetStA_Files	( subdir );
		Instrument->SetDir	( subdir );
		Notes->SetDir		( subdir );
		Info( "using session directory", subdir );
		if( Mixer->state.composer ) // new sessiondir was defined by the composer
			DaTA->EmitEvent( UPDATE_FILEDIALOG_FLAG, "update Scorename" );

		BREAK
	}
	case FOURIERRESOLUTION_KEY	:
	{
		BREAK
	}
	case EXITKEY	:
	{
		Appstate->Shutdown( sds_p, SYNTHID );
		BREAK
	}
	default:
	{
		Exception( "Communication Key Id >" + to_string((int) (event ))	+ "< undefined");
	}
	} // switch event

}
