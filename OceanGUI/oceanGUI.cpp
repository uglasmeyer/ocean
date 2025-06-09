
// qtcreator
#include <EventKeys.h>
#include <Mainwindow.h>
#include <ui_mainwindow.h>
#include <Oszilloscopewidget.h>
#include <Spectrum_dialog_class.h>

// Synhesizer
#include <Wavedisplay_base.h>
#include <Logfacility.h>
#include <Mixer.h>
#include <Ocean.h>

// Qt
#include <QLabel>
#include <QPolygon>
#include <QTimer>
#include <QRect>




const string 		Module 		= "OceanGUI";

//Ui::MainWindow	Ui_Mainwindow_obj{};

auto set_cb_ssta_value = [ ]( MainWindow* M )
{

	for( MainWindow::cb_state_t map : M->cb_store_sta_vec )
    {
    	map.cb->setChecked( (bool) *map.state );
    }
};
auto set_cb_psta_value = [ ]( MainWindow* M )
{
    for( MainWindow::cb_state_t map : M->cb_play_sta_vec )
    	map.cb->setChecked( *map.state );
};
auto set_sl_sta_value = [ ]( MainWindow* M )
{
    for( MainWindow::sl_value_t map : M->sl_sta_vec )
    	map.sl->setValue( *map.value );
};
auto set_cb_filled_value = [  ]( MainWindow* M  )
{
	for( MainWindow::cb_state_t map : M->cb_filld_sta_vec )
		map.cb->setChecked( *map.state );
};



MainWindow::MainWindow(	QWidget *parent ) :
		Logfacility_class( Module ),
		osc_struct(),
		ui(new Ui::MainWindow{} )
{
	ui->setupUi(this);
	initPanel();

	Sds->Set(Sds_master->UpdateFlag, true);
	Sds->Set( Sds->addr->UpdateFlag, true);
	Sds->Set( Sds_master->Record, false);

    initGuiVectors();

	initStateButtons();
    initOscillatorDisplay();
    initFreqSlider();
    initScrollbars();
    initComboBoxes();

    initUiConnectors();
    initTimer();
}

MainWindow::~MainWindow()
{
	cout << "visited ~MainWindow" << endl;
	printf( "%p\n\n", OscWidget_item );
	if ( OscWidget_item ) delete ( OscWidget_item );
}


void MainWindow::GUI_Exit()
{
    qDebug("%s", "Exit" );
    QApplication::quit();
}

void MainWindow::mixer_balance()
{
	int value = ui->hs_balance->value();
	Sds->Set( Sds->addr->mixer_balance, (int8_t) value );
}
void MainWindow::wavfile_selected( const QString &arg)
{
    qDebug() << "WAV file " << arg ;
    QString QStr = arg;
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        Sds->Write_str( WAVFILESTR_KEY, str );
        Eventlog.add( SDS_ID, READ_EXTERNAL_WAVFILE);
    }
}

void MainWindow::cB_Capture( QString str )
{
	Sds->capture_flag = Eventlog.capture( SDS_ID, not Sds->capture_flag );
    (Sds->capture_flag) ? 	ui->cB_Capture->setCurrentText( QCapture_str_lst[Eventlog.CAPTURING] ):
    						ui->cB_Capture->setCurrentText( QCapture_str_lst[Eventlog.SPOOLING ] );
}

void MainWindow::adsr_decay()
{
	uint8_t value = ui->hs_adsr_sustain->value();
    Sds->Set( Sds->addr->OSC_adsr.decay , value);
    Eventlog.add( SDS_ID,ADSR_KEY);
}
void MainWindow::adsr_hall( )
{
	uint8_t value = ui->hs_hall_effect->value();
    Sds->Set(Sds->addr->OSC_adsr.hall , value);
    Eventlog.add( SDS_ID, ADSR_KEY);
}
void MainWindow::adsr_attack()
{
    uint8_t dial = ui->hs_adsr_attack->value();
    Sds->Set(Sds->addr->OSC_adsr.attack , dial);
    Eventlog.add( SDS_ID, ADSR_KEY);
}
void MainWindow::cB_Beat_per_sec( int bps_id )
{
	uint8_t bps_val = ui->cb_bps->currentText().toInt();
    Sds->Set( Sds->addr->OSC_adsr.bps, bps_val  );
	Eventlog.add( SDS_ID, ADSR_KEY );
}

void MainWindow::slideFrq( int value )
{
    Sds->Set(Sds->addr->OSC_wp.glide_effect , (uint8_t)value);
    Eventlog.add( SDS_ID, SOFTFREQUENCYKEY);

};


void MainWindow::dial_PMW_value_changed()
{
    uint8_t dial = ui->dial_PMW->value();
    Sds->Set(Sds->addr->VCO_wp.PMW_dial , dial);
    Eventlog.add( SDS_ID,PWMDIALKEY);
}

void MainWindow::Rtsp_Dialog()
{
    if ( this->Rtsp_Dialog_p->isVisible()   )
    {
        this->Rtsp_Dialog_p->hide();
    }
    else
    {
        Rtsp_Dialog_p->proc_table_update_all( );
        this->Rtsp_Dialog_p->show();
    }
}
void MainWindow::SDS_Dialog()
{
	if( Appstate->IsRunning( Sds_master, COMSTACKID ) )
	{
		Appstate->SetExitserver( Sds_master, COMSTACKID );
		Sem_p->Lock( SEMAPHORE_EXIT);
	}
	else
	{
		string Start_Comstack = Cfg_p->Server_cmd( Cfg_p->Config.Term, fs.comstack_bin, "" );
		system_execute( Start_Comstack );
	}
	return;

}

void MainWindow::File_Director()
{
    if ( this->File_Dialog_p->isVisible()   )
    {
        this->File_Dialog_p->hide();
    }
    else
    {
    	File_Dialog_p->Setup_widgets();
		this->File_Dialog_p->show();
    }
}

void MainWindow::Spectrum_Dialog()
{
    if ( this->Spectrum_Dialog_p->isVisible()   )
    {
        this->Spectrum_Dialog_p->hide();
    }
    else
    {
        Spectrum_Dialog_p->setGeometry(Spectrum_Dialog_Rect );
        this->Spectrum_Dialog_p->show();
    }
}


void MainWindow::waveform_slot(	uint8_t* wf_addr,
								uint8_t wfid,
								int ID,
								int wf_key,
								QLabel* label  )
{
	Sds->Set( *wf_addr, wfid );
	Eventlog.add( SDS_ID, wf_key);
	label->setText( QWaveform_vec[ wfid ] );
	this->Spectrum_Dialog_p->SetLabelWaveform( QWaveform_vec[ wfid ] );
}
void MainWindow::Main_Waveform_slot( int _wfid )
{
	waveform_slot( &Sds->addr->OSC_spectrum.wfid[0], _wfid, OSCID, SETWAVEFORMMAINKEY, ui->wf_OSC );
}
void MainWindow::FMO_Waveform_slot(int _wfid)
{
	waveform_slot( &Sds->addr->FMO_spectrum.wfid[0], _wfid, FMOID, SETWAVEFORMFMOKEY, ui->wf_fmo );
}
void MainWindow::VCO_Waveform_slot( int _wfid )
{
	waveform_slot( &Sds->addr->VCO_spectrum.wfid[0], _wfid, VCOID, SETWAVEFORMVCOKEY, ui->wf_vco );
}


void MainWindow::select_Sds( uint8_t sdsid ) // TODO working
{
	Sds->Set( this->Sds_master->config, sdsid );
	Sds->Set( this->Sds_master->UpdateFlag	, true);
	Sds->Set( this->Sds->addr->UpdateFlag	, true); // update old sds

	this->Sds = DaTA.SDS.GetSds( sdsid ); // set new sds
	Sds->Set( this->SDS_ID, sdsid );

	initGuiVectors();
	initStateButtons();

    Info( AppIdName( GUI_ID) + " set to SDS Id: " + to_string( (int) Sds->addr->SDS_Id ));

	File_Dialog_p->SetSds( this->Sds, sdsid );
	Spectrum_Dialog_p->SetSds( this->Sds, sdsid );

	Rtsp_Dialog_p->proc_table_update_row( sdsid + 1 );
	Rtsp_Dialog_p->SDS_ID = sdsid;

   	rb_S_vec[ Sds_master->config ]->setChecked( true );

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
		Sds->Set( Sds->addr->frq_slidermode, (uint8_t) COMBINE );
	else
		Sds->Set( Sds->addr->frq_slidermode, (uint8_t) SLIDE );
}

auto setStaPlay( MainWindow* M, uint8_t id )
{
    M->Sds->Set( M->Sds->addr->MIX_Id , id );
    bool play = not M->Sds->addr->StA_state[id].play;
    M->Sds->Set( M->Sds->addr->StA_state[id].play, play);
    M->Eventlog.add( M->SDS_ID, SETSTA_KEY );
    M->cb_play_sta_vec[ id ].cb->setChecked( play ) ;
}

void MainWindow::toggle_mute0(  )
{
	setStaPlay( this, 0 );
}
void MainWindow::toggle_mute1(  )
{
	setStaPlay( this, 1 );
}
void MainWindow::toggle_mute2(  )
{
	setStaPlay( this, 2 );
}
void MainWindow::toggle_mute3(  )
{
	setStaPlay( this, 3 );
}
void MainWindow::toggle_mute4(  )
{
	setStaPlay( this, STA_INSTRUMENT );
}
void MainWindow::toggle_mute5(  )
{
	setStaPlay( this, STA_KEYBOARD );
}
void MainWindow::toggle_mute6(  )
{
	setStaPlay( this, STA_NOTES );
}
void MainWindow::toggle_mute7(  )
{
	setStaPlay( this, STA_EXTERNAL );
}

auto toggle_store_sta( MainWindow* M, uint8_t ID )
{
    M->Sds->Set( M->Sds->addr->MIX_Id , ID );
    if ( M->Sds->addr->StA_state[ID].store )
    {
    	M->Eventlog.add( M->SDS_ID, STOPRECORD_KEY);
    }
    else
    {
        M->Eventlog.add( M->SDS_ID, STORESOUNDKEY);
    }
    M->cb_store_sta_vec[ ID ].cb->setChecked( false );
};

void MainWindow::toggle_store_sta0()
{
	toggle_store_sta( this, STA_USER00 );
}
void MainWindow::toggle_store_sta1()
{
	toggle_store_sta( this, STA_USER01 );
}
void MainWindow::toggle_store_sta2()
{
	toggle_store_sta( this, STA_USER02 );
}
void MainWindow::toggle_store_sta3()
{
	toggle_store_sta( this, STA_USER03 );
}
void MainWindow::toggle_store_sta4()
{
	toggle_store_sta( this, STA_INSTRUMENT );
}
void MainWindow::toggle_store_sta5()
{
	toggle_store_sta( this, STA_KEYBOARD );
}
void MainWindow::toggle_store_sta6()
{
	toggle_store_sta( this, STA_NOTES );
}
void MainWindow::toggle_store_sta7()
{
	toggle_store_sta( this, STA_EXTERNAL );
}


void MainWindow::memory_clear()
{
	for( uint id : StAMemIds )
	{
		*cb_filld_sta_vec[id].state = false;
	}
    Eventlog.add( SDS_ID, CLEAR_KEY );
}

void MainWindow::mixer_slider( sl_value_t map )
{

    Sds->Set( Sds->addr->MIX_Id , map.id );
    Sds->Set( *map.value, (uint8_t)map.sl->value() );
    Eventlog.add( SDS_ID, SETSTA_KEY);
};

void MainWindow::Sl_mix0( int value )
{
	mixer_slider( sl_sta_vec[0] );
};
void MainWindow::Sl_mix1( int value )
{
	mixer_slider( sl_sta_vec[1] );
};
void MainWindow::Sl_mix2( int value )
{
	mixer_slider( sl_sta_vec[2] );
};
void MainWindow::Sl_mix3( int value )
{
	mixer_slider( sl_sta_vec[3] );
};
void MainWindow::Sl_mix4( int value )
{
	mixer_slider( sl_sta_vec[4] );
};
void MainWindow::Sl_mix5( int value )
{
	mixer_slider( sl_sta_vec[5] );
};
void MainWindow::Sl_mix6( int value )
{
	mixer_slider( sl_sta_vec[6] );
};
void MainWindow::Sl_mix7( int value )
{
	mixer_slider( sl_sta_vec[7] );
};

void MainWindow::slideVol( int value )
{
    Sds->Set( Sds_master->slide_duration, (uint8_t)value); // % of 4*max_seconds
    Eventlog.add( 0, MASTERAMP_KEY);
}

void MainWindow::Clear_Banks()
{
	for( cb_state_t map : cb_play_sta_vec )
		map.cb->setChecked(false);
    Eventlog.add( SDS_ID, MUTEMBKEY);
}
void MainWindow::toggle_Mute()
{
    bool mute_flag 	= not Sds->addr->mixer_status.mute ;
    Eventlog.add( SDS_ID, MASTERAMP_MUTE_KEY);
    QString Qstr = mute_flag ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );
}

void MainWindow::setwidgetvalues()
{
	for ( int oscid : { VCOID, FMOID, OSCID } )
	{
		sl_volume_vec[oscid].sl->setValue( *sl_volume_vec[oscid].value );
		sl_volume_vec[oscid].lcd->display( *sl_volume_vec[oscid].value );

		sl_frqidx_vec[oscid].sl->setValue( *sl_frqidx_vec[oscid].value );
		sl_frqidx_vec[oscid].lcd->display( Spectrum.GetFrq( *sl_frqidx_vec[oscid].value ));

		sB_lbl_vec[oscid].lbl->setText( QWaveform_vec[ *sB_lbl_vec[oscid].value] );
		sB_lbl_vec[oscid].sb->setValue( *sB_lbl_vec[oscid].value );
	};

    ui->hs_adsr_sustain->setValue	( (int) Sds->addr->OSC_adsr.decay );
    ui->hs_adsr_attack->setValue	( (int) Sds->addr->OSC_adsr.attack);
    ui->dial_PMW->setValue			( (int) Sds->addr->VCO_wp.PMW_dial  );
    ui->hs_hall_effect->setValue	( (int) Sds->addr->OSC_adsr.hall );
    ui->Slider_slideFrq->setValue	( (int) Sds->addr->OSC_wp.glide_effect );
    ui->Slider_slideVol->setValue	( Sds_master->slide_duration);//Master_Amp);
    ui->hs_balance->setValue		( Sds->addr->mixer_balance );

    get_record_status();


    QString
	Qstr = Sds->addr->mixer_status.mute ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );

    Qstr	= int2char( Sds->addr->OSC_adsr.bps );
    ui->cb_bps->setCurrentText( Qstr );

	set_cb_psta_value( this );
	set_cb_ssta_value( this );
	set_sl_sta_value( this );
	set_cb_filled_value( this );

	ui->Pbar_telapsed->setValue( Sds->addr->time_elapsed );

	if ( Sds->addr->WD_status.roleId == osc_struct::AUDIOOUTID )
		OscWidget_item->sds = Sds_master;
	else
		OscWidget_item->sds = this->Sds->addr;

	if( Spectrum_Dialog_p->isVisible( ))
		Spectrum_Dialog_p->Update_spectrum();
	if( Rtsp_Dialog_p->isVisible() )
		Rtsp_Dialog_p->proc_table_update_all();


   	ui->cB_Capture->setCurrentText( QCapture_str_lst[ Eventlog.capture_state ] );

   	bool combine = ( Sds->addr->frq_slidermode == COMBINE );
   	ui->cB_Combine->setChecked( combine );
   	//    rb_S_vec[ Sds->addr->SDS_Id ]->setChecked( true );

    ui->cb_connect_fmo->setChecked ( Sds->addr->connect[FMOID].vol );
    ui->cb_connect_vco->setChecked ( Sds->addr->connect[VCOID].vol );
    ui->cb_connect_oscv->setChecked( Sds->addr->connect[OSCID].vol );
    ui->cb_connect_oscf->setChecked( Sds->addr->connect[OSCID].frq );

    double frq_slide_duration = Sds->addr->OSC_wp.glide_effect * max_sec * 0.01;
    ui->lbl_frqglide_sec->setText( QString::number( frq_slide_duration ) + "\n[sec[");
    updateColorButtons();
	Appstate->SetRunning(  );

}

void MainWindow::sliderFreq( sl_lcd_t map, uint8_t value )
{
	float freq 		= Spectrum.GetFrq( value );
	map.lcd->display(  freq  );
	Sds->Set( *map.value, value );

	Eventlog.add( SDS_ID, map.event );
};
void MainWindow::Slider_OSC_Freq( int value )
{
	sliderFreq( sl_frqidx_vec[OSCID], value );
}
void MainWindow::Slider_VCO_Freq( int value )
{
	( (uint)value < C0 ) ? ui->lb_VCO_LFO->show() : ui->lb_VCO_LFO->hide();

	sliderFreq( sl_frqidx_vec[VCOID], value );
	Sds->Set( Sds->addr->connect[OSCID].vol, true );

}
void MainWindow::Slider_FMO_Freq( int value )
{
	( (uint)value < C0 ) ? ui->lb_FMO_LFO->show() : ui->lb_FMO_LFO->hide();

	sliderFreq( sl_frqidx_vec[FMOID], value );
	Sds->Set( Sds->addr->connect[OSCID].frq, true );

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
	Sds->Set( Sds->addr->connect[osc_struct::OSCID].vol, true );

}
void MainWindow::FMO_slot_volume()
{
	sliderVolume( sl_volume_vec[ FMOID ] );
	Sds->Set( Sds->addr->connect[osc_struct::OSCID].frq, true );

}

void MainWindow::Slider_VCO_Adjust( int value )
{
	Sds->Set( Sds->addr->VCO_wp.adjust, (uint8_t) value);
	Eventlog.add( SDS_ID, ADJUST_KEY);
}
void MainWindow::Slider_FMO_Adjust( int value )
{
	Sds->Set( Sds->addr->FMO_wp.adjust, (uint8_t) value);
	Eventlog.add( SDS_ID, ADJUST_KEY );
}

void MainWindow::start_composer()
{

    string Start_Composer = Cfg_p->Server_cmd( Cfg_p->Config.Term, fs.composer_bin, "" );
	system_execute( Start_Composer );
}


void MainWindow::start_audio_srv()
{
	if( Appstate->IsRunning( Sds_master, RTSPID ) )
		return;
	if( Appstate->IsRunning( Sds_master, AUDIOID))
	{
		Appstate->SetExitserver( Sds_master, AUDIOID) ;
		return;
	}

    string Start_Audio_Srv = Cfg_p->Server_cmd( Cfg_p->Config.Nohup, fs.audio_bin,
			" > " + fs.nohup_file);

	system_execute( Start_Audio_Srv.data() );

}

auto start_synth = [  ]( MainWindow* M, string cmd )
{
	if( M->Appstate->IsRunning( M->Sds_master, RTSPID ) )
	{
	    if ( M->Sem_p->Getval( SYNTHESIZER_START, GETVAL ) > 0 )
	    	M->Sem_p->Release( SYNTHESIZER_START );
	    return M->SDS_ID;
	}

	int sdsid = M->DaTA.Reg.GetStartId( );
	if ( sdsid < 0 ) return M->SDS_ID;

    system_execute( cmd.data() );
    return (uint8_t) sdsid;

};
void MainWindow::start_synthesizer()
{
	if (Appstate->IsInconsistent( Sds->addr, SYNTHID ))
		return;
	if( Appstate->IsRunning( Sds->addr, SYNTHID ) )
	{
		exit_synthesizer();
		return;
	}
	string cmd = Cfg_p->Server_cmd( Cfg_p->Config.Nohup, fs.synth_bin,
			" >> " + fs.nohup_file );

    uint8_t sdsid = start_synth( this, cmd );
    select_Sds(sdsid);

}
void MainWindow::start_keyboard()
{
	string cmd = Cfg_p->Server_cmd( Cfg_p->Config.Term, fs.synth_bin, "" );
	int sdsid = start_synth( this, cmd );
	select_Sds(sdsid);
}
void MainWindow::read_polygon_data()
{
    OscWidget_item->read_polygon_data();
};

void MainWindow::exit_synthesizer()
{
	if ( Appstate->IsRunning( Sds_master, RTSPID ) )
	{
		if ( Sem_p->Getval( SEMAPHORE_EXIT, GETVAL ) > 0 )
			Sem_p->Release( SEMAPHORE_EXIT );
		return;
	}
	Appstate->SetExitserver( Sds->addr, SYNTHID );
    DaTA.Sem_p->Lock( SEMAPHORE_EXIT, 2 );
	setwidgetvalues();
}



void MainWindow::Save_Config()
{
    Sds->Write_str( INSTRUMENTSTR_KEY, "default");
	Eventlog.add( SDS_ID, SAVEINSTRUMENTKEY);
}

void MainWindow::connect_oscf( bool val ) // TODO no yet ready
{
	Sds->Set( Sds->addr->connect[OSCID].frq, val );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
}
void MainWindow::connect_oscv( bool val )
{
	Sds->Set( Sds->addr->connect[OSCID].vol, val );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
}
void MainWindow::connect_fmo( bool val )
{
	Sds->Set( Sds->addr->connect[FMOID].vol, val );
	Eventlog.add( SDS_ID, CONNECTFMO_KEY );
}
void MainWindow::connect_vco( bool val )
{	//connect VCO volume with FMO data
	Sds->Set( Sds->addr->connect[VCOID].vol, val );
	Eventlog.add( SDS_ID, CONNECTVCO_KEY );
}

void MainWindow::get_record_status( )
{
    int pb_value = Sds_master->RecCounter;
    ui->progressBar_record->setValue( pb_value );
}
void MainWindow::SaveRecord()
{
    Sds->Set( Sds_master->FileNo ,(uint8_t) 0); // 0=automatic numbering

    if ( Sds_master->Record )
    {
    	Appstate->Set( Sds_master, AUDIOID, sdsstate_struct::RECORDSTOP );
    }
    else
    {
    	Appstate->Set( Sds_master, AUDIOID, sdsstate_struct::RECORDSTART );
    }

}


void MainWindow::pB_Debug_clicked()
{
    uint8_t counter = ( Sds->addr->WD_status.wd_mode + 1 ) % WD_MODE_SIZE;
    Sds->Set( Sds->addr->WD_status.wd_mode , counter);
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );

    ui->pB_wd_mode->setText( Qwd_wdmode_names[ (int)counter ] );
}

void MainWindow::pB_oscgroup_clicked()
{
    uint8_t counter = ( Sds->addr->WD_status.oscId + 1 ) % WD_OSC_SIZE;
    Sds->Set( Sds->addr->WD_status.oscId, counter );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY);

    ui->pB_oscgroup->setText( Qwd_osc_names[ (int)counter ] );
}

void MainWindow::pB_Wavedisplay_clicked()
{
    uint8_t counter = (Sds->addr->WD_status.roleId + 1) % WD_ROLES_SIZE;
    Sds->Set( Sds->addr->WD_status.roleId , counter);
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY);

    ui->pB_Wavedisplay->setText( Qwd_display_names[ (int)counter ] );
};

void MainWindow::pB_fftmode_clicked()
{
	bool fft_mode = not (Sds->addr->WD_status.fftmode );
	Sds->Set( Sds->addr->WD_status.fftmode, fft_mode );
	Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );
	cout << "pB_fftmode_clicked" << boolalpha << Sds->addr->WD_status.fftmode << endl;

	ui->pb_fftmode->setText( Qwd_fftmodes[ (int)fft_mode ] );
}

void MainWindow::update_CB_external()
{
	Info( "update recording info");
	CB_external->clear();
	CB_external->addItems( Qread_filenames( EventWAV.path ));
	QString Qfile	= QReadStr( Sds, EventWAV.event );
	CB_external->setCurrentText( Qfile );
}

void MainWindow::updateColorButtons()
{

    setButton( ui->pBAudioServer, Appstate->IsRunning( Sds_master, AUDIOID ) +1);
    setButton( ui->pb_SDSview	, Appstate->IsRunning( Sds_master, COMSTACKID )+1 );
    setButton( ui->pBSynthesizer, Appstate->IsRunning( Sds->addr, SYNTHID )+1 );
    setButton( ui->pBComposer	, Appstate->IsRunning( Sds_master, COMPID )+1 );

    bool record = ( Sds_master->Record);
    setButton( ui->pBtoggleRecord, not record );
    ( record ) ? 	ui->pBtoggleRecord->setText( "Stop Rec") :
    				ui->pBtoggleRecord->setText( "Record");

    if( Rtsp_Dialog_p->isVisible() )
    	setButton( ui->pB_Rtsp, 2 );
    else
        setButton( ui->pB_Rtsp, 1 );

    if( File_Dialog_p->isVisible() )
        setButton( ui->pB_play_notes, 2 );
    else
        setButton( ui->pB_play_notes, 1 );

    if( Spectrum_Dialog_p->isVisible() )
        setButton( ui->pB_Specrum, 2 );
    else
        setButton( ui->pB_Specrum, 1 );
}
