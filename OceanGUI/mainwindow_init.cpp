/**************************************************************************
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
 * mainwidow_init.cpp
 *
 *  Created on: Feb 12, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


// Ocean
#include <Oscwaveform.h>

// OceanGUI
#include <Mainwindow.h>




/**************************************************
 * MainWindow
 *************************************************/
template <typename T, size_t N>
vector<T> arrayToVector(const array<T, N>& arr)
{
    return vector<T>(arr.begin(), arr.end());
}

void MainWindow::initWfDisplay_que()
{
	Qwd_wdmode_names		= Vstringvector	( wavedisplay_struct().button_types );
	Qwd_osc_names 			= Vstringvector	( typeNames );
//	Qwd_role_names 			= Vstringvector	( StaRole_map.StANames );
//	Qwd_role_names.push_back( "Audio" );

	for( interface_t* sds_p : DaTA_p->SDS.vec )
	{
		WfDisplay_que.Init( sds_p );
	}

}

void MainWindow::initUiVectors()
{
    rb_S_vec =
    {
    	ui->rb_S0,
		ui->rb_S1,
		ui->rb_S2,
		ui->rb_S3
    };
}
void MainWindow::initPanel()
{
    // https://stackoverflow.com/questions/17095957/qt-creator-and-main-window-background-image
    QString Ocean_png = Qstring( fs->bkground_file );
    QPixmap bkgnd( Ocean_png );
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);

    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Button, QColor( defaultButtonRGB ) );
    this->setPalette(palette);

}

void MainWindow::initLables()
{
	ui->cb_ssta0->setText( Qstring( StaRole_map.GetName( STA_USER00 )));
	ui->cb_ssta1->setText( Qstring( StaRole_map.GetName( STA_USER01 )));
	ui->cb_ssta2->setText( Qstring( StaRole_map.GetName( STA_USER02 )));
	ui->cb_ssta3->setText( Qstring( StaRole_map.GetName( STA_USER03 )));
	ui->cb_ssta4->setText( Qstring( StaRole_map.GetName( STA_INSTRUMENT )));
	ui->cb_ssta5->setText( Qstring( StaRole_map.GetName( STA_KEYBOARD )));
	ui->cb_ssta6->setText( Qstring( StaRole_map.GetName( STA_NOTES )));
	ui->cb_ssta7->setText( Qstring( StaRole_map.GetName( STA_EXTERNAL )));
}
void MainWindow::initComboBoxes()
{
	EventWAV = direvent_struct( DIRWAV_ID, ui->cB_external, READ_WAVFILE_KEY		,
				&sds_master->WAV_filename, fs->musicdir		, fs->wav_type );
    EventWAV.Update_cb();

	Qbps_str_lst 				= Qstringlist( Bps.Bps_lst );
    ui->CB_kbd_bps->addItems	( Qbps_str_lst );
    ui->CB_inst_bps->addItems	( Qbps_str_lst );
}

void MainWindow::initDataDisplay()
{
    						scene	= new QGraphicsScene( this );
    Data_t					scale	= 1.0 / ( max_data_amp * 2.0 );
    datagraphics_param_t 	param	{ scale, true, true };
    					DataDisplay	= new DataGraphic_class( ifb.sds_p, ui->oscilloscope_view, param );
    ui->oscilloscope_view->setScene	( scene );
    scene->addItem					( DataDisplay );
}

void MainWindow::set_pb_wd_mode_text( WdModeID_t mode )
{
	if( mode < Qwd_wdmode_names.size() )
	{
		QString txt = Qwd_wdmode_names[ mode ];
		ui->pB_wd_mode->setText( txt );
	}
};

void MainWindow::initStateButtons()
{

	ui->pB_oscgroup->setText( Qwd_osc_names[ Sds->addr->WD_state.oscId ] );

	set_pb_wd_mode_text( sds_p->WD_state.mode );

	StAId_e staid = StaRole_map.GetStaid( sds_p->WD_state.roleId );
	QString sta_name = "";
	if ( staid < WD_ROLES_SIZE )
		sta_name= Qwd_role_names[ staid ];
	ui->pB_WdRole->setText( sta_name );

    set_StateButton( ui->pB_Rtsp, DISABLED, QString( "" ), QString( "" ) );
    set_StateButton( ui->pB_play_notes, DISABLED, QString( "" ), QString( "" ) );
    set_StateButton( ui->pB_Spectrum, DISABLED, QString( "" ),QString( "" ) );
    set_StateButton( ui->pBGuiExit, ENABLED, QString( "" ),QString( "" ) );
}

void MainWindow::initFreqSlider()
{
	for( sl_lcd_t map : sl_freq_vec )
	{
		map.sl->setMinimum( frqarr_range.min );
		map.sl->setMaximum( frqarr_range.max );
		int frqidx = *map.value;
		map.sl->setValue( GetFrq( frqidx ));
	}
	ui->cB_Combine_fmo->setChecked( sds_p->features[FMOID].combine );
	ui->cB_Combine_vco->setChecked( sds_p->features[VCOID].combine );

	ui->Slider_slideFrq->setMaximum( Sds->frq_slide.max_steps );
	ui->Slider_slideFrq->setSingleStep( 1 );
	ui->Slider_slideFrq->setPageStep( Sds->frq_slide.page_steps );
}

void MainWindow::initGuiVectors()
{
	cb_sta_store_vec =
    {
    	{ ui->cb_ssta0, &sds_p->StA_state_arr[STA_USER00] },
		{ ui->cb_ssta1, &sds_p->StA_state_arr[STA_USER01] },
		{ ui->cb_ssta2, &sds_p->StA_state_arr[STA_USER02] },
		{ ui->cb_ssta3, &sds_p->StA_state_arr[STA_USER03] },
		{ ui->cb_ssta4, &sds_p->StA_state_arr[STA_INSTRUMENT] },
		{ ui->cb_ssta5, &sds_p->StA_state_arr[STA_KEYBOARD] },
		{ ui->cb_ssta6, &sds_p->StA_state_arr[STA_NOTES] },
		{ ui->cb_ssta7, &sds_p->StA_state_arr[STA_EXTERNAL] }
    };

    cb_sta_store_vec[STA_NOTES].cb->setCheckable( false );
    cb_sta_store_vec[STA_KEYBOARD].cb->setCheckable( false );

    cb_sta_filled_vec =
    {
    	{ ui->cb_fsta0, &sds_p->StA_state_arr[STA_USER00] },
		{ ui->cb_fsta1, &sds_p->StA_state_arr[STA_USER01] },
		{ ui->cb_fsta2, &sds_p->StA_state_arr[STA_USER02] },
		{ ui->cb_fsta3, &sds_p->StA_state_arr[STA_USER03] },
		{ ui->cb_fsta4, &sds_p->StA_state_arr[STA_INSTRUMENT] },
		{ ui->cb_fsta5, &sds_p->StA_state_arr[STA_KEYBOARD] },
		{ ui->cb_fsta6, &sds_p->StA_state_arr[STA_NOTES] },
		{ ui->cb_fsta7, &sds_p->StA_state_arr[STA_EXTERNAL] }
    };

    tb_sta_play_vec =
    {
		{ ui->tB_play_0, &sds_p->StA_state_arr[STA_USER00]  },
		{ ui->tB_play_1, &sds_p->StA_state_arr[STA_USER01]  },
		{ ui->tB_play_2, &sds_p->StA_state_arr[STA_USER02]  },
		{ ui->tB_play_3, &sds_p->StA_state_arr[STA_USER03]  },
		{ ui->tB_play_4, &sds_p->StA_state_arr[STA_INSTRUMENT]  },
		{ ui->tB_play_5, &sds_p->StA_state_arr[STA_KEYBOARD]  },
		{ ui->tB_play_6, &sds_p->StA_state_arr[STA_NOTES]  },
		{ ui->tB_play_7, &sds_p->StA_state_arr[STA_EXTERNAL]  }
	};
    sl_sta_vec =
    {
		{ ui->Slider_mix_vol0, &sds_p->StA_amp_arr[STA_USER00]  },
		{ ui->Slider_mix_vol1, &sds_p->StA_amp_arr[STA_USER01]  },
		{ ui->Slider_mix_vol2, &sds_p->StA_amp_arr[STA_USER02]  },
		{ ui->Slider_mix_vol3, &sds_p->StA_amp_arr[STA_USER03]  },
		{ ui->Slider_mix_vol4, &sds_p->StA_amp_arr[STA_INSTRUMENT]  },
		{ ui->Slider_mix_vol5, &sds_p->StA_amp_arr[STA_KEYBOARD]  },
		{ ui->Slider_mix_vol6, &sds_p->StA_amp_arr[STA_NOTES]  },
		{ ui->Slider_mix_vol7, &sds_p->StA_amp_arr[STA_EXTERNAL]  }
	};

    sl_freq_vec =
    {
   		{ FREQUENCYKEY, ui->VCOLCD_Hz, ui->Slider_VCO_Hz, &sds_p->spectrum_arr[VCOID].frqidx[0] },
   		{ FREQUENCYKEY, ui->FMOLCD_Hz, ui->Slider_FMO_Hz, &sds_p->spectrum_arr[FMOID].frqidx[0] },
   		{ FREQUENCYKEY, ui->OSCLCD_Hz, ui->Slider_OSC_Hz, &sds_p->spectrum_arr[OSCID].frqidx[0] }
    };
    sl_volume_vec =
    {
   		{ VCOAMPKEY, 	ui->VCOLCD_Amp, ui->Slider_VCO_vol, &sds_p->spectrum_arr[VCOID].volidx[0] },
   		{ FMOAMPKEY, 	ui->FMOLCD_Amp, ui->Slider_FMO_vol, &sds_p->spectrum_arr[FMOID].volidx[0] },
   		{ MASTERAMP_KEY,ui->OSCLCD_Amp, ui->Slider_OSC_Vol, &sds_master->Master_Amp }
    };
    wf_vec =
    {
        { WAVEFORMKEY, ui->pB_wfVCO, &sds_p->spectrum_arr[VCOID].wfid[0] },
       	{ WAVEFORMKEY, ui->pB_wfFMO, &sds_p->spectrum_arr[FMOID].wfid[0] },
       	{ WAVEFORMKEY, ui->pB_wfOSC, &sds_p->spectrum_arr[OSCID].wfid[0] }
    };
}
void MainWindow::updateGuiVectors( interface_t* sds)
{
	for( StAId_e staid : StAIds )
	{
		cb_sta_store_vec[staid].state	= &sds->StA_state_arr[staid];
		cb_sta_filled_vec[staid].state	= &sds->StA_state_arr[staid];
		tb_sta_play_vec[staid].state	= &sds->StA_state_arr[staid];
	    sl_sta_vec[staid].value			= &sds->StA_amp_arr[staid];
	}

    for( OSCID_e oscid : oscIds )
    {
    	sl_freq_vec[oscid].value		= &sds->spectrum_arr[oscid].frqidx[0];
		sl_volume_vec[oscid].value		= &sds->spectrum_arr[oscid].volidx[0];
		wf_vec[oscid].value				= &sds->spectrum_arr[oscid].wfid[0];
    }
	sl_volume_vec[OSCID].value	= &sds_master->Master_Amp;
	set_wdrole( sds_p->WD_state.roleId, true );

}


void MainWindow::initPushbuttons()
{
	auto initEventFilter	= [ this ]( QPushButton* ui_pb )
	{
	    Pushbutton_class* 	pb = new Pushbutton_class( ui_pb );
	    pb->hide();
	    return pb;
	};

	Pushbutton_class*
	ui_pb = initEventFilter( ui->pB_WdRole );
    connect( ui_pb , SIGNAL( clickdirection(int) ),this, SLOT(pB_Role_clicked(int) ) );
	ui_pb = initEventFilter( Spectrum_Dialog_p->ui->pB_fourier_input );
    connect( ui_pb , SIGNAL( clickdirection(int) ),this, SLOT(pB_Role_clicked(int) ) );
	ui_pb = initEventFilter( ui->pB_oscgroup );
    connect( ui_pb , SIGNAL( clickdirection(int) ),this, SLOT(pB_OSC_clicked( int) ) );
	ui_pb = initEventFilter( ui->pB_wd_mode );
    connect( ui_pb , SIGNAL( clickdirection(int) ),this, SLOT(pB_Mode_clicked( int) ) );

	wf_pb_vec.clear();
    for( QPushButton* ui_pb : { ui->pB_wfVCO, ui->pB_wfFMO, ui->pB_wfOSC } )
    {
    	Pushbutton_class*	pb	= initEventFilter( ui_pb );
    	wf_pb_vec.push_back	( pb );
    }

}

void MainWindow::initSignals()
{
    connect(ui->pB_Spectrum		, SIGNAL(clicked() )		,this, SLOT(Spectrum_Dialog() ));
    connect(ui->pB_ADSR			, SIGNAL(clicked() )		,this, SLOT(ADSR_Dialog() ));
    connect(ui->pB_Rtsp			, SIGNAL(clicked() )		,Rtsp_Dialog_p, SLOT(Dialog() ));
    connect(ui->pB_play_notes	, SIGNAL(clicked() )		,File_Dialog_p, SLOT(Dialog() ));
    connect(ui->pB_Cutter		, SIGNAL(clicked() )		,CutDesk_Dialog_p, SLOT(Dialog() ));

    connect(ui->pb_SDSview		, SIGNAL(clicked() )		,this, SLOT(start_sdsview() 	));
    connect(ui->pBSynthesizer	, SIGNAL(clicked() )		,this, SLOT(start_synthesizer() ));
    connect(ui->pb_Keyboard		, SIGNAL(clicked() )		,this, SLOT(start_keyboard() 	));
    connect(ui->pBAudioServer	, SIGNAL(clicked() )		,this, SLOT(start_audio_srv() 	));
    connect(ui->pBComposer		, SIGNAL(clicked() )		,this, SLOT(start_composer() 	));
    connect(ui->pBGuiExit		, SIGNAL(clicked() )		,this, SLOT(GUI_Exit() 			));
    connect(ui->pBtoggleRecord	, SIGNAL(clicked() )		,this, SLOT(SaveRecord() 		));
    connect(ui->pB_Capture		, SIGNAL(clicked() )		,this, SLOT(capture_audio() 	));
    connect(ui->pB_Mute			, SIGNAL(clicked() )		,this, SLOT(toggle_Mute() 		));
    connect(ui->pB_Mute_StA		, SIGNAL(clicked() )		,this, SLOT(clear_StAs_play() 	));
    connect(ui->pB_Save			, SIGNAL(clicked() )		,this, SLOT(Save_Config() 		));
    connect(ui->pb_clear		, SIGNAL(clicked() )		,this, SLOT(memory_clear() 		));

    connect(ui->Slider_VCO_Hz	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_VCO_Freq(int) ));
    connect(ui->Slider_FMO_Hz	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_FMO_Freq(int) ));
    connect(ui->Slider_OSC_Hz	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_OSC_Freq(int) ));

    connect(ui->Slider_VCO_vol	, SIGNAL(valueChanged(int) ),this, SLOT(VCO_slot_volume() ));
    connect(ui->Slider_FMO_vol	, SIGNAL(valueChanged(int) ),this, SLOT(FMO_slot_volume() ));
    connect(ui->Slider_OSC_Vol	, SIGNAL(valueChanged(int) ),this, SLOT(Main_slot_volume() ));

    connect(ui->Slider_slideFrq	, SIGNAL(valueChanged(int) ),this, SLOT(slideFrq(int )) );
    connect(ui->hs_pmw      	, SIGNAL(valueChanged(int) ),this, SLOT(dial_PMW_value_changed() ));
    connect(ui->Slider_slideVol	, SIGNAL(valueChanged(int) ),this, SLOT(slideVol(int)) );

    connect(ui->hs_hall_effect	, SIGNAL(valueChanged(int) ),this, SLOT(adsr_hall() ));
    connect(ui->hs_balance		, SIGNAL(valueChanged(int) ),this, SLOT(mixer_balance() ));
    connect(ui->hs_chord_delay	, SIGNAL(valueChanged(int) ),this, SLOT(chord_delay() ));

    connect(ui->CB_kbd_bps		, SIGNAL(activated(int) )	,this, SLOT(Notes_per_measure(int) ));
    connect(ui->CB_inst_bps		, SIGNAL(activated(int) )	,this, SLOT(Beat_per_sec(int) ));

    connect(ui->Slider_mix_vol0	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix0(int) ));
    connect(ui->Slider_mix_vol1	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix1(int) ));
    connect(ui->Slider_mix_vol2	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix2(int) ));
    connect(ui->Slider_mix_vol3	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix3(int) ));
    connect(ui->Slider_mix_vol4	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix4(int) ));
    connect(ui->Slider_mix_vol5	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix5(int) ));
    connect(ui->Slider_mix_vol6	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix6(int) ));
    connect(ui->Slider_mix_vol7	, SIGNAL(valueChanged(int) ),this, SLOT(Sl_mix7(int) ));

    connect(ui->cb_ssta0		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta0() ));
    connect(ui->cb_ssta1		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta1() ));
    connect(ui->cb_ssta2		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta2() ));
    connect(ui->cb_ssta3		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta3() ));
    connect(ui->cb_ssta4		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta4() ));
    connect(ui->cb_ssta5		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta5() ));
    connect(ui->cb_ssta6		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta6() ));
    connect(ui->cb_ssta7		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta7() ));

    connect(ui->tB_play_0		, SIGNAL(clicked() )		,this, SLOT(setStaPlay0() ));
    connect(ui->tB_play_1		, SIGNAL(clicked() )		,this, SLOT(setStaPlay1() ));
    connect(ui->tB_play_2		, SIGNAL(clicked() )		,this, SLOT(setStaPlay2() ));
    connect(ui->tB_play_3		, SIGNAL(clicked() )		,this, SLOT(setStaPlay3() ));
    connect(ui->tB_play_4		, SIGNAL(clicked() )		,this, SLOT(setStaPlay4() ));
    connect(ui->tB_play_5		, SIGNAL(clicked() )		,this, SLOT(setStaPlay5() ));
    connect(ui->tB_play_6		, SIGNAL(clicked() )		,this, SLOT(setStaPlay6() ));
    connect(ui->tB_play_7		, SIGNAL(clicked() )		,this, SLOT(setStaPlay7() ));

    connect(ui->cb_fsta0		, SIGNAL(clicked() )		,this, SLOT(setStaStored0() ));
    connect(ui->cb_fsta1		, SIGNAL(clicked() )		,this, SLOT(setStaStored1() ));
    connect(ui->cb_fsta2		, SIGNAL(clicked() )		,this, SLOT(setStaStored2() ));
    connect(ui->cb_fsta3		, SIGNAL(clicked() )		,this, SLOT(setStaStored3() ));
    connect(ui->cb_fsta4		, SIGNAL(clicked() )		,this, SLOT(setStaStored4() ));
    connect(ui->cb_fsta5		, SIGNAL(clicked() )		,this, SLOT(setStaStored5() ));
    connect(ui->cb_fsta6		, SIGNAL(clicked() )		,this, SLOT(setStaStored6() ));
    connect(ui->cb_fsta7		, SIGNAL(clicked() )		,this, SLOT(setStaStored7() ));

    connect(ui->rb_S0			, SIGNAL(clicked() )		,this, SLOT(select_Sds0() ));
    connect(ui->rb_S1			, SIGNAL(clicked() )		,this, SLOT(select_Sds1() ));
    connect(ui->rb_S2			, SIGNAL(clicked() )		,this, SLOT(select_Sds2() ));
    connect(ui->rb_S3			, SIGNAL(clicked() )		,this, SLOT(select_Sds3() ));
   	rb_S_vec[ sds_master->comId ]->setChecked( true );

    connect( Rtsp_Dialog_p->ui->rB_1	, SIGNAL( clicked() )	,this, SLOT( select_Sds0()));
    connect( Rtsp_Dialog_p->ui->rB_2	, SIGNAL( clicked() )	,this, SLOT( select_Sds1()));
    connect( Rtsp_Dialog_p->ui->rB_3	, SIGNAL( clicked() )	,this, SLOT( select_Sds2()));
    connect( Rtsp_Dialog_p->ui->rB_4	, SIGNAL( clicked() )	,this, SLOT( select_Sds3()));

    connect(ui->cB_Combine_fmo	, SIGNAL(clicked() )		,this, SLOT(CombineFreq_fmo() ));
    connect(ui->cB_Combine_vco	, SIGNAL(clicked() )		,this, SLOT(CombineFreq_vco() ));

    connect(wf_pb_vec[FMOID]	, SIGNAL(clickdirection(int) ),this, SLOT(FMO_Waveform_slot(int) )) ;
    connect(wf_pb_vec[VCOID]	, SIGNAL(clickdirection(int) ),this, SLOT(VCO_Waveform_slot(int) )) ;
    connect(wf_pb_vec[OSCID]	, SIGNAL(clickdirection(int) ),this, SLOT(Main_Waveform_slot(int) )) ;

    connect(ui->SliderFMOadjust	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_FMO_Adjust(int) ));
    connect(ui->SliderVCOadjust	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_VCO_Adjust(int) ));

    connect(ui->cB_external		, SIGNAL(textActivated(QString) ),this, SLOT(wavfile_selected(QString) ));

    connect(ui->cb_connect_fmov	, SIGNAL(clicked( bool ))	,this, SLOT( connect_fmov( bool ) ));
    connect(ui->cb_connect_vcov	, SIGNAL(clicked( bool ))	,this, SLOT( connect_vcov( bool ) ));
    connect(ui->cb_connect_oscf	, SIGNAL(clicked( bool ))	,this, SLOT( connect_oscf( bool ) ));
    connect(ui->cb_connect_oscv	, SIGNAL(clicked( bool ))	,this, SLOT( connect_oscv( bool ) ));

    connect( File_Dialog_p->ui->cb_score, SIGNAL(textActivated(QString)),
    		this, SLOT( Update_Statusbar( QString ) ));

    connect( ui->cb_overwrite	, SIGNAL(clicked( bool ))	,
    		this, SLOT( set_overwritemode( bool ) ));
    connect( Keyboard_Dialog_p	, SIGNAL( toggleKbdPlay( StAId_e )),
    	this					, SLOT( remote_sta_off( StAId_e ) ));
    connect( ui->tB_logState	, SIGNAL( clicked()), this, SLOT( tb_logState_slot() ) );
}

void MainWindow::initTimer()
{
    connect( uiUpdate_timer		, &QTimer::timeout, this, &MainWindow::setwidgetvalues );
    uiUpdate_timer->start		(1000); 	// update widgets
    dataGraphic_timer->start	( min_msec*2 ); 	// update oscilloscope widget 4 Hz
}

void MainWindow::initOther()
{
    for( cb_state_t map : cb_sta_store_vec )
    {
    	map.cb->setChecked( map.state->store );
    }
	Update_Statusbar( Qstring( sds_master->Session ) );
	ui->cb_overwrite->setChecked( sds_master->overwritemode );

}
