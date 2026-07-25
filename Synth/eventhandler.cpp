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
							Mixer_class*		mixer,
							Wavedisplay_class*	wavedisplay,
							Dataworld_class*	data,
							External_class*		external,
							ProgressBar_class*	progressbar,
							CutDesk_class*		cutter,
							Capture_class*		capture )
	: Logfacility_class	("Event_class")
	, sdsstate_struct	()
	, StAExternal		( &mixer->StA, data->Cfg_p, data->SDS.master )
	, eventbuffer		( &data->Sds_p->Eventque )
{
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
	Assert_equal( Mixer->StA[staid].state.Play(), false );


	sds_p->StA_state_arr[staid].play = true;
	Sds->Eventque.Add( SETSTA_KEY );
	Handler();
	Assert_equal( Mixer->StA[staid].state.Play(), true );

	sds_p->StA_state_arr[staid].store = true;
	Mixer->StA[staid].state.Store(false);
	Sds->Eventque.Add( SETSTA_KEY );
	Handler();
	Assert_equal( Mixer->StA[staid].state.Store(), true );

	Instrument->Oscgroup.member[OSCID]->Set_frequency( A3, FIXED );
	sds_p->frq_slidermode = FIXED;
	sds_p->spectrum_arr[OSCID].frqidx[0] = 72;
	sds_p->spectrum_arr[VCOID].frqidx[0] = 71;
	sds_p->features[VCOID].combine = true;
	Sds->Eventque.Add( OSCFREQUENCYKEY );
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
	case UPDATESPECTRUM_KEY:
	{
		EvInfo( event, "Spectrum update");
		char 		oscid 	= sds_p->SpectrumTypeId;
		Instrument->Update_osc_spectrum( oscid );
		BREAK
	}
	case MAINFRQ_KEY :
	{
		int8_t shift 						= sds_p->Noteline_prefix.octave_shift;
		sds_p->Noteline_prefix.octave_shift = 0;
		int 	idx							= sds_p->spectrum_arr[OSCID].frqidx[0];
		uint8_t frqidx 						= check_range( frqext_range, idx + shift*oct_steps );
		Instrument->Oscgroup.SetSliderFrequency( sds_p, frqidx, COMBINE );
		sds_p->spectrum_arr[OSCID].frqidx[0]= frqidx;

		BREAK
	}
	case OSCFREQUENCYKEY:
	{
		DYNAMIC_e 	mode 		= sds_p->frq_slidermode;
		uint8_t		frqidx 		= sds_p->spectrum_arr[OSCID].frqidx[0];
		EvInfo					( event, "Slider Mode: " + slidermode_str[ mode ] );
		Instrument->Oscgroup.SetSliderFrequency( sds_p, frqidx, mode );
		sds_p->WD_state.oscId	= OSCID;
		Wavedisplay->Set_WdData	();
		BREAK
	}
	case VCOFREQUENCYKEY: // modify the secondary oscillator
	{
		EvInfo( event, "VCO frequency change");

		Instrument->Oscgroup.vco.Set_frequency( sds_p->spectrum_arr[VCOID].frqidx[0], sds_p->frq_slidermode );
		Instrument->Connect( &Instrument->Oscgroup.osc, &Instrument->Oscgroup.vco, CONV );
		sds_p->WD_state.oscId	= VCOID;
		BREAK
	}
	case FMOFREQUENCYKEY : // modify the fm_track data
	{
		EvInfo( event, "FMO frequency change");

		Instrument->Oscgroup.member[ FMOID ]->Set_frequency( sds_p->spectrum_arr[FMOID].frqidx[0], sds_p->frq_slidermode );
		Instrument->Connect( &Instrument->Oscgroup.osc, &Instrument->Oscgroup.fmo, CONF );
		sds_p->WD_state.oscId	= FMOID;

		BREAK
	}
	case VCOAMPKEY : // modify the VCO volume
	{
		EvInfo( event, "VCO amplitude change");

		Value vol = sds_p->spectrum_arr[VCOID].volidx[0];
		Instrument->Oscgroup.vco.Set_spectrum_volume(vol.ch );
		Instrument->Connect( &Instrument->Oscgroup.osc, &Instrument->Oscgroup.vco, CONV);
		BREAK
	}
	case FMOAMPKEY : // modify the FMO volume
	{
		EvInfo( event, "FMO amplitude change");

		Value vol = sds_p->spectrum_arr[FMOID].volidx[0];
		Instrument->Oscgroup.fmo.Set_spectrum_volume(vol.ch );
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
		Instrument->Oscgroup.vco.features.adjust = sds_p->features[VCOID].adjust;
		Instrument->Oscgroup.fmo.features.adjust = sds_p->features[FMOID].adjust;
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
		std::ranges::for_each( Instrument->Oscgroup.member, [ this ]( Oscillator* osc )
			{ osc->Set_bps(); } );
		BREAK
	}
	case ADSR_KEY:
	{
		EvInfo( event, "ADSR change");
		OSCID_e oscid = sds_p->SpectrumTypeId;
		Instrument->Oscgroup.member[oscid]->Set_adsr( sds_p->adsr_arr[oscid] );

		sds_p->WD_state.adsrmode = true; // generate adsr, fft and wave data
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
		sds_p->WD_state.adsrmode = false; // do not generate adsr and fft data
		BREAK
	}
	case PWMDIALKEY:
	{
		EvInfo( event, "PWM change");
		std::ranges::for_each( Instrument->Oscgroup.member, [ this ]( Oscillator* osc )
				{ osc->Set_pmw( sds_p->features[VCOID].PWM ); } );
		BREAK
	}
	case FREQDYNAMIC_KEY:
	{
		EvInfo( event, "Frequency slide effect update ");
		uint8_t	slidevalue = sds_p->features[OSCID].frq_slide_value;
		sds_p->features[FMOID].frq_slide_value	= slidevalue;
		sds_p->features[VCOID].frq_slide_value	= slidevalue;
		Instrument->Oscgroup.SetFeatures( sds_p );

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
		string wav_file = Sds_master->Read_str( WAVFILE_STR );
		if ( External->Read_file_header( wav_file ) )
		{
			External->Read_file_data();
			sds_p->StA_state_arr[STA_EXTERNAL ].start	= true;
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
	case STA_RECORD_KEY: // stop record on data array id
	{
		StAId_e id = sds_p->StA_Id ;
		if( not in_range( staid_range, id ) )
			BREAK

		bool store = sds_p->StA_state_arr[id].store;
		Info( "receive command <switch record" , store, "on storage area", (int)id, ">" );
		Mixer->SetStAProperties( id );
		if ( store )
			ProgressBar->Set(	Mixer->StA[id].Store_counter(),
								Mixer->StA[id].mem_ds.max_records);
		else
			ProgressBar->Unset();

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
		StAId_e		staid		= sds_p->StA_Id;
		uint8_t		volume 		= sds_p->StA_amp_arr[staid].amp;
		DYNAMIC_e	mode		= sds_p->StA_amp_arr[staid].mode;
		Mixer->StA[staid].Volume( volume, mode );
		Info( "STA_VOLUME", (int)staid, (int)volume, (int)mode );
		BREAK
	}
	case SETSTA_KEY:
	{
		StAId_e staid = sds_p->StA_Id;
		if( staid == STAID_ALL )
			Mixer->SetStAs();
		else
			Mixer->SetStAProperties( staid );
		BREAK
	}
	case MUTESTA_KEY:
	{
		StAId_e staid = sds_p->StA_Id;
		if ( staid == STAID_ALL )
		{
			for( StAId_e id : StAIds )
				sds_p->StA_state_arr[id].play = false;
			Mixer->SetStAs();
		}
		else
		{
			sds_p->StA_state_arr[staid].play = false;
			Mixer->SetStAProperties( staid );
		}
		Info( "receive command <mute and stop record on id", (int)staid, ">");
		BREAK
	}
	case CLEAR_KEY: // all or sta id
	{
		StAId_e	staid = sds_p->StA_Id;
		Mixer->ResetStA( staid );
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
			Mixer->SetStAProperties( STA_NOTES );
			Sem->Release( SEMAPHORE_INITNOTES ); //other
			Sds->Commit();
		}
		if ( sds_p->NotesTypeId == NTE_ID )
		{
			sds_p->NotesVerified = Notes->Verify_noteline( sds_p->Noteline_prefix.origin ); // read from sds
			sds_p->StA_state_arr[STA_NOTES].start = false;
			sds_p->StA_state_arr[STA_NOTES].play = true; //immediate start
			Mixer->SetStAProperties( STA_NOTES );
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

		Mixer->SetStAProperties					( STA_NOTES );
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
		Mixer->SetStAProperties( STA_NOTES );
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
		Instrument->Oscgroup.OscConnectState( sds_p );
		BREAK
	}
	case SETWAVEFORMFMOKEY:
	{
		EvInfo( event, "FMO waveform " + to_string((int) sds_p->spectrum_arr[OSCID].wfid[0] ) );
		Instrument->Oscgroup.fmo.Set_waveform( sds_p->spectrum_arr[FMOID].wfid);

		BREAK
	}
	case SETWAVEFORMVCOKEY:
	{
		EvInfo( event, "VCO waveform " + to_string((int) sds_p->spectrum_arr[OSCID].wfid[0] ) );
		Instrument->Oscgroup.vco.Set_waveform( sds_p->spectrum_arr[VCOID].wfid);

		BREAK
	}
	case SETWAVEFORMMAINKEY:
	{
		EvInfo( event, "OSC waveform " + to_string((int) sds_p->spectrum_arr[OSCID].wfid[0] ) );
		Instrument->Oscgroup.osc.Set_waveform( sds_p->spectrum_arr[OSCID].wfid);

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
		std::ranges::for_each( Mixer->StA, [ this ]( Storage_class& sta )
			{ Mixer->DumpStA( sta ) ;} );
		BREAK

	}
	case SAVEDEFAULT_KEY :
	{
		Instrument->Save( default_name );

		std::ranges::for_each( Mixer->StA, [ this ]( Storage_class& sta )
			{ Mixer->DumpStA( sta ) ;} );
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
		StAExternal.Convert_StA2WAV( CutDesk->StAId, CutDesk->restore_range );
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
	case RESETSTA_KEY		:
	{	// define and activate the initial state of the StAs
		sds_p->frq_slidermode				= FIXED;

		std::ranges::for_each( Mixer->StA, [this](Storage_class& StA )
		{
			StA.scanner.Set_rpos			( 0 ) ; // set start
			sds_p->StA_amp_arr[StA.Id].amp	= 0;
			sds_p->StA_state_arr[StA.Id] 	= StA_state_data();
		});
		Mixer->StA[ STA_NOTES ].Reset	();
		Mixer->StA[ STA_EXTERNAL ].Reset();
		Mixer->SetStAs					();		//all StAs
		BREAK
	}
	case SETSESSIONDIR_KEY : // distribute sessiondir to all invelved parties
	{
		string subdir 		= Sds_master->Read_str( SESSIONSTR );
		Mixer->SetStA_Files	( subdir );
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
