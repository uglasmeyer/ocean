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
 * mainwidow_init.cpp
 *
 *  Created on: Feb 12, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <include/Mainwindow.h>

void MainWindow::initPanel()
{
    // https://stackoverflow.com/questions/17095957/qt-creator-and-main-window-background-image
    QString Ocean_png = Qstring( fs->bkground_file );
    QPixmap bkgnd( Ocean_png );
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);

    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Button, QColor(0,179,255) );
    this->setPalette(palette);

    Spectrum_Dialog_Rect = Spectrum_Dialog_p->geometry();  //get current geometry of help window
    QRect parentRect = this->geometry();      //get current geometry of this window
//    QPoint oscview_TopLeft = QPoint( ui->oscilloscope_view->geometry().bottomLeft() );
    QPoint oscview_TopLeft = QPoint (0 ,0 );
    Spectrum_Dialog_Rect= QRect( oscview_TopLeft, Spectrum_Dialog_p->geometry().size() );
    //get current geometry of this window
    //    rect.moveTo(mapToGlobal(QPoint(parentRect.x() + parentRect.width() - rect.width(), parentRect.y())));
//    rect.moveTo(mapToGlobal( Spectrum_Dialog_TopLeft ));

    QRect rect = File_Dialog_p->geometry();  //get current geometry of help window
    parentRect = this->geometry();      //get current geometry of this window
    rect.moveTo(mapToGlobal(QPoint(parentRect.x() + parentRect.width() - rect.width(), parentRect.y())));
    File_Dialog_p->setGeometry(rect);
}

void MainWindow::initLables()
{
	ui->cb_ssta0->setText( Qstring( StAIdName( STA_USER00 )));
	ui->cb_ssta1->setText( Qstring( StAIdName( STA_USER01 )));
	ui->cb_ssta2->setText( Qstring( StAIdName( STA_USER02 )));
	ui->cb_ssta3->setText( Qstring( StAIdName( STA_USER03 )));
	ui->cb_ssta4->setText( Qstring( StAIdName( STA_INSTRUMENT )));
	ui->cb_ssta5->setText( Qstring( StAIdName( STA_KEYBOARD )));
	ui->cb_ssta6->setText( Qstring( StAIdName( STA_NOTES )));
	ui->cb_ssta7->setText( Qstring( StAIdName( STA_EXTERNAL )));
}
void MainWindow::initComboBoxes()
{
    string wavfile_path = fs->musicdir;
    CB_external->clear();

    Path_t path { wavfile_path, fs->wav_type } ;
    CB_external->addItems( Qread_filenames(	path ) );
    QString Qfile	= QReadStr( Sds, OTHERSTR_KEY );
    CB_external->setCurrentText( Qfile );

	Qbps_str_lst 				= Qstringlist( Bps.Bps_lst );
    ui->CB_kbd_bps->addItems	( Qbps_str_lst );
    ui->CB_inst_bps->addItems	( Qbps_str_lst );


}

void MainWindow::initOscillatorDisplay()
{
    QRectF 				rect         = ui->oscilloscope_view->geometry();
    OszilloscopeWidget	OscWidg( Sds->addr, rect );
    OscWidget_item 		= new OszilloscopeWidget( Sds->addr, rect ) ;

    ui->oscilloscope_view->setScene( scene );
    scene->addItem( OscWidget_item );
}

template <typename T, std::size_t N>
std::vector<T> arrayToVector(const std::array<T, N>& arr)
{
    return std::vector<T>(arr.begin(), arr.end());
}

void MainWindow::initStateButtons()

{
	Qwd_osc_names 	= Vstringvector( typeNames );
	ui->pB_oscgroup->setText( Qwd_osc_names[ Sds->addr->WD_state.oscId ]);

	Qwd_fftmodes	= Vstringvector( fftmodes );
	ui->pb_fftmode->setText( Qwd_fftmodes [ Sds->addr->WD_state.fftmode ]);

	Qwd_wdmode_names= Vstringvector( wavedisplay_struct::types );
	int wd_mode = check_range( wd_mode_range, (int)sds->WD_state.wd_mode );
	ui->pB_wd_mode->setText( Qwd_wdmode_names[ wd_mode ] );


	Qwd_role_names 	= Vstringvector( arrayToVector( roleNames ) );
	ui->pB_Wavedisplay->setText( Qwd_role_names[ Sds->addr->WD_state.roleId ]);

    setButton( ui->pB_Rtsp, 2 );
    setButton( ui->pB_play_notes, 2 );
    setButton( ui->pB_Specrum, 2 );
    setButton( ui->pBGuiExit, 3 );




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
		map.sl->setMaximum( map.max );
		int frqidx = *map.value;
		map.sl->setValue( Spectrum.GetFrq( frqidx ));
	}
	if ( Sds->addr->frq_slidermode == COMBINE )
		ui->cB_Combine->setChecked( true );
}

void MainWindow::initGuiVectors( interface_t* sds)
{

    rb_S_vec =
    {
    	ui->rb_S0,
		ui->rb_S1,
		ui->rb_S2,
		ui->rb_S3
    };

    cb_store_sta_vec =
    {
    	{ STA_USER00, ui->cb_ssta0, &sds->StA_state_arr[STA_USER00].store },
		{ STA_USER01, ui->cb_ssta1, &sds->StA_state_arr[STA_USER01].store },
		{ STA_USER02, ui->cb_ssta2, &sds->StA_state_arr[STA_USER02].store },
		{ STA_USER03, ui->cb_ssta3, &sds->StA_state_arr[STA_USER03].store },
		{ STA_INSTRUMENT, ui->cb_ssta4, &sds->StA_state_arr[STA_INSTRUMENT].store },
		{ STA_KEYBOARD, ui->cb_ssta5, &sds->StA_state_arr[STA_KEYBOARD].store },
		{ STA_NOTES, ui->cb_ssta6, &sds->StA_state_arr[STA_NOTES].store },
		{ STA_EXTERNAL, ui->cb_ssta7, &sds->StA_state_arr[STA_EXTERNAL].store }
    };
    cb_filled_sta_vec =
    {
    	{ STA_USER00, ui->cb_fsta0, &sds->StA_state_arr[STA_USER00].filled },
		{ STA_USER01, ui->cb_fsta1, &sds->StA_state_arr[STA_USER01].filled },
		{ STA_USER02, ui->cb_fsta2, &sds->StA_state_arr[STA_USER02].filled },
		{ STA_USER03, ui->cb_fsta3, &sds->StA_state_arr[STA_USER03].filled },
		{ STA_INSTRUMENT, ui->cb_fsta4, &sds->StA_state_arr[STA_INSTRUMENT].filled },
		{ STA_KEYBOARD, ui->cb_fsta5, &sds->StA_state_arr[STA_KEYBOARD].filled },
		{ STA_NOTES, ui->cb_fsta6, &sds->StA_state_arr[STA_NOTES].filled },
		{ STA_EXTERNAL, ui->cb_fsta7, &sds->StA_state_arr[STA_EXTERNAL].filled }
    };

    cb_play_sta_vec =
    {
		{ STA_USER00, ui->cb_psta0, &sds->StA_state_arr[STA_USER00].play  },
		{ STA_USER01, ui->cb_psta1, &sds->StA_state_arr[STA_USER01].play  },
		{ STA_USER02, ui->cb_psta2, &sds->StA_state_arr[STA_USER02].play  },
		{ STA_USER03, ui->cb_psta3, &sds->StA_state_arr[STA_USER03].play  },
		{ STA_INSTRUMENT, ui->cb_psta4, &sds->StA_state_arr[STA_INSTRUMENT].play  },
		{ STA_KEYBOARD, ui->cb_psta5, &sds->StA_state_arr[STA_KEYBOARD].play  },
		{ STA_NOTES, ui->cb_psta6, &sds->StA_state_arr[STA_NOTES].play  },
		{ STA_EXTERNAL, ui->cb_psta7, &sds->StA_state_arr[STA_EXTERNAL].play  }
	};
    sl_sta_vec =
    {
		{ STA_USER00, ui->Slider_mix_vol0, &sds->StA_amp_arr[STA_USER00]  },
		{ STA_USER01, ui->Slider_mix_vol1, &sds->StA_amp_arr[STA_USER01]  },
		{ STA_USER02, ui->Slider_mix_vol2, &sds->StA_amp_arr[STA_USER02]  },
		{ STA_USER03, ui->Slider_mix_vol3, &sds->StA_amp_arr[STA_USER03]  },
		{ STA_INSTRUMENT, ui->Slider_mix_vol4, &sds->StA_amp_arr[STA_INSTRUMENT]  },
		{ STA_KEYBOARD, ui->Slider_mix_vol5, &sds->StA_amp_arr[STA_KEYBOARD]  },
		{ STA_NOTES, ui->Slider_mix_vol6, &sds->StA_amp_arr[STA_NOTES]  },
		{ STA_EXTERNAL, ui->Slider_mix_vol7, &sds->StA_amp_arr[STA_EXTERNAL]  }
	};

    int max = frqarr_range.max - 2*oct_steps;  // 2 refers to max keyboard octave
    sl_frqidx_vec =
    {
   		{ VCOFREQUENCYKEY, ui->VCOLCD_Hz, ui->Slider_VCO_Hz, &sds->spectrum_arr[VCOID].frqidx[0], max },
   		{ FMOFREQUENCYKEY, ui->FMOLCD_Hz, ui->Slider_FMO_Hz, &sds->spectrum_arr[FMOID].frqidx[0], max },
   		{ OSCFREQUENCYKEY, ui->OSCLCD_Hz, ui->Slider_OSC_Hz, &sds->spectrum_arr[OSCID].frqidx[0], frqarr_range.max   }
    };
    sl_volume_vec =
    {
   		{ VCOAMPKEY, 	ui->VCOLCD_Amp, ui->Slider_VCO_vol, &sds->spectrum_arr[VCOID].volidx[0], volidx_range.max },
   		{ FMOAMPKEY, 	ui->FMOLCD_Amp, ui->Slider_FMO_vol, &sds->spectrum_arr[FMOID].volidx[0], volidx_range.max },
   		{ MASTERAMP_KEY,ui->OSCLCD_Amp, ui->Slider_OSC_Vol, &sds_master->Master_Amp	 , volidx_range.max }
    };
    sB_lbl_vec =
    {
        { VCOFREQUENCYKEY, ui->sB_VCO, ui->wf_vco, &sds->spectrum_arr[VCOID].wfid[0] },
       	{ FMOFREQUENCYKEY, ui->sB_FMO, ui->wf_fmo, &sds->spectrum_arr[FMOID].wfid[0] },
       	{ OSCFREQUENCYKEY, ui->sB_OSC, ui->wf_OSC, &sds->spectrum_arr[OSCID].wfid[0] }
    };
}

void MainWindow::initUiConnectors()
{
    connect(ui->pB_Rtsp			, SIGNAL(clicked() )		,this, SLOT(Rtsp_Dialog() ));
    connect(ui->pb_SDSview		, SIGNAL(clicked() )		,this, SLOT(SDS_Dialog() ));
    connect(ui->pB_Specrum		, SIGNAL(clicked() )		,this, SLOT(Spectrum_Dialog() ));
    connect(ui->pB_play_notes	, SIGNAL(clicked() )		,this, SLOT(File_Director() ));
    connect(ui->pB_ADSR			, SIGNAL(clicked() )		,this, SLOT(ADSR_Dialog() ));
    connect(ui->pB_Cutter		, SIGNAL(clicked() )		,this, SLOT(Cutter_Dialog() ));

    connect(ui->pBSynthesizer	, SIGNAL(clicked() )		,this, SLOT(start_synthesizer() ));
    connect(ui->pb_Keyboard		, SIGNAL(clicked() )		,this, SLOT(start_keyboard() ));
    connect(ui->pBAudioServer	, SIGNAL(clicked() )		,this, SLOT(start_audio_srv() ));
    connect(ui->pBComposer		, SIGNAL(clicked() )		,this, SLOT(start_composer() ));
    connect(ui->pBGuiExit		, SIGNAL(clicked() )		,this, SLOT(GUI_Exit() ));
    connect(ui->pBtoggleRecord	, SIGNAL(clicked(bool) )	,this, SLOT(SaveRecord() ));
    connect(ui->pB_Mute			, SIGNAL(clicked() )		,this, SLOT(toggle_Mute() ));
    connect(ui->pB_Mute_StA		, SIGNAL(clicked() )		,this, SLOT(Clear_Banks() ));
    connect(ui->pB_Save			, SIGNAL(clicked() )		,this, SLOT(Save_Config() ));
    connect(ui->pb_clear		, SIGNAL(clicked() )		,this, SLOT(memory_clear() ));

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

    connect(ui->cb_ssta0		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta0() ));
    connect(ui->cb_ssta1		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta1() ));
    connect(ui->cb_ssta2		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta2() ));
    connect(ui->cb_ssta3		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta3() ));
    connect(ui->cb_ssta4		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta4() ));
    connect(ui->cb_ssta5		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta5() ));
    connect(ui->cb_ssta6		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta6() ));
    connect(ui->cb_ssta7		, SIGNAL(clicked() )		,this, SLOT(toggle_store_sta7() ));

    connect(ui->cb_psta0			, SIGNAL(clicked() )		,this, SLOT(setStaPlay0() ));
    connect(ui->cb_psta1			, SIGNAL(clicked() )		,this, SLOT(setStaPlay1() ));
    connect(ui->cb_psta2			, SIGNAL(clicked() )		,this, SLOT(setStaPlay2() ));
    connect(ui->cb_psta3			, SIGNAL(clicked() )		,this, SLOT(setStaPlay3() ));
    connect(ui->cb_psta4			, SIGNAL(clicked() )		,this, SLOT(setStaPlay4() ));
    connect(ui->cb_psta5			, SIGNAL(clicked() )		,this, SLOT(setStaPlay5() ));
    connect(ui->cb_psta6			, SIGNAL(clicked() )		,this, SLOT(setStaPlay6() ));
    connect(ui->cb_psta7			, SIGNAL(clicked() )		,this, SLOT(setStaPlay7() ));

    connect(ui->cb_fsta0			, SIGNAL(clicked() )		,this, SLOT(setStaStored0() ));
    connect(ui->cb_fsta1			, SIGNAL(clicked() )		,this, SLOT(setStaStored1() ));
    connect(ui->cb_fsta2			, SIGNAL(clicked() )		,this, SLOT(setStaStored2() ));
    connect(ui->cb_fsta3			, SIGNAL(clicked() )		,this, SLOT(setStaStored3() ));
    connect(ui->cb_fsta4			, SIGNAL(clicked() )		,this, SLOT(setStaStored4() ));
    connect(ui->cb_fsta5			, SIGNAL(clicked() )		,this, SLOT(setStaStored5() ));
    connect(ui->cb_fsta6			, SIGNAL(clicked() )		,this, SLOT(setStaStored6() ));
    connect(ui->cb_fsta7			, SIGNAL(clicked() )		,this, SLOT(setStaStored7() ));

    connect(ui->rb_S0			, SIGNAL(clicked() )		,this, SLOT(select_Sds0() ));
    connect(ui->rb_S1			, SIGNAL(clicked() )		,this, SLOT(select_Sds1() ));
    connect(ui->rb_S2			, SIGNAL(clicked() )		,this, SLOT(select_Sds2() ));
    connect(ui->rb_S3			, SIGNAL(clicked() )		,this, SLOT(select_Sds3() ));
   	rb_S_vec[ sds_master->config ]->setChecked( true );

    connect(ui->cB_Combine		, SIGNAL(clicked() )		,this, SLOT(CombineFreq() ));

    connect(ui->sB_FMO			, SIGNAL(valueChanged(int) ),this, SLOT(FMO_Waveform_slot(int) )) ;
    connect(ui->sB_VCO			, SIGNAL(valueChanged(int) ),this, SLOT(VCO_Waveform_slot(int) )) ;
    connect(ui->sB_OSC			, SIGNAL(valueChanged(int) ),this, SLOT(Main_Waveform_slot(int) )) ;

    connect(ui->SliderFMOadjust	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_FMO_Adjust(int) ));
    connect(ui->SliderVCOadjust	, SIGNAL(valueChanged(int) ),this, SLOT(Slider_VCO_Adjust(int) ));

    connect(ui->cB_external		, SIGNAL(textActivated(QString) ),this, SLOT(wavfile_selected(QString) ));

    connect(ui->cb_connect_fmov	, SIGNAL(clicked( bool ))	,this, SLOT( connect_fmov( bool ) ));
    connect(ui->cb_connect_vcov	, SIGNAL(clicked( bool ))	,this, SLOT( connect_vcov( bool ) ));
    connect(ui->cb_connect_oscf	, SIGNAL(clicked( bool ))	,this, SLOT( connect_oscf( bool ) ));
    connect(ui->cb_connect_oscv	, SIGNAL(clicked( bool ))	,this, SLOT( connect_oscv( bool ) ));

}

void MainWindow::initTimer()
{
    connect(status_timer, &QTimer::timeout, this, &MainWindow::setwidgetvalues );
    connect(osc_timer, &QTimer::timeout, this, &MainWindow::read_polygon_data );
    status_timer->start(1000); 	// update widgets
    osc_timer->start(250); 		// update oscilloscope widget 4 Hz
}
