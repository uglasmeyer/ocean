/**********************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * oceanGUI.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

// Qt

// Ocean

// OceanGUI
#include <Mainwindow.h>
#include <GUIcommon.h>

const string 		Module 			= OCEANGUI;
typedef MainWindow::cb_state_map cb_state_t;
typedef MainWindow::sl_value_map sl_value_t;

auto set_sl_sta_value = [ ]( MainWindow* M )
{
    for( sl_value_t map : M->sl_sta_vec )
    	map.sl->setValue( *map.value );
};
auto set_cb_ssta_value = [ ]( MainWindow* M )
{
	for( cb_state_t map : M->cb_store_sta_vec )
    	map.cb->setChecked( (bool) *map.state );
};
auto set_cb_psta_value = [ ]( MainWindow* M )
{
    for( cb_state_t map : M->cb_play_sta_vec )
    	map.cb->setChecked( *map.state );
};
auto set_cb_filled_value = [  ]( MainWindow* M  )
{
	for( cb_state_t map : M->cb_filled_sta_vec )
		map.cb->setChecked( *map.state );
};

auto AppIdRole = [ ]( APPID appid )
{
	map<APPID, RoleId_e> AppIdRole_map // @suppress("Invalid arguments")
	{
			{ AUDIOID		, AUDIOROLE },
			{ SYNTHID		, INSTRROLE },
			{ KEYBOARDID	, KBDROLE }
	};

	auto role_itr = AppIdRole_map.find( appid );
	if( role_itr != AppIdRole_map.end() )
		return role_itr->second;
	else
		return ROLE_SIZE;
};

/**************************************************
 * MainWindow
 *************************************************/
MainWindow::MainWindow		( QWidget *parent )
	: Logfacility_class		( "OceanGUI" )
	, wavedisplay_struct	()
	, ui					( new Ui::MainWindow )
	, Rtsp_Dialog_p			( new Rtsp_Dialog_class { this, DaTA_p } )
	, File_Dialog_p			( new File_Dialog_class { this, DaTA_p, Eventlog_p } )
	, Spectrum_Dialog_p 	( new Spectrum_Dialog_class { this, DaTA_p, Eventlog_p } )
	, Keyboard_Dialog_p		( new Keyboad_Dialog_class{ this, DaTA_p, Eventlog_p } )
	, CutDesk_Dialog_p		( new CutDesk_Dialog_class{ this, DaTA_p, Eventlog_p } )
	, Bps					()
{
	Sds->Set( sds_master->UpdateFlag, true);
	Sds->Set( sds_p->UpdateFlag, true);

	ui->setupUi				( this );

	initPanel				();
	initLables				();
    initGuiVectors			( sds_p );
    initScrollbars			();
	initStateButtons		();
    initDataDisplay			();
    initFreqSlider			();
    initComboBoxes			();

    initUiConnectors		();
    updateColorButtons		();
    initTimer				(); //-< update widgets

    Info( "User Interface initialized" );
}

MainWindow::~MainWindow()
{
	DESTRUCTOR( className );
	delete Rtsp_Dialog_p;
	delete File_Dialog_p;
	delete OscWidget_item;
	delete Spectrum_Dialog_p;
	delete Keyboard_Dialog_p;
	delete CutDesk_Dialog_p;
	delete scene;
	delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	kbdkey_t	 	key 	{};
					key.Int	= Keymap.Qt_key( event->key() ) ;
	switch ( key.Int )
	{
		case Qt::Key_Escape :
		{
			GUI_Exit();
			break;
		}
		default :
		{
			sds_p->Kbd_state.key = key.Int;
			QMainWindow::keyPressEvent( event );
			break;
		}
	} // switch  key
	set_wdrole( KBDROLE );
}


void MainWindow::GUI_Exit()
{
    Info( "Exit" );

    QApplication::quit();
}

void MainWindow::mixer_balance()
{
	int value = ui->hs_balance->value();
	Sds->Set( sds_master->mixer_balance, (int8_t) value );
}

void MainWindow::chord_delay()
{
	int value = ui->hs_chord_delay->value();
	Sds->Set( sds_p->noteline_prefix.chord_delay, value );
}
void MainWindow::set_wdrole( RoleId_e roleid )
{
	WfDisplay_que.Add( roleid );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );
}

void MainWindow::unset_wdrole( RoleId_e role )
{
	WfDisplay_que.Remove( role );
}

void MainWindow::wavfile_selected( QString arg)
{
    if ( arg.length() > 0 )
    {
        string str = arg.toStdString();
        Sds->Write_str( WAVFILESTR_KEY, str );
        Eventlog.add( SDS_ID, READ_EXTERNAL_WAVFILE);
        if( CutDesk_Dialog_p->isVisible() )
        {
        	CutDesk_Dialog_p->Setup( Sds );
        }
    }
}


void MainWindow::adsr_hall( )
{
	uint8_t value = ui->hs_hall_effect->value();
    Sds->Set(sds_p->adsr_arr[OSCID].hall , value);
    Eventlog.add( SDS_ID, ADSR_KEY);
}

void MainWindow::Beat_per_sec( int bps_id )
{
	uint8_t bps = Bps.Bps_vec[bps_id];
	Sds->Set( sds_p->instrumentClock, bps );
    Eventlog.add( SDS_ID, BEATCLOCK_KEY );
}

void MainWindow::Notes_per_measure( int bps_id )
{
	uint8_t bps = Bps.Bps_vec[bps_id];
	Sds->Set( sds_p->beatClock, bps );
}

void MainWindow::slideFrq( int value )
{
    Sds->Set(sds_p->features[OSCID].slide_frq , (uint8_t)value);
    Eventlog.add( SDS_ID, SOFTFREQUENCYKEY);

};


void MainWindow::dial_PMW_value_changed()
{
    uint8_t value = ui->hs_pmw->value();
    Sds->Set(sds_p->features[VCOID].PWM , value );
    Eventlog.add( SDS_ID,PWMDIALKEY);
}



void MainWindow::ADSR_Dialog()
{
	this->Spectrum_Dialog_p->Dialog( true );
	Sds->Set( sds_p->WD_state.adsrmode, Spectrum_Dialog_p->isVisible() );
}

void MainWindow::Spectrum_Dialog()
{
    this->Spectrum_Dialog_p->Dialog( false);
}


void MainWindow::waveform_slot(	uint8_t* wf_addr,
								uint8_t wfid,
								OSCID_e oscid,
								EVENTKEY_e wf_key,
								QLabel* label  )
{
	Sds->Set( *wf_addr, wfid );
	label->setText( QWaveform_vec[ wfid ] );
	sds_p->WD_state.oscId=oscid;
	set_wdrole( INSTRROLE );
	Eventlog.add( SDS_ID, wf_key);
}

void MainWindow::Main_Waveform_slot( int _wfid )
{
	waveform_slot( &sds_p->spectrum_arr[OSCID].wfid[0], _wfid, OSCID, SETWAVEFORMMAINKEY, ui->wf_osc );
}
void MainWindow::FMO_Waveform_slot(int _wfid)
{
	waveform_slot( &sds_p->spectrum_arr[FMOID].wfid[0], _wfid, FMOID, SETWAVEFORMFMOKEY, ui->wf_fmo );
}
void MainWindow::VCO_Waveform_slot( int _wfid )
{
	waveform_slot( &sds_p->spectrum_arr[VCOID].wfid[0], _wfid, VCOID, SETWAVEFORMVCOKEY, ui->wf_vco );
}

void MainWindow::select_Sds( Id_t sdsid )
{
	Sds->Set( this->sds_master->config, sdsid );
	Sds->Set( this->sds_master->UpdateFlag	, true);
	Sds->Set( this->sds_p->UpdateFlag	, true); // update old sds

	this->Sds 		= DaTA.SDS.GetSds( sdsid ); // set new sds
	this->SDS_ID	= sdsid ;
	this->sds_p 	= Sds->addr;

	initGuiVectors( this->sds_p );
	initStateButtons();

    Info( "Activate SDS ", to_string( (int) sds_p->SDS_Id ));

	File_Dialog_p->SetSds();
	Spectrum_Dialog_p->SetSds();
	Rtsp_Dialog_p->Proc_table_update( this->sds_p, SYNTHID );
	Keyboard_Dialog_p->SetSds();
	CutDesk_Dialog_p->SetSds();
   	rb_S_vec[ sds_master->config ]->setChecked( true );

	setwidgetvalues();
};

void MainWindow::select_Sds0()
{
	select_Sds( 0);
}
void MainWindow::select_Sds1()
{
	select_Sds( 1);
}
void MainWindow::select_Sds2()
{
	select_Sds( 2);
}
void MainWindow::select_Sds3()
{
	select_Sds( 3);
}

void MainWindow::CombineFreq()
{
	if ( ui->cB_Combine->isChecked() )
		Sds->Set( sds_p->frq_slidermode,  COMBINE );
	else
		Sds->Set( sds_p->frq_slidermode, SLIDE );
}

void MainWindow::setStaPlay( StAId_e staid )
{
    bool 	play		= not sds_p->StA_state_arr[staid].play ;
    		Sds->Set	( sds_p->StA_state_arr[staid].play, play );
    		Sds->Set	( sds_p->StA_Id , staid );
    RoleId_e role		= StaRole_map.GetRoleid( staid );
    		if( play )
    			set_wdrole	( role );
    		else
    			unset_wdrole( role );

    		Eventlog.add( SDS_ID, SETSTA_KEY );
}

void MainWindow::setStaPlay0(  )
{
	setStaPlay( STA_USER00 );
}
void MainWindow::setStaPlay1(  )
{
	setStaPlay( STA_USER01 );
}
void MainWindow::setStaPlay2(  )
{
	setStaPlay( STA_USER02 );
}
void MainWindow::setStaPlay3(  )
{
	setStaPlay( STA_USER03 );
}
void MainWindow::setStaPlay4(  )
{
	setStaPlay( STA_INSTRUMENT );
}
void MainWindow::setStaPlay5(  )
{
	setStaPlay( STA_KEYBOARD );
}
void MainWindow::setStaPlay6(  )
{
	setStaPlay( STA_NOTES );
}
void MainWindow::setStaPlay7(  )
{
	setStaPlay( STA_EXTERNAL );
}

void MainWindow::setStaStored( StAId_e staId )
{
    Sds->Set( sds_p->StA_Id, staId );
    Eventlog.add( SDS_ID, CLEAR_KEY );
}
void MainWindow::setStaStored0()
{
	setStaStored( STA_USER00 );
}
void MainWindow::setStaStored1()
{
	setStaStored( STA_USER01 );
}
void MainWindow::setStaStored2()
{
	setStaStored( STA_USER02 );
}
void MainWindow::setStaStored3()
{
	setStaStored( STA_USER03 );
}
void MainWindow::setStaStored4()
{
	setStaStored( STA_INSTRUMENT );
}
void MainWindow::setStaStored5()
{
	setStaStored( STA_KEYBOARD );
}
void MainWindow::setStaStored6()
{
	setStaStored( STA_NOTES );
}
void MainWindow::setStaStored7()
{
	setStaStored( STA_EXTERNAL );
}
void MainWindow::memory_clear()
{
	setStaStored( STA_SIZE );
}

void MainWindow::toggle_store_sta( StAId_e id )
{

	Sds->Set( sds_p->StA_Id , id );
    if ( sds_p->StA_state_arr[id].store )
    {
    	Eventlog.add( SDS_ID, STARECORD_STOP_KEY);
    }
    else
    {
        Eventlog.add( SDS_ID, STARECORD_START_KEY);
    }
//    set_wdrole( AUDIOROLE );
};

void MainWindow::toggle_store_sta0()
{
	toggle_store_sta( STA_USER00 );
}
void MainWindow::toggle_store_sta1()
{
	toggle_store_sta( STA_USER01 );
}
void MainWindow::toggle_store_sta2()
{
	toggle_store_sta( STA_USER02 );
}
void MainWindow::toggle_store_sta3()
{
	toggle_store_sta( STA_USER03 );
}
void MainWindow::toggle_store_sta4()
{
	toggle_store_sta( STA_INSTRUMENT );
}
void MainWindow::toggle_store_sta5()
{
	toggle_store_sta( STA_KEYBOARD );
}
void MainWindow::toggle_store_sta6()
{
	toggle_store_sta( STA_NOTES );
}
void MainWindow::toggle_store_sta7()
{
	toggle_store_sta( STA_EXTERNAL );
}


void MainWindow::mixer_slider( sl_value_t map )
{
    Sds->Set( sds_p->StA_Id , map.id );
    Sds->Set( *map.value, (uint8_t)map.sl->value() );
    Eventlog.add( SDS_ID, STA_VOLUME_KEY );
};

void MainWindow::Sl_mix0( int value )
{
	mixer_slider( sl_sta_vec[STA_USER00] );
};
void MainWindow::Sl_mix1( int value )
{
	mixer_slider( sl_sta_vec[STA_USER01] );
};
void MainWindow::Sl_mix2( int value )
{
	mixer_slider( sl_sta_vec[STA_USER02] );
};
void MainWindow::Sl_mix3( int value )
{
	mixer_slider( sl_sta_vec[STA_USER03] );
};
void MainWindow::Sl_mix4( int value )
{
	mixer_slider( sl_sta_vec[STA_INSTRUMENT] );
};
void MainWindow::Sl_mix5( int value )
{
	mixer_slider( sl_sta_vec[STA_KEYBOARD] );
};
void MainWindow::Sl_mix6( int value )
{
	mixer_slider( sl_sta_vec[STA_NOTES] );
};
void MainWindow::Sl_mix7( int value )
{
	mixer_slider( sl_sta_vec[STA_EXTERNAL] );
};

void MainWindow::slideVol( int value )
{
    Sds->Set( sds_master->slide_duration, (uint8_t)value); // % of 4*max_seconds
    Eventlog.add( 0, MASTERAMP_KEY);
}

void MainWindow::clear_StAs_play()
{
	for( cb_state_t map : cb_play_sta_vec )
		map.cb->setChecked(false);
    Eventlog.add( SDS_ID, STAS_CLEARPLAY_KEY );
}
void MainWindow::toggle_Mute()
{
	Eventlog.add( SDS_ID, MASTERAMP_MUTE_KEY);
	return;
    bool	mute_flag 	= not sds_p->mixer_state.mute ;
    QString Qstr 		= mute_flag ? "UnMute" : "Mute";
   			Sds->Set	( sds_p->mixer_state.mute, mute_flag );
   			ui->pB_Mute->setText( Qstr );
}


void MainWindow::sliderFreq( sl_lcd_t map, int value )
{
	float freq 		= Spectrum_Dialog_p->Frequency.GetFrq( value );
	map.lcd->display(  freq  );
	Sds->Set( *map.value, uint8_t( value ) );

	Eventlog.add( SDS_ID, map.event );
};
void MainWindow::Slider_OSC_Freq( int value )
{
	sliderFreq( sl_frqidx_vec[OSCID], value );
	set_wdrole( INSTRROLE );

}
void MainWindow::Slider_VCO_Freq( int value )
{
	( (uint)value < C0 ) ? ui->lb_VCO_LFO->show() : ui->lb_VCO_LFO->hide();

	sliderFreq( sl_frqidx_vec[VCOID], value );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
	set_wdrole( INSTRROLE );

}
void MainWindow::Slider_FMO_Freq( int value )
{
	( (uint)value < C0 ) ? ui->lb_FMO_LFO->show() : ui->lb_FMO_LFO->hide();

	sliderFreq( sl_frqidx_vec[FMOID], value );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
	set_wdrole( INSTRROLE );
}

void MainWindow::sliderVolume( sl_lcd_t map )
{
	uint8_t value = map.sl->value();
	Sds->Set( *map.value, value);
	map.lcd->display( value );
	Eventlog.add( SDS_ID, map.event);
};

void MainWindow::Main_slot_volume()
{
	sliderVolume( sl_volume_vec[ OSCID ] );
}
void MainWindow::VCO_slot_volume()
{
	sliderVolume( sl_volume_vec[ VCOID ] );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
}
void MainWindow::FMO_slot_volume()
{
	sliderVolume( sl_volume_vec[ FMOID ] );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
}

void MainWindow::Slider_VCO_Adjust( int value )
{
	Sds->Set( sds_p->features[VCOID].adjust, (uint8_t) value);
	Sds->Set( sds_p->WD_state.oscId, VCOID );
	set_wdrole( INSTRROLE );
	Eventlog.add( SDS_ID, ADJUST_KEY);
}
void MainWindow::Slider_FMO_Adjust( int value )
{
	Sds->Set( sds_p->features[FMOID].adjust, (uint8_t) value);
	Sds->Set( sds_p->WD_state.oscId, FMOID );
	set_wdrole( INSTRROLE );
	Eventlog.add( SDS_ID, ADJUST_KEY );

}

void MainWindow::changeAppState( APPID appid, string terminal, string binary  )
{
	interface_t* sds = sds_p;
	if( Appstate->assignMasterSds.contains( appid ) )
		sds = sds_master;
	if( Appstate->IsRunning( sds, appid ) )
	{
		Appstate->SetExitserver( sds, appid ) ;
		return;
	}

	if( Appstate->IsRunning( sds_master, RTSPID ) )
	{
		if ( Sem_p->Getval( SYNTHESIZER_START, GETVAL ) > 0 )
			Sem_p->Release( SYNTHESIZER_START );
		return;
	}

	string opt = "";
	if( strEqual( terminal, "nohup"  ) )
		opt = " 2>&1 >> " + fs->nohup_file;
	string cmd = Cfg_p->Server_cmd( terminal, binary, opt );
	System_execute( cmd, true );

	Id_t sdsid = Appstate->GetNextSdsId( appid );
	if( sdsid != sds_master->config )
	{
		select_Sds( sdsid );
	}

	RoleId_e role = AppIdRole( appid );
	set_wdrole( role );
}

void MainWindow::start_audio_srv()
{
	changeAppState( AUDIOID, Cfg_p->Config.Nohup, fs->audio_bin );
	return;
}
void MainWindow::start_synthesizer()
{
	changeAppState( SYNTHID, Cfg_p->Config.Nohup, fs->synth_bin );
	return;
}
void MainWindow::start_keyboard()
{
	changeAppState( KEYBOARDID, Cfg_p->Config.Term, fs->Keyboard_bin );
	return;
}
void MainWindow::start_sdsview()
{
	changeAppState( SDSVIEWID, Cfg_p->Config.Term, fs->sdsview_bin );
	return;
}
void MainWindow::start_composer()
{
    string Start_Composer = Cfg_p->Server_cmd( Cfg_p->Config.Term, fs->composer_bin, "" );
	System_execute( Start_Composer );
}


void MainWindow::read_wave_data()
{
    OscWidget_item->ReadWaveData();
};


void MainWindow::Save_Config()
{
    Sds->Write_str( INSTRUMENTSTR_KEY, "default");
	Eventlog.add( SDS_ID, SAVEINSTRUMENTKEY);
}

auto connect_secundary = [  ]( MainWindow* M, OSCID_e oscid, OSCID_e secid, char mode, bool val )
{
	if ( mode == CONF )
	{
		if( val )
			M->Sds->Set( M->sds_p->connect_arr[oscid].frq, secid );
		else
			M->Sds->Set( M->sds_p->connect_arr[oscid].frq, oscid );
	}
	if ( mode == CONV )
	{
		if( val )
			M->Sds->Set( M->sds_p->connect_arr[oscid].vol, secid );
		else
			M->Sds->Set( M->sds_p->connect_arr[oscid].vol, oscid );
	}
	M->Eventlog.add( M->SDS_ID, CONNECTOSC_KEY );
};

void MainWindow::connect_oscf( bool val )
{
	connect_secundary( this, OSCID, FMOID, CONF, val );
}
void MainWindow::connect_oscv( bool val )
{
	connect_secundary( this, OSCID, VCOID, CONV, val );
}
void MainWindow::connect_fmov( bool val )
{
	connect_secundary( this, FMOID, VCOID, CONV, val );
}
void MainWindow::connect_vcov( bool val )
{
	connect_secundary( this, VCOID, FMOID, CONV, val );
}

void MainWindow::get_record_status( )
{
    int pb_value = sds_master->RecCounter;
    ui->progressBar_record->setValue( pb_value );
}

void MainWindow::capture_audio()
{
	StateId_t state = sds_master->Capture_state;
	if ( state == sdsstate_struct::RECORDING )
	{
		state = sdsstate_struct::INACTIVE;
	}
	else
	{
		state = sdsstate_struct::RECORDSTART;
		set_wdrole( EXTERNALROLE );
	}
	Sds->Set( sds_p->Capture_state, state );
	Eventlog.add( SDS_ID, CAPTURE_KEY );
}

void MainWindow::SaveRecord()
{

	if( ui->cb_overwrite->isChecked() )
		Sds->Set( sds_master->FileNo, 0_uint ); // take filename from the Other-cstring
	else
		Sds->Set( sds_master->FileNo ,1_uint ); // 1=automatic numbering
	// see also: Config.MAXWAVFILES, fs->counter_file

    if ( sds_master->Record_state == sdsstate_struct::RECORDING )
    {
        Eventlog.add( SDS_ID, STOPRECORD_KEY );
    }
    if ( sds_master->Record_state == sdsstate_struct::INACTIVE )
    {
        Eventlog.add( SDS_ID, STARTRECORD_KEY );
    }
}


void MainWindow::pB_Debug_clicked()
{
    uint8_t counter = sds_p->WD_state.wd_mode;
	counter = ( counter + 1 ) % ( WD_MODE_SIZE );// no cursor mode
    Sds->Set( sds_p->WD_state.wd_mode , (WdModeID_t)counter );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );

    ui->pB_wd_mode->setText( Qwd_wdmode_names[ (int)counter ] );
}

void MainWindow::pB_oscgroup_clicked()
{
    uint8_t counter = sds_p->WD_state.oscId ;
	counter = ( counter + 1 ) % WD_OSC_SIZE;
    Sds->Set( sds_p->WD_state.oscId, (OSCID_e)counter );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY);

    ui->pB_oscgroup->setText( Qwd_osc_names[ (int)counter ] );
}

void MainWindow::pB_Wavedisplay_clicked()
{
	RoleId_e	wdStateRole	= sds_p->WD_state.roleId;
	RoleId_e 	prev_role	= wdStateRole;
	unset_wdrole			( prev_role );
	RoleId_e 	role	 	= RoleId_e( ( prev_role + 1 ) %  WD_ROLES_SIZE ) ;
	set_wdrole				( role );
};

void MainWindow::pB_fftmode_clicked()
{
	bool fft_mode = not (sds_p->WD_state.fftmode );
	Sds->Set( sds_p->WD_state.fftmode, fft_mode );
	Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );
}

void MainWindow::update_CB_external()
{
	Info( "update recording info");
	ui->cB_external->clear();
	ui->cB_external->addItems( Qread_filenames( EventWAV.path ));
	QString Qfile	= QReadStr( Sds, EventWAV.event );
	ui->cB_external->setCurrentText( Qfile );
}

void MainWindow::setwidgetvalues()
{
	Comment( DEBUG, "MainWindow::setwidgetvalues" );
	if( Appstate->IsExitserver( sds_p, GUI_ID ) )
		GUI_Exit();
	else
		Appstate->SetRunning(  );

	for ( int oscid : oscIds )
	{
		sl_volume_vec[oscid].sl->setValue( *sl_volume_vec[oscid].value );
		sl_volume_vec[oscid].lcd->display( *sl_volume_vec[oscid].value );

		sl_frqidx_vec[oscid].sl->setValue( *sl_frqidx_vec[oscid].value );
		sl_frqidx_vec[oscid].lcd->display( frqArray[ *sl_frqidx_vec[oscid].value ]);

		sB_lbl_vec[oscid].lbl->setText( QWaveform_vec[ *sB_lbl_vec[oscid].value] );
		sB_lbl_vec[oscid].sb->setValue( *sB_lbl_vec[oscid].value );
	};
    ui->hs_pmw->setValue			( (int) sds_p->features[VCOID].PWM  );
    ui->hs_hall_effect->setValue	( (int) sds_p->adsr_arr[OSCID].hall );
    ui->Slider_slideFrq->setValue	( (int) sds_p->features[OSCID].slide_frq );
    ui->Slider_slideVol->setValue	( sds_master->slide_duration);//Master_Amp);
    ui->hs_balance->setValue		( sds_master->mixer_balance );

    get_record_status();


    QString
	Qstr = sds_p->mixer_state.mute ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );

    Qstr	= int2char( sds_p->beatClock );
    ui->CB_kbd_bps->setCurrentText( Qstr );
    Qstr	= int2char( sds_p->instrumentClock );
    ui->CB_inst_bps->setCurrentText( Qstr );

	set_cb_psta_value	( this );
	set_cb_ssta_value	( this );
	set_sl_sta_value	( this );
	set_cb_filled_value	( this );

	ui->Pbar_telapsed->setValue( sds_p->time_elapsed );


	if( Rtsp_Dialog_p->isVisible() )
		Rtsp_Dialog_p->Proc_table_update_all();

	WD_data_t	wd_data	= sds_p->WD_state;
	int			wd_mode	= check_range( wd_mode_range, (int)sds_p->WD_state.wd_mode );
	ui->pB_wd_mode->setText( Qwd_wdmode_names[ wd_mode ] );
	ui->pB_WdRole->setText( Qwd_role_names[ wd_data.roleId ] );
    ui->pB_oscgroup->setText( Qwd_osc_names[ wd_data.oscId ] );
	ui->pb_fftmode->setText( Qwd_fftmodes[ (int)wd_data.fftmode ] );
	if ( wd_data.roleId == AUDIOROLE )
		OscWidget_item->sds = sds_master;
	else
		OscWidget_item->sds = sds_p;

   	bool combine = ( sds_p->frq_slidermode == COMBINE );
   	ui->cB_Combine->setChecked( combine );
   	//    rb_S_vec[ sds_p->SDS_Id ]->setChecked( true );

	ui->cb_connect_fmov->setChecked ( (bool)(sds_p->connect_arr[FMOID].vol != FMOID));
	ui->cb_connect_vcov->setChecked ( (bool)(sds_p->connect_arr[VCOID].vol != VCOID) );
	ui->cb_connect_oscv->setChecked( (bool)(sds_p->connect_arr[OSCID].vol != OSCID) );
	ui->cb_connect_oscf->setChecked( (bool)(sds_p->connect_arr[OSCID].frq != OSCID) );

    double frq_slide_duration = sds_p->features[OSCID].slide_frq * max_sec * percent;
    ui->lbl_frqglide_sec->setText( QString::number( frq_slide_duration ) + "\n[sec[");

	if( sds_p->StA_state_arr[ STA_KEYBOARD ].play  )
	{
		if( not Keyboard_Dialog_p->isVisible() )
		{
			Keyboard_Dialog_p->show();
			Keyboard_Dialog_p->Setup_Widget();
		}
	}
	else
	{
		if( Keyboard_Dialog_p->isVisible() )
			Keyboard_Dialog_p->hide();
	}
    updateColorButtons();
}

void MainWindow::updateColorButtons()
{
	Comment( DEBUG, "MainWindow::updateColorButtons" );
	auto isRunning = [ this ]( APPID appid, interface_t* sds )
	{
		return Appstate->IsRunning( sds , appid );
	};

	uint audio_state		=	isRunning( AUDIOID, sds_master ) ;
	uint synthesizer_state	= 	isRunning( SYNTHID, sds_p )*2 + // red or yellow
								audio_state;   // red or yellow or green
	bool keyboard_up		= 	isRunning( APPID::KEYBOARDID, sds_p );
	uint keyboard_state 	= 	keyboard_up*2 + audio_state;
    setButton( ui->pBSynthesizer, synthesizer_state );
    setButton( ui->pb_Keyboard	, keyboard_state );
    audio_state = ( sds_master->overmodulated ) ? 2 : audio_state*3;
    setButton( ui->pBAudioServer, audio_state ); // red or green
    setButton( ui->pb_SDSview	, isRunning( SDSVIEWID, sds_master )*2+1 );
    setButton( ui->pBComposer	, isRunning( COMPID, sds_master )*2+1 );

    bool record 	= ( sds_master->Record_state == sdsstate_struct::RECORDING );
    bool stopping	= ( sds_master->Record_state == sdsstate_struct::STOPPING );
    bool starting	= ( sds_master->Record_state == sdsstate_struct::STARTING );
    if( stopping or starting )
        setButton( ui->pBtoggleRecord, 2 ); // yellow
    else
    	setButton( ui->pBtoggleRecord, not record );
    ( record ) ? 	ui->pBtoggleRecord->setText( "Stop Rec") :
    				ui->pBtoggleRecord->setText( "Record");

    if( Rtsp_Dialog_p->isVisible() )
    	setButton( ui->pB_Rtsp, 3 );
    else
        setButton( ui->pB_Rtsp, 1 );

    if( File_Dialog_p->isVisible() )
        setButton( ui->pB_play_notes, 3 );
    else
        setButton( ui->pB_play_notes, 1 );

    if( Spectrum_Dialog_p->isVisible() )
        setButton( ui->pB_Specrum, 3 );
    else
        setButton( ui->pB_Specrum, 1 );

    if( CutDesk_Dialog_p->isVisible() )
	    setButton( ui->pB_Cutter, 3 );
    else
	    setButton( ui->pB_Cutter, 1 );

    if( sds_master->Capture_state == sdsstate_struct::RECORDSTART )
    	setButton( ui->pB_Capture, 2 ); // yellow
    if( sds_master->Capture_state == sdsstate_struct::RECORDING )
    	setButton( ui->pB_Capture, 0 ); // red
    if( sds_master->Capture_state == sdsstate_struct::INACTIVE )
    	setButton( ui->pB_Capture, 4 ); // blue

}
