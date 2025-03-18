/*
 * connectors.cpp
 *
 *  Created on: Feb 12, 2025
 *      Author: sirius
 */

#include <Mainwindow.h>

void MainWindow::initPanel()
{
    // https://stackoverflow.com/questions/17095957/qt-creator-and-main-window-background-image
    QString Ocean_png = QString::fromStdString( fs.Dir.libdir + "Ocean.png" );
    QPixmap bkgnd( Ocean_png );
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);

    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Button, QColor(0,179,255) );
    this->setPalette(palette);

    QRect rect = Spectrum_Dialog_p->geometry();  //get current geometry of help window
    QRect parentRect = this->geometry();      //get current geometry of this window
    rect.moveTo(mapToGlobal(QPoint(parentRect.x() + parentRect.width() - rect.width(), parentRect.y())));
    Spectrum_Dialog_p ->setGeometry(rect);

    rect = File_Dialog_p->geometry();  //get current geometry of help window
    parentRect = this->geometry();      //get current geometry of this window
    rect.moveTo(mapToGlobal(QPoint(parentRect.x() + parentRect.width() - rect.width(), parentRect.y())));
    File_Dialog_p->setGeometry(rect);
}

void MainWindow::initComboBoxes()
{
    CB_external         = ui->cb_external;
    string wavfile_path = fs.Dir.musicdir;
    CB_external->clear();

    Path_t path { wavfile_path, fs.wav_type } ;
    CB_external->addItems( Qread_filenames(	path ) );
    QString Qfile	= QReadStr( Sds, OTHERSTR_KEY );
    CB_external->setCurrentText( Qfile );

	Qbps_str_lst 		= Qstringlist( bps_struct().Bps_lst );
    ui->cb_bps->addItems( Qbps_str_lst );

    QCapture_str_lst	= Qstringlist( {"Capture", "capturing", "Spool", "spooling" } );
    ui->cB_Capture->addItems( QCapture_str_lst );
    ui->cB_Capture->setCurrentText( QCapture_str_lst[0] );
}

void MainWindow::initOscillatorDisplay()
{
    QRectF rect         = ui->oscilloscope_view->geometry();
    OszilloscopeWidget	OscWidg( Sds->addr, rect );
    OscWidget_item = new OszilloscopeWidget( Sds->addr, rect ) ;

    ui->oscilloscope_view->setScene( scene );
    scene->addItem( OscWidget_item );
}

void MainWindow::initStateButtons()
{
	Qwd_osc_names 	= Vstringvector( osc_struct().types );
	ui->pB_oscgroup->setText( Qwd_osc_names[ Sds->addr->WD_status.oscId ]);

	Qwd_fftmodes	= Vstringvector( wavedisplay_struct().fftmodes );
	ui->pb_fftmode->setText( Qwd_fftmodes [ Sds->addr->WD_status.fftmode ]);

	Qwd_wdmode_names= Vstringvector( wavedisplay_struct().types );
	ui->pB_wd_mode->setText( Qwd_wdmode_names[ Sds->addr->WD_status.wd_mode ]);

	Qwd_display_names 	= Vstringvector( osc_struct().roles );
	ui->pB_Wavedisplay->setText( Qwd_display_names[ Sds->addr->WD_status.roleId ]);
}

void MainWindow::initScrollbars()
{
	QWaveform_vec 		= Vstringvector( waveform_str_vec);
    uint sb_max = QWaveform_vec.size()-1;

    ui->sB_OSC->setMaximum(sb_max);
    ui->sB_FMO->setMaximum(sb_max);
    ui->sB_VCO->setMaximum(sb_max);
}

void MainWindow::initFreqSlider()
{
	for( sl_lcd_t map : sl_frqidx_vec )
	{
		map.sl->setMinimum( 1 );
		map.sl->setMaximum( freqarr_range.max );
		map.sl->setValue( Spectrum.Calc( *map.value));
	}
	if ( Sds->addr->slidermode == COMBINE )
		ui->cB_Combine->setChecked( true );
}

void MainWindow::initGuiVectors()
{

    rb_S_vec =
    {
    	ui->rb_S0,
		ui->rb_S1,
		ui->rb_S2,
		ui->rb_S3
    };
    rb_sta_vec =
    {
    	{ 0, ui->rb_sta0, &Sds->addr->StA_state[0].store },
		{ 1, ui->rb_sta1, &Sds->addr->StA_state[1].store },
		{ 2, ui->rb_sta2, &Sds->addr->StA_state[2].store },
		{ 3, ui->rb_sta3, &Sds->addr->StA_state[3].store },
		{ 4, ui->rb_sta4, &Sds->addr->StA_state[4].store },
		{ 5, ui->rb_sta5, &Sds->addr->StA_state[5].store },
		{ 6, ui->rb_sta6, &Sds->addr->StA_state[6].store },
		{ 7, ui->rb_sta7, &Sds->addr->StA_state[7].store }
    };
    cb_sta_vec =
    {
		{ 0, ui->cb_sta0, &Sds->addr->StA_state[0].play  },
		{ 1, ui->cb_sta1, &Sds->addr->StA_state[1].play  },
		{ 2, ui->cb_sta2, &Sds->addr->StA_state[2].play  },
		{ 3, ui->cb_sta3, &Sds->addr->StA_state[3].play  },
		{ 4, ui->cb_sta4, &Sds->addr->StA_state[4].play  },
		{ 5, ui->cb_sta5, &Sds->addr->StA_state[5].play  },
		{ 6, ui->cb_sta6, &Sds->addr->StA_state[6].play  },
		{ 7, ui->cb_sta7, &Sds->addr->StA_state[7].play  }
	};
    sl_sta_vec =
    {
		{ 0, ui->Slider_mix_vol0, &Sds->addr->StA_amp_arr[0]  },
		{ 1, ui->Slider_mix_vol1, &Sds->addr->StA_amp_arr[1]  },
		{ 2, ui->Slider_mix_vol2, &Sds->addr->StA_amp_arr[2]  },
		{ 3, ui->Slider_mix_vol3, &Sds->addr->StA_amp_arr[3]  },
		{ 4, ui->Slider_mix_vol4, &Sds->addr->StA_amp_arr[4]  },
		{ 5, ui->Slider_mix_vol5, &Sds->addr->StA_amp_arr[5]  },
		{ 6, ui->Slider_mix_vol6, &Sds->addr->StA_amp_arr[6]  },
		{ 7, ui->Slider_mix_vol7, &Sds->addr->StA_amp_arr[7]  }
	};
    sl_frqidx_vec =
    {
   		{ VCOFREQUENCYKEY, ui->VCOLCD_Hz, ui->Slider_VCO_Hz, &Sds->addr->VCO_wp.frqidx },
   		{ FMOFREQUENCYKEY, ui->FMOLCD_Hz, ui->Slider_FMO_Hz, &Sds->addr->FMO_wp.frqidx },
   		{ OSCFREQUENCYKEY, ui->OSCLCD_Hz, ui->Slider_OSC_Hz, &Sds->addr->OSC_wp.frqidx }
    };
    sl_volume_vec =
    {
   		{ VCOAMPKEY, 	ui->VCOLCD_Amp, ui->Slider_VCO_vol, &Sds->addr->VCO_wp.volume },
   		{ FMOAMPKEY, 	ui->FMOLCD_Amp, ui->Slider_FMO_vol, &Sds->addr->FMO_wp.volume },
   		{ MASTERAMP_KEY,ui->OSCLCD_Amp, ui->Slider_OSC_Vol, &Sds_master->Master_Amp }
    };
    sB_lbl_vec =
    {
        { VCOFREQUENCYKEY, ui->sB_VCO, ui->wf_vco, &Sds->addr->VCO_spectrum.wfid[0] },
       	{ FMOFREQUENCYKEY, ui->sB_FMO, ui->wf_fmo, &Sds->addr->FMO_spectrum.wfid[0] },
       	{ OSCFREQUENCYKEY, ui->sB_OSC, ui->wf_OSC, &Sds->addr->OSC_spectrum.wfid[0] }
    };
}

void MainWindow::initUiConnectors()
{
    connect(ui->pB_Rtsp			, SIGNAL(clicked() )		,this, SLOT(Rtsp_Dialog() ));
    connect(ui->pb_SDSview		, SIGNAL(clicked() )		,this, SLOT(SDS_Dialog() ));
    connect(ui->pB_Specrum		, SIGNAL(clicked() )		,this, SLOT(Spectrum_Dialog() ));
    connect(ui->pB_play_notes	, SIGNAL(clicked() )		,this, SLOT(File_Director() ));

    connect(ui->pBSynthesizer	, SIGNAL(clicked() )		,this, SLOT(start_synthesizer() ));
    connect(ui->pBSynthesizerExit,SIGNAL(clicked() )		,this, SLOT(Controller_Exit() ));
    connect(ui->pBAudioServer	, SIGNAL(clicked() )		,this, SLOT(start_audio_srv() ));
    connect(ui->pBAudioServerExit,SIGNAL(clicked() )		,this, SLOT(Audio_Exit() ));
    connect(ui->pBComposer		, SIGNAL(clicked() )		,this, SLOT(start_composer() ));
    connect(ui->pBGuiExit		, SIGNAL(clicked() )		,this, SLOT(GUI_Exit() ));
    connect(ui->pBtoggleRecord	, SIGNAL(clicked(bool) )	,this, SLOT(SaveRecord() ));
    connect(ui->pB_Mute			, SIGNAL(clicked() )		,this, SLOT(toggle_Mute() ));
    connect(ui->pB_Save			, SIGNAL(clicked() )		,this, SLOT(Save_Config() ));
    connect(ui->pb_clear		, SIGNAL(clicked() )		,this, SLOT(memory_clear() ));

    connect(ui->Slider_VCO_Hz	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_VCO_Freq(int) ));
    connect(ui->Slider_FMO_Hz	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_FMO_Freq(int) ));
    connect(ui->Slider_OSC_Hz	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_OSC_Freq(int) ));

    connect(ui->dial_soft_freq	, SIGNAL(valueChanged(int) ),this, SLOT(dial_soft_freq_value_changed() ));
    connect(ui->dial_PMW      	, SIGNAL(valueChanged(int) ),this, SLOT(dial_PMW_value_changed() ));
    connect(ui->dial_glide_vol	, SIGNAL(valueChanged(int) ),this, SLOT(dial_glide_volume(int)) );

    connect(ui->hs_adsr_attack	, SIGNAL(valueChanged(int) ),this, SLOT(dial_decay_value_changed() ));
    connect(ui->hs_adsr_sustain	, SIGNAL(valueChanged(int) ),this, SLOT(main_adsr_sustain() ));
    connect(ui->hs_hall_effect	, SIGNAL(valueChanged(int) ),this, SLOT(hs_hall_effect_value_changed(int) ));

    connect(ui->cb_bps			, SIGNAL(activated(int) )	,this, SLOT(cB_Beat_per_sec(int) ));
    connect(ui->pB_Wavedisplay 	, SIGNAL(clicked() )		,this, SLOT(pB_Wavedisplay_clicked() ));
    connect(ui->pB_wd_mode		, SIGNAL(clicked() )		,this, SLOT(pB_Debug_clicked() ));
    connect(ui->pB_oscgroup		, SIGNAL(clicked() )		,this, SLOT(pB_oscgroup_clicked() ));
    connect(ui->pb_fftmode		, SIGNAL(clicked() )		,this, SLOT(pB_fftmode_clicked() ));

    connect(ui->Slider_mix_vol0	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix0(int) ));
    connect(ui->Slider_mix_vol1	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix1(int) ));
    connect(ui->Slider_mix_vol2	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix2(int) ));
    connect(ui->Slider_mix_vol3	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix3(int) ));
    connect(ui->Slider_mix_vol4	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix4(int) ));
    connect(ui->Slider_mix_vol5	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix5(int) ));
    connect(ui->Slider_mix_vol6	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix6(int) ));
    connect(ui->Slider_mix_vol7	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix7(int) ));

    connect(ui->rb_sta0			, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta0() ));
    connect(ui->rb_sta1			, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta1() ));
    connect(ui->rb_sta2			, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta2() ));
    connect(ui->rb_sta3			, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta3() ));
    connect(ui->rb_sta4			, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta4() ));
    connect(ui->rb_sta5			, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta5() ));
    connect(ui->rb_sta6			, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta6() ));
    connect(ui->rb_sta7			, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta7() ));

    connect(ui->cb_sta0			, SIGNAL(clicked() )		,this, SLOT(toggle_mute0() ));
    connect(ui->cb_sta1			, SIGNAL(clicked() )		,this, SLOT(toggle_mute1() ));
    connect(ui->cb_sta2			, SIGNAL(clicked() )		,this, SLOT(toggle_mute2() ));
    connect(ui->cb_sta3			, SIGNAL(clicked() )		,this, SLOT(toggle_mute3() ));
    connect(ui->cb_sta4			, SIGNAL(clicked() )		,this, SLOT(toggle_mute4() ));
    connect(ui->cb_sta5			, SIGNAL(clicked() )		,this, SLOT(toggle_mute5() ));
    connect(ui->cb_sta6			, SIGNAL(clicked() )		,this, SLOT(toggle_mute6() ));
    connect(ui->cb_sta7			, SIGNAL(clicked() )		,this, SLOT(toggle_mute7() ));

    connect(ui->rb_S0			, SIGNAL(clicked() )		,this, SLOT(select_Sds0() ));
    connect(ui->rb_S1			, SIGNAL(clicked() )		,this, SLOT(select_Sds1() ));
    connect(ui->rb_S2			, SIGNAL(clicked() )		,this, SLOT(select_Sds2() ));
    connect(ui->rb_S3			, SIGNAL(clicked() )		,this, SLOT(select_Sds3() ));

    connect(ui->cB_Combine		, SIGNAL(clicked() )		,this, SLOT(CombineFreq() ));

    connect(ui->sB_FMO			, SIGNAL(valueChanged(int) ),this, SLOT(FMO_Waveform_slot(int) )) ;
    connect(ui->sB_VCO			, SIGNAL(valueChanged(int) ),this, SLOT(VCO_Waveform_slot(int) )) ;
    connect(ui->sB_OSC			, SIGNAL(valueChanged(int) ),this, SLOT(Main_Waveform_slot(int) )) ;

    connect(ui->SliderFMOadjust	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_FMO_Adjust(int) ));
    connect(ui->SliderVCOadjust	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_VCO_Adjust(int) ));

    connect(ui->cB_Capture		, SIGNAL(activated(QString) ),this, SLOT(cB_Capture(QString) ));
    connect(ui->cb_external		, SIGNAL(activated(QString) ),this, SLOT(wavfile_selected(QString) ));
}

void MainWindow::initTimer()
{
    connect(status_timer, &QTimer::timeout, this, &MainWindow::updateWidgets);
    connect(osc_timer, &QTimer::timeout, this, &MainWindow::read_polygon_data );
    status_timer->start(1000); 	// update widgets
    osc_timer->start(250); 		// update oscilloscope widget 4 Hz
}
