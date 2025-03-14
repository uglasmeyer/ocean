
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

auto set_rb_sta_value = [ ]( MainWindow* M )
{
    for( MainWindow::rb_state_t map : M->rb_sta_vec )
    	map.rb->setChecked( *map.state);
};
auto set_cb_sta_value = [ ]( MainWindow* M )
{
    for( MainWindow::cb_state_t map : M->cb_sta_vec )
    	map.cb->setChecked( *map.state );
};
auto set_sl_sta_value = [ ]( MainWindow* M )
{
    for( MainWindow::sl_value_t map : M->sl_sta_vec )
    	map.sl->setValue( *map.value );
};



MainWindow::MainWindow(	QWidget *parent ) :
	Logfacility_class( Module )
	,	ui(new Ui::MainWindow{} )
{
	ui->setupUi(this);
	initPanel();

	Sds->Set(Sds_master->UpdateFlag, true);
	Sds->Set( Sds->addr->UpdateFlag, true);
	Sds->Set( Sds_master->Record, false);
	DaTA->Master_Sds_p->Announce( );

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

void MainWindow::wavfile_selected( const QString &arg)
{
    qDebug() << "WAV file " << arg ;
    QString QStr = arg;
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        Sds->Write_str( WAVEFILESTR_KEY, str );
        Eventlog.add( SDS_ID, READ_EXTERNALWAVEFILE);
    }
}

void MainWindow::cB_Capture( QString str )
{
	Sds->capture_flag = Eventlog.capture( SDS_ID, not Sds->capture_flag );
    (Sds->capture_flag) ? 	ui->cB_Capture->setCurrentText( QCapture_str_lst[Eventlog.CAPTURING] ):
    						ui->cB_Capture->setCurrentText( QCapture_str_lst[Eventlog.SPOOLING ] );
}

void MainWindow::hs_hall_effect_value_changed( int value)
{
    Sds->Set(Sds->addr->OSC_adsr.hall , (uint8_t) value);
    Eventlog.add( SDS_ID, ADSR_KEY);
}


void MainWindow::dial_soft_freq_value_changed()
{
    uint8_t value = ui->dial_soft_freq->value();
    Sds->Set(Sds->addr->OSC_wp.glide_effect , value);
    Eventlog.add( SDS_ID, SOFTFREQUENCYKEY);

};

void MainWindow::dial_decay_value_changed()
{
    uint8_t dial = ui->hs_adsr_attack->value();
    Sds->Set(Sds->addr->OSC_adsr.attack , dial);
    Eventlog.add( SDS_ID, ADSR_KEY);

}
void MainWindow::dial_PMW_value_changed()
{
    uint8_t dial = ui->dial_PMW->value();
    Sds->Set(Sds->addr->VCO_wp.PMW_dial , dial);
    Eventlog.add( SDS_ID,PMWDIALKEY);
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
    string Start_Comstack = Cfg->Server_cmd( Cfg->Config.Term, fs.comstack_bin, "" );
	system_execute( Start_Comstack.data() );
	return;

	/*
    if ( this->SDS_Dialog_p->isVisible()   )
    {
        this->SDS_Dialog_p->hide();
    }
    else
    {
        this->SDS_Dialog_p->show();
    }
    */
}

void MainWindow::File_Director()
{
    if ( this->File_Dialog_p->isVisible()   )
        this->File_Dialog_p->hide();
    else
    {
    	File_Dialog_p->Setup_widgets();
		this->File_Dialog_p->show();
    }
}

void MainWindow::Spectrum_Dialog()
{
    if ( this->Spectrum_Dialog_p->isVisible()   )
        this->Spectrum_Dialog_p->hide();
    else
    {
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
	Sds->Set( this->Sds_master->UpdateFlag, true);
	Sds->Set( this->Sds->addr->UpdateFlag, true); // update old sds

	Sds->Set( this->SDS_ID, sdsid );
	this->Sds = DaTA->GetSds( sdsid ); // set new sds

	initGuiVectors();
	initStateButtons();

    Info( Type_map( GUI_ID) + " set to SDS Id: " + to_string( (int) Sds->addr->SDS_Id ));
	DaTA->Reg.Show_proc_register( sdsid );

	File_Dialog_p->SetSds( this->Sds, sdsid );
	Spectrum_Dialog_p->SetSds( this->Sds, sdsid );

	Rtsp_Dialog_p->proc_table_update_row( sdsid + 1 );
	Rtsp_Dialog_p->SDS_ID = sdsid;


};

void MainWindow::select_Sds0()
{
	select_Sds( 0);
	setwidgetvalues();
}
void MainWindow::select_Sds1()
{
	select_Sds( 1);
	setwidgetvalues();
}
void MainWindow::select_Sds2()
{
	select_Sds( 2);
	setwidgetvalues();
}
void MainWindow::select_Sds3()
{
	select_Sds( 3);
	setwidgetvalues();
}

auto setStaPlay( MainWindow* M, uint8_t id )
{
    M->Sds->Set( M->Sds->addr->MIX_Id , id );
    bool play = not M->Sds->addr->StA_state[id].play;
    M->Sds->Set( M->Sds->addr->StA_state[id].play, play);
    M->Eventlog.add( M->SDS_ID, SETSTAPLAY_KEY );
    M->cb_sta_vec[ id ].cb->setChecked( play ) ;
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
	setStaPlay( this, MbIdInstrument );
}
void MainWindow::toggle_mute5(  )
{
	setStaPlay( this, MbIdKeyboard );
}
void MainWindow::toggle_mute6(  )
{
	setStaPlay( this, MbIdNotes );
}
void MainWindow::toggle_mute7(  )
{
	setStaPlay( this, MbIdExternal );
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
    M->rb_sta_vec[ ID ].rb->setChecked( false );
};

void MainWindow::toggle_store_sta0()
{
	toggle_store_sta( this, 0 );
}
void MainWindow::toggle_store_sta1()
{
	toggle_store_sta( this, 1 );
}
void MainWindow::toggle_store_sta2()
{
	toggle_store_sta( this, 2 );
}
void MainWindow::toggle_store_sta3()
{
	toggle_store_sta( this, 3 );
}
void MainWindow::toggle_store_sta4()
{
	toggle_store_sta( this, 4 );
}
void MainWindow::toggle_store_sta5()
{
	toggle_store_sta( this, 5 );
}
void MainWindow::toggle_store_sta6()
{
	toggle_store_sta( this, 6 );
}
void MainWindow::toggle_store_sta7()
{
	toggle_store_sta( this, MbIdExternal );
}


void MainWindow::memory_clear()
{
	uint8_t id = 0;
    for ( rb_state_t map : rb_sta_vec )
    {
    	if ( map.rb->isChecked() )
    	{
    		Sds->Set( Sds->addr->MIX_Id, id);
    		map.rb->setChecked( false );
    	}
    	id++;
    }
    Eventlog.add( SDS_ID, CLEAR_KEY );
}

void MainWindow::mixer_slider( sl_value_t map )
{

    Sds->Set( Sds->addr->MIX_Id , map.id );
    Sds->Set( *map.value, (uint8_t)map.sl->value() );
    Eventlog.add( SDS_ID, SETMBAMPPLAYKEY);
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

void MainWindow::dial_glide_volume( int value )
{
    Sds->Set( Sds_master->slide_duration, (uint8_t)value); // % of 4*max_seconds
    Eventlog.add( 0, MASTERAMP_KEY);
}

void MainWindow::Clear_Banks()
{
	for( cb_state_t map : cb_sta_vec )
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

void MainWindow::cB_Beat_per_sec( int bps_id )
{
	uint8_t bps_val = ui->cb_bps->currentText().toInt();
    Sds->Set( Sds->addr->OSC_adsr.bps, bps_val  );
	Eventlog.add( SDS_ID, ADSR_KEY );
}

void MainWindow::setwidgetvalues()
{
	for ( int oscid : {VCOID, FMOID, OSCID } )
	{
		sl_volume_vec[oscid].sl->setValue( *sl_volume_vec[oscid].value );
		sl_volume_vec[oscid].lcd->display( *sl_volume_vec[oscid].value );

		sl_frqidx_vec[oscid].sl->setValue( *sl_frqidx_vec[oscid].value );
		sl_frqidx_vec[oscid].lcd->display( Spectrum.Calc( *sl_frqidx_vec[oscid].value ));

		sB_lbl_vec[oscid].lbl->setText( QWaveform_vec[ *sB_lbl_vec[oscid].value] );
		sB_lbl_vec[oscid].sb->setValue( *sB_lbl_vec[oscid].value );
	};

    ui->hs_adsr_sustain->setValue(  (int)Sds->addr->OSC_adsr.decay );
    ui->hs_adsr_attack->setValue(  (int) Sds->addr->OSC_adsr.attack);
    ui->dial_PMW->setValue( (int)Sds->addr->VCO_wp.PMW_dial  );
    ui->dial_soft_freq->setValue( (int)  Sds->addr->OSC_wp.glide_effect );
    ui->hs_hall_effect->setValue( (int)  Sds->addr->OSC_adsr.hall );

    get_record_status();

    ui->dial_glide_vol->setValue( Sds->addr->slide_duration);//Master_Amp);

    QString Qstr = Sds->addr->mixer_status.mute ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );

    ui->cb_bps->setCurrentText( QString( int2char( Sds->addr->OSC_adsr.bps)));

	set_cb_sta_value( this );
	set_sl_sta_value( this );
	set_rb_sta_value( this );

	ui->Pbar_telapsed->setValue( Sds->addr->time_elapsed );

	if ( Sds->addr->WD_status.roleId == osc_struct::AUDIOID )
		OscWidget_item->sds = Sds_master;
	else
		OscWidget_item->sds = this->Sds->addr;
	if( Spectrum_Dialog_p->isVisible( ))
		Spectrum_Dialog_p->Update_spectrum();
	if( Rtsp_Dialog_p->isVisible() )
		Rtsp_Dialog_p->proc_table_update_all();

    Sds->Set( Sds->addr->UserInterface 	, (uint8_t)RUNNING );

   	ui->cB_Capture->setCurrentText( QCapture_str_lst[ Eventlog.capture_state ] );
}

void MainWindow::sliderFreq( sl_lcd_t map, uint8_t value )
{
	float freq 		= Spectrum.Calc( value );
	map.lcd->display(  freq  );

	uint diff = abs(value - *map.value);
	if ( diff > 1 )
		Sds->Set( Sds->addr->slidermode, (uint8_t)SLIDE);
	else
		Sds->Set( Sds->addr->slidermode, (uint8_t)STEP);
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
}
void MainWindow::Slider_FMO_Freq( int value )
{
	( (uint)value < C0 ) ? ui->lb_FMO_LFO->show() : ui->lb_FMO_LFO->hide();

	sliderFreq( sl_frqidx_vec[FMOID], value );
}


void MainWindow::sliderVolume( sl_lcd_t map )
{
	uint8_t value = map.sl->value();
	Sds->Set( *map.value, value);
	map.lcd->display( value );
//	uint diff = abs(value - *map.value);
//	if ( diff > 1 )
	Sds->Set( Sds_master->vol_slidemode, (uint8_t)SLIDE);
//	else
//		Sds->Set( Sds_master->LOOP_step, (uint8_t)STEP);
	Eventlog.add( SDS_ID, map.event);
};

void MainWindow::MAIN_slot_volume()
{
	sliderVolume( sl_volume_vec[ OSCID ] );
}
void MainWindow::VCO_slot_volume()
{
	sliderVolume( sl_volume_vec[ VCOID ] );
}
void MainWindow::FMO_slot_volume()
{
	sliderVolume( sl_volume_vec[ FMOID ] );
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
    string Start_Composer = Cfg->Server_cmd( Cfg->Config.Term, fs.composer_bin, "" );
	system_execute( Start_Composer.data() );
}

void MainWindow::start_audio_srv()
{
	if( Sds->addr->Rtsp == RUNNING )
		return;
    string Start_Audio_Srv = Cfg->Server_cmd( Cfg->Config.Term,
    		fs.audio_bin,
			"-S 0");
	system_execute( Start_Audio_Srv.data() );
    Sem->Lock( SEMAPHORE_STARTED, 2 );
	setwidgetvalues();
}

void MainWindow::start_synthesizer()
{
	if( Sds->addr->Rtsp == RUNNING )
	{
	    if ( Sem->Getval( SYNTHESIZER_START, GETVAL ) > 0 )
	    	Sem->Release( SYNTHESIZER_START );
	    return;
	}

	int sdsid = DaTA->Reg.GetStartId( );
	if ( sdsid < 0 ) return;

	string Start_Synthesizer = Cfg->Server_cmd( Cfg->Config.Term,
    		fs.synth_bin,
			"-S " + to_string( sdsid ) );

    system_execute( Start_Synthesizer.data() );
    Sem->Lock( SEMAPHORE_STARTED, 2 );
    select_Sds(sdsid);
	setwidgetvalues();
}

void MainWindow::read_polygon_data()
{
    OscWidget_item->read_polygon_data();
};

void MainWindow::Controller_Exit()
{
	if ( Sds->addr->Rtsp == RUNNING )
	{
		if ( Sem->Getval( SEMAPHORE_EXIT, GETVAL ) > 0 )
			Sem->Release( SEMAPHORE_EXIT );
		return;
	}
    Sds->Set( Sds->addr->Synthesizer , (uint8_t) EXITSERVER );
    DaTA->Sem.Lock( SEMAPHORE_EXIT, 2 );
	setwidgetvalues();
}

void MainWindow::Audio_Exit()
{
    Sds->Set( Sds_master->AudioServer, (uint8_t) EXITSERVER);
    DaTA->Sem.Lock( SEMAPHORE_EXIT, 2 );
	setwidgetvalues();
}

void MainWindow::Save_Config()
{
    Sds->Write_str( INSTRUMENTSTR_KEY, "default");
	Eventlog.add( SDS_ID, SAVEINSTRUMENTKEY);
}

void MainWindow::set_mode_f()
{
	Eventlog.add( SDS_ID, RESETFMOKEY );
}

void MainWindow::set_mode_v()
{
    Eventlog.add( SDS_ID, RESETVCOKEY); //
}
void MainWindow::set_mode_o()
{
    Eventlog.add( SDS_ID, RESETMAINKEY); //
}

void MainWindow::connect_fmo()
{
    Eventlog.add( SDS_ID, CONNECTFMOVCOKEY);
}

void MainWindow::connect_vco()
{
    Eventlog.add( SDS_ID, CONNECTVCOFMOKEY);
}

void MainWindow::get_record_status( )
{
    int pb_value = Sds->addr->RecCounter;
    ui->progressBar_record->setValue( pb_value );
}
void MainWindow::SaveRecord()
{
    Sds->Set( Sds_master->FileNo ,(uint8_t) 0); // 0=automatic numbering

    if ( Sds_master->Record )
    	Sds_master->AudioServer = RECORDSTOP;
    else
    {
    	Sds_master->AudioServer = RECORDSTART;
    }

}

void MainWindow::main_adsr_sustain()
{
	uint8_t value = ui->hs_adsr_sustain->value();
    Sds->Set( Sds->addr->OSC_adsr.decay , value);
    Eventlog.add( SDS_ID,ADSR_KEY);
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


void MainWindow::updateWidgets()
{
    if (  Sds_master->UserInterface == UPDATEGUI  )
    {
        if ( not Sds_master->Composer )
        {
			switch( Sds->addr->FLAG )
			{
				case RECORDWAVFILEFLAG :
				{
					Info( "update recording info");
					CB_external->clear();
					CB_external->addItems( Qread_filenames( EventWAV.path ));
					QString Qfile	= QReadStr( Sds, EventWAV.event );
					CB_external->setCurrentText( Qfile );
					break;
				}
				case NEWINSTRUMENTFLAG :
				{
					File_Dialog_p->Setup_widgets();
					break;
				}
				case NEWNOTESLINEFLAG :
				{
					File_Dialog_p->Setup_widgets();
					break;
				}
			}
        }
    }

    Sds->Set( Sds->addr->UserInterface, (uint8_t) OFFLINE );

    bool a_running = ( Sds_master->AudioServer == RUNNING );
    setButton( ui->pBAudioServer, a_running );

    bool s_running = ( Sds->addr->Synthesizer == RUNNING );
    setButton( ui->pBSynthesizer, s_running );

    bool record = ( Sds_master->Record);
    setButton( ui->pBtoggleRecord, not record );
    if( record )
    {
    	ui->pBtoggleRecord->setText( "Stop Rec");
    }
    else
    {
    	ui->pBtoggleRecord->setText( "Record");
    }

    setwidgetvalues();

}
